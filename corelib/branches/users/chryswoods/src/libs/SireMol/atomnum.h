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

#ifndef SIREMOL_ATOMNUM_H
#define SIREMOL_ATOMNUM_H

#include "atomid.h"
#include "atomidx.h"

#include "SireID/number.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomNum;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomNum&);
QDataStream& operator>>(QDataStream&, SireMol::AtomNum&);

namespace SireMol
{

/** This ID number is used to identify an atom by the user-supplied
    atom number (this is typically the number assigned to the
    atom from the PDB or other coordinate file)

    Be careful not to confuse this with AtomID, which is the
    index of the atom in the residue or CutGroup (e.g. the
    fifth atom in the residue would have AtomID '4' but has
    whatever AtomNum the user supplied.

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomNum : public SireID::Number, public AtomID
{

friend QDataStream& ::operator<<(QDataStream&, const AtomNum&);
friend QDataStream& ::operator>>(QDataStream&, AtomNum&);

public:
    AtomNum() : SireID::Number(), AtomID()
    {}

    explicit AtomNum(quint32 num) : SireID::Number(num), AtomID()
    {}

    AtomNum(const AtomNum &other) : SireID::Number(other), AtomID(other)
    {}

    ~AtomNum()
    {}
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<AtomNum>() );
    }
    
    const char* what() const
    {
        return AtomNum::typeName();
    }
    
    AtomNum* clone() const
    {
        return new AtomNum(*this);
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
        return QString("AtomNum(%1)").arg(_num);
    }
    
    AtomNum& operator=(const AtomNum &other)
    {
        SireID::Number::operator=(other);
        AtomID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<AtomNum>(*this, other);
    }

    bool operator==(const AtomNum &other) const
    {
        return _num == other._num;
    }
    
    bool operator!=(const AtomNum &other) const
    {
        return _num != other._num;
    }

    QList<AtomIdx> map(const MolInfo &molinfo) const;
};

}

Q_DECLARE_TYPEINFO(SireMol::AtomNum, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMol::AtomNum);

SIRE_EXPOSE_CLASS( SireMol::AtomNum )

SIRE_END_HEADER

#endif
