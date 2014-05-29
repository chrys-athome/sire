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

#include "sphere.h"

#include "SireStream/datastream.h"

#include <QDebug>

using namespace SireStream;
using namespace SireMaths;

static const RegisterMetaType<Sphere> r_sphere(NO_ROOT);

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Sphere &sphere)
{
    writeHeader(ds, r_sphere, 1) << sphere._center << sphere._radius;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Sphere &sphere)
{
    VersionID v = readHeader(ds, r_sphere);

    if (v == 1)
    {
        ds >> sphere._center >> sphere._radius;
    }
    else
        throw version_error(v, "1", r_sphere, CODELOC);

    return ds;
}

/** Create a default sphere, centered on the origin with zero radius */
Sphere::Sphere() : _center(0.0), _radius(0.0)
{}

/** Create a sphere with radius 'radius', centered on the origin */
Sphere::Sphere(const double &radius) : _center(0.0), _radius(radius)
{
    if (radius < 0.0)
        _radius = -radius;
}

/** Create a sphere centered at 'position' and with radius 'radius' */
Sphere::Sphere(const Vector &position, const double &radius)
       : _center(position), _radius(radius)
{
    if (radius < 0.0)
        _radius = -radius;
}

/** Copy constructor */
Sphere::Sphere(const Sphere &other)
       : _center(other._center), _radius(other._radius)
{}

/** Destructor */
Sphere::~Sphere()
{}

/** Comparison operator */
bool Sphere::operator==(const Sphere &other) const
{
    return _radius == other._radius and _center == other._center;
}

/** Comparison operator */
bool Sphere::operator!=(const Sphere &other) const
{
    return _radius != other._radius or _center != other._center;
}

/** Set the position of the center of this sphere */
void Sphere::setPosition(const Vector &position)
{
    _center = position;
}

/** Set the position of the center of this sphere */
void Sphere::setCenter(const Vector &center)
{
    _center = center;
}

/** Set the radius of this sphere */
void Sphere::setRadius(double radius)
{
    _radius = radius;

    if (radius < 0.0)
        _radius = 0.0;
}

/** Return the volume of this sphere */
double Sphere::volume() const
{
    return (4.0/3.0) * SireMaths::pi * SireMaths::pow_3(_radius);
}

/** Return the surface area of this sphere */
double Sphere::surfaceArea() const
{
    return 4.0 * SireMaths::pi * SireMaths::pow_2(_radius);
}

/** Return whether or not this sphere intersects with 'other' */
bool Sphere::intersects(const Sphere &other) const
{
    return Vector::distance(_center, other._center) < (_radius + other._radius);
}

/** Return the volume of space formed at the intersection of this sphere with 'other' */
double Sphere::intersectionVolume(const Sphere &other) const
{
    const double dist = Vector::distance(_center, other._center);
    const double sum_radii = _radius + other._radius;
    const double big_radius = qMax(_radius, other._radius);
    const double small_radius = qMin(_radius, other._radius);
    
    if (dist >= sum_radii)
    {
        //no intersection
        return 0;
    }
    else if (dist + small_radius <= big_radius)
    {
        //the small sphere is entirely enclosed in the big sphere
        return (4.0/3.0) * SireMaths::pi * SireMaths::pow_3(big_radius);
    }
    else
    {
        //the intersection volume is the two spherical caps cut off at
        //the top of two spheres
        
        // we can work out the height of the two caps by imagining
        // two triangles formed by the intersection of the spheres
        // (x axis is along the sphere centers, r0 is radius 0, r1 is radius 1,
        //  h0 is the cap distance for sphere 0, h1 is the cap distance for sphere 1,
        //  and d is the distance between the center of the spheres
        
        //         *         *
        //   r0  / |  x    x | \  r1
        //     /   |         |   \
        //    *----*         *----*
        //     r0-h0         r1-h1
        
        //  this gives us three equations with three unknowns (x, h0 and h1)
        //
        //  (1) d = r0 - h0 + r1 - h1
        //  (2) (r0-h0)**2 + x**2 = r0**2
        //  (3) (r1-h1)**2 + x**2 = r1**2
        //
        //  Rearranging these to solve for h0 and h1 gives us;
        //
        //  (i)  h0 = [ r1**2 - (d - r0)**2 ] / 2d
        //  (ii) h1 = r0 - h0 + r1 - d
        
        const double d = dist;
        const double r0 = _radius;
        const double r1 = other._radius;
        
        const double h0 = (r1*r1 - SireMaths::pow_2(d-r0)) / (2*d);
        const double h1 = r0 - h0 + r1 - d;
        
        //we can check this is true by calculating x...
        //const double x0 = r0*r0 - SireMaths::pow_2(r0-h0);
        //const double x1 = r1*r1 - SireMaths::pow_2(r1-h1);
        //qDebug() << h0 << h1 << x0 << x1;
        
        // now the volume will be
        // volume(sphere 0) + volume(sphere 1) - cap(sphere 0) - cap(sphere 1)
        //
        // (where the cap has volume (1/3) pi h**2 (3r - h)
        const double cap0 = (1.0/3.0) * SireMaths::pi * h0 * h0 * (3*r0 - h0);
        const double cap1 = (1.0/3.0) * SireMaths::pi * h1 * h1 * (3*r1 - h1);
        
        return cap0 + cap1;
    }
}

/** Return the combined volume of the passed array of spheres */
double Sphere::combinedVolume(const QVector<SireMaths::Sphere> &spheres)
{
    if (spheres.isEmpty())
    {
        return 0;
    }
    else if (spheres.count() == 1)
    {
        return spheres.at(0).volume();
    }
    else if (spheres.count() == 2)
    {
        //volume is sum of volumes minus the volume of overlap
        return spheres.at(0).volume() + spheres.at(1).volume()
                 - spheres.at(0).intersectionVolume(spheres.at(1));
    }
    else
    {
        //calculate the volume of all spheres
        double total_volume = 0;
        
        for (int i=0; i<spheres.count(); ++i)
        {
            total_volume += spheres.constData()[i].volume();
        }
        
        //now find all intersecting pairs of spheres and subtract their volume
        //from the total
        QVector< QSet<int> > intersecting_pairs( spheres.count() );
        
        for (int i=0; i<spheres.count()-1; ++i)
        {
            const Sphere &sphere0 = spheres.constData()[i];
        
            for (int j=i+1; j<spheres.count(); ++j)
            {
                const Sphere &sphere1 = spheres.constData()[j];
            
                if (sphere0.intersects(sphere1))
                {
                    total_volume -= sphere0.intersectionVolume(sphere1);
                    intersecting_pairs[i].insert(j);
                    intersecting_pairs[j].insert(i);
                }
            }
        }
        
        //now find all of the sets of 3+ intersections
        for (int i=0; i<spheres.count(); ++i)
        {
            if (intersecting_pairs[i].count() > 1)
            {
                //this sphere is intersecting with more than one other sphere - are
                //these spheres intersecting with each other too? If they are, then
                //there is a good chance that there is a combined volume between these
                //spheres
                const QSet<int> &intersects = intersecting_pairs[i];
            }
        }
        
        return total_volume;
    }
}

const char* Sphere::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Sphere>() );
}
