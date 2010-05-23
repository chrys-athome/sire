/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#include "rbworkspace.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireMol;
using namespace SireSystem;
using namespace SireMaths;
using namespace SireBase;
using namespace SireUnits::Dimension;
using namespace SireStream;

static const RegisterMetaType<RBWorkspace> r_rbws;

QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const RBWorkspace &rbws)
{
    writeHeader(ds, r_rbws, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const IntegratorWorkspace&>(rbws);
        
    return ds;
}

QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, RBWorkspace &rbws)
{
    VersionID v = readHeader(ds, r_rbws);
    
    if (v == 1)
    {
        RBWorkspace rb;
    
        SharedDataStream sds(ds);
    
        sds >> static_cast<IntegratorWorkspace&>(rb);
        
        rb.rebuildFromScratch();
        
        rbws = rb;
    }
    else
        throw version_error(v, "1", r_rbws, CODELOC);
        
    return ds;
}

/** Rebuild all of the data array from the current state of the system */
void RBWorkspace::rebuildFromScratch()
{}

/** Construct an empty workspace */
RBWorkspace::RBWorkspace(const PropertyMap &map)
            : ConcreteProperty<RBWorkspace,IntegratorWorkspace>(map)
{}

/** Construct a workspace for the passed molecule group */
RBWorkspace::RBWorkspace(const MoleculeGroup &molgroup, const PropertyMap &map)
            : ConcreteProperty<RBWorkspace,IntegratorWorkspace>(molgroup, map)
{
    this->rebuildFromScratch();
}

/** Copy constructor */
RBWorkspace::RBWorkspace(const RBWorkspace &other)
            : ConcreteProperty<RBWorkspace,IntegratorWorkspace>(other),
              bead_coordinates(other.bead_coordinates),
              bead_orientations(other.bead_orientations),
              bead_linear_momenta(other.bead_linear_momenta),
              bead_angular_momenta(other.bead_angular_momenta),
              bead_forces(other.bead_forces),
              bead_torques(other.bead_torques),
              bead_masses(other.bead_masses),
              bead_inertia(other.bead_inertia),
              vel_generator(other.vel_generator)
{}

/** Destructor */
RBWorkspace::~RBWorkspace()
{}

/** Copy assignment operator */
RBWorkspace& RBWorkspace::operator=(const RBWorkspace &other)
{
    if (this != &other)
    {
        IntegratorWorkspace::operator=(other);
        
        bead_coordinates = other.bead_coordinates;
        bead_orientations = other.bead_orientations;
        bead_linear_momenta = other.bead_linear_momenta;
        bead_angular_momenta = other.bead_angular_momenta;
        bead_forces = other.bead_forces;
        bead_torques = other.bead_torques;
        bead_masses = other.bead_masses;
        bead_inertia = other.bead_inertia;
        vel_generator = other.vel_generator;
    }
    
    return *this;
}

/** Comparison operator */
bool RBWorkspace::operator==(const RBWorkspace &other) const
{
    return this == &other or 
           (IntegratorWorkspace::operator==(other) and
            bead_coordinates == other.bead_coordinates and
            bead_orientations == other.bead_orientations and
            bead_linear_momenta == other.bead_linear_momenta and
            bead_angular_momenta == other.bead_angular_momenta and
            bead_masses == other.bead_masses and
            bead_inertia == other.bead_inertia and
            vel_generator == other.vel_generator);
            
    //don't need forces or torques as these are implied by the
    //forcetable in IntegratorWorkspace
}

/** Comparison operator */
bool RBWorkspace::operator!=(const RBWorkspace &other) const
{
    return not RBWorkspace::operator==(other);
}

const char* RBWorkspace::typeName()
{
    return QMetaType::typeName( qMetaTypeId<RBWorkspace>() );
}

/** Return the kinetic energy of all of the molecules being integrated */
MolarEnergy RBWorkspace::kineticEnergy() const
{
    throw SireError::incomplete_code( QObject::tr("Need to write!"), CODELOC );
    return MolarEnergy(0);
}

/** Return the kinetic energy of the molecule with number 'molnum'

    \throw SireMol::missing_molecule
*/
MolarEnergy RBWorkspace::kineticEnergy(MolNum molnum) const
{
    throw SireError::incomplete_code( QObject::tr("Need to write!"), CODELOC );
    return MolarEnergy(0);
}

/** Return the kinetic energy of the atoms in the view 'molview'

    \throw SireMol::missing_molecule
*/
MolarEnergy RBWorkspace::kineticEnergy(const MoleculeView &molview) const
{
    throw SireError::incomplete_code( QObject::tr("Need to write!"), CODELOC );
    return MolarEnergy(0);
}

/** Return the number of rigid body beads to be integrated */
int RBWorkspace::nBeads() const
{
    return bead_coordinates.count();
}

/** Return the number of atoms in the ith bead */
int RBWorkspace::nAtoms(int i) const
{
    throw SireError::incomplete_code( QObject::tr("Need to write!"), CODELOC );
    return 0;
}

/** Set the system to be integrated */
bool RBWorkspace::setSystem(const System &system)
{
    if (IntegratorWorkspace::setSystem(system))
    {
        this->rebuildFromScratch();
        return true;
    }
    else
        return false;
}

/** Regenerate all of the linear and angular velocities using the passed generator */
void RBWorkspace::regenerateVelocities(const VelocityGenerator &generator)
{
    throw SireError::incomplete_code( QObject::tr("Need to write!"), CODELOC );
}

/** Commit the coordinates back to the system. This maps the bead coordinates
    and orientations back to atomic coordinates and position and 
    updates the system with these */
void RBWorkspace::commitCoordinates()
{
    throw SireError::incomplete_code( QObject::tr("Need to write!"), CODELOC );
}

/** Commit the linear and angular velocities back to the system. This saves
    the velocities as bead properties */
void RBWorkspace::commitVelocities()
{
    throw SireError::incomplete_code( QObject::tr("Need to write!"), CODELOC );
}

/** Commit both the coordinates and velocities - this performs the 
    equivalent of commitCoordinates() and commitVelocities() in
    a single call */
void RBWorkspace::commitCoordinatesAndVelocities()
{
    throw SireError::incomplete_code( QObject::tr("Need to write!"), CODELOC );
}

/** This internal function is called whenever a property is changed. 
    This is used to see if the data has to be regenerated */
void RBWorkspace::changedProperty(const QString &property)
{
    this->rebuildFromScratch();
}
