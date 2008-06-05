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

#ifndef SIREMOL_CGNUMATOMID_H
#define SIREMOL_CGNUMATOMID_H

#include "atomid.h"
#include "cutgroupnum.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class CGNumAtomID;
}

QDataStream& operator<<(QDataStream&, const SireMol::CGNumAtomID&);
QDataStream& operator>>(QDataStream&, SireMol::CGNumAtomID&);

uint qHash(const SireMol::CGNumAtomID&);

namespace SireMol
{

using boost::tuple;

/** A CGNumAtomID is the ID number that uniquely identifies an Atom within
    an identified CutGroup. It is the combination of the CutGroupNum with an
    index number into the CutGroup of the Atom */
class SIREMOL_EXPORT CGNumAtomID
{

friend QDataStream& ::operator>>(QDataStream&, CGNumAtomID&);

public:
    explicit CGNumAtomID(CutGroupNum cgid = CutGroupNum(0), AtomID atmid = AtomID(0))
          : _cgnum(cgid), _atmid(atmid)
    {}

    CGNumAtomID(const tuple<CutGroupNum,AtomID> &t)
              : _cgnum( t.get<0>() ), _atmid( t.get<1>() )
    {}

    CGNumAtomID(const CGNumAtomID &other) : _cgnum(other._cgnum), _atmid(other._atmid)
    {}

    ~CGNumAtomID()
    {}

    CutGroupNum cutGroupNum() const
    {
        return _cgnum;
    }

    AtomID atomID() const
    {
        return _atmid;
    }

    bool operator==(const CGNumAtomID &other) const
    {
        return _cgnum == other._cgnum and _atmid == other._atmid;
    }

    bool operator!=(const CGNumAtomID &other) const
    {
        return _cgnum != other._cgnum or _atmid != other._atmid;
    }

    bool operator<(const CGNumAtomID &other) const
    {
        return _cgnum < other._cgnum or
               (_cgnum == other._cgnum and _atmid < other._atmid);
    }

    bool operator<=(const CGNumAtomID &other) const
    {
        return _cgnum < other._cgnum or
               (_cgnum == other._cgnum and _atmid <= other._atmid);
    }

    bool operator>(const CGNumAtomID &other) const
    {
        return _cgnum > other._cgnum or
               (_cgnum == other._cgnum and _atmid > other._atmid);
    }

    bool operator>=(const CGNumAtomID &other) const
    {
        return _cgnum > other._cgnum or
               (_cgnum == other._cgnum and _atmid >= other._atmid);
    }

    QString toString() const;

private:
    CutGroupNum _cgnum;
    AtomID _atmid;
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::CGNumAtomID, Q_MOVABLE_TYPE);

/** Hash this ID type */
inline uint qHash(const SireMol::CGNumAtomID &id)
{
    return ( (qHash(id.cutGroupNum())) << 16 ) |
           ( (qHash(id.atomID())) | 0x0000FFFF );
}

SIRE_END_HEADER

#endif
