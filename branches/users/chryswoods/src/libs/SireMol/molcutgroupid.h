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

#ifndef SIREMOL_MOLCUTGROUPID_H
#define SIREMOL_MOLCUTGROUPID_H

#include "moleculeid.h"
#include "cutgroupid.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class MolCutGroupID;
}

QDataStream& operator<<(QDataStream&, const SireMol::MolCutGroupID&);
QDataStream& operator>>(QDataStream&, SireMol::MolCutGroupID&);

uint qHash(const SireMol::MolCutGroupID&);

namespace SireMol
{

using boost::tuple;

/** A MolCutGroupID is the ID number that uniquely identifies a CutGroup within 
    an identified Molecule. It is the combination of the MoleculeID with the
    CutGroupID */
class SIREMOL_EXPORT MolCutGroupID
{

friend QDataStream& ::operator>>(QDataStream&, MolCutGroupID&);

public:
    MolCutGroupID(MoleculeID molid = MoleculeID(0), 
                  CutGroupID cgid = CutGroupID(0)) 
        : _molid(molid), _cgid(cgid)
    {}
    
    MolCutGroupID( const tuple<MoleculeID,CutGroupID> &t )
               : _molid( t.get<0>() ), _cgid( t.get<1>() )
    {}
    
    MolCutGroupID(const MolCutGroupID &other) : _molid(other._molid), _cgid(other._cgid)
    {}
    
    ~MolCutGroupID()
    {}
    
    MoleculeID moleculeID() const
    {
        return _molid;
    }
    
    CutGroupID cutGroupID() const
    {
        return _cgid;
    }
    
    bool operator==(const MolCutGroupID &other) const
    {
        return _molid == other._molid and _cgid == other._cgid;
    }
    
    bool operator!=(const MolCutGroupID &other) const
    {
        return _molid != other._molid or _cgid != other._cgid;
    }
    
    bool operator<(const MolCutGroupID &other) const
    {
        return _molid < other._molid or 
               (_molid == other._molid and _cgid < other._cgid);
    }
    
    bool operator<=(const MolCutGroupID &other) const
    {
        return _molid < other._molid or
               (_molid == other._molid and _cgid <= other._cgid);
    }
    
    bool operator>(const MolCutGroupID &other) const
    {
        return _molid > other._molid or
               (_molid == other._molid and _cgid > other._cgid);
    }
    
    bool operator>=(const MolCutGroupID &other) const
    {
        return _molid > other._molid or
               (_molid == other._molid and _cgid >= other._cgid);
    }
    
    QString toString() const;
    
private:
    MoleculeID _molid;
    CutGroupID _cgid;
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::MolCutGroupID, Q_MOVABLE_TYPE);

/** Hash this ID type */
inline uint qHash(const SireMol::MolCutGroupID &id)
{
    return ( (qHash(id.moleculeID())) << 16 ) | 
           ( (qHash(id.cutGroupID())) | 0x0000FFFF );
}

SIRE_END_HEADER

#endif
