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
#include "SireMol/atommasses.h"
#include "SireMol/atomcoords.h"

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

/** Internal function used to regenerate all of the velocities */
void VelocityVerlet::recalculateVelocities()
{
}

/** Internal function used to redimension the force and velocity
    tables to match the molecules in 'new_molgroup' */
void VelocityVerlet::updateTables(const MoleculeGroup &new_molgroup)
{
}

/** Internal function used to update this integrator from the 
    passed system - this involves calculating the forces if
    the molecule group or system has changed */
void VelocityVerlet::updateFrom(System &system, const Symbol &nrg_component)
{
    if (system.UID() == last_sys_uid and 
        system.version() == last_sys_version and
        nrg_component == last_nrg_component)
    {
        //we are at the same version, and using the same Hamiltonian
        return;
    }
    
    //the system has changed, but has the molecule group?
    const MoleculeGroup &molgroup = Integrator::moleculeGroup();
    const MoleculeGroup &new_molgroup = system[ molgroup.number() ];
    
    if (new_molgroup.version() != molgroup.version())
    {
        if (new_molgroup.version().majorVersion() != molgroup.version().majorVersion())
        {
            //the molecules in the group have changed
            this->updateTables(new_molgroup);
        }
        
        Integrator::setMoleculeGroup(new_molgroup);
    }
    else if (recalc_velocities)
    {
        this->recalculateVelocities();
    }
    
    //now recalculate the forces
    system.force(f, nrg_component);
    
    //save the ID of the system
    last_sys_uid = system.UID();
    last_sys_version = system.version();
    last_nrg_component = nrg_component;
}

/** Half-integrate the velocities */
static void integrateVelocities(const PartialMolecule &molecule,
                                const PropertyName &masses_property,
                                MolForceTable &mol_vels, 
                                const MolForceTable &mol_forces,
                                const Time &timestep)
{
    const AtomMasses &masses = molecule.property( masses_property )
                                       .asA<AtomMasses>();

    // a(t + dt) = -(1/m) f( r(t+dt) )
    
    // v(t + dt) = v(t + dt/2) + (1/2) a(t + dt) dt
    
    //also calculate the kinetic energy as we finish integrating
    //the velocities - do it here as then we don't need to save
    //the masses of the atoms
}

/** Integrate the coordinates of the molecule 'molecule' and half-integrate
    the velocities */
static PartialMolecule integrateCoordinates(PartialMolecule molecule, 
                                            const PropertyName &coords_property, 
                                            const PropertyName &masses_property,
                                            const MolForceTable &mol_force, 
                                            MolForceTable &mol_vel,
                                            const Time &timestep)
{
    //get the coordinates and masses
    AtomCoords coords = molecule.property( coords_property )
                                .asA<AtomCoords>();
    
    const AtomMasses &masses = molecule.property( masses_property )
                                       .asA<AtomMasses>();

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
    //update the integrator - this calculates the force
    //on the molecules, and assigns initial velocities if necessary
    this->updateFrom(system, nrg_component);
    
    //loop over all of the molecules to update the coordinates
    int nmols = f.count();
    
    BOOST_ASSERT( nmols == v.count() );

    PropertyName coords_property = map["coordinates"];
    PropertyName masses_property = map["mass"];

    //// confine temp variables to a local scope
    {
        //// update the coordinates
        const MolForceTable *mol_forces = f.constData();
        MolForceTable *mol_vels = v.data();

        Molecules new_molecules;

        const MoleculeGroup &molgroup = Integrator::moleculeGroup();

        for (int i=0; i<nmols; ++i)
        {
            const MolForceTable &mol_force = mol_forces[i];
            MolForceTable &mol_vel = mol_vels[i];
            
            BOOST_ASSERT( mol_force.molNum() == mol_vel.molNum() );
        
            //get the molecule
            PartialMolecule molecule = molgroup.molecule(mol_force.molNum());
        
            //get r(t+dt) from v(t) and f(t)
            // (this also gets v(t+0.5dt) from v(t) and f(t)
            molecule = ::integrateCoordinates(molecule, coords_property, 
                                              masses_property,
                                              mol_force, mol_vel, timestep);
    
            //add this molecule to the list of updated molecules
            new_molecules += molecule;
        }
    
        //now update the system
        system.update(new_molecules);
    }
    
    //now update this again from the system - this recalculates the forces
    this->updateFrom(system, nrg_component);
    
    //we now need to half-integrate the velocities...
    {
        const MolForceTable *mol_forces = f.constData();
        MolForceTable *mol_vels = v.data();
    
        BOOST_ASSERT( nmols == f.count() );
        BOOST_ASSERT( nmols == v.count() );
    
        const MoleculeGroup &molgroup = Integrator::moleculeGroup();
    
        for (int i=0; i<nmols; ++i)
        {
            const MolForceTable &mol_force = mol_forces[i];
            MolForceTable &mol_vel = mol_vels[i];
        
            BOOST_ASSERT( mol_force.molNum() == mol_vel.molNum() );

            //get the molecule
            PartialMolecule molecule = molgroup.molecule(mol_force.molNum());

            ::integrateVelocities(molecule, masses_property, 
                                  mol_vel, mol_force, timestep);
        }
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

/** Clear all of the velocities (they will thus be regenerated
    on the next timestep) */
void VelocityVerlet::clearVelocities()
{
    recalc_velocities = true;
    last_kinetic_nrg = MolarEnergy(0);
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
    #warning Need kineticEnergy(mol);
    return MolarEnergy(0);
}

/** Clear all statistics */
void VelocityVerlet::clearStatistics()
{}

/** Set the random number generator */
void VelocityVerlet::setGenerator(const RanGenerator&)
{}
