/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "moleculardynamics.h"
#include "velocityverlet.h"

#include "SireSystem/system.h"

#include "SireMol/moleculegroup.h"

#include "SireVol/space.h"

#include "SireMaths/rangenerator.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireMol;
using namespace SireVol;
using namespace SireBase;
using namespace SireStream;
using namespace SireUnits::Dimension;

static const RegisterMetaType<MolecularDynamics> r_moldyn;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const MolecularDynamics &moldyn)
{
    writeHeader(ds, r_moldyn, 1);
    
    SharedDataStream sds(ds);
    
    sds << moldyn.intgrator << moldyn.wspace << moldyn.num_moves
        << static_cast<const Dynamics&>(moldyn);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        MolecularDynamics &moldyn)
{
    VersionID v = readHeader(ds, r_moldyn);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
    
        sds >> moldyn.intgrator >> moldyn.wspace >> moldyn.num_moves
            >> static_cast<Dynamics&>(moldyn);
             
    }
    else
        throw version_error(v, "1", r_moldyn, CODELOC);
        
    return ds;
}

/** Constructor */
MolecularDynamics::MolecularDynamics()
                  : ConcreteProperty<MolecularDynamics,Dynamics>(),
                    intgrator( Integrator::null() ), 
                    wspace( IntegratorWorkspace::null() ),
                    num_moves(0)
{}

/** Construct to perform moves on the molecules in the group 'molgroup'. This
    defaults to an all-atom velocity-verlet integrator */
MolecularDynamics::MolecularDynamics(const MoleculeGroup &moleculegroup)
                  : ConcreteProperty<MolecularDynamics,Dynamics>(),
                    intgrator( VelocityVerlet() ), num_moves(0)
{
    wspace = intgrator.read().createWorkspace(moleculegroup);
}

/** Construct using the supplied integrator */
MolecularDynamics::MolecularDynamics(const Integrator &integrator)
                  : ConcreteProperty<MolecularDynamics,Dynamics>(),
                    intgrator(integrator), num_moves(0)
{
    wspace = intgrator.read().createWorkspace();
}
    
/** Construct a move for the passed molecule group, integrated
    using the supplied integrator */
MolecularDynamics::MolecularDynamics(const MoleculeGroup &moleculegroup, 
                                     const Integrator &integrator)
                  : ConcreteProperty<MolecularDynamics,Dynamics>(),
                    intgrator(integrator), num_moves(0)
{
    wspace = intgrator.read().createWorkspace(moleculegroup);
}

/** Construct a move for the passed molecule group, integrated
    using the supplied integrator */
MolecularDynamics::MolecularDynamics(const Integrator &integrator, 
                                     const MoleculeGroup &moleculegroup)
                  : ConcreteProperty<MolecularDynamics,Dynamics>(),
                    intgrator(integrator), num_moves(0)
{
    wspace = intgrator.read().createWorkspace(moleculegroup);
}

/** Copy constructor */
MolecularDynamics::MolecularDynamics(const MolecularDynamics &other)
                  : ConcreteProperty<MolecularDynamics,Dynamics>(other),
                    intgrator(other.intgrator), wspace(other.wspace),
                    num_moves(other.num_moves)
{}

/** Destructor */
MolecularDynamics::~MolecularDynamics()
{}

/** Copy assignment operator */
MolecularDynamics& MolecularDynamics::operator=(const MolecularDynamics &other)
{
    intgrator = other.intgrator;
    wspace = other.wspace;
    num_moves = other.num_moves;
    
    Dynamics::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool MolecularDynamics::operator==(const MolecularDynamics &other) const
{
    return intgrator == other.intgrator and
           wspace == other.wspace and
           num_moves == other.num_moves and 
           Dynamics::operator==(other);
}

/** Comparison operator */
bool MolecularDynamics::operator!=(const MolecularDynamics &other) const
{
    return intgrator != other.intgrator or
           wspace != other.wspace or
           num_moves != other.num_moves or
           Dynamics::operator!=(other);
}

/** Return a string representation of this move */
QString MolecularDynamics::toString() const
{
    return QObject::tr("MolecularDynamics( %1, nMoves() == %2 )")
                .arg(intgrator->toString()).arg(num_moves);
}
    
/** Return the number of moves completed using this object */
int MolecularDynamics::nMoves() const
{
    return num_moves;
}
    
/** Return the molecule group on which this move operates */
const MoleculeGroup& MolecularDynamics::moleculeGroup() const
{
    return wspace.read().moleculeGroup();
}

/** Return the integrator used to advance the coordinates
    from one timestep to the next */
const Integrator& MolecularDynamics::integrator() const
{
    return intgrator.read();
}

/** Return the workspace used while integrating the system */
const IntegratorWorkspace& MolecularDynamics::workspace() const
{
    return wspace.read();
}

/** Set the molecule group containing the molecules to be moved */
void MolecularDynamics::setMoleculeGroup(const MoleculeGroup &new_molgroup)
{
    if (new_molgroup.number() != this->moleculeGroup().number())
    {
        wspace = intgrator.read().createWorkspace(new_molgroup);
    }
}

/** Set the integrator to be used to advance the coordinates from 
    one timestep to the next. */
void MolecularDynamics::setIntegrator(const Integrator &integrator)
{
    if (intgrator != integrator)
    {
        intgrator = integrator;
        wspace = intgrator.read().createWorkspace( this->moleculeGroup() );
    }
}

/** Return the timestep for the integration */
Time MolecularDynamics::timeStep() const
{
    return intgrator.read().timeStep();
}

/** Set the timestep for the dynamics integration */
void MolecularDynamics::setTimeStep(const Time &timestep)
{
    if (this->timeStep() != timestep)
    {
        intgrator.edit().setTimeStep(timestep);
    }
}

/** Return the kinetic energy of the system at the last move. */
MolarEnergy MolecularDynamics::kineticEnergy() const
{
    return wspace.read().kineticEnergy();
}

/** Completely clear any move statistics - this clears all existing
    velocities */
void MolecularDynamics::clearStatistics()
{
    wspace = intgrator.read().createWorkspace(this->moleculeGroup());
    num_moves = 0;
}

/** Set the random number generator used by this move
    (this move may be completely deterministic, so may not
     use a generator) */
void MolecularDynamics::setGenerator(const RanGenerator &generator)
{
    intgrator.edit().setGenerator(generator);
}

/** Perform this move on the System 'system' - perform the move
    'nmoves' times, optionally recording simulation statistics
    if 'record_stats' is true */
void MolecularDynamics::move(System &system, int nmoves, bool record_stats)
{
    if (nmoves <= 0)
        return;
        
    MolecularDynamics old_state(*this);
        
    System old_system_state(system);
    
    try
    {
        PropertyMap map;
        map.set("coordinates", this->coordinatesProperty());
        map.set("space", this->spaceProperty());
    
        for (int i=0; i<nmoves; ++i)
        {
            //perform the move
            intgrator.edit().integrate(system, wspace.edit(),
                                       this->energyComponent(), map);

            if (record_stats)
            {
                system.collectStats();
            }
        }
        
        num_moves += nmoves;
    }
    catch(...)
    {
        system = old_system_state;
        this->operator=(old_state);

        throw;
    }
}
