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

#ifndef SIREMOL_MGIDX_H
#define SIREMOL_MGIDX_H

#include "SireID/index.h"

#include "mgid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MGIdx;
}

QDataStream& operator<<(QDataStream&, const SireMol::MGIdx&);
QDataStream& operator>>(QDataStream&, SireMol::MGIdx&);

namespace SireMol
{

/** This is an ID object that is used to index molecule groups (e.g. index
    in a list or array, or in a MoleculeGroups set).

    @author Christopher Woods
*/
class SIREMOL_EXPORT MGIdx : public SireID::Index_T_<MGIdx>, public MGID
{

friend QDataStream& ::operator<<(QDataStream&, const MGIdx&);
friend QDataStream& ::operator>>(QDataStream&, MGIdx&);

public:
    MGIdx();
    
    explicit MGIdx(qint32 idx);
    
    MGIdx(const MGIdx &other);
    
    ~MGIdx();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MGIdx>() );
    }
    
    const char* what() const
    {
        return MGIdx::typeName();
    }
    
    MGIdx* clone() const
    {
        return new MGIdx(*this);
    }
    
    static MGIdx null()
    {
        return MGIdx();
    }
    
    bool isNull() const
    {
        return SireID::Index_T_<MGIdx>::isNull();
    }
    
    uint hash() const
    {
        return SireID::Index_T_<MGIdx>::hash();
    }

    QString toString() const
    {
        return QString("MGIdx(%1)").arg(_idx);
    }
    
    MGIdx& operator=(const MGIdx &other)
    {
        SireID::IndexBase::operator=(other);
        MGID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<MGIdx>(*this, other);
    }
    
    using SireID::Index_T_<MGIdx>::operator=;

    using SireID::Index_T_<MGIdx>::operator==;
    using SireID::Index_T_<MGIdx>::operator!=;

    using SireID::Index_T_<MGIdx>::operator+=;
    using SireID::Index_T_<MGIdx>::operator++;
    using SireID::Index_T_<MGIdx>::operator-=;
    using SireID::Index_T_<MGIdx>::operator--;
    
    using SireID::Index_T_<MGIdx>::map;
    
    QList<MGNum> map(const MolGroupsBase &molgroups) const;
};
    
}

Q_DECLARE_TYPEINFO(SireMol::MGIdx, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMol::MGIdx);

SIRE_EXPOSE_CLASS( SireMol::MGIdx )

SIRE_END_HEADER

#endif
