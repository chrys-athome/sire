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

#ifndef SIRESYSTEM_MONITORS_H
#define SIRESYSTEM_MONITORS_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class SystemMonitors;
}

QDataStream& operator<<(QDataStream&, const SireSystem::SystemMonitors&);
QDataStream& operator>>(QDataStream&, SireSystem::SystemMonitors&);

namespace SireSystem
{

/** This class holds a set of SystemMonitor objects, and controls
    when those monitors are evaluated on a system
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SystemMonitors
{

friend QDataStream& ::operator<<(QDataStream&, const SystemMonitors&);
friend QDataStream& ::operator>>(QDataStream&, SystemMonitors&);

public:
    SystemMonitors();
    
    ~SystemMonitors();
    
    bool isEmpty() const;
};

}

Q_DECLARE_METATYPE( SireSystem::SystemMonitors )

SIRE_EXPOSE_CLASS( SireSystem::SystemMonitors )

SIRE_END_HEADER

#endif
