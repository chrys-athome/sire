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

#include "idversion.h"

#include "SireError/errors.h"

using namespace SireBase;

/** Constructor */
IDVersion::IDVersion() : IDPair()
{}

/** Constructor - provide the Incremint that is used
    to get the major version number */
IDVersion::IDVersion(Incremint *majint) : IDPair(majint)
{}

/** Copy constructor */
IDVersion::IDVersion(const IDVersion &other)
              : IDPair(other)
{}

/** Destructor */
IDVersion::~IDVersion()
{}

/** Return a string representation of this version number */
QString IDVersion::toString() const
{
    return QString("%1.%2").arg(major()).arg(minor());
}

/** Internal function */
void IDVersion::_pvt_throwIDError(const IDVersion &other) const
{
    throw SireError::id_error( QObject::tr(
                "Different ID numbers (%1 vs. %2)")
                    .arg(major()).arg(other.major()), CODELOC );
}

/** Internal function */
void IDVersion::_pvt_throwVersionError(const IDVersion &other) const
{
    throw SireError::version_error( QObject::tr(
                "Different version numbers (%1 vs. %2) for objects withs IDs %3 and %4")
                    .arg(minor()).arg(other.minor())
                    .arg(major()).arg(other.major()), CODELOC );
}
