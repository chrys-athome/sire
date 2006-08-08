#ifndef SIRESYSTEM_CHECKPOINT_H
#define SIRESYSTEM_CHECKPOINT_H

namespace SireSystem
{

/**
A Checkpoint actually holds the Checkpoint data...

@author Christopher Woods
*/
// class Checkpoint
// {
// friend class CheckpointMgr;
// 
// public:
//     ~Checkpoint();
//     
//     /** Clear this checkpoint */
//     void clear();
//     
//     /** Return the set of CutGroups that have changed since this
//         checkpoint was set */
//     CutGroupSet changedGroups() const;
//     
//     /** Return the set of CutGroups that have moved since this
//         checkpoint was set */
//     CutGroupSet movedGroups() const;
//     
// protected:
//     /** Construct a checkpoint that is a child of manager 'mgr', and
//         has revision 'rev' */
//     Checkpoint(CheckpointMgr *mgr, int rev);
//     
//     /** Checkpoint the coordinates in 'data', that belonged to CutGroup 'id' */
//     void checkpointCoordinates(CutGroupID id, const QByteArray &data);
//     /** Checkponit the molecule in 'data', that belonged to CutGroup 'id' */
//     void checkpoint(CutGroupID id, const QByteArray &data);
// 
//     /** Say that the molecule 'id' has been added */
//     void added(CutGroupID id);
//     
//     /** Pointer to the manager of this Checkpoint */
//     const CheckpointMgr *manager;
//     /** The revision of this Checkpoint */
//     const int revision;
//     
//     /** The set of changed coordinates indexed by CutGroupID */
//     QHash<CutGroupID, QByteArray> oldcoords;
//     
//     /** The set of changed groups indexed by CutGroupID */
//     QHash<CutGroupID, QByteArray> oldgroups;
//     
//     /** The list of molecules that were added since this Checkpoint was taken */
//     QHash<CutGroupID, CutGroupID> addedmols;
// };
    
/**
A CheckpointMgr manages checkpoints at which the entire system is saved. This class is used to provide a saved version of the system at every move which may be restored if the move is rejected. Because of efficiency concerns, setting a checkpoint *does not* actually copy the entire system into storage. Rather it sets a point at which copies of parts of the system will be stored if they are moved or changed. In this way, a checkpoint only stores the differences between the current system and the checkpointed system. This makes a checkpoint much more efficient.
 
@author Christopher Woods
*/
// class CheckpointMgr
// {
// public:
//     /** Construct a checkpoint that saves the state of SimSystem 'sys' */
//     CheckpointMgr(const SimSystemHdle &sys);
//     ~CheckpointMgr();
// 
//     /** Set a new checkpoint - returns shared_ptr to the created Checkpoint. If this
//         pointer goes out of scope then the Checkpoint is deleted. */
//     CheckpointPtr set();
//     
//     /** Restore the system to the state at Checkpoint 'chkpoint'. Note that any
//         checkpoints newer than 'chkpoint' will be invalidated, e.g.
//          
//         set checkpoints a -> b -> c -> d -> e
//         if restore c, then d and e are invalidated. However a and b are not invalid,
//         and we may later restore a or b. This is to prevent branching, where we
//         restore to c, then continue to d' and e',
//         
//                     d' -> e'
//                    /
//         a -> b -> c -> d -> e
//         
//         Branching would severly complicate the implementation of checkpoints,
//         (e.g. how do we move from e' to e?) and I currently doubt that it would be useful.
//         
//     */
//     void restore(const CheckpointPtr &chkpoint);
//     
//     /** Register that the molecule 'mol' will be moved. You must call this function
//         before you move the molecule. */
//     void willMove(const CutGroupPtr &mol);
//     
//     /** Register that the molecule 'mol' will be changed, e.g. atom identities,
//         number of atoms or bonding patterns have changed. You must call this function
//         before the molecule is changed. */
//     void willChange(const CutGroupPtr &mol);
//     
//     /** Register that the molecule 'mol' will be added to the system. You must call
//         this function before you add the molecule. */
//     void willAdd(const CutGroupPtr &mol);
//     
//     /** Register that the molecule 'mol' will be removed from the system. You must
//         call this function before you remove the molecule. */
//     void willRemove(const CutGroupPtr &mol);
//         
//     /** Return the set of CutGroups that have moved since the checkpoint 'chkpnt' was set */
//     CutGroupSet movedGroups(const Checkpoint *chkpoint) const;
//     
//     /** Return the set of CutGroups that have changed since the checkpoint 'chkpnt' was set */
//     CutGroupSet changedGroups(const Checkpoint *chkpoint) const;
//     
// protected:
// 
//     /** The collection of Checkpoints of the system that are currently held,
//         held in order from the oldest checkpoint to the newest. */
//     QList<CheckpointWeakPtr> chkpoints;
//     
//     /** The SimSystem that is checkpointed */
//     SimSystemHdle sys;
//     
// };

}

#endif
