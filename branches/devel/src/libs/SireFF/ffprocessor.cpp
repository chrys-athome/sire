
#include "SireCAS/qhash_sirecas.h"

#include "ffprocessor.h"
#include "forcefield.h"

#include "ffworker.h"
#include "ffcalculator.h"

#include "SireMol/molecule.h"
#include "SireMol/moleculeid.h"

#include "SireError/errors.h"

using namespace SireFF;
using namespace SireFF::detail;

using namespace SireCluster;
using namespace SireCluster::detail;

using namespace SireMol;

/////////
///////// Implementation of FFProcessorBasePvt
/////////

/** Construct an unnamed processor info */
FFProcessorBasePvt::FFProcessorBasePvt() : ProcessorPvt()
{}

/** Construct a named processor info */
FFProcessorBasePvt::FFProcessorBasePvt(const QString &name) : ProcessorPvt(name)
{}

/** Destructor */
FFProcessorBasePvt::~FFProcessorBasePvt()
{}

/////////
///////// Implementation of FFProcessorBase
/////////

/** Constructor, passing the supplied data */
FFProcessorBase::FFProcessorBase(const boost::shared_ptr<FFProcessorBasePvt> &data)
                : Processor(data)
{
    d = dynamic_cast<FFProcessorBasePvt*>( &(Processor::data()) );

    if (not d)
        throw SireError::program_bug( QObject::tr(
                "Cannot create an FFProcessorBase with a non FFProcessorBasePvt "
                "derived info object!"), CODELOC );
}

/** Protected copy constructor */
FFProcessorBase::FFProcessorBase(const FFProcessorBase &other)
            : Processor(other), d(other.d)
{}

/** Destructor */
FFProcessorBase::~FFProcessorBase()
{}

/** Return the object containing the data for this FFProcessorBase */
FFProcessorBasePvt& FFProcessorBase::data()
{
    return *d;
}

/** Return the object containing the data for this FFProcessorBase */
const FFProcessorBasePvt& FFProcessorBase::data() const
{
    return const_cast<FFProcessorBasePvt&>(*d);
}

/** Return a copy of the forcefield that is being processed by
    this processor. */
ForceField FFProcessorBase::forcefield() const
{
    return data().forcefield();
}

/** Give this processor the forcefield 'forcefield'. This will throw
    an exception if the forcefield is already active, or if the
    forcefield is not compatible with the processor (some processors
    are forcefield specific, e.g. the MolproProcessor)

    \throw SireCluster::already_active
    \throw SireError::incompatible_error
*/
void FFProcessorBase::setForceField(const ForceField &forcefield)
{
    data().setForceField(forcefield);
}

/** Return a copy of the molecule with MoleculeID == molid from the
    forcefield that is being processed by this processor.

    \throw SireMol::missing_molecule
*/
Molecule FFProcessorBase::molecule(MoleculeID molid) const
{
    return data().molecule(molid);
}

/** Activate this processor and return a pointer to the
    activated processor.

    \throw SireCluster::already_active
*/
boost::shared_ptr<FFWorkerBase> FFProcessorBase::activate()
{
    boost::shared_ptr<WorkerBase> worker = data().activate();

    boost::shared_ptr<FFWorkerBase> ffworker =
                      boost::dynamic_pointer_cast<FFWorkerBase,WorkerBase>(worker);

    if (not ffworker)
        throw SireError::program_bug( QObject::tr(
                "Activating an FFProcessorBase has not resulted in an FFWorkerBase!"),
                    CODELOC );

    return ffworker;
}

/////////
///////// Implementation of FFProcessorPvt
/////////

/** Constructor (using a null forcefield) */
FFProcessorPvt::FFProcessorPvt() : FFProcessorBasePvt()
{}

/** Constructor, specifying the forcefield to be evaluated */
FFProcessorPvt::FFProcessorPvt(const ForceField &forcefield)
               : FFProcessorBasePvt(), ffield(forcefield)
{}

/** Destructor */
FFProcessorPvt::~FFProcessorPvt()
{}

/** Return a copy of the forcefield being evaluated */
ForceField FFProcessorPvt::forcefield() const
{
    return ffield;
}

/** Set the forcefield to be evaluated */
void FFProcessorPvt::setForceField(const ForceField &forcefield)
{
    ffield = forcefield;
}

/** Return a copy of the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
Molecule FFProcessorPvt::molecule(MoleculeID molid) const
{
    return ffield.molecule(molid);
}

/** Activate this forcefield and return a pointer to the active
    processor worker */
boost::shared_ptr<WorkerBase> FFProcessorPvt::_pvt_activate()
{
    return boost::shared_ptr<WorkerBase>( new FFWorker( new FFCalculator(ffield) ) );
}

/////////
///////// Implementation of FFProcessor
/////////

/** Construct an FFProcessor that works on a null forcefield */
FFProcessor::FFProcessor()
            : FFProcessorBase(
                  boost::shared_ptr<FFProcessorBasePvt>(
                                                  new FFProcessorPvt()))
{}

/** Construct an FFProcessor that works on the forcefield 'forcefield' */
FFProcessor::FFProcessor(const ForceField &forcefield)
            : FFProcessorBase(
                  boost::shared_ptr<FFProcessorBasePvt>(
                                                  new FFProcessorPvt(forcefield)))
{}

/** Copy constructor */
FFProcessor::FFProcessor(const FFProcessor &other)
            : FFProcessorBase(other)
{}

/** Destructor */
FFProcessor::~FFProcessor()
{}
