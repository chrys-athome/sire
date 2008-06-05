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

#include "version.h"

#include "SireStream/datastream.h"

using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<Version> r_version;

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const Version &version)
{
    writeHeader(ds, r_version, 1) << version.major() << version.minor();

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, Version &version)
{
    VersionID v = readHeader(ds, r_version);

    if (v == 1)
    {
        ds >> version._major >> version._minor;
    }
    else
        throw version_error(v, "1", r_version, CODELOC);

    return ds;
}

/** Constructor */
Version::Version(quint32 major, quint32 minor)
        : _major(major), _minor(minor)
{}

/** Copy constructor */
Version::Version(const Version &other)
        : _major(other._major), _minor(other._minor)
{}

/** Destructor */
Version::~Version()
{}

/** Assignment operator */
Version& Version::operator=(const Version &other)
{
    _major = other._major;
    _minor = other._minor;

    return *this;
}

/** Return a string version of this version */
QString Version::toString() const
{
    return QString("%1.%2").arg(_major).arg(_minor);
}
