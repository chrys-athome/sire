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

#ifndef SIREMOL_CGATOMID_H
#define SIREMOL_CGATOMID_H

#include "atomid.h"
#include "cutgroupid.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class CGAtomID;
}

QDataStream& operator<<(QDataStream&, const SireMol::CGAtomID&);
QDataStream& operator>>(QDataStream&, SireMol::CGAtomID&);

uint qHash(const SireMol::CGAtomID&);

namespace SireMol
{

using boost::tuple;

/** A CGAtomID is the ID number that uniquely identifies an Atom within 
    an identified CutGroup. It is the combination of the CutGroupID with an 
    index number into the CutGroup of the Atom */
class SIREMOL_EXPORT CGAtomID
{

friend QDataStream& ::operator>>(QDataStream&, CGAtomID&);

public:
    explicit CGAtomID(CutGroupID cgid = CutGroupID(0), AtomID atmid = AtomID(0)) 
          : _cgid(cgid), _atmid(atmid)
    {}
    
    CGAtomID(const tuple<CutGroupID,AtomID> &t)
              : _cgid( t.get<0>() ), _atmid( t.get<1>() )
    {}
    
    CGAtomID(const CGAtomID &other) : _cgid(other._cgid), _atmid(other._atmid)
    {}
    
    ~CGAtomID()
    {}
    
    CutGroupID cutGroupID() const
    {
        return _cgid;
    }
    
    AtomID atomID() const
    {
        return _atmid;
    }
    
    bool operator==(const CGAtomID &other) const
    {
        return _cgid == other._cgid and _atmid == other._atmid;
    }
    
    bool operator!=(const CGAtomID &other) const
    {
        return _cgid != other._cgid or _atmid != other._atmid;
    }
    
    bool operator<(const CGAtomID &other) const
    {
        return _cgid < other._cgid or 
               (_cgid == other._cgid and _atmid < other._atmid);
    }
    
    bool operator<=(const CGAtomID &other) const
    {
        return _cgid < other._cgid or
               (_cgid == other._cgid and _atmid <= other._atmid);
    }
    
    bool operator>(const CGAtomID &other) const
    {
        return _cgid > other._cgid or
               (_cgid == other._cgid and _atmid > other._atmid);
    }
    
    bool operator>=(const CGAtomID &other) const
    {
        return _cgid > other._cgid or
               (_cgid == other._cgid and _atmid >= other._atmid);
    }
    
    QString toString() const;
    
private:
    CutGroupID _cgid;
    AtomID _atmid;
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::CGAtomID, Q_MOVABLE_TYPE);

/** Hash this ID type */
inline uint qHash(const SireMol::CGAtomID &id)
{
    return ( (qHash(id.cutGroupID())) << 16 ) | 
           ( (qHash(id.atomID())) | 0x0000FFFF );
}

SIRE_END_HEADER

#endif
