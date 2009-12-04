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

#include "Siren/stream.h"

#include "line.h"

using namespace SireMaths;
using namespace Siren;

static const RegisterPrimitive<Line> r_line;

/** Construct a zero line */
Line::Line() : Primitive<Line>()
{}

/** Construct a line from point0 to point1 */
Line::Line(const Vector &point0, const Vector &point1) : Primitive<Line>()
{
    points[0] = point0;
    points[1] = point1;
}

/** Destructor */
Line::~Line()
{}

Line& Line::operator=(const Line &other)
{
    if (this != &other)
    {
        points[0] = other.points[0];
        points[1] = other.points[1];
    }
    
    return *this;
}

bool Line::operator==(const Line &other) const
{
    return points[0] == other.points[0] and
           points[1] == other.points[1];
}

bool Line::operator!=(const Line &other) const
{
    return not Line::operator==(other);
}

/** Return a string representation of the line */
QString Line::toString() const
{
    return QObject::tr("Line from %1 to %2, length = %3")
            .arg(point(0).toString(),point(1).toString()).arg(length());
}

uint Line::hashCode() const
{
    return qHash( Line::typeName() ) + qHash(points[0]) + 
           qHash(points[1]);
}

void Line::stream(Stream &s)
{
    s.assertVersion<Line>(1);
    
    Schema schema = s.item<Line>();
    
    schema.data("point_0") & points[0];
    schema.data("point_1") & points[1];
}
