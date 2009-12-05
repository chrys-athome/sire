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

#include <QDataStream>

#include "triangle.h"

#include "SireUnits/units.h"

#include "Siren/stream.h"

using namespace Siren;
using namespace SireUnits;
using namespace SireMaths;

static const RegisterPrimitive<Triangle> r_triangle;

/** Create a zero triangle */
Triangle::Triangle() : Primitive<Triangle>()
{}

/** Create a triangle from point 0 -> 1 -> 2 */
Triangle::Triangle(const Vector &point0, const Vector &point1, const Vector &point2)
         : Primitive<Triangle>()
{
    points[0] = point0;
    points[1] = point1;
    points[2] = point2;
}

/** Copy constructor */
Triangle::Triangle(const Triangle &other) : Primitive<Triangle>()
{
    for (int i=0; i<3; ++i)
        points[i] = other.points[i];
}

/** Destructor */
Triangle::~Triangle()
{}

Triangle& Triangle::operator=(const Triangle &other)
{
    if (this != &other)
    {
        points[0] = other.points[0];
        points[1] = other.points[1];
        points[2] = other.points[2];
    }
    
    return *this;
}

bool Triangle::operator==(const Triangle &other) const
{
    return points[0] == other.points[0] and
           points[1] == other.points[1] and
           points[2] == other.points[2];
}

/** Return a string representation of the triangle */
QString Triangle::toString() const
{
    return QObject::tr("Triangle: Angles %1 degs, %2 degs, %3 degs")
                .arg(angle0().to(degrees))
                .arg(angle1().to(degrees))
                .arg(angle2().to(degrees));
}

uint Triangle::hashCode() const
{
    return qHash(Triangle::typeName()) + qHash(points[0])
            + qHash(points[1]) + qHash(points[2]);
}

void Triangle::stream(Stream &s)
{
    s.assertVersion<Triangle>(1);
    
    Schema schema = s.item<Triangle>();
    
    schema.data("point_0") & points[0];
    schema.data("point_1") & points[1];
    schema.data("point_2") & points[2];
}
