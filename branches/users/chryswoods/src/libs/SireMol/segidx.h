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

#ifndef SIREMOL_SEGIDX_H
#define SIREMOL_SEGIDX_H

#include "SireID/index.h"

#include "SegID.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class SegIdx;
}

XMLStream& operator<<(XMLStream&, const SireMol::SegIdx&);
XMLStream& operator>>(XMLStream&, SireMol::SegIdx&);

namespace SireMol
{

/** This is an ID object that is used to index CutGroups

    @author Christopher Woods
*/
class SIREMOL_EXPORT SegIdx 
       : public SireID::Index_T_<SegIdx>, public SegID
{

friend XMLStream& ::operator<<(XMLStream&, const SegIdx&);
friend XMLStream& ::operator>>(XMLStream&, SegIdx&);

public:
    SegIdx() : SireID::Index_T_<SegIdx>(), SegID()
    {}
    
    explicit SegIdx(quint32 idx) 
              : SireID::Index_T_<SegIdx>(idx), SegID()
    {}
    
    SegIdx(const SegIdx &other) 
              : SireID::Index_T_<SegIdx>(other), SegID(other)
    {}
    
    ~SegIdx()
    {}
    
    static const char* typeName()
    {
        return "SireMol::SegIdx";
    }
    
    const char* what() const
    {
        return SegIdx::typeName();
    }
    
    SegIdx* clone() const
    {
        return new SegIdx(*this);
    }
    
    static SegIdx null()
    {
        return SegIdx();
    }
    
    bool isNull() const
    {
        return SireID::Index_T_<SegIdx>::isNull();
    }
    
    uint hash() const
    {
        return SireID::Index_T_<SegIdx>::hash();
    }

    QString toString() const
    {
        return QString("SegIdx(%1)").arg(_idx);
    }
    
    SegIdx& operator=(const SegIdx &other)
    {
        SireID::IndexBase::operator=(other);
        SegID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<SegIdx>(*this, other);
    }
    
    using SireID::Index_T_<SegIdx>::operator=;

    using SireID::Index_T_<SegIdx>::operator==;
    using SireID::Index_T_<SegIdx>::operator!=;

    using SireID::Index_T_<SegIdx>::operator+=;
    using SireID::Index_T_<SegIdx>::operator++;
    using SireID::Index_T_<SegIdx>::operator-=;
    using SireID::Index_T_<SegIdx>::operator--;
    
    using SireID::Index_T_<SegIdx>::map;
    
    QList<SegIdx> map(const MoleculeInfoData&) const
    {
        QList<SegIdx> segidxs;
        segidxs.append(*this);
        return segidxs;
    }
};
    
}

Q_DECLARE_TYPEINFO(SireMol::SegIdx, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMol::SegIdx);

SIRE_END_HEADER

#endif
