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

#include "Siren/stream.h"

using namespace Siren;
using namespace SireMaths;

static const RegisterPrimitive<Sphere> r_sphere;

/** Create a default sphere, centered on the origin with zero radius */
Sphere::Sphere() : Primitive<Sphere>(), _center(0.0), _radius(0.0)
{}

/** Create a sphere with radius 'radius', centered on the origin */
Sphere::Sphere(const double &radius) 
       : Primitive<Sphere>(), _center(0.0), _radius(radius)
{
    if (radius < 0.0)
        _radius = -radius;
}

/** Create a sphere centered at 'position' and with radius 'radius' */
Sphere::Sphere(const Vector &position, const double &radius)
       : Primitive<Sphere>(), _center(position), _radius(radius)
{
    if (radius < 0.0)
        _radius = -radius;
}

/** Copy constructor */
Sphere::Sphere(const Sphere &other)
       : Primitive<Sphere>(), _center(other._center), _radius(other._radius)
{}

/** Destructor */
Sphere::~Sphere()
{}

Sphere& Sphere::operator=(const Sphere &other)
{
    _radius = other._radius;
    _center = other._center;
    return *this;
}

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

QString Sphere::toString() const
{
    return QObject::tr("Sphere( center:[%1,%2,%3], radius:%4 )")
                .arg(_center.x()).arg(_center.y()).arg(_center.z())
                .arg(_radius);
}

uint Sphere::hashCode() const
{
    return qHash(Sphere::typeName()) + qHash(_center) + qHash(_radius);
}

void Sphere::stream(Stream &s)
{
    s.assertVersion<Sphere>(1);
    
    Schema schema = s.item<Sphere>();
    
    schema.data("center") & _center;
    schema.data("radius") & _radius;
}
