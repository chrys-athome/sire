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

#include "SireSystem/errors.h"

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

Specify<MonitorID> MonitorID::operator[](int i) const
{
    return Specify<MonitorID>(*this, i);
}

Specify<MonitorID> MonitorID::operator()(int i) const
{
    return Specify<MonitorID>(*this, i);
}

Specify<MonitorID> MonitorID::operator()(int i, int j) const
{
    return Specify<MonitorID>(*this, i, j);
}

IDAndSet<MonitorID> MonitorID::operator+(const MonitorID &other) const
{
    return IDAndSet<MonitorID>(*this, other);
}

IDAndSet<MonitorID> MonitorID::operator&&(const MonitorID &other) const
{
    return this->operator+(other);
}

IDOrSet<MonitorID> MonitorID::operator*(const MonitorID &other) const
{
    return IDOrSet<MonitorID>(*this, other);
}

IDOrSet<MonitorID> MonitorID::operator||(const MonitorID &other) const
{
    return this->operator*(other);
}

QList<MonitorName> MonitorID::processMatches(QList<MonitorName> &matches,
                                             const SystemMonitors &monitors) const
{
    if (matches.isEmpty())
        throw SireSystem::missing_monitor( QObject::tr(
            "There are no monitors available that match the ID \"%1\".")
                .arg(this->toString()), CODELOC );
                
    return matches;
}

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

///////
///////

template class Specify<MonitorID>;
template class IDAndSet<MonitorID>;
template class IDOrSet<MonitorID>;

static const RegisterMetaType< Specify<MonitorID> > r_specify_monid;
static const RegisterMetaType< IDAndSet<MonitorID> > r_idandset_monid;
static const RegisterMetaType< IDOrSet<MonitorID> > r_idorset_monid;
