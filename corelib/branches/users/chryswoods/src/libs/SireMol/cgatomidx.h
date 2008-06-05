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

#ifndef SIREMOL_CGATOMIDX_H
#define SIREMOL_CGATOMIDX_H

#include "cgidx.h"
#include "atomidx.h"
#include "molinfo.h"

#include "SireID/index.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class CGAtomIdx;
}

QDataStream& operator<<(QDataStream&, const SireMol::CGAtomIdx&);
QDataStream& operator>>(QDataStream&, SireMol::CGAtomIdx&);

namespace SireMol
{

/** This is the basic type used to ID atoms within a molecule. This
    provides the fastest way of indexing atoms and is the base
    type that all other AtomID classes map to.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT CGAtomIdx : public AtomID
{

friend QDataStream& ::operator<<(QDataStream&, const CGAtomIdx&);
friend QDataStream& ::operator>>(QDataStream&, CGAtomIdx&);

public:
    CGAtomIdx() : AtomID()
    {}
    
    CGAtomIdx(CGIdx cgid, SireID::Index atomid)
          : AtomID(), _cgidx(cgid), _atmidx(atomid)
    {}
    
    CGAtomIdx(const CGAtomIdx &other)
          : AtomID(other), _cgidx(other._cgidx), _atmidx(other._atmidx)
    {}
    
    ~CGAtomIdx()
    {}
    
    static const char* typeName()
    {
        return "SireMol::CGAtomIdx";
    }
    
    const char* what() const
    {
        return CGAtomIdx::typeName();
    }
    
    CGAtomIdx* clone() const
    {
        return new CGAtomIdx(*this);
    }
    
    static CGAtomIdx null()
    {
        return CGAtomIdx( CGIdx::null(), SireID::Index::null() );
    } 
    
    bool isNull() const
    {
        return _cgidx.isNull() and _atmidx.isNull();
    }
    
    uint hash() const
    {
        return (qHash(_cgidx) << 16) | (qHash(_atmidx) & 0x0000FFFF);
    }
    
    QString toString() const
    {
        return QString("{%1,%2}").arg(_cgidx.toString(), _atmidx.toString());
    }
    
    CGAtomIdx& operator=(const CGAtomIdx &other)
    {
        _cgidx = other._cgidx;
        _atmidx = other._atmidx;
        
        AtomID::operator=(other);
        
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<CGAtomIdx>(*this, other);
    }
    
    bool operator==(const CGAtomIdx &other) const
    {
        return _cgidx == other._cgidx and _atmidx == other._atmidx;
    }
    
    bool operator!=(const CGAtomIdx &other) const
    {
        return _cgidx != other._cgidx or _atmidx != other._atmidx;
    }
    
    QList<AtomIdx> map(const MolInfo &molinfo) const
    {
        QList<AtomIdx> atomidxs;
        atomidxs.append( molinfo.getAtom(_cgidx, _atmidx) );
        return atomidxs;
    }
    
    CGIdx cutGroup() const
    {
        return _cgidx;
    }
    
    SireID::Index atom() const
    {
        return _atmidx;
    }
    
private:
    /** The index of the CutGroup that contains the atom */
    CGIdx _cgidx;
    
    /** The index of the atom within the CutGroup */
    SireID::Index _atmidx;
};

}

inline QDataStream& operator<<(QDataStream &ds, 
                               const SireMol::CGAtomIdx &cgatomidx)
{
    ds << cgatomidx._cgidx << cgatomidx._atmidx;
    return ds;
}

inline QDataStream& operator>>(QDataStream &ds,
                               SireMol::CGAtomIdx &cgatomidx)
{
    ds >> cgatomidx._cgidx >> cgatomidx._atmidx;
    return ds;
}

Q_DECLARE_TYPEINFO(SireMol::CGAtomIdx, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMol::CGAtomIdx);

SIRE_END_HEADER

#endif
