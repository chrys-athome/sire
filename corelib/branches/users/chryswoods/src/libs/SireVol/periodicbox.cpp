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

#include <QMutex>

#include <limits>
#include <cmath>

#ifdef SIRE_USE_SSE
#include <emmintrin.h>
#endif

#include "periodicbox.h"
#include "coordgroup.h"

#include "SireBase/countflops.h"

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

/** This is the maximum dimension of the box (so that .volume() doesn't overflow) */
const Vector max_boxlength( std::pow(0.4 * std::numeric_limits<double>::max(),
                                     1.0/3.0) );

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

    //don't allow boxes that would cause .volume() to overflow
    mincoords.setMax(-max_boxlength);
    maxcoords.setMin( max_boxlength);

    boxlength = maxcoords - mincoords;

    if (boxlength.x() == 0 or boxlength.y() == 0 or boxlength.z() == 0)
    {
        throw SireError::invalid_arg( QObject::tr(
            "Cannot set the box size to %1 to %2 as this would create "
            "a box with at least one side that is equal to zero.")
                .arg(min.toString(), max.toString()), CODELOC );
    }

    for (int i=0; i<3; ++i)
    {
        invlength.set(i, 1.0/boxlength[i]);
        halflength.set(i, 0.5 * boxlength[i]);
    }
}

/** Construct a default PeriodicBox volume (zero volume) */
PeriodicBox::PeriodicBox() : ConcreteProperty<PeriodicBox,Cartesian>()
{
    //set this to be a ridiculously large box
    this->setDimension( -max_boxlength, max_boxlength );
}

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

/** Copy assignment operator */
PeriodicBox& PeriodicBox::operator=(const PeriodicBox &other)
{
    if (this != &other)
    {
        mincoords = other.mincoords;
        maxcoords = other.maxcoords;
        boxlength = other.boxlength;
        halflength = other.halflength;
        invlength = other.invlength;
        Cartesian::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool PeriodicBox::operator==(const PeriodicBox &other) const
{
    return this == &other or
           (mincoords == other.mincoords and boxlength == other.boxlength);
}

/** Comparison operator */
bool PeriodicBox::operator!=(const PeriodicBox &other) const
{
    return this != &other and
           (mincoords != other.mincoords or boxlength != other.boxlength);
}

/** Return a string representation of this space */
QString PeriodicBox::toString() const
{
    return QObject::tr("PeriodicBox( %1 to %2 )")
                .arg( this->minCoords().toString(), 
                      this->maxCoords().toString() );
}

/** Return the number of boxes that are covered by the distance 'del', where
    'invlgth' is the inverse of the length of the box along the dimension of 'del',
    and 'halflgth' is half the length of the box along the dimension of 'del'. */
inline int PeriodicBox::getWrapVal(double del, double invlgth, double halflgth)
{
    if (del <= halflgth and del >= -halflgth)  // we are in the same box
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
    return Vector( getWrapVal( v1.x()-v0.x(), invlength.x(), halflength.x())
                                          * boxlength.x(),
                   getWrapVal( v1.y()-v0.y(), invlength.y(), halflength.y())
                                          * boxlength.y(),
                   getWrapVal( v1.z()-v0.z(), invlength.z(), halflength.z())
                                          * boxlength.z()
                 );
}

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
SpacePtr PeriodicBox::setVolume(SireUnits::Dimension::Volume vol) const
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

    const int n0 = group0.count();
    const int n1 = group1.count();

    #ifdef SIRE_TIME_ROUTINES
    int nflops = 0;
    #endif

    //redimension the matrix to hold all of the pairs
    mat.redimension(n0, n1);

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(group0.aaBox().center(), group1.aaBox().center());

    #ifdef SIRE_TIME_ROUTINES
    nflops += 3;  // above is at least three operations, sometimes more!
    #endif

    //get raw pointers to the arrays - this provides more efficient access
    const Vector *array0 = group0.constData();
    const Vector *array1 = group1.constData();

    #ifdef SIRE_USE_SSE
    {
        //version of the algorithm suitable for use with SSE2 or above
        const int remainder = n1 % 2;
    
        __m128d sse_mindist = { mindist, mindist };

        for (int i=0; i<n0; ++i)
        {
            //add the delta to the coordinates of atom0
            Vector point0 = array0[i] + wrapdelta;
            
            #ifdef SIRE_TIME_ROUTINES
            nflops += 3;
            #endif
            
            mat.setOuterIndex(i);

            __m128d sse_x0 = { point0.x(), point0.x() };
            __m128d sse_y0 = { point0.y(), point0.y() };
            __m128d sse_z0 = { point0.z(), point0.z() };

            //process the other points, two at a time
            for (int j=0; j < n1-remainder; j+=2)
            {
                const Vector &point1 = array1[j];
                const Vector &point2 = array1[j+1];
        
                __m128d sse_x1 = { point1.x(), point2.x() };
                __m128d sse_y1 = { point1.y(), point2.y() };
                __m128d sse_z1 = { point1.z(), point2.z() };
            
                __m128d dx = sse_x0 - sse_x1;    // 2 flops
                __m128d tmpdist = dx * dx;       // 2 flops
            
                dx = sse_y0 - sse_y1;            // 2 flops
                tmpdist += dx * dx;              // 4 flops
            
                dx = sse_z0 - sse_z1;            // 2 flops
                tmpdist += dx * dx;              // 4 flops

                tmpdist = _mm_sqrt_pd(tmpdist);  // 2 flops

                #ifdef SIRE_TIME_ROUTINES
                nflops += 18;
                #endif

                sse_mindist = _mm_min_pd( sse_mindist, tmpdist );

                //place this distance into the matrix
                mat[j] = *((const double*)&tmpdist);
                mat[j+1] = *( ((const double*)&tmpdist) + 1 );
            }
        
            mindist = qMin( *((const double*)&sse_mindist),
                            *( ((const double*)&sse_mindist) + 1 ) );
        
            if (remainder == 1)
            {
                const double tmpdist = Vector::distance(point0, array1[n1-1]);
                
                #ifdef SIRE_TIME_ROUTINES
                nflops += 9;
                #endif
                
                mindist = qMin(tmpdist,mindist);
                mat[n1-1] = tmpdist;
            }
        }
    }
    #else
    {
        //generic version suitable for any processor
        for (int i=0; i<n0; ++i)
        {
            //add the delta to the coordinates of atom0
            Vector point0 = array0[i] + wrapdelta;
            
            #ifdef SIRE_TIME_ROUTINES
            nflops += 3;
            #endif
            
            mat.setOuterIndex(i);

            for (int j=0; j<n1; ++j)
            {
                const double dist = Vector::distance(point0, array1[j]);
                
                #ifdef SIRE_TIME_ROUTINES
                nflops += 9;
                #endif
                
                mindist = qMin(mindist, dist);
                mat[j] = dist;
            }
        }
    }
    #endif

    #ifdef SIRE_TIME_ROUTINES
    ADD_FLOPS(nflops);
    #endif

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

    const int n0 = group0.count();
    const int n1 = group1.count();

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
            const double tmpdist = Vector::distance2(point0,array1[j]);

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

/** Calculate the distance vector between two points */
DistVector PeriodicBox::calcDistVector(const Vector &point0, 
                                       const Vector &point1) const
{
    Vector wrapdelta = this->wrapDelta(point0, point1);

    return point1 - (point0+wrapdelta);
}

/** Populate the matrix 'distmat' between all the points of the two CoordGroups
    'group1' and 'group2' - the returned matrix has the vectors pointing
    from each point in 'group1' to each point in 'group2'. This returns
    the shortest distance between two points in the group */
double PeriodicBox::calcDistVectors(const CoordGroup &group0, const CoordGroup &group1,
                                    DistVectorMatrix &mat) const
{
    double mindist(std::numeric_limits<double>::max());

    const int n0 = group0.count();
    const int n1 = group1.count();

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
            mat[j] = (array1[j] - point0);

            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            mindist = qMin(mat[j].length(),mindist);
        }
    }

    //return the minimum distance
    return mindist;
}

/** Return whether or not two groups enclosed by the AABoxes 'aabox0' and 
    'aabox1' are definitely beyond the cutoff distance 'dist' */
bool PeriodicBox::beyond(double dist, const AABox &aabox0, const AABox &aabox1) const
{
    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(aabox0.center(), aabox1.center());

    #ifdef SIRE_TIME_ROUTINES
    ADD_FLOPS( 14 );
    #endif

    return Vector::distance2( aabox0.center()+wrapdelta, aabox1.center() ) >
                      SireMaths::pow_2(dist + aabox0.radius() + aabox1.radius());
}

/** Return whether or not these two groups are definitely beyond the cutoff distance. */
bool PeriodicBox::beyond(double dist, const CoordGroup &group0,
                         const CoordGroup &group1) const
{
    return PeriodicBox::beyond(dist, group0.aaBox(), group1.aaBox());
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
CoordGroupArray PeriodicBox::_pvt_getMinimumImage(const CoordGroupArray &groups,
                                                  const Vector &point) const
{
    int ncg = groups.count();

    const CoordGroup *group_array = groups.constData();

    if (ncg == 1)
        return CoordGroupArray( this->getMinimumImage(group_array[0], point) );

    //create a new array of the right size
    QVector<CoordGroup> moved_groups(ncg);
    CoordGroup *moved_array = moved_groups.data();

    for (int i=0; i<ncg; ++i)
    {
        moved_array[i] = this->getMinimumImage( group_array[i], point );
    }

    return CoordGroupArray(moved_groups);
}

/** Return the closest periodic copy of each group in 'groups' to the
    point 'point', according to the minimum image convention.
    The effect of this is to move each 'group' into the box which is
    now centered on 'point' */
CoordGroupArray PeriodicBox::getMinimumImage(const CoordGroupArray &groups,
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

CoordGroup PeriodicBox::mapToCartesian(const CoordGroup &group) const
{
    return group;
}

/** Return a copy of an array of passed CoordGroups that have been mapped
    from an infinite cartesian space into this space. */
CoordGroupArray PeriodicBox::mapFromCartesian(const CoordGroupArray &groups) const
{
    return getMinimumImage(groups, this->center());
}

CoordGroupArray PeriodicBox::mapToCartesian(const CoordGroupArray &groups) const
{
    return groups;
}

CoordGroupArray PeriodicBox::mapAsOneFromCartesian(const CoordGroupArray &groups) const
{
    if (groups.nCoordGroups() == 0)
        return groups;

    //wrap all of the groups using the delta center of the groups
    Vector wrapdelta = wrapDelta(groups.aaBox().center(), this->center());

    if ( not wrapdelta.isZero() )
    {
        CoordGroupArray mapped_groups(groups);
        mapped_groups.translate(wrapdelta);
        return mapped_groups;
    }
    else
        return groups;
}

CoordGroupArray PeriodicBox::mapAsOneToCartesian(const CoordGroupArray &groups) const
{
    return groups;
}

/** Return the vector by which the point 'point' would need to be translated
    so that it is mapped from this PeriodicBox into 'other' */
Vector PeriodicBox::getScaleDelta(const Vector &point, 
                                  const PeriodicBox &other) const
{
    //now get the scale factor for the change in dimension
    Vector scale( other.boxlength.x() * this->invlength.x(),
                  other.boxlength.y() * this->invlength.y(),
                  other.boxlength.z() * this->invlength.z() );

    //get the vector needed to translate this group into the central box
    Vector wrapdelta = this->wrapDelta(this->center(), point);

    //get the vector from the center of this box to the point
    //when it is in this central box
    Vector delta = point - wrapdelta - this->center();
    
    //multiply this by 'scale'
    Vector new_delta = Vector( (scale.x() - 1) * delta.x(),
                               (scale.y() - 1) * delta.y(),
                               (scale.z() - 1) * delta.z() );

    return other.center() - this->center() + new_delta - wrapdelta;
}

/** Return a copy of the passed CoordGroup mapped from this space to 
    the other space 'space' */
CoordGroup PeriodicBox::mapToSpace(const CoordGroup &group, const Space &space) const
{
    if (not space.isA<PeriodicBox>())
        return Cartesian::mapToSpace(group, space);
    
    const PeriodicBox &other = space.asA<PeriodicBox>();
    
    if (this->operator==(other))
        //there is nothing to do
        return group;

    return group.edit().translate( this->getScaleDelta(group.aaBox().center(),
                                                       other) ).commit();
}

/** Return a copy of the passed CoordGroups mapped individually from this
    space to the other space 'space' */
CoordGroupArray PeriodicBox::mapToSpace(const CoordGroupArray &groups,
                                        const Space &space) const
{
    if (not space.isA<PeriodicBox>())
        return Cartesian::mapToSpace(groups, space);

    const PeriodicBox &other = space.asA<PeriodicBox>();
    
    if (this->operator==(other))
        //there is nothing to do
        return groups;

    int ncg = groups.count();
    const CoordGroup *group_array = groups.constData();

    //create a new array of the right size
    QVector<CoordGroup> moved_groups(ncg);
    CoordGroup *moved_array = moved_groups.data();

    for (int i=0; i<ncg; ++i)
    {
        moved_array[i] = group_array[i].edit().translate(
                                this->getScaleDelta( group_array[i].aaBox().center(),
                                                     other ) ).commit();
    }

    return CoordGroupArray(moved_groups);    
}
                                   
/** Return a copy of the passed CoordGroups mapped as a single unit from this
    space to the other space 'space' */
CoordGroupArray PeriodicBox::mapAsOneToSpace(const CoordGroupArray &groups,
                                             const Space &space) const
{
    if (not space.isA<PeriodicBox>())
        return Cartesian::mapAsOneToSpace(groups, space);

    const PeriodicBox &other = space.asA<PeriodicBox>();
    
    if (this->operator==(other))
        //there is nothing to do
        return groups;

    CoordGroupArray new_groups(groups);
    
    new_groups.translate( this->getScaleDelta(groups.aaBox().center(), other) );
                          
    return new_groups;
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
    if (dist > max_boxlength.x())
        throw SireError::invalid_arg( QObject::tr(
            "You cannot use a distance (%1) that is greater than the "
            "maximum box length (%2).")
                .arg(dist).arg(max_boxlength.x()), CODELOC );

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
                    {
                        neargroups.append(
                                  tuple<double,CoordGroup>(mindist,periodic_replica) );
                    }
                }
            }
        }
    }

    return neargroups;
}
