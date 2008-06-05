#ifndef MOVE_H
#define MOVE_H

#include "cutgroupptr.h"
#include "cutgroupset.h"

namespace ProtoMS3
{
namespace Core
{

/**
This is the virtual base class of all simulation moves that may be made. A move is given a collection of CutGroups on which it may operate. Performing the move consists of three stages;
 
(1) 'prime' the move. This chooses which of the CutGroups will be moved (e.g. all of them for MD, or biased towards the closest solvent for preferential sampling MC). The set of CutGroups that will be moved are then available via the 'toBeMoved()' member function.
 
(2) Actually performing the move. This (optionally) calculates the energy before the move, performs the move, then updates the energy for after the move. 
 
(3) Accepting the move. If this is an MC type of move then the results of the move are tested and the move accepted or rejected.
 
Step 1 is initiated via the 'prime' function, which also returns the set of CutGroups that will be moved. The move will be in the 'PRIMED' state after calling this function.
 
Steps 2 and 3 are called from the 'move' function, with the actual mechanics of the move called via other virtual functions. The list of CutGroups to be moved will be cleared after this function has done its work, and the move will be taken out of the 'PRIMED' state.
 
 
@author Christopher Woods
*/
class Move
{
public:
    /** The different states that a Move can be in */
    enum MoveState{ PRIMED, SAFE };

    Move();
    virtual ~Move();

    /** Return the state of this move */
    MoveState state() const;
    
    /** Generate the move - this means choose which CutGroups to move and pre-calculate
        any biasing factors based on that choice. This function returns the CutGroups that 
        will be moved. The set of moving CutGroups can also be obtained via the toBeMoved() 
        function */
    virtual CutGroupSet generate()=0;
    
    /** Actually perform the move. The system must be left in a sane state at the 
        end of the move (so that the results of one move cannot cause bugs in the 
        running of another move). Returns whether the move was successful
        (resulted in a change to the coordinates of the system). An exception
        will be thrown if 'move' is called before 'generate'. */
    virtual bool move()=0;
    
    /** Return the set of CutGroups that may be moved by this move. This
        must return the same set of CutGroups as returned by the 'generate' 
        function. */
    virtual CutGroupSet toBeMoved() const=0;
    
    /** Add a set of CutGroups to this move. Note that you cannot add CutGroups
        to a move while it is in the 'PRIMED' state - an exception will be 
        thrown if you try! */
    virtual void add(const CutGroupSet &mols)=0;
    virtual void add(const CutGroupPtr &mol);
    
    /** Remove a set of CutGroups from this move. Note that you cannot remove
        CutGroups from a move while it is in the 'PRIMED' state. */
    virtual void remove(const CutGroupSet &mols)=0;
    virtual void remove(const CutGroupPtr &mol);

protected:

    /** The current state of the Move */
    MoveState stat;
};

inline Move::MoveState Move::state() const
{
    return stat;
}

inline void Move::add(const CutGroupPtr &mol)
{
    this->add(CutGroupSet(mol));
}

inline void Move::remove(const CutGroupPtr &mol)
{
    this->remove(CutGroupSet(mol));
}

}
}

#endif
