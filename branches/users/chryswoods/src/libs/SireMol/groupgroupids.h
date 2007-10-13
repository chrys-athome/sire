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

#ifndef SIREMOL_GROUPGROUPIDS_H
#define SIREMOL_GROUPGROUPIDS_H

#include "groupatomids.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

template<class G0, class G1>
class GroupGroupID : public GroupAtomIDBase
{
public:
    GroupGroupID();
    
    GroupGroupID(const GroupGroupID<G0,G1> &other);
    
    ~GroupGroupID();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< GroupGroupID<G0,G1> >() );
    }
    
    const char* what() const
    {
        return GroupGroupID<G0,G1>::typeName();
    }
       
    GroupGroupID<G0,G1>* clone() const
    {
        return new GroupGroupID<G0,G1>(*this);
    }
    
    using SireID::ID::operator!=;
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare< GroupGroupID<G0,G1> >(*this, other);
    }
    
    bool operator==(const GroupGroupID<G0,G1> &other) const
    {
        return g0 == other.g0 and g1 == other.g1;
    }
    
    bool operator!=(const GroupGroupID<G0,G1> &other) const
    {
        return g0 != other.g0 or g1 != other.g1;
    }
    
    QString toString() const;
    
    uint hash() const
    {
        return (g0.hash() << 16) | (g1.hash() & 0x0000FFFF);
    }
    
    bool isNull() const
    {
        return g0.isNull() and g1.isNull();
    }
             
    QList<AtomIdx> map(const MoleculeInfoData &molinfo) const;

private:
    typename G0::Identifier g0;
    typename G1::Identifier g1;
};

/** Return a string representation of this match */
template<class G0, class G1>
QString GroupGroupID<G0,G1>::toString() const
{
    if (this->isNull())
        return "*";
    else if (g0.isNull())
        return g1.toString();
    else if (g1.isNull())
        return g0.toString();
    else
        return QString("%1 and %2").arg(g0.toString(), g1.toString());
}

/** Map this pair of group IDs to the list of indicies of atoms that are
    in both of these groups.
  
    \throw ???::missing_G0
    \throw ???::missing_G1
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
template<class G0, class G1>
QList<AtomIdx> GroupGroupID<G0,G1>::map(const MoleculeInfoData &molinfo) const
{
    if (this->isNull())
        return molinfo.getAtoms();
    else if (g0.isNull())
        return g1.map(molinfo);
    else if (g1.isNull())
        return g0.map(molinfo);
        
    QList<AtomIdx> atomidxs = 
            MoleculeInfoData::intersection(molinfo.getAtomsIn(g0),
                                           molinfo.getAtomsIn(g1) );
        
    if (atomidxs.isEmpty())
        this->throwMissingAtom(molinfo);
            
    qSort(atomidxs);
    return atomidxs;
}

typedef GroupGroupID<SegID,ResID> SegResID;
typedef GroupGroupID<SegID,ChainID> SegChainID;
typedef GroupGroupID<SegID,CGID> SegCGID;

typedef GroupGroupID<CGID,ResID> CGResID;
typedef GroupGroupID<CGID,ChainID> CGChainID;

SegResID operator+(const SegID &segid, const ResID &resid);
SegResID operator+(const ResID &resid, const SegID &segid);

SegChainID operator+(const SegID &segid, const ChainID &chainid);
SegChainID operator+(const ChainID &chainid, const SegID &segid);

SegCGID operator+(const SegID &segid, const CGID &cgid);
SegCGID operator+(const CGID &cgid, const SegID &segid);

CGResID operator+(const CGID &cgid, const ResID &resid);
CGResID operator+(const ResID &resid, const CGID &cgid);

CGChainID operator+(const CGID &cgid, const ChainID &chainid);
CGChainID operator+(const ChainID &chainid, const CGID &cgid);

}

Q_DECLARE_METATYPE(SireMol::SegResID);
Q_DECLARE_METATYPE(SireMol::SegChainID);
Q_DECLARE_METATYPE(SireMol::SegCGID);
Q_DECLARE_METATYPE(SireMol::CGResID);
Q_DECLARE_METATYPE(SireMol::CGChainID);

SIRE_END_HEADER

#endif
