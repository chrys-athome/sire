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

#ifndef SIREMOL_ATOMIDX_H
#define SIREMOL_ATOMIDX_H

#include "SireID/index.h"

#include "atomid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

class CutGroupID;
class ResidueID;
class SegmentID;

/** This is an ID object that is used to index atoms (e.g. index
    in a list or array, or in a molecule).

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomIdx 
       : public SireID::Index_T_<AtomIdx>, public AtomID
{
public:
    AtomIdx() : SireID::Index_T_<AtomIdx>(), AtomID()
    {}
    
    explicit AtomIdx(quint32 idx) 
              : SireID::Index_T_<AtomIdx>(idx), AtomID()
    {}
    
    AtomIdx(const AtomIdx &other) 
              : SireID::Index_T<AtomIdx>(other), AtomID(other)
    {}
    
    ~AtomIdx()
    {}
    
    static const char* typeName()
    {
        return "SireMol::AtomIdx";
    }
    
    const char* what() const
    {
        return AtomIdx::typeName();
    }
    
    AtomIdx* clone() const
    {
        return new AtomIdx(*this);
    }
    
    uint hash() const
    {
        return qHash( static_cast<const SireID::Index&>(*this) );
    }

    QString toString() const
    {
        return QString("AtomID(%1)").arg(_idx);
    }
    
    AtomIdx& operator=(const AtomIdx &other)
    {
        SireID::Index::operator=(other);
        SireID::ID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<AtomIdx>(*this, other);
    }
    
    using SireID::Index_T_<AtomIdx>::operator=;

    using SireID::Index_T_<AtomIdx>::operator==;
    using SireID::Index_T_<AtomIdx>::operator!=;

    using SireID::Index_T_<AtomIdx>::operator+=;
    using SireID::Index_T_<AtomIdx>::operator++;
    using SireID::Index_T_<AtomIdx>::operator-=;
    using SireID::Index_T_<AtomIdx>::operator--;
    
    CGAtomIdx map(const MoleculeInfo &molinfo) const;

    CGAtomIdx map(const MoleculeInfo &molinfo, const ResidueID &resid) const;
    CGAtomIdx map(const MoleculeInfo &molinfo, const CutGroupID &cgid) const;
    CGAtomIdx map(const MoleculeInfo &molinfo, const SegmentID &segid) const;
};
    
}

Q_DECLARE_TYPEINFO(SireMol::AtomIdx, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMol::AtomIdx);

SIRE_END_HEADER

#endif

