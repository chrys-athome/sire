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

#include "integratorworkspace.h"

#include "SireSystem/system.h"

#include "SireMol/moleculeview.h"

#include "SireBase/quickcopy.hpp"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireMol/errors.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireMol;
using namespace SireCAS;
using namespace SireBase;
using namespace SireStream;
using namespace SireUnits::Dimension;

//////////
////////// Implementation of IntegratorWorkspace
//////////

static const RegisterMetaType<IntegratorWorkspace> r_intws(MAGIC_ONLY,
                                                    "SireMove::IntegratorWorkspace");

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const IntegratorWorkspace &intws)
{
    writeHeader(ds, r_intws, 1);
    
    SharedDataStream sds(ds);
    
    sds << intws.molgroup << intws.molforces
        << static_cast<const Property&>(intws);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, IntegratorWorkspace &intws)
{
    VersionID v = readHeader(ds, r_intws);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> intws.molgroup >> intws.molforces
            >> static_cast<Property&>(intws);
            
        intws.last_nrg_component = Symbol();
        intws.last_system_uid = QUuid();
        intws.last_system_version = Version();
    }
    else
        throw version_error( v, "1", r_intws, CODELOC );

    return ds;
}

/** Constructor */
IntegratorWorkspace::IntegratorWorkspace() : Property()
{}

/** Construct to hold the variables used to integrate the molecules in 'molgroup' */
IntegratorWorkspace::IntegratorWorkspace(const MoleculeGroup &molecule_group)
                    : Property(), molgroup(molecule_group)
{}

/** Copy constructor */
IntegratorWorkspace::IntegratorWorkspace(const IntegratorWorkspace &other)
                    : Property(other), molgroup(other.molgroup),
                      molforces(other.molforces),
                      last_nrg_component(other.last_nrg_component),
                      last_system_uid(other.last_system_uid),
                      last_system_version(other.last_system_version)
{}

/** Destructor */
IntegratorWorkspace::~IntegratorWorkspace()
{}

/** Copy assignment operator */
IntegratorWorkspace& IntegratorWorkspace::operator=(const IntegratorWorkspace &other)
{
    if (this != &other)
    {
        Property::operator=(other);
        molgroup = other.molgroup;
        molforces = other.molforces;
        last_nrg_component = other.last_nrg_component;
        last_system_uid = other.last_system_uid;
        last_system_version = other.last_system_version;
    }
    
    return *this;
}

/** Comparison operator */
bool IntegratorWorkspace::operator==(const IntegratorWorkspace &other) const
{
    return (this == &other) or
           (molgroup == other.molgroup and 
            last_nrg_component == other.last_nrg_component and
            last_system_uid == other.last_system_uid and
            last_system_version == other.last_system_version and
            molforces == other.molforces);
}

/** Comparison operator */
bool IntegratorWorkspace::operator!=(const IntegratorWorkspace &other) const
{
    return not this->operator==(other);
}

/** Update this workspace using the passed system, calculating forces using
    the Hamiltonian described by the passed energy component */
void IntegratorWorkspace::updateFrom(System &system, const Symbol &nrg_component)
{
    if (last_system_uid == system.UID() and
        last_system_version == system.version() and
        last_nrg_component == nrg_component)
    {
        //nothing has changed - no need to update anything
        return;
    }
    
    MolGroupPtr new_molgroup = system.at( molgroup.read().number() );
    
    ForceTable new_forces = molforces;
    
    if (new_molgroup.read().version().majorVersion() 
                        != molgroup.read().version().majorVersion())
    {
        //the molecules in the group may have changed - get a new forcetable
        new_forces = ForceTable(new_molgroup);
    }
    
    system.force(new_forces, nrg_component);

    molforces = new_forces;
    molgroup = new_molgroup;
    
    last_system_uid = system.UID();
    last_system_version = system.version();
    last_nrg_component = nrg_component;
}

/** Return the molecule group being integrated */
const MoleculeGroup& IntegratorWorkspace::moleculeGroup() const
{
    return molgroup.read();
}

/** Return the current forces on the molecules in the molecule
    group being integrated */
const ForceTable& IntegratorWorkspace::forceTable() const
{
    return molforces;
}

Q_GLOBAL_STATIC( NullIntegratorWorkspace, nullIntegratorWorkspace );

/** Return the global null workspace */
NullIntegratorWorkspace& IntegratorWorkspace::null()
{
    return *(nullIntegratorWorkspace());
}

//////////
////////// Implementation of NullIntegratorWorkspace
//////////

static const RegisterMetaType<NullIntegratorWorkspace> r_nullintws;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const NullIntegratorWorkspace &nullintws)
{
    writeHeader(ds, r_nullintws, 1);
    
    ds << static_cast<const IntegratorWorkspace&>(nullintws);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, 
                                        NullIntegratorWorkspace &nullintws)
{
    VersionID v = readHeader(ds, r_nullintws);
    
    if (v == 1)
    {
        ds >> static_cast<IntegratorWorkspace&>(nullintws);
    }
    else
        throw version_error( v, "1", r_nullintws, CODELOC );
        
    return ds;
}

/** Constructor */
NullIntegratorWorkspace::NullIntegratorWorkspace()
                        : ConcreteProperty<NullIntegratorWorkspace,IntegratorWorkspace>()
{}

/** Copy constructor */
NullIntegratorWorkspace::NullIntegratorWorkspace(const NullIntegratorWorkspace &other)
          : ConcreteProperty<NullIntegratorWorkspace,IntegratorWorkspace>(other)
{}

/** Destructor */
NullIntegratorWorkspace::~NullIntegratorWorkspace()
{}

/** Assignment operator */
NullIntegratorWorkspace& NullIntegratorWorkspace::operator=(
                                            const NullIntegratorWorkspace &other)
{
    IntegratorWorkspace::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullIntegratorWorkspace::operator==(const NullIntegratorWorkspace&) const
{
    return true;
}

/** Comparison operator */
bool NullIntegratorWorkspace::operator!=(const NullIntegratorWorkspace &) const
{
    return false;
}

/** Zero kinetic energy */
MolarEnergy NullIntegratorWorkspace::kineticEnergy() const
{
    return MolarEnergy(0);
}

/** This contains no molecules 

    \throw SireMol::missing_molecule
*/
MolarEnergy NullIntegratorWorkspace::kineticEnergy(const MoleculeView &molview) const
{
    throw SireMol::missing_molecule( QObject::tr(
        "The null integrator workspace does not contain any molecules, so it "
        "definitely does not contain %1.")
            .arg(molview.toString()), CODELOC );
            
    return MolarEnergy(0);
}


//////////
////////// Implementation of AtomicVelocityWorkspace
//////////

static const RegisterMetaType<AtomicVelocityWorkspace> r_avws;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const AtomicVelocityWorkspace &avws)
{
    writeHeader(ds, r_avws, 1);
    
    SharedDataStream sds(ds);
    
    sds << avws.forces_array << avws.coords_array << avws.vels_array
        << avws.inv_masses_array << avws.propnames
        << static_cast<const IntegratorWorkspace&>(avws);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, AtomicVelocityWorkspace &avws)
{
    VersionID v = readHeader(ds, r_avws);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> avws.forces_array >> avws.coords_array >> avws.vels_array
            >> avws.inv_masses_array >> avws.propnames
            >> static_cast<IntegratorWorkspace&>(avws);
    }
    else
        throw version_error( v, "1", r_avws, CODELOC );
        
    return ds;
}

/** Constructor */
AtomicVelocityWorkspace::AtomicVelocityWorkspace()
                        : ConcreteProperty<AtomicVelocityWorkspace,IntegratorWorkspace>()
{}

/** Construct to hold the variables used to integrate the molecule group 'molgroup' */
AtomicVelocityWorkspace::AtomicVelocityWorkspace(const MoleculeGroup &molgroup)
         : ConcreteProperty<AtomicVelocityWorkspace,IntegratorWorkspace>(molgroup)
{}

/** Copy constructor */
AtomicVelocityWorkspace::AtomicVelocityWorkspace(const AtomicVelocityWorkspace &other)
         : ConcreteProperty<AtomicVelocityWorkspace,IntegratorWorkspace>(other),
           forces_array(other.forces_array),
           coords_array(other.coords_array),
           vels_array(other.vels_array),
           inv_masses_array(other.inv_masses_array),
           propnames(other.propnames)
{}

/** Destructor */
AtomicVelocityWorkspace::~AtomicVelocityWorkspace()
{}

/** Copy assignment operator */
AtomicVelocityWorkspace& AtomicVelocityWorkspace::operator=(
                                                const AtomicVelocityWorkspace &other)
{
    if (this != &other)
    {
        IntegratorWorkspace::operator=(other);
        
        forces_array = other.forces_array;
        coords_array = other.coords_array;
        vels_array = other.vels_array;
        inv_masses_array = other.inv_masses_array;
        propnames = other.propnames;
    }
    
    return *this;
}

/** Comparison operator */
bool AtomicVelocityWorkspace::operator==(const AtomicVelocityWorkspace &other) const
{
    return (this == &other) or
           (forces_array == other.forces_array and 
            coords_array == other.coords_array and
            vels_array == other.vels_array and
            inv_masses_array == other.inv_masses_array and
            propnames == other.propnames and
            IntegratorWorkspace::operator==(other) );
}

/** Comparison operator */
bool AtomicVelocityWorkspace::operator!=(const AtomicVelocityWorkspace &other) const
{
    return not this->operator==(other);
}

/** Return the total kinetic energy of the system when it was last integrated */
MolarEnergy AtomicVelocityWorkspace::kineticEnergy() const
{
    double nrg = 0;
    
    const int nmols = vels_array.count();
    BOOST_ASSERT( inv_masses_array.count() == nmols );
    
    const QVector<Vector> *molvels = vels_array.constData();
    const QVector<double> *molmasses = inv_masses_array.constData();
    
    for (int i=0; i<nmols; ++i)
    {
        const int nats = molvels[i].count();
        BOOST_ASSERT( molmasses[i].count() == nats );
    
        const Vector *vels = molvels[i].constData();
        const double *inv_mass = molmasses[i].constData();
    
        for (int i=0; i<nats; ++i)
        {
            nrg += (0.5 / inv_mass[i]) * vels[i].length2();
        }
    }
    
    return MolarEnergy(nrg);
}

/** Return the kinetic energy of the atoms in the view of the molecule in 
    'molview' */
MolarEnergy AtomicVelocityWorkspace::kineticEnergy(const MoleculeView &molview) const
{
    return MolarEnergy(0);
}

/** Return all of the forces on all of the molecules in this workspace */
QHash<MolNum,AtomForces> AtomicVelocityWorkspace::forces() const
{
    return QHash<MolNum,AtomForces>();
}

/** Return all of the velocities of all of the molecules in this workspace */
QHash<MolNum,AtomVelocities> AtomicVelocityWorkspace::velocities() const
{
    return QHash<MolNum,AtomVelocities>();
}

/** Return all of the masses of the molecules in this workspace */
QHash<MolNum,AtomMasses> AtomicVelocityWorkspace::masses() const
{
    return QHash<MolNum,AtomMasses>();
}

/** Return the forces on the atoms in the molecule matching the ID 'molid'

    \throw SireMol::missing_molecule
    \throw SireMol::duplicate_molecule
    \throw SireError::invalid_index
*/
AtomForces AtomicVelocityWorkspace::forces(const MolID &molid) const
{
    return AtomForces();
}

/** Return the velocities of the atoms in the molecule matching the ID 'molid'

    \throw SireMol::missing_molecule
    \throw SireMol::duplicate_molecule
    \throw SireError::invalid_index
*/
AtomVelocities AtomicVelocityWorkspace::velocities(const MolID &molid) const
{
    return AtomVelocities();
}

/** Return the masses of the atoms in the molecule matching the ID 'molid'

    \throw SireMol::missing_molecule
    \throw SireMol::duplicate_molecule
    \throw SireError::invalid_index
*/
AtomMasses AtomicVelocityWorkspace::masses(const MolID &molid) const
{
    return AtomMasses();
}

/** Return the number of molecules in this workspace */
int AtomicVelocityWorkspace::nMolecules() const
{
    return forces_array.count();
}

/** Return the number of molecules in this workspace */
int AtomicVelocityWorkspace::count() const
{
    return this->nMolecules();
}

/** Return the number of molecules in this workspace */
int AtomicVelocityWorkspace::size() const
{
    return this->nMolecules();
}

void AtomicVelocityWorkspace::assertValidIndex(const int i) const
{
    if (i < 0 or i >= forces_array.count())
        throw SireError::invalid_index( QObject::tr(
            "Invalid index (%1). Number of molecules == %2.")
                .arg(i).arg(forces_array.count()), CODELOC );
}

/** Return the array of forces on the ith molecule in the workspace 

    \throw SireError::invalid_index
*/
const QVector<Vector>& AtomicVelocityWorkspace::forceArray(int i) const
{
    assertValidIndex(i);
    return forces_array.constData()[i];
}

/** Return the array of coordinates on the ith molecule in the workspace

    \throw SireError::invalid_index
*/
const QVector<Vector>& AtomicVelocityWorkspace::coordinateArray(int i) const
{
    assertValidIndex(i);
    return coords_array.constData()[i];
}

/** Return the array of velocities on the ith molecule in the workspace 

    \throw SireError::invalid_index
*/
const QVector<Vector>& AtomicVelocityWorkspace::velocityArray(int i) const
{
    assertValidIndex(i);
    return vels_array.constData()[i];
}

/** Return the array of reciprocal masses on the ith molecule in the workspace 

    \throw SireError::invalid_index
*/
const QVector<double>& AtomicVelocityWorkspace::reciprocalMassArray(int i) const
{
    assertValidIndex(i);
    return inv_masses_array.constData()[i];
}

/** Set the coordinates of the ith molecule to 'new_coords'. The number
    of coordinates must be the same as the current number of coordinates
    
    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void AtomicVelocityWorkspace::setCoordinates(int i, const QVector<Vector> &new_coords)
{
    if (this->coordinateArray(i).count() != new_coords.count())
        throw SireError::incompatible_error( QObject::tr(
            "Cannot set the coordinates of molecule %1 as the number "
            "of coordinates (%2) is different (%3).")
                .arg(i).arg(this->coordinateArray(i).count()).arg(new_coords.count()),
                    CODELOC );
                    
    coords_array.data()[i] = new_coords;
}

/** Set the velocities of the ith molecule to 'new_velocities'. The number
    of velocities must be the same as the current number of velocities
    
    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void AtomicVelocityWorkspace::setVelocities(int i, const QVector<Vector> &new_velocities)
{
    if (this->velocityArray(i).count() != new_velocities.count())
        throw SireError::incompatible_error( QObject::tr(
            "Cannot set the velocities of molecule %1 as the number "
            "of velocities (%2) is different (%3).")
                .arg(i).arg(this->velocityArray(i).count()).arg(new_velocities.count()),
                    CODELOC );
                    
    vels_array.data()[i] = new_velocities;
}


void AtomicVelocityWorkspace::updateFrom(System &system, const Symbol &nrg_component,
                                         const VelocityGenerator &velgen,
                                         const PropertyMap &map)
{}

void AtomicVelocityWorkspace::updateSystem(System &system, const Symbol &nrg_component)
{}
