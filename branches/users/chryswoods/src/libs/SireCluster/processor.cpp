
#include "processor.h"

#include "SireError/errors.h"
#include "SireCluster/errors.h"

using namespace SireCluster;
using namespace SireCluster::detail;

/////////
///////// Implementation of WorkerBase
/////////

/** Constructor */
WorkerBase::WorkerBase() : boost::noncopyable()
{}

/** Destructor */
WorkerBase::~WorkerBase()
{}

/////////
///////// Implementation of ProcessorPvt
/////////

/** Construct with the supplied name */
ProcessorPvt::ProcessorPvt(const QString &name)
             : boost::noncopyable(), datamutex(QMutex::Recursive), nme(name)
{}

/** Destructor */
ProcessorPvt::~ProcessorPvt()
{}

/** Return whether or not this processor is active */
bool ProcessorPvt::isActive() const
{
    QMutexLocker l(mutex());

    return not active_processor.expired();
}

/** Return the name of the processor */
QString ProcessorPvt::name() const
{
    QMutexLocker l(mutex());

    return nme;
}

/** Set the name of the processor - this cannot be done
    if the processor is already active.

    \throw SireCluster::already_active
*/
void ProcessorPvt::setName(const QString &name)
{
    QMutexLocker l(mutex());

    if (isActive())
        throw SireCluster::already_active( QObject::tr(
                  "Cannot change the name of the processor \"%1\" to \"%2\" "
                  "as the processor is currently active.")
                      .arg(nme, name), CODELOC );

    nme = name;
}

/** Activate this processor. An already activated processor cannot
    be activated. Return a shared pointer to the active processor.

    \throw SireCluster::already_active
*/
boost::shared_ptr<WorkerBase> ProcessorPvt::activate()
{
    QMutexLocker l(mutex());

    if (isActive())
        throw SireCluster::already_active( QObject::tr(
                "Cannot activate the processor \"%1\" as it is already active!")
                    .arg(nme), CODELOC );

    boost::shared_ptr<WorkerBase> retval = _pvt_activate();

    active_processor = retval;

    return retval;
}

/////////
///////// Implementation of Processor
/////////

/** Protected constructor used to pass in the processor data.
    It is a program bug to pass in a null data pointer!
*/
Processor::Processor(const boost::shared_ptr<ProcessorPvt> &data) : d(data)
{
    if (not d)
        throw SireError::program_bug( QObject::tr(
                "You must supply a valid ProcessorPvt pointer to the "
                "Processor's constructor."), CODELOC );
}

/** Protected copy constructor */
Processor::Processor(const Processor &other) : d(other.d)
{}

/** Destructor */
Processor::~Processor()
{}

/** Return the private implementation holding the processor data */
ProcessorPvt& Processor::data()
{
    if (not d)
        throw SireError::program_bug( QObject::tr(
                "The Processor data should never be null!"), CODELOC );

    return *d;
}

/** Return the private implementation holding the processor data */
const ProcessorPvt& Processor::data() const
{
    if (not d)
        throw SireError::program_bug( QObject::tr(
                "The Processor data should never be null!"), CODELOC );

    return *d;
}

/** Return the name of the processor */
QString Processor::name() const
{
    return data().name();
}

/** Activate this processor - a processor may only be activated
    if it is not already active. Returns the activated processor.

    \throw SireCluster::already_active
*/
boost::shared_ptr<ThreadProcessor> Processor::activate()
{
    return data().activate();
}

/** Set the name of the processor - you may only change a processors
    name if it is not already active.

    \throw SireCluster::already_active
*/
void Processor::setName(const QString &name)
{
    data().setName(name);
}
