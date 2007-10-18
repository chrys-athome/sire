/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREBASE_MAJMINVERSION_H
#define SIREBASE_MAJMINVERSION_H

#include "idpair.h"

#include <QString>

SIRE_BEGIN_HEADER

namespace SireBase
{

/** This class provides a major:minor version number that can be
    incremented in a thread-safe manner (e.g. incrementing will
    result in a new version that is known to be unique - however
    each MajMinVersion is not itself thread-safe).

    There are two version numbers;

    (1) Major number - this is global, and is meant to ID the
        object.

    (2) Minor number - this is local to all objects with the
        same major number, and is guaranteed to be unique
        for all versions of that ID. The minor version number
        is reset when the major number is incremented.

    @author Christopher Woods
*/
class SIREBASE_EXPORT MajMinVersion : protected IDPair
{

public:
    MajMinVersion();
    MajMinVersion(Incremint *majint);

    MajMinVersion(const MajMinVersion &other);
    MajMinVersion(const IDPair &other);

    ~MajMinVersion();

    MajMinVersion& operator=(const MajMinVersion &other);

    bool operator==(const MajMinVersion &other) const;
    bool operator!=(const MajMinVersion &other) const;

    QString toString() const;

    quint32 major() const;
    quint32 minor() const;

    const Version& version() const;

    void incrementMajor();
    void incrementMinor();

    bool sameMajorVersion(const MajMinVersion &other) const;
    bool sameMinorVersion(const MajMinVersion &other) const;
    bool sameVersion(const MajMinVersion &other) const;

    void assertSameMajorVersion(const MajMinVersion &other) const;
    void assertSameMinorVersion(const MajMinVersion &other) const;
    void assertSameVersion(const MajMinVersion &other) const;

private:
    void _pvt_throwMajorError(const MajMinVersion &other) const;
    void _pvt_throwMinorError(const MajMinVersion &other) const;
    void _pvt_throwVersionError(const MajMinVersion &other) const;
};

/** Return the major number */
inline quint32 MajMinVersion::major() const
{
    return IDPair::major();
}

/** Return the minor number */
inline quint32 MajMinVersion::minor() const
{
    return IDPair::minor();
}

/** Return the raw version */
inline const Version& MajMinVersion::version() const
{
    return IDPair::version();
}

/** Increment the minor version number */
inline void MajMinVersion::incrementMinor()
{
    IDPair::incrementMinor();
}

/** Increment the major version number */
inline void MajMinVersion::incrementMajor()
{
    IDPair::incrementMajor();
}

/** Return whether or not this has the same major version as 'other' */
inline bool MajMinVersion::sameMajorVersion(const MajMinVersion &other) const
{
    return IDPair::sameMajorVersion(other);
}

/** Return whether or not this has the same minor version as 'other' */
inline bool MajMinVersion::sameMinorVersion(const MajMinVersion &other) const
{
    return IDPair::sameMinorVersion(other);
}

/** Return whether or not this has the same version as 'other' */
inline bool MajMinVersion::sameVersion(const MajMinVersion &other) const
{
    return IDPair::sameVersion(other);
}

/** Comparison operator */
inline bool MajMinVersion::operator==(const MajMinVersion &other) const
{
    return IDPair::operator==(other);
}

/** Comparison operator */
inline bool MajMinVersion::operator!=(const MajMinVersion &other) const
{
    return IDPair::operator!=(other);
}

/** Assert that this has the same major version as 'other'

    \throw SireError::version_error
*/
inline void MajMinVersion::assertSameMajorVersion(const MajMinVersion &other) const
{
    if (not sameMajorVersion(other))
        this->_pvt_throwMajorError(other);
}

/** Assert that this has the same minor version as 'other'

    \throw SireError::version_error
*/
inline void MajMinVersion::assertSameMinorVersion(const MajMinVersion &other) const
{
    if (not sameMinorVersion(other))
        this->_pvt_throwMinorError(other);
}

/** Assert that this is the same version as 'other'

    \throw SireError::version_error
*/
inline void MajMinVersion::assertSameVersion(const MajMinVersion &other) const
{
    if (not sameVersion(other))
        this->_pvt_throwVersionError(other);
}

}

SIRE_END_HEADER

#endif
