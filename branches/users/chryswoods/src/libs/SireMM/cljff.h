#ifndef SIREMM_CLJFF_H
#define SIREMM_CLJFF_H

#include "SireFF/ffworker.h"
#include "SireMaths/maths.h"

#include "SireVol/simvolume.h"
#include "SireUnits/units.h"

#include "cljpair.h"

namespace SireMM
{

using SireFF::ForceField;
using SireVol::DistMatrix;

/**
A CLJFF is the base-class for all forcefields that implement a charge/Lennard Jones (CLJ) forcefield. This provides basic functions for calculating the CLJ energy of pairs of CutGroups.
 
@author Christopher Woods
*/
class CLJFF : public FFWorker
{

public:
    CLJFF();
    CLJFF(const CLJFF &other);
    
    ~CLJFF();
    
protected:
    static void calculateEnergy(DistMatrix &distmat, CLJPairMatrix &cljmat,
                                double &cnrg, double &ljnrg);
    
    void setSpace(const Space &space);
};

/** This inline function is used to calculate the charge and LJ energy
    of two molecules based on the inter-atomic distances stored in 'distmatrix'
    and using the combined CLJ parameters listed in 'cljmatrix'. The
    charge and LJ energy are added onto the references 'chgnrg' and 'ljnrg'
    as I have found that this is the fastest method of returning a pair of
    values from a function. */
inline void CLJFF::calculateEnergy(DistMatrix &distmat, CLJPairMatrix &cljmatrix,
                                   double &chgnrg, double &ljnrg)
{
    int nats0 = distmatrix.nOuter();
    int nats1 = distmatrix.nInner();
    
    //loop over all pairs of atoms
    for (int i=0; i<nats0; ++i)
    {
        distmatrix.setOuterIndex(i);
        cljmatrix.setOuterIndex(i);
        
        for (int j=0; j<nats1; ++j)
        {
            //get the distance and CLJPair for this atom pair
            const double &invdist2 = distmat[j];
            const CLJPair &cljpair = cljmat[j];
               
            double sig2_over_dist2 = SireMaths::pow_2(cljpair.sigma()) * invdist2;
            double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
            double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);
                
            //coulomb energy
            chgnrg += SireUnits::one_over_four_pi_eps0 * 
                                       cljpair.charge2() * std::sqrt(invdist2);
                                      
            //LJ energy
            ljnrg += 4.0 * cljpair.epsilon() * 
                                     ( sig12_over_dist12 - sig6_over_dist6 );
        }
    }
}

}

#endif
