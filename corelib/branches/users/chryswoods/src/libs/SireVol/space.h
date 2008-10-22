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

#ifndef SIREVOL_SPACE_H
#define SIREVOL_SPACE_H

#include <QList>
#include <QVector>
#include <boost/tuple/tuple.hpp>

#include "SireBase/pairmatrix.hpp"
#include "SireBase/property.h"
#include "SireBase/sharedpolypointer.hpp"

#include "SireUnits/dimensions.h"

#include "SireMaths/vector.h"
#include "SireMaths/distvector.h"

#include "coordgroup.h"

SIRE_BEGIN_HEADER

namespace SireVol
{
class Space;
class Space;
}

QDataStream& operator<<(QDataStream&, const SireVol::Space&);
QDataStream& operator>>(QDataStream&, SireVol::Space&);

namespace SireVol
{

using SireMaths::Vector;
using SireMaths::DistVector;

class Cartesian;

typedef SireBase::PropPtr<Space> SpacePtr;

/** Define a distance matrix as being a PairMatrix of doubles */
typedef SireBase::PairMatrix<double> DistMatrix;

/** Define a vector field matrix as being a PairMatrix of DistVectors
    (these are Vectors that have the magnitude and direction separated out) */
typedef SireBase::PairMatrix<DistVector> DistVectorMatrix;

/**
This pure virtual base class represents the volume of space within which a SimSystem
resides. This class provides functions that calculate the distances between CoordGroups
in this volume, and has most of the optimisation (since most of the hard-work
double distance loops occur in this class!). Key overloaded classes that inherit
Space are Cartesian, which represents infinite 3D cartesian space, and
PeriodicBox which represents a 3D periodic box.

As this class is used right in the heart of the double loop it is very important
that it is not implemented in a way that requires a lot of virtual function calls.
This means that the class is implemented to calculate the distances of all pairs of points
between two CoordGroups in a single virtual function call. The results are held
in a special matrix that can be queried via read-only, inline, non-virtual
functions, with the items ordered in memory in the same order that you would get
by iterating over all pairs (group1 in outer loop, group2 in inner loop). An advantage
of this approach is that as the distances are calculated in one go, it is possible for
the Space class to find out whether two CoordGroups are within the non-bonded cutoff
distance before any further calculation is performed.

To prevent continual reallocation, the Space class works on an already-allocated
matrix class. This is only reallocated if it is found that it is not sufficiently
large to hold the pair of CoordGroups.

As a further optimisation, the distance matrix may be populated with the interpoint
distances, or the square of the interatomic distances, or the 1 / distances
or 1 / distances^2

The inheritors of this class should be the only parts of this code where
distance calculations are calculated between and within CoordGroups. This will
allow you to change the space of the system here, and have that space used in the
rest of the code that uses CoordGroups.

This is a virtual class that is designed to be used with SharedPolyPointer.

@author Christopher Woods
*/
class SIREVOL_EXPORT Space : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Space&);
friend QDataStream& ::operator>>(QDataStream&, Space&);

public:
    Space();
    Space(const Space &other);

    ~Space();

    virtual Space* clone() const=0;

    /** Return the volume of the central box of this space. This
        throws an exception if it is not possible to calculate the
        volume of this space (e.g. it is an infinite space!) */
    virtual SireUnits::Dimension::Volume volume() const=0;

    /** Return a copy of this space with the volume of set to 'volume'
        - this will scale the space uniformly, keeping the center at
        the same location, to achieve this volume */
    virtual SpacePtr setVolume(SireUnits::Dimension::Volume volume) const=0;

    /** Change the volume of this space by 'delta' */
    SpacePtr changeVolume(SireUnits::Dimension::Volume delta) const;

    /** Calculate the distance between two points */
    virtual double calcDist(const Vector &point0, const Vector &point1) const=0;

    /** Calculate the distance squared between two points */
    virtual double calcDist2(const Vector &point0, const Vector &point1) const=0;

    /** Populate the matrix 'mat' with the distances between all of the
        points within a CoordGroup. This creates a symmetrical matrix,
        with a 0 diagonal. This returns the shortest distance between
        two points within the group. */
    virtual double calcDist(const CoordGroup &group, DistMatrix &distmat) const=0;

    /** Populate the matrix 'mat' with the distances^2 between all of the
        points within a CoordGroup. This creates a symmetrical matrix,
        with a 0 diagonal. This returns the shortest distance^2 between
        two points within the group. */
    virtual double calcDist2(const CoordGroup &group, DistMatrix &distmat) const=0;

    /** Populate the matrix 'mat' with the inverse distances between all of the
        points within a CoordGroup. This creates a symmetrical matrix,
        with a 0 diagonal. This returns the the largest inverse distance between
        two points within the group. */
    virtual double calcInvDist(const CoordGroup &group, DistMatrix &distmat) const=0;

    /** Populate the matrix 'mat' with the inverse distances^2 between all of the
        points within a CoordGroup. This creates a symmetrical matrix,
        with a 0 diagonal. This returns the the largest inverse distance^2 between
        two points within the group. */
    virtual double calcInvDist2(const CoordGroup &group, DistMatrix &distmat) const=0;

    /** Populate the matrix 'mat' with the distances between all of the
        points of the two CoordGroups. Return the shortest distance^2 between the two
        CoordGroups. */
    virtual double calcDist(const CoordGroup &group1, const CoordGroup &group2,
                            DistMatrix &distmat) const=0;

    /** Populate the matrix 'mat' with the distances^2 between all of the
        points of the two CoordGroups. Return the shortest distance^2 between the
        two CoordGroups. */
    virtual double calcDist2(const CoordGroup &group1, const CoordGroup &group2,
                             DistMatrix &distmat) const=0;

    /** Populate the matrix 'mat' with the inverse distances between all of the
        points of the two CoordGroups. Return the largest inverse distance between the two
        CoordGroups. */
    virtual double calcInvDist(const CoordGroup &group1, const CoordGroup &group2,
                               DistMatrix &distmat) const=0;

    /** Populate the matrix 'mat' with the inverse distances^2 between all of the
        points of the two CoordGroups. Return the largest inverse distance^2 between the two
        CoordGroups. */
    virtual double calcInvDist2(const CoordGroup &group1, const CoordGroup &group2,
                                DistMatrix &distmat) const=0;

    /** Calculate the distance vector between two points */
    virtual DistVector calcDistVector(const Vector &point0, 
                                      const Vector &point1) const=0;

    /** Populate the matrix 'distmat' with all of the interpoint distance vectors
        between all points within the CoordGroup. This is *not* a symmetrical matrix,
        as the direction from point A to point B is the negative of the 
        direction from point B to point A. This returns the shortest distance
        between two points in the group (that is not the self-self distance) */
    virtual double calcDistVectors(const CoordGroup &group,
                                   DistVectorMatrix &distmat) const=0;
                                       
    /** Populate the matrix 'distmat' between all the points of the two CoordGroups
        'group1' and 'group2' - the returned matrix has the vectors pointing
        from each point in 'group1' to each point in 'group2'. This returns
        the shortest distance between two points in the group */
    virtual double calcDistVectors(const CoordGroup &group1,
                                   const CoordGroup &group2,
                                   DistVectorMatrix &distmat) const=0;

    /** Return whether or not these two groups are definitely beyond the distance 'dist'.

        \warning Note 'beyond' does not mean definitely within the distance!
    */
    virtual bool beyond(double dist, const CoordGroup &group0,
                        const CoordGroup &group1) const=0;

    /** Return whether or not two groups that are enclosed by the AABoxes
        'aabox0' and 'aabox1' are beyond the cutoff distance 'dist'.
        
        \warning Note 'beyond' does not mean definitely within the distance!
    */
    virtual bool beyond(double dist, const AABox &aabox0, const AABox &aabox1) const=0;

    /** Return the minimum distance between the points in 'group0' and 'group1'.
        If this is a periodic space then this uses the minimum image convention
        (i.e. the minimum distance between the closest periodic replicas are
        used) */
    virtual double minimumDistance(const CoordGroup &group0,
                                   const CoordGroup &group1) const=0;

    /** Return the minimum distance between points within the group 'group'. */
    virtual double minimumDistance(const CoordGroup &group) const=0;

    /** Return a copy of the passed CoordGroup that has been mapped from
        an infinite cartesian space into this space */
    virtual CoordGroup mapFromCartesian(const CoordGroup &group) const=0;

    /** Return a copy of an array of passed CoordGroups that have been mapped
        individually from an infinite cartesian space into this space. */
    virtual CoordGroupArray mapFromCartesian(const CoordGroupArray &groups) const=0;

    /** Return a copy of the passed CoordGroup that has been mapped into
        the infinite cartesian space from this space */
    virtual CoordGroup mapToCartesian(const CoordGroup &group) const=0;
    
    /** Return a copy of an array of passed CoordGroups that have been mapped
        individually from to an infinite cartesian space from this space */
    virtual CoordGroupArray mapToCartesian(const CoordGroupArray &groups) const=0;
    
    /** Return a copy of the passed CoordGroups that have been mapped as
        a single unit from an infinite cartesian space into this space */
    virtual CoordGroupArray mapAsOneFromCartesian(const CoordGroupArray &groups) const=0;
    
    /** Return a copy of the passed CoordGroups that have been mapped as a single
        unit to the infinite Cartesian space from this space */
    virtual CoordGroupArray mapAsOneToCartesian(const CoordGroupArray &groups) const=0;

    /** Return the minimum image copy of 'group' with respect to 'center'.
        For periodic spaces, this translates 'group' into the box that
        has its center at 'center' (i.e. returns the closest copy of
        'group' to 'center' according to the minimum image convention) */
    virtual CoordGroup getMinimumImage(const CoordGroup &group,
                                       const Vector &center) const=0;

    /** Return the minimum image copy of 'groups' with respect to 'center'.
        For periodic spaces, this translates 'groups' into the box that
        has its center at 'center' (i.e. returns the closest copy of
        each 'group' to 'center' according to the minimum image convention) */
    virtual CoordGroupArray getMinimumImage(const CoordGroupArray &groups,
                                            const Vector &center) const=0;

    /** Return a list of copies of CoordGroup 'group' that are within
        'distance' of the CoordGroup 'center', translating 'group' so that
        it has the right coordinates to be around 'center'. Note that multiple
        copies of 'group' may be returned if this is a periodic space and
        there are multiple periodic replicas of 'group' within 'dist' of
        'center'. The copies of 'group' are returned together with the
        minimum distance between that periodic replica and 'center'.

        If there are no periodic replicas of 'group' that are within
        'dist' of 'center', then an empty list is returned.
    */
    virtual QList< boost::tuple<double,CoordGroup> >
                    getCopiesWithin(const CoordGroup &group, const CoordGroup &center,
                                    double dist) const=0;

    static const Cartesian& null();

protected:
    void assertCompatible(const Space &other) const;
};

}

SIRE_EXPOSE_CLASS( SireVol::Space )

SIRE_EXPOSE_PROPERTY( SireVol::SpacePtr, SireVol::Space )

SIRE_END_HEADER

///need to include "cartesian.h" as it is needed to instantiate the null() function
#include "cartesian.h"

#endif
