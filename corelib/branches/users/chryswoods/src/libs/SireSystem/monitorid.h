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

#ifndef SIRESYSTEM_MONITORID_H
#define SIRESYSTEM_MONITORID_H

#include <QList>

#include "SireID/id.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{

class MonitorIdx;
class MonitorIdentifier;
class MonitorName;

class SystemMonitors;

/** The base class of all system monitor identifiers

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MonitorID : public SireID::ID
{
public:
    typedef MonitorIdx Index;
    typedef MonitorIdentifier Identifier;

    MonitorID();
    MonitorID(const MonitorID &other);

    virtual ~MonitorID();
    
    static const char* typeName()
    {
        return "SireSystem::MonitorID";
    }
    
    virtual MonitorID* clone() const=0;
    
    virtual QList<MonitorName> map(const SystemMonitors &monitors) const=0;
};

}

SIRE_EXPOSE_CLASS( SireSystem::MonitorID )

SIRE_END_HEADER

#endif
