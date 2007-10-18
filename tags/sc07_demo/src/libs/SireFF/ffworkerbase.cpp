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

#include "ffworkerbase.h"
#include "forcefield.h"
#include "ffcalculator.h"

#include "SireCAS/function.h"

#include "SireMol/molecule.h"
#include "SireMol/molecules.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/residue.h"
#include "SireMol/moleculeid.h"

#include "SireBase/property.h"

#include "SireError/errors.h"
#include "SireFF/errors.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireCAS;

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
    else
    {
        //wait until the processor has finished
        this->_pvt_waitUntilReady();

        if ( current_state | CALCULATING_ENERGY )
        {
            //get the values of the energy and its components
            total_nrg = this->_pvt_getEnergies(nrg_components);

            //the energy no longer needs recalculating
            needs_energy_recalculation = false;
        }

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

/** Set that the energy needs to be recalculated... */
bool FFWorkerBase::needsRecalculation(bool needs_recalc)
{
    needs_energy_recalculation = needs_energy_recalculation or needs_recalc;

    return needs_energy_recalculation;
}

/** Return a copy of the forcefield being evaluated */
ForceField FFWorkerBase::forceField()
{
    //wait until the processor is idle
    this->waitUntilReady();

    return this->_pvt_forceField();
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

/** Set the property 'name' to the value 'value'

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
bool FFWorkerBase::setProperty(const QString &name, const Property &value)
{
    //wait until the processor is idle
    this->waitUntilReady();

    return needsRecalculation( this->_pvt_setProperty(name,value) );
}

/** Return the property with name 'name'

    \throw SireBase::missing_property
*/
Property FFWorkerBase::getProperty(const QString &name)
{
    //wait until the processor is idle
    this->waitUntilReady();

    return this->_pvt_getProperty(name);
}

/** Return whether or not this forcefield contains a property with name 'name' */
bool FFWorkerBase::containsProperty(const QString &name)
{
    this->waitUntilReady();
    return this->_pvt_containsProperty(name);
}

/** Return all of the properties in this forcefield */
QHash<QString,Property> FFWorkerBase::properties()
{
    this->waitUntilReady();
    return this->_pvt_properties();
}

/** Add the molecule 'molecule' to this forcefield using the
    optional parameter map to find any necessay parameters
    from properties of the atom. This will replace any
    existing copy of the atom that already exists in
    this forcefield. This returns whether or not the
    forcefield has been changed by this addition, and therefore
    whether its energy needs recalculating.

    \throw SireError::invalid_operation
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
bool FFWorkerBase::add(const PartialMolecule &molecule, const ParameterMap &map)
{
    this->waitUntilReady();

    return needsRecalculation( this->_pvt_add(molecule,map) );
}

/** Add lots of molecules

    \throw SireError::invalid_operation
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
bool FFWorkerBase::add(const Molecules &molecules, const ParameterMap &map)
{
    this->waitUntilReady();
    return needsRecalculation( this->_pvt_add(molecules,map) );
}

/** Add the molecule 'molecule' to the group 'group' in this forcefield using
    the optional parameter map to find any necessary parameters
    from properties of the molecule. This will replace any
    existing copy of the molecule that already exists in
    this forcefield. This returns whether or not the
    forcefield has been changed by this addition, and therefore
    whether its energy needs recalculating.

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
    \throw SireFF::invalid_group
*/
bool FFWorkerBase::addTo(const FFBase::Group &group,
                         const PartialMolecule &molecule, const ParameterMap &map)
{
    this->waitUntilReady();
    return needsRecalculation( this->_pvt_addTo(group,molecule,map) );
}

/** Adds lots of molecules to the group 'group'

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
    \throw SireMol::invalid_operation
    \throw SireFF::invalid_group
*/
bool FFWorkerBase::addTo(const FFBase::Group &group,
                         const Molecules &molecules, const ParameterMap &map)
{
    this->waitUntilReady();
    return needsRecalculation( this->_pvt_addTo(group,molecules,map) );
}

/** Tell the forcefield that it has to recalculate everything from
    scratch */
void FFWorkerBase::mustNowRecalculateFromScratch()
{
    this->waitUntilReady();
    this->_pvt_mustNowRecalculateFromScratch();
    needs_energy_recalculation = true;
}

/** Change the molecule 'molecule' (e.g. move it, or change its
    parameters). This does nothing if the molecule is not
    in this forcefield. Returns whether or not the forcefield
    has been changed by this change, and thus whether the
    energy needs to be recalculated. The same parameter map
    that was used when this molecule was added will be used
    to extract any necessary parameters from the molecule's
    properties

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool FFWorkerBase::change(const PartialMolecule &molecule)
{
    this->waitUntilReady();
    return needsRecalculation( this->_pvt_change(molecule) );
}

/** Change a whole load of partial molecules

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool FFWorkerBase::change(const Molecules &molecules)
{
    this->waitUntilReady();
    return needsRecalculation( this->_pvt_change(molecules) );
}

/** Remove the molecule 'molecule' from this forcefield - this
    does nothing if the molecule is not in this forcefield. This
    returns whether this has changed the forcefield (therefore
    necessitating a recalculation of the energy)

    \throw SireError::invalid_operation
*/
bool FFWorkerBase::remove(const PartialMolecule &molecule)
{
    this->waitUntilReady();
    return needsRecalculation( this->_pvt_remove(molecule) );
}

/** Remove a whole load of molecules */
bool FFWorkerBase::remove(const Molecules &molecules)
{
    this->waitUntilReady();
    return needsRecalculation( this->_pvt_remove(molecules) );
}

/** Remove the molecule 'molecule' from the group 'group'
    from this forcefield - this does nothing if the molecule
    is not in this forcefield. This returns whether or not
    this has changed the forcefield (therefore necessitating
    a recalculation of the energy)

    \throw SireFF::invalid_group
*/
bool FFWorkerBase::removeFrom(const FFBase::Group &group,
                              const PartialMolecule &molecule)
{
    this->waitUntilReady();
    return needsRecalculation( this->_pvt_removeFrom(group,molecule) );
}

/** Remove lots of molecules from the group 'group'

    \throw SireFF::invalid_group
*/
bool FFWorkerBase::removeFrom(const FFBase::Group &group,
                              const Molecules &molecules)
{
    this->waitUntilReady();
    return needsRecalculation( this->_pvt_removeFrom(group,molecules) );
}

/** Return whether this forcefield contains a complete copy of
    any version of the partial molecule 'molecule' */
bool FFWorkerBase::contains(const PartialMolecule &molecule)
{
    this->waitUntilReady();
    return this->_pvt_contains(molecule);
}

/** Return whether this forcefield contains a complete copy of
    any version of the partial molecule 'molecule' in the group 'group'

    \throw SireFF::invalid_group
*/
bool FFWorkerBase::contains(const PartialMolecule &molecule,
                            const FFBase::Group &group)
{
    this->waitUntilReady();
    return this->_pvt_contains(molecule, group);
}

/** Return whether or not this forcefield contains *any part* of
    any version of the molecule with ID 'molid' */
bool FFWorkerBase::refersTo(MoleculeID molid)
{
    this->waitUntilReady();
    return this->_pvt_refersTo(molid);
}

/** Return whether or not the group 'group' in this forcefield
    contains *any part* of any version of the molecule with ID
    'molid'

    \throw SireFF::invalid_group
*/
bool FFWorkerBase::refersTo(MoleculeID molid, const FFBase::Group &group)
{
    this->waitUntilReady();
    return this->_pvt_refersTo(molid, group);
}

/** Return all of the groups that refer to the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
QSet<FFBase::Group> FFWorkerBase::groupsReferringTo(MoleculeID molid)
{
    this->waitUntilReady();
    return this->_pvt_groupsReferringTo(molid);
}

/** Return the set of all of the ID numbers of all of the
    molecules that are referred to by this forcefield
    (i.e. all molecules that have at least some part
    in this forcefield) */
QSet<MoleculeID> FFWorkerBase::moleculeIDs()
{
    this->waitUntilReady();
    return this->_pvt_moleculeIDs();
}

/** Return the set of all of the ID numbers of all of the
    molecules that are referred to by group 'group' in
    this forcefield (i.e. all molecules that have at least
    some part in this group in this forcefield)

    \throw SireFF::invalid_group
*/
QSet<MoleculeID> FFWorkerBase::moleculeIDs(const FFBase::Group &group)
{
    this->waitUntilReady();
    return this->_pvt_moleculeIDs(group);
}

/** Return the copy of the molecule in this forcefield that
    has the ID == molid

    \throw SireMol::missing_molecule
*/
PartialMolecule FFWorkerBase::molecule(MoleculeID molid)
{
    this->waitUntilReady();
    return this->_pvt_molecule(molid);
}

/** Return a copy of the molecule with ID == molid in the group 'group'

    \throw SireMol::missing_molecule
    \throw SireFF::missing_group
*/
PartialMolecule FFWorkerBase::molecule(MoleculeID molid, const FFBase::Group &group)
{
    this->waitUntilReady();
    return this->_pvt_molecule(molid, group);
}

/** Return copies of all of the molecules that are in this forcefield */
Molecules FFWorkerBase::molecules()
{
    this->waitUntilReady();
    return this->_pvt_molecules();
}

/** Return copies of all of the molecules that are in the group 'group'

    \throw SireFF::missing_group
*/
Molecules FFWorkerBase::molecules(const FFBase::Group &group)
{
    this->waitUntilReady();
    return this->_pvt_molecules(group);
}

/** Return copies of the molecules in this forcefield whose IDs
    are in 'molids'

    \throw SireMol::missing_molecule
*/
Molecules FFWorkerBase::molecules(const QSet<MoleculeID> &molids)
{
    this->waitUntilReady();
    return this->_pvt_molecules(molids);
}

/** Return the contents of the group 'group' in this forcefield

    \throw SireFF::invalid_group
*/
Molecules FFWorkerBase::contents(const FFBase::Group group)
{
    this->waitUntilReady();
    return this->_pvt_contents(group);
}

/** Return all of the molecules (and parts of molecules) that
    are in this forcefield */
Molecules FFWorkerBase::contents()
{
    this->waitUntilReady();
    return this->_pvt_contents();
}

/** Return whether or not the forcefield is dirty (the energy
    needs to be recalculated) */
bool FFWorkerBase::isDirty()
{
    this->waitUntilReady();
    return this->_pvt_isDirty();
}

/** Return whether or not the forcefield is clean (the energy
    does not need to be recalculated) */
bool FFWorkerBase::isClean()
{
    this->waitUntilReady();
    return this->_pvt_isClean();
}

/** Return the ID number of the forcefield */
ForceFieldID FFWorkerBase::ID()
{
    this->waitUntilReady();
    return this->ID();
}

/** Return the version number of the forcefield */
Version FFWorkerBase::version()
{
    this->waitUntilReady();
    return this->version();
}

/** Assert that this forcefield contains the energy component 'component'

    \throw SireFF::missing_component
*/
void FFWorkerBase::assertContains(const FFComponent &component)
{
    if (not nrg_components.values().contains(component.ID()))
    {
        this->waitUntilReady();
        this->_pvt_assertContains(component);
    }
}
