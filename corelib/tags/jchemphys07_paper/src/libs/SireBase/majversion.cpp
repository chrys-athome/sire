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

#include "incremint.h"
#include "majversion.h"

#include "SireStream/datastream.h"
#include "SireError/errors.h"

using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<MajVersion> r_majver;

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const MajVersion &majver)
{
    writeHeader(ds, r_majver, 1) << majver.vers;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, MajVersion &majver)
{
    VersionID v = readHeader(ds, r_majver);

    if (v == 1)
    {
        ds >> majver.vers;
    }
    else
        throw version_error(v, "1", r_majver, CODELOC);

    return ds;
}

/** Shared global incremint used by the default constructor */
static Incremint shared_global_incremint;

/** Null constructor - construct a version that
    is incremented from the global MajVersion incremint */
MajVersion::MajVersion() : incremint(&shared_global_incremint)
{
    this->increment();
}

/** Construct using the specified incremint */
MajVersion::MajVersion(Incremint *inc) : incremint(inc)
{
    this->increment();
}

/** Copy constructor */
MajVersion::MajVersion(const MajVersion &other)
           : incremint(other.incremint), vers(other.vers)
{}

/** Destructor */
MajVersion::~MajVersion()
{}

/** Assignment operator */
MajVersion& MajVersion::operator=(const MajVersion &other)
{
    incremint = other.incremint;
    vers = other.vers;

    return *this;
}

/** Comparison operator */
bool MajVersion::operator==(const MajVersion &other) const
{
    return vers == other.vers and incremint == other.incremint;
}

/** Comparison operator */
bool MajVersion::operator!=(const MajVersion &other) const
{
    return vers != other.vers or incremint != other.incremint;
}

/** Increment the version number */
void MajVersion::increment()
{
    vers = incremint->increment();
}

/** Assert that this has the same version number as 'other'

    \throw SireError::version_error
*/
void MajVersion::assertSameVersion(const MajVersion &other) const
{
    if (vers != other.vers)
        throw SireError::version_error( QObject::tr(
                 "Different version numbers (%1 vs. %2)")
                      .arg(vers).arg(other.vers), CODELOC );
}
