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

#ifndef SIREMOL_RESIDX_H
#define SIREMOL_RESIDX_H

#include "SireID/index.h"

#include "resid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResIdx;
}

XMLStream& operator<<(XMLStream&, const SireMol::ResIdx&);
XMLStream& operator>>(XMLStream&, SireMol::ResIdx&);

namespace SireMol
{

/** This is an ID object that is used to index CutGroups

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResIdx 
       : public SireID::Index_T_<ResIdx>, public ResID
{

friend XMLStream& ::operator<<(XMLStream&, const ResIdx&);
friend XMLStream& ::operator>>(XMLStream&, ResIdx&);

public:
    ResIdx() : SireID::Index_T_<ResIdx>(), ResID()
    {}
    
    explicit ResIdx(quint32 idx) 
              : SireID::Index_T_<ResIdx>(idx), ResID()
    {}
    
    ResIdx(const ResIdx &other) 
              : SireID::Index_T_<ResIdx>(other), ResID(other)
    {}
    
    ~ResIdx()
    {}
    
    static const char* typeName()
    {
        return "SireMol::ResIdx";
    }
    
    const char* what() const
    {
        return ResIdx::typeName();
    }
    
    ResIdx* clone() const
    {
        return new ResIdx(*this);
    }
    
    bool isNull() const
    {
        return SireID::Index_T_<ResIdx>::isNull();
    }
    
    uint hash() const
    {
        return SireID::Index_T_<ResIdx>::hash();
    }

    QString toString() const
    {
        return QString("AtomID(%1)").arg(_idx);
    }
    
    ResIdx& operator=(const ResIdx &other)
    {
        SireID::IndexBase::operator=(other);
        ResID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<ResIdx>(*this, other);
    }
    
    using SireID::Index_T_<ResIdx>::operator=;

    using SireID::Index_T_<ResIdx>::operator==;
    using SireID::Index_T_<ResIdx>::operator!=;

    using SireID::Index_T_<ResIdx>::operator+=;
    using SireID::Index_T_<ResIdx>::operator++;
    using SireID::Index_T_<ResIdx>::operator-=;
    using SireID::Index_T_<ResIdx>::operator--;
    
    using SireID::Index_T_<ResIdx>::map;
    
    QList<ResIdx> map(const MoleculeInfoData&) const
    {
        QList<ResIdx> residxs;
        residxs.append(*this);
        return residxs;
    }
};
    
}

Q_DECLARE_TYPEINFO(SireMol::ResIdx, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMol::ResIdx);

SIRE_END_HEADER

#endif

