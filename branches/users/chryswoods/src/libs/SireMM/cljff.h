// #ifndef SIREMM_CLJFF_H
// #define SIREMM_CLJFF_H
// 
// #include "SireFF/forcefield.h"
// #include "SireMaths/maths.h"
// #include "SireVol/simvolume.h"
// #include "SireUnits/units.h"
// 
// #include "cljpair.h"
// 
// namespace SireFF
// {
// 
// using SireFF::ForceField;
// using SireVol::DistMatrix;
// 
// /** This inline function is used to calculate the charge and LJ energy
//     of two molecules based on the inter-atomic distances stored in 'distmatrix'
//     and using the combined CLJ parameters listed in 'cljmatrix'. The
//     charge and LJ energy are added onto the references 'chgnrg' and 'ljnrg'
//     as I have found that this is the fastest method of returning a pair of
//     values from a function. */
// inline void calculateCLJEnergy(DistMatrix &distmat, CLJPairMatrix &cljmatrix,
//                                double &chgnrg, double &ljnrg)
// {
//     int nats0 = distmatrix.nOuter();
//     int nats1 = distmatrix.nInner();
//     
//     //loop over all pairs of atoms
//     for (int i=0; i<nats0; ++i)
//     {
//         distmatrix.setOuterIndex(i);
//         cljmatrix.setOuterIndex(i);
//         
//         for (int j=0; j<nats1; ++j)
//         {
//             //get the distance and CLJPair for this atom pair
//             const double &invdist2 = distmat[j];
//             const CLJPair &cljpair = cljmat[j];
//                
//             double sig2_over_dist2 = SireMaths::pow_2(cljpair.sigma()) * invdist2;
//             double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
//             double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);
//                 
//             //coulomb energy
//             chgnrg += SireUnits::one_over_four_pi_eps0 * 
//                                        cljpair.charge2() * std::sqrt(invdist2);
//                                       
//             //LJ energy
//             ljnrg += 4.0 * cljpair.epsilon() * 
//                                      ( sig12_over_dist12 - sig6_over_dist6 );
//         }
//     }
// }
// 
// /**
// A CLJFF is the base-class for all forcefields that implement a charge/Lennard Jones (CLJ) forcefield. This provides basic functions for calculating the CLJ energy of pairs of CutGroups.
//  
// @author Christopher Woods
// */
// class CLJFF : public ForceField
// {
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
// protected:
//     
//     void setVolume(const SimVolume &vol);
//     
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
