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

#include "querysystem.h"

#include "checkpoint.h"
#include "systemdata.h"
#include "systemmonitors.h"

#include "SireBase/property.h"
#include "SireFF/forcefields.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireFF;
using namespace SireCAS;
using namespace SireBase;

static SystemData null_sysdata;
static ForceFields null_ffields;
static SystemMonitors null_monitors;

/** Protected null constructor - the results system is invalid
    and dangerous to use!!! */
QuerySystem::QuerySystem()
            : sysdata(null_sysdata),
              ffields(null_ffields),
              sysmonitors(null_monitors)
{}

/** Protected constructor used to construct the QuerySystem
    from the passed components of a System */
QuerySystem::QuerySystem(SystemData &system_data,
                         ForceFieldsBase &forcefields,
                         SystemMonitors &monitors)
            : sysdata(system_data),
              ffields(forcefields),
              sysmonitors(monitors)
{}

/** Destructor */
QuerySystem::~QuerySystem()
{}

/** Checkpoint this System */
CheckPoint QuerySystem::checkPoint()
{
    return CheckPoint(*this);
}

/** Return the ID of the system */
SystemID QuerySystem::ID() const
{
    return sysdata.ID();
}

/** Return the version number of the system */
const Version& QuerySystem::version() const
{
    return sysdata.version();
}

/** Return the molecule groups in the system */
const MoleculeGroups& QuerySystem::groups() const
{
    return sysdata.groups();
}

/** Return a copy of the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
*/
ForceField QuerySystem::forceField(ForceFieldID ffid)
{
    return ffields.forceField(ffid);
}

/** Return a copy of the MoleculeGroup with ID == id

    \throw SireMol::missing_group
*/
MoleculeGroup QuerySystem::group(MoleculeGroupID id) const
{
    return sysdata.group(id);
}

/** Return the current total energy of the system */
double QuerySystem::energy()
{
    return ffields.energy();
}

/** Return the current total energy of the component 'component'

    \throw SireFF::missing_component
*/
double QuerySystem::energy(const Function &component)
{
    return ffields.energy(component);
}

/** Return the current total energy of the component 'component'

    \throw SireFF::missing_component
*/
double QuerySystem::energy(const FFComponent &component)
{
    return ffields.energy(component);
}

/** Return the energy of the expression 'expression'. This expression
    does not need to have been added to this forcefield, though it must be
    able to resolve all of its dependencies from components in this forcefield

    \throw SireError::dependency_error
*/
double QuerySystem::energy(const Expression &expression)
{
    return ffields.energy(expression);
}

/** Return the energy of the expression 'expression'. This expression
    does not need to have been added to this forcefield, though it must be
    able to resolve all of its dependencies from components in this forcefield

    \throw SireError::dependency_error
*/
double QuerySystem::energy(const FFExpression &expression)
{
    return ffields.energy(expression);
}

/** Return the values of all of the energy components in this system */
Values QuerySystem::energies()
{
    return ffields.energies();
}

/** Return the values of all of the energy components in this system
    that are listed in 'components'

    \throw SireFF::missing_component
*/
Values QuerySystem::energies(const QSet<FFComponent> &components)
{
    return ffields.energies(components);
}

/** Return the values of all of the energy components in this system
    that are listed in 'components'

    \throw SireFF::missing_component
*/
Values QuerySystem::energies(const QSet<Function> &components)
{
    return ffields.energies(components);
}

/** Return the value of the property called 'name' in all of the forcefields
    that contain that property

    \throw SireBase::missing_property
*/
QHash<ForceFieldID,Property> QuerySystem::getProperty(const QString &name)
{
    return ffields.getProperty(name);
}

/** Return the value of the property 'name' in the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
    \throw SireBase::missing_property
*/
Property QuerySystem::getProperty(ForceFieldID ffid, const QString &name)
{
    return ffields.getProperty(ffid, name);
}

/** Return the value of the property 'name' in all of the forcefields
    whose IDs are in 'ffids'

    \throw SireFF::missing_forcefield
    \throw SireBase::missing_property
*/
QHash<ForceFieldID,Property>
QuerySystem::getProperty(const QSet<ForceFieldID> &ffids, const QString &name)
{
    return ffields.getProperty(ffids, name);
}

/** Return whether or not any of the forcefields contain a property
    called 'name' */
bool QuerySystem::containsProperty(const QString &name)
{
    return ffields.containsProperty(name);
}

/** Return whether or not the forcefield with ID == ffid contains the
    property called 'name'

    \throw SireFF::missing_forcefield
*/
bool QuerySystem::containsProperty(ForceFieldID ffid, const QString &name)
{
    return ffields.containsProperty(ffid, name);
}

/** Return the IDs of all forcefields that contain a property with
    the name 'name'

    \throw SireBase::missing_property
*/
QSet<ForceFieldID> QuerySystem::forceFieldsWithProperty(const QString &name)
{
    return ffields.forceFieldsWithProperty(name);
}

/** Return all of the properties of all of the forcefields, indexed
    by name, then forcefield ID */
QHash< QString,QHash<ForceFieldID,Property> > QuerySystem::properties()
{
    return ffields.properties();
}
