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

#include "SireSystem/qhash_siresystem.h"

#include "system.h"
#include "simsystem.h"
#include "moves.h"
#include "checkpoint.h"
#include "localsimsystem.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireFF/errors.h"

using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<System> r_system;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const System &system)
{
    writeHeader(ds, r_system, 1);

    SharedDataStream sds(ds);

    sds << system.sysdata 
        << system.ffields 
        << system.sysmonitors
        << system.is_consistent;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, System &system)
{
    VersionID v = readHeader(ds, r_system);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> system.sysdata 
            >> system.ffields 
            >> system.sysmonitors
            >> system.is_consistent;
    }
    else
        throw version_error(v, "1", r_system, CODELOC);

    return ds;
}

/** Construct a System containing the groups 'groups',
    the forcefields in 'forcefields' and the using the 
    monitors 'monitors' */
System::System(const MoleculeGroups &groups, const ForceFields &forcefields,
               const SystemMonitors &monitors)
       : ffields(forcefields), sysmonitors(monitors), is_consistent(false)
{
    sysdata.add(groups);
}

/** Construct a System called 'name' containing the groups 'groups',
    the forcefields in 'forcefields' and the using the monitors 'monitors' */
System::System(const QString &name,
               const MoleculeGroups &groups, const ForceFields &forcefields,
               const SystemMonitors &monitors)
       : sysdata(name), ffields(forcefields),
         sysmonitors(monitors), is_consistent(false)
{
    sysdata.add(groups);
}

/** Construct a System containing the groups 'groups' and using
    the monitors 'monitors' */
System::System(const MoleculeGroups &groups, const SystemMonitors &monitors)
       : sysmonitors(monitors), is_consistent(false)
{
    sysdata.add(groups);
}

/** Construct a System called 'name' containing the groups 'groups' 
    and using the monitors 'monitors' */
System::System(const QString &name, const MoleculeGroups &groups,
               const SystemMonitors &monitors)
       : sysdata(name), sysmonitors(monitors), is_consistent(false)
{
    sysdata.add(groups);
}

/** Construct a System containing the forcefields 'forcefields' 
    and using the monitors 'monitors' */
System::System(const ForceFields &forcefields, const SystemMonitors &monitors)
       : ffields(forcefields), sysmonitors(monitors), is_consistent(false)
{}

/** Construct a System called 'name' containing the forcefields 
    'forcefields' and using the monitors 'monitors' */
System::System(const QString &name, const ForceFields &forcefields,
               const SystemMonitors &monitors)
       : sysdata(name), ffields(forcefields), 
         sysmonitors(monitors), is_consistent(false)
{}

/** Construct a System that consists only of the passed monitors */
System::System(const SystemMonitors &monitors)
       : sysmonitors(monitors), is_consistent(false)
{}

/** Construct a System called 'name' that consists of just
    the passed monitors */
System::System(const QString &name, const SystemMonitors &monitors)
       : sysdata(name), sysmonitors(monitors), is_consistent(false)
{}

/** Construct from a CheckPoint */
System::System(const CheckPoint &checkpoint)
{
    *this = checkpoint;
}

/** Construct from a running simulation */
System::System(QuerySystem &simsystem)
{
    *this = simsystem.checkPoint();
}

/** Copy constructor */
System::System(const System &other)
       : sysdata(other.sysdata), ffields(other.ffields),
         sysmonitors(other.sysmonitors), is_consistent(other.is_consistent)
{}

/** Destructor. This will delete everything in this system unless it is
    referenced elsewhere. */
System::~System()
{}

/** Assignment operator */
System& System::operator=(const System &other)
{
    if (this != &other)
    {
        sysdata = other.sysdata;
        ffields = other.ffields;
        sysmonitors = other.sysmonitors;
        is_consistent = other.is_consistent;
    }

    return *this;
}

/** Assignment operator from a CheckPoint */
System& System::operator=(const CheckPoint &checkpoint)
{
    sysdata = checkpoint.info();
    ffields = checkpoint.forceFields();
    sysmonitors = checkpoint.monitors();
    is_consistent = true;
    
    return *this;
}

/** Assignment operator from a running simulation */
System& System::operator=(QuerySystem &simsystem)
{
    return this->operator=(simsystem.checkPoint());
}

/** Comparison operator - two systems are equal if they have the
    same ID and version */
bool System::operator==(const System &other) const
{
    return sysdata == other.sysdata;
}

/** Comparison operator - two systems are equal if they have the
    same ID and version */
bool System::operator!=(const System &other) const
{
    return sysdata != other.sysdata;
}

/** Set the space in which the molecules are mapped */
void System::setSpace(const Space &space)
{
    #warning Need to set the space properly
    sysdata.setSpace(space);
}

/** Prepare ourselves for simulation... */
void System::prepareForSimulation()
{
    //resolve all expressions etc.
    if (not is_consistent)
    {
    
    
    
        is_consistent = true;
    }
}

/** Return a checkpoint of this System - this will update
    the system and set it in a completely consistent state */
CheckPoint System::checkPoint()
{
    this->prepareForSimulation();
    return CheckPoint(*this);
}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This performs 'nmoves' moves.
*/
Moves System::run(const Moves &moves, quint32 nmoves)
{
    LocalSimSystem simsystem(this->checkPoint());

    //work with a copy of the moves
    Moves run_moves(moves);

    run_moves.run(simsystem, nmoves);

    //everything went well - copy back into this system
    *this = simsystem.checkPoint();

    return run_moves;
}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This performs one cycles of the supplied moves
    (e.g. moves.count() moves)
*/
Moves System::run(const Moves &moves)
{
    return this->run( moves, moves.count() );
}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This runs the move 'nmoves' times.
*/
Moves System::run(const Move &move, quint32 nmoves)
{
    return this->run( Moves(move), nmoves );
}
