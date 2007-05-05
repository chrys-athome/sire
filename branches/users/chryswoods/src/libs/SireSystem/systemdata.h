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

#ifndef SIRESYSTEM_SYSTEMDATA_H
#define SIRESYSTEM_SYSTEMDATA_H

#include <QVector>

#include "systemid.h"

#include "SireBase/idmajminversion.h"

#include "SireMol/moleculegroups.h"

#include "SireVol/space.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class SystemData;
}

QDataStream& operator<<(QDataStream&, const SireSystem::SystemData&);
QDataStream& operator>>(QDataStream&, SireSystem::SystemData&);

namespace SireCAS
{
class Values;
}

namespace SireMol
{
class Molecule;
class Molecules;
class PartialMolecule;
class MoleculeID;
}

namespace SireFF
{
class ForceFields;
}

namespace SireBase
{
class Property;
}

namespace SireSystem
{

using SireBase::IDMajMinVersion;
using SireBase::Version;
using SireBase::Property;

using SireVol::Space;

using SireFF::ForceFields;

using SireMol::Molecule;
using SireMol::Molecules;
using SireMol::PartialMolecule;
using SireMol::MoleculeID;
using SireMol::MoleculeGroup;
using SireMol::MoleculeGroups;
using SireMol::MoleculeGroupID;

/** This class provides all of the metadata about a System
    (to be honest, all data except for the forcefields).

    This allows a system to be split easily into
    forcefield and non-forcefield parts, which is important
    when used with ClusterSimSystem.

    This class is not intended to be used on its own
    (it is virtual, with all protected interface)
    and must be used by a derived class.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SystemData
{

friend QDataStream& ::operator<<(QDataStream&, const SystemData&);
friend QDataStream& ::operator>>(QDataStream&, SystemData&);

public:
    SystemData();

    SystemData(const QString &name);

    SystemData(const SystemData &other);

    virtual ~SystemData();

    SystemData& operator=(const SystemData &other);

    bool operator==(const SystemData &other) const;
    bool operator!=(const SystemData &other) const;

    const QString& name() const;
    SystemID ID() const;
    const Version& version() const;

    const Space& space() const;

    const MoleculeGroup& group(MoleculeGroupID id) const;
    const MoleculeGroups& groups() const;

    bool refersTo(MoleculeGroupID groupid) const;

    void add(const MoleculeGroup &group);
    void add(const MoleculeGroups &groups);

    void remove(MoleculeGroupID groupid);
    void remove(const MoleculeGroup &group);

    PartialMolecule mapIntoSystemSpace(const PartialMolecule &molecule) const;

    Molecules mapIntoSystemSpace(const Molecules &molecules) const;

    PartialMolecule change(const PartialMolecule &molecule);

    Molecules change(const Molecules &molecules);

    PartialMolecule add(const PartialMolecule &molecule,
                        const QSet<MoleculeGroupID> &groupids);

    Molecules add(const Molecules &molecules,
                  const QSet<MoleculeGroupID> &groupids);

    void remove(const PartialMolecule &molecule,
                const QSet<MoleculeGroupID> &groupids);

    void remove(const Molecules &molecules,
                const QSet<MoleculeGroupID> &groupids);

    void remove(const PartialMolecule &molecule);
    void remove(const Molecules &molecules);

    void incrementMinorVersion();
    void incrementMajorVersion();

private:

    /** All of the MoleculeGroups in the System */
    MoleculeGroups molgroups;

    /** The Space of the System. All molecules in the System
        will be mapped to lie within this Space when they are
        moved - in addition any moves that change the space
        of the system must use this space (e.g. volume moves). */
    Space sys_space;

    /** The list of constraints, indexed by the molecule IDs of the
        molecules to which they must be applied */
    //QHash<MoleculeID, MoleculeConstraint> mol_constraints;

    /** The name of the system */
    QString nme;

    /** The ID and version of this system */
    IDMajMinVersion id_and_version;
};

/** Return the name of this System */
inline const QString& SystemData::name() const
{
    return nme;
}

/** Return the ID number of this System */
inline SystemID SystemData::ID() const
{
    return id_and_version.ID();
}

/** Return the version number of this System */
inline const Version& SystemData::version() const
{
    return id_and_version.version();
}

/** Increment the major version number of the System. The major number
    is incremented if forcefields or groups are added or removed from
    the system. The minor version number is incremented if anything
    else in the system is changed. */
inline void SystemData::incrementMajorVersion()
{
    id_and_version.incrementMajor();
}

/** Increment the minor version number. This is incremented if any of the
    molecules or properties in the system are changed. */
inline void SystemData::incrementMinorVersion()
{
    id_and_version.incrementMinor();
}

/** Return the space into which the coordinates of all molecules will
    be mapped. */
inline const Space& SystemData::space() const
{
    return sys_space;
}

/** Return all of the Molecule groups in this System */
inline const MoleculeGroups& SystemData::groups() const
{
    return molgroups;
}

}

SIRE_END_HEADER

#endif
