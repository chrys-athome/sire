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

#include "idmajminversion.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<IDMajMinVersion> r_idmajmin;

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const IDMajMinVersion &idmajmin)
{
    writeHeader(ds, r_idmajmin, 1)
                << static_cast<const IDTriple&>(idmajmin);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, IDMajMinVersion &idmajmin)
{
    VersionID v = readHeader(ds, r_idmajmin);

    if (v == 1)
    {
        ds >> static_cast<IDTriple&>(idmajmin);
    }
    else
        throw version_error(v, "1", r_idmajmin, CODELOC);

    return ds;
}

/** Constructor */
IDMajMinVersion::IDMajMinVersion() : IDTriple()
{}

/** Constructor - provide the Incremint that is used
    to get the major version number */
IDMajMinVersion::IDMajMinVersion(Incremint *majint) : IDTriple(majint)
{}

/** Copy constructor */
IDMajMinVersion::IDMajMinVersion(const IDTriple &other)
                : IDTriple(other)
{}

/** Copy constructor */
IDMajMinVersion::IDMajMinVersion(const IDMajMinVersion &other)
                : IDTriple(other)
{}

/** Destructor */
IDMajMinVersion::~IDMajMinVersion()
{}

/** Copy assignment */
IDMajMinVersion& IDMajMinVersion::operator=(const IDMajMinVersion &other)
{
    IDTriple::operator=(other);

    return *this;
}

/** Return a string representation of this version number */
QString IDMajMinVersion::toString() const
{
    return QString("%1:%2").arg(ID()).arg(version().toString());
}

/** Internal function */
void IDMajMinVersion::_pvt_throwIDError(const IDMajMinVersion &other) const
{
    throw SireError::id_error( QObject::tr(
                "Different ID numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}

/** Internal function */
void IDMajMinVersion::_pvt_throwMajorError(const IDMajMinVersion &other) const
{
    throw SireError::version_error( QObject::tr(
                "Different major version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}

/** Internal function */
void IDMajMinVersion::_pvt_throwMinorError(const IDMajMinVersion &other) const
{
    throw SireError::version_error( QObject::tr(
                "Different minor version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}


/** Internal function */
void IDMajMinVersion::_pvt_throwVersionError(const IDMajMinVersion &other) const
{
    throw SireError::version_error( QObject::tr(
                "Different version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}
