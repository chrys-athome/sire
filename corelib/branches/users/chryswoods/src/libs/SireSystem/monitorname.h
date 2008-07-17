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

#ifndef SIRESYSTEM_MONITORNAME_H
#define SIRESYSTEM_MONITORNAME_H

#include "SireID/name.h"

#include "monitorid.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{

/** This class holds the name of a simulation system
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MonitorName : public SireID::Name, public MonitorID
{

public:
    MonitorName();
    explicit MonitorName(const QString &name);
    
    MonitorName(const MonitorName &other);
    
    ~MonitorName();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MonitorName>() );
    }
    
    const char* what() const
    {
        return MonitorName::typeName();
    }
    
    MonitorName* clone() const
    {
        return new MonitorName(*this);
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
        return QString("MonitorName('%1')").arg(_name);
    }
    
    MonitorName& operator=(const MonitorName &other)
    {
        SireID::Name::operator=(other);
        MonitorID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<MonitorName>(*this, other);
    }
    
    bool operator==(const MonitorName &other) const
    {
        return _name == other._name;
    }
    
    bool operator!=(const MonitorName &other) const
    {
        return _name != other._name;
    }
    
    QList<MonitorName> map(const SystemMonitors &monitors) const;
};

}

Q_DECLARE_METATYPE( SireSystem::MonitorName );

SIRE_EXPOSE_CLASS( SireSystem::MonitorName )

SIRE_END_HEADER

#endif
