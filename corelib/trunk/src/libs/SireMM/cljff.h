// #ifndef SIREFF_CLJFF_H
// #define SIREFF_CLJFF_H
// 
// #include <boost/shared_ptr.hpp>
// 
// #include "SireMol/cutgroupptr.h"
// #include "SireMol/cutgroupset.h"
// #include "SireVol/simvolumeptr.h"
// #include "SireVol/simvolume.h"
// 
// #include "energytypes.h"
// #include "cljtable.h"
// #include "cljmutator.h"
// 
// #include "forcefield.h"
// 
// namespace SireFF
// {
// class CLJFF;
// }
// 
// namespace SireMol
// {
// inline bool sameMolecule(const CutGroup&, const CutGroup&)
// {
//     return false;
// }
// }
// 
// QDataStream& operator<<(QDataStream&, const SireFF::CLJFF&);
// QDataStream& operator>>(QDataStream&, SireFF::CLJFF&);
// 
// namespace SireFF
// {
// 
// using SireVol::SimVolume;
// using SireVol::SimVolumePtr;
// using SireVol::DistMatrix;
// 
// using SireMol::CutGroupPtr;
// using SireMol::CutGroupSet;
// using SireMol::CutGroupID;
// using SireMol::CutGroup;
// 
// class CLJFF;
// typedef void (CLJFF::*cljCombineFunc)(const CLJWindow&, const CLJWindow&, CLJPairMatrix&) const;
// 
// /** Small function used to find the smallest CutGroupSet out of a pair of sets */
// inline const CutGroupSet& smallestCutGroupSet(const CutGroupSet &g0, const CutGroupSet &g1)
// {
//     if (g0.count() <= g1.count())
//         return g0;
//     else
//         return g1;
// }
// 
// /** Small function used to find the largest CutGroupSet out of a pair of sets */
// inline const CutGroupSet& largestCutGroupSet(const CutGroupSet &g0, const CutGroupSet &g1)
// {
//     if (g1.count() > g0.count())
//         return g1;
//     else
//         return g0;
// }
// 
// /**
// A CLJFF is a non-visible base-class for all forcefields that implement a charge/Lennard Jones (CLJ) forcefield. This provides basic functions for calculating the CLJ energy of pairs of CutGroups.
//  
// @author Christopher Woods
// */
// class CLJFF : public ForceField
// {
// 
// friend QDataStream& ::operator<<(QDataStream&, const CLJFF&);
// friend QDataStream& ::operator>>(QDataStream&, CLJFF&);
// 
// public:
// 
//     /** Define the type of energy stored and returned by
//         CLJ forcefields */
//     typedef CLJEnergy _ENERGY_TYPE_;
// 
//     CLJFF();
//     CLJFF(const CLJFF &other);
//     
//     ~CLJFF();
//     
//     const char* what() const
//     {
//         return "SireFF::CLJFF";
//     }
//     
//     void setVolume(const SimVolume &vol);
//     void setLambda(const Lambda &lam);
// 
//     void setCutoff(double cut);
//     void setCutoff(double cut, double feather);
//     void setFeather(double feath);
//     
//     const double& cutoff() const;
//     const double& cutoff2() const;
//     const double& feather() const;
//     const double& feather2() const;
// 
//     bool hasPerturbableParams(const CutGroupID &id) const;
// 
// protected:
//     
//     void combineGeometric(const CLJWindow &cljwindow0, const CLJWindow &cljwindow1, 
//                           CLJPairMatrix &cljmat) const;
//     void combineArithmetic(const CLJWindow &cljwindow0, const CLJWindow &cljwindow1,
//                            CLJPairMatrix &cljmat) const;
// 
//     void setParameters(const CutGroupPtr &group, const CLJTable &params);
//     void setParameters(const CutGroupPtr &group, const CLJMutator &mutator);
// 
//     void setParameters(const CutGroupID &id, const CLJWindows &windows);
//     const CLJWindow& getParameters(const CutGroupID &id, const LambdaState &stat) const;
// 
//     virtual double featherFactor(double dist2) const;
//     
//     void cljEnergy(const CutGroup &group0, const CutGroup &group1,
//                         const CLJWindow &clj0, const LambdaState &lamstate,
//                         DistMatrix &distmat, CLJPairMatrix &cljmat,
//                         double &cnrg, double &ljnrg) const;
// 
//     /** Hash of all of the CLJ parameters for the groups in this forcefield,
//         indexed by CutGroupID */
//     QHash< CutGroupID, CLJWindows* > cljparams;
//     
//     /** Hash of all of the mutators for perturbable cutgroup, indexed by 
//         CutGroupID */
//     QHash< CutGroupID, CLJMutatorPtr > cljmutators;
//     
//     /** Set of all of the cutgroups that are perturbable in this forcefield */
//     CutGroupSet pertgroups;
//     
//     /** The current value of lambda for this forcefield */
//     SireBase::Lambda lambda;
// 
//     /** Pointer to the function used to combine together CLJ parameters */
//     cljCombineFunc combineCLJ;
// 
//     /** Pointer to the volume for this forcefield */
//     SimVolumePtr volptr;
// 
//     /** The non-bonded cutoff distance used within this forcefield */
//     double nbcut;
//     /** The square of the cutoff */
//     double nbcut2;
//     
//     /** The non-bonded feather distance used within this forcefield.
//         (if this cutoff is 15, then feathering over the last 0.5
//         would require the feather to be 14.5) - this must be 
//         less than nbcut. */
//     double nbfeather;
//     /** The square of the feather distance*/
//     double nbfeather2;
// 
// };
// 
// /** Set the cutoff to 'cutdist' */
// inline void CLJFF::setCutoff(double cutdist)
// {
//     this->setCutoff(cutdist, nbcut - nbfeather);
// }
// 
// /** Set the feather distance to 'feather'. This is the distance over which the nonbonded
//     interactions are feathered */
// inline void CLJFF::setFeather(double feather)
// {
//     this->setCutoff(nbcut, feather);
// }
// 
// /** Return the non-bonded cutoff associated with this forcefield */
// inline const double& CLJFF::cutoff() const
// {
//     return nbcut;
// }
// 
// /** Return the square of the cutoff */
// inline const double& CLJFF::cutoff2() const
// {
//     return nbcut2;
// }
// 
// /** Return the non-bonded feather associated with this forcefield. This is the 
//     length beyond which the non-bonded interactions are feathered, e.g.
//     if the cutoff is 15 A, and the non-bonded interactions are feathered
//     over the last 0.5 A, then the feather distance is equal to 14.5 A */
// inline const double& CLJFF::feather() const
// {
//     return nbfeather;
// }
// 
// /** Return the square of the feather */
// inline const double& CLJFF::feather2() const
// {
//     return nbfeather2;
// }
// 
// /** Set the parameters for the CutGroup with ID = id */
// inline void CLJFF::setParameters(const CutGroupID &id, const CLJWindows &windows)
// {
//     //delete any pre-existing value
//     delete cljparams.value(id,0);
//     
//     cljparams.insert(id, new CLJWindows(windows) );
// }
//     
// /** Return a const reference to the parameters for the CutGroup with ID 'id' at
//     lambda state 'stat'. The parameters must exist or else we will crash! */
// inline const CLJWindow& CLJFF::getParameters(const CutGroupID &id, const LambdaState &stat) const
// {
//     CLJWindows *windows = cljparams.value(id,0);
//     
//     if (windows == 0)
//         throw SireError::nullptr_error(QObject::tr(
//             "CutGroup ID = %1 does not have a valid CLJWindow!").arg(id), CODELOC);
//     
//     return windows->get(stat);
// }
// 
// /** Return whether or not this CutGroup with ID 'id' has perturbable forcefield
//     parameters in this forcefield */
// inline bool CLJFF::hasPerturbableParams(const CutGroupID &id) const
// {
//     return pertgroups.has_key(id);
// }
// 
// }
// 
// #endif
