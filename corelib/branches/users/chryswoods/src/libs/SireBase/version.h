/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "Siren/primitive.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

/** This is a simple class that holds a major and minor version number */
class SIREBASE_EXPORT Version : public Siren::Primitive<Version>
{
public:
    Version();
    Version(quint64 major, quint64 minor);

    Version(const Version &other);
    
    ~Version();
    
    Version& operator=(const Version &other);
    
    bool operator==(const Version &other) const;
    bool operator!=(const Version &other) const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);
    
    quint64 majorVersion() const;
    quint64 minorVersion() const;

private:
    quint64 maj, min;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Copy assignment operator */
inline Version& Version::operator=(const Version &other)
{
    maj = other.maj;
    min = other.min;
    return *this;
}

/** Comparison operator */
inline bool Version::operator==(const Version &other) const
{
    return maj == other.maj and min == other.min;
}

/** Comparison operator */
inline bool Version::operator!=(const Version &other) const
{
    return maj != other.maj or min != other.min;
}

/** Return the major version number. We cannot call this 
    .major() as this name is reserved on linux in sys/sysmacros.h
    (this is a glibc bug) */
inline quint64 Version::majorVersion() const
{
    return maj;
}

/** Return the minor version number. We cannot call this
    .minor() as this name is reserved on linux in sys/sysmacros.h
    (this is a glibc bug) */
inline quint64 Version::minorVersion() const
{
    return min;
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE( SireBase::Version )

SIRE_EXPOSE_PRIMITIVE( SireBase::Version )

SIRE_END_HEADER

#endif
