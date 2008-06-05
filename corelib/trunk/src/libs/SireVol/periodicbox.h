#ifndef SIREVOL_PERIODICBOX_H
#define SIREVOL_PERIODICBOX_H

#include "simvolume.h"

#include "SireMaths/vector.h"

SIRE_BEGIN_HEADER

namespace SireVol
{

using SireMaths::Vector;

/**
A PeriodicBox is a volume  that represents standard periodic boundary conditions (a 3D box replicated to infinity along all three dimensions).
 
@author Christopher Woods
*/
class SIREVOL_EXPORT PeriodicBox : public SimVolume
{
public:
    PeriodicBox();
    PeriodicBox(const Vector &min, const Vector &max);
    PeriodicBox(const PeriodicBox &other);
    
    ~PeriodicBox();
    
    void setDimension(const Vector &min, const Vector &max);
    
    const Vector& minCoords() const;
    const Vector& maxCoords() const;
    
    Vector center() const;
    
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

protected:

    Vector wrapDelta(const Vector &v0, const Vector &v1) const;

    static int getWrapVal(double delta, double invlgth, double halflgth);

    /** The origin of the box (minimum coordinates) */
    Vector mincoords;
    
    /** The maximum coordinates of the box */
    Vector maxcoords;
    
    /** The lengths of each side of the box */
    Vector boxlength;
    
    /** Half the box length */
    Vector halflength;
    
    /** The inverse of the lengths of each side of the box */
    Vector invlength;
};

/** Return the minimum coordinates of the box (the origin) */
inline const Vector& PeriodicBox::minCoords() const
{
    return mincoords;
}

/** Return the maximum coordinates of the box */
inline const Vector& PeriodicBox::maxCoords() const
{
    return maxcoords;
}

/** Return the coordinates of the center of the box */
inline Vector PeriodicBox::center() const
{
    return 0.5 * (maxcoords + mincoords);
}

/** Return the number of boxes that are covered by the distance 'del', where
    'invlgth' is the inverse of the length of the box along the dimension of 'del',
    and 'halflgth' is half the length of the box along the dimension of 'del'. */
inline int PeriodicBox::getWrapVal(double del, double invlgth, double halflgth)
{
    if (del < halflgth and del > -halflgth)  // we are in the same box
        return 0;
    else
        // 'int()' always rounds down (e.g. 1.6 -> 1, 1.2 -> 1, -1.2 -> -1, -1.8 -> -1)
        //  (0.5 * ((del > 0)*2 - 1)) will equal
        //  + 0.5 if del > 0, and will equal -0.5 if del < 0. This ensures that
        // the call to int() will now round to the nearest integer, rather than
        // rounding down. (e.g. 1.3 -> 1, 1.6 -> 2, -1.3 -> -1 and -1.8 -> -2)
        return int( (del * invlgth) + (0.5 * ((del > 0)*2 - 1)) );
}

/** Calculate the delta that needs to be subtracted from the interatomic
    distances so that the molecules are all wrapped into the same periodic box */
inline Vector PeriodicBox::wrapDelta(const Vector &v0, const Vector &v1) const
{
    return Vector( getWrapVal( v1.x()-v0.x(), invlength.x(), halflength.x()) * boxlength.x(), 
                   getWrapVal( v1.y()-v0.y(), invlength.y(), halflength.y()) * boxlength.y(), 
                   getWrapVal( v1.z()-v0.z(), invlength.z(), halflength.z()) * boxlength.z() );
}

}

SIRE_END_HEADER

#endif
