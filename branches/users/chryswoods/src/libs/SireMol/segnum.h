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

#ifndef SIREMOL_SEGNUM_H
#define SIREMOL_SEGNUM_H

#include "SireID/number.h"

#include "SegID.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class SegNum;
}

XMLStream& operator<<(XMLStream&, const SireMol::SegNum&);
XMLStream& operator>>(XMLStream&, SireMol::SegNum&);

namespace SireMol
{

/** This ID number is used to identify a CutGroup by the user-supplied
    number

    @author Christopher Woods
*/
class SIREMOL_EXPORT SegNum : public SireID::Number, public SegID
{

friend XMLStream& ::operator<<(XMLStream&, const SegNum&);
friend XMLStream& ::operator>>(XMLStream&, SegNum&);

public:
    SegNum() : SireID::Number(), SegID()
    {}

    explicit SegNum(quint32 num) : SireID::Number(num), SegID()
    {}

    SegNum(const SegNum &other) : SireID::Number(other), SegID(other)
    {}

    ~SegNum()
    {}
    
    static const char* typeName()
    {
        return "SireMol::SegNum";
    }
    
    const char* what() const
    {
        return SegNum::typeName();
    }
    
    SegNum* clone() const
    {
        return new SegNum(*this);
    }
    
    uint hash() const
    {
        return qHash( static_cast<const SireID::Number&>(*this) );
    }
    
    QString toString() const
    {
        return QString("SegNum(%1)").arg(_num);
    }
    
    SegNum& operator=(const SegNum &other)
    {
        SireID::Number::operator=(other);
        SegID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<SegNum>(*this, other);
    }

    bool operator==(const SegNum &other) const
    {
        return _num == other._num;
    }
    
    bool operator!=(const SegNum &other) const
    {
        return _num != other._num;
    }

    SegName map(const MoleculeInfo &molinfo) const;
};

}

Q_DECLARE_TYPEINFO(SireMol::SegNum, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMol::SegNum);

SIRE_END_HEADER

#endif
