
#include "ffthreadworker.h"

#include "SireError/errors.h"

using namespace SireFF;
using namespace SireCluster;

/** Constructor */
FFThreadWorker::FFThreadWorker(FFCalculator *calculator)
               : FFProcessor(), ThreadProcessor(),
                 ffcalculator(calculator)
{
    if (not calculator)
        throw SireError::program_bug( QObject::tr(
                "Cannot create an FFThreadWorker with a null FFCalculator!"),
                    CODELOC );
}

/** Destructor */
FFThreadWorker::~FFThreadWorker()
{}

/** Return a copy of the forcefield being calculated. This
    will block until the processor is idle */
ForceField FFThreadWorker::forcefield() const
{
    //wait until any running calculation has finished
    ThreadProcessor::waitUntilReady();

    return ffcalculator->forcefield();
}

/** Set the forcefield to be calculated, returning whether or not
    the energy needs to be recalculated

    \throw SireError::incompatible_error
*/
bool FFThreadWorker::_pvt_setForceField(const ForceField &forcefield)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->setForceField(forcefield);
}

/** Move the molecule 'molecule', returning whether or not the
    energy now needs to be recalculated */
bool FFThreadWorker::_pvt_move(const Molecule &molecule)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->move(molecule);
}

/** Move the residue 'residue', returning whether or not the
    energy now needs to be recalculated */
bool FFThreadWorker::_pvt_move(const Residue &residue)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->move(residue);
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
Molecule FFThreadWorker::molecule(MoleculeID molid) const
{
    return ffcalculator->molecule(molid);
}

/** Recalculate the energy in the background */
void FFThreadWorker::_pvt_recalculateEnergy()
{
    //wait until the last calculation has finished
    ThreadProcessor::runBG();
}

/** Recalculate the energy in the foreground */
void FFThreadWorker::_pvt_recalculateEnergyFG()
{
    ThreadProcessor::runFG();
}

/** Wait until the last calculation has finished */
void FFThreadWorker::_pvt_waitUntilReady()
{
    ThreadProcessor::waitUntilReady();
}

/** Get the energies from the forcefield */
double FFThreadWorker::_pvt_getEnergies(Values &nrg_components)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->getEnergies(nrg_components);
}

/** Perform the actual energy recalculation */
void FFThreadWorker::calculate()
{
    ffcalculator->calculateEnergy();
}
