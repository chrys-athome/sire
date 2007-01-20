#ifndef SIRESYSTEM_SYSTEMDATA_H
#define SIRESYSTEM_SYSTEMDATA_H

#include <QVector>

#include "systemid.h"
#include "ffexpression.h"

#include "SireBase/idmajminversion.h"

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

namespace SireFF
{
class FFComponent;
}

namespace SireMol
{
class Molecule;
class Residue;
class NewAtom;
class Property;
}

namespace SireSystem
{

class FFExpression;
class MoleculeGroup;
class MoleculeGroupID;

using SireBase::IDMajMinVersion;
using SireBase::Version;

using SireCAS::Values;
using SireCAS::Symbol;
using SireCAS::SymbolID;
using SireCAS::Expression;

using SireVol::Space;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::NewAtom;
using SireMol::Property;

using SireFF::FFComponent;

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

    const MoleculeGroup& group(MoleculeGroupID id) const;
    const QHash<MoleculeGroupID,MoleculeGroup>& groups() const;

    QHash<Function,FFExpression> equations() const;

    const Values& parameters() const;

    void change(const Molecule &molecule);
    void change(const Residue &residue);
    void change(const NewAtom &atom);

    void remove(const Molecule &molecule);

    void updateStatistics();

protected:
    void setForceFields(ForceFieldsBase &forcefields);

    void incrementMinorVersion();
    void incrementMajorVersion();

private:

    /** All of the MoleculeGroups in the System */
    MoleculeGroups molgroups;

    /** Hash mapping all of the forcefield functions that should be averaged
        to their corresponding averaging function (which includes the running
        average) */
    //QHash<SymbolID, Averager> ff_averages;
    
    /** Hash mapping the monitors in this system to their ID */
    //QHash<MonitorID, Monitor> mntrs;

    /** Hash mapping the properties of this system to their name */
    QHash<QString, Property> props;

    /** The Space of the System. All molecules in the System
        will be mapped to lie within this Space when they are
        moved - in addition any moves that change the space
        of the system must use this space (e.g. volume moves). */
    Space sys_space;

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

/** Return all of the Molecule groups in this System,
    indexed by their MoleculeGroupID number */
inline const QHash<MoleculeGroupID,MoleculeGroup>& SystemData::groups() const
{
    return molgroups;
}

/** Return all of the parameters and their in this system */
inline const Values& SystemData::parameters() const
{
    return ff_params;
}

}

SIRE_END_HEADER

#endif
