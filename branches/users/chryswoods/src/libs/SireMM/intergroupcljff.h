/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

// #ifndef SIREFF_INTERGROUPCLJFF_H
// #define SIREFF_INTERGROUPCLJFF_H
// 
// #include "cljff.h"
// #include "ffwrapperptr.h"
// 
// #include "SireMol/moleculeptr.h"
// #include "SireMol/moleculeset.h"
// #include "SireMol/residueptr.h"
// #include "SireMol/residueset.h"
// 
// #include <QSet>
// 
// namespace SireFF
// {
// class InterGroupCLJFF;
// }
// 
// QDataStream& operator<<(QDataStream&, const SireFF::InterGroupCLJFF&);
// QDataStream& operator>>(QDataStream&, SireFF::InterGroupCLJFF&);
// 
// namespace SireFF
// {
// 
// using SireMol::MoleculePtr;
// using SireMol::MoleculeSet;
// using SireMol::MoleculeID;
// using SireMol::ResiduePtr;
// using SireMol::ResidueSet;
// 
// /**
// This forcefield is used to calculate the CLJ (charge / Lennard Jones) interaction energy between two groups. It does not calculate the energy within the groups - just the energy between the groups. Note that a single molecule *may not* be split over the two groups - they are strictly intermolecular groups.
//  
// The two groups are labelled 'A' and 'B' (imaginative). Molecules or residues can be added to group A via the "addToA" series of functions, or can be added to group B via the "addToB" series of functions. 
//  
// @author Christopher Woods
// */
// class InterGroupCLJFF : public CLJFF
// {
// 
// friend QDataStream& ::operator<<(QDataStream&, const InterGroupCLJFF&);
// friend QDataStream& ::operator>>(QDataStream&, InterGroupCLJFF&);
// 
// public:
//     InterGroupCLJFF();
//     InterGroupCLJFF(const InterGroupCLJFF &other);
//     ~InterGroupCLJFF();
// 
//     const char* what() const
//     {
//         return "SireFF::InterGroupCLJFF";
//     }
// 
//     CutGroupSet cutGroups() const;
//  
//     const CutGroupSet& groupA() const;
//     const CutGroupSet& groupB() const;
//  
//     void addToA( const MoleculePtr &mol, const CLJTable &cljtable );
//     void addToA( const MoleculePtr &mol, const CLJMutator &mutator );    
//     void addToA( const MoleculeSet &mols, const CLJTable &cljtable );
//     void addToA( const MoleculeSet &mols, const CLJMutator &mutator );
//     
//     void addToA( const ResiduePtr &res, const CLJTable &cljtable );
//     void addToA( const ResiduePtr &res, const CLJMutator &mutator );
//     void addToA( const ResidueSet &ress, const CLJTable &cljtable );
//     void addToA( const ResidueSet &ress, const CLJMutator &mutator );
//  
//     void addToB( const MoleculePtr &mol, const CLJTable &cljtable );
//     void addToB( const MoleculePtr &mol, const CLJMutator &mutator );    
//     void addToB( const MoleculeSet &mols, const CLJTable &cljtable );
//     void addToB( const MoleculeSet &mols, const CLJMutator &mutator );
//     
//     void addToB( const ResiduePtr &res, const CLJTable &cljtable );
//     void addToB( const ResiduePtr &res, const CLJMutator &mutator );
//     void addToB( const ResidueSet &ress, const CLJTable &cljtable );
//     void addToB( const ResidueSet &ress, const CLJMutator &mutator );
//     
//     CLJEnergy energy() const;
//     CLJEnergy energy(const LambdaState &stat) const;
//  
//     CLJEnergy energy(const CutGroupSet &groups) const;
//     CLJEnergy energy(const CutGroupSet &groups, const LambdaState &stat) const;
//  
//     CLJEnergy deltaEnergy(const LambdaState &stat0, const LambdaState &stat1) const;
//     CLJEnergy deltaEnergy(const CutGroupSet &groups, const LambdaState &stat0,
//                           const LambdaState &stat1) const;
// 
// //     void addForce(double scl, ForceTableSet &workspace) const;
// //     void addForce(double scl, ForceTableSet &workspace, const LambdaState &stat) const;
// //     
// //     void addForce(double scl, ForceTableSet &workspace, const CutGroupSet &groups) const;
// //     void addForce(double scl, ForceTableSet &workspace, const CutGroupSet &groups,
// //                   const LambdaState &stat) const;
// 
//     operator FFWrapperPtr() const;
// 
// protected:
// 
//     void checkNotInGroupA(const MoleculePtr &mol) const;
//     void checkNotInGroupB(const MoleculePtr &mol) const;
//     
//     CLJEnergy groupGroupEnergy(const CutGroupSet &groups0, const CutGroupSet &groups1,
//                                const LambdaState &stat) const;
//     
//     void addToA( const CutGroupPtr &cgroup, const CLJTable &cljtable );
//     void addToA( const CutGroupPtr &cgroup, const CLJMutator &mutator );
//     void addToA( const CutGroupSet &cgroups, const CLJTable &cljtable );
//     void addToA( const CutGroupSet &cgroups, const CLJMutator &mutator );
//     
//     void addToB( const CutGroupPtr &cgroup, const CLJTable &cljtable );
//     void addToB( const CutGroupPtr &cgroup, const CLJMutator &mutator );
//     void addToB( const CutGroupSet &cgroups, const CLJTable &cljtable );
//     void addToB( const CutGroupSet &cgroups, const CLJMutator &mutator );
// 
//     /** The set of CutGroups in group A */
//     CutGroupSet group_a;
//     /** The set of CutGroups in group B */
//     CutGroupSet group_b;
//     
//     /** The set of perturbable CutGroups in group A */
//     CutGroupSet pertgroups_a;
//     /** The set of perturbable CutGroups in group B */
//     CutGroupSet pertgroups_b;
//     
//     /** ID numbers of the molecules in group A */
//     QSet<MoleculeID> group_a_mols;
//     /** ID numbers of the molecules in group B */
//     QSet<MoleculeID> group_b_mols;
// 
// };
// 
// /** Return the total energy of this forcefield at the reference state */
// inline CLJEnergy InterGroupCLJFF::energy() const
// {
//     return energy(LAM_REFERENCE);
// }
// 
// /** Return the total energy of 'groups' in this forcefield at the reference state. 
//     This will only return the energy of those CutGroups that have been added to this forcefield */
// inline CLJEnergy InterGroupCLJFF::energy(const CutGroupSet &groups) const
// {
//     return energy(groups, LAM_REFERENCE);
// }
// 
// /** Return the set of CutGroups in group A */
// inline const CutGroupSet& InterGroupCLJFF::groupA() const
// {
//     return group_a;
// }
// 
// /** Return the set of CutGroups in group B */
// inline const CutGroupSet& InterGroupCLJFF::groupB() const
// {
//     return group_b;
// }
// 
// /** Return the set of all CutGroups in this forcefield */
// inline CutGroupSet InterGroupCLJFF::cutGroups() const
// {
//     return group_a + group_b;
// }
// 
// }
// 
// #endif
