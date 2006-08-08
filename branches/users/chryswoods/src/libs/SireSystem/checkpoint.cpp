
#include "checkpoint.h"

using namespace SireSystem;

/**
  Implementation of CheckpointMgr
**/

// CheckpointMgr::CheckpointMgr(const SimSystemHdle &syshdle) : sys(syshdle)
// {
//     if (sys.expired())
//     {
//         QString err = QObject::tr("Cannot create a CheckpointMgr for a null SimSystem!");
//         printLine(WARNING,err);
//         throw ProtoMS3::invalid_arg(err,CODELOC);
//     }
// }
// 
// CheckpointMgr::~CheckpointMgr()
// {}
// 
// CheckpointPtr CheckpointMgr::set()
// {
//     CheckpointPtr chkpnt( new Checkpoint(this,chkpoints.count()) );
//     chkpoints.append(CheckpointWeakPtr(chkpnt));
//     
//     return chkpnt;
// }
// 
// void CheckpointMgr::restore(const CheckpointPtr &chkpnt)
// {
//     //if the revision is < 0 then this is an invalid Checkpoint
//     if (chkpnt->revision < 0)
//         return;
// 
//     if (chkpnt->manager != this)
//     {
//         QString err = QObject::tr("Trying to restore a Checkpoint that is not a child "
//                                   "of this CheckpointMgr!");
//         printLine(WARNING,err);
//         throw ProtoMS3::invalid_arg(err,CODELOC);
//     }
//     
//     //restore the molecules in this checkpoint...
//     
//     //Do this by first getting the list of molecules that have changed...
//     
//     
//     //now get the list of molecules that have moved - restore the coordinates...
//     
//     //now clear all checkpoints above this one...
//     int rev = chkpnt->revision;
//     for (int i=chkpoints.count(); i>=rev; i--)
//     {
//         CheckpointPtr ptr = chkpoints[i].lock();
//         
//         if (ptr.get() != 0)
//             ptr->clear();
//             
//         chkpoints.removeAt(i);
//     }
// }
// 
// void CheckpointMgr::willMove(const CutGroupPtr &mol)
// {
//     //get a dump of the coordinates of this molecule...
//     QByteArray data;
//     QDataStream strm(&data, QIODevice::WriteOnly);
//     
//     mol->dumpCoordinates(strm);
//     
//     //get the CutGroupID of the molecule...
//     CutGroupID id = mol->id();
//     
//     //now save these coordinates in all of the checkpoints...
//     int nchk = chkpoints.count();
//     for (int i=0; i<nchk; i++)
//     {
//         CheckpointPtr ptr = chkpoints[i].lock();
//         
//         if (ptr.get() != 0)
//             ptr->checkpointCoordinates(id,data);
//     }
// }
// 
// void CheckpointMgr::willChange(const CutGroupPtr &mol)
// {
//     //first ensure that the coordinates of this molecule are
//     //checkpointed...
//     this->willMove(mol);
// 
//     //get a dump of the full data of this molecule... If
//     //this molecule is not a full molecule, then we dump the
//     //entire molecule!
//     CutGroupPtr wholemol = mol->molecule();
// 
//     QByteArray data;
//     QDataStream strm(&data, QIODevice::WriteOnly);
//     
//     wholemol->dump(strm);
//     
//     //get the CutGroupID of the molecule...
//     CutGroupID id = wholemol->id();
//     
//     //now save this data in all of the checkpoints...
//     int nchk = chkpoints.count();
//     for (int i=0; i<nchk; i++)
//     {
//         CheckpointPtr ptr = chkpoints[i].lock();
//         if (ptr.get() != 0)
//             ptr->checkpoint(id,data);
//     }
// }
// 
// void CheckpointMgr::willAdd(const CutGroupPtr &mol)
// {
//     //we only need to record the addition of whole molecules.
//     //Adding a residue will be recorded via the 'willChange' that is 
//     //called on the parent molecule.
//     if (mol->isMolecule())
//     {
//         CutGroupID id = mol->id();
//         int nchk = chkpoints.count();
//         for (int i=0; i<nchk; i++)
//         {
//             CheckpointPtr ptr = chkpoints[i].lock();
//             if (ptr.get() != 0)
//                 ptr->added(id);
//         }
//     }  
// }
// 
// void CheckpointMgr::willRemove(const CutGroupPtr &mol)
// {
//     //we only need to fully checkpoint this molecule
//     this->willChange(mol);
// }
// 
// CutGroupSet CheckpointMgr::changedGroups(const Checkpoint *chkpoint) const
// {
//     SimSystemPtr sysptr = sys.lock();
//     if (sysptr.get() != 0)
//     {
//         //get all of the cutgroups that have moved
//         CutGroupSet movgroups = sysptr->groups( chkpoint->oldgroups.keys() );
//         //now get all of the molecules that have been added
//         MoleculeSet addmols = sysptr->molecules( chkpoint->addedmols.keys() );
//         
//         //we need to turn the MoleculeSet into a CutGroupSet and add this onto
//         //the first set.
//         movgroups.append( convertToCutGroupSet(addmols) );
//         
//         return movgroups;
//     }
//     else
//         return CutGroupSet();
// }
// 
// CutGroupSet CheckpointMgr::movedGroups(const Checkpoint *chkpoint) const
// {
//     SimSystemPtr sysptr = sys.lock();
//     if (sysptr.get() != 0)
//        return sysptr->groups( chkpoint->oldcoords.keys() );
//     else
//         return CutGroupSet();
// }
// 
// /**
//   Implementation of Checkpoint
// **/
// 
// Checkpoint::Checkpoint(CheckpointMgr *mgr, int rev) : manager(mgr), revision(rev)
// {
//     if (manager == 0)
//     {
//         QString err = QObject::tr("Cannot create a Checkpoint will a null CheckpointMgr!");
//         printLine(WARNING,err);
//         throw ProtoMS3::invalid_arg(err,CODELOC);
//     }
// }
// 
// Checkpoint::~Checkpoint()
// {}
// 
// void Checkpoint::clear()
// {
//     oldcoords.clear();
//     oldgroups.clear();
//     addedmols.clear();
//     int *revptr = const_cast<int*>(&revision);
//     *revptr = -1;
// }
// 
// CutGroupSet Checkpoint::changedGroups() const
// {
//     return manager->changedGroups(this);
// }
// 
// CutGroupSet Checkpoint::movedGroups() const
// {
//     return manager->movedGroups(this);
// }
// 
// void Checkpoint::checkpointCoordinates(CutGroupID id, const QByteArray &data)
// {
//     //do nothing if this molecule has already been added to this checkpoint
//     if (oldcoords.contains(id))
//         return;    
//     //don't checkpoint added molecules    
//     else if (addedmols.contains(id))
//         return;
//     else
//         oldcoords.insert(id,data);
// }
// 
// void Checkpoint::checkpoint(CutGroupID id, const QByteArray &data)
// {
//     //do nothing if this molecule has already been added to this checkpoint
//     if (oldgroups.contains(id))
//         return;
//     //don't checkpoint added molecules
//     else if (addedmols.contains(id))
//         return;
//     else
//         oldgroups.insert(id,data);
// }
// 
// void Checkpoint::added(CutGroupID id)
// {
//     //do nothing if this molecule is already in this checkpoint
//     if (oldcoords.contains(id) or oldgroups.contains(id))
//         return;
//     else
//         addedmols.insert(id,id);
// }
