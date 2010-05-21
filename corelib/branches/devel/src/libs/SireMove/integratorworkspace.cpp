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
#include "velocitygenerator.h"

#include "SireSystem/system.h"

#include "SireMol/moleculeview.h"
#include "SireMol/molecule.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/moleditor.h"
#include "SireMol/atomcoords.h"

#include "SireBase/quickcopy.hpp"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireMol/errors.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireFF;
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
    the Hamiltonian described by the passed energy component. This returns
    whether or not the system has changed since the last update, and
    also returns (in 'changed_layout') whether the arrangement of
    molecules has changed */
bool IntegratorWorkspace::updateFrom(System &system, const Symbol &nrg_component,
                                     bool *changed_layout)
{
    if (system.subVersion() != 0)
        throw SireError::incompatible_error( QObject::tr(
                "You cannot update an integrator workspace with a partially "
                "updated System (%1) - the subVersion() must be 0.")
                    .arg(system.toString()), CODELOC );


    if (last_system_uid == system.UID() and
        last_system_version == system.version() and
        last_nrg_component == nrg_component)
    {
        //nothing has changed - no need to update anything
        if (changed_layout)
            *changed_layout = false;
            
        return false;
    }
        
    last_system_uid = QUuid();

    MolGroupPtr new_molgroup = system.at( molgroup.read().number() );
    
    if (new_molgroup.read().version().majorVersion() 
                        != molgroup.read().version().majorVersion())
    {
        //the molecules in the group may have changed - get a new forcetable
        molforces = ForceTable(new_molgroup);
        
        if (changed_layout)
            *changed_layout = true;
    }
    else if (changed_layout)
    {
        *changed_layout = false;
    }
    
    molforces.initialiseTables();
    system.force(molforces, nrg_component);

    molgroup = new_molgroup;
    
    last_system_uid = system.UID();
    last_system_version = system.version();
    last_nrg_component = nrg_component;
    
    return true;
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
const NullIntegratorWorkspace& IntegratorWorkspace::null()
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

const char* NullIntegratorWorkspace::typeName()
{
    return QMetaType::typeName( qMetaTypeId<NullIntegratorWorkspace>() );
}

//////////
////////// Implementation of AtomicVelocityWorkspace
//////////

AtomicVelocityWorkspace();
AtomicVelocityWorkspace(const MoleculeGroup &molgroup);
AtomicVelocityWorkspace(const MoleculeGroup &molgroup,
                        const VelocityGenerator &generator);

AtomicVelocityWorkspace(const AtomicVelocityWorkspace &other);

~AtomicVelocityWorkspace();

AtomicVelocityWorkspace& operator=(const AtomicVelocityWorkspace &other);

bool operator==(const AtomicVelocityWorkspace &other) const;
bool operator!=(const AtomicVelocityWorkspace &other) const;

static const char* typeName();

SireUnits::Dimension::MolarEnergy kineticEnergy() const;
SireUnits::Dimension::MolarEnergy kineticEnergy(const MoleculeView &molview) const;

int AtomicVelocityWorkspace::nMolecules() const
{
    return delta_atom_coords.count();
}

int AtomicVelocityWorkspace::nAtoms(int i) const
{
    return delta_atom_coords.constData()[i].count();
}

Vector* AtomicVelocityWorkspace::deltaCoordsArray(int i)
{
    return delta_atom_coords.data()[i].data();
}

Velocity3D* AtomicVelocityWorkspace::velocityArray(int i)
{
    return atom_velocities.data()[i].data();
}

const Vector* AtomicVelocityWorkspace::deltaCoordsArray(int i) const
{
    return delta_atom_coords.constData()[i].constData();
}

const Vector* AtomicVelocityWorkspace::forceArray(int i) const
{
    const QVector<Vector> &forces = atom_forces.constData()[i];

    if (forces.isEmpty())
    {
        //all of the molecule is selected, so we are taking the
        //forces directly from the forcetable
        return forceTable().constData()[i].constValueData();
    }
}

const Velocity3D* AtomicVelocityWorkspace::velocityArray(int i) const
{
    return atom_velocities.constData()[i].constData();
}

const double* AtomicVelocityWorkspace::reciprocalMassArray(int i) const
{
    return inv_atom_masses.constData()[i].constData();
}

const Vector* AtomicVelocityWorkspace::constDeltaCoordsArray(int i) const
{
    return AtomicVelocityWorkspace::deltaCoordsArray(i);
}

const Vector* AtomicVelocityWorkspace::constForceArray(int i) const
{
    return AtomicVelocityWorkspace::forceArray(i);
}

const Vector* AtomicVelocityWorkspace::constVelocityArray(int i) const
{
    return AtomicVelocityWorkspace::velocityArray(i);
}

const double* AtomicVelocityWorkspace::constReciprocalMassArray(int i) const
{
    return AtomicVelocityWorkspace::reciprocalMassArray(i);
}

void AtomicVelocityWorkspace::setSystem(const System &system)
{
    //check if need to set
    
    IntegratorWorkspace::setSystem(system);
    
    //get all of the arrays
    ...
}

void AtomicVelocityWorkspace::commitCoordinates()
{
    System new_system = system();
    
    //update the coordinates
    
    IntegratorWorkspace::setSystem(new_system);
}

void AtomicVelocityWorkspace::commitVelocities();

void AtomicVelocityWorkspace::commitCoordinatesAndVelocities();
