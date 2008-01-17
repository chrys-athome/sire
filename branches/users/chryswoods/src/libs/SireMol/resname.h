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

#ifndef SIREMOL_RESNAME_H
#define SIREMOL_RESNAME_H

#include "SireID/name.h"

#include "resid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResName;
}

XMLStream& operator<<(XMLStream&, const SireMol::ResName&);
XMLStream& operator>>(XMLStream&, SireMol::ResName&);

namespace SireMol
{

/** This class holds the name of a CutGroup.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT ResName : public SireID::Name, public ResID
{

friend XMLStream& ::operator<<(XMLStream&, const ResName&);
friend XMLStream& ::operator>>(XMLStream&, ResName&);

public:
    ResName() : SireID::Name(), ResID()
    {}
    
    explicit ResName(const QString &name) : SireID::Name(name), ResID()
    {}
    
    ResName(const ResName &other) : SireID::Name(other), ResID(other)
    {}
    
    ~ResName()
    {}
    
    static const char* typeName()
    {
        return "SireMol::ResName";
    }
    
    const char* what() const
    {
        return ResName::typeName();
    }
    
    ResName* clone() const
    {
        return new ResName(*this);
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
        return QString("ResName('%1')").arg(_name);
    }
    
    ResName& operator=(const ResName &other)
    {
        SireID::Name::operator=(other);
        ResID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<ResName>(*this, other);
    }
    
    bool operator==(const ResName &other) const
    {
        return _name == other._name;
    }
    
    bool operator!=(const ResName &other) const
    {
        return _name != other._name;
    }

    QList<ResIdx> map(const MoleculeInfoData &molinfo) const;
};

}

Q_DECLARE_METATYPE(SireMol::ResName);

SIRE_END_HEADER

#endif

