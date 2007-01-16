#ifndef SIRESYSTEM_SYSTEM_H
#define SIRESYSTEM_SYSTEM_H

#include <QString>

#include "systemid.h"
#include "moleculegroup.h"

#include "SireBase/idmajminversion.h"
#include "SireVol/space.h"
#include "SireMol/property.h"

namespace SireSystem
{

using SireBase::IDMajMinVersion;
using SireBase::Version;

using SireCAS::SymbolID;

using SireVol::Space;

using SireMol::Property;

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

    virtual ~SystemData();

    SystemID ID() const;
    const Version& version() const;

    const QHash<MoleculeGroupID,MoleculeGroup>& groups() const;

    void add(const MoleculeGroup &group);
    void remove(const MoleculeGroup &group);

    void change(const Molecule &molecule);
    void change(const Residue &residue);
    void change(const NewAtom &atom);

    void updateStatistics();

    double evaluate(const Function &component);
    double evaluate(const FFComponent &component);

private:

    virtual Values getEnergyComponents(const QSet<ForceFieldID> &ffids);

    /** Small internal class used by SystemData to hold
        metainformation about the forcefield expressions used
        to calculate energies */
    class ExpressionInfo
    {
    public:
        ExpressionInfo();

        ExpressionInfo(const FFExpression &ex,
                       const QHash<SymbolID,ExpressionInfo> &ff_equations);

        ExpressionInfo(const ExpressionInfo &other);

        ~ExpressionInfo();

        const FFExpression& expression() const;

    private:
        /** This is the actual expression */
        FFExpression ex;

        /** The list of expressions on which this expression depends */
        QVector<FFExpression> deps;

        /** The complete list of ForceFieldIDs of all of the forcefields
            on which this expression depends (includes dependencies from
            expressions that this depends on) */
        QSet<ForceFieldID> ffids;
    };

    /** Hash mapping all of the different forcefield functions
        to their corresponding equation infos */
    QHash<SymbolID, ExpressionInfo> ff_equations;

    /** A collection of parameters of this forcefield */
    Values ff_params;

    /** A cache of component energies (this is cleared whenever the
        system is changed) */
    QHash<SymbolID, double> energy_cache;

    /** Hash mapping the groups of Molecules in this system to their ID */
    QHash<MoleculeGroupID, MoleculeGroup> molgroups;

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

/** This class holds all of the data necessary to specify a single
    system of molecules (including how to calculate their energy).

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT System : public SystemData
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
};

}

#endif
