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

#include "plane.h"

#include "Siren/stream.h"

using namespace SireMaths;
using namespace Siren;

static const RegisterPrimitive<Plane> r_plane;

/** Construct a default plane - this is the x/y plane, lying at the origin */
Plane::Plane() : Primitive<Plane>(), norm(0.0,0.0,1.0), dist(0.0)
{}

/** Construct the plane lying perpendicular to 'normal' and at a distance of 'dist'
    from the origin */
Plane::Plane(const Vector &normal, const double &distance)
      : Primitive<Plane>(), dist(distance)
{
    //normalise the normal
    try
    {
        norm = normal.normalise();
    }
    catch( SireMaths::math_error& )
    {
        //trying to normalise a zero vector
        norm.set(0.0,0.0,1.0);
    }
}

/** Construct a plane that lies perpendicular to 'normal' and that also contains
    the point 'contains_point' */
Plane::Plane(const Vector &normal, const Vector &contains_point)
      : Primitive<Plane>()
{
    //normalise the normal
    try
    {
        norm = normal.normalise();
    }
    catch( SireMaths::math_error& )
    {
        //trying to normalise a zero vector
        norm.set(0.0,0.0,1.0);
    }

    //the equation of the plane is 'ax + by + cz + d = 0'
    //If the plane contains 'contains_point', then we can just
    //plug this into the equation to find 'd'
    dist = -( Vector::dot(norm, contains_point) );
}

/** Construct the plane that fulfills the equation "ax + by + cz + d = 0" */
Plane::Plane(const double &a, const double &b, const double &c, const double &d)
      : Primitive<Plane>(), norm(a,b,c), dist(d)
{
    double lgth = norm.length();

    //we need to ensure that the plane normal is normalised
    try
    {
        norm /= lgth;
        dist /= lgth;
    }
    catch( SireMaths::math_error& )
    {
        //trying to normalise a zero vector
        norm.set(0.0,0.0,1.0);
        dist = 0.0;
    }
}

/** Copy constructor */
Plane::Plane(const Plane &other) : Primitive<Plane>(), norm(other.norm), dist(other.dist)
{}

/** Destructor */
Plane::~Plane()
{}

Plane& Plane::operator=(const Plane &other)
{
    norm = other.norm;
    dist = other.dist;
    return *this;
}

bool Plane::operator==(const Plane &other) const
{
    return norm == other.norm and dist == other.dist;
}

QString Plane::toString() const
{
    return QObject::tr("Plane( [%1,%2,%3] : %4 )")
                .arg(norm.x()).arg(norm.y()).arg(norm.z()).arg(dist);
}

uint Plane::hashCode() const
{
    return qHash(Plane::typeName()) + qHash(norm) + qHash(dist);
}

void Plane::stream(Stream &s)
{
    s.assertVersion<Plane>(1);
    
    Schema schema = s.item<Plane>();
    
    schema.data("normal") & norm;
    schema.data("distance") & dist;
}
