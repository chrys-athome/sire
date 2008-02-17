/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include <limits>
#include <cmath>

#include "periodicbox.h"
#include "coordgroup.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

#include <QDebug>

using namespace SireVol;
using namespace SireBase;
using namespace SireMaths;
using namespace SireStream;

static const RegisterMetaType<PeriodicBox> r_box;

/** Serialise to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds, const PeriodicBox &box)
{
    writeHeader(ds, r_box, 1)
               << box.mincoords << box.maxcoords
               << static_cast<const Cartesian&>(box);

               //no need to store anything else as it can be regenerated

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, PeriodicBox &box)
{
    VersionID v = readHeader(ds, r_box);

    if (v == 1)
    {
        Vector mincoords, maxcoords;

        ds >> mincoords >> maxcoords
           >> static_cast<Cartesian&>(box);

           //regenerate the box dimensions
           box.setDimension(mincoords, maxcoords);
    }
    else
        throw version_error(v, "1", r_box, CODELOC);

    return ds;
}

/** Construct a default PeriodicBox volume (zero volume) */
PeriodicBox::PeriodicBox() : ConcreteProperty<PeriodicBox,Cartesian>()
{}

/** Construct a PeriodicBox volume that goes from min to max */
PeriodicBox::PeriodicBox(const Vector &min, const Vector &max)
            : ConcreteProperty<PeriodicBox,Cartesian>()
{
    this->setDimension(min,max);
}

/** Copy constructor */
PeriodicBox::PeriodicBox(const PeriodicBox &other)
            : ConcreteProperty<PeriodicBox,Cartesian>(other),
              mincoords(other.mincoords), maxcoords(other.maxcoords),
              boxlength(other.boxlength),
              halflength(other.halflength), invlength(other.invlength)
{}

/** Destructor */
PeriodicBox::~PeriodicBox()
{}

/** Return the volume of the central box of this space.  */
SireUnits::Dimension::Volume PeriodicBox::volume() const
{
    return SireUnits::Dimension::Volume( boxlength.x() *
                                         boxlength.y() *
                                         boxlength.z() );
}

/** Return a copy of this space with the volume of set to 'volume'
    - this will scale the space uniformly, keeping the center at
    the same location, to achieve this volume */
Space PeriodicBox::setVolume(SireUnits::Dimension::Volume vol) const
{
    double old_volume = this->volume();
    double new_volume = vol;

    if (new_volume < 0)
        throw SireError::invalid_arg( QObject::tr(
            "You cannot set the volume of a periodic box to a negative value! (%1)")
                .arg(new_volume), CODELOC );

    if (old_volume == new_volume)
        return *this;

    double scl = std::pow( new_volume / old_volume, 1.0/3.0 ); // rats - no cbrt function!

    Vector cent = this->center();
    Vector new_halflength = scl * halflength;

    return PeriodicBox(cent - new_halflength, cent + new_halflength);
}

/** Set the dimensions of this box so that it is the smallest possible that contains
    the points 'min' and 'max'. The minimum coordinates of this box will be set to
    the minimum of the coordinates of these two points, and the maximum coordinates
    will be set to the maximum of the two points. */
void PeriodicBox::setDimension(const Vector &min, const Vector &max)
{
    mincoords = min;
    mincoords.setMin(max);

    maxcoords = max;
    maxcoords.setMax(min);

    boxlength = maxcoords - mincoords;

    for (int i=0; i<3; ++i)
    {
        invlength.set(i, 1.0/boxlength[i]);
        halflength.set(i, 0.5 * boxlength[i]);
    }
}

/** Calculate the distance between two points */
double PeriodicBox::calcDist(const Vector &point0, const Vector &point1) const
{
    //do we need to wrap the coordinates?
    Vector wrapdelta = this->wrapDelta(point0, point1);
    return Vector::distance( point0 + wrapdelta, point1 );
}

/** Calculate the distance squared between two points */
double PeriodicBox::calcDist2(const Vector &point0, const Vector &point1) const
{
    //do we need to wrap the coordinates?
    Vector wrapdelta = this->wrapDelta(point0, point1);
    return Vector::distance2( point0 + wrapdelta, point1 );
}

/** Populate the matrix 'mat' with the distances between all of the
    atoms of the two CoordGroups. Return the shortest distance^2 between the two
    CoordGroups. */
double PeriodicBox::calcDist(const CoordGroup &group0, const CoordGroup &group1,
                             DistMatrix &mat) const
{
    double mindist(std::numeric_limits<double>::max());
    double tmpdist;

    int n0 = group0.count();
    int n1 = group1.count();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n0, n1);

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(group0.aaBox().center(), group1.aaBox().center());

    //get raw pointers to the arrays - this provides more efficient access
    const Vector *array0 = group0.constData();
    const Vector *array1 = group1.constData();

    for (int i=0; i<n0; ++i)
    {
        //add the delta to the coordinates of atom0
        Vector point0 = array0[i] + wrapdelta;
        mat.setOuterIndex(i);

        for (int j=0; j<n1; ++j)
        {
            //calculate the distance between the two atoms
            tmpdist = Vector::distance(point0,array1[j]);

            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            mindist = qMin(tmpdist,mindist);

            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }

    //return the minimum distance
    return mindist;
}

/** Populate the matrix 'mat' with the distances between all of the
    atoms of the two CoordGroups. Return the shortest distance^2 between the two
    CoordGroups. */
double PeriodicBox::calcDist(const CoordGroup2 &group0, const CoordGroup2 &group1,
                             DistMatrix &mat) const
{
    double mindist(std::numeric_limits<double>::max());
    double tmpdist;

    int n0 = group0.count();
    int n1 = group1.count();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n0, n1);

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(group0.aaBox().center(), group1.aaBox().center());

    //get raw pointers to the arrays - this provides more efficient access
    const Vector *array0 = group0.constData();
    const Vector *array1 = group1.constData();

    for (int i=0; i<n0; ++i)
    {
        //add the delta to the coordinates of atom0
        Vector point0 = array0[i] + wrapdelta;
        mat.setOuterIndex(i);

        for (int j=0; j<n1; ++j)
        {
            //calculate the distance between the two atoms
            tmpdist = Vector::distance(point0,array1[j]);

            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            mindist = qMin(tmpdist,mindist);

            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }

    //return the minimum distance
    return mindist;
}

/** Populate the matrix 'mat' with the distances^2 between all of the
    atoms of the two CoordGroups. Return the shortest distance between the
    two CoordGroups. */
double PeriodicBox::calcDist2(const CoordGroup &group0, const CoordGroup &group1,
                              DistMatrix &mat) const
{
    double mindist2(std::numeric_limits<double>::max());
    double tmpdist;

    int n0 = group0.count();
    int n1 = group1.count();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n0, n1);

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(group0.aaBox().center(), group1.aaBox().center());

    //get raw pointers to the arrays - this provides more efficient access
    const Vector *array0 = group0.constData();
    const Vector *array1 = group1.constData();

    for (int i=0; i<n0; ++i)
    {
        //add the delta to the coordinates of atom0
        Vector point0 = array0[i] + wrapdelta;
        mat.setOuterIndex(i);

        for (int j=0; j<n1; ++j)
        {
            //calculate the distance between the two atoms
            tmpdist = Vector::distance2(point0,array1[j]);

            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            mindist2 = qMin(tmpdist,mindist2);

            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }

    //return the minimum distance
    return sqrt(mindist2);
}

/** Populate the matrix 'mat' with the inverse distances between all of the
    atoms of the two CoordGroups. Return the shortest distance between the two CoordGroups. */
double PeriodicBox::calcInvDist(const CoordGroup &group0, const CoordGroup &group1,
                                DistMatrix &mat) const
{
    double maxinvdist(0);
    double tmpdist;

    int n0 = group0.count();
    int n1 = group1.count();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n0, n1);

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(group0.aaBox().center(), group1.aaBox().center());

    //get raw pointers to the arrays - this provides more efficient access
    const Vector *array0 = group0.constData();
    const Vector *array1 = group1.constData();

    for (int i=0; i<n0; ++i)
    {
        //add the delta to the coordinates of atom0
        Vector point0 = array0[i] + wrapdelta;
        mat.setOuterIndex(i);

        for (int j=0; j<n1; ++j)
        {
            //calculate the distance between the two atoms
            tmpdist = Vector::invDistance(point0,array1[j]);

            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            maxinvdist = qMax(tmpdist,maxinvdist);

            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }

    //return the shortest distance
    return 1.0 / maxinvdist;
}

/** Populate the matrix 'mat' with the inverse distances^2 between all of the
    atoms of the two CoordGroups. Return the shortest distance between the two CoordGroups. */
double PeriodicBox::calcInvDist2(const CoordGroup &group0, const CoordGroup &group1,
                                 DistMatrix &mat) const
{
    double maxinvdist2(0);
    double tmpdist;

    int n0 = group0.count();
    int n1 = group1.count();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n0, n1);

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(group0.aaBox().center(), group1.aaBox().center());

    //get raw pointers to the arrays - this provides more efficient access
    const Vector *array0 = group0.constData();
    const Vector *array1 = group1.constData();

    for (int i=0; i<n0; ++i)
    {
        //add the delta to the coordinates of atom0
        //const Vector &point0 = array0[i] + wrapdelta;
        Vector point0 = array0[i] + wrapdelta;
        mat.setOuterIndex(i);

        for (int j=0; j<n1; ++j)
        {
            //calculate the distance between the two atoms
            tmpdist = Vector::invDistance2(point0, array1[j]);

            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            maxinvdist2 = qMax(tmpdist,maxinvdist2);

            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }

    //return the shortest distance
    return 1.0 / sqrt(maxinvdist2);
}

/** Return whether or not these two groups are definitely beyond the cutoff distance. */
bool PeriodicBox::beyond(double dist, const CoordGroup &group0,
                         const CoordGroup &group1) const
{
    const AABox &box0 = group0.aaBox();
    const AABox &box1 = group1.aaBox();

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(box0.center(), box1.center());

    return Vector::distance2( box0.center()+wrapdelta, box1.center() ) >
                      SireMaths::pow_2(dist + box0.radius() + box1.radius());
}

/** Return the minimum distance between the points in 'group0' and 'group1'.
    If this is a periodic space then this uses the minimum image convention
    (i.e. the minimum distance between the closest periodic replicas are
    used) */
double PeriodicBox::minimumDistance(const CoordGroup &group0,
                                    const CoordGroup &group1) const
{
    double mindist2(std::numeric_limits<double>::max());

    int n0 = group0.count();
    int n1 = group1.count();

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(group0.aaBox().center(), group1.aaBox().center());

    //get raw pointers to the arrays - this provides more efficient access
    const Vector *array0 = group0.constData();
    const Vector *array1 = group1.constData();

    for (int i=0; i<n0; ++i)
    {
        //add the delta to the coordinates of atom0
        Vector point0 = array0[i] + wrapdelta;

        for (int j=0; j<n1; ++j)
        {
            //calculate the distance between the two atoms
            double tmpdist = Vector::distance2(point0,array1[j]);

            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            mindist2 = qMin(tmpdist,mindist2);
        }
    }

    //return the minimum distance
    return sqrt(mindist2);
}

/** Return the closest periodic copy of 'group' to the point 'point',
    according to the minimum image convention. The effect of this is
    to move 'group' into the box which is now centered on 'point' */
CoordGroup PeriodicBox::getMinimumImage(const CoordGroup &group,
                                        const Vector &point) const
{
    Vector wrapdelta = wrapDelta(group.aaBox().center(), point);

    if (wrapdelta.isZero())
    {
        //already got the minimum image
        return group;
    }
    else
    {
        CoordGroupEditor editor = group.edit();
        editor.translate(wrapdelta);

        return editor.commit();
    }
}

/** Private function used to get the minimum image of all of the
    groups in 'groups' */
QVector<CoordGroup> PeriodicBox::_pvt_getMinimumImage(const QVector<CoordGroup> &groups,
                                                      const Vector &point) const
{
    int ncg = groups.count();
    const CoordGroup *group_array = groups.constData();

    //create a new array of the right size
    QVector<CoordGroup> moved_groups(ncg);
    CoordGroup *moved_array = moved_groups.data();

    for (int i=0; i<ncg; ++i)
    {
        moved_array[i] = this->getMinimumImage( group_array[i], point );
    }

    return moved_groups;
}

/** Return the closest periodic copy of each group in 'groups' to the
    point 'point', according to the minimum image convention.
    The effect of this is to move each 'group' into the box which is
    now centered on 'point' */
QVector<CoordGroup> PeriodicBox::getMinimumImage(const QVector<CoordGroup> &groups,
                                                 const Vector &point) const
{
    //run through all of the groups and see if any of them need moving...
    int ncg = groups.count();

    const CoordGroup *group_array = groups.constData();

    for (int i=0; i<ncg; ++i)
    {
        const CoordGroup &group = group_array[i];

        Vector wrapdelta = wrapDelta(point, group.aaBox().center());

        if ( not wrapdelta.isZero() )
        {
            //there is at least one CoordGroup that needs moving
            // - look to translate them all!
            return this->_pvt_getMinimumImage(groups, point);
        }
    }

    //all of the CoordGroups are in the box - just return the original array
    return groups;

}

/** Return a copy of the passed CoordGroup that has been mapped from
    an infinite cartesian space into this space */
CoordGroup PeriodicBox::mapFromCartesian(const CoordGroup &group) const
{
    //this is the same as getting the minimum image from the
    //center of the box
    return getMinimumImage(group, this->center());
}

/** Return a copy of an array of passed CoordGroups that have been mapped
    from an infinite cartesian space into this space. */
QVector<CoordGroup> PeriodicBox::mapFromCartesian(
                                  const QVector<CoordGroup> &groups) const
{
    //this is the same as getting the minimum image from the
    //center of the box
    return getMinimumImage(groups, this->center());
}

/** Return a copy of the passed CoordGroup that has been mapped from
    another copy of this space into this space (e.g. map from a
    small PeriodicBox to a large PeriodicBox) - note that the
    other space must have the same type as this space!

    \throw SireError::incompatible_error
*/
CoordGroup PeriodicBox::mapFromSelf(const CoordGroup &group, const Space &other) const
{
    assertCompatible(other);

    const PeriodicBox &other_box = other->asA<PeriodicBox>();

    if (other_box == *this)
        return group;
    else
    {
        //now get the scale factor for the change in dimension
        Vector scale( this->boxlength.x() * other_box.invlength.x(),
                      this->boxlength.y() * other_box.invlength.y(),
                      this->boxlength.z() * other_box.invlength.z() );

        //get the vector needed to translate this group into the central box
        Vector other_center = other_box.center();
        Vector wrapdelta = other_box.wrapDelta(other_center, group.aaBox().center());

        //get the vector from the center of the other box to the center
        //of the molecule when it is in the central box
        Vector delta = group.aaBox().center() - wrapdelta - other_center;

        //multiply this by 'scale'
        Vector new_delta = Vector( (scale.x() - 1) * delta.x(),
                                   (scale.y() - 1) * delta.y(),
                                   (scale.z() - 1) * delta.z() );

        return group.edit().translate( this->center() - other_center +
                                       new_delta - wrapdelta );
    }
}

/** Return a copy an array of passed CoordGroups that have been mapped
    from another copy of this space into this space (e.g. map from a
    small PeriodicBox to a large PeriodicBox) - note that the
    other space must have the same type as this space!

    \throw SireError::incompatible_error
*/
QVector<CoordGroup> PeriodicBox::mapFromSelf(const QVector<CoordGroup> &groups,
                                             const Space &other) const
{
    assertCompatible(other);

    const PeriodicBox &other_box = other->asA<PeriodicBox>();

    if (other_box == *this)
    {
        return groups;
    }
    else
    {
        QVector<CoordGroup> moved_groups = groups;

        Vector this_center = this->center();
        Vector other_center = other_box.center();

        Vector center_delta = this_center - other_center;

        //now get the scale factor for the change in dimension
        Vector scale( this->boxlength.x() * other_box.invlength.x(),
                      this->boxlength.y() * other_box.invlength.y(),
                      this->boxlength.z() * other_box.invlength.z() );

        int ngroups = moved_groups.count();
        CoordGroup *groups_array = moved_groups.data();

        for (int i=0; i<ngroups; ++i)
        {
            CoordGroup &group = groups_array[i];

            //get the vector needed to translate this group into the central box
            Vector wrapdelta = other_box.wrapDelta(other_center,
                                                   group.aaBox().center());

            Vector delta = group.aaBox().center() - wrapdelta - other_center;

            Vector new_delta = Vector( (scale.x() - 1) * delta.x(),
                                       (scale.y() - 1) * delta.y(),
                                       (scale.z() - 1) * delta.z() );

            group = group.edit().translate( center_delta + new_delta - wrapdelta );
        }

        return moved_groups;
    }
}

/** Return a list of copies of CoordGroup 'group' that are within
    'distance' of the CoordGroup 'center', translating 'group' so that
    it has the right coordinates to be around 'center'. Note that multiple
    copies of 'group' may be returned in this is a periodic space and
    there are multiple periodic replicas of 'group' within 'dist' of
    'center'. The copies of 'group' are returned together with the
    minimum distance between that periodic replica and 'center'.

    If there are no periodic replicas of 'group' that are within
    'dist' of 'center', then an empty list is returned. */
QList< tuple<double,CoordGroup> >
PeriodicBox::getCopiesWithin(const CoordGroup &group, const CoordGroup &center,
                             double dist) const
{
    //are there any copies within range?
    if (this->beyond(dist,group,center))
        //yep - there are no copies that are sufficiently close
        return QList< tuple<double,CoordGroup> >();

    //ok, first move 'group' into the box that has its center at the
    //same point as the center of the center group - this will give us
    //the group that is closest to us (the minimum image)
    CoordGroup minimum_image = this->getMinimumImage(group, center.aaBox().center());

    //now loop over periodic boxes, moving ever outward, trying to find
    //all copies that are within the distance

    //we can work out the maximum number of layers to go out to based on
    //the radii of the two groups, the maximum distance, and the dimensions
    //of the box
    const AABox &centerbox = center.aaBox();
    const AABox &imagebox = minimum_image.aaBox();

    double sum_of_radii_and_distance = centerbox.radius() +
                                       imagebox.radius() + dist;

    double sum_of_radii_and_distance2 = SireMaths::pow_2(sum_of_radii_and_distance);

    //this rounds to the nearest number of box lengths, e.g.
    // if sum_of_radii_and_distance is >= halflength.x() and < 1.5 length.x()
    // then there is only the need to go out to the first layer in the
    // x-dimension.
    int nlayers_x = int( (sum_of_radii_and_distance*invlength.x()) + 0.5 );
    int nlayers_y = int( (sum_of_radii_and_distance*invlength.y()) + 0.5 );
    int nlayers_z = int( (sum_of_radii_and_distance*invlength.z()) + 0.5 );

    QList< tuple<double,CoordGroup> > neargroups;

    //loop over all cubes
    for (int i = -nlayers_x; i <= nlayers_x; ++i)
    {
        for (int j = -nlayers_y; j <= nlayers_y; ++j)
        {
            for (int k = -nlayers_z; k <= nlayers_z; ++k)
            {
                //get the delta value needed to translate the minimum
                //image into the i,j,k box
                Vector delta( i * boxlength.x(),
                              j * boxlength.y(),
                              k * boxlength.z() );

                //translate just the center of the minimum image...
                Vector center_of_replica = imagebox.center() + delta;

                //is the box in range?
                if ( Vector::distance2(center_of_replica,centerbox.center())
                                    <= sum_of_radii_and_distance2 )
                {
                    //yes it is! Translate the entire CoordGroup
                    CoordGroupEditor editor = minimum_image.edit();
                    editor.translate(delta);
                    CoordGroup periodic_replica = editor.commit();

                    //calculate the minimum distance... (using the cartesian space)
                    double mindist = Cartesian::minimumDistance(periodic_replica, center);

                    if (mindist <= dist)
                        neargroups.append(
                                  tuple<double,CoordGroup>(mindist,periodic_replica) );
                }
            }
        }
    }

    return neargroups;
}
