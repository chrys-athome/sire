/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIRESYSTEM_SYSIDX_H
#define SIRESYSTEM_SYSIDX_H

#include "SireID/index.h"

#include "sysid.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{

/** This is an ID object that is used to index simulation systems (e.g. index
    in a list or array).

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SysIdx : public SireID::Index_T_<SysIdx>, public SysID
{

public:
    SysIdx();
    explicit SysIdx(qint32 idx);
    
    SysIdx(const SysIdx &other);
    
    ~SysIdx();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<SysIdx>() );
    }
    
    const char* what() const
    {
        return SysIdx::typeName();
    }
    
    SysIdx* clone() const
    {
        return new SysIdx(*this);
    }
    
    static SysIdx null()
    {
        return SysIdx();
    }
    
    bool isNull() const
    {
        return SireID::Index_T_<SysIdx>::isNull();
    }
    
    uint hash() const
    {
        return SireID::Index_T_<SysIdx>::hash();
    }

    QString toString() const
    {
        return QString("SysIdx(%1)").arg(_idx);
    }
    
    SysIdx& operator=(const SysIdx &other)
    {
        SireID::IndexBase::operator=(other);
        SysID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<SysIdx>(*this, other);
    }
    
    using SireID::Index_T_<SysIdx>::operator=;

    using SireID::Index_T_<SysIdx>::operator==;
    using SireID::Index_T_<SysIdx>::operator!=;

    using SireID::Index_T_<SysIdx>::operator+=;
    using SireID::Index_T_<SysIdx>::operator++;
    using SireID::Index_T_<SysIdx>::operator-=;
    using SireID::Index_T_<SysIdx>::operator--;
    
    using SireID::Index_T_<SysIdx>::map;
};
    
}

Q_DECLARE_TYPEINFO(SireSystem::SysIdx, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireSystem::SysIdx);

SIRE_EXPOSE_CLASS( SireSystem::SysIdx )

SIRE_END_HEADER

#endif
