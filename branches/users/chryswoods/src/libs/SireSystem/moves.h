#ifndef SIRESYSTEM_MOVES_H
#define SIRESYSTEM_MOVES_H

#include <QMutex>

#include "move.h"
#include "systemid.h"

#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class Moves;
class MovesBase;

class SameMoves;
}

QDataStream& operator<<(QDataStream&, const SireSystem::Moves&);
QDataStream& operator>>(QDataStream&, SireSystem::Moves&);

QDataStream& operator<<(QDataStream&, const SireSystem::MovesBase&);
QDataStream& operator>>(QDataStream&, SireSystem::MovesBase&);

QDataStream& operator<<(QDataStream&, const SireSystem::SameMoves&);
QDataStream& operator>>(QDataStream&, SireSystem::SameMoves&);

namespace SireSystem
{

/** This is the base class of all collections of moves.
    This class provides a collection of moves that will
    be performed on a system, together with rules for
    how to choose the order in which the moves are
    performed.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MovesBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MovesBase&);
friend QDataStream& ::operator>>(QDataStream&, MovesBase&);

public:
    MovesBase();

    MovesBase(const MovesBase &other);

    virtual ~MovesBase();

    static const char* typeName()
    {
        return "SireSystem::MovesBase";
    }

    virtual const char* what() const=0;

    virtual MovesBase* clone() const=0;

    /** Return the total number of moves necessary
        to complete one cycle */
    virtual int count() const=0;

    /** Return a reference to the next move that
        should be performed */
    virtual Move& nextMove()=0;

    /** Initialise all of the moves in the set to
        work with the passed system */
    virtual void initialise(SimSystem &system)=0;
};

/** This class represents moves which are a collection
    of a single type of Moves
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SameMoves : public MovesBase
{

friend QDataStream& ::operator<<(QDataStream&, const SameMoves&);
friend QDataStream& ::operator>>(QDataStream&, SameMoves&);

public:
    SameMoves();
    
    SameMoves(const Move &move);
    
    SameMoves(const SameMoves &other);
    
    ~SameMoves();
    
    static const char* typeName()
    {
        return "SireSystem::SameMoves";
    }

    const char* what() const
    {
        return SameMoves::typeName();
    }
    
    SameMoves* clone() const
    {
        return new SameMoves(*this);
    }
    
    int count() const
    {
        return 1;
    }
    
    Move& nextMove()
    {
        return single_move;
    }
    
    void initialise(SimSystem &system)
    {
        single_move.initialise(system);
    }

private:
    /** The only move in this set */
    Move single_move;
};

/** This is class holds a collection of moves that may
    be performed on the system, together with the
    rules on the order of how they are applied to
    the system.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT Moves
{

friend QDataStream& ::operator<<(QDataStream&, const Moves&);
friend QDataStream& ::operator>>(QDataStream&, Moves&);

public:
    Moves();

    Moves(const Move &move);
    Moves(const MovesBase &moves);

    Moves(const Moves &other);

    ~Moves();

    Moves& operator=(const Moves &other);

    void initialise(SimSystem &system);

    int count() const;

    void run(SimSystem &system, quint32 nmoves);
    void resume(SimSystem &system);
    void rerun(SimSystem &system);

    void play();
    void pause();
    void stop();

    int percentProgress() const;

private:
    void _pvt_run(SimSystem &system);

    /** Pointer to the implementation of this class */
    SireBase::SharedPolyPointer<MovesBase> d;

    /** A lock used to pause and restart the simulation */
    QMutex runmutex;

    /** This mutex is locked whenever a set of moves
        need to be performed */
    QMutex movemutex;

    /** The ID number of the System being moved by this block
        of moves */
    SystemID sysid;

    /** The total number of moves to perform */
    quint32 ntotal;

    /** The number of moves completed so far... */
    quint32 ncomplete;
};

}

Q_DECLARE_METATYPE(SireSystem::Moves);
Q_DECLARE_METATYPE(SireSystem::SameMoves);

SIRE_END_HEADER

#endif
