#ifndef SIREVOL_CARTESIAN_H
#define SIREVOL_CARTESIAN_H

#include "simvolume.h"

SIRE_BEGIN_HEADER

namespace SireVol
{

/**
This class overloads SimVolume to provide an infinite Cartesian (3-dimensional, orthoganol dimensions) volume. This corresponds to a traditional gas-phase or no-boundary system.
 
@author Christopher Woods
*/
class SIREVOL_EXPORT Cartesian : public SimVolume
{
public:
    Cartesian();
    Cartesian(const Cartesian &other);
    
    ~Cartesian();
    
    SimVolumePtr clone() const;
    
    double calcDist(const CutGroup &group1, const CutGroup &group2,
                    DistMatrix &mat) const;

    double calcDist2(const CutGroup &group1, const CutGroup &group2,
                     DistMatrix &mat) const;

    double calcInvDist(const CutGroup &group1, const CutGroup &group2,
                       DistMatrix &mat) const;

    double calcInvDist2(const CutGroup &group1, const CutGroup &group2,
                        DistMatrix &mat) const;
    
    bool beyond(const double &dist, const CutGroup &group0, 
                const CutGroup &group1) const;
};

}

SIRE_END_HEADER

#endif
