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

#include "SireMol/atomelements.h"
#include "SireMol/atommasses.h"
#include "SireMol/atomcoords.h"

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

static QVector<MolarMass> getMasses(const QVector<Element> &elements)
{
    int sz = elements.count();
    QVector<MolarMass> masses(sz);
    masses.squeeze();
    
    const Element *elements_array = elements.constData();
    MolarMass *masses_array = masses.data();
    
    for (int i=0; i<sz; ++i)
    {
        masses_array[i] = elements_array[i].mass();
    }
    
    return masses;
}

static QVector<Vector> getCOMPlusInertia(const QVector<Vector> &coords,
                                         const QVector<MolarMass> &masses,
                                         Vector &com, double &mass, 
                                         Vector &principle_inertia,
                                         Matrix &orientation)
{
    int nats = coords.count();
    BOOST_ASSERT( masses.count() == nats );
    
    const Vector *coords_array = coords.constData();
    const MolarMass *masses_array = masses.constData();
    
    // Calculate the center of mass
    com = Vector(0);
    mass = 0;
    
    for (int i=0; i<nats; ++i)
    {
        com += coords_array[i] * masses_array[i].value();
        mass += masses_array[i].value();
    }
    
    com /= mass;
    
    // Now calculate the moment of inertia tensor in the cartesian frame
    // (calculate from the center of mass)
    Matrix inertia(0);
    double *inertia_array = inertia.data();
    
    for (int i=0; i<nats; ++i)
    {
        Vector d = coords_array[i] - com;
        
        double m = masses_array[i].value();
        
        inertia_array[ inertia.offset(0,0) ] += m * (d.x()*d.x() + d.z()*d.z());
        inertia_array[ inertia.offset(1,1) ] += m * (d.x()*d.x() + d.y()*d.y());
        inertia_array[ inertia.offset(2,2) ] += m * (d.y()*d.y() + d.z()*d.z());
        
        inertia_array[ inertia.offset(0,1) ] -= m * d.x() * d.y();
        inertia_array[ inertia.offset(0,2) ] -= m * d.x() * d.z();
        inertia_array[ inertia.offset(1,2) ] -= m * d.y() * d.z();
    }
    
    inertia_array[ inertia.offset(1,0) ] = inertia_array[ inertia.offset(0,1) ];
    inertia_array[ inertia.offset(2,0) ] = inertia_array[ inertia.offset(0,2) ];
    inertia_array[ inertia.offset(2,1) ] = inertia_array[ inertia.offset(1,2) ];

    std::pair<Vector,Matrix> eigs = inertia.diagonalise();

    principle_inertia = eigs.first;
    orientation = eigs.second;
    
    Matrix inv = orientation.inverse();
    
    //now calculate the coordinates of all of the atoms in terms
    //of the center of mass / orientaton frame
    QVector<Vector> internal_coords(coords);
    Vector *internal_coords_array = internal_coords.data();

    for (int i=0; i<nats; ++i)
    {
        internal_coords_array[i] = inv * (coords_array[i] - com);
    }

    return internal_coords;
}

/** Rebuild all of the data array from the current state of the system */
void RBWorkspace::rebuildFromScratch()
{
    const System &sys = this->system();
    
    PropertyName coords_property = this->coordinatesProperty();
    PropertyName mass_property = this->massesProperty();
    PropertyName element_property = this->elementsProperty();
    PropertyName velgen_property = this->velocityGeneratorProperty();
    
    const MoleculeGroup &molgroup = this->moleculeGroup();
    const ForceTable &forcetable = this->forceTable();
    
    int nmols = molgroup.nMolecules();
    
    //for the moment, we'll make one bead per molecule
    int nbeads = nmols;
    
    if (sys.containsProperty(velgen_property))
        vel_generator = sys.property(velgen_property).asA<VelocityGenerator>();
    else
        vel_generator = NullVelocityGenerator();

    int ibead = 0;
    
    atom_coordinates = QVector< QVector<Vector> >(nbeads);
    bead_coordinates = QVector<Vector>(nbeads);
    bead_orientations = QVector<Matrix>(nbeads);
    bead_masses = QVector<double>(nbeads);
    bead_inertia = QVector<Vector>(nbeads);
    
    atom_coordinates.squeeze();
    bead_coordinates.squeeze();
    bead_orientations.squeeze();
    bead_masses.squeeze();
    bead_inertia.squeeze();
    
    QVector<Vector> *atom_coords_array = atom_coordinates.data();
    Vector *bead_coords_array = bead_coordinates.data();
    Matrix *bead_orients_array = bead_orientations.data();
    double *bead_masses_array = bead_masses.data();
    Vector *bead_inertia_array = bead_inertia.data();
    
    for (int i=0; i<nmols; ++i)
    {
        MolNum molnum = molgroup.molNumAt(i);
        const ViewsOfMol &mol = molgroup[molnum].data();
        
        const MoleculeData &moldata = mol.data();
        
        QVector<Vector> coords;
        QVector<MolarMass> masses;
        
        if (mol.selectedAll())
        {
            coords = moldata.property(coords_property)
                            .asA<AtomCoords>().toVector();
            
            if (moldata.hasProperty(mass_property))
            {
                masses = moldata.property(mass_property)
                                .asA<AtomMasses>().toVector();
            }
            else
            {
                masses = ::getMasses( moldata.property(element_property)
                                             .asA<AtomElements>().toVector() );
            }
        }
        else
        {
            AtomSelection selected_atoms = mol.selection();
            
            coords = moldata.property(coords_property)
                            .asA<AtomCoords>().toVector(selected_atoms);
            
            if (moldata.hasProperty(mass_property))
            {
                masses = moldata.property(mass_property)
                                .asA<AtomMasses>()
                                .toVector(selected_atoms);
            }
            else
            {
                masses = ::getMasses( moldata.property(element_property)
                                             .asA<AtomElements>()
                                             .toVector(selected_atoms) );
            }
        }
        
        atom_coords_array[ibead] = ::getCOMPlusInertia(coords, masses,
                                       bead_coords_array[ibead],
                                       bead_masses_array[ibead],
                                       bead_inertia_array[ibead],
                                       bead_orients_array[ibead]);
    
        ++ibead;
    }
    
    //for now, set initial momenta to 0
    
}

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
