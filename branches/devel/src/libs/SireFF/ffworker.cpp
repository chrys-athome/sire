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
#include "SireMol/partialmolecule.h"
#include "SireMol/residue.h"
#include "SireMol/moleculeid.h"

#include "SireBase/property.h"

#include "SireError/errors.h"
#include "SireFF/errors.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireCAS;

/////////
///////// Implementation of FFLocalWorker
/////////

/** Construct a local worker that uses the passed calculator
    to evaluate the forcefield. This takes over ownership
    of the calculator and will delete it once it has finished
    with it.
     
    \throw SireError::nullptr_error
*/
FFLocalWorker::FFLocalWorker(FFCalculatorBase *ffcalc)
              : FFWorkerBase(), ffcalculator(ffcalc)
{
    if (not ffcalc)
        throw SireError::nullptr_error( QObject::tr(
                "Cannot create an FFLocalWorker with a null FFCalculator!"),
                    CODELOC );
}

/** Destructor */
FFLocalWorker::~FFLocalWorker()
{}

/** Return a copy of the forcefield being evaluated */
ForceField FFLocalWorker::_pvt_forceField()
{
    return ffcalculator->forceField();
}

/** Set the forcefield to be evaluated on this processor

    \throw SireError::incompatible_error
*/
bool FFLocalWorker::_pvt_setForceField(const ForceField &forcefield)
{
    return ffcalculator->setForceField(forcefield);
}

/** Return all of the energy components of the forcefield in 'components',
    and also additionally return the total energy of the forcefield */
double FFLocalWorker::_pvt_getEnergies(Values &components)
{
    return ffcalculator->getEnergies(components);
}

/** Set the property 'name' to the value 'value'

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
bool FFLocalWorker::_pvt_setProperty(const QString &name, const Property &value)
{
    return ffcalculator->setProperty(name,value);
}

/** Return the property with name 'name'

    \throw SireBase::missing_property
*/    
Property FFLocalWorker::_pvt_getProperty(const QString &name)
{
    return ffcalculator->getProperty(name);
}

/** Return whether or not this forcefield contains a property with name 'name' */
bool FFLocalWorker::_pvt_containsProperty(const QString &name)
{
    return ffcalculator->containsProperty(name);
}

/** Return all of the properties in this forcefield */
QHash<QString,Property> FFLocalWorker::_pvt_properties()
{
    return ffcalculator->properties();
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
bool FFLocalWorker::_pvt_add(const PartialMolecule &molecule, 
                             const ParameterMap &map)
{
    return ffcalculator->add(molecule,map);
}

/** Add lots of molecules 

    \throw SireError::invalid_operation
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
bool FFLocalWorker::_pvt_add(const QList<PartialMolecule> &molecules, 
                             const ParameterMap &map)
{
    return ffcalculator->add(molecules,map);
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
bool FFLocalWorker::_pvt_addTo(const FFBase::Group &group, 
                               const PartialMolecule &molecule,
                               const ParameterMap &map)
{
    return ffcalculator->addTo(group,molecule,map);
}

/** Adds lots of molecules to the group 'group' 

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
    \throw SireMol::invalid_operation
    \throw SireFF::invalid_group
*/
bool FFLocalWorker::_pvt_addTo(const FFBase::Group &group,
                               const QList<PartialMolecule> &molecules,
                               const ParameterMap &map)
{
    return ffcalculator->addTo(group,molecules,map);
}

/** Tell the forcefield that it has to recalculate everything from
    scratch */
void FFLocalWorker::_pvt_mustNowRecalculateFromScratch()
{
    ffcalculator->mustNowRecalculateFromScratch();
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
bool FFLocalWorker::_pvt_change(const PartialMolecule &molecule)
{
    return ffcalculator->change(molecule);
}

/** Change a whole load of partial molecules

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool FFLocalWorker::_pvt_change(const QHash<MoleculeID,PartialMolecule> &molecules)
{
    return ffcalculator->change(molecules);
}

/** Change a whole load of partial molecules

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool FFLocalWorker::_pvt_change(const QList<PartialMolecule> &molecules)
{
    return ffcalculator->change(molecules);
}

/** Remove the molecule 'molecule' from this forcefield - this
    does nothing if the molecule is not in this forcefield. This
    returns whether this has changed the forcefield (therefore
    necessitating a recalculation of the energy)

    \throw SireError::invalid_operation
*/
bool FFLocalWorker::_pvt_remove(const PartialMolecule &molecule)
{
    return ffcalculator->remove(molecule);
}

/** Remove a whole load of molecules */
bool FFLocalWorker::_pvt_remove(const QList<PartialMolecule> &molecules)
{
    return ffcalculator->remove(molecules);
}

/** Remove the molecule 'molecule' from the group 'group'
    from this forcefield - this does nothing if the molecule
    is not in this forcefield. This returns whether or not
    this has changed the forcefield (therefore necessitating
    a recalculation of the energy)

    \throw SireFF::invalid_group
*/
bool FFLocalWorker::_pvt_removeFrom(const FFBase::Group &group,
                                    const PartialMolecule &molecule)
{
    return ffcalculator->removeFrom(group,molecule);
}

/** Remove lots of molecules from the group 'group' 

    \throw SireFF::invalid_group
*/
bool FFLocalWorker::_pvt_removeFrom(const FFBase::Group &group,
                                    const QList<PartialMolecule> &molecules)
{
    return ffcalculator->removeFrom(group,molecules);
}
  
/** Return whether this forcefield contains a complete copy of
    any version of the partial molecule 'molecule' */
bool FFLocalWorker::_pvt_contains(const PartialMolecule &molecule)
{
    return ffcalculator->contains(molecule);
}

/** Return whether this forcefield contains a complete copy of
    any version of the partial molecule 'molecule' in the group 'group' 
    
    \throw SireFF::invalid_group
*/
bool FFLocalWorker::_pvt_contains(const PartialMolecule &molecule,
                                  const FFBase::Group &group)
{
    return ffcalculator->contains(molecule,group);
}

/** Return whether or not this forcefield contains *any part* of
    any version of the molecule with ID 'molid' */
bool FFLocalWorker::_pvt_refersTo(MoleculeID molid)
{
    return ffcalculator->refersTo(molid);
}

/** Return whether or not the group 'group' in this forcefield
    contains *any part* of any version of the molecule with ID
    'molid' 
    
    \throw SireFF::invalid_group
*/
bool FFLocalWorker::_pvt_refersTo(MoleculeID molid, const FFBase::Group &group)
{
    return ffcalculator->refersTo(molid,group);
}

/** Return all of the groups that refer to the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
QSet<FFBase::Group> FFLocalWorker::_pvt_groupsReferringTo(MoleculeID molid)
{
    return ffcalculator->groupsReferringTo(molid);
}

/** Return the set of all of the ID numbers of all of the
    molecules that are referred to by this forcefield
    (i.e. all molecules that have at least some part
    in this forcefield) */
QSet<MoleculeID> FFLocalWorker::_pvt_moleculeIDs()
{
    return ffcalculator->moleculeIDs();
}

/** Return the set of all of the ID numbers of all of the
    molecules that are referred to by group 'group' in
    this forcefield (i.e. all molecules that have at least
    some part in this group in this forcefield) 
    
    \throw SireFF::invalid_group
*/
QSet<MoleculeID> FFLocalWorker::_pvt_moleculeIDs(const FFBase::Group &group)
{
    return ffcalculator->moleculeIDs(group);
}

/** Return the copy of the molecule in this forcefield that
    has the ID == molid

    \throw SireMol::missing_molecule
*/
PartialMolecule FFLocalWorker::_pvt_molecule(MoleculeID molid)
{
    return ffcalculator->molecule(molid);
}

/** Return a copy of the molecule with ID == molid in the group 'group'

    \throw SireMol::missing_molecule
    \throw SireFF::missing_group
*/
PartialMolecule FFLocalWorker::_pvt_molecule(MoleculeID molid, 
                                             const FFBase::Group &group)
{
    return ffcalculator->molecule(molid,group);
}

/** Return copies of all of the molecules that are in this forcefield */
QHash<MoleculeID,PartialMolecule> FFLocalWorker::_pvt_molecules()
{
    return ffcalculator->molecules();
}

/** Return copies of all of the molecules that are in the group 'group'

    \throw SireFF::missing_group
*/
QHash<MoleculeID,PartialMolecule> 
FFLocalWorker::_pvt_molecules(const FFBase::Group &group)
{
    return ffcalculator->molecules(group);
}

/** Return copies of the molecules in this forcefield whose IDs 
    are in 'molids'
    
    \throw SireMol::missing_molecule
*/
QHash<MoleculeID,PartialMolecule> 
FFLocalWorker::_pvt_molecules(const QSet<MoleculeID> &molids)
{
    return ffcalculator->molecules(molids);
}

/** Return the contents of the group 'group' in this forcefield 

    \throw SireFF::invalid_group
*/
QHash<MoleculeID,PartialMolecule> 
FFLocalWorker::_pvt_contents(const FFBase::Group &group)
{
    return ffcalculator->contents(group);
}

/** Return all of the molecules (and parts of molecules) that
    are in this forcefield */
QHash<MoleculeID,PartialMolecule> FFLocalWorker::_pvt_contents()
{
    return ffcalculator->contents();
}

/** Return whether or not the forcefield is dirty (the energy
    needs to be recalculated) */
bool FFLocalWorker::_pvt_isDirty()
{
    return ffcalculator->isDirty();
}

/** Return whether or not the forcefield is clean (the energy
    does not need to be recalculated) */
bool FFLocalWorker::_pvt_isClean()
{
    return ffcalculator->isClean();
}

/** Return the ID number of the forcefield */
ForceFieldID FFLocalWorker::_pvt_ID()
{
    return ffcalculator->ID();
}

/** Return the version number of the forcefield */
Version FFLocalWorker::_pvt_version()
{
    return ffcalculator->version();
}

/** Assert that this forcefield contains the energy component 'component'

    \throw SireFF::missing_component
*/
void FFLocalWorker::_pvt_assertContains(const FFComponent &component)
{
    return ffcalculator->assertContains(component);
}

/////////
///////// Implementation of FFWorker
/////////

/** Constructor, specifying the FFCalculator to use */
FFWorker::FFWorker(FFCalculatorBase *calculator)
         : FFLocalWorker(calculator), WorkerBase()
{}

/** Destructor */
FFWorker::~FFWorker()
{}

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
