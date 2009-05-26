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
    
    sds 
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
        
        sds 
            >> static_cast<Integrator&>(velver);
    }
    else
        throw version_error(v, "1", r_velver, CODELOC);
        
    return ds;
}

/** Constructor */
VelocityVerlet::VelocityVerlet()
               : ConcreteProperty<VelocityVerlet,Integrator>()
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
               : ConcreteProperty<VelocityVerlet,Integrator>(other)
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
    
    }
    
    return *this;
}

/** Comparison operator */
bool VelocityVerlet::operator==(const VelocityVerlet &other) const
{
    return Integrator::operator==(other);
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

/** Internal function used to regenerate all of the velocities */
void VelocityVerlet::recalculateVelocities()
{
}

/** Internal function used to redimension the force and velocity
    tables to match the molecules in 'new_molgroup' */
void VelocityVerlet::updateTables(const MoleculeGroup &new_molgroup)
{}

/** Internal function used to update this integrator from the 
    passed system - this involves calculating the forces if
    the molecule group or system has changed */
void VelocityVerlet::updateFrom(System &system, const Symbol &nrg_component)
{}

/** Half-integrate the velocities - this returns the new kinetic energy
    of the atoms in the partial molecule */
static double integrateVelocities(const PartialMolecule &molecule,
                                  const PropertyName &masses_property,
                                  MolForceTable &mol_vels, 
                                  const MolForceTable &mol_forces,
                                  const Time &timestep)
{
            // a(t + dt) = -(1/m) f( r(t+dt) )

            // v(t + dt) = v(t + dt/2) + (1/2) a(t + dt) dt

    return 0;
}

/** Integrate the coordinates of the molecule 'molecule' and half-integrate
    the velocities */
static PartialMolecule integrateCoordinates(PartialMolecule molecule, 
                                            const MolForceTable &mol_force, 
                                            MolForceTable &mol_vel,
                                            const Space &space,
                                            const Time &timestep,
                                            const PropertyMap &map)
{
                // a(t) = -(1/m) f( r(t) )

                // r(t + dt) = r(t) + v(t) dt + (1/2) a(t) dt^2
    
                // v(t + dt/2) = v(t) + (1/2) a(t) dt
        
    return molecule;
}
                                                        
/** Integrate the coordinates of the atoms in the molecules in 'molgroup'
    using the forces in 'forcetable', using the optionally supplied 
    property map to find the necessary molecular properties 
    
    \throw SireMol::missing_molecule
    \throw SireBase::missing_property
    \throw SireError:invalid_cast
    \throw SireError::incompatible_error
*/
void VelocityVerlet::integrate(System &system, const Symbol &nrg_component,
                               const PropertyMap &map)
{
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

/** Return the kinetic energy at the last timestep */
MolarEnergy VelocityVerlet::kineticEnergy() const
{
    return last_kinetic_nrg;
}

/** Return the kinetic energy of the passed molecule

    \throw SireMol::missing_molecule
    \throw SireError::incompatible_error
*/
MolarEnergy VelocityVerlet::kineticEnergy(const MoleculeView &molview) const
{
    return MolarEnergy(0);
}

/** Clear all statistics */
void VelocityVerlet::clearStatistics()
{}

/** Clear all of the velocities (they will thus be regenerated
    on the next timestep) */
void VelocityVerlet::clearVelocities()
{
    recalc_velocities = true;
    last_kinetic_nrg = MolarEnergy(0);
}

void VelocityVerlet::clearForces()
{}

QHash<MolNum,AtomVelocities> VelocityVerlet::velocities() const
{
    return QHash<MolNum,AtomVelocities>();
}

AtomVelocities VelocityVerlet::velocities(SireMol::MolID const&) const
{
    return AtomVelocities();
}

ForceTable VelocityVerlet::forceTable() const
{
    return f;
}

AtomForces VelocityVerlet::forces(SireMol::MolID const&) const
{
    return AtomForces();
}

QHash<MolNum,AtomForces> VelocityVerlet::forces() const
{
    return QHash<MolNum,AtomForces>();
}
