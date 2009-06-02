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

#include <QDebug>

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
A PeriodicBox is a volume  that represents standard periodic boundary conditions
(a 3D box replicated to infinity along all three dimensions).

@author Christopher Woods
*/
class SIREVOL_EXPORT PeriodicBox 
        : public SireBase::ConcreteProperty<PeriodicBox,Cartesian>
{

friend QDataStream& ::operator<<(QDataStream&, const PeriodicBox&);
friend QDataStream& ::operator>>(QDataStream&, PeriodicBox&);

public:
    PeriodicBox();
    PeriodicBox(const Vector &min, const Vector &max);

    PeriodicBox(const PeriodicBox &other);

    ~PeriodicBox();

    PeriodicBox& operator=(const PeriodicBox &other);
    
    bool operator==(const PeriodicBox &other) const;
    bool operator!=(const PeriodicBox &other) const;

    QString toString() const;

    SireUnits::Dimension::Volume volume() const;
    SpacePtr setVolume(SireUnits::Dimension::Volume volume) const;

    void setDimension(const Vector &min, const Vector &max);

    const Vector& minCoords() const;
    const Vector& maxCoords() const;

    Vector center() const;

    static const char* typeName();

    double calcDist(const Vector &point0, const Vector &point1) const;
    double calcDist2(const Vector &point0, const Vector &point1) const;

    double calcDist(const CoordGroup &group1, const CoordGroup &group2,
                    DistMatrix &distmat) const;

    double calcDist2(const CoordGroup &group1, const CoordGroup &group2,
                     DistMatrix &distmat) const;

    double calcInvDist(const CoordGroup &group1, const CoordGroup &group2,
                       DistMatrix &distmat) const;

    double calcInvDist2(const CoordGroup &group1, const CoordGroup &group2,
                        DistMatrix &distmat) const;

    DistVector calcDistVector(const Vector &point0, const Vector &point1) const;
    
    double calcDistVectors(const CoordGroup &group1, const CoordGroup &group2,
                           DistVectorMatrix &distmat) const;

    bool beyond(double dist, const AABox &aabox0, const AABox &aabox1) const;

    bool beyond(double dist, const CoordGroup &group0,
                const CoordGroup &group1) const;

    double minimumDistance(const CoordGroup &group0, const CoordGroup &group1) const;

    CoordGroup mapFromCartesian(const CoordGroup &group) const;
    CoordGroup mapToCartesian(const CoordGroup &group) const;
    
    CoordGroupArray mapFromCartesian(const CoordGroupArray &groups) const;
    CoordGroupArray mapToCartesian(const CoordGroupArray &groups) const;
    
    CoordGroupArray mapAsOneFromCartesian(const CoordGroupArray &groups) const;
    CoordGroupArray mapAsOneToCartesian(const CoordGroupArray &groups) const;

    CoordGroup mapToSpace(const CoordGroup &group, const Space &space) const;
    CoordGroupArray mapToSpace(const CoordGroupArray &groups,
                               const Space &space) const;
                                       
    CoordGroupArray mapAsOneToSpace(const CoordGroupArray &groups,
                                    const Space &space) const;

    CoordGroup getMinimumImage(const CoordGroup &group, const Vector &center) const;

    CoordGroupArray getMinimumImage(const CoordGroupArray &groups,
                                    const Vector &center) const;

    QList< boost::tuple<double,CoordGroup> >
               getCopiesWithin(const CoordGroup &group,
                               const CoordGroup &center, double dist) const;

    bool contains(const Vector &point) const;

protected:

    Vector wrapDelta(const Vector &v0, const Vector &v1) const;

    CoordGroupArray _pvt_getMinimumImage(
                                const CoordGroupArray &groups,
                                const Vector &point) const;

    static int getWrapVal(double delta, double invlgth, double halflgth);

    Vector getScaleDelta(const Vector &point, const PeriodicBox &other) const;

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

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

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

#endif //SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE(SireVol::PeriodicBox)

SIRE_EXPOSE_CLASS( SireVol::PeriodicBox )

SIRE_END_HEADER

#endif
