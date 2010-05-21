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

#include "velocityverlet.h"

#include "SireMol/moleculegroup.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"
#include "SireMol/atommasses.h"
#include "SireMol/atomcoords.h"
#include "SireMol/moleditor.h"

#include "SireSystem/system.h"

#include "SireFF/forcetable.h"

#include "SireMaths/rangenerator.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireUnits/units.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireMol;
using namespace SireFF;
using namespace SireCAS;
using namespace SireVol;
using namespace SireBase;
using namespace SireStream;
using namespace SireUnits;
using namespace SireUnits::Dimension;

static const RegisterMetaType<VelocityVerlet> r_velver;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const VelocityVerlet &velver)
{
    writeHeader(ds, r_velver, 1);
    
    SharedDataStream sds(ds);
    
    sds << velver.timestep << velver.vel_generator
        << static_cast<const Integrator&>(velver);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, VelocityVerlet &velver)
{
    VersionID v = readHeader(ds, r_velver);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> velver.timestep >> velver.vel_generator
            >> static_cast<Integrator&>(velver);
    }
    else
        throw version_error(v, "1", r_velver, CODELOC);
        
    return ds;
}

/** Constructor */
VelocityVerlet::VelocityVerlet()
               : ConcreteProperty<VelocityVerlet,Integrator>(), timestep(0)
{}

/** Construct, specifying the timestep */
VelocityVerlet::VelocityVerlet(const Time &dt)
               : ConcreteProperty<VelocityVerlet,Integrator>(),
                 timestep(dt)
{
    if (timestep < 0)
        timestep = Time(0);
}

/** Copy constructor */
VelocityVerlet::VelocityVerlet(const VelocityVerlet &other)
               : ConcreteProperty<VelocityVerlet,Integrator>(other),
                 timestep(other.timestep), vel_generator(other.vel_generator)
{}

/** Destructor */
VelocityVerlet::~VelocityVerlet()
{}

/** Copy assignment operator */
VelocityVerlet& VelocityVerlet::operator=(const VelocityVerlet &other)
{
    if (this != &other)
    {
        Integrator::operator=(other);
    
        timestep = other.timestep;
        vel_generator = other.vel_generator;
    }
    
    return *this;
}

/** Comparison operator */
bool VelocityVerlet::operator==(const VelocityVerlet &other) const
{
    return Integrator::operator==(other) and
           timestep == other.timestep and
           vel_generator == other.vel_generator;
}

/** Comparison operator */
bool VelocityVerlet::operator!=(const VelocityVerlet &other) const
{
    return Integrator::operator!=(other);
}

/** Return a string representation of this integrator */
QString VelocityVerlet::toString() const
{
    return QObject::tr("VelocityVerlet( timestep == %1 fs )")
                .arg( this->timeStep().to(femtosecond) );
}

/** Set the random number generator used to generate new velocities */
void VelocityVerlet::setGenerator(const RanGenerator &rangenerator)
{
    if (vel_generator->isA<RandomVelocities>())
        vel_generator.edit().asA<RandomVelocities>().setGenerator(rangenerator);
}
                                                        
/** Integrate the coordinates of the atoms in the molecules in 'molgroup'
    using the forces in 'forcetable', using the optionally supplied 
    property map to find the necessary molecular properties 
    
    \throw SireMol::missing_molecule
    \throw SireBase::missing_property
    \throw SireError:invalid_cast
    \throw SireError::incompatible_error
*/
void VelocityVerlet::integrate(IntegratorWorkspace &workspace,
                               const Symbol &nrg_component, 
                               int nmoves, bool record_stats) const
{
    AtomicVelocityWorkspace &ws = workspace.asA<AtomicVelocityWorkspace>();
    
    const double dt = timestep;

    const int nmols = ws.count();
    
    for (int imove=0; imove<nmoves; ++imove)
    {
        ws.calculateForces(nrg_component);
        
        //first integrate the coordinates - loop over all molecules
        for (int i=0; i<nmols; ++i)
        {
            const int nats = ws.nAtoms(i);
        
            Vector *dx = ws.deltaCoordsArray(i);
            const Vector *forces = ws.forceArray(i);
            Vector *vels = ws.velocityArray(i);
            const double *inv_masses = ws.reciprocalMassArray(i);

            for (int j=0; j<nats; ++j)
            {
                // (1/2) a(t) dt = (1/2) -(1/m) f( r(t) ) dt
                const Vector half_a_t_dt = (-0.5*inv_masses[j]*dt) * forces[j];
                
                // v(t + dt/2) = v(t) + (1/2) a(t) dt
                vels[j] += half_a_t_dt;

                // r(t + dt) = r(t) + v(t) dt + (1/2) a(t) dt^2
                //           = r(t) + dt [ v(t) + (1/2) a(t) dt ]
                dx[j] = dt * (vels[j] + half_a_t_dt);
            }
        }

        ws.commitCoordinates();
        ws.calculateForces(nrg_component);
        
        //now need to integrate the velocities
        for (int i=0; i<nmols; ++i)
        {
            const int nats = ws.nAtoms(i);
        
            const Vector *forces = ws.forceArray(i);
            Vector *vels = ws.velocityArray(i);
            const double *inv_masses = ws.reciprocalMassArray(i);

            for (int j=0; j<nats; ++j)
            {
                // a(t + dt) = -(1/m) f( r(t+dt) )

                // v(t + dt) = v(t + dt/2) + (1/2) a(t + dt) dt
                vels[j] -= (0.5 * dt * inv_masses[j]) * forces[j];
            }
        }
        
        ws.commitVelocities();
        
        if (record_stats)
            ws.collectStatistics();
    }
}

/** Set the timestep for integration */
void VelocityVerlet::setTimeStep(const Time &new_timestep)
{
    timestep = new_timestep;
    
    if (timestep < 0)
        timestep = Time(0);
}

/** Return the timestep of integration */
Time VelocityVerlet::timeStep() const
{
    return timestep;
}

/** Create a workspace for this integrator for the molecule group 'molgroup' */
IntegratorWorkspacePtr VelocityVerlet::createWorkspace(
                                                const MoleculeGroup &molgroup) const
{
    return IntegratorWorkspacePtr( new AtomicVelocityWorkspace(molgroup) );
}

const char* VelocityVerlet::typeName()
{
    return QMetaType::typeName( qMetaTypeId<VelocityVerlet>() );
}
