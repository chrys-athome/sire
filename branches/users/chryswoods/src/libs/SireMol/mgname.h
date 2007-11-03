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

#ifndef SIREMOL_MGNAME_H
#define SIREMOL_MGNAME_H

#include "SireID/name.h"

#include "mgid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MGName;
}

XMLStream& operator<<(XMLStream&, const SireMol::MGName&);
XMLStream& operator>>(XMLStream&, SireMol::MGName&);

namespace SireMol
{

/** This class holds the name of a MoleculeGroup.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT MGName : public SireID::Name, public MGID
{

friend XMLStream& ::operator<<(XMLStream&, const MGName&);
friend XMLStream& ::operator>>(XMLStream&, MGName&);

public:
    MGName() : SireID::Name(), MGID()
    {}
    
    explicit MGName(const QString &name) : SireID::Name(name), MGID()
    {}
    
    MGName(const MGName &other) : SireID::Name(other), MGID(other)
    {}
    
    ~MGName()
    {}
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MGName>() );
    }
    
    const char* what() const
    {
        return MGName::typeName();
    }
    
    MGName* clone() const
    {
        return new MGName(*this);
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
        return QString("MGName('%1')").arg(_name);
    }
    
    MGName& operator=(const MGName &other)
    {
        SireID::Name::operator=(other);
        MGID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<MGName>(*this, other);
    }
    
    bool operator==(const MGName &other) const
    {
        return _name == other._name;
    }
    
    bool operator!=(const MGName &other) const
    {
        return _name != other._name;
    }
};

}

Q_DECLARE_METATYPE(SireMol::MGName);

SIRE_END_HEADER

#endif

