/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include <QUuid>
#include <QRegExp>
#include <QHostInfo>

#include "molproff.h"
#include "molprosession.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace Squire;
using namespace SireBase;

QMutex MolproSession::starter_mutex;

/** Return the path to the null file (/dev/null on unix, NUL on windows) */
QString devNull()
{
    #ifdef Q_OS_UNIX
      return "/dev/null";
    #else
      #ifdef Q_OS_WIN32
        return "NUL";
      #else
        #error You need to supply the path to the null device on this platform!
        return "";
      #endif
    #endif
}

/** Construct a session that will represent the forcefield 'molproff' */
MolproSession::MolproSession(MolproFF &molproff)
              : boost::noncopyable(),
                molpro_exe( molproff.molproExe() ),
                ffid( molproff.ID() ),
                qm_version( molproff.qmVersion() )
{
    if (not molpro_exe.isExecutable())
        throw SireError::process_error( QObject::tr(
            "Cannot start a separate Molpro process as the molpro file \"%1\" "
            "is not executable, or does not exist!")
                .arg(molpro_exe.absoluteFilePath()), CODELOC );

    //only one molpro job may start at a time!
    QMutexLocker lkr(&starter_mutex);

    try
    {
        //create a new temporary directory for the molpro process
        int n = 1;

        do
        {
            #ifdef Q_OS_UNIX

            //construct a unique string that should equal
            // /USERNAME_molpro_PID_UUID_IDNUMBER
            QString unique_dir = QString("/%1_molpro_%2_%3_%4")
                                      .arg(getlogin()).arg(getpid())
                                      .arg(QUuid::createUuid().toString())
                                      .arg(n);

            #else

            //need to find the non-unix equivalent of the above... using
            // /molpro_UUID_IDNUMBER
            QString unique_dir = QString("/molpro_%1_%2")
                                      .arg(QUuid::createUuid().toString()).arg(n);

            #endif

            rundir = QDir( molproff.molproTempDir().absolutePath() + unique_dir );
            ++n;

        } while (rundir.exists());

        //ok, the directory doesn't exist - create it!
        if ( not rundir.mkpath(rundir.absolutePath()) )
        {
            throw SireError::file_error( QObject::tr(
                          "Cannot create the tmp directory for molpro, \"%1\". "
                          "Check that you have permission to create directories in "
                          "\"%2\".")
                              .arg(rundir.absolutePath(),
                                   molproff.molproTempDir().absolutePath()), CODELOC );
        }

        //Read the STDERR channel - this is the channel to which the RPC port and magic
        //numbers will be written, and it also contains any molpro errors. In addition,
        //I use STDERR as Molpro is highly talkative on STDOUT and I don't want to
        //waste time processing lots of unnecessary text.
        molpro_process.setReadChannel(QProcess::StandardError);

        //ignore the STDOUT channel
        // The STDOUT channel must be ignored or else it may lead to deadlock.
        // This process uses the RPC client call to request an energy calculation.
        // The Molpro server carries out the calculation, then writes a lot of output
        // to STDOUT. However, the pipe buffer is small, and the output is greater
        // than the size of the pipe buffer. This means that Molpro will block
        // until the pipe buffer is read or cleared. However, this process cannot
        // clear or read the pipe buffer because it is itself blocked waiting for
        // molpro to complete the calculation. Eventually the RPC timeout kicks in,
        // and the call has failed. I have seen this occur with both Qt 4.1 and 4.2,
        // though only 4.2 has the ability to fix it!
        #if QT_VERSION >= 0x040200
          molpro_process.setStandardOutputFile( devNull() );
        #else
          #error Qt >= 4.2 is required to ensure a robust connection to Molpro. \
                 Please upgrade to at least Qt 4.2
        #endif

        //tell the process to use 'rundir' as its working directory
        molpro_process.setWorkingDirectory(rundir.absolutePath());

        //as the process is now in 'rundir', we can set '.' as the TMPDIR
        QStringList env = QProcess::systemEnvironment();

        //remove any existing TMPDIR=
        QRegExp find_tmpdir_regexp("\\s*TMPDIR\\s*=\\s*.*", Qt::CaseInsensitive);

        int idx = env.indexOf(find_tmpdir_regexp);

        while (idx != -1)
        {
            env.removeAt(idx);
            idx = env.indexOf(find_tmpdir_regexp);
        }

        //add the new TMPDIR location
        env.append( QString("TMPDIR=%1").arg(rundir.absolutePath()) );
        molpro_process.setEnvironment(env);

        //start the molpro process
        molpro_process.start(molpro_exe.absoluteFilePath(), QIODevice::ReadWrite);

        //wait for the process to start... (give it one minute)
        if (not molpro_process.waitForStarted(60000))
        {
            throw SireError::process_error( molpro_exe.absoluteFilePath(),
                                            molpro_process, CODELOC );
        }

        //ok - other molpro jobs are allowed to start now
        lkr.unlock();

        //initialise the molpro process...
        int nbytes = molpro_process.write( molproff.molproCommandInput().toLatin1() );
        BOOST_ASSERT(nbytes != -1);

        //tell the molpro process to start its RPC connection
        nbytes = molpro_process.write("user\n");  //eventually will be write("start_rpc\n");
        BOOST_ASSERT(nbytes != -1);

        //close the input (so that molpro starts up fully)
        molpro_process.closeWriteChannel();

        //read the output until we see that the molpro process has started
        //its RPC server...
        int nsecs = 0;
        int portnumber = 0;
        uint magic_key = 0;
        bool rpc_has_started = false;

        do
        {
            if (molpro_process.waitForReadyRead(1000))
            {
                //reset the amount of time without any contact
                //from molpro
                nsecs = 0;

                while (molpro_process.canReadLine())
                {
                    //there is a line of text available to read
                    QString line = molpro_process.readLine();

                    //does the line contain the magic strings that say
                    //whether the RPC server has started?
                    if (line.startsWith("MOLPRO_RPC_STARTED"))
                    {
                        //The RPC server has started - get the hostname and port number
                        QStringList words = line.split(" ",QString::SkipEmptyParts);

                        BOOST_ASSERT(words.count() >= 5);

                        portnumber = words[2].toInt();
                        magic_key = words[4].toUInt();

                        rpc_has_started = true;

                        break;
                    }
                    else if (line.startsWith("MOLPRO_RPC_ERROR"))
                    {
                        //There has been a problem while trying to start the RPC
                        throw SireError::process_error(line, CODELOC);
                    }
                }
            }
            else
            {
                //check that molpro is still running!
                this->assertMolproIsRunning();

                //increment the number of seconds that we have
                //waited without having any output from Molpro...
                ++nsecs;

                if (nsecs > 3600)
                    //one hour without any output suggests
                    //that something has gone wrong...
                    throw SireError::process_error( QObject::tr(
                            "We have gone more than %d seconds without any contact "
                            "from Molpro and it still hasn't started its RPC server!")
                                .arg(nsecs), CODELOC );
            }

        } while(not rpc_has_started);

        //ok, it has started, we no longer need to read anything from molpro
        //as all communication now is via RPC
        //molpro_process.closeReadChannel(QProcess::StandardOutput);

        //now try to connect to the process via RPC
        molpro_rpc = ::connectToMolproHost(QHostInfo::localHostName().toLatin1(),
                                           portnumber, magic_key, 0);

        //has the connection been successful?
        if (not molpro_rpc.client)
            throw SireError::process_error( QObject::tr(
                     "We failed to establish an RPC connection to the Molpro process "
                     "at %1:%2 (magic == %3)")
                        .arg(QHostInfo::localHostName()).arg(portnumber)
                        .arg(magic_key), CODELOC );

        //save the qm and mm arrays that are now loaded into the process
        current_mm = molproff.mmCoordsAndChargesArray();
        current_qm = molproff.qmCoordsArray();

        new_mm = current_mm;
        new_qm = current_qm;

        //ok, everything is now complete!
    }
    catch(...)
    {
        //something went wrong - close the molpro process and remove
        //the run directory
        if (molpro_process.state() == QProcess::Running or
            molpro_process.state() == QProcess::Starting)
        {
            molpro_process.kill();
        }

        //remove the directory
        removeDirectory(rundir);

        //re-throw the exception
        throw;
    }
}

/** Destructor - this closes the molpro process and removes the
    temporary run directory */
MolproSession::~MolproSession()
{
    //stop molpro
    if (molpro_process.state() == QProcess::Running)
    {
        if (molpro_rpc.client)
        {
            //use RPC to tell molpro to stop the RPC loop
            ::exitRPC(molpro_rpc,0);

            //wait until molpro has exited
            if (not molpro_process.waitForFinished())
            {
                //we'll just have to kill it, as it hasn't ended gracefully!
                molpro_process.kill();
            }
        }
        else
        {
            //somehow we've lost the RPC connection - we'll
            //have to kill molpro as we have no other way of
            //communicating with it
            molpro_process.kill();
        }
    }

    //remove the run directory
    removeDirectory(rundir);
}

/** Completely remove the passed directory (and all of its contents)
      - USE WITH CARE!!! */
void MolproSession::removeDirectory(QDir dir)
{
    foreach (QFileInfo file, dir.entryInfoList(QDir::NoDotAndDotDot))
    {
        if (file.isDir())
        {
            removeDirectory(file.absoluteFilePath());
        }
        else
        {
            dir.remove(file.absoluteFilePath());
        }
    }

    dir.rmdir(dir.absolutePath());
}

/** Assert that this session is compatible with the
    Molpro forcefield 'molproff'. This is only compatible
    if the ID and QM version numbers are the same as
    those in the forcefield

    \throw SireError::incompatible_error
*/
void MolproSession::assertCompatibleWith(const MolproFF &molproff) const
{
    if (molproff.ID() != ffid)
        throw SireError::incompatible_error( QObject::tr(
            "The passed Molpro forcefield is incompatible with this session "
            "as it has a different ID number! (%1 vs. %2)")
                .arg(molproff.ID()).arg(ffid), CODELOC );

    else if (molproff.qmVersion() != qm_version)
        throw SireError::incompatible_error( QObject::tr(
            "The passed Molpro forcefield has a different QM version "
            "number to the one on this session! (%1 vs. %2)")
                .arg(molproff.qmVersion()).arg(qm_version),
                    CODELOC );
}

/** Return whether the forcefield is incompatible with this session */
bool MolproSession::incompatibleWith(const MolproFF &molproff) const
{
    return (molproff.ID() != ffid) or (molproff.qmVersion() != qm_version);
}

/** Assert that the Molpro process is still running

    \throw SireError::process_error
*/
void MolproSession::assertMolproIsRunning() const
{
    if (molpro_process.state() != QProcess::Running)
           throw SireError::process_error( molpro_exe.absoluteFilePath(),
                                           molpro_process, CODELOC );
}

/** Set the arrays containing the QM coordinates and MM coordinates and charges */
void MolproSession::setArrays(const QVector<double> &qm_array,
                              const QVector<double> &mm_array)
{
    //use lazy programming - i.e. don't set the
    //arrays now, but wait until the next evaluation
    new_qm = qm_array;
    new_mm = mm_array;
}

/** Capture the output to STDERR and append to the log */
void MolproSession::captureErrorLog()
{
    while (molpro_process.canReadLine())
    {
        QString error = molpro_process.readLine();
        errorlog.append(error);
    }
}

/** Return the current value of the energy */
double MolproSession::getCurrentEnergy()
{
    char *error = 0;

    double nrg = ::get_scalar(molpro_rpc, "ENERGY", &error);

    this->captureErrorLog();

    if (error)
    {
        QString qerror( error );
        free(error);

        throw SireError::process_error( QObject::tr(
                "There was an error obtaining the current energy from Molpro: %1.\n"
                "Errors reported from Molpro:\n%2")
                    .arg(qerror, errorlog.join("")), CODELOC );
    }

    return nrg;
}

/** Calculate the energy of the current system using the
    Molpro commands in 'cmds' - returns the QM energy.

    \throw SireError::process_error
*/
double MolproSession::calculateEnergy(const QString &qcmds)
{
    QByteArray bcmds = qcmds.toLatin1();
    BOOST_ASSERT( not bcmds.isEmpty() );
    const char *cmds = bcmds.constData();

    //molpro still needs to be running!
    this->assertMolproIsRunning();

    double nrg;
    char *error = 0;

    if ( new_qm.isEmpty() )
    {
        //there are no QM atoms, therefore the energy must be zero!
        nrg = 0;
    }
    else if ( new_qm.constData() == current_qm.constData() )
    {
        if (new_mm.constData() == current_mm.constData())
        {
            //the coordinates of the QM and MM regions have not changed
            //since the last evaluation - just run the commands and
            //get the energy
            //nrg = ::run_hf(molpro_rpc, &error);
            nrg = ::calculateEnergy( molpro_rpc, cmds, 0, &error );
        }
        else
        {
            //only the MM region has changed since the last update
            nrg = ::calculateEnergyWithNewMM( molpro_rpc,
                                              cmds,
                                              const_cast<double*>(new_mm.constData()),
                                              new_mm.count() / 4,
                                              0, &error );
        }
    }
    else
    {
        if (new_mm.constData() == current_mm.constData())
        {
            //the coordinates of only the QM region
            //has changed since the last update
            nrg = ::calculateEnergyWithNewQM( molpro_rpc,
                                              cmds,
                                              const_cast<double*>(new_qm.constData()),
                                              new_qm.count() / 3,
                                              0, &error );
        }
        else
        {
            //both the QM and MM regions have changed since the last update
            nrg = ::calculateEnergyWithNewQMMM( molpro_rpc, cmds,
                                                const_cast<double*>(new_qm.constData()),
                                                new_qm.count() / 3,
                                                const_cast<double*>(new_mm.constData()),
                                                new_mm.count() / 4,
                                                0, &error );
        }
    }

    //capture any errors
    this->captureErrorLog();

    if (error)
    {
        //something went wrong!
        QString qerror(error);
        free(error);

        throw SireError::process_error( QObject::tr(
                "Error detected while running the Molpro commands \"%1\" : %2\n"
                "Molpro error log:\n%3")
                    .arg(qcmds, qerror, errorlog.join("")), CODELOC );

    }

    //ok - copy the new arrays to old as they are now definitely
    //on the molpro process
    current_qm = new_qm;
    current_mm = new_mm;

    //return the energy
    return nrg;
}
