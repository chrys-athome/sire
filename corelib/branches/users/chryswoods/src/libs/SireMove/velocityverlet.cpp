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

/** Half-integrate the velocities - this returns the new kinetic energy
    of the atoms in the partial molecule */
static double integrateVelocities(const PartialMolecule &molecule,
                                  const PropertyName &masses_property,
                                  MolForceTable &mol_vels, 
                                  const MolForceTable &mol_forces,
                                  const Time &timestep)
{
    const double dt = 0.5 * timestep;

    if (molecule.selection().selectedAll())
    {
        const AtomMasses &masses = molecule.property( masses_property )
                                        .asA<AtomMasses>();

        int natoms = masses.nAtoms();
        
        BOOST_ASSERT( mol_vels.nValues() == natoms );
        BOOST_ASSERT( mol_forces.nValues() == natoms );
        
        const Mass *masses_array = masses.array().constValueData();
        const Vector *forces_array = mol_forces.constValueData();
        
        Vector *vels_array = mol_vels.valueData();
        
        double kinetic_nrg = 0;
        
        for (int i=0; i<natoms; ++i)
        {
            // a(t + dt) = -(1/m) f( r(t+dt) )
            const double mass = masses_array[i];
            
            const Vector a_t = (dt / mass) * forces_array[i];
    
            // v(t + dt) = v(t + dt/2) + (1/2) a(t + dt) dt
            vels_array[i] -= a_t;
    
            //also calculate the kinetic energy as we finish integrating
            //the velocities - do it here as then we don't need to save
            //the masses of the atoms
            kinetic_nrg += mass * vels_array[i].length2();
        }
        
        return 0.5 * kinetic_nrg;
    }
    else
        throw SireError::incomplete_code( QObject::tr(
            "The code to perform dynamics on parts of a molecule has yet "
            "to be written."), CODELOC );
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
    const double dt = timestep;
    const double half_dt = 0.5*dt;

    const PropertyName coords_property = map["coordinates"];
    const PropertyName masses_property = map["mass"];

    if (molecule.selection().selectedAll())
    {
        Molecule new_molecule = molecule.molecule().move().toCartesian(space, map);

        //get the coordinates and masses
        AtomCoords coords = molecule.property( coords_property )
                                    .asA<AtomCoords>();
    
        const AtomMasses &masses = molecule.property( masses_property )
                                           .asA<AtomMasses>();

        int ncg = coords.nCutGroups();
        
        QVector<CoordGroup> new_coords(ncg);
        
        for (CGIdx i(0); i<ncg; ++i)
        {
            int natoms = molecule.nAtoms();
        
            BOOST_ASSERT( coords.nAtoms(i) == natoms );
            BOOST_ASSERT( masses.nAtoms(i) == natoms );
            BOOST_ASSERT( mol_force.nValues(i) == natoms );
            BOOST_ASSERT( mol_vel.nValues(i) == natoms );

            const Mass *masses_array = masses.constData(i);
            const Vector *forces_array = mol_force.constData(i);
            Vector *vels_array = mol_vel.data(i);

            CoordGroupEditor coord_editor = coords.at(i).edit();
            Vector *coords_array = coord_editor.data();
        
            for (int j=0; j<natoms; ++j)
            {
                // a(t) = -(1/m) f( r(t) )
                const Vector a_t = (-half_dt / masses_array[j]) * forces_array[j];

                // r(t + dt) = r(t) + v(t) dt + (1/2) a(t) dt^2
                coords_array[j] += vels_array[j]*dt + a_t*dt;
    
                // v(t + dt/2) = v(t) + (1/2) a(t) dt
                vels_array[j] += a_t;
            }
            
            new_coords[i] = coord_editor.commit();
        }

        coords = AtomCoords(new_coords);

        molecule = molecule.molecule().edit()
                           .setProperty(coords_property.source(), coords)
                           .move().fromCartesian(space, map)
                           .commit();
    }
    else
        throw SireError::incomplete_code( QObject::tr(
            "The code to perform dynamics on parts of a molecule has yet "
            "to be written."), CODELOC );
        
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

    const Space &space = system.property( map["space"] ).asA<Space>();

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
            molecule = ::integrateCoordinates(molecule, mol_force, mol_vel, 
                                              space, timestep, map);
    
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
    
        const PropertyName masses_property = map["mass"];
    
        double kinetic_nrg = 0;
    
        for (int i=0; i<nmols; ++i)
        {
            const MolForceTable &mol_force = mol_forces[i];
            MolForceTable &mol_vel = mol_vels[i];
        
            BOOST_ASSERT( mol_force.molNum() == mol_vel.molNum() );

            //get the molecule
            PartialMolecule molecule = molgroup.molecule(mol_force.molNum());

            kinetic_nrg += ::integrateVelocities(molecule, masses_property, 
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
    MolNum molnum = molview.data().number();

    if (not v.containsTable(molnum))
        return MolarEnergy(0);
    
    const MolForceTable &mol_vels = v.constGetTable(molnum);
    
    if (mol_vels.molUID() != molview.data().info().UID())
        throw SireError::incompatible_error( QObject::tr(
            "Cannot get the velocity of %1 as its structure has changed.")
                .arg( molview.toString() ), CODELOC );
    
    double kinetic_nrg = 0;
    
    return MolarEnergy( 0.5 * kinetic_nrg );
}

/** Clear all statistics */
void VelocityVerlet::clearStatistics()
{}
