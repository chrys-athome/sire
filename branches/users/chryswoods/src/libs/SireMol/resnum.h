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

#ifndef SIREMOL_RESNUM_H
#define SIREMOL_RESNUM_H

#include "SireID/number.h"

#include "ResID.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResNum;
}

XMLStream& operator<<(XMLStream&, const SireMol::ResNum&);
XMLStream& operator>>(XMLStream&, SireMol::ResNum&);

namespace SireMol
{

/** This ID number is used to identify a CutGroup by the user-supplied
    number

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResNum : public SireID::Number, public ResID
{

friend XMLStream& ::operator<<(XMLStream&, const ResNum&);
friend XMLStream& ::operator>>(XMLStream&, ResNum&);

public:
    ResNum() : SireID::Number(), ResID()
    {}

    explicit ResNum(quint32 num) : SireID::Number(num), ResID()
    {}

    ResNum(const ResNum &other) : SireID::Number(other), ResID(other)
    {}

    ~ResNum()
    {}
    
    static const char* typeName()
    {
        return "SireMol::ResNum";
    }
    
    const char* what() const
    {
        return ResNum::typeName();
    }
    
    ResNum* clone() const
    {
        return new ResNum(*this);
    }
    
    bool isNull() const
    {
        return SireID::Number::isNull();
    }
    
    uint hash() const
    {
        return qHash( static_cast<const SireID::Number&>(*this) );
    }
    
    QString toString() const
    {
        return QString("ResNum(%1)").arg(_num);
    }
    
    ResNum& operator=(const ResNum &other)
    {
        SireID::Number::operator=(other);
        ResID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<ResNum>(*this, other);
    }

    bool operator==(const ResNum &other) const
    {
        return _num == other._num;
    }
    
    bool operator!=(const ResNum &other) const
    {
        return _num != other._num;
    }

    QList<ResIdx> map(const MoleculeInfoData &molinfo) const;
};

}

Q_DECLARE_TYPEINFO(SireMol::ResNum, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMol::ResNum);

SIRE_END_HEADER

#endif
