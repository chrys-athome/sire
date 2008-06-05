/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIRESYSTEM_QUERYSYSTEM_H
#define SIRESYSTEM_QUERYSYSTEM_H

#include <boost/noncopyable.hpp>

#include "SireFF/ffbase.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
class Version;
class Property;
}

namespace SireCAS
{
class Function;
class Expression;
class Values;
}

namespace SireMol
{
class PartialMolecule;
class MoleculeGroup;
class MoleculeGroups;
class MoleculeGroupID;
}

namespace SireFF
{
class FFComponent;
class FFExpression;
class ForceField;
class ForceFieldID;
class ForceFieldsBase;
}

namespace SireSystem
{

class SystemData;
class System;
class SystemID;
class CheckPoint;
class SystemMonitors;

using SireCAS::Function;
using SireCAS::Expression;
using SireCAS::Values;

using SireMol::PartialMolecule;
using SireMol::MoleculeGroup;
using SireMol::MoleculeGroups;
using SireMol::MoleculeGroupID;
using SireMol::MoleculeID;

using SireFF::ForceField;
using SireFF::ForceFieldsBase;
using SireFF::ForceFieldID;
using SireFF::FFComponent;
using SireFF::FFExpression;
using SireFF::FFBase;

using SireBase::Property;
using SireBase::Version;

class SimSystem;

/** This class holds a system that is being actively
    simulated, but that can only be queried
    (not changed). This is the interface used
    by SystemMonitors when it queries a running
    simulation to extract the current values
    of the properties that are being monitored.

    While System is an implicitly-shared class,
    SimSystem is designed to be a non-copyable
    class (as a Simulation is occuring only once).

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT QuerySystem : public boost::noncopyable
{

friend class SimSystem; //so can see private references

public:
    virtual ~QuerySystem();

    CheckPoint checkPoint();

    SystemID ID() const;
    const Version& version() const;

    const SystemData& info() const;
    const ForceFieldsBase& forceFields() const;
    const SystemMonitors& monitors() const;

    const MoleculeGroups& groups() const;

    ForceField forceField(ForceFieldID ffid);
    MoleculeGroup group(MoleculeGroupID id) const;

    double energy();
    double energy(const Function &component);
    double energy(const FFComponent &component);

    double energy(const Expression &expression);
    double energy(const FFExpression &expression);

    Values energies();
    Values energies(const QSet<FFComponent> &components);
    Values energies(const QSet<Function> &components);

    QHash<ForceFieldID,Property> getProperty(const QString &name);

    Property getProperty(ForceFieldID ffid, const QString &name);
    QHash<ForceFieldID,Property> getProperty(const QSet<ForceFieldID> &ffids,
                                            const QString &name);

    bool containsProperty(const QString &name);
    bool containsProperty(ForceFieldID ffid, const QString &name);
    QSet<ForceFieldID> forceFieldsWithProperty(const QString &name);

    QHash< QString,QHash<ForceFieldID,Property> > properties();

protected:
    QuerySystem();
    
    QuerySystem(SystemData &sysdata,
                ForceFieldsBase &ffields,
                SystemMonitors &monitors);

    /** Reference to the data of the System being simulated */
    SystemData &sysdata;

    /** Reference to the forcefields that are used to
        calculate the energy / forces */
    ForceFieldsBase &ffields;

    /** Reference to the monitors that are used to monitor
        and collect statistics about the simulation */
    SystemMonitors &sysmonitors;
};

/** Return the metainfo that describes this system */
inline const SystemData& QuerySystem::info() const
{
    return sysdata;
}

/** Return the forcefields that describe the potential energy
    surface */
inline const ForceFieldsBase& QuerySystem::forceFields() const
{
    return ffields;
}

/** Return the monitors that are used to monitor properties
    of the system */
inline const SystemMonitors& QuerySystem::monitors() const
{
    return sysmonitors;
}

}

SIRE_END_HEADER

#endif
