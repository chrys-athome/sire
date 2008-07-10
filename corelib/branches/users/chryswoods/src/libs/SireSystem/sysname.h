/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIRESYSTEM_SYSNAME_H
#define SIRESYSTEM_SYSNAME_H

#include "SireID/name.h"

#include "sysid.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{

/** This class holds the name of a simulation system
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SysName : public SireID::Name, public SysID
{

public:
    SysName();
    explicit SysName(const QString &name);
    
    SysName(const SysName &other);
    
    ~SysName();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<SysName>() );
    }
    
    const char* what() const
    {
        return SysName::typeName();
    }
    
    SysName* clone() const
    {
        return new SysName(*this);
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
        return QString("SysName('%1')").arg(_name);
    }
    
    SysName& operator=(const SysName &other)
    {
        SireID::Name::operator=(other);
        SysID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<SysName>(*this, other);
    }
    
    bool operator==(const SysName &other) const
    {
        return _name == other._name;
    }
    
    bool operator!=(const SysName &other) const
    {
        return _name != other._name;
    }
};

}

Q_DECLARE_METATYPE( SireSystem::SysName );

SIRE_EXPOSE_CLASS( SireSystem::SysName )

SIRE_END_HEADER

#endif
