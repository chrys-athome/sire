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
class MoleculeID;
class Residue;
class NewAtom;
class Property;
}

namespace SireSystem
{

using SireBase::IDMajMinVersion;
using SireBase::Version;

using SireVol::Space;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::NewAtom;
using SireMol::Property;
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

    bool operator==(const SystemData &other) const;
    bool operator!=(const SystemData &other) const;
    
    const QString& name() const;
    SystemID ID() const;
    const Version& version() const;

    void add(const MoleculeGroup &group);
    
    bool contains(const Molecule &molecule) const;
    bool contains(MoleculeID molid) const;
    
    void remove(MoleculeGroupID groupid);
    void remove(const MoleculeGroup &group);

    //void add(const MoleculeConstraint &constraint);
    
    //void remove(const MoleculeConstraint &constraint);
    //void removeConstraint(const Molecule &molecule);
    //void removeConstraint(MoleculeID molid);

    QHash<MoleculeID,Molecule> applyConstraints(const Molecule &molecule);

    const MoleculeGroup& group(MoleculeGroupID id) const;
    const MoleculeGroups& groups() const;

    QHash<MoleculeID,Molecule> change(const Molecule &molecule);
    QHash<MoleculeID,Molecule> change(const Residue &residue);
    QHash<MoleculeID,Molecule> change(const NewAtom &atom);

    void remove(const Molecule &molecule);

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

/** Return whether or not this object contains information about
    the molecule with ID == molid */
inline bool SystemData::contains(MoleculeID molid) const
{
    return molgroups.contains(molid); // or mol_constraints.contains(molid);
}

/** Return all of the Molecule groups in this System */
inline const MoleculeGroups& SystemData::groups() const
{
    return molgroups;
}

}

SIRE_END_HEADER

#endif
