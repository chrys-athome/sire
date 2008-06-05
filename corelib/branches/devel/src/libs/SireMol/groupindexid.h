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

#ifndef SIREMOL_GROUPINDEXID_H
#define SIREMOL_GROUPINDEXID_H

#include "groupid.h"
#include "index.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class GroupIndexID;
}

QDataStream& operator<<(QDataStream&, const SireMol::GroupIndexID&);
QDataStream& operator>>(QDataStream&, SireMol::GroupIndexID&);

uint qHash(const SireMol::GroupIndexID&);

namespace SireMol
{

using boost::tuple;

/** A GroupIndexID is the ID number that uniquely identifies an item
    in a group. It is the combination of the GroupID with an 
    index number into the group */
class SIREMOL_EXPORT GroupIndexID
{

friend QDataStream& ::operator>>(QDataStream&, GroupIndexID&);

public:
    GroupIndexID(GroupID gid = GroupID(0), Index idx = Index(0)) 
              : _gid(gid), _idx(idx)
    {}
    
    GroupIndexID(const tuple<GroupID,Index> &t)
              : _gid( t.get<0>() ), _idx( t.get<1>() )
    {}
    
    GroupIndexID(const GroupIndexID &other) : _gid(other._gid), _idx(other._idx)
    {}
    
    ~GroupIndexID()
    {}
    
    GroupID groupID() const
    {
        return _gid;
    }
    
    Index index() const
    {
        return _idx;
    }
    
    bool operator==(const GroupIndexID &other) const
    {
        return _gid == other._gid and _idx == other._idx;
    }
    
    bool operator!=(const GroupIndexID &other) const
    {
        return _gid != other._gid or _idx != other._idx;
    }
    
    bool operator<(const GroupIndexID &other) const
    {
        return _gid < other._gid or 
               (_gid == other._gid and _idx < other._idx);
    }
    
    bool operator<=(const GroupIndexID &other) const
    {
        return _gid < other._gid or
               (_gid == other._gid and _idx <= other._idx);
    }
    
    bool operator>(const GroupIndexID &other) const
    {
        return _gid > other._gid or
               (_gid == other._gid and _idx > other._idx);
    }
    
    bool operator>=(const GroupIndexID &other) const
    {
        return _gid > other._gid or
               (_gid == other._gid and _idx >= other._idx);
    }
    
    QString toString() const;
    
private:
    GroupID _gid;
    Index _idx;
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::GroupIndexID, Q_MOVABLE_TYPE);

/** Hash this ID type */
inline uint qHash(const SireMol::GroupIndexID &id)
{
    return ( (qHash(id.groupID())) << 16 ) | 
           ( (qHash(id.index())) | 0x0000FFFF );
}

SIRE_END_HEADER

#endif
