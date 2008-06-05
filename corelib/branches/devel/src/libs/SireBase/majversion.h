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

#ifndef SIREBASE_MAJVERSION_H
#define SIREBASE_MAJVERSION_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
class MajVersion;
}

QDataStream& operator<<(QDataStream&, const SireBase::MajVersion&);
QDataStream& operator>>(QDataStream&, SireBase::MajVersion&);

namespace SireBase
{

class Incremint;

/** This class provides a single ID or version number,
    that can be used to version or ID an object. This
    uses an Incremint to increment the number, so
    thereby guaranteeing uniqueness (at least for that
    incremint)

    @author Christopher Woods
*/
class SIREBASE_EXPORT MajVersion
{

friend QDataStream& ::operator<<(QDataStream&, const MajVersion&);
friend QDataStream& ::operator>>(QDataStream&, MajVersion&);

public:
    MajVersion();
    MajVersion(Incremint *incremint);

    MajVersion(const MajVersion &other);

    ~MajVersion();

    MajVersion& operator=(const MajVersion &other);

    bool operator==(const MajVersion &other) const;
    bool operator!=(const MajVersion &other) const;

    void increment();

    void assertSameVersion(const MajVersion &other) const;

    /** Allow automatic casting to a quint32,
        - this returns the version number */
    operator quint32() const
    {
        return vers;
    }

private:
    /** Pointer to the incremint used to increment the
        version number */
    Incremint *incremint;

    /** The actual version number */
    quint32 vers;
};

}

Q_DECLARE_METATYPE(SireBase::MajVersion);

SIRE_END_HEADER

#endif
