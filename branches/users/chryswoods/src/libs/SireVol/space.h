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

#include "coordgroup.h"

SIRE_BEGIN_HEADER

namespace SireVol
{
class SpaceBase;
class Space;
}

QDataStream& operator<<(QDataStream&, const SireVol::SpaceBase&);
QDataStream& operator>>(QDataStream&, SireVol::SpaceBase&);

QDataStream& operator<<(QDataStream&, const SireVol::Space&);
QDataStream& operator>>(QDataStream&, SireVol::Space&);

namespace SireVol
{

using SireMaths::Vector;

/** Define a distance matrix as being a PairMatrix of doubles */
typedef SireBase::PairMatrix<double> DistMatrix;

/**
This pure virtual base class represents the volume of space within which a SimSystem
resides. This class provides functions that calculate the distances between CoordGroups
in this volume, and has most of the optimisation (since most of the hard-work
double distance loops occur in this class!). Key overloaded classes that inherit
SpaceBase are Cartesian, which represents infinite 3D cartesian space, and
PeriodicBox which represents a 3D periodic box.

As this class is used right in the heart of the double loop it is very important
that it is not implemented in a way that requires a lot of virtual function calls.
This means that the class is implemented to calculate the distances of all pairs of points
between two CoordGroups in a single virtual function call. The results are held
in a special matrix that can be queried via read-only, inline, non-virtual
functions, with the items ordered in memory in the same order that you would get
by iterating over all pairs (group1 in outer loop, group2 in inner loop). An advantage
of this approach is that as the distances are calculated in one go, it is possible for
the SpaceBase class to find out whether two CoordGroups are within the non-bonded cutoff
distance before any further calculation is performed.

To prevent continual reallocation, the SpaceBase class works on an already-allocated
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
class SIREVOL_EXPORT SpaceBase : public SireBase::PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const SpaceBase&);
friend QDataStream& ::operator>>(QDataStream&, SpaceBase&);

public:
    SpaceBase();
    SpaceBase(const SpaceBase &other);

    ~SpaceBase();

    virtual SpaceBase* clone() const=0;

    /** Return the volume of the central box of this space. This
        throws an exception if it is not possible to calculate the
        volume of this space (e.g. it is an infinite space!) */
    virtual SireUnits::Dimension::Volume volume() const=0;

    /** Return a copy of this space with the volume of set to 'volume'
        - this will scale the space uniformly, keeping the center at
        the same location, to achieve this volume */
    virtual Space setVolume(SireUnits::Dimension::Volume volume) const=0;

    /** Change the volume of this space by 'delta' */
    Space changeVolume(SireUnits::Dimension::Volume delta) const;

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
        from an infinite cartesian space into this space. */
    virtual QVector<CoordGroup> mapFromCartesian(
                                    const QVector<CoordGroup> &groups) const=0;

    /** Return a copy of the passed CoordGroup that has been mapped from
        another copy of this space into this space (e.g. map from a
        small PeriodicBox to a large PeriodicBox) - note that the
        other space must have the same type as this space!

        \throw SireError::incompatible_error
    */
    virtual CoordGroup mapFromSelf(const CoordGroup &group,
                                   const Space &other) const=0;

    /** Return a copy an array of passed CoordGroups that have been mapped
        from another copy of this space into this space (e.g. map from a
        small PeriodicBox to a large PeriodicBox) - note that the
        other space must have the same type as this space!

        \throw SireError::incompatible_error
    */
    virtual QVector<CoordGroup> mapFromSelf(const QVector<CoordGroup> &groups,
                                            const Space &other) const=0;

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
    virtual QVector<CoordGroup> getMinimumImage(const QVector<CoordGroup> &groups,
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

protected:
    void assertCompatible(const Space &other) const;
};

/** This is the user-handle class that is used to hold the dynanmic Space classes.

    @author Christopher Woods
*/
class SIREVOL_EXPORT Space : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Space&);
friend QDataStream& ::operator>>(QDataStream&, Space&);

public:
    Space();
    Space(const SpaceBase &other);
    Space(const SireBase::PropertyBase &property);

    Space(const Space &other);

    ~Space();

    virtual Space& operator=(const SpaceBase &other);
    virtual Space& operator=(const SireBase::PropertyBase &other);

    const SpaceBase* operator->() const;
    const SpaceBase& operator*() const;
    
    const SpaceBase& read() const;
    SpaceBase& edit();
    
    const SpaceBase* data() const;
    const SpaceBase* constData() const;
    
    SpaceBase* data();
    
    operator const SpaceBase&() const;

    static const Space& shared_null();
};

}

Q_DECLARE_METATYPE(SireVol::Space)

SIRE_END_HEADER

#endif
