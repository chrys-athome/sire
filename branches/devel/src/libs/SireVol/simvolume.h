#ifndef SIREVOL_SIMVOLUME_H
#define SIREVOL_SIMVOLUME_H

#include "simvolumeptr.h"

#include "SireMol/cutgroupset.h"
#include "SireMol/moleculeset.h"

#include "SireBase/pairmatrix.hpp"

#include "SireMaths/vector.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireVol
{

using SireMol::CutGroupID;
using SireMol::CutGroup;
using SireMol::CutGroupSet;
using SireMol::CutGroupArray;
using SireMol::MoleculeSet;

using SireMaths::Vector;

/** Define a distance matrix as being a PairMatrix of doubles */
typedef SireBase::PairMatrix<double> DistMatrix;

/**
This pure virtual base class represents the volume of space within which a SimSystem resides. This class provides functions that calculate the distances between CutGroups in this volume, and has most of the optimisation (since most of the hard-work double distance loops occur in this class!). Key overloaded classes that inherit SimVolume are Cartesian, which represents infinite 3D cartesian space, and PeriodicBox which represents a 3D periodic box.
 
As this class is used right in the heart of the double loop it is very important that it is not implemented in a way that requires a lot of virtual function calls. This means that the class is implemented to calculate the distances of all atom pairs between two CutGroups in a single virtual function call. The results are held in a special matrix that can be queried via read-only, inline, non-virtual functions, with the items ordered in memory in the same order that you would get by iterating over all pairs (group1 in outer loop, group2 in inner loop). An advantage of this approach is that as the distances are calculated in one go, it is possible for the SimVolume class to find out whether two CutGroups are within the non-bonded cutoff distance before any further calculation is performed.
 
To prevent continual reallocation, the SimVolume class works on an already-allocated matrix class. This is only reallocated if it is found that it is not sufficiently large to hold the pair of CutGroups.
 
As a further optimisation, the distance matrix may be populated with the interatomic distances, or the square of the interatomic distances, or the 1 / distances or 1 / distances^2
 
The inheritors of this class are the only parts of this code where inter-cutgroup interatomic distances are calculated. You can thus inherit from this class and let your imagination run free! (*intra-cutgroup* distances are calculated in other places, as these are used in intramoleclular energy calculations, e.g. bond/angle/dihedral and intra-CLJ energy calculations).
 
@author Christopher Woods
*/
class SIREVOL_EXPORT SimVolume
{

public:
    SimVolume();
    SimVolume(const SimVolume &other);
    
    virtual ~SimVolume();

    /** Return a clone of this SimVolume. */
    virtual SimVolumePtr clone() const=0;
    
    /** Populate the matrix 'mat' with the distances between all of the
        atoms of the two CutGroups. Return the shortest distance^2 between the two
        CutGroups. */
    virtual double calcDist(const CutGroup &group1, const CutGroup &group2,
                            DistMatrix &mat) const=0;

    /** Populate the matrix 'mat' with the distances^2 between all of the
        atoms of the two CutGroups. Return the shortest distance^2 between the
        two CutGroups. */
    virtual double calcDist2(const CutGroup &group1, const CutGroup &group2,
                             DistMatrix &mat) const=0;

    /** Populate the matrix 'mat' with the inverse distances between all of the
        atoms of the two CutGroups. Return the shortest distance^2 between the two
        CutGroups. */
    virtual double calcInvDist(const CutGroup &group1, const CutGroup &group2,
                               DistMatrix &mat) const=0;

    /** Populate the matrix 'mat' with the inverse distances^2 between all of the
        atoms of the two CutGroups. Return the shortest distance^2 between the two
        CutGroups. */
    virtual double calcInvDist2(const CutGroup &group1, const CutGroup &group2,
                                DistMatrix &mat) const=0;

    /** Return whether or not these two groups are definitely beyond the distance 'dist'.
    
        \warning Not 'beyond' does not mean definitely within the distance!
    */
    virtual bool beyond(const double &dist, 
                        const CutGroup &group0, const CutGroup &group1) const=0;
};

}

SIRE_END_HEADER

#endif
