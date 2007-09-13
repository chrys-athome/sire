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

#ifndef SIREMOL_ATOMIDCOMBO_HPP
#define SIREMOL_ATOMIDCOMBO_HPP

#include "atomid.h"
#include "cgatomidx.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
template<class A, class B>
class AtomIDCombo;
}

template<class A, class B>
QDataStream& operator<<(QDataStream&, const SireMol::AtomIDCombo<A,B>&);
template<class A, class B>
QDataStream& operator>>(QDataStream&, SireMol::AtomIDCombo<A,B>&);

template<class A, class B>
XMLStream& operator<<(XMLStream&, const SireMol::AtomIDCombo<A,B>&);
template<class A, class B>
XMLStream& operator>>(XMLStream&, SireMol::AtomIDCombo<A,B>&);

namespace SireMol
{

/** This is the generic version of all paired AtomID combinations, 
    e.g. CGNumAtomNum, ResIdxAtomName etc. This class is used as
    a first resort, and you only have to provide a specific override
    if you find this class to be too slow.
    
    Class A can be any ID object that refers to a group within
    a molecule, while class B must be an ID object that refers
    to a specific atom in the group, e.g. AtomIdx, AtomNum or AtomName
    
    @author Christopher Woods
*/
template<class A, class B>
class SIREMOL_EXPORT AtomIDCombo : public AtomID
{

friend QDataStream& operator<<<>(QDataStream&, const AtomIDCombo<A,B>&);
friend QDataStream& operator>><>(QDataStream&, AtomIDCombo<A,B>&);

friend XMLStream& operator<<<>(XMLStream&, const AtomIDCombo<A,B>&);
friend XMLStream& operator>><>(XMLStream&, AtomIDCombo<A,B>&);

static QString typenam = QString("SireMol::AtomIDCombo<%1,%2>")
                            .arg(A::typeName()).arg(B::typeName());

public:
    AtomIDCombo()
    {}
    
    AtomIDCombo(const A &a, const B &b) : _a(a), _b(b)
    {}
    
    AtomIDCombo(const boost::tuple::tuple<A,B> &pair)
          : _a(pair.get<0>()), _b(pair.get<1>())
    {}
    
    AtomIDCombo(const AtomIDCombo<A,B> &other)
          : _a(other._a), _b(other._b)
    {}
    
    ~AtomIDCombo()
    {}
    
    static const char* typeName()
    {
        return qPrintable(AtomIDCombo<A,B>::typenam);
    }
    
    const char* what() const
    {
        return AtomIDCombo<A,B>::typeName();
    }
    
    AtomIDCombo<A,B>* clone() const
    {
        return new AtomIDCombo<A,B>(*this);
    }
    
    uint hash() const
    {
        return (qHash(_a) << 16) | (qHash(_b) & 0x0000FFFF);
    }
    
    QString toString() const
    {
        return QString("{%1,%2}").arg(_a.toString(), _b.toString());
    }
    
    AtomIDCombo<A,B>& operator=(const AtomIDCombo<A,B> &other)
    {
        _a = other._a;
        _b = other._b;
        AtomID::operator=(other);
        
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare< AtomIDCombo<A,B> >(*this, other);
    }
    
    bool operator==(const AtomIDCombo<A,B> &other) const
    {
        return _a == other._a and _b == other._b;
    }
    
    bool operator!=(const AtomIDCombo<A,B> &other) const
    {
        return _a != other._a or _b != other._b;
    }
    
    CGAtomIdx map(const MoleculeInfo &molinfo) const
    {
        return _b.map(molinfo, _a);
    }
    
private:
    /** The first part of the ID - this identifies the part
        of the molecule that the atom is in (e.g. residue, cutgroup, segment) */
    A _a;
    
    /** The second part of the ID - this identifies the atom
        within the part of the molecule */
    B _b;
};

}

template<class A, class B>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, 
                        const SireMol::AtomIDCombo<A,B> &atomid)
{
    ds << atomid._a << atomid._b;
    return ds;
}

template<class A, class B>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireMol::AtomIDCombo<A,B> &atomid)
{
    ds >> atomid._a >> atomid._b;
    return ds;
}

SIRE_END_HEADER

#endif
