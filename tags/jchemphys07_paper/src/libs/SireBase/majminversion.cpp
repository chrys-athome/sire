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

#include "majminversion.h"

#include "SireError/errors.h"

using namespace SireBase;

/** Constructor */
MajMinVersion::MajMinVersion() : IDPair()
{}

/** Constructor - provide the Incremint that is used
    to get the major version number */
MajMinVersion::MajMinVersion(Incremint *majint) : IDPair(majint)
{}

/** Copy constructor */
MajMinVersion::MajMinVersion(const IDPair &other)
              : IDPair(other)
{}

/** Copy constructor */
MajMinVersion::MajMinVersion(const MajMinVersion &other)
              : IDPair(other)
{}

/** Destructor */
MajMinVersion::~MajMinVersion()
{}

/** Return a string representation of this version number */
QString MajMinVersion::toString() const
{
    return QString("%1.%2").arg(major()).arg(minor());
}

/** Internal function */
void MajMinVersion::_pvt_throwMajorError(const MajMinVersion &other) const
{
    throw SireError::version_error( QObject::tr(
                "Different major version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}

/** Internal function */
void MajMinVersion::_pvt_throwMinorError(const MajMinVersion &other) const
{
    throw SireError::version_error( QObject::tr(
                "Different minor version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}


/** Internal function */
void MajMinVersion::_pvt_throwVersionError(const MajMinVersion &other) const
{
    throw SireError::version_error( QObject::tr(
                "Different version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}
