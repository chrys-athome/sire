
#include "molproprocessor.h"
#include "molprocalculator.h"

#include "molproff.h"

#include "SireFF/ffthreadworker.h"

#include "SireError/errors.h"

using namespace Squire;
using namespace Squire::detail;

using namespace SireFF;
using namespace SireFF::detail;

using namespace SireCluster;

//////////
////////// Implementation of MolproProcessorPvt
//////////

/** Null constructor - the resulting worker will not be able to work! */
MolproProcessorPvt::MolproProcessorPvt()
                   : FFThreadProcessorPvt(),
                     molpro_exe("molpro"), tmpdir(QDir::temp())
{}

/** Construct with a specified forcefield (which must be derived from MolproFF),
    specified molpro executable, and specified temp directory

    \throw SireError::incompatible_error
*/
MolproProcessorPvt::MolproProcessorPvt(const ForceField &forcefield,
                                       const QString &molpro_executable,
                                       const QDir &temp_dir)
                   : FFThreadProcessorPvt(forcefield),
                     molpro_exe(molpro_executable),
                     tmpdir(temp_dir)
{
    if (not forcefield.isA<MolproFF>())
    {
        throw SireError::incompatible_error( QObject::tr(
                  "Cannot place a non-MolproFF derived forcefield (\"%1\") "
                  "onto a MolproProcessor!").arg(forcefield.what()), CODELOC );
    }
}

/** Destructor */
MolproProcessorPvt::~MolproProcessorPvt()
{}

/** Set the full name and path to the molpro executable */
void MolproProcessorPvt::setMolpro(const QString &molpro_executable)
{
    molpro_exe = molpro_executable;
}

/** Set the location of the temporary directory in which to run molpro */
void MolproProcessorPvt::setTempDir(const QDir &temp_dir)
{
    tmpdir = temp_dir;
}

/** Activate this processor, returning a pointer to the worker */
boost::shared_ptr<WorkerBase> MolproProcessorPvt::_pvt_activate()
{
    return boost::shared_ptr<WorkerBase>(
                  new FFThreadWorker(
                      new MolproCalculator(
                              forcefield(), molpro_exe, tmpdir
                                          )
                                    )
                                        );
}

//////////
////////// Implementation of MolproProcessor
//////////

/** Constructor */
MolproProcessor::MolproProcessor()
                : FFThreadProcessor( boost::shared_ptr<FFThreadProcessorPvt>(
                                                    new MolproProcessorPvt())
                                   )
{}

/** Construct for a specified forcefield (which must be derived from MolproFF),
    molpro executable and temp dir */
MolproProcessor::MolproProcessor(const ForceField &forcefield,
                                 const QString &molpro_executable,
                                 const QDir &temp_dir)
                : FFThreadProcessor( boost::shared_ptr<FFThreadProcessorPvt>(
                                               new MolproProcessorPvt(forcefield,
                                                                      molpro_executable,
                                                                      temp_dir)
                                                                            )
                                   )
{}

/** Construct from a MolproProcessorPvt pointer */
MolproProcessor::MolproProcessor(const boost::shared_ptr<MolproProcessorPvt> &data)
                : FFThreadProcessor(data)
{}

/** Copy constructor */
MolproProcessor::MolproProcessor(const MolproProcessor &other)
                : FFThreadProcessor(other)
{}

/** Destructor */
MolproProcessor::~MolproProcessor()
{}

/** Return the data of this processor */
MolproProcessorPvt& MolproProcessor::data()
{
    return dynamic_cast<MolproProcessorPvt&>( FFThreadProcessor::data() );
}

/** Return the data of this processor */
const MolproProcessorPvt& MolproProcessor::data() const
{
    return dynamic_cast<const MolproProcessorPvt&>( FFThreadProcessor::data() );
}

/** Set the full path and name of the molpro executable to use to
    evaluate QM energies */
void MolproProcessor::setMolpro(const QString &molpro_exe)
{
    data().setMolpro(molpro_exe);
}

/** Set the directory used as the temporary directory */
void MolproProcessor::setTempDir(const QDir &tmpdir)
{
    data().setTempDir(tmpdir);
}
