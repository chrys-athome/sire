
#include "ffworker.h"

#include "SireError/errors.h"

using namespace SireFF;

/////////
///////// Implementation of FFWorkerBase
/////////

/** Constructor */
FFWorkerBase::FFWorkerBase()
             : current_state(FFWorkerBase::IDLE),
               needs_energy_recalculation(true)   //need to calculate the first energy!
{}

/** Destructor */
FFWorkerBase::~FFWorkerBase()
{}

/** Wait until the processor has finished its last job */
void FFWorkerBase::waitUntilReady()
{
    if ( current_state == IDLE )
    {
        //the processor is not running
        return;
    }
    else if ( current_state | CALCULATING_ENERGY )
    {
        //the processor is recalculating the energy... wait until it has finished
        this->_pvt_waitUntilReady();

        //the processor is now idle
        current_state = IDLE;

        //get the values of the energy and its components
        total_nrg = this->getEnergies(nrg_components);

        //the energy no longer needs recalculating
        needs_energy_recalculation = false;
    }
    else
    {
        //wait until the processor has finished
        this->_pvt_waitUntilReady();

        //the processor is now idle
        current_state = IDLE;
    }
}

/** Trigger a recalculation of the energy in the background.
    Use 'waitUntilReady()' to block until the energy calculation
    is complete */
void FFWorkerBase::recalculateEnergy()
{
    if ( needs_energy_recalculation )
    {
        if (current_state == IDLE)
        {
            // Trigger a recalculation
            this->_pvt_recalculateEnergy();
            current_state = CALCULATING_ENERGY;
        }
        else if (current_state | CALCULATING_ENERGY)
        {
            //an energy recalculation is already underway
            return;
        }
        else
        {
            // wait for whatever else the processor is doing...
            this->waitUntilReady();

            //try again...
            this->recalculateEnergy();
        }
    }
}

/** Check that the energies are up to date - if not, then block
    until they have been recalculated */
void FFWorkerBase::checkEnergiesUpToDate()
{
    if ( needs_energy_recalculation )
    {
        if (current_state == IDLE)
        {
            // Trigger a foreground recalculation - this blocks
            // until the calculation is complete
            this->_pvt_recalculateEnergyFG();

            total_nrg = this->getEnergies(nrg_components);
            needs_energy_recalculation = false;
        }
        else if (current_state | CALCULATING_ENERGY)
        {
            //an energy calculation is underway - wait until it has finished
            this->waitUntilReady();
        }
        else
        {
            //wait for the processor to finish whatever else it is doing...
            this->waitUntilReady();

            //now check again - (this should trigger a foreground calculation)
            this->checkEnergiesUpToDate();
        }
    }
}

/** Return the total energy of the forcefield */
double FFWorkerBase::energy()
{
    this->checkEnergiesUpToDate();

    return total_nrg;
}

/** Return the energy of the component represented by the function 'component'

    \throw SireFF::missing_component
*/
double FFWorkerBase::energy(const Function &component)
{
    this->assertContains(component);

    this->checkEnergiesUpToDate();

    return nrg_components.value(component);
}

/** Return the values of all of the energy components from this forcefield */
Values FFWorkerBase::energies()
{
    this->checkEnergiesUpToDate();

    return nrg_components;
}

/** Set the forcefield to be evaluated on this processor

    \throw SireError::incompatible_error
*/
void FFWorkerBase::setForceField(const ForceField &forcefield)
{
    //wait until the processor is idle
    this->waitUntilReady();

    //set the forcefield, recording whether it needs energy recalculation
    needs_energy_recalculation = this->_pvt_setForceField(forcefield);
}

/** Move the molecule 'molecule' */
void FFWorkerBase::move(const Molecule &molecule)
{
    //wait until the processor is idle
    this->waitUntilReady();

    needs_energy_recalculation = needs_energy_recalculation or
                                 this->_pvt_move(molecule);
}

/** Move the residue 'residue' */
void FFWorkerBase::move(const Residue &residue)
{
    //wait until the processor is idle
    this->waitUntilReady();

    needs_energy_recalculation = needs_energy_recalculation or
                                 this->_pvt_move(residue);
}

/////////
///////// Implementation of FFWorker
/////////

/** Constructor, specifying the FFCalculator to use */
FFWorker::FFWorker(FFCalculator *calculator)
         : FFWorkerBase(), ffcalculator(calculator)
{
    if (not calculator)
        throw SireError::program_bug( QObject::tr(
                "Cannot create an FFWorker with a null FFCalculator!"),
                    CODELOC );
}

/** Destructor */
FFWorker::~FFWorker()
{}

/** Return the forcefield being evaluated */
ForceField FFWorker::forcefield() const
{
    return ffcalculator->forcefield();
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
Molecule FFWorker::molecule(MoleculeID molid) const
{
    return ffcalculator->molecule(molid);
}

/** Set the forcefield to be evaluated, returning whether it
    needs an energy recalculation */
bool FFWorker::_pvt_setForceField(const ForceField &forcefield)
{
    return ffcalculator->setForceField(forcefield);
}

/** Move the molecule 'molecule', returning whether or not
    it needs an energy recalculation */
bool FFWorker::_pvt_move(const Molecule &molecule)
{
    return ffcalculator->move(molecule);
}

/** Move the residue 'residue', returning whether or not
    it needs an energy recalculation */
bool FFWorker::_pvt_move(const Residue &residue)
{
    return ffcalculator->move(residue);
}

/** Recalculate the energy is the foreground */
void FFWorker::_pvt_recalculateEnergyFG()
{
}

/** Recalculate the energy */
void FFWorker::_pvt_recalculateEnergy()
{
    //uses main thread, so do it in the foreground
    this->_pvt_recalculateEnergyFG();
}

void FFWorker::_pvt_waitUntilReady();

double FFWorker::_pvt_getEnergies(Values &components);
