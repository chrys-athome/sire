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

#include "monitorid.h"
#include "monitoridx.h"
#include "monitorname.h"

#include "systemmonitors.h"

using namespace SireSystem;

///////
/////// Implementation of MonitorID
///////

MonitorID::MonitorID() : SireID::ID()
{}

MonitorID::MonitorID(const MonitorID &other) : SireID::ID(other)
{}

MonitorID::~MonitorID()
{}

///////
/////// Implementation of MonitorIdx
///////

MonitorIdx::MonitorIdx() : SireID::Index_T_<MonitorIdx>(), MonitorID()
{}

MonitorIdx::MonitorIdx(qint32 idx) : SireID::Index_T_<MonitorIdx>(idx), MonitorID()
{}

MonitorIdx::MonitorIdx(const MonitorIdx &other) 
           : SireID::Index_T_<MonitorIdx>(other), MonitorID(other)
{}

MonitorIdx::~MonitorIdx()
{}

QList<MonitorName> MonitorIdx::map(const SystemMonitors &monitors) const
{
    return monitors.map(*this);
}

///////
/////// Implementation of MonitorName
///////

MonitorName::MonitorName() : SireID::Name(), MonitorID()
{}

MonitorName::MonitorName(const QString &name) : SireID::Name(name), MonitorID()
{}

MonitorName::MonitorName(const MonitorName &other) : SireID::Name(other), MonitorID(other)
{}

MonitorName::~MonitorName()
{}

QList<MonitorName> MonitorName::map(const SystemMonitors &monitors) const
{
    return monitors.map(*this);
}
