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

#ifndef SIREMOL_ATOMNAME_H
#define SIREMOL_ATOMNAME_H

#include "SireID/name.h"

#include "atomid.h"
#include "atomidx.h"
#include "molinfo.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomName;
}

XMLStream& operator<<(XMLStream&, const SireMol::AtomName&);
XMLStream& operator>>(XMLStream&, SireMol::AtomName&);

namespace SireMol
{

/** This class holds the name of an atom. This can be used
    to identify an atom within a residue.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomName : public SireID::Name, public AtomID
{

friend XMLStream& ::operator<<(XMLStream&, const AtomName&);
friend XMLStream& ::operator>>(XMLStream&, AtomName&);

public:
    AtomName() : SireID::Name(), AtomID()
    {}
    
    explicit AtomName(const QString &name) : SireID::Name(name), AtomID()
    {}
    
    AtomName(const AtomName &other) : SireID::Name(other), AtomID(other)
    {}
    
    ~AtomName()
    {}
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<AtomName>() );
    }
    
    const char* what() const
    {
        return AtomName::typeName();
    }
    
    AtomName* clone() const
    {
        return new AtomName(*this);
    }
    
    bool isNull() const
    {
        return SireID::Name::isNull();
    }
    
    uint hash() const
    {
        return qHash(_name);
    }
    
    QString toString() const
    {
        return QString("AtomName('%1')").arg(_name);
    }
    
    AtomName& operator=(const AtomName &other)
    {
        SireID::Name::operator=(other);
        AtomID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<AtomName>(*this, other);
    }
    
    bool operator==(const AtomName &other) const
    {
        return _name == other._name;
    }
    
    bool operator!=(const AtomName &other) const
    {
        return _name != other._name;
    }

    QList<AtomIdx> map(const MolInfo &molinfo) const
    {
        return molinfo.map(*this);
    }
};

}

Q_DECLARE_METATYPE(SireMol::AtomName);

SIRE_EXPOSE_CLASS( SireMol::AtomName )

SIRE_END_HEADER

#endif
