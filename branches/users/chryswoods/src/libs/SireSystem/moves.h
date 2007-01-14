#ifndef SIRESYSTEM_MOVES_H
#define SIRESYSTEM_MOVES_H

#include <QMutex>

#include "move.h"
#include "systemid.h"

#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

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

/** This is class holds a collection of moves that may
    be performed on the system, together with the
    rules on the order of how they are applied to
    the system.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT Moves
{
public:
    Moves();

    Moves(const MovesBase &moves);

    Moves(const Moves &other);

    ~Moves();

    Moves& operator=(const Moves &other);

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

SIRE_END_HEADER

#endif
