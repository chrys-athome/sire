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

#include "monitorname.h"
#include "monitoridx.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<SystemMonitors> r_sysmons;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const SystemMonitors &sysmons)
{
    writeHeader(ds, r_sysmons, 1);
    
    SharedDataStream sds(ds);
    
    sds << sysmons.mons_by_name << sysmons.mons_by_idx
        << sysmons.mons_by_freqency << sysmons.stepnum;

    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, SystemMonitors &sysmons)
{
    VersionID v = readHeader(ds, r_sysmons);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> sysmons.mons_by_name >> sysmons.mons_by_idx
            >> sysmons.mons_by_frequency >> sysmons.stepnum;
    }
    else
        throw version_error(v, "1", r_sysmons, CODELOC);

    return ds;
}

/** Constructor */
SystemMonitors::SystemMonitors() : stepnum(0)
{}

/** Copy constructor */
SystemMonitors::SystemMonitors(const SystemMonitors &other)
               : mons_by_name(other.mons_by_name),
                 mons_by_idx(other.mons_by_idx),
                 mons_by_frequency(other.mons_by_frequency),
                 stepnum(other.stepnum)
{}

/** Destructor */
SystemMonitors::~SystemMonitors()
{}

/** Copy assignment operator */
SystemMonitors& SystemMonitors::operator=(const SystemMonitors &other)
{
    if (this != &other)
    {
        mons_by_name = other.mons_by_name;
        mons_by_idx = other.mons_by_idx;
        mons_by_frequency = other.mons_by_frequency;
        stepnum = other.stepnum;
    }
    
    return *this;
}

/** Comparison operator */
bool SystemMonitors::operator==(const SystemMonitors &other) const
{
    return mons_by_name == other.mons_by_name and
           mons_by_idx == other.mons_by_idx and
           mons_by_frequency == other.mons_by_frequency and
           stepnum == other.stepnum;
}

/** Comparison operator */
bool SystemMonitors::operator!=(const SystemMonitors &other) const
{
    return not this->operator==(other);
}

/** Return whether or not this is empty (contains no monitors) */
bool SystemMonitors::isEmpty() const
{
    return mons_by_idx.isEmpty();
}
