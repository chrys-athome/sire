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

#include "SireFF/forcetable.h"

#include "SireMaths/rangenerator.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireUnits/units.h"

using namespace SireMove;
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
    
    sds // <<
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
        
        sds // >>
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
    Integrator::operator=(other);
    
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

/** Half-integrate the velocities */
void VelocityVerlet::integrateVelocities(const PartialMolecule &molecule,
                                         MolForceTable &mol_vels, 
                                         const AtomicMasses &masses, 
                                         const MolForceTable &mol_forces) const
{
    // a(t + dt) = -(1/m) f( r(t+dt) )
    
    // v(t + dt) = v(t + dt/2) + (1/2) a(t + dt) dt
}

/** Integrate the coordinates of the molecule 'molecule' and half-integrate
    the velocities */
PartialMolecule 
VelocityVerlet::integrateCoordinates(const PartialMolecule &molecule, 
                                     const PropertyName &coords_property, 
                                     const PropertyName &masses_property,
                                     const MolForceTable &mol_force, 
                                     MolForceTable &mol_vel) const
{
    // a(t) = -(1/m) f( r(t) )

    // r(t + dt) = r(t) + v(t) dt + (1/2) a(t) dt^2
    
    // v(t + dt/2) = v(t) + (1/2) a(t) dt
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
    //update the integrator - this calculates the force
    //on the molecules, and assigns initial velocities if necessary
    this->updateFrom(system);
    
    //loop over all of the molecules to update the coordinates
    int nmols = f.count();
    
    BOOST_ASSERT( nmols == a.count() );

    //// update the coordinates
    const MolForceTable *mol_forces = f.constData();
    MolForceTable *mol_vels = v.data();

    Molecules new_molecules;

    for (int i=0; i<nmols; ++i)
    {
        const MolForceTable &mol_force = mol_forces[i];
        MolForceTable &mol_vel = mol_vels[i];
        
        BOOST_ASSERT( mol_force.molNum() == mol_vel.molNum() );
        
        //get the molecule
        PartialMolecule molecule = molgroup.read().molecule(mol_force.molNum());
        
        //get the coordinates and masses
        AtomicCoords coords = molecule.property( coords_property );
        AtomicMasses masses = molecule.property( masses_property );
        
        //get r(t+dt) from v(t) and f(t)
        // (this also gets v(t+0.5dt) from v(t) and f(t)
        molecule = this->integrateCoordinates(molecule, coords_property, 
                                                        masses_property,
                                                        mol_force, mol_vel);

        //add this molecule to the list of updated molecules
        new_molecules += molecule;
    }
    
    //now update the system
    system.update(new_molecules);
    
    //now update this again from the system - this recalculates the forces
    this->updateFrom(system);
    
    //we now need to half-integrate the velocities...
    mol_forces = f.constData();
    mol_vels = v.data();
    
    BOOST_ASSERT( nmols == f.count() );
    BOOST_ASSERT( nmols == v.count() );
    
    for (int i=0; i<nmols; ++i)
    {
        const MolForceTable &mol_force = mol_forces[i];
        MolForceTable &mol_vel = mol_vels[i];
        
        BOOST_ASSERT( mol_force.molNum() == mol_vel.molNum() );

        //get the molecule
        PartialMolecule molecule = molgroup.read().molecule(mol_force.molNum());
        
        //get the masses
        AtomicMasses masses = molecule.property( masses_property );

        this->integrateVelocities(molecule, mol_vel, masses, mol_force);
    }
}

/** Set the timestep for integration */
void VelocityVerlet::setTimeStep(const Time &new_timestep)
{
    timestep = new_timestep;
}

/** Return the timestep of integration */
Time VelocityVerlet::timeStep() const
{
    return timestep;
}

/** Return the kinetic energy at the last timestep */
MolarEnergy VelocityVerlet::kineticEnergy() const
{
    return 0*MolarEnergy();
}

/** Clear all statistics */
void VelocityVerlet::clearStatistics()
{}

/** Set the random number generator */
void VelocityVerlet::setGenerator(const RanGenerator&)
{}
