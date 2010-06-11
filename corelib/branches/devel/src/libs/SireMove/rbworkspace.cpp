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

#include "stdio.h"

#include "rbworkspace.h"

#include "SireMol/atomelements.h"
#include "SireMol/atommasses.h"
#include "SireMol/atomcoords.h"
#include "SireMol/molecule.h"
#include "SireMol/moleditor.h"

#include "SireMaths/axisset.h"

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
                                         const QVector<qint32> &bead_idxs,
                                         QVector<Vector> &bead_coms, 
                                         QVector<double> &bead_masses, 
                                         QVector<Vector> &principle_inertias,
                                         QVector<Matrix> &orientations
                                        )
{
    int nats = coords.count();
    
    BOOST_ASSERT( masses.count() == nats );
    BOOST_ASSERT( beadidxs.count() == nats );

    QHash<qint32,Matrix> inertia;
    
    const qint32 *bead_idxs_array = bead_idxs.constData();
    const Vector *coords_array = coords.constData();
    const MolarMass *masses_array = masses.constData();
    
    Vector *bead_coms_array = bead_coms.data();
    double *bead_masses_array = bead_masses.data();
    
    // Calculate the center(s) of mass
    for (int i=0; i<nats; ++i)
    {
        qint32 bead_idx = bead_idxs_array[i];
        
        if (bead_idx == -1)
            continue;
    
        if (not mats.contains(bead_idx))
        {
            inertia.insert( bead_idx, Matrix(0) );
            bead_coms_array[bead_idx] = Vector(0);
            bead_masses_array[bead_idx] = 0;
        }
        
        bead_coms_array[bead_idx] += coords_array[i] * masses_array[i].value();
        bead_masses_array[bead_idx] += masses_array[i].value();
    }

    for (QHash<qint32,Matrix>::const_iterator it = inertia.constBegin();
         it != inertia.constEnd();
         ++it)
    {
        bead_coms_array[it.key()] /= bead_masses_array[it.key()];
    }

    // Now calculate the moment of inertia tensor in the cartesian frame
    // (calculate from the center of mass)
    for (int i=0; i<nats; ++i)
    {
        qint32 bead_idx = bead_idxs_array[i];
        
        if (bead_idx == -1)
            continue;

        double *inertia_array = inertia[bead_idx].data();

        Vector d = coords_array[i] - bead_coms_array[bead_idx];
        
        double m = masses_array[i].value();
        
        inertia_array[ inertia.offset(0,0) ] += m * (d.y()*d.y() + d.z()*d.z());
        inertia_array[ inertia.offset(1,1) ] += m * (d.x()*d.x() + d.z()*d.z());
        inertia_array[ inertia.offset(2,2) ] += m * (d.x()*d.x() + d.y()*d.y());
        
        inertia_array[ inertia.offset(0,1) ] -= m * d.x() * d.y();
        inertia_array[ inertia.offset(0,2) ] -= m * d.x() * d.z();
        inertia_array[ inertia.offset(1,2) ] -= m * d.y() * d.z();
    }

    for (QHash<qint32,Matrix>::iterator it = inertia.begin();
         it != inertia.end();
         ++it)
    {
        double *inertia_array = it->data();

        for (int i=0; i<9; ++i)
        {
            if (inertia_array[i] < 1e-6 and inertia_array[i] > -1e-6)
                inertia_array[i] = 0;
        }
    
        inertia_array[ inertia.offset(1,0) ] = inertia_array[ inertia.offset(0,1) ];
        inertia_array[ inertia.offset(2,0) ] = inertia_array[ inertia.offset(0,2) ];
        inertia_array[ inertia.offset(2,1) ] = inertia_array[ inertia.offset(1,2) ];

        std::pair<Vector,Matrix> eigs = it->diagonalise();

        Vector &principle_inertia = eigs.first;
        Matrix &orientation = eigs.second;
    
        //if one or more of the eigenvalues is zero then we may have a problem
        //because the wrong eigenvector direction may be chosen - in this case,
        //we will build this eigenvector using a cross product to ensure that 
        //the right-hand-rule definition of our axes is maintained
        //
        // Also, even if we have three eigenvalues, we still need to make sure
        // that a right-hand-rule set is chosen, rather than the left-hand set
        {
            bool zero_x = std::abs(principle_inertia[0]) < 1e-6;
            bool zero_y = std::abs(principle_inertia[1]) < 1e-6;
            bool zero_z = std::abs(principle_inertia[2]) < 1e-6;
        
            int n_zeroes = int(zero_x) + int(zero_y) + int(zero_z);
        
            if (n_zeroes == 3)
            {
                //no axes!
                orientation = Matrix(1);
            }
            else if (n_zeroes == 2)
            {
                //just one well-defined axis - I don't know how to handle this...
                throw SireError::incompatible_error( QObject::tr(
                        "Sire cannot yet handle rigid body systems with only "
                        "a single non-zero eigenvalue - %1, moment of interia equals\n%2")
                            .arg(principle_inertia.toString(),
                                 orientation.toString()), CODELOC );
            }
            else if (n_zeroes == 1)
            {
                Vector r0 = orientation.row0();
                Vector r1 = orientation.row1();
                Vector r2 = orientation.row2();
            
                if (zero_x)
                    r0 = Vector::cross(r1,r2);
                else if (zero_y)
                    r1 = Vector::cross(r2,r0);
                else if (zero_z)
                    r2 = Vector::cross(r0,r1);
            
                orientation = Matrix(r0, r1, r2);
            }
            else
            {
                Vector r0 = orientation.row0();
                Vector r1 = orientation.row1();
                     
                orientation = Matrix( r0, r1, Vector::cross(r0,r1) );
            }
        }

        *it = orientation.inverse();
        orientations[bead_idx] = orientation;
        principle_inertias[bead_idx] = principle_inertia;
    }

    //now calculate the coordinates of all of the atoms in terms
    //of the center of mass / orientaton frame
    QVector<Vector> internal_coords(nats);
    Vector *internal_coords_array = internal_coords.data();

    for (int i=0; i<nats; ++i)
    {
        qint32 bead_idx = bead_idxs_array[i];
        
        if (bead_idx == -1)
            continue;
    
        internal_coords_array[i] = inv.value(bead_idx) 
                                        * (coords_array[i] - bead_coms_array[bead_idx]);
    }

    return internal_coords;
}

/** Return the property used to bead up a molecule */
PropertyName RBWorkspace::beadingProperty() const
{
    return propertyMap()["beading"];
}

static void getBeading(const ViewsOfMol &mol, const PropertyName &beading_property,
                       QPair< qint32,QVector<qint32> > &beading, qint32 &nbeads)
{
    beading.first = nbeads;
    beading.second = QVector<qint32>();
    nbeads += 1;
}

/** Rebuild all of the data array from the current state of the system */
void RBWorkspace::rebuildFromScratch()
{
    const System &sys = this->system();
    
    PropertyName coords_property = this->coordinatesProperty();
    PropertyName mass_property = this->massesProperty();
    PropertyName element_property = this->elementsProperty();
    PropertyName velgen_property = this->velocityGeneratorProperty();
    PropertyName beading_property = this->beadingProperty();
    
    const MoleculeGroup &molgroup = this->moleculeGroup();
    
    int nmols = molgroup.nMolecules();
    
    if (sys.containsProperty(velgen_property))
        vel_generator = sys.property(velgen_property).asA<VelocityGenerator>();
    else
        vel_generator = NullVelocityGenerator();
    
    atom_int_coords = QVector< QVector<Vector> >(nmols);
    atoms_to_beads = QVector< QVector<qint32> >(nmols);
    
    atom_int_coords.squeeze();
    atoms_to_beads.squeeze();
    
    QVector<Vector> *atom_int_coords_array = atom_int_coords.data();
    QPair< qint32,QVector<qint32> > *atoms_to_beads_array = atoms_to_beads.data();

    //bead up the molecules
    qint32 nbeads = 0;
    
    for (int i=0; i<nmols; ++i)
    {
        MolNum molnum = molgroup.molNumAt(i);
        
        const ViewsOfMol &mol = molgroup[molnum];
        
        const MoleculeData &moldata = mol.data();
        
        if (moldata.hasProperty(beading_property))
        {
            //use the beading property to generate the beading for this molecule
            ::getBeading(mol, beading_property, atoms_to_beads_array[i], nbeads);
        }
        else
        {
            //the entire molecule is a single bead
            atoms_to_beads_array[i].first = nbeads;
            atoms_to_beads_array[i].second = QVector<qint32>();
            
            ++nbeads;
        }
    }
    
    if (nbeads == 0)
    {
        bead_coordinates.clear();
        bead_orientations.clear();
        bead_to_world.clear();
        bead_masses.clear();
        bead_inertia.clear();
        return;
    }

    bead_coordinates = QVector<Vector>(nbeads);
    bead_orientations = QVector<Quaternion>(nbeads);
    bead_to_world = QVector<Matrix>(nbeads);
    bead_masses = QVector<double>(nbeads);
    bead_inertia = QVector<Vector>(nbeads);
    
    bead_coordinates.squeeze();
    bead_orientations.squeeze();
    bead_to_world.squeeze();
    bead_masses.squeeze();
    bead_inertia.squeeze();
    
    Vector *bead_coords_array = bead_coordinates.data();
    Matrix *bead_to_world_array = bead_to_world.data();
    Quaternion *bead_orients_array = bead_orientations.data();
    double *bead_masses_array = bead_masses.data();
    Vector *bead_inertia_array = bead_inertia.data();
    const Beading *beading_array = beads_to_atoms.constData();
    
    for (int i=0; i<nmols; ++i)
    {
        MolNum molnum = molgroup.molNumAt(i);
        
        const ViewsOfMol &mol = molgroup[molnum];
        
        const MoleculeData &moldata = mol.data();
        
        if (moldata.hasProperty(beading_property))
        {
            //use the beading property to generate the beading for this molecule
            
        }
        else
        {
            //the entire molecule is a single bead
            atoms_to_beads_array[i] = QVector<qint32>();
        }

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
        
        //we now have arrays of the coords and masses for the atoms in each
        //molecule - the next step is to arrange these arrays so that they
        //can be used to generate the data for each bead
        for (int ibead=bead_idx; ibead <= bead_idx+nmolbeads; ++ibead)
        {
            const Beading &beading = beading_array[ibead];
        
            atom_int_coords_array[ibead] = ::getCOMPlusInertia(
                                                coords.constData() + beading.start,
                                                masses.constData() + beading.start,
                                                beading.count,
                                                bead_masses_array[ibead],
                                                bead_inertia_array[ibead],
                                                bead_to_world_array[ibead]);
                                                
            bead_orients_array[ibead] = Quaternion(); // equals identity matrix
        }
    }
    
    //create space for the forces, torques and momenta
    bead_forces = QVector<Vector>(nbeads, Vector(0));
    bead_torques = QVector<Vector>(nbeads, Vector(0));
    bead_linear_momenta = QVector<Vector>(nbeads, Vector(0));
    bead_angular_momenta = QVector<Vector>(nbeads, Vector(0));
    
    bead_forces.squeeze();
    bead_torques.squeeze();
    bead_linear_momenta.squeeze();
    bead_angular_momenta.squeeze();
}

/** Return the array of forces of the ith bead. This does not
    check that 'i' is a valid index - use of an invalid index
    will lead to undefined results (e.g. crash or worse) */
const Vector* RBWorkspace::atomForceArray(int i) const
{
    if (atom_forces.isEmpty())
        return forceTable().getTable( moleculeGroup().molNumAt(i) ).constValueData();

    const QVector<Vector> &forces = atom_forces.constData()[i];
    
    if (forces.isEmpty())
        //we can get the forces straight from the forcetable
        return forceTable().getTable( moleculeGroup().molNumAt(i) ).constValueData();

    else
        return forces.constData();
}

static Vector cross(const Vector &v0, const Vector &v1)
{
    return Vector( v0.y()*v1.z() - v0.z()*v1.y(),
                   v0.z()*v1.x() - v0.x()*v1.z(),
                   v0.x()*v1.y() - v0.y()*v1.x() );
}

/** Calculate the forces and torques */
bool RBWorkspace::calculateForces(const Symbol &nrg_component)
{
    if (not IntegratorWorkspace::calculateForces(nrg_component))
        return false;

    else if (not atom_forces.isEmpty())
    {
        //we need to update the arrays of forces for partial molecules
        int nmols = atom_forces.count();
        QVector<Vector> *atom_forces_array = atom_forces.data();
        
        const MoleculeGroup &molgroup = moleculeGroup();
        const ForceTable &forcetable = forceTable();
        
        for (int i=0; i<nmols; ++i)
        {
            MolNum molnum = molgroup.molNumAt(i);
            const ViewsOfMol &mol = molgroup[molnum].data();
        
            if (mol.selectedAll())
                atom_forces_array[i] = QVector<Vector>();

            else
                atom_forces_array[i] = forcetable.getTable(molnum)
                                                 .toVector(mol.selection());
        }
    }

    const MoleculeGroup &molgroup = moleculeGroup();
    int nmols = molgroup.nMolecules();
    
    //for the moment, we'll make one bead per molecule
    int nbeads = nmols;
    
    Vector *bead_forces_array = bead_forces.data();
    Vector *bead_torques_array = bead_torques.data();
    const QVector<Vector> *atom_int_coords_array = atom_int_coords.constData();
    
    for (int i=0; i<nmols; ++i)
    {
        int ibead = i;

        MolNum molnum = molgroup.molNumAt(i);

        //now calculate the forces and torques on each bead
        const Vector *atomforces = this->atomForceArray(ibead);
        int nats = this->nAtoms(ibead);
        
        BOOST_ASSERT( nats != 0 );
        
        if (nats == 1)
        {
            //only a force - no torque
            bead_forces_array[ibead] = atomforces[0];
            bead_torques_array[ibead] = Vector(0);
        }
        else
        {
            Vector &bead_force = bead_forces_array[ibead];
            Vector &bead_torque = bead_torques_array[ibead];
        
            bead_force = Vector(0);
            bead_torque = Vector(0);
            
            Matrix orient = bead_orientations.constData()[ibead].toMatrix() * 
                            bead_to_world.constData()[ibead];
            
            const Vector &com = bead_coordinates.constData()[ibead];
                                                               
            const Vector *int_coords = atom_int_coords_array[ibead].constData();
            
            for (int j=0; j<nats; ++j)
            {
                bead_force += atomforces[j];
                
                //calculate the vector from the center of mass to 
                //the atom, in the World cartesian frame
                Vector r = orient * int_coords[j];

                //the torque is r cross force (need unnormalised cross product)
                bead_torque -= ::cross(r, atomforces[j]);
            }

            //map the torque back from the cartesian frame to the 
            //internal frame
            bead_torque = orient.inverse() * bead_torque;
        }
    }
    
    return true;
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
              beads_to_atoms(other.beads_to_atoms),
              mols_to_beads(other.mols_to_beads),
              atom_int_coords(other.atom_int_coords),
              atom_forces(other.atom_forces),
              bead_coordinates(other.bead_coordinates),
              bead_to_world(other.bead_to_world),
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
     
        beads_to_atoms = other.beads_to_atoms;
        mols_to_beads = other.mols_to_beads;
        atom_int_coords = other.atom_int_coords;
        atom_forces = other.atom_forces;
        bead_coordinates = other.bead_coordinates;
        bead_to_world = other.bead_to_world;
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
            beads_to_atoms == other.beads_to_atoms and
            bead_coordinates == other.bead_coordinates and
            bead_to_world == other.bead_to_world and
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
    //sum together the linear kinetic energy of the beads...
    double nrg = 0;
    
    int nbeads = bead_linear_momenta.count();
    
    const Vector *p = bead_linear_momenta.constData();
    const double *m = bead_masses.constData();
    
    for (int i=0; i<nbeads; ++i)
    {
        if (m[i] != 0)
            nrg += p[i].length2() / (2 * m[i]);
    }
    
    //now the angular kinetic energy
    const Vector *q = bead_angular_momenta.constData();
    const Vector *I = bead_inertia.constData();
    
    for (int i=0; i<nbeads; ++i)
    {
        if (I[i].x() != 0)
            nrg += pow_2(q[i].x()) / (2 * I[i].x());
            
        if (I[i].y() != 0)
            nrg += pow_2(q[i].y()) / (2 * I[i].y());
            
        if (I[i].z() != 0)
            nrg += pow_2(q[i].z()) / (2 * I[i].z());
    }
    
    return MolarEnergy(nrg);
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
    return atom_int_coords.constData()[i].count();
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

/** Return the array of coordinates of the center of masses of the beads */
Vector* RBWorkspace::beadCoordsArray()
{
    return bead_coordinates.data();
}

/** Return the array of orientations of the beads (this is the rotation
    to be applied to the matrix that maps from the cartesian frame
    to the internal principle inertia frame) */
Quaternion* RBWorkspace::beadOrientationArray()
{
    return bead_orientations.data();
}

/** Return the array of bead linear momenta */
Vector* RBWorkspace::beadLinearMomentaArray()
{
    return bead_linear_momenta.data();
}

/** Return the array of bead angular momenta */
Vector* RBWorkspace::beadAngularMomentaArray()
{
    return bead_angular_momenta.data();
}

/** Return the array of bead masses */
const double* RBWorkspace::beadMassesArray() const
{
    return bead_masses.constData();
}

/** Retunr the array of bead inertia */
const Vector* RBWorkspace::beadInertiasArray() const
{
    return bead_inertia.constData();
}
    
/** Return the array of forces acting on the center of mass
    of each bead */
const Vector* RBWorkspace::beadForcesArray() const
{
    return bead_forces.data();
}

/** Return the array of bead torques */
const Vector* RBWorkspace::beadTorquesArray() const
{
    return bead_torques.data();
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
    int nmols = atom_int_coords.count();
    
    const MoleculeGroup &molgroup = moleculeGroup();
    const Molecules &molecules = molgroup.molecules();
    
    BOOST_ASSERT( molgroup.nMolecules() == nmols );
    
    const QVector<Vector> *atom_intcoords_array = atom_int_coords.constData();
    
    PropertyName coords_property = coordinatesProperty();
    
    Molecules changed_mols;
    changed_mols.reserve(nmols);
    
    for (int i=0; i<nmols; ++i)
    {
        //currently assume one bead per molecule
        int ibead = i;
    
        MolNum molnum = molgroup.molNumAt(i);
        
        const ViewsOfMol &mol = molecules[molnum];
        
        AtomCoords coords = mol.data().property(coords_property)
                                      .asA<AtomCoords>();

        //calculate the world-cartesian coordinates of the beads
        //from the current orientation and internal coordinates
        const QVector<Vector> &int_coords = atom_intcoords_array[ibead];
        QVector<Vector> new_coords = int_coords;
        
        const Vector &com = bead_coordinates.constData()[ibead];
        Matrix orient = bead_orientations.constData()[ibead].toMatrix() * 
                        bead_to_world.constData()[ibead];
        
        int nats = int_coords.count();
        const Vector *int_coords_array = int_coords.constData();
        Vector *new_coords_array = new_coords.data();
        
        for (int j=0; j<nats; ++j)
        {
            new_coords_array[j] = com + (orient * int_coords_array[j]);
        }
                                          
        if (mol.selectedAll())
            coords.copyFrom(new_coords);
        else
            coords.copyFrom(new_coords, mol.selection());

        changed_mols.add( mol.molecule().edit()
                             .setProperty(coords_property, coords)
                             .commit() );
    }
    
    IntegratorWorkspace::pvt_update(changed_mols);
}

/** Commit the linear and angular velocities back to the system. This saves
    the velocities as bead properties */
void RBWorkspace::commitVelocities()
{
    //throw SireError::incomplete_code( QObject::tr("Need to write!"), CODELOC );
}

/** Commit both the coordinates and velocities - this performs the 
    equivalent of commitCoordinates() and commitVelocities() in
    a single call */
void RBWorkspace::commitCoordinatesAndVelocities()
{
    RBWorkspace::commitCoordinates();
    //throw SireError::incomplete_code( QObject::tr("Need to write!"), CODELOC );
}

/** This internal function is called whenever a property is changed. 
    This is used to see if the data has to be regenerated */
void RBWorkspace::changedProperty(const QString &property)
{
    this->rebuildFromScratch();
}
