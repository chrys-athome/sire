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

#ifndef SIREBASE_VERSION_H
#define SIREBASE_VERSION_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
class Version;
}

QDataStream& operator<<(QDataStream&, const SireBase::Version&);
QDataStream& operator>>(QDataStream&, SireBase::Version&);

#ifdef major
//glibc #defines major == gnu_dev_major
//This ruins any use of 'major' - this is why macros are BAD!!!
//It does this in sys/sysmacros.h
#undef major
#endif

#ifdef minor
//glibc does the same thing with 'minor' as well...
// #define minor == gnu_dev_minor
#undef minor
#endif

namespace SireBase
{

/** This class provides a simple version number that consists
    of a major and minor number.

    @author Christopher Woods
*/
class SIREBASE_EXPORT Version
{

friend QDataStream& ::operator<<(QDataStream&, const Version&);
friend QDataStream& ::operator>>(QDataStream&, Version&);

public:
    Version(quint32 major=0, quint32 minor=0);
    Version(const Version &other);

    ~Version();

    Version& operator=(const Version &other);

    bool operator==(const Version &other) const
    {
        return _major == other._major and _minor == other._minor;
    }

    bool operator!=(const Version &other) const
    {
        return _major != other._major or _minor != other._minor;
    }

    bool operator<(const Version &other) const
    {
        return _major < other._major or 
                  (_major == other._major and _minor < other._minor);
    }

    bool operator<=(const Version &other) const
    {
        return _major < other._major or 
                  (_major == other._major and _minor <= other._minor);
    }

    bool operator>(const Version &other) const
    {
        return _major > other._major or 
                  (_major == other._major and _minor > other._minor);
    }

    bool operator>=(const Version &other) const
    {
        return _major > other._major or 
                  (_major == other._major and _minor >= other._minor);
    }

    bool sameMajorVersion(const Version &other) const
    {
        return _major == other._major;
    }

    bool sameMinorVersion(const Version &other) const
    {
        return _minor == other._minor;
    }

    quint32 major() const
    {
        return _major;
    }

    quint32 minor() const
    {
        return _minor;
    }

    void setMajor(quint32 major)
    {
        _major = major;
    }

    void setMinor(quint32 minor)
    {
        _minor = minor;
    }

    QString toString() const;

private:
    quint32 _major;
    quint32 _minor;
};

}

Q_DECLARE_METATYPE(SireBase::Version);

SIRE_END_HEADER

#endif
