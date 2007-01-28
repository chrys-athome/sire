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
#include "SireBase/sharedpolypointer.hpp"

#include "SireMaths/vector.h"

#include "sireglobal.h"

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

This is a virtual class that is designed to be used with DynamicSharedPtr.

@author Christopher Woods
*/
class SIREVOL_EXPORT SpaceBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const SpaceBase&);
friend QDataStream& ::operator>>(QDataStream&, SpaceBase&);

public:
    SpaceBase();
    SpaceBase(const SpaceBase &other);

    virtual ~SpaceBase();

    /** Return a clone of this SpaceBase - you are responsible
        for managing the returned pointer. */
    virtual SpaceBase* clone() const=0;

    /** Return the name of this class */
    virtual const char* what() const=0;

    /** Return whether or not this is an instance of class 'T' */
    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }

    /** Return this class cast as an instance of class 'T' - this will
        have undefined results unless isA<T>() returns true. */
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }

    /** Return this class cast as an instance of class 'T' - this will
        have undefined results unless isA<T>() returns true. */
    template<class T>
    T& asA()
    {
        return dynamic_cast<T&>(*this);
    }

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

    /** Return the minimum distance between the points in 'group0' and 'group1'.
        If this is a periodic space then this uses the minimum image convention
        (i.e. the minimum distance between the closest periodic replicas are
        used) */
    virtual double minimumDistance(const CoordGroup &group0,
                                   const CoordGroup &group1) const=0;

    /** Return the minimum distance between points within the group 'group'. */
    virtual double minimumDistance(const CoordGroup &group) const=0;

    /** Return a copy of the passed CoordGroup that has been moved into the
        central box. This only does something for periodic spaces. */
    virtual CoordGroup moveToCenterBox(const CoordGroup &group) const=0;

    /** Return a copy of an array of passed CoordGroups that have been moved
        into the central box. This only does something for periodic spaces. */
    virtual QVector<CoordGroup> moveToCenterBox(
                                    const QVector<CoordGroup> &groups) const=0;

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
};

/** This is the user-handle class that is used to hold the dynanmic Space classes.

    @author Christopher Woods
*/
class SIREVOL_EXPORT Space
{

friend QDataStream& ::operator<<(QDataStream&, const Space&);
friend QDataStream& ::operator>>(QDataStream&, Space&);

public:
    Space();
    Space(const SpaceBase &other);

    Space(const Space &other);

    ~Space();

    Space& operator=(const SpaceBase &other);
    Space& operator=(const Space &other);

    const char* what() const;

    template<class T>
    bool isA() const
    {
        return d->isA<T>();
    }

    template<class T>
    const T& asA() const
    {
        return d->asA<T>();
    }

    template<class T>
    T& asA()
    {
        return d->asA<T>();
    }

    double calcDist(const CoordGroup &group, DistMatrix &distmat) const;
    double calcDist2(const CoordGroup &group, DistMatrix &distmat) const;
    double calcInvDist(const CoordGroup &group, DistMatrix &distmat) const;
    double calcInvDist2(const CoordGroup &group, DistMatrix &distmat) const;

    double calcDist(const CoordGroup &group0, const CoordGroup &group1,
                    DistMatrix &distmat) const;
    double calcDist2(const CoordGroup &group0, const CoordGroup &group1,
                     DistMatrix &distmat) const;
    double calcInvDist(const CoordGroup &group0, const CoordGroup &group1,
                       DistMatrix &distmat) const;
    double calcInvDist2(const CoordGroup &group0, const CoordGroup &group1,
                        DistMatrix &distmat) const;

    bool beyond(double dist, const CoordGroup &group0, const CoordGroup &group1) const;

    double minimumDistance(const CoordGroup &group0, const CoordGroup &group1) const;

    double minimumDistance(const CoordGroup &group) const;

    CoordGroup moveToCenterBox(const CoordGroup &group) const;

    QVector<CoordGroup> moveToCenterBox(const QVector<CoordGroup> &groups) const;

    CoordGroup getMinimumImage(const CoordGroup &group, const Vector &center) const;

    QVector<CoordGroup> getMinimumImage(const QVector<CoordGroup> &groups,
                                        const Vector &center) const;

    QList< boost::tuple<double,CoordGroup> >
                getCopiesWithin(const CoordGroup &group,
                                const CoordGroup &center, double dist) const;

    QList< boost::tuple<double,CoordGroup> >
                getCopiesWithin(const CoordGroup &group,
                                const QVector<CoordGroup> &center, double dist) const;

private:
    /** Dynamic shared pointer to the virtual SpaceBase class
        that is used to perform the distance calculations. */
    SireBase::SharedPolyPointer<SpaceBase> d;
};

/** Populate the matrix 'mat' with the distances between all of the
    points within a CoordGroup. This creates a symmetrical matrix,
    with a 0 diagonal. This returns the shortest distance between
    two points within the group. */
inline double Space::calcDist(const CoordGroup &group, DistMatrix &distmat) const
{
    return d->calcDist(group, distmat);
}

/** Populate the matrix 'mat' with the distances^2 between all of the
    points within a CoordGroup. This creates a symmetrical matrix,
    with a 0 diagonal. This returns the shortest distance^2 between
    two points within the group. */
inline double Space::calcDist2(const CoordGroup &group, DistMatrix &distmat) const
{
    return d->calcDist2(group, distmat);
}

/** Populate the matrix 'mat' with the inverse distances^2 between all of the
    points within a CoordGroup. This creates a symmetrical matrix,
    with a 0 diagonal. This returns the the largest inverse distance^2 between
    two points within the group. */
inline double Space::calcInvDist(const CoordGroup &group, DistMatrix &distmat) const
{
    return d->calcInvDist(group, distmat);
}

/** Populate the matrix 'mat' with the inverse distances^2 between all of the
    points within a CoordGroup. This creates a symmetrical matrix,
    with a 0 diagonal. This returns the the largest inverse distance^2 between
    two points within the group. */
inline double Space::calcInvDist2(const CoordGroup &group, DistMatrix &distmat) const
{
    return d->calcInvDist2(group, distmat);
}

/** Populate the matrix 'mat' with the distances between all of the
    points of the two CoordGroups. Return the shortest distance^2 between the two
    CoordGroups. */
inline double Space::calcDist(const CoordGroup &group0, const CoordGroup &group1,
                              DistMatrix &distmat) const
{
    return d->calcDist(group0, group1, distmat);
}

/** Populate the matrix 'mat' with the distances^2 between all of the
    points of the two CoordGroups. Return the shortest distance^2 between the
    two CoordGroups. */
inline double Space::calcDist2(const CoordGroup &group0, const CoordGroup &group1,
                               DistMatrix &distmat) const
{
    return d->calcDist2(group0, group1, distmat);
}

/** Populate the matrix 'mat' with the inverse distances between all of the
     points of the two CoordGroups. Return the largest inverse distance between the two
    CoordGroups. */
inline double Space::calcInvDist(const CoordGroup &group0, const CoordGroup &group1,
                                 DistMatrix &distmat) const
{
    return d->calcInvDist(group0, group1, distmat);
}

/** Populate the matrix 'mat' with the inverse distances^2 between all of the
    points of the two CoordGroups. Return the largest inverse distance^2 between the two
    CoordGroups. */
inline double Space::calcInvDist2(const CoordGroup &group0, const CoordGroup &group1,
                                  DistMatrix &distmat) const
{
    return d->calcInvDist2(group0, group1, distmat);
}

/** Return whether or not these two groups are definitely beyond the distance 'dist'.

    \warning Note 'beyond' does not mean definitely within the distance!
*/
inline bool Space::beyond(double dist, const CoordGroup &group0,
                          const CoordGroup &group1) const
{
    return d->beyond(dist, group0, group1);
}

/** Return the minimum distance between the points in 'group0' and 'group1'.
    If this is a periodic space then this uses the minimum image convention
    (i.e. the minimum distance between the closest periodic replicas are
    used) */
inline double Space::minimumDistance(const CoordGroup &group0,
                                     const CoordGroup &group1) const
{
    return d->minimumDistance(group0, group1);
}

/** Return the minimum distance between points within the group 'group'. */
inline double Space::minimumDistance(const CoordGroup &group) const
{
    return d->minimumDistance(group);
}

/** Return a copy of the passed CoordGroup that has been moved into the
    central box. This only does something for periodic spaces. */
inline CoordGroup Space::moveToCenterBox(const CoordGroup &group) const
{
    return d->moveToCenterBox(group);
}

/** Return a copy of an array of passed CoordGroups that have been moved
    into the central box. This only does something for periodic spaces. */
inline QVector<CoordGroup> Space::moveToCenterBox(
                                      const QVector<CoordGroup> &groups) const
{
    return d->moveToCenterBox(groups);
}

/** Return the minimum image copy of 'group' with respect to 'center'.
    For periodic spaces, this translates 'group' into the box that
    has its center at 'center' (i.e. returns the closest copy of
    'group' to 'center' according to the minimum image convention) */
inline CoordGroup Space::getMinimumImage(const CoordGroup &group,
                                         const Vector &center) const
{
    return d->getMinimumImage(group, center);
}

/** Return the minimum image copy of 'groups' with respect to 'center'.
    For periodic spaces, this translates 'groups' into the box that
    has its center at 'center' (i.e. returns the closest copy of
    each 'group' to 'center' according to the minimum image convention) */
inline QVector<CoordGroup> Space::getMinimumImage(const QVector<CoordGroup> &groups,
                                                  const Vector &center) const
{
    return d->getMinimumImage(groups, center);
}

/** Return a list of copies of CoordGroup 'group' that are within
    'distance' of the CoordGroup 'center', translating 'group' so that
    it has the right coordinates to be around 'center'. Note that multiple
    copies of 'group' may be returned if this is a periodic space and
    there are multiple periodic replicas of 'group' within 'dist' of
    'center'. The copies of 'group' are returned together with the
    minimum distance between that periodic replica and 'center'.

    If there are no periodic replicas of 'group' that are within
    'dist' of 'center', then an empty list is returned. */
inline QList< boost::tuple<double,CoordGroup> >
Space::getCopiesWithin(const CoordGroup &group, const CoordGroup &center,
                       double dist) const
{
    return d->getCopiesWithin(group, center, dist);
}

}

Q_DECLARE_METATYPE(SireVol::Space)

SIRE_END_HEADER

#endif
