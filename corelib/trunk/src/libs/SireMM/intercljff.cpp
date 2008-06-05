//
// #include "SireMol/qhash_siremol.h"
// 
// #include "intercljff.h"
// #include "ffwrapper.hpp"
// #include "registerff.hpp"
// 
// #include "SireMol/molecule.h"
// #include "SireMol/cutgroup.h"
// 
// #include "SireVol/simvolume.h"
// #include "SireVol/cartesian.h"
// 
// #include "SireUnits/units.h"
// #include "SireError/exceptions.h"
// #include "SireStream/txtstream.h"
// 
// using namespace SireFF;
// using namespace SireMol;
// using namespace SireStream;
// using namespace SireBase;
// using namespace SireMaths;
// 
// typedef FFWrapper<InterCLJFF> InterCLJFFWrapper;
// 
// /** Register this forcefield with the FFRegistry */
// static RegisterFF< InterCLJFFWrapper > RegisterInterCLJFF;
// 
// /** Serialise this forcefield to a data stream */
// QDataStream& operator<<(QDataStream &ds, const InterCLJFF &ff)
// {
//     //save a MagicID to the stream to aid the detection of stream corruption
//     ds << RegisterInterCLJFF.magic();
//     
//     //save the list of CutGroups in the forcefield
//     //ds << allgroups;
//     
//     //now save the CLJFF specific details
//     ds << static_cast<const CLJFF&>(ff);
//     
//     return ds;
// }
// 
// /** Load this forcefield from a data stream */
// QDataStream& operator>>(QDataStream &ds, InterCLJFF &ff)
// {
//     //check the MagicID
//     MagicID id;
//     ds >> id;
//     RegisterInterCLJFF.checkMagic(id, CODELOC);
// 
//     //load the list of CutGroups in the forcefield
//     //ds >> allgroups;
//     
//     //now load the CLJFF specific details
//     ds >> static_cast<CLJFF&>(ff);
//     
//     return ds;
// }
// 
// /** Construct an empty InterCLJFF forcefield */
// InterCLJFF::InterCLJFF() : CLJFF()
// {}
// 
// /** Copy constructor. This should be fast as nearly everything in this
//     forcefield is implicitly shared */
// InterCLJFF::InterCLJFF(const InterCLJFF &other) : CLJFF(other), allgroups(other.allgroups)
// {}
// 
// /** Destructor */
// InterCLJFF::~InterCLJFF()
// {}
// 
// /** Return a copy of this forcefield wrapped up by an FFWrapper */
// InterCLJFF::operator FFWrapperPtr() const
// {
//     return FFWrapperPtr( new InterCLJFFWrapper(*this) );
// }
// 
// /** Add the passed cutgroup to this forcefield, using the parameters supplied
//     in the CLJTable.
// */    
// void InterCLJFF::add(const CutGroupPtr &group, const CLJTable &cljtable)
// {
//     if (group.isNull() or group->natoms() == 0)
//         return;
//         
//     //save the parameters for this CutGroup
//     this->setParameters(group, cljtable);
//     
//     //add this cutgroup to the list...
//     allgroups.append(group);
//     
//     //if this is a perturbable cutgroup, then add it to the perturbable list
//     if (group->isPerturbable())
//         pertgroups.append(group);
// }
// 
// /** Add the passed cutgroup to this forcefield, using the parameters contained
//     in the CLJMutator. This means that the parameters of this cutgroup could
//     change with respect to lambda, so this could be a perturbable cutgroup */
// void InterCLJFF::add(const CutGroupPtr &group, const CLJMutator &cljmutator)
// {
//     if (group.isNull() or group->natoms() == 0)
//         return;
//         
//     //save the parameters for this CutGroup
//     this->setParameters(group, cljmutator);
//     
//     allgroups.append(group);
//     
//     //if this is a perturbable cutgroup, or has perturbable parameters, then
//     //add this to the perturbable list
//     if (group->isPerturbable() or hasPerturbableParams(group.objID()))
//         pertgroups.append(group);
// }
// 
// /** Add the set of CutGroups to this forcefield using the provided parameters.
//     As the same parameter table is used for all of the CutGroups, either all
//     of the CutGroups must come from the same molecule, or they must have all
//     come from the same type of molecule. */
// void InterCLJFF::add(const CutGroupSet &groups, const CLJTable &cljtable)
// {
//     int ngroups = groups.count();
//     for (int i=0; i<ngroups; ++i)
//         this->add(groups[i], cljtable);
// }
// 
// /** Add the set of CutGroups to this forcefield using the provided mutator.
//     As the same mutator is used for all of the CutGroups, either all of them
//     must be from the same molecule, or they must be from the same type
//     of molecule */
// void InterCLJFF::add(const CutGroupSet &groups, const CLJMutator &cljmutator)
// {
//     int ngroups = groups.count();
//     for (int i=0; i<ngroups; ++i)
//         this->add(groups[i], cljmutator);
// }
// 
// /** Add the passed molecule to this forcefield, using the parameters supplied
//     in the CLJTable. 
// */
// void InterCLJFF::add(const MoleculePtr &mol, const CLJTable &cljtable)
// {
//     if (mol.isNull())
//         return;
//         
//     this->add(mol->cutGroups(), cljtable);
// }
// 
// /** Add the passed molecule to this forcefield, using the parameters in the 
//     supplied mutator */
// void InterCLJFF::add(const MoleculePtr &mol, const CLJMutator &cljmutator)
// {
//     if (mol.isNull())
//         return;
//         
//     this->add( mol->cutGroups(), cljmutator );
// }
// 
// /** Add the list of molecules to the forcefield, all using the same parameters.
//     This must mean that all of the molecules are the same sort of molecule. */
// void InterCLJFF::add(const MoleculeSet &mols, const CLJTable &cljtable)
// {
//     int nmols = mols.count();
//     for (int i=0; i<nmols; ++i)
//         this->add(mols[i]->cutGroups(), cljtable);
// }
// 
// /** Add the list of molecules to the forcefield, all using the same parameters
//     from the same mutator. This must mean that these are all the same type of molecule. */
// void InterCLJFF::add(const MoleculeSet &mols, const CLJMutator &cljmutator)
// {
//     int nmols = mols.count();
//     for (int i=0; i<nmols; ++i)
//         this->add(mols[i]->cutGroups(), cljmutator);
// }
// 
// /** Add the residue 'res' to the forcefield, using the parameters in 'cljtable' */
// void InterCLJFF::add(const ResiduePtr &res, const CLJTable &cljtable)
// {
//     if (res.get() == 0)
//         return;
// 
//     this->add(res->cutGroup(), cljtable);
// }
// 
// /** Add the residue 'res' to the forcefield, using the (possibly perturbing) parameters
//     in the mutator 'cljmutator' */
// void InterCLJFF::add(const ResiduePtr &res, const CLJMutator &cljmutator)
// {
//     if (res.get() == 0)
//         return;
//         
//     this->add(res->cutGroup(), cljmutator);
// }
// 
// /** Add the residues in 'resset' to this forcefield, all using the parameters 
//     in 'cljtable' */
// void InterCLJFF::add(const ResidueSet &resset, const CLJTable &cljtable)
// {
//     int nres = resset.count();
//     for (int i=0; i<nres; ++i)
//         this->add( resset.at(i)->cutGroup(), cljtable );
// }
// 
// /** Add the residues in 'resset' to this forcefield, all using the parameters
//     in the mutator 'cljmutator' */
// void InterCLJFF::add(const ResidueSet &resset, const CLJMutator &cljmutator)
// {
//     int nres = resset.count();
//     for (int i=0; i<nres; ++i)
//         this->add( resset.at(i)->cutGroup(), cljmutator );
// }
// 
// /** Calculate and return the total Coulomb and LJ energy of this forcefield  
//     at the lambda state 'stat' */
// CLJEnergy InterCLJFF::energy(const LambdaState &stat) const
// {
//     //create workspace for the calculation
//     DistMatrix distmat;
//     CLJPairMatrix cljmat;
// 
//     //calculate the group-group energy for all cutgroups in the volume
//     return this->intraGroupEnergy(allgroups, stat, distmat, cljmat);    
// }
// 
// /** Return the energy of the CutGroups in 'cgroups' that are in this forcefield, 
//     in the lambda state 'stat'. This only calculates the energy of the specified
//     groups in this forcefield. */
// CLJEnergy InterCLJFF::energy(const CutGroupSet &cgroups, const LambdaState &stat) const
// {
//     //get the intersection of these groups with those that are in this forcefield
//     CutGroupSet intersec = allgroups.intersection(cgroups);
//     
//     //return 0.0 if none of these molecules are in this forcefield
//     if (intersec.count() == 0)
//         return CLJEnergy();
//         
//     //create workspace for these calculations (these are created on the stack)
//     DistMatrix distmat;
//     CLJPairMatrix cljmat;
//     
//     //calculate the interaction between the specified groups and the rest of 
//     //the CutGroups in this forcefield
//     CLJEnergy cljnrg = interGroupEnergy(intersec, stat, distmat, cljmat);
//     
//     //add on the intra-group energy for the supplied groups
//     if (intersec.count() > 1)
//         cljnrg += intraGroupEnergy(intersec, stat, distmat, cljmat);
//         
//     return cljnrg;
// }
// 
// /** Return the difference in total energy between lambda state 'stat0' and lambda 
//     state 'stat1' */
// CLJEnergy InterCLJFF::deltaEnergy(const LambdaState &stat0, const LambdaState &stat1) const
// {
//     //if the lambda states are the same or there are no perturbable groups
//     //then return 0
//     if (stat0 == stat1 or pertgroups.isEmpty())
//         return CLJEnergy();
//         
//     //return the difference of the interaction energy of the perturbable groups
//     //for the two lambda states
//     return energy(pertgroups, stat0) - energy(pertgroups, stat1);
// }
// 
// /** Return the difference in energy of the CutGroups in 'groups' for the two different
//     lambda states */
// CLJEnergy InterCLJFF::deltaEnergy( const CutGroupSet &groups, const LambdaState &stat0,
//                                    const LambdaState &stat1 ) const
// {
//     //if the lambda states are the same or there are no perturbable groups
//     //then return 0
//     if (stat0 == stat1 or pertgroups.isEmpty() or groups.isEmpty())
//         return CLJEnergy();
//         
//     //split 'groups' into two lists. The first list is the set of CutGroups in 'groups'
//     //that are also in pertgroups (so are perturbable). The second list is the set of 
//     //CutGroups that are in 'groups' but are not in 'pertgroups' (so are not perturbable)
//     boost::tuple<CutGroupList, CutGroupList> lists = groups.splitIntoLists(pertgroups);
//     
//     CutGroupList pert_and_moved = lists.get<0>();
//     CutGroupList moved = lists.get<1>();
//     
//     //create workspace and temp variables for these calculations
//     DistMatrix distmat;
//     CLJPairMatrix cljmat;
//     double deltacnrg(0.0), deltaljnrg(0.0);
//     int ngroups = pert_and_moved.count();
//     int npert = pertgroups.count();
//     
//     //calculate the energy of all of the moved perturbable groups with one another
//     if (ngroups > 1)
//     {
//         for (int i=0; i<ngroups-1; ++i)
//         {
//             const CutGroup &group0 = *(pert_and_moved.at(i));
//             const CLJWindow &clj0_stat0 = this->getParameters(group0.ID(), stat0);
//             const CLJWindow &clj0_stat1 = this->getParameters(group0.ID(), stat1);
//             
//             for (int j=i+1; j<ngroups; ++j)
//             {
//                 const CutGroup &group1 = *(pert_and_moved.at(j));
//                 
//                 double icnrg, iljnrg;
//                 
//                 //add the energy at lambda state 'stat1' onto the delta
//                 if (not SireMol::sameMolecule(group0,group1) 
//                       and not volptr->beyond(nbcut, group0, group1, stat1))
//                 {
//                     cljEnergy(group0, group1, clj0_stat1, stat1, distmat, cljmat, icnrg, iljnrg);
//                     deltacnrg += icnrg;
//                     deltaljnrg += iljnrg;
//                 }
//                 
//                 //subtract the energy at lambda state 'stat0' from the delta
//                 if (not SireMol::sameMolecule(group0,group1) 
//                       and not volptr->beyond(nbcut, group0, group1, stat0))
//                 {
//                     cljEnergy(group0, group1, clj0_stat0, stat0, distmat, cljmat, icnrg, iljnrg);
//                     deltacnrg -= icnrg;
//                     deltaljnrg -= iljnrg;
//                 }
//            }
//        }
//     }
//     
//     //calculate the delta energy for the moved, perturbable groups with all of the
//     //other perturbable groups
//     if (ngroups > npert)
//     {
//         //there are some perturbable groups that have not been moved
//         for (int i=0; i<ngroups; ++i)
//         {
//             const CutGroup &group0 = *(pert_and_moved.at(i));
//             const CLJWindow &clj0_stat0 = this->getParameters(group0.ID(), stat0);
//             const CLJWindow &clj0_stat1 = this->getParameters(group0.ID(), stat1);
//             
//             for (int j=0; j<ngroups; ++j)
//             {
//                 const CutGroup &group1 = *(pertgroups.at(j));
//                 //only calculate the energy if this groups is not also moved
//                 if (not groups.has_key(group1.ID()) 
//                       and not SireMol::sameMolecule(group0,group1))
//                 {
//                     double icnrg, iljnrg;
//                     
//                     if (not volptr->beyond(nbcut, group0, group1, stat1))
//                     {
//                         cljEnergy(group0, group1, clj0_stat1, stat1, distmat, cljmat, icnrg, iljnrg);
//                         deltacnrg += icnrg;
//                         deltaljnrg += iljnrg;
//                     }
//                     
//                     if (not volptr->beyond(nbcut, group0, group1, stat0))
//                     {
//                         cljEnergy(group0, group1, clj0_stat0, stat0, distmat, cljmat, icnrg, iljnrg);
//                         deltacnrg -= icnrg;
//                         deltaljnrg -= iljnrg;
//                     }
//                 }
//             }
//         }
//     }
//     
//     //finally, calculate the energy of all of the moved, non-perturbable groups with 
//     //all of the perturbable groups in this forcefield
//     ngroups = moved.count();
//     
//     for (int i=0; i<ngroups; ++i)
//     {
//         const CutGroup &group0 = *(moved.at(i));
//         //only need to get the parameters at stat0 as they will be the same as at stat1
//         //for this non-perturbable group
//         const CLJWindow &clj0 = this->getParameters(group0.ID(), stat0);
//     
//         for (int j=0; j<npert; ++j)
//         {
//             const CutGroup &group1 = *(pertgroups.at(j));
//         
//             double icnrg, iljnrg;
//             
//             if (not SireMol::sameMolecule(group0,group1) 
//                   and not volptr->beyond(nbcut, group0, group1, stat1))
//             {
//                 cljEnergy(group0, group1, clj0, stat1, distmat, cljmat, icnrg, iljnrg);
//                 deltacnrg += icnrg;
//                 deltaljnrg += iljnrg;
//             }
//             
//             if (not SireMol::sameMolecule(group0,group1) 
//                   and not volptr->beyond(nbcut, group0, group1, stat0))
//             {
//                 cljEnergy(group0, group1, clj0, stat0, distmat, cljmat, icnrg, iljnrg);
//                 deltacnrg -= icnrg;
//                 deltaljnrg -= iljnrg;
//             }
//         }
//     }
//       
//     //all done!
//     return CLJEnergy(deltacnrg, deltaljnrg);
// } 
//     
// /** Calculate the group-group CLJ energy of all groups in 'cgroups'. This is an internal
//     function of this class, and we have already guaranteed that 'cgroups' contains
//     only CutGroups that are in this forcefield. If 'cgroups' contains any CutGroups
//     that are not in this forcefield then there will be undefined results! */
// CLJEnergy InterCLJFF::intraGroupEnergy(const CutGroupSet &cgroups, const LambdaState &stat,
//                                        DistMatrix &distmat, CLJPairMatrix &cljmat) const
// {
//     double cnrg(0.0), ljnrg(0.0);
//     int ngroups = cgroups.count();
//     if (ngroups < 2)
//         return CLJEnergy();
//         
//     for (int i=0; i<ngroups-1; ++i)
//     {
//         const CutGroup &group0 = *(cgroups.at(i));
//         const CLJWindow &clj0 = this->getParameters(group0.ID(), stat);
// 
//         for (int j=i+1; j<ngroups; ++j)
//         {
//             const CutGroup &group1 = *(cgroups.at(j));
//             
//             //skip intra-molecular pairs and pairs that are definitely beyond cutoff distance.
//             //I do not group the two conditions together
//             //(e.g. not (group0.sameMolecule(group1) and volptr->beyondCutoff...) because
//             //this causes the speed of this function to half! I don't why, so just leave
//             //this as it is. This shows how important it is to constantly speed-test the
//             //code that you write.
//             if (not SireMol::sameMolecule(group0,group1) 
//                     and not volptr->beyond(nbcut,group0,group1,stat) )
//             {
//                 double icnrg,iljnrg;
//                 cljEnergy(group0, group1, clj0, stat, distmat, cljmat, icnrg, iljnrg);
//                 cnrg += icnrg;
//                 ljnrg += iljnrg;
//             }
//         }
//     }
//     
//     return CLJEnergy(cnrg, ljnrg);
// }
//     
// /** Calculate the interaction energy of the CutGroups in 'cgroups' with all of the 
//     remaining CutGroups in this forcefield. This is an internal
//     function of this class, and we have already guaranteed that 'cgroups' contains
//     only CutGroups that are in this forcefield. If 'cgroups' contains any CutGroups
//     that are not in this forcefield then there will be undefined results! 
// */
// CLJEnergy InterCLJFF::interGroupEnergy(const CutGroupSet &cgroups, const LambdaState &stat,
//                                        DistMatrix &distmat, CLJPairMatrix &cljmat) const
// {
//     //calculate the energy with the rest of the system (excluding the above groups)
//     int ngroups = cgroups.count();
//     
//     //get the ratio of the number of CutGroups in 'cgroups' and the number in this
//     //forcefield
//     int ratio = allgroups.count() / ngroups;
//     
//     //if 'cgroups' contains more than a third of the CutGroups in this forcefield, then
//     //we will speed up the calculation by caching the difference
//     if (ratio <= 3)
//     {
//         //calculate the difference between these CutGroups (i.e. the 
//         //CutGroups that are in this forcefield that are not in 'cgroups')
//         CutGroupSet diffgroups = allgroups.leftDifference(cgroups);
//         
//         //if there are no different CutGroups, then there is no interGroupEnergy
//         if (diffgroups.count() == 0)
//             return CLJEnergy();
//             
//         //calculate the interaction energy between 'cgroups' and 'diffgroups'
//         return groupGroupEnergy(cgroups, diffgroups, stat, distmat, cljmat);
//     }
//     else
//     {
//         //create a workspace in which to make the calculation
//         double cnrg(0.0), ljnrg(0.0);
//         
//         int n0 = cgroups.count();
//         int n1 = allgroups.count();
//         
//         for (int i=0; i<n0; ++i)
//         {
//             const CutGroup &group0 = *(cgroups.at(i));
//             const CLJWindow &clj0 = this->getParameters(group0.ID(), stat);
//             
//             for (int j=0; j<n1; ++j)
//             {
//                 const CutGroup &group1 = *(allgroups.at(j));
//                 
//                 //changing the order or grouping of this 'if' can have very negative
//                 //effect on the speed of this function.
//                 if ( not (cgroups.has_key(group1.ID()) and SireMol::sameMolecule(group0,group1))
//                            and not volptr->beyond(nbcut, group0, group1, stat) )
//                 {
//                     double icnrg,iljnrg;
//                     cljEnergy(group0, group1, clj0, stat, distmat, cljmat, icnrg, iljnrg);
//                     cnrg += icnrg;
//                     ljnrg += iljnrg;
//                 }
//             }
//         }
//         
//         return CLJEnergy(cnrg, ljnrg);
//     }
// }
// 
// /** Calculate the CLJ interaction energy between all of the CutGroups in group0 with 
//     all of the CutGroups in group1. This is an internal function, and it is already
//     guaranteed that groups0 and groups1 only contain CutGroups that are in this forcefield,
//     and that no one CutGroup is in both groups. Failure to meet either of these conditions
//     will lead to undefined results. */
// CLJEnergy InterCLJFF::groupGroupEnergy(const CutGroupSet &groups0, const CutGroupSet &groups1,
//                                        const LambdaState &stat, DistMatrix &distmat,
//                                        CLJPairMatrix &cljmat) const
// {
//     //arrange the groups so that we iterate over the smallest group
//     const CutGroupSet &g0 = smallestCutGroupSet(groups0, groups1);
//     const CutGroupSet &g1 = largestCutGroupSet(groups0, groups1);
//     
//     //create a workspace in which to do the calculation
//     double cnrg(0.0), ljnrg(0.0);
//     CutGroupArray closegroups;
//     
//     //iterate over all CutGroups in the smallest CutGroupSet    
//     int n0 = g0.count();
//     int n1 = g1.count();
//     
//     for (int i=0; i<n0; ++i)
//     {
//         const CutGroup &group0 = *(g0.at(i));
//         const CLJWindow &clj0 = this->getParameters(group0.ID(), stat);
//         
//         for (int j=0; j<n1; ++j)
//         {
//             const CutGroup &group1 = *(g1.at(j));
//             
//             //changing the order or grouping of this 'if' can have very negative
//             //effect on the speed of this function.
//             if ( not (g0.has_key(group1.ID()) and SireMol::sameMolecule(group0,group1))
//                        and not volptr->beyond(nbcut, group0, group1, stat) )
//             {
//                 double icnrg, iljnrg;
//                 
//                 cljEnergy(group0, group1, clj0, stat, distmat, cljmat, icnrg, iljnrg);
//                 cnrg += icnrg;
//                 ljnrg += iljnrg;
//             }
//         }
//     }
//                     
//     return CLJEnergy(cnrg, ljnrg);
// }
