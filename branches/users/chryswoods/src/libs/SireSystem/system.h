#ifndef SIRESYSTEM_SYSTEM_H
#define SIRESYSTEM_SYSTEM_H

#include <QString>

#include "systemdata.h"

#include "SireFF/forcefields.h"

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
using SireFF::ForceFields;

/** This class holds all of the data necessary to specify a single
    system of molecules (including how to calculate their energy).

    This is the class/interface visible to the user for creating,
    editing and using simulation systems. This class is also used
    as the checkpoint object for saving the complete state
    of a simulation.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT System
{

friend QDataStream& ::operator<<(QDataStream&, const System&);
friend QDataStream& ::operator>>(QDataStream&, System&);

friend class SimSystem;  //friend so can mess with this System ;-)

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

    void prepareForSimulation();

private:
    System(const SystemData &sysdata, 
           const ForceFields &ffields);

    /** The object holding information about this system */
    SystemData sysdata;
    
    /** The forcefields that are used to calculate
        energies / forces of molecules */
    ForceFields ffields;
};

/** Return the forcefields contained in this system */
inline const ForceFields& System::forceFields() const
{
    return ffields;
}

/** Return the information about this system */
inline const SystemData& System::info() const
{
    return sysdata;
}

}

Q_DECLARE_METATYPE(SireSystem::System);

#endif
