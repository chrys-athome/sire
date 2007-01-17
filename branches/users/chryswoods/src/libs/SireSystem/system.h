#ifndef SIRESYSTEM_SYSTEM_H
#define SIRESYSTEM_SYSTEM_H

#include <QString>

#include "systemdata.h"
#include "simsystem.h"

#include "SireFF/forcefield.h"

namespace SireSystem
{
class System;
}

QDataStream& operator<<(QDataStream&, const SireSystem::System&);
QDataStream& operator>>(QDataStream&, SireSystem::System&);

namespace SireSystem
{

class Move;
class Moves;

using SireFF::ForceField;

/** This class holds all of the data necessary to specify a single
    system of molecules (including how to calculate their energy).

    This is the class/interface visible to the user for creating,
    editing and using simulation systems. This class is also used
    as the checkpoint object for saving the complete state
    of a simulation.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT System : public SystemData
{

friend QDataStream& ::operator<<(QDataStream&, const System&);
friend QDataStream& ::operator>>(QDataStream&, System&);

//allow access from the 'makeSystem' function of SimSystem so that
//SimSystems can construct a System from data and forcefields without
//requiring any validation (as we must assume that the SimSystem is capable
//of respecting the need to keep the forcefields and metadata in a consistent
//state!) The function is able to call the protected constructor from
//a SystemData and set of forcefields.
friend System SimSystem::makeSystem(const SystemData &data,
                                    const QHash<ForceFieldID,ForceField> &ffields);

public:
    System();
    System(const QString &name);

    System(const System &other);

    ~System();

    System& operator=(const System &other);

    bool operator==(const System &other) const;
    bool operator!=(const System &other) const;

    void add(const ForceField &forcefield);
    void remove(const ForceField &forcefield);

    void add(const FFExpression &ff_equation);
    void remove(const FFExpression &ff_equation);
    void remove(const Function &component);

    //void recordAverage(const FFExpression &ff_equation,
    //                   const Averager &averager = MeanAndStdDev());

    //void getAverage(const FFExpression &ff_equation);
    //void getAverage(const Function &component);

    void add(const MoleculeGroup &group);
    void remove(const MoleculeGroup &group);

    //void add(const Monitor &monitor);
    //void remove(const Monitor &monitor);

//    void add(const Molecule &molecule,
//             const FFieldIDs &forcefields,
//             const QStringList &groups);

    void change(const Molecule &molecule);
    void change(const Residue &residue);
    void change(const NewAtom &atom);

    void remove(const Molecule &molecule);

    const QHash<ForceFieldID,ForceField>& forceFields() const;

    void run(const Move &move, quint32 nmoves=1);
    void run(const Moves &moves);
    void run(const Moves &moves, quint32 nmoves);

protected:
    System(const SystemData &data,
           const QHash<ForceFieldID, ForceField> &forcefields);

    Values getEnergyComponents(const QSet<ForceFieldID> &ffids);
    Values getEnergyComponents(const QSet<FFComponent> &components);

    double getEnergyComponent(const FFComponent &component);

private:
    /** All of the forcefields in the system, indexed by ID */
    QHash<ForceFieldID, ForceField> ffields;

    /** The set of forcefield expressions that have been added to
        the system, but have yet to be fully resolved. */
    QSet<FFExpression> tmp_expressions;

};

/** Return the forcefields contained in this system, indexed by
    their ForceFieldID */
inline const QHash<ForceFieldID,ForceField>& System::forceFields() const
{
    return ffields;
}

}

Q_DECLARE_METATYPE(SireSystem::System);

#endif
