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

#ifndef SIREMOL_CHAINIDX_H
#define SIREMOL_CHAINIDX_H

#include "SireID/index.h"

#include "chainid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ChainIdx;
}

QDataStream& operator<<(QDataStream&, const SireMol::ChainIdx&);
QDataStream& operator>>(QDataStream&, SireMol::ChainIdx&);

namespace SireMol
{

/** This is an ID object that is used to index atoms (e.g. index
    in a list or array, or in a molecule).

    @author Christopher Woods
*/
class SIREMOL_EXPORT ChainIdx : public SireID::Index_T_<ChainIdx>, public ChainID
{

friend QDataStream& ::operator<<(QDataStream&, const ChainIdx&);
friend QDataStream& ::operator>>(QDataStream&, ChainIdx&);

public:
    ChainIdx() : SireID::Index_T_<ChainIdx>(), ChainID()
    {}
    
    explicit ChainIdx(qint32 idx) : SireID::Index_T_<ChainIdx>(idx), ChainID()
    {}
    
    ChainIdx(const ChainIdx &other) : SireID::Index_T_<ChainIdx>(other), ChainID(other)
    {}
    
    ~ChainIdx()
    {}
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<ChainIdx>() );
    }
    
    const char* what() const
    {
        return ChainIdx::typeName();
    }
    
    ChainIdx* clone() const
    {
        return new ChainIdx(*this);
    }
    
    static ChainIdx null()
    {
        return ChainIdx();
    }
    
    bool isNull() const
    {
        return SireID::Index_T_<ChainIdx>::isNull();
    }
    
    uint hash() const
    {
        return SireID::Index_T_<ChainIdx>::hash();
    }

    QString toString() const
    {
        return QString("ChainIdx(%1)").arg(_idx);
    }
    
    ChainIdx& operator=(const ChainIdx &other)
    {
        SireID::IndexBase::operator=(other);
        ChainID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<ChainIdx>(*this, other);
    }
    
    using SireID::Index_T_<ChainIdx>::operator=;

    using SireID::Index_T_<ChainIdx>::operator==;
    using SireID::Index_T_<ChainIdx>::operator!=;

    using SireID::Index_T_<ChainIdx>::operator+=;
    using SireID::Index_T_<ChainIdx>::operator++;
    using SireID::Index_T_<ChainIdx>::operator-=;
    using SireID::Index_T_<ChainIdx>::operator--;
    
    using SireID::Index_T_<ChainIdx>::map;
    
    QList<ChainIdx> map(const MolInfo &molinfo) const;
};
    
}

Q_DECLARE_TYPEINFO(SireMol::ChainIdx, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMol::ChainIdx);

SIRE_EXPOSE_CLASS( SireMol::ChainIdx )

SIRE_END_HEADER

#endif

