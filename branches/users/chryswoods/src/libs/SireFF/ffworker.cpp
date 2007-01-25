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

#include "SireCAS/qhash_sirecas.h"

#include "ffworker.h"
#include "forcefield.h"
#include "ffcalculator.h"

#include "SireCAS/function.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/moleculeid.h"

#include "SireError/errors.h"
#include "SireFF/errors.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireCAS;

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
        total_nrg = this->_pvt_getEnergies(nrg_components);

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

            total_nrg = this->_pvt_getEnergies(nrg_components);
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
double FFWorkerBase::energy(const FFComponent &component)
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

/** Add the molecule 'molecule' */
void FFWorkerBase::add(const Molecule &molecule, const ParameterMap &map)
{
    //wait until the processor is idle
    this->waitUntilReady();

    needs_energy_recalculation = needs_energy_recalculation or
                                 this->_pvt_add(molecule,map);
}

/** Add the residue 'residue' */
void FFWorkerBase::add(const Residue &residue, const ParameterMap &map)
{
    //wait until the processor is idle
    this->waitUntilReady();

    needs_energy_recalculation = needs_energy_recalculation or
                                 this->_pvt_add(residue,map);
}

/** Change the molecule 'molecule' */
void FFWorkerBase::change(const Molecule &molecule)
{
    //wait until the processor is idle
    this->waitUntilReady();

    needs_energy_recalculation = needs_energy_recalculation or
                                 this->_pvt_change(molecule);
}

/** Change the residue 'residue' */
void FFWorkerBase::change(const Residue &residue)
{
    //wait until the processor is idle
    this->waitUntilReady();

    needs_energy_recalculation = needs_energy_recalculation or
                                 this->_pvt_change(residue);
}

/** Remove the molecule 'molecule' */
void FFWorkerBase::remove(const Molecule &molecule)
{
    //wait until the processor is idle
    this->waitUntilReady();

    needs_energy_recalculation = needs_energy_recalculation or
                                 this->_pvt_remove(molecule);
}

/** Remove the residue 'residue' */
void FFWorkerBase::remove(const Residue &residue)
{
    //wait until the processor is idle
    this->waitUntilReady();

    needs_energy_recalculation = needs_energy_recalculation or
                                 this->_pvt_remove(residue);
}

/** Replace the molecule 'oldmol' with 'newmol' */
void FFWorkerBase::replace(const Molecule &oldmol,
                           const Molecule &newmol, const ParameterMap &map)
{
    //wait until the processor is idle
    this->waitUntilReady();

    needs_energy_recalculation = needs_energy_recalculation or
                                 this->_pvt_replace(oldmol,newmol,map);
}

/** Assert that this forcefield contains the energy component 'component'

    \throw SireFF::missing_component
*/
void FFWorkerBase::assertContains(const FFComponent &component) const
{
    if (not nrg_components.values().contains(component.ID()))
        this->forcefield().components().assertContains(component);
}

/////////
///////// Implementation of FFWorker
/////////

/** Constructor, specifying the FFCalculator to use */
FFWorker::FFWorker(FFCalculator *calculator)
         : FFWorkerBase(), WorkerBase(), ffcalculator(calculator)
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

/** Set the forcefield to be evaluated, returning whether it
    needs an energy recalculation */
bool FFWorker::_pvt_setForceField(const ForceField &forcefield)
{
    return ffcalculator->setForceField(forcefield);
}

/** Add the molecule 'molecule', returning whether or not
    it needs an energy recalculation */
bool FFWorker::_pvt_add(const Molecule &molecule, const ParameterMap &map)
{
    return ffcalculator->add(molecule, map);
}

/** Add the residue 'residue', returning whether or not
    it needs an energy recalculation */
bool FFWorker::_pvt_add(const Residue &residue, const ParameterMap &map)
{
    return ffcalculator->add(residue, map);
}

/** Change the molecule 'molecule', returning whether or not
    it needs an energy recalculation */
bool FFWorker::_pvt_change(const Molecule &molecule)
{
    return ffcalculator->change(molecule);
}

/** Change the residue 'residue', returning whether or not
    it needs an energy recalculation */
bool FFWorker::_pvt_change(const Residue &residue)
{
    return ffcalculator->change(residue);
}

/** Remove the molecule 'molecule', returning whether or not
    it needs an energy recalculation */
bool FFWorker::_pvt_remove(const Molecule &molecule)
{
    return ffcalculator->remove(molecule);
}

/** Remove the residue 'residue', returning whether or not
    it needs an energy recalculation */
bool FFWorker::_pvt_remove(const Residue &residue)
{
    return ffcalculator->remove(residue);
}

/** Replace 'oldmol' with 'newmol', returning whether or not
    it needs an energy recalculation */
bool FFWorker::_pvt_replace(const Molecule &oldmol,
                            const Molecule &newmol, const ParameterMap &map)
{
    return ffcalculator->replace(oldmol, newmol, map);
}

/** Recalculate the energy is the foreground */
void FFWorker::_pvt_recalculateEnergyFG()
{
    ffcalculator->calculateEnergy();
}

/** Recalculate the energy */
void FFWorker::_pvt_recalculateEnergy()
{
    //uses main thread, so do it in the foreground
    this->_pvt_recalculateEnergyFG();
}

/** Wait until the calculation has completed - in this case,
    as the calculation is in the main thread, it has already
    finished! */
void FFWorker::_pvt_waitUntilReady()
{
    return;
}

/** Return the calculated total energy of the forcefield,
    and the values of the components */
double FFWorker::_pvt_getEnergies(Values &components)
{
    return ffcalculator->getEnergies(components);
}
