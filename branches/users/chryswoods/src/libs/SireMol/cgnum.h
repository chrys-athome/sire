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

#ifndef SIREMOL_CGNUM_H
#define SIREMOL_CGNUM_H

#include "SireID/number.h"

#include "cgid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class CGNum;
}

XMLStream& operator<<(XMLStream&, const SireMol::CGNum&);
XMLStream& operator>>(XMLStream&, SireMol::CGNum&);

namespace SireMol
{

/** This ID number is used to identify a CutGroup by the user-supplied
    number

    @author Christopher Woods
*/
class SIREMOL_EXPORT CGNum : public SireID::Number, public CGID
{

friend XMLStream& ::operator<<(XMLStream&, const CGNum&);
friend XMLStream& ::operator>>(XMLStream&, CGNum&);

public:
    CGNum() : SireID::Number(), CGID()
    {}

    explicit CGNum(quint32 num) : SireID::Number(num), CGID()
    {}

    CGNum(const CGNum &other) : SireID::Number(other), CGID(other)
    {}

    ~CGNum()
    {}
    
    static const char* typeName()
    {
        return "SireMol::CGNum";
    }
    
    const char* what() const
    {
        return CGNum::typeName();
    }
    
    CGNum* clone() const
    {
        return new CGNum(*this);
    }
    
    uint hash() const
    {
        return qHash( static_cast<const SireID::Number&>(*this) );
    }
    
    QString toString() const
    {
        return QString("CGNum(%1)").arg(_num);
    }
    
    CGNum& operator=(const CGNum &other)
    {
        SireID::Number::operator=(other);
        CGID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<CGNum>(*this, other);
    }

    bool operator==(const CGNum &other) const
    {
        return _num == other._num;
    }
    
    bool operator!=(const CGNum &other) const
    {
        return _num != other._num;
    }

    CGIdx map(const MoleculeInfo &molinfo) const;
};

}

Q_DECLARE_TYPEINFO(SireMol::CGNum, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMol::CGNum);

SIRE_END_HEADER

#endif

