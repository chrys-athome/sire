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

#include "complex.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMaths;

static const RegisterMetaType<Complex> r_complex;

/** Serialise a Complex to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const SireMaths::Complex &z)
{
    writeHeader(ds, r_complex, 1) << z.real() << z.imag();

    return ds;
}

/** Deserialise a Complex from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, SireMaths::Complex &z)
{
    VersionID v = readHeader(ds, r_complex);

    if (v == 1)
    {
        double r,i;
        ds >> r >> i;
        z.setRectangular(r,i);
    }
    else
        throw version_error(v, "1", r_complex, CODELOC);

    return ds;
}

const char* Complex::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Complex>() );
}
