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

#include "incremint.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
class MajVersion;
}

QDataStream& operator<<(QDataStream&, const SireBase::MajVersion&);
QDataStream& operator>>(QDataStream&, SireBase::MajVersion&);

namespace SireBase
{

class SIREBASE_EXPORT MajVersion
{

friend QDataStream& ::operator<<(QDataStream&, const MajVersion&);
friend QDataStream& ::operator>>(QDataStream&, MajVersion&);

public:
    MajVersion(Incremint *incremint = &MajVersion::shared_id_incremint);
    
    MajVersion(const MajVersion &other);
    
    ~MajVersion();
    
    void increment();
    
    operator quint32() const
    {
        return vers;
    }

private:
    static Incremint shared_id_incremint;

    Incremint *incremint;
    quint32 vers;
};

}

Q_DECLARE_METATYPE(SireBase::MajVersion);

SIRE_END_HEADER

#endif
