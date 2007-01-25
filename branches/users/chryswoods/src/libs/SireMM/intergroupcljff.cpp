/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

// 
// #include "SireMol/qhash_siremol.h"
// 
// #include "intergroupcljff.h"
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
// typedef FFWrapper<InterGroupCLJFF> InterGroupCLJFFWrapper;
// 
// /** Register this forcefield with the FFRegistry */
// static RegisterFF<InterGroupCLJFFWrapper> RegisterInterGroupCLJFF;
// 
// /** Function to serialise this forcefield to a data stream */
// QDataStream& operator<<(QDataStream &ds, const InterGroupCLJFF &ff)
// {
//     //serialise the magic
//     ds << RegisterInterGroupCLJFF.magic();
//     
//     //serialise the data for this forcefield...
//     
//     //now serialise the CLJFF part...
//     ds << static_cast<const CLJFF&>(ff);
//     
//     return ds;
// }
// 
// /** Function to load this forcefield from a data stream */
// QDataStream& operator>>(QDataStream &ds, InterGroupCLJFF &ff)
// {
//     //check the magic
//     MagicID id;
//     ds >> id;
//     RegisterInterGroupCLJFF.checkMagic(id, CODELOC);
//     
//     //load the data for this forcefield
//     
//     //now load the CLJFF part...
//     ds >> static_cast<CLJFF&>(ff);
//     
//     return ds;
// }
// 
// /** Construct a default InterGroupCLJFF */
// InterGroupCLJFF::InterGroupCLJFF() : CLJFF()
// {}
// 
// /** Copy constructor. Most of the contents of this class are implicitly shared
//     so this should be pretty quick.
// */
// InterGroupCLJFF::InterGroupCLJFF(const InterGroupCLJFF &other)
//                 : CLJFF(other),
//                   group_a(other.group_a), group_b(other.group_b),
//                   pertgroups_a(other.pertgroups_a), pertgroups_b(other.pertgroups_b),
//                   group_a_mols(other.group_a_mols), group_b_mols(other.group_b_mols)
// {}
// 
// /** Destructor */
// InterGroupCLJFF::~InterGroupCLJFF()
// {}
// 
// /** Return a copy of this forcefield wrapped up in an FFWrapper */
// InterGroupCLJFF::operator FFWrapperPtr() const
// {
//     return FFWrapperPtr( new InterGroupCLJFFWrapper(*this) );
// }
// 
// /** Add the passed cutgroup to group A in this forcefield, using the parameters supplied
//     in the CLJTable.
// */    
// void InterGroupCLJFF::addToA( const CutGroupPtr &cgroup, const CLJTable &cljtable )
// {
//     if (cgroup.isNull() or cgroup->natoms() == 0)
//         return;
//         
//     //save the parameters for this CutGroup
//     this->setParameters(cgroup, cljtable);
//     
//     //add this cutgroup to the list...
//     group_a.append(cgroup);
//     
//     //if this is a perturbable cutgroup, then add it to the perturbable list
//     if (cgroup->isPerturbable())
//     {
//         pertgroups.append(cgroup);
//         pertgroups_a.append(cgroup);
//     }
// }
// 
// /** Add the passed cutgroup to group A in this forcefield, using the parameters contained
//     in the CLJMutator. This means that the parameters of this cutgroup could
//     change with respect to lambda, so this could be a perturbable cutgroup */
// void InterGroupCLJFF::addToA( const CutGroupPtr &cgroup, const CLJMutator &mutator )
// {
//     if (cgroup.isNull() or cgroup->natoms() == 0)
//         return;
//         
//     //save the parameters for this CutGroup
//     this->setParameters(cgroup, mutator);
//     
//     group_a.append(cgroup);
//     
//     //if this is a perturbable cutgroup, or has perturbable parameters, then
//     //add this to the perturbable list
//     if (cgroup->isPerturbable() or hasPerturbableParams(cgroup.objID()))
//     {
//         pertgroups.append(cgroup);
//         pertgroups_a.append(cgroup);
//     }
// }
// 
// /** Add the passed cutgroup to group B in this forcefield, using the parameters supplied
//     in the CLJTable.
// */    
// void InterGroupCLJFF::addToB( const CutGroupPtr &cgroup, const CLJTable &cljtable )
// {
//     if (cgroup.isNull() or cgroup->natoms() == 0)
//         return;
//         
//     //save the parameters for this CutGroup
//     this->setParameters(cgroup, cljtable);
//     
//     //add this cutgroup to the list...
//     group_b.append(cgroup);
//     
//     //if this is a perturbable cutgroup, then add it to the perturbable list
//     if (cgroup->isPerturbable())
//     {
//         pertgroups.append(cgroup);
//         pertgroups_b.append(cgroup);
//     }
// }
// 
// /** Add the passed cutgroup to group B in this forcefield, using the parameters contained
//     in the CLJMutator. This means that the parameters of this cutgroup could
//     change with respect to lambda, so this could be a perturbable cutgroup */
// void InterGroupCLJFF::addToB( const CutGroupPtr &cgroup, const CLJMutator &mutator )
// {
//     if (cgroup.isNull() or cgroup->natoms() == 0)
//         return;
//         
//     //save the parameters for this CutGroup
//     this->setParameters(cgroup, mutator);
//     
//     group_b.append(cgroup);
//     
//     //if this is a perturbable cutgroup, or has perturbable parameters, then
//     //add this to the perturbable list
//     if (cgroup->isPerturbable() or hasPerturbableParams(cgroup.objID()))
//     {
//         pertgroups.append(cgroup);
//         pertgroups_b.append(cgroup);
//     }
// }
//                                                       
// /** Add the set of CutGroups to group A in this forcefield using the provided parameters.
//     As the same parameter table is used for all of the CutGroups, either all
//     of the CutGroups must come from the same molecule, or they must have all
//     come from the same type of molecule. */
// void InterGroupCLJFF::addToA( const CutGroupSet &cgroups, const CLJTable &cljtable )
// {
//     int ngroups = cgroups.count();
//     for (int i=0; i<ngroups; ++i)
//         this->addToA(cgroups[i], cljtable);
// }
// 
// /** Add the set of CutGroups to group A in this forcefield using the provided mutator.
//     As the same mutator is used for all of the CutGroups, either all of them
//     must be from the same molecule, or they must be from the same type
//     of molecule */
// void InterGroupCLJFF::addToA( const CutGroupSet &cgroups, const CLJMutator &mutator )
// {
//     int ngroups = cgroups.count();
//     for (int i=0; i<ngroups; ++i)
//         this->addToA(cgroups[i], mutator);
// }
//                          
// /** Add the set of CutGroups to group B in this forcefield using the provided parameters.
//     As the same parameter table is used for all of the CutGroups, either all
//     of the CutGroups must come from the same molecule, or they must have all
//     come from the same type of molecule. */
// void InterGroupCLJFF::addToB( const CutGroupSet &cgroups, const CLJTable &cljtable )
// {
//     int ngroups = cgroups.count();
//     for (int i=0; i<ngroups; ++i)
//         this->addToB(cgroups[i], cljtable);
// }
// 
// /** Add the set of CutGroups to group B in this forcefield using the provided mutator.
//     As the same mutator is used for all of the CutGroups, either all of them
//     must be from the same molecule, or they must be from the same type
//     of molecule */
// void InterGroupCLJFF::addToB( const CutGroupSet &cgroups, const CLJMutator &mutator )
// {
//     int ngroups = cgroups.count();
//     for (int i=0; i<ngroups; ++i)
//         this->addToB(cgroups[i], mutator);
// }
// 
// /** Throw an exception if this molecule is in group A */
// void InterGroupCLJFF::checkNotInGroupA( const MoleculePtr &mol ) const
// {
//     if (group_a_mols.contains(mol.objID()))
//         throw SireError::invalid_arg(QObject::tr(
//             "Cannot insert a molecule (%1) into group B that is also in group A")
//                   .arg(mol->name()));
// }
// 
// /** Throw an exception if this molecule is in group A */
// void InterGroupCLJFF::checkNotInGroupB( const MoleculePtr &mol ) const
// {
//     if (group_b_mols.contains(mol.objID()))
//         throw SireError::invalid_arg(QObject::tr(
//             "Cannot insert a molecule (%1) into group A that is also in group B")
//                   .arg(mol->name()));
// }
// 
// /** Add Molecule 'mol' to group A in this forcefield, with parameters contained in 'cljtable' */
// void InterGroupCLJFF::addToA( const MoleculePtr &mol, const CLJTable &cljtable )
// {
//     if (mol.isNull())
//         return;
//        
//     //ensure that this molecule is not in groupB
//     checkNotInGroupB(mol);
//                   
//     this->addToA(mol->cutGroups(), cljtable);
// }
// 
// /** Add Molecule 'mol' to group A in this forcefield, with (possibly perturbable) parameters
//     contained in 'cljmutator' */
// void InterGroupCLJFF::addToA( const MoleculePtr &mol, const CLJMutator &mutator )
// {
//     if (mol.isNull())
//         return;
//         
//     checkNotInGroupB(mol);
//     this->addToA(mol->cutGroups(), mutator);
// }
// 
// /** Add the Molecules in 'mols' to group A in this forcefield, with parameters contained in 'cljtable' */
// void InterGroupCLJFF::addToA( const MoleculeSet &mols, const CLJTable &cljtable )
// {
//     int nmols = mols.count();
//     for (int i=0; i<nmols; ++i)
//         this->addToA(mols[i], cljtable);
// }
// 
// /** Add the Molecules in 'mols' to group A in this forcefield, with (possibly perturbable) parameters
//     contained in 'mutator' */
// void InterGroupCLJFF::addToA( const MoleculeSet &mols, const CLJMutator &mutator )
// {
//     int nmols = mols.count();
//     for (int i=0; i<nmols; ++i)
//         this->addToA(mols[i], mutator);
// }
// 
// /** Add the residue 'res' to group A in this forcefield, with parameters contained in 'cljtable' */
// void InterGroupCLJFF::addToA( const ResiduePtr &res, const CLJTable &cljtable )
// {
//     if (res.get() == 0)
//         return;
//         
//     //ensure that the molecule containing this residue
//     //is not being split over two groups
//     checkNotInGroupB(res->molecule());
// 
//     this->addToA(res->cutGroup(), cljtable);
// }
// 
// /** Add the residue 'res' to group A in this forcefield, with (possibly perturbable) parameters
//     contained in the CLJMutator 'mutator' */
// void InterGroupCLJFF::addToA( const ResiduePtr &res, const CLJMutator &mutator )
// {
//     if (res.get() == 0)
//         return;
//         
//     checkNotInGroupB(res->molecule());
//     this->addToA(res->cutGroup(), mutator);
// }
// 
// /** Add all of the residues in 'ress' to group A in this forcefield, using the parameters in 
//     the CLJTable 'cljtable' */
// void InterGroupCLJFF::addToA( const ResidueSet &ress, const CLJTable &cljtable )
// {
//     int nres = ress.count();
//     for (int i=0; i<nres; ++i)
//         this->addToA(ress[i], cljtable);
// }
// 
// /** Add all of the residues in 'ress' to group A in this forcefield, using the
//     (possibly perturbable) parameters in 'mutator' */
// void InterGroupCLJFF::addToA( const ResidueSet &ress, const CLJMutator &mutator )
// {
//     int nres = ress.count();
//     for (int i=0; i<nres; ++i)
//         this->addToA(ress[i], mutator);
// }
// 
// /** Add Molecule 'mol' to group B in this forcefield, with parameters contained in 'cljtable' */
// void InterGroupCLJFF::addToB( const MoleculePtr &mol, const CLJTable &cljtable )
// {
//     if (mol.isNull())
//         return;
//        
//     //ensure that this molecule is not in groupB
//     checkNotInGroupA(mol);
//                   
//     this->addToB(mol->cutGroups(), cljtable);
// }
// 
// /** Add Molecule 'mol' to group B in this forcefield, with (possibly perturbable) parameters
//     contained in 'cljmutator' */
// void InterGroupCLJFF::addToB( const MoleculePtr &mol, const CLJMutator &mutator )
// {
//     if (mol.isNull())
//         return;
//         
//     checkNotInGroupA(mol);
//     this->addToB(mol->cutGroups(), mutator);
// }
// 
// /** Add the Molecules in 'mols' to group B in this forcefield, with parameters contained in 'cljtable' */
// void InterGroupCLJFF::addToB( const MoleculeSet &mols, const CLJTable &cljtable )
// {
//     int nmols = mols.count();
//     for (int i=0; i<nmols; ++i)
//         this->addToB(mols[i], cljtable);
// }
// 
// /** Add the Molecules in 'mols' to group B in this forcefield, with (possibly perturbable) parameters
//     contained in 'mutator' */
// void InterGroupCLJFF::addToB( const MoleculeSet &mols, const CLJMutator &mutator )
// {
//     int nmols = mols.count();
//     for (int i=0; i<nmols; ++i)
//         this->addToB(mols[i], mutator);
// }
// 
// /** Add the residue 'res' to group B in this forcefield, with parameters contained in 'cljtable' */
// void InterGroupCLJFF::addToB( const ResiduePtr &res, const CLJTable &cljtable )
// {
//     if (res.get() == 0)
//         return;
//         
//     //ensure that the molecule containing this residue
//     //is not being split over two groups
//     checkNotInGroupA(res->molecule());
// 
//     this->addToB(res->cutGroup(), cljtable);
// }
// 
// /** Add the residue 'res' to group B in this forcefield, with (possibly perturbable) parameters
//     contained in the CLJMutator 'mutator' */
// void InterGroupCLJFF::addToB( const ResiduePtr &res, const CLJMutator &mutator )
// {
//     if (res.get() == 0)
//         return;
//         
//     checkNotInGroupA(res->molecule());
//     this->addToB(res->cutGroup(), mutator);
// }
// 
// /** Add all of the residues in 'ress' to group B in this forcefield, using the parameters in 
//     the CLJTable 'cljtable' */
// void InterGroupCLJFF::addToB( const ResidueSet &ress, const CLJTable &cljtable )
// {
//     int nres = ress.count();
//     for (int i=0; i<nres; ++i)
//         this->addToB(ress[i], cljtable);
// }
// 
// /** Add all of the residues in 'ress' to group B in this forcefield, using the
//     (possibly perturbable) parameters in 'mutator' */
// void InterGroupCLJFF::addToB( const ResidueSet &ress, const CLJMutator &mutator )
// {
//     int nres = ress.count();
//     for (int i=0; i<nres; ++i)
//         this->addToB(ress[i], mutator);
// }
//     
// /** Return the total energy of the CutGroups in this forcefield at the lambda state 'stat' */
// CLJEnergy InterGroupCLJFF::energy(const LambdaState &stat) const
// {
//     if (group_a.count() == 0 or group_b.count() == 0)
//         return CLJEnergy();
// 
//     //as an optimisation, we ensure that the outer loop is over the smaller set
//     if (group_a.count() >= group_b.count())
//         return groupGroupEnergy(group_b, group_a, stat);
//     else
//         return groupGroupEnergy(group_a, group_b, stat);
// }
//  
// /** Return the energy of the CutGroups in 'groups' in this forcefield at the lambda state 'stat'.
//     This only calculates the energies of the CutGroups that have been added to this forcefield. */
// CLJEnergy InterGroupCLJFF::energy(const CutGroupSet &groups, const LambdaState &stat) const
// {
//     if (group_a.count() == 0 or group_b.count() == 0)
//         return CLJEnergy();
//     
//     //get the CutGroups in 'groups' that are also in group A
//     CutGroupSet groups_in_a = groups.intersection(group_a);
//     //get the CutGroups in 'groups' that are also in group B
//     CutGroupSet groups_in_b = groups.intersection(group_b);
//     
//     int ngroups_in_a = groups_in_a.count();
//     int ngroups_in_b = groups_in_b.count();
//     
//     if (ngroups_in_a == 0 and ngroups_in_b == 0)
//     {
//         //none of 'groups' are in this forcefield - they thus have
//         //zero energy in this forcefield
//         return CLJEnergy();
//     }
//     else if (ngroups_in_b == 0)
//     {
//         //we only need to calculate the energy of groups_in_a with 
//         //all of the groups in group b
//         if (ngroups_in_a >= group_b.count())
//             return groupGroupEnergy(group_b, groups_in_a, stat);
//         else
//             return groupGroupEnergy(groups_in_a, group_b,  stat);
//     }
//     else if (ngroups_in_a == 0)
//     {
//         //we only need to calculate the energy of groups_in_b with
//         //all of the groups in group a
//         if (ngroups_in_b >= group_a.count())
//             return groupGroupEnergy(group_a, groups_in_b, stat);
//         else
//             return groupGroupEnergy(groups_in_b, group_a, stat);
//     }
//     else
//     {
//         //we need to calculate the energy of groups_in_a with all of group_b...
//         double cnrg(0.0), ljnrg(0.0);
//         DistMatrix distmat;
//         CLJPairMatrix cljmat;
//         
//         int ngroup_a = group_a.count();
//         int ngroup_b = group_b.count();
//         
//         for (int i=0; i<ngroups_in_a; ++i)
//         {
//             const CutGroup &group0 = *(groups_in_a.at(i));
//             const CLJWindow &clj0 = this->getParameters(group0.ID(), stat);
//             
//             for (int j=0; j<ngroup_b; ++j)
//             {
//                 const CutGroup &group1 = *(group_b.at(j));
//                 
//                 if (not volptr->beyond(nbcut, group0, group1, stat))
//                 {
//                     double icnrg, iljnrg;
//                     cljEnergy(group0, group1, clj0, stat, distmat, cljmat, icnrg, iljnrg);
//                     
//                     cnrg += icnrg;
//                     ljnrg += iljnrg;
//                 }
//             }
//         }
//         
//         //add on to this the energy of groups_in_b with all of group_a except
//         //for groups_in_a
//         if (ngroup_a > ngroups_in_a)
//         {
//             for (int i=0; i<ngroups_in_b; ++i)
//             {
//                 const CutGroup &group0 = *(groups_in_b.at(i));
//                 const CLJWindow &clj0 = this->getParameters(group0.ID(),stat);
//             
//                 for (int j=0; j<ngroup_a; ++j)
//                 {
//                     const CutGroup &group1 = *(group_a.at(j));
//                     
//                     if (not groups_in_a.has_key(group1.ID()) and 
//                                not volptr->beyond(nbcut, group0, group1, stat))
//                     {
//                         double icnrg, iljnrg;
//                         cljEnergy(group0, group1, clj0, stat, distmat, cljmat, icnrg, iljnrg);
//                         
//                         cnrg += icnrg;
//                         ljnrg += iljnrg;
//                     }
//                 }
//             }
//         }
//         
//         return CLJEnergy(cnrg, ljnrg);
//     }
// }
//  
// CLJEnergy InterGroupCLJFF::deltaEnergy(const LambdaState &stat0, const LambdaState &stat1) const
// {
//     if (pertgroups.isEmpty())
//         return CLJEnergy();
//     else
//         //return the difference in energy of the perturbable groups at the two lambda states
//         return energy(pertgroups, stat0) - energy(pertgroups, stat1);
// }
// 
// CLJEnergy InterGroupCLJFF::deltaEnergy(const CutGroupSet &groups, const LambdaState &stat0,
//                                        const LambdaState &stat1) const
// {
//     if (pertgroups.isEmpty() or stat0 == stat1 or group_a.isEmpty() or group_b.isEmpty())
//         return CLJEnergy();
//         
//     // We split 'groups' up into four sets;
//     //
//     //    (1) Perturbable CutGroups in group A              (pert_in_a)
//     //    (2) Perturbable CutGroups in group B              (pert_in_b)
//     //    (3) Non-perturbable CutGroups in group A          (nonpert_in_a)
//     //    (4) Non-perturbable CutGroups in group B          (nonpert_in_b)
//     //        
//     // delta energy is  pert_in_a with all of group B + 
//     //                  pert_in_b with all of A except for pert_in_a + 
//     //                  nonpert_in_a with all perturbable groups in B except for pert_in_b + 
//     //                  nonpert_in_b with all perturbable groups in A except for pert_in_a
//     
//     //split 'groups' into the set of CutGroups that are in group A, and the 
//     //set of CutGroups that are in group B.
//     CutGroupSet groups_in_a = groups.intersection(group_a);
//     CutGroupSet groups_in_b = groups.intersection(group_b);
// 
//     if (groups_in_a.count() == 0 and groups_in_b.count() == 0)
//         return CLJEnergy();
// 
//     //now split these two CutGroupSets into the four groups mentioned above...
//     boost::tuple<CutGroupSet,CutGroupSet> tuple_a = groups_in_a.splitIntoSets(pertgroups_a);
//     boost::tuple<CutGroupSet,CutGroupSet> tuple_b = groups_in_b.splitIntoSets(pertgroups_b);
//     
//     CutGroupSet pert_in_a = tuple_a.get<0>();
//     CutGroupSet nonpert_in_a = tuple_a.get<1>();
//     
//     CutGroupSet pert_in_b = tuple_b.get<0>();
//     CutGroupSet nonpert_in_b = tuple_b.get<1>();
//     
//     //create temporary variables and a workspace for the calculation
//     double deltacnrg(0.0), deltaljnrg(0.0);
//     DistMatrix distmat;
//     CLJPairMatrix cljmat;
//     
//     int npert_in_a = pert_in_a.count();
//     int npert_in_b = pert_in_b.count();
//     int nnonpert_in_a = nonpert_in_a.count();
//     int nnonpert_in_b = nonpert_in_b.count();
//     
//     int ngroup_a = group_a.count();
//     int ngroup_b = group_b.count();
//     int npertgroups_a = pertgroups_a.count();
//     int npertgroups_b = pertgroups_b.count();
//     
//     //do the first part of the calculation, pert_in_a with all of group B
//     if (npert_in_a > 0)
//     {
//         for (int i=0; i<npert_in_a; ++i)
//         {
//             const CutGroup &group0 = *(pert_in_a.at(i));
//             const CLJWindow &clj0_0 = this->getParameters(group0.ID(), stat0);
//             const CLJWindow &clj0_1 = this->getParameters(group0.ID(), stat1);
//             
//             for (int j=0; j<ngroup_b; ++j)
//             {
//                 const CutGroup &group1 = *(group_b.at(j));
//                 double icnrg, iljnrg;
//                 
//                 //add on the energy for lambda state 'stat1' to the delta...
//                 if (not volptr->beyond(nbcut, group0, group1, stat1))
//                 {
//                     cljEnergy(group0, group1, clj0_1, stat1, distmat, cljmat, icnrg, iljnrg);
//                     deltacnrg += icnrg;
//                     deltaljnrg += iljnrg;
//                 }
//                 
//                 //now subtract the energy for lambda state 'stat0' from the delta...
//                 if (not volptr->beyond(nbcut, group0, group1, stat0))
//                 {
//                     cljEnergy(group0, group1, clj0_0, stat0, distmat, cljmat, icnrg, iljnrg);
//                     deltacnrg -= icnrg;
//                     deltaljnrg -= iljnrg;
//                 }
//             }
//         }
//     }
//     
//     //do the second part of the calculation, pert_in_b with all of group A except for pert_in_a
//     if (npert_in_b > 0 and (ngroup_a - npert_in_a) > 0)
//     {
//         for (int i=0; i<npert_in_b; ++i)
//         {
//             const CutGroup &group0 = *(pert_in_b.at(i));
//             const CLJWindow &clj0_0 = this->getParameters(group0.ID(), stat0);
//             const CLJWindow &clj0_1 = this->getParameters(group0.ID(), stat1);
//             
//             for (int j=0; j<ngroup_a; ++j)
//             {
//                 const CutGroup &group1 = *(group_a.at(j));
//                 
//                 if (not pert_in_a.has_key(group1.ID()))
//                 {
//                     double icnrg, iljnrg;
//                 
//                     if (not volptr->beyond(nbcut, group0, group1, stat1))
//                     {
//                         cljEnergy(group0, group1, clj0_1, stat1, distmat, cljmat, icnrg, iljnrg);
//                         deltacnrg += icnrg;
//                         deltaljnrg += iljnrg;
//                     }
//                     
//                     if (not volptr->beyond(nbcut, group0, group1, stat0))
//                     {
//                         cljEnergy(group0, group1, clj0_0, stat0, distmat, cljmat, icnrg, iljnrg);
//                         deltacnrg -= icnrg;
//                         deltaljnrg -= iljnrg;
//                     }
//                 }
//             }
//         }
//     }
//     
//     //now the third part of the calculation - nonpert_in_a with all perturbable 
//     //groups in B except for pert_in_b
//     if (nnonpert_in_a > 0 and (npertgroups_b - npert_in_b) > 0)
//     {
//         for (int i=0; i<nnonpert_in_a; ++i)
//         {
//             const CutGroup &group0 = *(nonpert_in_a.at(i));
//             const CLJWindow &clj0_0 = this->getParameters(group0.ID(), stat0);
//             const CLJWindow &clj0_1 = this->getParameters(group0.ID(), stat1);
//             
//             for (int j=0; j<npertgroups_b; ++j)
//             {
//                 const CutGroup &group1 = *(pertgroups_b.at(j));
//                 
//                 if (not pert_in_b.has_key(group1.ID()))
//                 {
//                     double icnrg, iljnrg;
//                     
//                     if (not volptr->beyond(nbcut, group0, group1, stat1))
//                     {
//                         cljEnergy(group0, group1, clj0_1, stat1, distmat, cljmat, icnrg, iljnrg);
//                         deltacnrg += icnrg;
//                         deltaljnrg += iljnrg;
//                     }
//                     
//                     if (not volptr->beyond(nbcut, group0, group1, stat0))
//                     {
//                         cljEnergy(group0, group1, clj0_0, stat0, distmat, cljmat, icnrg, iljnrg);
//                         deltacnrg -= icnrg;
//                         deltaljnrg -= iljnrg;
//                     }
//                 }
//             }
//         }
//     }
//     
//     //finally, the fourth part of the calculation - nonpert_in_b with all perturbable groups 
//     //in A except for pert_in_a
//     if (nnonpert_in_b > 0 and (npertgroups_a - npert_in_a) > 0)
//     {
//         for (int i=0; i<nnonpert_in_b; ++i)
//         {
//             const CutGroup &group0 = *(nonpert_in_b.at(i));
//             const CLJWindow &clj0_0 = this->getParameters(group0.ID(), stat0);
//             const CLJWindow &clj0_1 = this->getParameters(group0.ID(), stat1);
//             
//             for (int j=0; j<npertgroups_a; ++j)
//             {
//                 const CutGroup &group1 = *(pertgroups_a.at(j));
//                 
//                 if (not pert_in_a.has_key(group1.ID()))
//                 {
//                     double icnrg, iljnrg;
//                     
//                     if (not volptr->beyond(nbcut, group0, group1, stat1))
//                     {
//                         cljEnergy(group0, group1, clj0_1, stat1, distmat, cljmat, icnrg, iljnrg);
//                         deltacnrg += icnrg;
//                         deltaljnrg += iljnrg;
//                     }
//                     
//                     if (not volptr->beyond(nbcut, group0, group1, stat0))
//                     {
//                         cljEnergy(group0, group1, clj0_0, stat0, distmat, cljmat, icnrg, iljnrg);
//                         deltacnrg -= icnrg;
//                         deltaljnrg -= iljnrg;
//                     }
//                 }
//             }
//         }
//     }
//     
//     //we are all done!
//     return CLJEnergy(deltacnrg, deltaljnrg);
// }
// 
// /** Calculate the interaction energy of the CutGroups in 'groups0' with those in 'groups1' - this
//     is an internal function, and you should have already guaranteed that no one CutGroup is 
//     in both groups, and that all of the CutGroups are in this forcefield */
// CLJEnergy InterGroupCLJFF::groupGroupEnergy(const CutGroupSet &groups0, 
//                                             const CutGroupSet &groups1,
//                                             const LambdaState &stat) const
// {
//     //loop over all group-group pairs
//     int n0 = groups0.count();
//     int n1 = groups1.count();
//     
//     //create workspace and tmp variables
//     double cnrg(0.0), ljnrg(0.0);
//     DistMatrix distmat;
//     CLJPairMatrix cljmat;
//     
//     for (int i=0; i<n0; ++i)
//     {
//         const CutGroup &group0 = *(groups0.at(i));
//         const CLJWindow &clj0 = this->getParameters(group0.ID(), stat);
//         
//         for (int j=0; j<n1; ++j)
//         {
//             const CutGroup &group1 = *(groups1.at(j));
//             
//             if (not volptr->beyond(nbcut, group0, group1, stat))
//             {
//                 double icnrg, iljnrg;
//                 this->cljEnergy(group0, group1, clj0, stat, distmat, cljmat, icnrg, iljnrg);
//                 
//                 cnrg += icnrg;
//                 ljnrg += iljnrg;
//             }
//         }
//     }
//     
//     return CLJEnergy(cnrg, ljnrg);
// }
