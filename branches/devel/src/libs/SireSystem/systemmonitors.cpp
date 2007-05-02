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

#include "systemmonitors.h"
#include "querysystem.h"

#include "SireStream/datastream.h"

#include <boost/assert.hpp>

using namespace SireStream;
using namespace SireSystem;

static const RegisterMetaType<SystemMonitors> r_sysmons;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const SystemMonitors&)
{
    writeHeader(ds, r_sysmons, 0);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds,
                                    SystemMonitors& )
{
    VersionID v = readHeader(ds, r_sysmons);

    if (v != 0)
        throw version_error(v, "0", r_sysmons, CODELOC);

    return ds;
}

/** Null constructor */
SystemMonitors::SystemMonitors()
{}

/** Copy constructor */
SystemMonitors::SystemMonitors(const SystemMonitors&)
{}

/** Destructor */
SystemMonitors::~SystemMonitors()
{}

/** Copy assignment operator */
SystemMonitors& SystemMonitors::operator=(const SystemMonitors&)
{
    return *this;
}

/** Update all of the monitors with the current state
    of the system in 'system' */
void SystemMonitors::update(QuerySystem &system)
{
    //need a circular dependency - the QuerySystem may only
    //update itself!
    BOOST_ASSERT( this == &(system.monitors()) );
}
