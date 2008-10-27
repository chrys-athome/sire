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

#ifndef SIREMOL_MOLIDX_H
#define SIREMOL_MOLIDX_H

#include "SireID/index.h"

#include "molid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MolIdx;
}

XMLStream& operator<<(XMLStream&, const SireMol::MolIdx&);
XMLStream& operator>>(XMLStream&, SireMol::MolIdx&);

namespace SireMol
{

/** This is an ID object that is used to index molecules (e.g. index
    in a list or array, or in a MoleculeGroup).

    @author Christopher Woods
*/
class SIREMOL_EXPORT MolIdx : public SireID::Index_T_<MolIdx>, public MolID
{

friend XMLStream& ::operator<<(XMLStream&, const MolIdx&);
friend XMLStream& ::operator>>(XMLStream&, MolIdx&);

public:
    MolIdx();
    explicit MolIdx(qint32 idx);
    
    MolIdx(const MolIdx &other);
    
    ~MolIdx();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MolIdx>() );
    }
    
    const char* what() const
    {
        return MolIdx::typeName();
    }
    
    MolIdx* clone() const
    {
        return new MolIdx(*this);
    }
    
    static MolIdx null()
    {
        return MolIdx();
    }
    
    bool isNull() const
    {
        return SireID::Index_T_<MolIdx>::isNull();
    }
    
    uint hash() const
    {
        return SireID::Index_T_<MolIdx>::hash();
    }

    QString toString() const
    {
        return QString("MolIdx(%1)").arg(_idx);
    }
    
    MolIdx& operator=(const MolIdx &other)
    {
        SireID::IndexBase::operator=(other);
        MolID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<MolIdx>(*this, other);
    }
    
    using SireID::Index_T_<MolIdx>::operator=;

    using SireID::Index_T_<MolIdx>::operator==;
    using SireID::Index_T_<MolIdx>::operator!=;

    using SireID::Index_T_<MolIdx>::operator+=;
    using SireID::Index_T_<MolIdx>::operator++;
    using SireID::Index_T_<MolIdx>::operator-=;
    using SireID::Index_T_<MolIdx>::operator--;
    
    using SireID::Index_T_<MolIdx>::map;
    
    QList<MolNum> map(const Molecules &molecules) const;
    QList<MolNum> map(const MoleculeGroup &molgroup) const;
    QList<MolNum> map(const MolGroupsBase &molgroups) const;
};
    
}

Q_DECLARE_TYPEINFO(SireMol::MolIdx, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMol::MolIdx);

SIRE_EXPOSE_CLASS( SireMol::MolIdx )

SIRE_END_HEADER

#endif

