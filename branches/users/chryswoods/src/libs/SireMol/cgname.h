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

#ifndef SIREMOL_CGNAME_H
#define SIREMOL_CGNAME_H

#include "SireID/name.h"

#include "cgid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class CGName;
}

XMLStream& operator<<(XMLStream&, const SireMol::CGName&);
XMLStream& operator>>(XMLStream&, SireMol::CGName&);

namespace SireMol
{

/** This class holds the name of a CutGroup.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT CGName : public SireID::Name, public CGID
{

friend XMLStream& ::operator<<(XMLStream&, const CGName&);
friend XMLStream& ::operator>>(XMLStream&, CGName&);

public:
    CGName() : SireID::Name(), CGID()
    {}
    
    explicit CGName(const QString &name) : SireID::Name(name), CGID()
    {}
    
    CGName(const CGName &other) : SireID::Name(other), CGID(other)
    {}
    
    ~CGName()
    {}
    
    static const char* typeName()
    {
        return "SireMol::CGName";
    }
    
    const char* what() const
    {
        return CGName::typeName();
    }
    
    CGName* clone() const
    {
        return new CGName(*this);
    }
    
    uint hash() const
    {
        return qHash(_name);
    }
    
    QString toString() const
    {
        return QString("CGName('%1')").arg(_name);
    }
    
    CGName& operator=(const CGName &other)
    {
        SireID::Name::operator=(other);
        CGID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<CGName>(*this, other);
    }
    
    bool operator==(const CGName &other) const
    {
        return _name == other._name;
    }
    
    bool operator!=(const CGName &other) const
    {
        return _name != other._name;
    }

    CGIdx map(const MoleculeInfo &molinfo) const;
};

}

Q_DECLARE_METATYPE(SireMol::CGName);

SIRE_END_HEADER

#endif
