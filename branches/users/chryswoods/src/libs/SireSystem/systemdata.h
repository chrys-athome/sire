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
    bool operator==(const SystemData &other) const;
    bool operator!=(const SystemData &other) const;
    
    const QString& name() const;
    SystemID ID() const;
    const Version& version() const;

    const MoleculeGroup& group(MoleculeGroupID id) const;
    const QHash<MoleculeGroupID,MoleculeGroup>& groups() const;

    QSet<FFExpression> equations() const;

    const Values& parameters() const;

protected:

    SystemData(const QString &name = QObject::tr("Unnamed"));

    SystemData(const SystemData &other);

    virtual ~SystemData();

    void add(const FFExpression &ff_equation);
    void add(const QSet<FFExpression> &ff_equations);

    void remove(const FFExpression &ff_equation);
    void remove(const QSet<FFExpression> &ff_equations);
    void remove(const Function &function);
    void remove(const QSet<Function> &functions);

    void replace(const FFExpression &newexpr);
    void replace(const FFExpression &oldexpr, const FFExpression &newexpr);
    void replace(const Function &oldfunc, const FFExpression &newexpr);

    //void recordAverage(const FFExpression &ff_equation,
    //                   const Averager &averager);

    void add(const MoleculeGroup &group);
    void remove(const MoleculeGroup &group);

    void change(const Molecule &molecule);
    void change(const Residue &residue);
    void change(const NewAtom &atom);

    void updateStatistics();

    double energy(const FFExpression &expression);
    double energy(const Function &component);
    double energy(const FFComponent &component);

    void setParameter(const Symbol &param, double value);

    virtual Values getEnergyComponents(const QSet<ForceFieldID> &ffids)=0;
    virtual Values getEnergyComponents(const QSet<FFComponent> &components)=0;

    virtual double getEnergyComponent(const FFComponent &component)=0;

private:

    /** Small internal class used by SystemData to hold
        metainformation about the forcefield expressions used
        to calculate energies */
    class ExpressionInfo
    {

    friend QDataStream& ::operator<<(QDataStream&, const SystemData&);
    friend QDataStream& ::operator>>(QDataStream&, SystemData&);

    public:
        ExpressionInfo();

        ExpressionInfo(const FFExpression &ex,
                       const QHash<SymbolID,ExpressionInfo> &ff_equations);

        ExpressionInfo(const ExpressionInfo &other);

        ~ExpressionInfo();

        ExpressionInfo& operator=(const ExpressionInfo &other);

        /** Return the expression that is described by
            this object */
        const FFExpression& expression() const
        {
            return ex;
        }

        /** Return all of the dependencies of this expression
            (all expressions on which this one depends, and before
            those, all that they depend on etc.) - this will
            return them in an order such that the first
            expressions don't depend on anything, and then
            each subsequent expression depends only on
            those that have come before. */
        const QVector<FFExpression>& dependencies() const
        {
            return deps;
        }

        /** Return the complete set of ID numbers of forcefields
            that contain components that are required by this
            expression, or required by any expression on
            which this depends. */
        const QSet<ForceFieldID>& forcefieldIDs() const
        {
            return ffids;
        }

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

    double energy(const ExpressionInfo &expr);
    
    /** Hash mapping all of the different forcefield functions
        to their corresponding equation infos */
    QHash<SymbolID, ExpressionInfo> ff_equations;

    /** Hash mapping all of the forcefield functions that should be averaged
        to their corresponding averaging function (which includes the running
        average) */
    //QHash<SymbolID, Averager> ff_averages;

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
