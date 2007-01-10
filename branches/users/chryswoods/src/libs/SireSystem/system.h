#ifndef SIRESYSTEM_SYSTEM_H
#define SIRESYSTEM_SYSTEM_H

#include <QString>

namespace SireSystem
{

/** This class provides all of the metadata about a System
    (to be honest, all data except for the forcefields).

    This allows a system to be split easily into
    forcefield and non-forcefield parts, which is important
    when used with ClusterSimSystem.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SystemData
{
public:
    SystemData();

    SystemData(const SystemData &other);

    ~SystemData();

    SystemID ID() const;
    const Version& version() const;

    const QHash<MolGroupID,MoleculeGroup>& groups() const;

    void add(const MoleculeGroup &group);
    void remove(const MoleculeGroup &group);

    void change(const Molecule &molecule);
    void change(const Residue &residue);
    void change(const NewAtom &atom);

    void updateStatistics();

private:
    /** Hash mapping all of the different forcefield functions
        to their corresponding equation infos */
    QHash<SymbolID, FFEquation> ff_equations;

    /** Hash mapping the groups of Molecules in this system to their ID */
    QHash<MolGroupID, MoleculeGroup> molgroups;

    /** Hash mapping the monitors in this system to their ID */
    //QHash<MonitorID, Monitor> mntrs;

    /** Hash mapping the properties of this system to their name */
    QHash<QString, Property> proptys;

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

/** This class holds all of the data necessary to specify a single
    system of molecules (including how to calculate their energy).

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT System
{
public:
    System(const QString &name = QObject::tr("Unnamed"));
    ~System();

    void add(const ForceField &forcefield);
    void remove(const ForceField &forcefield);

    void add(const MoleculeGroup &group);
    void remove(const MoleculeGroup &group);

    //void add(const Monitor &monitor);
    //void remove(const Monitor &monitor);

    void run(const Move &move, quint32 nmoves=1);
    void run(const Moves &moves);
    void run(const Moves &moves, quint32 nmoves);

//    void add(const Molecule &molecule,
//             const FFieldIDs &forcefields,
//             const QStringList &groups);

    void change(const Molecule &molecule);
    void change(const Residue &residue);
    void change(const NewAtom &atom);

    void remove(const Molecule &molecule);

private:

    /** All of the forcefields in the system, indexed by ID */
    QHash<FFID, ForceField> ffields;

    /** Metadata about the forcefield, e.g. the equations giving
        its total energy, the groups, monitors etc. */
    SystemData sysdata;
};

}

#endif
