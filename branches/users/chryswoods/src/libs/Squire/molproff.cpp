/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "molproff.h"
#include "molproprocessor.h"
#include "molprosession.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/residueinfo.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"
#include "SireMol/element.h"

#include "SireMM/atomiccharges.h"

#include "SireUnits/convert.h"
#include "SireUnits/units.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace Squire;
using namespace SireMM;
using namespace SireFF;
using namespace SireMol;
using namespace SireMaths;
using namespace SireBase;
using namespace SireUnits;
using namespace SireStream;

///////////
/////////// Implementation of MolproFF::Parameters
///////////

/** Constructor - by default the coulomb properties come from the 'charges'
    property */
MolproFF::Parameters::Parameters()
         : FFBase::Parameters(), coulomb_params("coulomb", "charges")
{}

/** Copy constructor */
MolproFF::Parameters::Parameters(const MolproFF::Parameters &other)
         : FFBase::Parameters(other), coulomb_params(other.coulomb_params)
{}

/** Destructor */
MolproFF::Parameters::~Parameters()
{}

/** Static object returned by MolproFF::parameters() */
MolproFF::Parameters MolproFF::Parameters::default_sources;

///////////
/////////// Implementation of MolproFF::Components
///////////

/** Constructor */
MolproFF::Components::Components() : FFBase::Components()
{}

/** Construct using the supplied forcefield */
MolproFF::Components::Components(const FFBase &ffbase, const Symbols &symbols)
         : FFBase::Components( ffbase, addToSymbols(symbols, x(), y(), z()) ),
           e_qm(ffbase, "qm", x(), y(), z())
{
    this->registerComponent(e_qm);
}

/** Copy constructor */
MolproFF::Components::Components(const MolproFF::Components &other)
         : FFBase::Components(other), e_qm(other.e_qm)
{}

/** Destructor */
MolproFF::Components::~Components()
{}

/** Assignment operator */
MolproFF::Components& MolproFF::Components::operator=(const MolproFF::Components &other)
{
    e_qm = other.e_qm;

    FFBase::Components::operator=(other);

    return *this;
}

/** Set the forcefield */
void MolproFF::Components::setForceField(const FFBase &ffbase)
{
    *this = MolproFF::Components(ffbase);
}

/** Describe the coulomb component */
QString MolproFF::Components::describe_qm()
{
    return QObject::tr("The total QM energy (includes the QM part of the QM/MM energy).");
}

///////////
/////////// Implementation of MolproFF::QMMolecule
///////////

/** Null constructor */
MolproFF::QMMolecule::QMMolecule()
{}

/** Construct a QM molecule from the passed Molecule */
MolproFF::QMMolecule::QMMolecule(const Molecule &molecule)
         : mol(molecule)
{
    //create the arrays that hold all of the element types
    //of each atom in the molecule, and all of the indicies
    //of each atom in the qm_coords array.
    uint ncg = mol.nCutGroups();

    elements.reserve(ncg);
    indicies.reserve(ncg);

    for (CutGroupID i(0); i<ncg; ++i)
    {
        uint natms = molecule.nAtoms(i);

        QVector<Element> group_elements(natms);
        QVector<int> group_indicies(natms, -1);

        for (AtomID j(0); j<natms; ++j)
        {
            group_elements[j] = mol.atom( CGAtomID(i,j) );
        }

        elements.append(group_elements);
        indicies.append(group_indicies);
    }
}

/** Copy constructor */
MolproFF::QMMolecule::QMMolecule(const MolproFF::QMMolecule &other)
         : mol(other.mol), elements(other.elements), indicies(other.indicies)
{}

/** Destructor */
MolproFF::QMMolecule::~QMMolecule()
{}

/** Copy assignment */
MolproFF::QMMolecule& MolproFF::QMMolecule::operator=(const MolproFF::QMMolecule &other)
{
    mol = other.mol;
    elements = other.elements;
    indicies = other.indicies;

    return *this;
}

/** Return the Molecule contained in this QMMolecule */
const Molecule& MolproFF::QMMolecule::molecule() const
{
    return mol;
}

/** Change the molecule - return whether it changed */
bool MolproFF::QMMolecule::change(const Molecule &molecule)
{
    BOOST_ASSERT(molecule.ID() == mol.ID());

    if (molecule.version() != mol.version())
    {
        mol = molecule;
        return true;
    }
    else
        return false;
}

/** Change the residue in the molecule */
bool MolproFF::QMMolecule::change(const Residue &residue)
{
    return this->change(residue.molecule());
}

/** Change the atom in the molecule */
bool MolproFF::QMMolecule::change(const NewAtom &atom)
{
    return this->change(atom.molecule());
}

/** Return the number of atoms in the array */
int MolproFF::QMMolecule::nAtomsInArray() const
{
    int nqmatoms = 0;

    int ngroups = elements.count();
    const QVector<Element> *elements_array = elements.constData();

    for (int i=0; i<ngroups; ++i)
    {
        const QVector<Element> &elements_vector = elements_array[i];

        int nats = elements_vector.count();
        const Element *elem_array = elements_vector.constData();

        for (int j=0; j<nats; ++j)
        {
            if (elem_array[j].nProtons() != 0)
                ++nqmatoms;
        }
    }

    return nqmatoms;
}

/** Add the coordinates of this QM molecule to the array 'qm_array' */
void MolproFF::QMMolecule::addTo(QVector<double> &qm_array)
{
    const QVector<CoordGroup> &coordgroups = mol.coordGroups();

    int ngroups = coordgroups.count();

    BOOST_ASSERT(ngroups == elements.count());
    BOOST_ASSERT(ngroups == indicies.count());

    const CoordGroup *groups_array = coordgroups.constData();
    const QVector<Element> *elements_array = elements.constData();
    QVector<int> *indicies_array = indicies.data();

    //loop over all atoms in every cutgroup
    for (int i=0; i<ngroups; ++i)
    {
        int natoms = groups_array[i].count();
        BOOST_ASSERT(natoms == elements_array[i].count());
        BOOST_ASSERT(natoms == indicies_array[i].count());

        const Vector *coords_array = groups_array[i].constData();
        const Element *elem_array = elements_array[i].constData();
        int *indx_array = indicies_array[i].data();

        for (int j=0; j<natoms; ++j)
        {
            if (elem_array[j].nProtons() != 0)
            {
                //add the coordinates of the atom to the qm_coords array,
                //recording the index in the array of the x-coordinates of
                //the atom
                indx_array[j] = qm_array.count();

                //need to convert from angstrom to bohr radii
                qm_array.append( convertTo(coords_array[j].x(), bohr_radii) );
                qm_array.append( convertTo(coords_array[j].y(), bohr_radii) );
                qm_array.append( convertTo(coords_array[j].z(), bohr_radii) );
            }
            else
                //this is a dummy atom, so is not included in the QM calculation
                indx_array[j] = -1;
        }
    }
}

/** Return Molpro format strings that describe the geometry of this molecule */
QString MolproFF::QMMolecule::coordString() const
{
    QString qmcoords = "";

    const QVector<CoordGroup> &coordgroups = mol.coordGroups();

    int ngroups = coordgroups.count();

    BOOST_ASSERT(ngroups == elements.count());

    const CoordGroup *groups_array = coordgroups.constData();
    const QVector<Element> *elements_array = elements.constData();

    //loop over all atoms in every cutgroup
    for (int i=0; i<ngroups; ++i)
    {
        int natoms = groups_array[i].count();
        BOOST_ASSERT(natoms == elements_array[i].count());

        const Vector *coords_array = groups_array[i].constData();
        const Element *elem_array = elements_array[i].constData();

        for (int j=0; j<natoms; ++j)
        {
            const Element &element = elem_array[j];

            if (element.nProtons() != 0)
            {
                //this is not a dummy atom - add it to the list of QM atoms
                qmcoords += QString("%1,%2,%3,%4\n")
                                .arg(element.symbol())
                                .arg( convertTo(coords_array[j].x(), angstrom) )
                                .arg( convertTo(coords_array[j].y(), angstrom) )
                                .arg( convertTo(coords_array[j].z(), angstrom) );
            }
        }
    }

    return qmcoords;
}

/** Update the coordinates of the atoms in the array 'qm_array' */
void MolproFF::QMMolecule::update(QVector<double> &qm_array) const
{
    const QVector<CoordGroup> &coordgroups = mol.coordGroups();

    int ncg = coordgroups.count();

    const CoordGroup *groups_array = coordgroups.constData();
    const QVector<int> *indicies_array = indicies.constData();

    double *qm_array_array = qm_array.data();
    int maxcoords = qm_array.count();

    for (int i=0; i<ncg; ++i)
    {
        int natms = groups_array[i].count();

        for (int j=0; j<natms; ++j)
        {
            const Vector *coords_array = groups_array[i].constData();
            int idx = indicies_array[i].constData()[j];

            if (idx >= 0)
            {
                BOOST_ASSERT( idx+2 < maxcoords );

                const Vector &coord = coords_array[j];

                //need to convert from angstroms to bohr radii...
                qm_array_array[idx] = convertTo(coord.x(), bohr_radii);
                qm_array_array[idx+1] = convertTo(coord.y(), bohr_radii);
                qm_array_array[idx+2] = convertTo(coord.z(), bohr_radii);
            }
        }
    }
}

///////////
/////////// Implementation of MolproFF::MMMolecule
///////////

/** Null constructor */
MolproFF::MMMolecule::MMMolecule() : idx(-1), nats(0)
{}

/** Construct an MMMolecule from the passed Molecule, using the
    supplied ParameterMap to find the atomic charges on the atoms.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
MolproFF::MMMolecule::MMMolecule(const Molecule &molecule,
                                 const QString &charge_property)
         : mol(molecule), chg_property(charge_property),
           idx(-1), nats(0), rebuild_all(true)
{
    //get the atomic charges...
    chgs = mol.getProperty(chg_property);
}

/** Copy constructor */
MolproFF::MMMolecule::MMMolecule(const MMMolecule &other)
         : mol(other.mol), chg_property(other.chg_property),
           chgs(other.chgs), coords(other.coords),
           cgids_to_be_rebuilt(other.cgids_to_be_rebuilt),
           idx(other.idx), nats(other.nats),
           rebuild_all(other.rebuild_all)
{}

/** Destructor */
MolproFF::MMMolecule::~MMMolecule()
{}

/** Copy assignment */
MolproFF::MMMolecule& MolproFF::MMMolecule::operator=(const MolproFF::MMMolecule &other)
{
    if (this != &other)
    {
        mol = other.mol;
        chg_property = other.chg_property;
        chgs = other.chgs;
        coords = other.coords;
        cgids_to_be_rebuilt = other.cgids_to_be_rebuilt;
        rebuild_all = other.rebuild_all;
        idx = other.idx;
        nats = other.nats;
    }

    return *this;
}

/** Return the actual molecule */
const Molecule& MolproFF::MMMolecule::molecule() const
{
    return mol;
}

/** Change the molecule */
bool MolproFF::MMMolecule::change(const Molecule &molecule)
{
    BOOST_ASSERT(molecule.ID() == mol.ID());

    if (molecule.version() != mol.version())
    {
        mol = molecule;
        cgids_to_be_rebuilt.clear();
        rebuild_all = true;

        return true;
    }
    else
        return false;
}

/** Change a residue in the molecule */
bool MolproFF::MMMolecule::change(const Residue &residue)
{
    BOOST_ASSERT(residue.ID() == mol.ID());

    if (rebuild_all)
    {
        return this->change(residue.molecule());
    }
    else if (residue.version() != mol.version())
    {
        mol = residue.molecule();

        cgids_to_be_rebuilt += residue.info().cutGroupIDs();

        if (cgids_to_be_rebuilt.count() == mol.nCutGroups())
        {
            cgids_to_be_rebuilt.clear();
            rebuild_all = true;
        }

        return true;
    }

    return false;
}

/** Change an atom in the molecule */
bool MolproFF::MMMolecule::change(const NewAtom &atom)
{
    BOOST_ASSERT(atom.ID() == mol.ID());

    if (rebuild_all)
    {
        return this->change(atom.molecule());
    }
    else if (atom.version() != mol.version())
    {
        mol = atom.molecule();

        cgids_to_be_rebuilt.insert( atom.cgAtomID().cutGroupID() );

        if (cgids_to_be_rebuilt.count() == mol.nCutGroups())
        {
            cgids_to_be_rebuilt.clear();
            rebuild_all = true;
        }

        return true;
    }

    return false;
}

/** Update the MMMolecule - this finds all of the CoordGroups from
    this molecule that are within the cutoff of the coordgroup
    'qm_coordgroup', based on the space 'space' using the
    switching function 'switchfunc' */
void MolproFF::MMMolecule::update(const CoordGroup &qm_coordgroup,
                                  const Space &space,
                                  const SwitchingFunction &switchfunc)
{
    //zero the number of atoms of this molecule in the array...
    nats = 0;

    //loop over all of the CoordGroups in this molecule and
    //get all copies that are within the cutoff distance of
    //any of the CoordGroups of the QM atoms
    const QVector<CoordGroup> &coordgroups = mol.coordGroups();
    int ncg = coordgroups.count();

    coords = QVector< QList< tuple<double,CoordGroup> > >(ncg);

    for (int i=0; i<ncg; ++i)
    {
        //how many atoms in this CoordGroup have a charge?
        const QVector<ChargeParameter> &chargegroup = chgs.at(i);

        int natms_with_charges = 0;
        int natms = chargegroup.count();

        for (int j=0; j<natms; ++j)
        {
           natms_with_charges += (chargegroup.at(j) != 0);
        }

        if (natms_with_charges > 0)
        {
            //some of the atoms in this CoordGroup have charges!

            //get all of the copies of this CoordGroup that are
            //within the cutoff distance of any of the QM atoms
            QList< tuple<double,CoordGroup> > copies =
                                    space.getCopiesWithin(coordgroups.at(i),
                                                qm_coordgroup,
                                                switchfunc.cutoffDistance());

            //loop through each copy and set its scale factor
            QMutableListIterator< tuple<double,CoordGroup> > it(copies);

            while( it.hasNext() )
            {
                it.next();

                double scalefac = switchfunc.electrostaticScaleFactor(it.value().get<0>());

                if (scalefac == 0)
                    //a zero scale factor would wipe out
                    //all of the atoms of this copy
                    it.remove();
                else
                    //update the scale factor
                    it.value().get<0>() = scalefac;
            }

            //save this list with the CutGroup
            coords[i] = copies;

            //increment the number of atoms - this is the number
            //of non-zero charge atoms in the group times the number
            //of copies of this group
            nats += natms_with_charges * copies.count();
        }
    }
}

/** Update the mm_coords_and_charges array with the coordinates
    and charges of atoms in this molecule. This will either
    replace the existing information about this molecule in
    mm_coords_and_charges, or it will add its information
    onto the end of the array. */
void MolproFF::MMMolecule::update(QVector<double> &mm_coords_and_charges)
{
    if (nats == 0)
    {
        //we aren't in the array, and we shouldn't be
        idx = -1;

        //There is nothing to do :-)
        return;
    }

    //make sure that there is sufficient space in the array
    if (idx + this->nAtomsInArray() > mm_coords_and_charges.count())
    {
        throw SireError::program_bug( QObject::tr(
            "There is insufficient space for this MM molecule in the "
            "mm_coords_and_charges array! (Have %1 but need %2)")
                  .arg(mm_coords_and_charges.count())
                  .arg(idx + this->nAtomsInArray()), CODELOC );
    }

    //ok - we assume that the array from idx to 4*nats is ours...
    int ngroups = coords.count();
    const QList< tuple<double,CoordGroup> > *coords_array = coords.constData();
    const QVector<ChargeParameter> *chgs_array = chgs.constData();

    int current_idx = idx;

    //add each CutGroup in turn...
    for (int i=0; i<ngroups; ++i)
    {
        const QList< tuple<double,CoordGroup> > &group_coords = coords_array[i];
        const ChargeParameter *group_charges = chgs_array[i].constData();

        //loop over each copy of the CoordGroup and add it in turn
        for (QList< tuple<double,CoordGroup> >::const_iterator it =
                                                    group_coords.constBegin();
             it != group_coords.constEnd();
             ++it)
        {
            double scalefac = it->get<0>();
            const CoordGroup &copy = it->get<1>();

            int natms = copy.count();
            const Vector *copy_array = copy.constData();

            for (int j=0; j<natms; ++j)
            {
                const Vector &atom = copy_array[j];
                double atom_charge = group_charges[j].charge();

                if (atom_charge != 0)
                {
                    mm_coords_and_charges[current_idx] = convertTo(atom.x(), bohr_radii);
                    ++current_idx;
                    mm_coords_and_charges[current_idx] = convertTo(atom.y(), bohr_radii);
                    ++current_idx;
                    mm_coords_and_charges[current_idx] = convertTo(atom.z(), bohr_radii);
                    ++current_idx;
                    mm_coords_and_charges[current_idx] = convertTo(scalefac*atom_charge,
                                                                   mod_electrons);
                    ++current_idx;
                }
            }
        }
    }
}

/** Return a Molpro format string providing the coordinates and charges of atoms
    in this molecule that are within the electrostatic cutoff of the QM atoms. */
QString MolproFF::MMMolecule::coordAndChargesString() const
{
    QString mmcoords = "";

    if (nats == 0)
        //There is nothing to do :-)
        return mmcoords;

    int ngroups = coords.count();
    const QList< tuple<double,CoordGroup> > *coords_array = coords.constData();
    const QVector<ChargeParameter> *chgs_array = chgs.constData();

    //add each CutGroup in turn...
    for (int i=0; i<ngroups; ++i)
    {
        const QList< tuple<double,CoordGroup> > &group_coords = coords_array[i];
        const ChargeParameter *group_charges = chgs_array[i].constData();

        //loop over each copy of the CoordGroup and add it in turn
        for (QList< tuple<double,CoordGroup> >::const_iterator it =
                                                    group_coords.constBegin();
             it != group_coords.constEnd();
             ++it)
        {
            double scalefac = it->get<0>();
            const CoordGroup &copy = it->get<1>();

            int natms = copy.count();
            const Vector *copy_array = copy.constData();

            for (int j=0; j<natms; ++j)
            {
                const Vector &atom = copy_array[j];
                double atom_charge = group_charges[j].charge();

                if (atom_charge != 0)
                {
                    mmcoords += QString("%1, %2, %3, %4\n")
                                  .arg( convertTo(atom.x(), angstrom) )
                                  .arg( convertTo(atom.y(), angstrom) )
                                  .arg( convertTo(atom.z(), angstrom) )
                                  .arg( convertTo(scalefac*atom_charge,
                                                  mod_electrons) );
                }
            }
        }
    }

    return mmcoords;
}

/** Add the atoms and their charges from this molecule to the
    end of the mm_coords_and_charges array. */
void MolproFF::MMMolecule::addTo(QVector<double> &mm_coords_and_charges,
                                 int start_idx)
{
    idx = start_idx;
    this->update(mm_coords_and_charges);
}

/** Return the current number of atoms that should
    be in the mm_coords_and_charges array */
int MolproFF::MMMolecule::nAtomsInArray() const
{
    return nats;
}

/** Return the name of the property containing the charges */
const QString& MolproFF::MMMolecule::chargeProperty() const
{
    return chg_property;
}

///////////
/////////// Implementation of MolproFF
///////////

static const RegisterMetaType<MolproFF> r_molproff;

/** Serialise to a binary data stream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const MolproFF &molproff)
{
    writeHeader(ds, r_molproff, 1);

    SharedDataStream sds(ds);

    sds << molproff.spce << molproff.switchfunc
        << molproff.molpro_exe.absoluteFilePath()     //save the full path to the
        << molproff.molpro_tmpdir.absolutePath()      //molpro exe and temp dirs...
        << molproff.qm_version;

    //now stream in all of the QM molecules
    const QHash<MoleculeID,MolproFF::QMMolecule> &qm_mols = molproff.qm_mols;

    sds << quint32( qm_mols.count() );

    for (QHash<MoleculeID,MolproFF::QMMolecule>::const_iterator it = qm_mols.begin();
         it != qm_mols.end();
         ++it)
    {
        sds << it->molecule();
    }

    //now stream in all of the MM molecules
    const QHash<MoleculeID,MolproFF::MMMolecule> &mm_mols = molproff.mm_mols;

    sds << quint32( mm_mols.count() );

    for (QHash<MoleculeID,MolproFF::MMMolecule>::const_iterator it = mm_mols.begin();
         it != mm_mols.end();
         ++it)
    {
        //save the molecule and the name of the property containing
        //the charges
        sds << it->molecule() << it->chargeProperty();
    }

    //stream in the base class...
    sds << static_cast<const FFBase&>(molproff);

    //don't stream qm_coords qm_coordgroup and
    //mm_coords_and_charges as they can be rebuilt

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, MolproFF &molproff)
{
    VersionID v = readHeader(ds, r_molproff);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        QString molpro_exe_path, molpro_tmpdir_path;

        sds >> molproff.spce >> molproff.switchfunc
            >> molpro_exe_path >> molpro_tmpdir_path
            >> molproff.qm_version;

        //get the fileinfo for the molpro executable
        QFileInfo molpro_exe( molpro_exe_path );

        //what should I do if this doesn't exist?

        //get the fileinfo for the molpro temp directory
        QDir molpro_tmpdir( molpro_tmpdir_path );

        //again, what should I do if this doesn't exist?

        molproff.molpro_exe = molpro_exe;
        molproff.molpro_tmpdir = molpro_tmpdir;

        //stream in all of the QM molecules
        quint32 nmols;
        sds >> nmols;

        QHash<MoleculeID,MolproFF::QMMolecule> qm_mols;

        if (nmols > 0)
        {
            qm_mols.reserve(nmols);

            for (uint i=0; i<nmols; ++i)
            {
                Molecule qm_mol;
                sds >> qm_mol;

                qm_mols.insert(qm_mol.ID(), qm_mol);
            }
        }

        molproff.qm_mols = qm_mols;

        //stream in all of the MM molecules
        sds >> nmols;

        QHash<MoleculeID,MolproFF::MMMolecule> mm_mols;

        if (nmols > 0)
        {
            mm_mols.reserve(nmols);

            for (uint i=0; i<nmols; ++i)
            {
                Molecule mm_mol;
                QString chg_property;

                sds >> mm_mol >> chg_property;

                mm_mols.insert(mm_mol.ID(), MolproFF::MMMolecule(mm_mol,chg_property));
            }
        }

        molproff.mm_mols = mm_mols;

        //read in the base class...
        sds >> static_cast<FFBase&>(molproff);

        //need to rebuild qm_coords, qm_coordgroup
        //and mm_coords_and_charges
        molproff.rebuild_all = true;
        molproff.need_recalculate_qmmm = true;
        molproff.rebuild_mm.clear();
    }
    else
        throw version_error(v, "1", r_molproff, CODELOC);

    return ds;
}

/** Incremint used to version MolproFF QM regions */
static Incremint global_molproff_qm_version;

/** Construct an empty MolproFF */
MolproFF::MolproFF()
         : FFBase(),
           molpro_exe("molpro"), molpro_tmpdir(QDir::temp()),
           qm_version(&global_molproff_qm_version),
           rebuild_all(true), need_recalculate_qmmm(true)
{
    this->registerComponents();
}

/** Construct a MolproFF with a specfied Space and switching function */
MolproFF::MolproFF(const Space &space, const SwitchingFunction &switchingfunction)
         : FFBase(),
           spce(space), switchfunc(switchingfunction),
           molpro_exe("molpro"), molpro_tmpdir(QDir::temp()),
           qm_version(&global_molproff_qm_version),
           rebuild_all(true), need_recalculate_qmmm(true)
{
    this->registerComponents();
}

/** Copy constructor */
MolproFF::MolproFF(const MolproFF &other)
         : FFBase(other),
           spce(other.spce), switchfunc(other.switchfunc),
           molpro_exe(other.molpro_exe), molpro_tmpdir(other.molpro_tmpdir),
           qm_coords(other.qm_coords),
           mm_coords_and_charges(other.mm_coords_and_charges),
           qm_mols(other.qm_mols), mm_mols(other.mm_mols),
           qm_version(other.qm_version),
           rebuild_mm(other.rebuild_mm), rebuild_all(other.rebuild_all),
           need_recalculate_qmmm(other.need_recalculate_qmmm)
{
    //get the pointer from the base class...
    components_ptr = dynamic_cast<const MolproFF::Components*>( &(FFBase::components()) );
    BOOST_ASSERT( components_ptr != 0 );
}

/** Destructor */
MolproFF::~MolproFF()
{}

/** Copy assignment */
MolproFF& MolproFF::operator=(const MolproFF &other)
{
    if (this != &other)
    {
        FFBase::operator=(other);

        spce = other.spce;
        switchfunc = other.switchfunc;
        molpro_exe = other.molpro_exe;
        molpro_tmpdir = other.molpro_tmpdir;
        qm_coords = other.qm_coords;
        mm_coords_and_charges = other.mm_coords_and_charges;
        qm_mols = other.qm_mols;
        mm_mols = other.mm_mols;
        qm_version = other.qm_version;
        rebuild_mm = other.rebuild_mm;
        rebuild_all = other.rebuild_all;
        need_recalculate_qmmm = other.need_recalculate_qmmm;

        components_ptr = dynamic_cast<const MolproFF::Components*>( &(FFBase::components()) );
        BOOST_ASSERT( components_ptr != 0 );
    }

    return *this;
}

/** Register the components of this forcefield */
void MolproFF::registerComponents()
{
    std::auto_ptr<MolproFF::Components> ptr( new MolproFF::Components(*this) );

    FFBase::registerComponents(ptr.get());

    components_ptr = ptr.release();
}

/** Set the Molpro executable to use to calculate the energy
     - this increments the major version number if this changes
     the Molpro executable */
void MolproFF::setMolproExe(const QFileInfo &molpro)
{
    if (molpro_exe != molpro)
    {
        molpro_exe = molpro;
        this->incrementMajorVersion();
        qm_version.increment();
    }
}

/** Change the temporary directory in which the molpro job is run */
void MolproFF::setMolproTempDir(const QDir &tempdir)
{
    molpro_tmpdir = tempdir;
}

/** Add a molecule to the QM region

    \throw SireMol::duplicate_molecule
*/
int MolproFF::_pvt_addToQM(const Molecule &molecule)
{
    MoleculeID molid = molecule.ID();

    //check that the molecule is not in the MM region
    if (mm_mols.contains(molid))
        throw SireMol::duplicate_molecule( QObject::tr(
            "Cannot add the molecule %1 to the QM region of the MolproFF "
            "\"%2\" (%3 %4) as this molecule already exists in the MM region.")
                .arg(molecule.idString(), this->name())
                .arg(this->ID()).arg(this->version().toString()), CODELOC );

    //do we already contain this molecule?
    if (qm_mols.contains(molid))
    {
        if (this->change(molecule))
            return MolproFF::CHANGE;
        else
            return MolproFF::NOCHANGE;
    }
    else
    {
        //create a QM molecule to represent this molecule
        MolproFF::QMMolecule qmmol(molecule);

        //add the coordinates of the QM molecule to the
        //QM coordinates array
        qmmol.addTo(qm_coords);

        //save the qm molecule, indexed by its ID
        qm_mols.insert(molid, MolproFF::QMMolecule(molecule));
        rebuild_all = true;
        rebuild_mm.clear();

        return MolproFF::ADD;
    }
}

/** Add a molecule to the MM region.

    \throw SireMol::duplicate_molecule
    \throw SireError::incompatible_error
*/
int MolproFF::_pvt_addToMM(const Molecule &molecule, const ParameterMap &map)
{
    MoleculeID molid = molecule.ID();

    //check that the molecule is not in the QM region
    if (qm_mols.contains(molid))
        throw SireMol::duplicate_molecule( QObject::tr(
            "Cannot add the molecule %1 to the MM region of the MolproFF "
            "\"%2\" (%3 %4) as this molecule already exists in the QM region.")
                .arg(molecule.idString(), this->name())
                .arg(this->ID()).arg(this->version().toString()), CODELOC );

    //do we already contain this molecule?
    if (mm_mols.contains(molid))
    {
        if (this->change(molecule))
            return MolproFF::CHANGE;
        else
            return MolproFF::NOCHANGE;
    }
    else
    {
        //convert to an MMMolecule (gets the atomic charges) and
        //add to the hash of MM molecules in this forcefield
        mm_mols.insert(molid,
                       MolproFF::MMMolecule(molecule,
                                            map.source(parameters().coulomb())
                                           )
                      );

        if (not rebuild_all)
            rebuild_mm.insert(molid);

        return MolproFF::ADD;
    }
}

/** Add a molecule to the QM region

    \throw SireMol::duplicate_molecule
*/
bool MolproFF::addToQM(const Molecule &molecule)
{
    int change = this->_pvt_addToQM(molecule);

    if (change & ADD)
    {
        this->incrementMajorVersion();

        //increment the version number tracking addition
        //and removal from the QM region
        qm_version.increment();
    }
    else if (change and CHANGE)
        this->incrementMinorVersion();

    return isDirty();
}

/** Add a load of molecules to the QM region

    \throw SireMol::duplicate_molecule
*/
bool MolproFF::addToQM(const QList<Molecule> &molecules)
{
    // Add the molecules to a copy of this forcefield - this
    // is to maintain the invariant

    MolproFF copy(*this);

    int change = 0;

    for (QList<Molecule>::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        int added_mol = copy._pvt_addToQM(*it);

        change = change | added_mol;
    }

    if (change & ADD)
    {
        copy.incrementMajorVersion();

        //increment the version number tracking addition
        //and removal from the QM region
        qm_version.increment();
    }
    else if (change & CHANGE)
        copy.incrementMinorVersion();

    //everything's ok - copy back to the original
    *this = copy;

    return isDirty();
}

/** Add a molecule to the MM region.

    \throw SireMol::duplicate_molecule
*/
bool MolproFF::addToMM(const Molecule &molecule, const ParameterMap &map)
{
    int change = this->_pvt_addToMM(molecule, map);

    if (change & ADD)
        this->incrementMajorVersion();
    else if (change & CHANGE)
        this->incrementMinorVersion();

    return isDirty();
}

/** Add a load of molecules to the MM region

    \throw SireMol::duplicate_molecule
*/
bool MolproFF::addToMM(const QList<Molecule> &molecules,
                       const ParameterMap &map)
{
    //add the Molecules to a copy of this forcefield -
    // this maintains the invariant

    MolproFF copy(*this);

    int change = 0;

    for (QList<Molecule>::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        int added_mol = copy._pvt_addToMM(*it, map);

        change = change | added_mol;
    }

    if (change & ADD)
        copy.incrementMajorVersion();
    else if (change & CHANGE)
        copy.incrementMinorVersion();

    //everything's ok - copy back to the original
    *this = copy;

    return isDirty();
}

/** Change the object 'obj'. Does nothing if this object
    is not in this forcefield */
template<class T>
bool MolproFF::_pvt_change(const T &obj)
{
    MoleculeID molid = obj.ID();

    if (qm_mols.contains(molid))
    {
        //the object exists in the QM region - make the change
        if (qm_mols[molid].change(obj))
        {
            //the molecule changed, so we need to rebuild
            //all of the arrays...
            rebuild_all = true;
            rebuild_mm.clear();

            //increment the minor version number
            this->incrementMinorVersion();
        }
    }
    else if (mm_mols.contains(molid))
    {
        //the object exists in the MM region
        if (rebuild_all)
        {
            //if we already have to rebuild everything, then we
            //may as well just change the entire molecule
            if ( mm_mols[molid].change( Molecule(obj) ) )
            {
                this->incrementMinorVersion();
            }
        }
        else
        {
            //try to save time by just changing the object
            if (mm_mols[molid].change(obj))
            {
                rebuild_mm.insert(molid);
                this->incrementMinorVersion();
            }
        }
    }

    return isDirty();
}

/** Change the molecule 'molecule'. Does nothing if this
    molecule is not in this forcefield. */
bool MolproFF::change(const Molecule &molecule)
{
    return this->_pvt_change<Molecule>(molecule);
}

/** Change the residue 'residue'. Does nothing if this
    residue is not in this forcefield. */
bool MolproFF::change(const Residue &residue)
{
    return this->_pvt_change<Residue>(residue);
}

/** Change the atom 'atom'. Does nothing if this
    atom is not in this forcefield. */
bool MolproFF::change(const NewAtom &atom)
{
    return this->_pvt_change<NewAtom>(atom);
}

/** Remove the molecule 'mol'. Does nothing if this
    molecule is not in this forcefield */
bool MolproFF::remove(const Molecule &molecule)
{
    MoleculeID molid = molecule.ID();

    if (qm_mols.contains(molid))
    {
        //remove the molecule from the QM region
        qm_mols.remove(molid);
        rebuild_all = true;
        rebuild_mm.clear();

        //we need to re-add the remaining QM molecules to the QM array
        qm_coords.clear();

        for (QHash<MoleculeID,QMMolecule>::iterator it = qm_mols.begin();
             it != qm_mols.end();
             ++it)
        {
            it->addTo(qm_coords);
        }

        this->incrementMajorVersion();

        //increment the version number that tracks changes in
        //the QM region
        qm_version.increment();
    }
    else if (mm_mols.contains(molid))
    {
        //remove the molecule from the MM region
        MMMolecule mm_mol = mm_mols.take(molid);

        if (mm_mol.nAtomsInArray() > 0)
        {
            //we need to rebuild everything as this molecule
            //was in the mm_coords_and_charges array
            rebuild_all = true;
            rebuild_mm.clear();
        }

        this->incrementMajorVersion();
    }

    return isDirty();
}

/** Rebuild qm_coordgroup - the CoordGroup that contains all of the
    atoms of all of the QM molecules. */
void MolproFF::rebuildQMCoordGroup()
{
    if (qm_mols.count() == 1 and
        qm_mols.constBegin().value().molecule().nCutGroups() == 1)
    {
        //take the easy route!
        qm_coordgroup = qm_mols.constBegin().value().molecule().coordGroups().at(0);
    }
    else
    {
        //how many QM atoms are there?
        int natms = 0;

        for (QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.constBegin();
             it != qm_mols.constEnd();
             ++it)
        {
            natms += it->molecule().nAtoms();
        }

        //create a vector of the right size
        QVector<Vector> all_atoms(natms);

        Vector *atoms_array = all_atoms.data();

        //now copy in the coordinates of all of the QM atoms...
        //(this won't work if the QM molecules are in different
        // simulation boxes... What should I do in this case?)
        // Maybe have CoordGroup = space.combineGroups(groups)?
        for (QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.constBegin();
             it != qm_mols.constEnd();
             ++it)
        {
            const QVector<CoordGroup> &groups = it->molecule().coordGroups();

            int ngroups = groups.count();

            for (int i=0; i<ngroups; ++i)
            {
                int ngroupatoms = groups.at(i).count();
                const Vector *group_atoms = groups.at(i).constData();

                //use memcpy to copy the vector (possible as this
                //is a movable class) (actually use the qMemCopy function provided by Qt
                //in qglobal.h)
                void *output = qMemCopy(atoms_array, group_atoms,
                                        ngroupatoms * sizeof(Vector));

                BOOST_ASSERT(output == atoms_array);

                //advance the atoms_array pointer on for the next group
                atoms_array += ngroupatoms*sizeof(Vector);
            }
        }

        //now we have all of the atom coordinates, place them into
        //the CoordGroup
        qm_coordgroup = CoordGroup(all_atoms);
    }
}

/** Update the QM and MM arrays. This should only be
    called by the energy routines as they use whether
    the arrays change to decide whether or not to
    recalculate the energy! */
bool MolproFF::updateArrays()
{
    if (rebuild_all)
    {
        //update all of the QM arrays...
        for (QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.constBegin();
             it != qm_mols.constEnd();
             ++it)
        {
            it->update(qm_coords);
        }

        //update the qm_coordgroup that contains the coordinates of
        //all of the atoms in the QM molecules
        this->rebuildQMCoordGroup();

        //now update all of the MM molecules - tell them to
        //calculate all of the groups that are within the cutoff
        //distance of any of the QM coordgroups, and count up
        //how many atoms are then going to be in the MM array...
        int natms = 0;

        for (QHash<MoleculeID,MMMolecule>::iterator it = mm_mols.begin();
             it != mm_mols.end();
             ++it)
        {
            it->update(qm_coordgroup, space(), switchingFunction());
            natms += it->nAtomsInArray();
        }

        //ok, all of the MM molecules have now got the list of CutGroups
        //that they need. We also now know how may atoms there are to put
        //in the mm_coords_and_charges array - create sufficient space
        mm_coords_and_charges.clear();

        if (natms > 0)
        {
            mm_coords_and_charges.resize(4*natms);

            int idx = 0;

            for (QHash<MoleculeID,MMMolecule>::iterator it = mm_mols.begin();
                 it != mm_mols.end();
                 ++it)
            {
                it->addTo(mm_coords_and_charges, idx);
                idx += it->nAtomsInArray();
            }
        }

        rebuild_all = false;
        rebuild_mm.clear();
        need_recalculate_qmmm = true;
    }
    else if (not rebuild_mm.isEmpty())
    {
        //we only need to rebuild some of the MM molecules...

        //flag set if we have to rebuild the MM array (happens when
        //the number of MM atoms changes)
        bool rebuild_mm_array = false;

        //flag set when something has changed (e.g. when there definitely
        //is a change to mm_coords_and_charges)
        bool something_changed = false;

        //see if any of the MM molecules have changed...
        for (QSet<MoleculeID>::const_iterator it = rebuild_mm.constBegin();
             it != rebuild_mm.constEnd();
             ++it)
        {
            MMMolecule &mm_mol = mm_mols[*it];

            //how many atoms does this molecule have currently in the array?
            int natms_in_array = mm_mol.nAtomsInArray();

            //update the molecule...
            mm_mols[*it].update(qm_coordgroup,
                                space(), switchingFunction());

            //has something changed (i.e. has this molecule moved from outside
            //the array to being inside the array?)
            something_changed = something_changed or
                                (natms_in_array != 0 or mm_mol.nAtomsInArray() != 0);

            rebuild_mm_array = rebuild_mm_array or
                               (natms_in_array != 0 and
                                natms_in_array != mm_mol.nAtomsInArray());
        }

        if (not something_changed)
        {
            //the change hasn't affected the mm_coords_and_charges array.
            //There is thus no need to redo the QM calculation :-)
            rebuild_mm.clear();
        }
        else if (rebuild_mm_array)
        {
            //rebuild the entire mm_coords_and_charges array - first get
            //its new size...
            int natms = 0;

            for (QHash<MoleculeID,MMMolecule>::const_iterator it = mm_mols.constBegin();
                 it != mm_mols.constEnd();
                 ++it)
            {
                natms += it->nAtomsInArray();
            }

            //reserve sufficient space
            mm_coords_and_charges.clear();

            if (natms > 0)
            {
                mm_coords_and_charges.reserve(4*natms);

                int idx = 0;

                for (QHash<MoleculeID,MMMolecule>::iterator it = mm_mols.begin();
                     it != mm_mols.end();
                     ++it)
                {
                    it->addTo(mm_coords_and_charges, idx);
                    idx += it->nAtomsInArray();
                }
            }
        }
        else
        {
            //no difficult changes in the numbers of MM atoms - just change
            //the existing ones (or add new ones onto the end!)
            for (QSet<MoleculeID>::const_iterator it = rebuild_mm.constBegin();
                 it != rebuild_mm.constEnd();
                 ++it)
            {
                mm_mols[*it].update(mm_coords_and_charges);
            }
        }

        rebuild_mm.clear();
        need_recalculate_qmmm = true;
    }

    return need_recalculate_qmmm;
}

/** Return the string of commands used to calculate the energy */
QString MolproFF::energyCmdString() const
{
    return "HF";
}

/** Return Molpro format strings used to specify the QM geometry */
QString MolproFF::qmCoordString() const
{
    QString qmcoords = "";

    for ( QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.begin();
          it != qm_mols.end();
          ++it )
    {
        qmcoords += it->coordString();
    }

    return qmcoords;
}

/** Return Molpro format strings used to specify the MM geometry and charges */
QString MolproFF::mmCoordAndChargesString()
{
    //need to ensure that the MM array is up to date
    this->updateArrays();

    QString mmcoords = "";

    for (QHash<MoleculeID,MMMolecule>::const_iterator it = mm_mols.begin();
         it != mm_mols.end();
         ++it)
    {
        mmcoords += it->coordAndChargesString();
    }

    return mmcoords;
}

/** Return the number of QM atoms in the array */
int MolproFF::nQMAtomsInArray() const
{
    int nats = 0;

    for (QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.begin();
         it != qm_mols.end();
         ++it)
    {
        nats += it->nAtomsInArray();
    }

    return nats;
}

/** Return the number of MM atoms in the array */
int MolproFF::nMMAtomsInArray() const
{
    int nats = 0;

    for (QHash<MoleculeID,MMMolecule>::const_iterator it = mm_mols.begin();
         it != mm_mols.end();
         ++it)
    {
        nats += it->nAtomsInArray();
    }

    return nats;
}

/** Return the total number of atoms in both the QM and MM arrays */
int MolproFF::nAtomsInArray() const
{
    return nQMAtomsInArray() + nMMAtomsInArray();
}

/** Return the commands used to initialise Molpro with this system
    and calculate the required energy */
QString MolproFF::molproCommandInput()
{
    return QString("geomtyp=xyz\n"
                   "geometry={ NOSYM, NOORIENT,\n"
                   "%1 ! number of atoms\n"
                   "This is an auto-generated molpro command file generated using Sire\n"
                   "%2}\n"
                   "lattice, NUCONLY\n"
                   "BEGIN_DATA\n"
                   "%3\n"
                   "END\n"
                   "START\n"
                   "%4\n")
              .arg( nQMAtomsInArray() )
              .arg( qmCoordString(), mmCoordAndChargesString(),
                    energyCmdString() );
}

/** Use the passed MolproSession to recalculate the energy of
    this forcefield - this throws an exception if the session is
    incompatible with this forcefield

    \throw SireError::incompatible_error
*/
Values MolproFF::recalculateEnergy(MolproSession &session)
{
    session.assertCompatibleWith(*this);

    //update the QM and MM arrays to account for any moves
    if (this->updateArrays())
    {
        //the arrays have changed! - pass them to the session
        session.setArrays(qm_coords, mm_coords_and_charges);

        //calculate the HF energy of the system
        double hf_nrg = session.calculateEnergy(this->energyCmdString());

        need_recalculate_qmmm = false;

        setComponent( components().total(), hf_nrg );
        setComponent( components().qm(), hf_nrg );
    }

    return currentEnergies();
}

/** Recalculate the energy - this starts its own Molpro
    instance to calculate the energy. This is not as
    efficient as when using the MolproProcessor,
    which uses a single instance of Molpro for
    all energy evaluations. */
void MolproFF::recalculateEnergy()
{
    if (this->updateArrays())
    {
        //start a new molpro session for this forcefield
        //(this calculates the energy)
        MolproSession session(*this);

        //obtain the calculated energy from molpro
        double hf_nrg = session.calculateEnergy("HF");

        need_recalculate_qmmm = false;

        //save the energy in the right components
        setComponent( components().total(), hf_nrg );
        setComponent( components().qm(), hf_nrg );

        //exiting destroys 'session', which closes the
        //Molpro process
    }
}
