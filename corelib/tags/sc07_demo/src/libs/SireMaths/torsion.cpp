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

#include "torsion.h"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireUnits;
using namespace SireMaths;

static RegisterMetaType<Torsion> r_torsion;

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Torsion &torsion)
{
    writeHeader(ds, r_torsion, 1) << torsion.points[0] << torsion.points[1]
                                  << torsion.points[2] << torsion.points[3];

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Torsion &torsion)
{
    VersionID v = readHeader(ds, r_torsion);

    if (v == 1)
    {
        ds >> torsion.points[0] >> torsion.points[1]
           >> torsion.points[2] >> torsion.points[3];
    }
    else
        throw version_error(v, "1", r_torsion, CODELOC);

    return ds;
}

/** Construct a zero torsion */
Torsion::Torsion()
{}

/** Construct a torsion from the points 0 to 4 */
Torsion::Torsion(const Vector &point0, const Vector &point1,
                 const Vector &point2, const Vector &point3)
{
    points[0] = point0;
    points[1] = point1;
    points[2] = point2;
    points[3] = point3;
}

/** Destructor */
Torsion::~Torsion()
{}

/** Return a string representation of this torsion */
QString Torsion::toString() const
{
    return QObject::tr("Torsion: Angle %1 degrees, length03 = %2")
                  .arg(angle().to(degrees)).arg(vector03().length());
}
