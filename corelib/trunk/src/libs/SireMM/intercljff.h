// #ifndef SIREFF_INTERCLJFF_H
// #define SIREFF_INTERCLJFF_H
// 
// #include "SireMol/moleculeptr.h"
// #include "SireMol/moleculeset.h"
// #include "SireMol/residueptr.h"
// #include "SireMol/residueset.h"
// 
// #include "cljff.h"
// #include "ffwrapperptr.h"
// 
// namespace SireFF
// {
// class InterCLJFF;
// }
// 
// QDataStream& operator<<(QDataStream&, const SireFF::InterCLJFF&);
// QDataStream& operator>>(QDataStream&, SireFF::InterCLJFF&);
// 
// namespace SireFF
// {
// 
// using SireMol::MoleculePtr;
// using SireMol::MoleculeSet;
// using SireMol::ResiduePtr;
// using SireMol::ResidueSet;
// 
// /** An InterCLJFF is a forcefield that calculates the intermolecular coulomb and
//     Lennard Jones energies of all contained molecules. An InterCLJFF is perhaps
//     the most expensive type of MM forcefield, as it must calculate the full
//     pair-pair interactions between all pairs of molecules in the forcefield
//     that are within the cutoff distance. 
//     
//     The forcefield can calculate energies or forces. It only calculates the
//     values and returns them - it does not do any clever cacheing of energies
//     or individual energy components. This is left to the FFGroup (forcefield
//     group) that holds this forcefield. It is the FFGroup that takes the
//     returned energies (CLJEnergy in this case) and works out if they are
//     'new' or 'old' (if MC) or should be added onto running totals etc.
//     
//     You can get pretty much any component you wish by using multiple
//     individual forcefields, each one calculating part of the component.
//     
//     e.g. to calculate the components of protein / ligand / solvent
//     system, then instead of using a single InterCLJFF for the whole
//     system (which would be fastest, but least informative), use;
//     
//     (1) InterCLJFF for the solvent  - gets solvent-solvent component
//     (2) IntraMMFF  for the protein  - gets protein intra-molecular energy
//     (3) IntraMMFF  for the solute   - gets the solute intra-molecular energy
//     (4) CLJ2CLJFF  for solute/solvent - calculates CLJ between the solute and solvent
//     (5) CLJ2CLJFF  for the solute/protein - calculates the CLJ between the solute and protein
//     (6) CLJ2CLJFF  for the protein/solvent - calculates the CLJ between the protein and solvent
//     
//     If you are doing dual topology, then you could scale the solute/protein and solute/solvent
//     forcefields by a function of lambda (see SireMaths::MathFunc), while scaling in 
//     another ligands forcefield. 
//     
//     \author Christopher Woods
// */
// class InterCLJFF : public CLJFF
// {
// 
// friend QDataStream& ::operator<<(QDataStream&, const InterCLJFF&);
// friend QDataStream& ::operator>>(QDataStream&, InterCLJFF&);
// 
// public:
//     InterCLJFF();
//     InterCLJFF(const InterCLJFF &other);
//     ~InterCLJFF();
// 
//     const char* what() const
//     {
//         return "SireFF::InterCLJFF";
//     }
// 
//     void add( const MoleculePtr &mol, const CLJTable &cljtable );
//     void add( const MoleculePtr &mol, const CLJMutator &mutator );    
//     void add( const MoleculeSet &mols, const CLJTable &cljtable );
//     void add( const MoleculeSet &mols, const CLJMutator &mutator );
//     
//     void add( const ResiduePtr &res, const CLJTable &cljtable );
//     void add( const ResiduePtr &res, const CLJMutator &mutator );
//     void add( const ResidueSet &ress, const CLJTable &cljtable );
//     void add( const ResidueSet &ress, const CLJMutator &mutator );
//     
//     const CutGroupSet& cutGroups() const;
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
//     void add( const CutGroupPtr &group, const CLJTable &cljtable );
//     void add( const CutGroupPtr &group, const CLJMutator &mutator );
//     void add( const CutGroupSet &groups, const CLJTable &cljtable );
//     void add( const CutGroupSet &groups, const CLJMutator &mutator );
// 
//     CLJEnergy intraGroupEnergy(const CutGroupSet &cgroups, const LambdaState &stat,
//                                DistMatrix &distmat, CLJPairMatrix &cljmat) const;
//     
//     CLJEnergy interGroupEnergy(const CutGroupSet &cgroups, const LambdaState &stat,
//                                DistMatrix &distmat, CLJPairMatrix &cljmat) const;
//     
//     CLJEnergy groupGroupEnergy(const CutGroupSet &groups0, const CutGroupSet &groups1,
//                                const LambdaState &stat, DistMatrix &distmat,
//                                CLJPairMatrix &cljmat) const;
// 
//     /** Set of all CutGroups that are in this forcefield */
//     CutGroupSet allgroups;
// };
// 
// /** Return the total energy of this forcefield in the reference state */
// inline CLJEnergy InterCLJFF::energy() const
// {
//     return this->energy(LAM_REFERENCE);
// }
// 
// /** Return the energy of the CutGroups in 'cgroups' that are in this forcefield, 
//     in the reference state. */
// inline CLJEnergy InterCLJFF::energy(const CutGroupSet &cgroups) const
// {
//     return this->energy(cgroups, LAM_REFERENCE);
// }
// 
// /** Return the set of all CutGroups that are in this forcefield */
// inline const CutGroupSet& InterCLJFF::cutGroups() const
// {
//     return allgroups;
// }
// 
// }
// 
// 
// #endif
