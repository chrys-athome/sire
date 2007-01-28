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

#include "molproff.h"
#include "molprosession.h"

#include "SireError/errors.h"

using namespace Squire;
using namespace SireBase;

QMutex MolproSession::starter_mutex;

/** Construct a session that will represent the forcefield 'molproff' */
MolproSession::MolproSession(const MolproFF &molproff)
              : boost::noncopyable(),
                ffid( molproff.ID() ),
                ffversion( molproff.version() )
{
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

            rundir = QDir( tmpdir.absolutePath() + unique_dir );

            ++n;

        } while (rundir.exists());

        //ok, the directory doesn't exist - create it!
        if ( not rundir.mkdir(rundir.absolutePath()) )
        {
            throw SireError::file_error( QObject::tr(
                          "Cannot create the tmp directory for molpro, \"%1\". "
                          "Check that you have permission to create directories in "
                          "\"%2\".")
                              .arg(rundir.absolutePath(), tmpdir.absolutePath()), CODELOC );
        }

        //merge the STDOUT and STDERR channels into one
        molpro_process.setProcessChannelMode(QProcess::MergedChannels);

        //tell the process to use 'rundir' as its working directory
        molpro_process.setWorkingDirectory(rundir.absolutePath());

        //as the process is now in 'rundir', we can set '.' as the TMPDIR
        QStringList env = molpro_process.environment();
        env.append( "TMPDIR=." );
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

        QTextStream ts(&molpro_process);

        //initialise the molpro process...
        molproff.initialise(ts);

        //tell the molpro process to start its RPC connection
        ts << "user\n";     //eventually will be ts << "start_rpc\n";

        //close the input (so that molpro starts up fully)
        molpro_process.closeWriteChannel();

        //read the output until we see that the molpro process has started
        //its RPC server...
        int nsecs = 0;
        QHostAddress serveraddress;
        int portnumber;
        QString magic_key;
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
                    //there is a line of text available to read - read it now
                    //using the QTextStream
                    QString line = ts.readLine();

                    //does the line contain the magic strings that say
                    //whether the RPC server has started?
                    if (line.startsWith("MOLPRO_RPC_STARTED"))
                    {
                        //The RPC server has started - get the hostname and port number
                        QStringList words = line.split(" ",QString::SkipEmptyParts);

                        BOOST_ASSERT(words.count() == 3);

                        hostname = words[1];
                        portnumber = words[2].toInt();
                        magic_key = words[3];

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

                if (nsecs > 60)
                    //60 seconds without any output suggests
                    //that something has gone wrong...
                    throw SireError::process_error( QObject::tr(
                            "We have gone more than %d seconds without any contact "
                            "from Molpro and it still hasn't started its RPC server!")
                                .arg(60), CODELOC );
            }

        } while(not rpc_has_started);

        //ok, it has started, we no longer need to read anything from molpro
        //as all communication now is via RPC
        molpro_process.closeReadChannel(QProcess::StandardOutput);
        molpro_process.closeReadChannel(QProcess::StandardError);

        //now try to connect to the process via RPC
        molpro_rpc = ::connectToMolpro(hostname.toLatin1(), portnumber,
                                       magic_key.toLatin1());

        //has the connection been successful?
        if (not molpro_rpc.client)
            throw SireError::process_error( QObject::tr(
                     "We failed to establish an RPC connection to the Molpro process "
                     "at %1:%2").arg(hostname).arg(portnumber), CODELOC );

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
        if (rundir.exists())
            rundir.remove(rundir.absolutePath());

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
            ::exitRPC(molpro_rpc);

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
    if (rundir.exists())
        rundir.remove(rundir.absolutePath());
}

/** Assert that this session is compatible with the
    Molpro forcefield 'molproff'. This is only compatible
    if the ID and major version numbers are the same as
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

    else if (molproff.version().major() != ffversion.major())
        throw SireError::incompatible_error( QObject::tr(
            "The passed Molpro forcefield has a different major version "
            "number to the one on this session! (%1 vs. %2)")
                .arg(molproff.version().toString(), ffversion.toString()),
                    CODELOC );

    //what about major version number change caused by just changing
    //MM molecules? This would still work?
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

/** Calculate the energy of the current system using the
    Molpro commands in 'cmds' - returns the QM energy.

    \throw SireError::process_error
*/
double MolproSession::calculateEnergy(const char *cmds)
{
    BOOST_ASSERT( cmds != 0 );

    //molpro still needs to be running!
    this->assertMolproIsRunning();

    double nrg;

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
            nrg = ::calculateEnergy(molpro_rpc, cmds);
        }
        else
        {
            //only the MM region has changed since the last update
            nrg = ::calculateEnergyWithNewMM( molpro_rpc,
                                              cmds,
                                              const_cast<double*>(new_mm.constData()),
                                              new_mm.count() / 4 );
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
                                              new_qm.count() / 3 );
        }
        else
        {
            //both the QM and MM regions have changed since the last update
            nrg = ::calculateEnergyWithNewQMMM( molpro_rpc, cmds,
                                                const_cast<double*>(new_qm.constData()),
                                                new_qm.count() / 3,
                                                const_cast<double*>(new_mm.constData()),
                                                new_mm.count() / 4 );
        }
    }

    //ok - copy the new arrays to old as they are now definitely
    //on the molpro process
    current_qm = new_qm;
    current_mm = new_mm;

    //return the energy
    return nrg;
}
