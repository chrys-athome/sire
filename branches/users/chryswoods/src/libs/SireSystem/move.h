#ifndef SIRESYSTEM_MOVE_H
#define SIRESYSTEM_MOVE_H

#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class MoveBase;
class Move;
}

QDataStream& operator<<(QDataStream&, const SireSystem::MoveBase&);
QDataStream& operator>>(QDataStream&, SireSystem::MoveBase&);

QDataStream& operator<<(QDataStream&, const SireSystem::Move&);
QDataStream& operator>>(QDataStream&, SireSystem::Move&);

namespace SireSim
{
class Simulation;
}

namespace SireSystem
{

using SireSim::Simulation;

/** This is the virtual base class of all moves that
    can be applied to a SimSystem

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MoveBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MoveBase&);
friend QDataStream& ::operator>>(QDataStream&, MoveBase&);

public:
    MoveBase();

    MoveBase(const MoveBase &other);

    virtual ~MoveBase();

    MoveBase& operator=(const MoveBase &other);

    static const char* typeName()
    {
        return "SireMove::MoveBase";
    }

    virtual const char* what() const=0;

    virtual MoveBase* clone() const=0;

    virtual void initialise(const SimSystem &system)=0;

    virtual void move(SimSystem &system)=0;
};

/** This is a convienient wrapper for SharedPolyPointer<MoveBase>

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT Move
{

friend QDataStream& ::operator<<(QDataStream&, const Move&);
friend QDataStream& ::operator>>(QDataStream&, Move&);

public:
    Move();

    Move(const MoveBase &move);

    Move(const SireBase::SharedPolyPointer<MoveBase> &ptr);

    Move(const Move &move);

    ~Move();

    Move& operator=(const MoveBase &move);
    Move& operator=(const SireBase::SharedPolyPointer<MoveBase> &ptr);
    Move& operator=(const Move &move);

    void initialise(const SimSystem &system);

    void move(SimSystem &system);

private:
    /** Shared pointer to the actual move object */
    SharedPolyPointer<MoveBase> d;
};

/** Initialise the move to work on the Simulation system 'system' */
inline void Move::initialise(const SimSystem &system)
{
    d->initialise(system);
}

/** Perform the move on the simulation system */
inline void Move::move(SimSystem &system)
{
    d->move(system);
}

}

Q_DECLARE_METATYPE(SireSystem::Move);

SIRE_END_HEADER

#endif
