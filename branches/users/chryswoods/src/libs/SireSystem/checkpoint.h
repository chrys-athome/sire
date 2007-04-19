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

#ifndef SIRESYSTEM_CHECKPOINT_H
#define SIRESYSTEM_CHECKPOINT_H

#include "systemdata.h"
#include "systemmonitors.h"

#include "SireFF/forcefields.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class CheckPoint;
}

QDataStream& operator<<(QDataStream&, const SireSystem::CheckPoint&);
QDataStream& operator>>(QDataStream&, SireSystem::CheckPoint&);

namespace SireSystem
{

class System;
class QuerySystem;

/** This class holds a checkpoint for a running simulation

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT CheckPoint
{

friend QDataStream& ::operator<<(QDataStream&, const CheckPoint&);
friend QDataStream& ::operator>>(QDataStream&, CheckPoint&);

public:
    CheckPoint();

    CheckPoint(const System &system);
    CheckPoint(const QuerySystem &system);

    CheckPoint(const CheckPoint &other);

    ~CheckPoint();

    CheckPoint& operator=(const CheckPoint &other);
    CheckPoint& operator=(const System &system);
    CheckPoint& operator=(const QuerySystem &system);

    bool operator==(const CheckPoint &other) const;
    bool operator!=(const CheckPoint &other) const;

    SystemID ID() const;
    const Version& version() const;

    const ForceFields& forceFields() const;
    const SystemData& info() const;
    const SystemMonitors& monitors() const;

private:
    /** Metainfo about the system */
    SystemData sysdata;

    /** Copies of the forcefields that describe the
        potential energy surface */
    ForceFields ffields;

    /** Copies of the monitors that are used
        to monitor properties and energies during
        the simulation */
    SystemMonitors sysmonitors;
};

/** Return the ID number of the system being checkpointed */
inline SystemID CheckPoint::ID() const
{
    return sysdata.ID();
}

/** Return the version number of the forcefield at this checkpoint */
inline const Version& CheckPoint::version() const
{
    return sysdata.version();
}

/** Return the metadata describing the system */
inline const SystemData& CheckPoint::info() const
{
    return sysdata;
}

/** Return the forcefields that describe the potential
    energy surface of the system */
inline const ForceFields& CheckPoint::forceFields() const
{
    return ffields;
}

/** Return the monitors that are used to monitor properties
    and energies during the simulation */
inline const SystemMonitors& CheckPoint::monitors() const
{
    return sysmonitors;
}

}

Q_DECLARE_METATYPE(SireSystem::CheckPoint);

SIRE_END_HEADER

#endif
