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

#include "SireBase/property.h"

#include "SireMol/molecule.h"
#include "SireMol/partialmolecule.h"

#include "ffcalculator.h"

#include "SireError/errors.h"

using namespace SireFF;
using namespace SireMol;

/////////
///////// Implementation of FFCalculatorBase
/////////

/** Constructor */
FFCalculatorBase::FFCalculatorBase() : boost::noncopyable()
{}

/** Destructor */
FFCalculatorBase::~FFCalculatorBase()
{}

/////////
///////// Implementation of FFCalculator
/////////

/** Constructor, using a null forcefield */
FFCalculator::FFCalculator()
             : FFCalculatorBase(), total_nrg(0)
{}

/** Constructor, specifying the forcefield */
FFCalculator::FFCalculator(const ForceField &forcefield)
             : FFCalculatorBase(), ffield(forcefield), total_nrg(0)
{
    if (not ffield.isDirty())
    {
        nrg_components = ffield.energies();
        total_nrg = nrg_components.value( ffield.components().total() );
    }
}

/** Destructor */
FFCalculator::~FFCalculator()
{}

/** Return the total energy of the forcefield, and populate
    'components' with all of the energy components. */
double FFCalculator::getEnergies(Values &components)
{
    components = nrg_components;

    return total_nrg;
}

/** Recalculate the energy of the forcefield */
void FFCalculator::calculateEnergy()
{
    nrg_components = ffield.energies();
    total_nrg = nrg_components.value(ffield.components().total());
}
             
/** Set the forcefield to be calculated and return whether its
    energy needs to be reevaluated. */
bool FFCalculator::setForceField(const ForceField &forcefield)
{
    ffield = forcefield;

    if (ffield.isDirty())
        return true;
    else
    {
        nrg_components = ffield.energies();
        total_nrg = nrg_components.value(ffield.components().total());
        return false;
    }
}

/** Return a copy of the forcefield that is being evaluated */
ForceField FFCalculator::forcefield() const
{
    return ffield;
}

/** Set the property 'name' to the value 'value' 

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
bool FFCalculator::setProperty(const QString &name, const Property &value)
{
    return ffield.setProperty(name, value);
}

/** Return the property with the name 'name'

    \throw SireError::missing_property
*/
Property FFCalculator::getProperty(const QString &name)
{
    return ffield.getProperty(name);
}

/** Return whether or not this forcefield contains a property
    called 'name' */
bool FFCalculator::containsProperty(const QString &name)
{
    return ffield.containsProperty(name);
}

/** Return all of the properties of this forcefield */
QHash<QString,Property> FFCalculator::properties()
{
    return ffield.properties();
}

/** Add the molecule 'molecule' to the forcefield

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
bool FFCalculator::add(const PartialMolecule &molecule, const ParameterMap &map)
{
    return ffield.add(molecule, map);
}

/** Add lots of molecules.

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
bool FFCalculator::add(const QList<PartialMolecule> &molecules,
                       const ParameterMap &map)
{
    return ffield.add(molecules, map);
}

/** Add the molecule 'molecule' to the group 'group'

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
bool FFCalculator::addTo(const FFBase::Group &group,
                         const PartialMolecule &molecule, const ParameterMap &map)
{
    return ffield.addTo(group, molecule, map);
}
                         
/** Add lots of molecules to the group 'group'

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
bool FFCalculator::addTo(const FFBase::Group &group,
                         const QList<PartialMolecule> &molecules,
                         const ParameterMap &map)
{
    return ffield.addTo(group, molecules, map);
}

/** Change the molecule 'molecule'

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
bool FFCalculator::change(const PartialMolecule &molecule)
{
    return ffield.change(molecule);
}

/** Change lots of molecules.

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
bool FFCalculator::change(const QHash<MoleculeID,PartialMolecule> &molecules)
{
    return ffield.change(molecules);
}

/** Change lots of molecules.

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
bool FFCalculator::change(const QList<PartialMolecule> &molecules)
{
    return ffield.change(molecules);
}

/** Remove the molecule 'molecule' */
bool FFCalculator::remove(const PartialMolecule &molecule)
{
    return ffield.remove(molecule);
}

/** Remove lots of molecules */
bool FFCalculator::remove(const QList<PartialMolecule> &molecules)
{
    return ffield.remove(molecules);
}

/** Remove the molecule 'molecule' from the group 'group'

    \throw SireFF::missing_group
*/
bool FFCalculator::removeFrom(const FFBase::Group &group,
                              const PartialMolecule &molecule)
{
    return ffield.removeFrom(group, molecule);
}

/** Remove lots of molecules from the group 'group'

    \throw SireFF::missing_group
*/
bool FFCalculator::removeFrom(const FFBase::Group &group,
                              const QList<PartialMolecule> &molecules)
{
    return ffield.removeFrom(group, molecules);
}

/** Return whether or not the forcefield contains all of the atoms
    of any version of the molecule 'molecule' */
bool FFCalculator::contains(const PartialMolecule &molecule)
{
    return ffield.contains(molecule);
}

/** Return whether or not the forcefield contains any version of
    the atoms in the molecule 'molecule' in the group 'group'
    
    \throw SireFF::missing_group
*/
bool FFCalculator::contains(const PartialMolecule &molecule,
                            const FFBase::Group &group)
{
    return ffield.contains(molecule, group);
}

/** Return whether or not this forcefield refers to the molecule with 
    ID == molid */
bool FFCalculator::refersTo(MoleculeID molid)
{
    return ffield.refersTo(molid);
}

/** Return whether or not this forcefield refers to the molecule with
    ID == molid in the group 'group'
    
    \throw SireFF::missing_group
*/
bool FFCalculator::refersTo(MoleculeID molid, const FFBase::Group &group)
{
    return ffield.refersTo(molid, group);
}

/** Return all of the groups that refer to the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
QSet<FFBase::Group> FFCalculator::groupsReferringTo(MoleculeID molid)
{
    return ffield.groupsReferringTo(molid);
}

/** Return the set of IDs of all molecules that are referred to by this
    forcefield */
QSet<MoleculeID> FFCalculator::moleculeIDs()
{
    return ffield.moleculeIDs();
}

/** Return the set of IDs of all molecules that are referred to by 
    the group 'group' in this forcefield 
    
    \throw SireFF::missing_group
*/
QSet<MoleculeID> FFCalculator::moleculeIDs(const FFBase::Group &group)
{
    return ffield.moleculeIDs(group);
}

/** Return the molecule in this forcefield with ID == molid

    \throw SireMol::missing_molecule
*/
PartialMolecule FFCalculator::molecule(MoleculeID molid)
{
    return ffield.molecule(molid);
}

/** Return the molecule with ID == molid in the group 'group'

    \throw SireMol::missing_molecule
    \throw SireFF::missing_group
*/
PartialMolecule FFCalculator::molecule(MoleculeID molid, const FFBase::Group &group)
{
    return ffield.molecule(molid, group);
}

/** Return all of the molecules in this forcefield */
QHash<MoleculeID,PartialMolecule> FFCalculator::molecules()
{
    return ffield.molecules();
}

/** Return all of the molecules in the group 'group'

    \throw SireFF::missing_group
*/
QHash<MoleculeID,PartialMolecule> FFCalculator::molecules(const FFBase::Group &group)
{
    return ffield.molecules(group);
}

/** Return all of the molecules whose IDs are in 'molids'

    \throw SireMol::missing_molecule
*/
QHash<MoleculeID,PartialMolecule> 
FFCalculator::molecules(const QSet<MoleculeID> &molids)
{
    return ffield.molecules(molids);
}

/** Return the complete contents of the group 'group' in this forcefield 

    \throw SireFF::missing_group
*/
QHash<MoleculeID,PartialMolecule> FFCalculator::contents(const FFBase::Group &group)
{
    return ffield.contents(group);
}

/** Return the complete contents of this forcefield */
QHash<MoleculeID,PartialMolecule> FFCalculator::contents()
{
    return ffield.contents();
}

/** Return whether or not the forcefield is dirty */
bool FFCalculator::isDirty()
{
    return ffield.isDirty();
}

/** Return whether or not the forcefield is clean */
bool FFCalculator::isClean()
{
    return ffield.isClean();
}

/** Return the ID number of the forcefield */
ForceFieldID FFCalculator::ID()
{
    return ffield.ID();
}

/** Return the version number of the forcefield */
Version FFCalculator::version()
{
    return ffield.version();
}

/** Assert that the forcefield contains the component 'component'

    \throw SireFF::missing_component
*/
void FFCalculator::assertContains(const FFComponent &component)
{
    ffield.assertContains(component);
}
