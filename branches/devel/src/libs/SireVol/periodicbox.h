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

#ifndef SIREVOL_PERIODICBOX_H
#define SIREVOL_PERIODICBOX_H

#include "cartesian.h"

#include "SireMaths/vector.h"

SIRE_BEGIN_HEADER

namespace SireVol
{
class PeriodicBox;
}

QDataStream& operator<<(QDataStream&, const SireVol::PeriodicBox&);
QDataStream& operator>>(QDataStream&, SireVol::PeriodicBox&);

namespace SireVol
{

using SireMaths::Vector;

/**
A PeriodicBox is a volume  that represents standard periodic boundary conditions (a 3D box replicated to infinity along all three dimensions).

@author Christopher Woods
*/
class SIREVOL_EXPORT PeriodicBox : public Cartesian
{

friend QDataStream& ::operator<<(QDataStream&, const PeriodicBox&);
friend QDataStream& ::operator>>(QDataStream&, PeriodicBox&);

public:
    PeriodicBox();
    PeriodicBox(const Vector &min, const Vector &max);

    PeriodicBox(const PeriodicBox &other);

    ~PeriodicBox();

    void setDimension(const Vector &min, const Vector &max);

    const Vector& minCoords() const;
    const Vector& maxCoords() const;

    Vector center() const;

    SpaceBase* clone() const
    {
        return new PeriodicBox(*this);
    }

    static const char* typeName()
    {
        return "SireVol::Cartesian";
    }

    const char* what() const
    {
        return PeriodicBox::typeName();
    }

    double calcDist(const CoordGroup &group1, const CoordGroup &group2,
                    DistMatrix &distmat) const;

    double calcDist2(const CoordGroup &group1, const CoordGroup &group2,
                     DistMatrix &distmat) const;

    double calcInvDist(const CoordGroup &group1, const CoordGroup &group2,
                       DistMatrix &distmat) const;

    double calcInvDist2(const CoordGroup &group1, const CoordGroup &group2,
                        DistMatrix &distmat) const;

    bool beyond(double dist, const CoordGroup &group0,
                const CoordGroup &group1) const;

    double minimumDistance(const CoordGroup &group0, const CoordGroup &group1) const;

    CoordGroup moveToCenterBox(const CoordGroup &group) const;

    QVector<CoordGroup> moveToCenterBox(const QVector<CoordGroup> &groups) const;

    CoordGroup getMinimumImage(const CoordGroup &group, const Vector &center) const;

    QVector<CoordGroup> getMinimumImage(const QVector<CoordGroup> &groups,
                                        const Vector &center) const;

    QList< boost::tuple<double,CoordGroup> >
               getCopiesWithin(const CoordGroup &group,
                               const CoordGroup &center, double dist) const;

    bool contains(const Vector &point) const;

protected:

    bool _pvt_isEqual(const SireBase::PropertyBase &other) const;

    Vector wrapDelta(const Vector &v0, const Vector &v1) const;

    QVector<CoordGroup> _pvt_getMinimumImage(
                                const QVector<CoordGroup> &groups,
                                const Vector &point) const;

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

/** Return whether this box contains the point 'point' (without the need
    for periodic translation) */
inline bool PeriodicBox::contains(const Vector &point) const
{
    return (point.x() >= mincoords.x() and
            point.y() >= mincoords.y() and
            point.z() >= mincoords.z()) and

           (point.x() <= maxcoords.x() and
            point.y() <= maxcoords.y() and
            point.z() <= maxcoords.z());
}

}

Q_DECLARE_METATYPE(SireVol::PeriodicBox)

SIRE_END_HEADER

#endif
