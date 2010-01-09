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

#include "aabox.h"
#include "coordgroup.h"

#include "SireMaths/sphere.h"

#include <QDebug>

#include "Siren/datastream.h"
#include "Siren/logger.h"
#include "Siren/tester.h"
#include "Siren/errors.h"

using namespace Siren;
using namespace SireVol;

static const RegisterPrimitive<AABox> r_aabox;

namespace Siren{ template class PrimitiveObject<AABox>; }

/** Construct an empty AABox */
AABox::AABox() : Primitive<AABox>(), cent(), halfextents(), rad(0)
{}

/** Construct an AABox that completely encloses the point 'point' */
AABox::AABox(const Vector &point) 
      : Primitive<AABox>(), cent(point), halfextents(), rad(0)
{}

/** Construct an AABox with center at 'cent', and half-extents 'extents' */
AABox::AABox(const Vector &c, const Vector &extents) 
      : Primitive<AABox>(), cent(c), halfextents(extents)
{
    rad = halfextents.length();
}

/** Construct an AABox that completely encases the CoordGroup 'coordgroup' */
AABox::AABox(const CoordGroupBase &coordgroup) : Primitive<AABox>()
{
    recalculate(coordgroup);
}

/** Construct an AABox that completely encases all of the points in all of the
    CoordGroups in 'cgarray' */
AABox::AABox(const CoordGroupArray &cgarray) : Primitive<AABox>()
{
    recalculate(cgarray.constAABoxData(), cgarray.nCoordGroups());
}

/** Construct an AABox that completely encases all of the points in all of the
    CoordGroups in all of the arrays in 'cgarrays' */
AABox::AABox(const CoordGroupArrayArray &cgarrays) : Primitive<AABox>()
{
    recalculate(cgarrays.constAABoxData(), cgarrays.nCoordGroups());
}

/** Construct an AABox that completely encases the points  in 'coordinates' */
AABox::AABox(const QVector<Vector> &coordinates) : Primitive<AABox>()
{
    recalculate(coordinates);
}

/** Construct an AABox that completely encases the points in 'coords' */
AABox::AABox(const Vector *coords, int ncoords) : Primitive<AABox>()
{
    recalculate(coords, ncoords);
}

/** Copy constructor */
AABox::AABox(const AABox &other) : Primitive<AABox>(other)
{}

/** Destructor */
AABox::~AABox()
{}

AABox& AABox::operator=(const AABox &other)
{
    if (this != &other)
    {
        cent = other.cent;
        halfextents = other.halfextents;
        rad = other.rad;
    }
    
    return *this;
}

/** Comparison operator */
bool AABox::operator==(const AABox &other) const
{
    return this == &other or
          (rad == other.rad and cent == other.cent and halfextents == other.halfextents);
}

/** Comparison operator */
bool AABox::operator!=(const AABox &other) const
{
    return not AABox::operator==(other);
}

/** Return if the AABox is null */
bool AABox::isNull() const
{
    return cent.isZero() and rad == 0;
}

/** Return if the AABox is empty */
bool AABox::isEmpty() const
{
    return rad == 0;
}

/** Return an AABox constructed to contain the coordinates in 'coordinates' */
AABox AABox::from(const QVector<Vector> &coordinates)
{
    return AABox(coordinates);
}

/** Return an AABox constructed to contain the coordinates of 'point' */
AABox AABox::from(const Vector &point)
{
    return AABox(point);
}

/** Return an AABox constructed to contain the coordinates in 'coordinates' */
AABox AABox::from(const CoordGroupBase &coordinates)
{
    return AABox(coordinates);
}

/** Return an AABox constructed to contain all of the CoordGroups in 'cgarray' */
AABox AABox::from(const CoordGroupArray &cgarray)
{
    return AABox(cgarray);
}

/** Return an AABox constructed to contain all of the CoordGroups in the 
    arrays in 'cgarrays' */
AABox AABox::from(const CoordGroupArrayArray &cgarrays)
{
    return AABox(cgarrays);
}

uint AABox::hashCode() const
{
    return qHash( AABox::typeName() ) + qHash(cent) + qHash(halfextents);
}

void AABox::stream(Stream &s)
{
    s.assertVersion<AABox>(1);

    Schema schema = s.item<AABox>();
    
    schema.data("center") & cent;
    schema.data("half_extents") & halfextents;
    
    if (s.isLoading())
        rad = halfextents.length();
}

bool AABox::test(Siren::Logger &logger) const
{
    Tester tester(*this, logger);
    
    #ifndef SIREN_DISABLE_TESTS
    
    try
    {
        /// test 1
        {
            tester.nextTest();
            
            AABox a;
            
            tester.expect_true( QObject::tr("Null box is null"),
                                CODELOC,
                                a.isNull() );
                            
            tester.expect_true( QObject::tr("Null box is empty"),
                                CODELOC,
                                a.isEmpty() );
                                
            tester.expect_equal( QObject::tr("Null box has no radius"),
                                 CODELOC,
                                 a.radius(), 0.0 );
                                 
            tester.expect_equal( QObject::tr("Null box is at the origin"),
                                 CODELOC,
                                 a.center(), Vector(0,0,0) );
                                 
            tester.expect_equal( QObject::tr("Null box top is at the center"),
                                 CODELOC,
                                 a.maxCoords(), Vector(0,0,0) );
                                 
            tester.expect_equal( QObject::tr("Null box bottom is at the center"),
                                 CODELOC,
                                 a.maxCoords(), Vector(0,0,0) );
        }
    }
    catch(const Siren::exception &e)
    {
        tester.unexpected_error(e);
    }
    catch(...)
    {
        tester.unexpected_error( unknown_error(CODELOC) );
    }
    
    #endif // SIREN_DISABLE_TESTS
    
    return tester.allPassed();
}
    
bool AABox::test() const
{
    Logger l;
    return this->test(l);
}

/** Return a string representation of this AABox */
QString AABox::toString() const
{
    return QObject::tr( "AABox( min=%1, max=%2 )" )
                .arg(this->minCoords().toString(), this->maxCoords().toString() );
}

/** Internal function used to recalculate the AABox from the passed 
    array of AABoxes */
void AABox::recalculate(const AABox *aaboxes, int sz)
{
    if (sz == 1)
    {
        this->operator=(aaboxes[0]);
    }
    else if (sz > 1)
    {
        //set the initial max and min coords from the first coordinate in the group
        Vector maxcoords( aaboxes[0].maxCoords() );
        Vector mincoords( aaboxes[0].minCoords() );

        //loop through all of the remaining coordinates in the group
        for (int i=1; i < sz; ++i)
        {
            //calculate the maximum and minimum coordinates
            const AABox &aabox = aaboxes[i];
            maxcoords.setMax( aabox.maxCoords() );
            mincoords.setMin( aabox.minCoords() );
        }

        //now calculate the center as half the maximum and minimum coordinates
        cent = 0.5 * (maxcoords + mincoords);

        //the positive half-extent is the difference between the maximum
        //coordinates and the center
        halfextents = maxcoords - cent;

        //the radius is the length of 'halfextents'
        rad = halfextents.length();
    }
    else
    {
        cent = Vector(0);
        halfextents = Vector(0);
        sz = 0;
    }
}

/** Internal function used to recalculate the AABox from the coordinates in the
    array 'coords' (which has size 'sz') */
void AABox::recalculate(const Vector *coords, int sz)
{
    if (sz > 0)
    {
        //set the initial max and min coords from the first coordinate in the group
        Vector maxcoords( coords[0] );
        Vector mincoords( maxcoords );

        //loop through all of the remaining coordinates in the group
        for (int i=1; i < sz; ++i)
        {
            //calculate the maximum and minimum coordinates
            const Vector &coord = coords[i];
            maxcoords.setMax( coord );
            mincoords.setMin( coord );
        }

        //now calculate the center as half the maximum and minimum coordinates
        cent = 0.5 * (maxcoords + mincoords);

        //the positive half-extent is the difference between the maximum
        //coordinates and the center
        halfextents = maxcoords - cent;

        //the radius is the length of 'halfextents'
        rad = halfextents.length();
    }
    else
    {
        cent = Vector(0);
        halfextents = Vector(0);
        sz = 0;
    }
}

/** Recalculate the AABox so that it completely encloses the CoordGroup 'coordgroup' */
void AABox::recalculate(const CoordGroupBase &coordgroup)
{
    this->recalculate( coordgroup.constData(), coordgroup.size() );
}

/** Recalculate the AABox so that it completely encloses the CoordGroups
    in the array 'cgarray' */
void AABox::recalculate(const CoordGroupArray &cgarray)
{
    this->recalculate( cgarray.constAABoxData(), cgarray.nCoordGroups() );
}

/** Recalculate the AABox so that it completely encloses the CoordGroups
    in the arrays 'cgarrays' */
void AABox::recalculate(const CoordGroupArrayArray &cgarrays)
{
    this->recalculate( cgarrays.constAABoxData(), cgarrays.nCoordGroups() );
}

/** Recalculate the AABox so that it completely encloses the 'coordinates' */
void AABox::recalculate(const QVector<Vector> &coordinates)
{
    this->recalculate( coordinates.constData(), coordinates.size() );
}

/** Return whether or not this box is within 'dist' of box 'box'.
    (using infinite cartesian axes) */
bool AABox::withinDistance(double dist, const AABox &box) const
{
   //look at the components of the distance along the x, y and z axes
    double dx = std::abs(cent.x() - box.cent.x()) - halfextents.x() - box.halfextents.x();
    double dy = std::abs(cent.y() - box.cent.y()) - halfextents.y() - box.halfextents.y();
    double dz = std::abs(cent.z() - box.cent.z()) - halfextents.z() - box.halfextents.z();

    dx = SIRE_MIN(dx,0.0);
    dy = SIRE_MIN(dy,0.0);
    dz = SIRE_MIN(dz,0.0);

    return dx*dx + dy*dy + dz*dz <= dist*dist;
}

/** Return whether this box intersects with 'box' */
bool AABox::intersects(const AABox &box) const
{
    //look at the components of the distance along the x, y and z axes
    double dx = std::abs(cent.x() - box.cent.x()) - halfextents.x() - box.halfextents.x();
    double dy = std::abs(cent.y() - box.cent.y()) - halfextents.y() - box.halfextents.y();
    double dz = std::abs(cent.z() - box.cent.z()) - halfextents.z() - box.halfextents.z();

    return dx <= 0.0 and dy <= 0.0 and dz <= 0.0;
}

/** Return a copy of this box that has been translated by 'delta' */
AABox AABox::translate(const Vector &delta) const
{
    AABox ret(*this);

    ret.cent += delta;
    
    return ret;
}

/** Add another AABox to this one - this forms the union of both of the
    boxes. */
AABox AABox::operator+(const AABox &other) const
{
    Vector this_mincoords = this->minCoords();
    Vector this_maxcoords = this->maxCoords();
    
    Vector other_mincoords = other.minCoords();
    Vector other_maxcoords = other.maxCoords();
    
    Vector ret_mincoords = this_mincoords;
    Vector ret_maxcoords = this_maxcoords;
    
    ret_mincoords.setMin(other_mincoords);
    ret_maxcoords.setMax(other_maxcoords);

    ret_mincoords.setMin( other.minCoords() );
    ret_maxcoords.setMax( other.maxCoords() );

    if (ret_mincoords == this_mincoords and 
        ret_maxcoords == this_maxcoords)
    {
        return *this;
    }
    else if (ret_mincoords == other_mincoords and
             ret_maxcoords == other_maxcoords)
    {
        return other;
    }
    else
    {
        AABox ret;

        ret.cent = 0.5 * (ret_maxcoords + ret_mincoords);

        ret.halfextents = ret_maxcoords - ret.cent;

        ret.rad = ret.halfextents.length();

        return ret;
    }
}

/** Return an AABox that has this box added to 'other' */
AABox AABox::add(const AABox &other) const
{
    return *this + other;
}

/** Return the addition of the point 'point' to this box */
AABox AABox::operator+(const Vector &point) const
{
    Vector this_mincoords = minCoords();
    Vector this_maxcoords = maxCoords();
    
    Vector ret_mincoords = this_mincoords;
    Vector ret_maxcoords = this_maxcoords;

    ret_mincoords.setMin( point );
    ret_maxcoords.setMax( point );

    if (ret_mincoords != this_mincoords or
        ret_maxcoords != this_maxcoords)
    {
        AABox ret;
    
        ret.cent = 0.5 * (ret_maxcoords + ret_mincoords);

        ret.halfextents = ret_maxcoords - ret.cent;

        ret.rad = ret.halfextents.length();
    
        return ret;
    }
    else
        return *this;
}

/** Return the addition of the point 'point' to this box */
AABox AABox::add(const Vector &point) const
{
    return *this + point;
}

/** Return the addition of lots of points to this box */
AABox AABox::operator+(const QVector<Vector> &points) const
{
    if (points.isEmpty())
        return *this;

    int npoints = points.count();
    const Vector *points_array = points.constData();

    Vector this_mincoords = minCoords();
    Vector this_maxcoords = maxCoords();
    
    Vector ret_mincoords = this_mincoords;
    Vector ret_maxcoords = this_maxcoords;

    for (int i=0; i<npoints; ++i)
    {
        const Vector &point = points_array[i];
        ret_mincoords.setMin(point);
        ret_maxcoords.setMax(point);
    }

    if (ret_mincoords != this_mincoords or
        ret_maxcoords != this_maxcoords)
    {
        AABox ret;
    
        ret.cent = 0.5 * (ret_maxcoords + ret_mincoords);

        ret.halfextents = ret_maxcoords - ret.cent;

        ret.rad = ret.halfextents.length();
        
        return ret;
    }
    else
        return *this;
}

/** Construct a new AABox from the passed minimum and maximum coordinates */
AABox AABox::from(const Vector &mincoords, const Vector &maxcoords)
{
    Vector min = mincoords;
    min.setMin(maxcoords);
    
    Vector max = maxcoords;
    max.setMax(mincoords);
    
    Vector cent = 0.5 * (maxcoords + mincoords);
    Vector halfextents = maxcoords - cent;
    
    return AABox(cent, halfextents);
}

/** Return the addition of lots of points to this box */
AABox AABox::add(const QVector<Vector> &points) const
{
    return *this + points;
}

/** Return the sphere that just contains this AABox */
Sphere AABox::boundingSphere() const
{
    return Sphere( this->center(), this->radius() );
}
