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

#ifndef SIREMOL_SEGNAME_H
#define SIREMOL_SEGNAME_H

#include "SireID/name.h"

#include "segid.h"
#include "molinfo.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class SegName;
}

XMLStream& operator<<(XMLStream&, const SireMol::SegName&);
XMLStream& operator>>(XMLStream&, SireMol::SegName&);

namespace SireMol
{

/** This class holds the name of a CutGroup.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT SegName : public SireID::Name, public SegID
{

friend XMLStream& ::operator<<(XMLStream&, const SegName&);
friend XMLStream& ::operator>>(XMLStream&, SegName&);

public:
    SegName() : SireID::Name(), SegID()
    {}
    
    explicit SegName(const QString &name) : SireID::Name(name), SegID()
    {}
    
    SegName(const SegName &other) : SireID::Name(other), SegID(other)
    {}
    
    ~SegName()
    {}
    
    static const char* typeName()
    {
        return "SireMol::SegName";
    }
    
    const char* what() const
    {
        return SegName::typeName();
    }
    
    SegName* clone() const
    {
        return new SegName(*this);
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
        return QString("SegName('%1')").arg(_name);
    }
    
    SegName& operator=(const SegName &other)
    {
        SireID::Name::operator=(other);
        SegID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<SegName>(*this, other);
    }
    
    bool operator==(const SegName &other) const
    {
        return _name == other._name;
    }
    
    bool operator!=(const SegName &other) const
    {
        return _name != other._name;
    }

    QList<SegIdx> map(const MolInfo &molinfo) const
    {
        return molinfo.map(*this);
    }
};

}

Q_DECLARE_METATYPE(SireMol::SegName);

SIRE_EXPOSE_CLASS( SireMol::SegName )

SIRE_END_HEADER

#endif
