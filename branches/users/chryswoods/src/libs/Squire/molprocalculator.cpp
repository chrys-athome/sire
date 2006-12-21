
#include "SireMol/qhash_siremol.h"
#include "SireCAS/qhash_sirecas.h"

#include "molprocalculator.h"

#include <QTextStream>
#include <QFileInfo>
#include <QDateTime>
#include <QUuid>

#ifdef Q_OS_UNIX
//Possible Unix-only header file
#include <unistd.h>
#endif

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

#include "SireError/errors.h"

using namespace Squire;
using namespace SireFF;
using namespace SireMol;

//////////
////////// Implementation of MolproSession
//////////

QMutex MolproSession::starter_mutex;

/** Construct a session that will represent the forcefield 'molproff'
    using the molpro executable whose full path and name are given in
    'molpro_executable' */
MolproSession::MolproSession(const QFileInfo &molpro_executable,
                             const MolproFF &molproff,
                             const QDir &tmpdir)
              : molpro_exe(molpro_executable),
                ff_id( molproff.ID() ),
                ff_version( molproff.version() )
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

        //tell the process to use '.' as the tmp directory - this is because
        //the SandboxProcess will chroot into the unique temporary directory
        //that we have just created
        molpro_process.setWorkingDirectory(rundir.absolutePath());

        QStringList env = molpro_process.environment();
        env.append( "TMPDIR=." );
        molpro_process.setEnvironment(env);

        //start the molpro process
        molpro_process.start(molpro_exe.absoluteFilePath(), QIODevice::WriteOnly);

        //wait for the process to start...
        if (not molpro_process.waitForStarted())
        {
            throw SireError::process_error( molpro_exe.absoluteFilePath(),
                                            molpro_process, CODELOC );
        }

        QTextStream ts(&molpro_process);

        //initialise the molpro process...
        molproff.initialise(ts);

        //tell the molpro process to start its RPC connection
        ts << "user\n";

        //close the input (so that molpro starts up fully)
        molpro_process.closeWriteChannel();

        //now try to connect to the process via RPC
        //....

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
        //use RPC to tell molpro to stop the RPC loop
        //

        //wait until molpro has exited
        if (not molpro_process.waitForFinished())
        {
            //we'll just have to kill it, as it hasn't ended gracefully!
            molpro_process.kill();
        }
    }

    //remove the run directory
    if (rundir.exists())
        rundir.remove(rundir.absolutePath());
}

//////////
////////// Implementation of MolproCalculator
//////////

/** Construct a calculator that evaluates the forcefield 'forcefield'
    using the molpro executable called 'molproexe'. Note that
    this calculator is only compatible with forcefields that are
    derived from MolproFF.

    \throw SireError::invalid_cast
*/
MolproCalculator::MolproCalculator(const ForceField &forcefield,
                                   const QFileInfo &molproexe,
                                   const QDir &tmpdir)
                 : FFCalculatorBase(),
                   molpro_exe(molproexe),
                   temp_dir(tmpdir)
{
    //its at this point that we ensure that the molpro executable exists and
    //is in the path (we also resolve it in the path)
    if ( not (molpro_exe.exists() and molpro_exe.isExecutable()) )
    {
    }

    //set the forcefield
    MolproCalculator::setForceField(forcefield);
}

/** Destructor */
MolproCalculator::~MolproCalculator()
{}

/** Return the value of the total energy of the forcefield, as well as
    the energy components */
double MolproCalculator::getEnergies(Values &values)
{
    values = nrg_components;
    return total_nrg;
}

/** Tell the molpro forcefield to recalculate its energy */
void MolproCalculator::calculateEnergy()
{
    if ( not molpro_session or molpro_session->incompatibleWith(*molproff) )
    {
        //kill the old session (do this first to prevent having two
        //molpro jobs running simultaneously - think of the memory!)
        molpro_session.reset();

        //start a new session
        molpro_session.reset( new MolproSession(molpro_exe, *molproff, temp_dir) );
    }

    nrg_components = molproff->recalculateEnergy(*molpro_session);
    total_nrg = nrg_components.value(molproff->components().total());
}

/** Move the molecule 'molecule' and return whether the energy of
    the forcefield needs to be recalculated */
bool MolproCalculator::move(const Molecule &molecule)
{
    return molproff->move(molecule);
}

/** Move the residue 'residue' and return whether the energy of
    the forcefield now needs to be recalculated */
bool MolproCalculator::move(const Residue &residue)
{
    return molproff->move(residue);
}

/** Set the forcefield for this calculator - this calculator is
    only compatible with forcefields that are derived from MolproFF.
    Return whether the energy of the forcefield needs to be
    recalculated.

    \throw SireError::invalid_cast
*/
bool MolproCalculator::setForceField(const ForceField &forcefield)
{
    molproff = forcefield;

    if (molproff->isDirty())
        return true;
    else
    {
        nrg_components = molproff->energies();
        total_nrg = nrg_components.value(molproff->total());

        return false;
    }
}

/** Return a copy of the forcefield being evaluated */
ForceField MolproCalculator::forcefield() const
{
    return ForceField(molproff);
}
