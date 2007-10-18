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

#ifndef SIREMOL_RESNUMATOMID_H
#define SIREMOL_RESNUMATOMID_H

#include "atomid.h"
#include "resnum.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResNumAtomID;
}

QDataStream& operator<<(QDataStream&, const SireMol::ResNumAtomID&);
QDataStream& operator>>(QDataStream&, SireMol::ResNumAtomID&);

uint qHash(const SireMol::ResNumAtomID&);

namespace SireMol
{

using boost::tuple;

/** A ResNumAtomID is the ID number that uniquely identifies an Atom within
    an identified Residue (residue identified via residue number). It is the
    combination of the ResNum with an index number into the residue of the Atom

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResNumAtomID
{

friend QDataStream& ::operator>>(QDataStream&, ResNumAtomID&);

public:
    explicit ResNumAtomID(ResNum resnum = ResNum(0), AtomID atmid = AtomID(0)) 
              : _resnum(resnum), _atmid(atmid)
    {}

    ResNumAtomID(const tuple<ResNum,AtomID> &t)
              : _resnum( t.get<0>() ), _atmid( t.get<1>() )
    {}

    ResNumAtomID(const ResNumAtomID &other) : _resnum(other._resnum), _atmid(other._atmid)
    {}

    ~ResNumAtomID()
    {}

    ResNum resNum() const
    {
        return _resnum;
    }

    AtomID atomID() const
    {
        return _atmid;
    }

    bool operator==(const ResNumAtomID &other) const
    {
        return _resnum == other._resnum and _atmid == other._atmid;
    }

    bool operator!=(const ResNumAtomID &other) const
    {
        return _resnum != other._resnum or _atmid != other._atmid;
    }

    bool operator<(const ResNumAtomID &other) const
    {
        return _resnum < other._resnum or
               (_resnum == other._resnum and _atmid < other._atmid);
    }

    bool operator<=(const ResNumAtomID &other) const
    {
        return _resnum < other._resnum or
               (_resnum == other._resnum and _atmid <= other._atmid);
    }

    bool operator>(const ResNumAtomID &other) const
    {
        return _resnum > other._resnum or
               (_resnum == other._resnum and _atmid > other._atmid);
    }

    bool operator>=(const ResNumAtomID &other) const
    {
        return _resnum > other._resnum or
               (_resnum == other._resnum and _atmid >= other._atmid);
    }

    QString toString() const;

private:
    ResNum _resnum;
    AtomID _atmid;
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::ResNumAtomID, Q_MOVABLE_TYPE);

/** Hash this ID type */
inline uint qHash(const SireMol::ResNumAtomID &id)
{
    return ( (qHash(id.resNum())) << 16 ) |
           ( (qHash(id.atomID())) | 0x0000FFFF );
}

SIRE_END_HEADER

#endif
