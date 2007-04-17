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

#include <QFile>

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
         : FFBase::Parameters(),
           coulomb_params("coulomb", "charges")
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
/////////// Implementation of MolproFF::Groups
///////////

/** Constructor */
MolproFF::Groups::Groups() : FFBase::Groups()
{
    _qm = this->getUniqueID();
    _mm = this->getUniqueID();
}

/** Copy constructor */
MolproFF::Groups::Groups(const Groups &other)
                : FFBase::Groups(other), _qm(other._qm), _mm(other._mm)
{}

/** Destructor */
MolproFF::Groups::~Groups()
{}

/** Static instance of this class returned by all MolproFF objects */
MolproFF::Groups MolproFF::Groups::default_group;

///////////
/////////// Implementation of MolproFF::QMMolecule
///////////

/** Null constructor */
MolproFF::QMMolecule::QMMolecule() : nats(0)
{}

int MolproFF::QMMolecule::countElements(const QVector< QVector<Element> > &elements)
{
    int nats = 0;

    //count up the number of non-dummy atoms
    int ngroups = elements.count();

    for (int i=0; i<ngroups; ++i)
    {
        const QVector<Element> &group_elements = elements.at(i);

        int nats = group_elements.count();

        for (int j=0; j<nats; ++j)
        {
            if (group_elements.at(j).nProtons() > 0)
                ++nats;
        }
    }

    return nats;
}

/** Construct a QM molecule from the passed Molecule */
MolproFF::QMMolecule::QMMolecule(const PartialMolecule &molecule)
         : mol(molecule)
{
    coords = mol.coordGroups();
    elements = mol.extract().elements();
    nats = countElements(elements);
}

/** Copy constructor */
MolproFF::QMMolecule::QMMolecule(const MolproFF::QMMolecule &other)
         : mol(other.mol), coords(other.coords), elements(other.elements)
{}

/** Destructor */
MolproFF::QMMolecule::~QMMolecule()
{}

/** Copy assignment */
MolproFF::QMMolecule& MolproFF::QMMolecule::operator=(const MolproFF::QMMolecule &other)
{
    mol = other.mol;
    coords = other.coords;
    elements = other.elements;
    nats = other.nats;

    return *this;
}

/** Return the Molecule contained in this QMMolecule */
const Molecule& MolproFF::QMMolecule::molecule() const
{
    return mol;
}

/** Change the molecule - return whether it changed */
bool MolproFF::QMMolecule::change(const PartialMolecule &molecule)
{
    if (mol.change(molecule))
    {
        coords = mol.coordGroups();
        return true;
    }
    else
        return false;
}

/** Add parts to the molecule - record whether it changed */
bool MolproFF::QMMolecule::add(const PartialMolecule &molecule)
{
    bool changed = mol.change(molecule);

    bool added = mol.add(molecule.selectedAtoms());

    if (changed or added)
    {
        coords = mol.coordGroups();
        elements = mol.extract().elements();
        nats = countElements(elements);

        return true;
    }
    else
        return false;
}

/** Remove some atoms from the molecule */
bool MolproFF::QMMolecule::remove(const AtomSelection &selected_atoms)
{
    if (mol.remove(selected_atoms))
    {
        coords = mol.coordGroups();
        elements = mol.extract().elements();
        nats = countElements(elements);

        return true;
    }
    else
        return false;
}

/** Return the number of atoms in the array */
int MolproFF::QMMolecule::nAtomsInArray() const
{
    return nats;
}

/** Return Molpro format strings that describe the geometry of this molecule */
QString MolproFF::QMMolecule::coordString() const
{
    if (nats == 0)
        return "";

    QString qmcoords = "";

    QVector<CoordGroup> coordgroups = mol.coordGroups();

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

            if (element.nProtons() > 0)
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
int MolproFF::QMMolecule::update(QVector<double> &qm_array, int strtidx) const
{
    if (nats == 0)
        return strtidx;

    QVector<CoordGroup> coordgroups = mol.coordGroups();

    int ncg = coordgroups.count();

    BOOST_ASSERT(ncg == elements.count());

    const CoordGroup *groups_array = coordgroups.constData();
    const QVector<Element> *elements_array = elements.constData();

    double *qm_array_array = qm_array.data();
    int maxcoords = qm_array.count();

    for (int i=0; i<ncg; ++i)
    {
        int natoms = groups_array[i].count();
        BOOST_ASSERT(natoms == elements_array[i].count());

        const Vector *coords_array = groups_array[i].constData();
        const Element *elem_array = elements_array[i].constData();

        for (int j=0; j<natms; ++j)
        {
            const Vector *coords_array = groups_array[i].constData();

            if (elem_array[j].nProtons() > 0)
            {
                BOOST_ASSERT( strtidx+2 < maxcoords );

                const Vector &coord = coords_array[j];

                //need to convert from angstroms to bohr radii...
                qm_array_array[strtidx] = convertTo(coord.x(), bohr_radii);

                ++strtidx;

                qm_array_array[strtidx] = convertTo(coord.y(), bohr_radii);

                ++strtidx;

                qm_array_array[strtidx] = convertTo(coord.z(), bohr_radii);

                ++strtidx;
            }
        }
    }

    return strtidx;
}

///////////
/////////// Implementation of MolproFF::MMMolecule
///////////

/** Null constructor */
MolproFF::MMMolecule::MMMolecule() : nats(0)
{}

/** Construct an MMMolecule from the passed Molecule, using the
    supplied ParameterMap to find the atomic charges on the atoms.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
MolproFF::MMMolecule::MMMolecule(const Molecule &molecule,
                                 const QString &charge_property)
         : mol(molecule), chg_property(charge_property),
           nats(0), rebuild_all(true)
{
    //get the atomic charges...
    chgs = mol.getProperty(chg_property);
}

/** Copy constructor */
MolproFF::MMMolecule::MMMolecule(const MMMolecule &other)
         : mol(other.mol), chg_property(other.chg_property),
           chgs(other.chgs), mol_coords(other.mol_coords),
           coords(other.coords),
           cgids_to_be_rebuilt(other.cgids_to_be_rebuilt),
           nats(other.nats), rebuild_all(other.rebuild_all)
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
        mol_coords = other.mol_coords;
        coords = other.coords;
        cgids_to_be_rebuilt = other.cgids_to_be_rebuilt;
        nats = other.nats;
        rebuild_all = other.rebuild_all;
    }

    return *this;
}

/** Return the actual molecule */
const Molecule& MolproFF::MMMolecule::molecule() const
{
    return mol;
}

/** Change the molecule */
bool MolproFF::MMMolecule::change(const PartialMolecule &molecule,
                                  const QString &chgproperty)
{
    if (chgproperty == QString::null or chgproperty == chg_property)
    {
        if (mol.version().major() != molecule.version().major())
        {
            //the charges may have changed - change the whole molecule
            mol.change(molecule);
            mol_coords = mol.coordGroups();
            chgs = mol.getProperty(chg_property);
            cgids_to_be_rebuilt.clear();
            rebuild_all = true;
            return true;
        }
        else if (mol.change(molecule))
        {
            //the molecule has been changed
            mol_coords = mol.coordGroups();

            if (not rebuild_all)
            {
                if (molecule.nSelectedCutGroups() == molecule.info().nCutGroups())
                {
                    cgids_to_be_rebuilt.clear();
                    rebuild_all = true;
                }
                else
                {
                    cgids_to_be_rebuilt.unite( molecule.selectedCutGroups() );

                    if (cgids.count() == molecule.info().nCutGroups())
                    {
                        //the entire molecule has now been moved
                        cgids_to_be_rebuilt.clear();
                        rebuild_all = true;
                    }
                }
            }

            return true;
        }
        else
            return false;
    }
    else
    {
        //the property has changed - just change the entire molecule
        mol.change(molecule);
        chg_property = chg_property;

        mol_coords = mol.coordGroups();
        chgs = mol.getProperty(chg_property);

        rebuild_all = true;
        cgids_to_be_rebuilt.clear();

        return true;
    }
}

/** Add parts to this molecule */
bool MolproFF::MMMolecule::add(const PartialMolecule &molecule,
                               const QString &chgproperty)
{
    bool changed = this->change(molecule,chgproperty);

    if (mol.add(molecule.selectedAtoms()))
    {
        mol_coords = mol.coordGroups();
        chgs = mol.getProperty(chg_property);

        if (not rebuild_all)
        {
            cgids_to_be_rebuilt.unite( molecule.selectedCutGroups() );

            if (cgids_to_be_rebuilt.count() == mol.info().nCutGroups())
            {
                rebuild_all = true;
                cgids_to_be_rebuilt.clear();
            }
        }

        return true;
    }
    else
        return changed;
}

/** Remove parts of the molecule */
bool MolproFF::MMMolecule::remove(const AtomSelection &selected_atoms)
{
    if (mol.remove(selected_atoms))
    {
        mol = mol.coordGroups();
        #warning Have screwed up the coordGroups() and getProperty() of
        #warning PartialMolecule. Code assumes consistent CutGroupID, but
        #warning changing atom selections make CutGroupID inconsistent!!!
    }
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
    if (nAtomsInArray() == 0)
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
            "mm_coords_and_charges array! (Have %1 but need %2) (%3, %4)")
                  .arg(mm_coords_and_charges.count())
                  .arg(idx + this->nAtomsInArray())
                  .arg(idx).arg(nAtomsInArray()), CODELOC );
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

    if (nAtomsInArray() == 0)
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
                                  .arg( convertTo(atom.x(), bohr_radii) )
                                  .arg( convertTo(atom.y(), bohr_radii) )
                                  .arg( convertTo(atom.z(), bohr_radii) )
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
        << molproff.qm_version << molproff.zero_nrg;

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
            >> molproff.qm_version >> molproff.zero_nrg;

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
           qm_version(&global_molproff_qm_version), zero_nrg(0),
           ff_status(NEED_ENERGY_CALC)
{
    this->registerComponents();
}

/** Construct a MolproFF with a specfied Space and switching function */
MolproFF::MolproFF(const Space &space, const SwitchingFunction &switchingfunction)
         : FFBase(),
           spce(space), switchfunc(switchingfunction),
           molpro_exe("molpro"), molpro_tmpdir(QDir::temp()),
           qm_version(&global_molproff_qm_version), zero_nrg(0),
           ff_status(NEED_ENERGY_CALC)
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
           qm_version(other.qm_version), zero_nrg(other.zero_nrg),
           rebuild_mm(other.rebuild_mm), ff_status(other.ff_status)
{
    //get the pointer from the base class...
    components_ptr = dynamic_cast<const MolproFF::Components*>( &(FFBase::components()) );
    BOOST_ASSERT( components_ptr != 0 );
}

/** Destructor */
MolproFF::~MolproFF()
{}

/** Register the components of this forcefield */
void MolproFF::registerComponents()
{
    std::auto_ptr<MolproFF::Components> ptr( new MolproFF::Components(*this) );

    FFBase::registerComponents(ptr.get());

    components_ptr = ptr.release();
}

/** Copy assignment function used by derived classes */
void MolproFF::_pvt_copy(const FFBase &ffbase)
{
    const MolproFF &other = dynamic_cast<const MolproFF&>(ffbase);

    spce = other.spce;
    switchfunc = other.switchfunc;
    molpro_exe = other.molpro_exe;
    molpro_tmpdir = other.molpro_tmpdir;
    qm_coords = other.qm_coords;
    mm_coords_and_charges = other.mm_coords_and_charges;
    qm_mols = other.qm_mols;
    mm_mols = other.mm_mols;
    qm_version = other.qm_version;
    zero_nrg = other.zero_nrg;
    rebuild_mm = other.rebuild_mm;
    ff_status = other.ff_status;

    components_ptr = dynamic_cast<const MolproFF::Components*>( &(FFBase::components()) );
    BOOST_ASSERT( components_ptr != 0 );
}

/** Tell the forcefield that it has to recalculate everything from
    scratch */
void MolproFF::mustNowRecalculateFromScratch()
{
    ff_status = NEED_REBUILD_ALL;
    rebuild_mm.clear();
    qm_version.increment();
}

/** Tell the forcefield that the QM array needs to be updated */
void MolproFF::mustNowUpdateQM()
{
    ff_status = ff_status | NEED_UPDATE_QM | NEED_REBUILD_MM | NEED_ENERGY_CALC;
    rebuild_mm.clear();
}

/** Tell the forcefield that is has to now rebuild the QM and MM arrays */
void MolproFF::mustNowRebuildQM()
{
    ff_status = ff_status | NEED_REBUILD_QM | NEED_REBUILD_MM | NEED_ENERGY_CALC;
    rebuild_mm.clear();
}

/** Tell the forcefield that we need to rebuild all of the
    MM array */
void MolproFF::mustNowRebuildMM()
{
    ff_status = ff_status | NEED_REBUILD_MM | NEED_ENERGY_CALC;
    rebuild_mm.clear();
}

/** Tell the forcefield that we need to update the
    MM molecule with ID == molid */
void MolproFF::mustNowUpdateMM(MoleculeID molid)
{
    if ( not (ff_status & NEED_REBUILD_MM) )
    {
        ff_status = ff_status | NEED_UPDATE_MM | NEED_ENERGY_CALC;

        rebuild_mm.insert(molid);

        if (rebuild_mm.count() == mm_mols.count())
        {
            //all of the MM molecules are being updated, so we might
            //as well rebuild the entire MM array
            rebuild_mm.clear();
            ff_status = ff_status | NEED_REBUILD_MM;
        }
    }
}

/** Whether or not we need to recalculate the energy */
bool MolproFF::needEnergyCalc() const
{
    return (ff_status & NEED_ENERGY_CALC);
}

/** Whether or not we need to rebuild the QM array */
bool MolproFF::needRebuildQM() const
{
    return (ff_status & NEED_REBUILD_QM);
}

/** Whether or not we need to rebuild all of the MM array */
bool MolproFF::needRebuildMM() const
{
    return (ff_status & NEED_REBUILD_MM);
}

/** Whether or not we need to update the QM array */
bool MolproFF::needUpdateQM() const
{
    return (ff_status & NEED_UPDATE_QM);
}

/** Whether or not we need to update the MM array -
    returns the IDs of the molecules that need to be
    updated */
QSet<MoleculeID> MolproFF::needUpdateMM() const
{
    if (ff_status & NEED_REBUILD_MM)
    {
        return rebuild_mm;
    }
    else
        return QSet<MoleculeID>();
}

/** Set the Molpro executable to use to calculate the energy
     - this increments the major version number if this changes
     the Molpro executable */
bool MolproFF::setMolproExe(const QFileInfo &molpro)
{
    if (molpro_exe != molpro)
    {
        molpro_exe = molpro;
        this->incrementMajorVersion();
        qm_version.increment();
        this->mustNowRecalculateFromScratch();
    }

    return isDirty();
}

/** Change the temporary directory in which the molpro job is run */
bool MolproFF::setMolproTempDir(const QDir &tempdir)
{
    if (molpro_tmpdir != tempdir)
    {
        molpro_tmpdir = tempdir;
        qm_version.increment();
        this->incrementMajorVersion();
        this->mustNowRecalculateFromScratch();
    }

    return isDirty();
}

/** Set the origin of the energy scale - this is the absolute
    energy which corresponds to zero. The supplied energy
    must be in kcal mol, and the energies returned by
    this forcefield will be the QM energy minus this
    origin energy. The purpose of this is to bring
    the QM energies down to the same sort of magnitude
    as the other MM energies, thereby minimising numerical
    errors when calculating energy differences. */
bool MolproFF::setEnergyOrigin(double nrg)
{
    //what is the change in origin?
    double delta = (zero_nrg * hartree) - nrg;

    //add this change onto the current energy
    if (not isZero(delta))
    {
        //we need to know if this forcefield is already dirty
        bool is_already_dirty = this->isDirty();

        //we need to increment the minor version as the forcefield
        //is undergoing a change that will alter the energy. This
        //will set the forcefield as dirty if it isn't already
        this->incrementMinorVersion();

        this->changeComponent( components().total(), delta );
        this->changeComponent( components().qm(), delta );

        //now save the new zero energy in hartrees
        zero_nrg = convertTo(nrg, hartree);

        //if the forcefield was originally clean, then we need to
        //restore the clean status
        if (not is_already_dirty)
            this->setClean();
    }

    return isDirty();
}

/** Return the zero energy of the forcefield, in internal
    units (kcal mol-1) */
double MolproFF::energyOrigin() const
{
    return zero_nrg * hartree;
}

/** Set the space within which the molecules exist */
bool MolproFF::setSpace(const Space &space)
{
    if (spce != space)
    {
        spce = space;
        this->incrementMajorVersion();
        this->mustNowRecalculateFromScratch();
    }

    return isDirty();
}

/** Set the switching function used to evaluate the electrostatic
    cutoff between the QM and MM atoms */
bool MolproFF::setSwitchingFunction(const SwitchingFunction &switchingfunction)
{
    if (switchfunc != switchingfunction)
    {
        switchfunc = switchingfunction;
        this->incrementMajorVersion();
        this->mustNowRecalculateFromScratch();
    }

    return isDirty();
}

/** Set the property 'name' to the value 'value'. This
    returns whether or not this changes the forcefield,
    and therefore the energy of the forcefield will need
    to be recalculated

    Note that you can only set pre-defined properties
    of forcefields - an exception will be thrown if
    you try to set the value of a property that does
    not exist in this forcefield.

    \throw SireBase::missing_property
*/
bool MolproFF::setProperty(const QString &name, const Property &value)
{
    if ( name == QLatin1String("space") )
    {
        this->setSpace(value);
        return this->isDirty();
    }
    else if ( name == QLatin1String("switching function") )
    {
        this->setSwitchingFunction(value);
        return this->isDirty();
    }
    else if ( name == QLatin1String("energy origin") )
    {
        if (not value.isA<VariantProperty>())
            throw SireError::invalid_cast( QObject::tr(
                "You must set the energy origin via a double (which will "
                "implicitly convert to a VariantProperty). You cannot "
                "set the energy origin from a %1!").arg(value.what()),
                      CODELOC )

        bool ok;

        double nrg = value.asA<VariantProperty>().toDouble(&ok);

        if (not ok)
            throw SireError::invalid_cast( QObject::tr(
                "You must set the energy origin to a number!"),
                    CODELOC );

        this->setEnergyOrigin(nrg);

        return this->isDirty();
    }
    else if ( name == QLatin1String("molpro") )
    {
        if (not value.isA<VariantProperty>())
            throw SireError::invalid_cast( QObject::tr(
                "You must set location of the Molpro executable "
                "via a string or file object (which will "
                "implicitly convert to a VariantProperty). You cannot "
                "set the energy origin from a %1!").arg(value.what()),
                      CODELOC )

        bool ok;

        const VariantProperty &varprop = value.asA<VariantProperty>();

        if (varprop.canConvert<QFileInfo>())
            this->setMolproExe( varprop.value<QFileInfo>() );
        else if (varprop.canConvert<QFile>())
            this->setMolproExe( QFileInfo( varprop.value<QFile>() ) );
        else if (varprop.canConvert<QString>())
            this->setMolproExe( QFileInfo( varprop.value<QString>() ) );
        else
            throw SireError::invalid_cast( QObject::tr(
                "You must set the Molpro executable to a valid file path!"),
                    CODELOC );

        return this->isDirty();
    }
    else if ( name == QLatin1String("molpro temporary directory") )
    {
        if (not value.isA<VariantProperty>())
            throw SireError::invalid_cast( QObject::tr(
                "You must set location of the Molpro temporary directory "
                "via a string or file object (which will "
                "implicitly convert to a VariantProperty). You cannot "
                "set the energy origin from a %1!").arg(value.what()),
                      CODELOC )

        bool ok;

        const VariantProperty &varprop = value.asA<VariantProperty>();

        if (varprop.canConvert<QFileInfo>())
            this->setMolproTempDir( varprop.value<QFileInfo>() );
        else if (varprop.canConvert<QDir>())
            this->setMolproTempDir( QFileInfo( varprop.value<QDir>() ) );
        else if (varprop.canConvert<QString>())
            this->setMolproTempDir( QFileInfo( varprop.value<QString>() ) );
        else
            throw SireError::invalid_cast( QObject::tr(
                "You must set the Molpro temporary directory to a "
                "valid directory path!"),
                    CODELOC );

        return this->isDirty();
    }
    else
        return FFBase::setProperty(name, value);
}

/** Return the property associated with the name 'name'

    \throw SireBase::missing_property
*/
Property MolproFF::getProperty(const QString &name) const
{
    if ( name == QLatin1String("space") )
    {
        return this->space();
    }
    else if ( name == QLatin1String("switching function") )
    {
        return this->switchingFunction();
    }
    else if ( name == QLatin1String("energy origin") )
    {
        return VariantProperty(this->energyOrigin());
    }
    else if ( name == QLatin1String("molpro") )
    {
        return VariantProperty(this->molproExe());
    }
    else if ( name == QLatin1String("molpro temporary directory") )
    {
        return VariantProperty(this->molproTempDir());
    }
    else
        return FFBase::getProperty(name);
}

/** Return whether or not this contains a property with the name 'name' */
bool MolproFF::containsProperty(const QString &name) const
{
    return ( name == QLatin1String("space") ) or
           ( name == QLatin1String("switching function") ) or
           ( name == QLatin1String("energy origin") ) or
           ( name == QLatin1String("molpro") ) or
           ( name == QLatin1String("molpro temporary directory") ) or
           FFBase::containsProperty(name);
}

/** Return all of the properties of this forcefield, indexed by name */
QHash<QString,Property> MolproFF::properties() const
{
    QHash<QString,Property> props;

    props.insert( QLatin1String("space"), this->space() );
    props.insert( QLatin1String("switching function"), this->switchingFunction() );
    props.insert( QLatin1String("energy origin"),
                    VariantProperty(this->energyOrigin()) );
    props.insert( QLatin1String("molpro"),
                    VariantProperty(this->molproExe()) );
    props.insert( QLatin1String("molpro temporary directory"),
                    VariantProperty(this->molproTempDir()) );

    props.unite( FFBase::properties() );

    return props;
}

/** Add a molecule to the QM region

    \throw SireMol::duplicate_molecule
*/
bool MolproFF::_pvt_addToQM(const PartialMolecule &molecule,
                            const ParameterMap &map)
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
        if ( qm_mols[molid].add(molecule) )
        {
            this->mustNowRecalculateFromScratch();
            return true;
        }
        else
            return false;
    }
    else
    {
        //create a QM molecule to represent this molecule
        MolproFF::QMMolecule qmmol(molecule);

        //save the qm molecule, indexed by its ID
        qm_mols.insert(molid, qmmol);
        this->mustNowRecalculateFromScratch();

        return true
    }
}

/** Add a molecule to the MM region.

    \throw SireMol::duplicate_molecule
    \throw SireError::incompatible_error
*/
int MolproFF::_pvt_addToMM(const PartialMolecule &molecule,
                           const ParameterMap &map)
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
        PartialMolecule &mm_mol = mm_mols[molid];

        if (mm_mol.add(molecule,
                       map.source(parameters().coulomb())))
        {
            if (mm_mol.nAtomsInArray() > 0)
            {
                //this molecule was in the array - must rebuild the MM array
                //now from scratch
                this->mustNowRebuildMM();
            }
            else
                //we only need to check if this molecule is now going to
                //be in the array
                this->mustNowRebuildMM(molid);

            return true;
        }
        else
            return false;
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

        this->mustNowRebuildMM(molid);
        return true;
    }
}

/** Add a molecule to the QM region

    \throw SireMol::duplicate_molecule
*/
bool MolproFF::addToQM(const PartialMolecule &molecule,
                       const ParameterMap &map)
{
    if (this->_pvt_addToQM(molecule,map))
    {
        this->incrementMajorVersion();
    }

    return isDirty();
}

/** Add a load of molecules to the QM region

    \throw SireMol::duplicate_molecule
*/
bool MolproFF::addToQM(const QList<PartialMolecule> &molecules,
                       const ParameterMap &map)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->addToQM(molecules.first(),map);
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        ForceField ffield(*this);
        MolproFF &copy = ffield.asA<MolproFF>();

        bool changed = false;

        for (QList<Molecule>::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            bool this_added = copy._pvt_addToQM(*it, map);

            added = added or this_added;
        }

        if (added)
        {
            copy.incrementMajorVersion();

            //everything's ok - copy back to the original
            *this = ffield;
        }

        return isDirty();
    }
}

/** Add a molecule to the MM region.

    \throw SireMol::duplicate_molecule
*/
bool MolproFF::addToMM(const PartialMolecule &molecule,
                       const ParameterMap &map)
{
    if (this->_pvt_addToMM(molecule, map))
    {
        this->incrementMajorVersion();
    }

    return isDirty();
}

/** Add a load of molecules to the MM region

    \throw SireMol::duplicate_molecule
*/
bool MolproFF::addToMM(const QList<PartialMolecule> &molecules,
                       const ParameterMap &map)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->addToMM(molecules.first(),map);
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        ForceField ffield(*this);
        MolproFF &copy = ffield.asA<MolproFF>();

        bool changed = false;

        for (QList<Molecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool this_added = copy._pvt_addToMM(*it, map);

            added = added or this_added;
        }

        if (added)
        {
            copy.incrementMajorVersion();

            //everything's ok - copy back to the original
            *this = ffield;
        }

        return isDirty();
    }
}

/** Add a molecule to the forcefield - this molecule must already
    exist in this forcefield

    \throw SireMol::missing_molecule
*/
bool MolProFF::add(const PartialMolecule &molecule,
                   const ParameterMap &map)
{
    if (qm_mols.contains(molecule.ID()))
        return this->addToQM(molecule,map);
    else if (mm_mols.contains(molecule.ID()))
        return this->addToMM(molecule,map);
    else
    {
        throw SireMol::missing_molecule( QObject::tr(
            "There is no molecule with ID == %1 in this forcefield.")
                .arg(molecule.ID()), CODELOC );

        return false;
    }
}

/** Add lots of molecules to this forcefield - they must all already
    exist in this forcefield

    \throw SireMol::missing_molecule
*/
bool MolproFF::add(const QList<PartialMolecule> &molecules,
                   const ParameterMap &map)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->add(molecules.first(),map);
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        ForceField ffield(*this);
        MolproFF &copy = ffield.asA<MolproFF>();

        bool changed = false;

        for (QList<PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool mol_changed = false;

            if (copy.qm_mols.contains(it->ID()))
                mol_changed = copy._pvt_addToQM(*it, map);
            else if (copy.mm_mols.contains(it->ID()))
                mol_changed = copy._pvt_addToMM(*it, map);
            else
                throw SireMol::missing_molecule( QObject::tr(
                    "There is no molecule with ID == %1 in this forcefield.")
                        .arg(molecule.ID()), CODELOC );

            changed = changed or mol_changed;
        }

        if (changed)
        {
            copy.incrementMajorVersion();
            *this = ffield;
        }

        return isDirty();
    }
}

/** Add the molecule 'molecule' to the group 'group'

    \throw SireFF::missing_group
*/
bool MolproFF::addTo(const FFBase::Group &group, const PartialMolecule &molecule,
                     const ParameterMap &map)
{
    if (group == groups().qm())
        return this->addToQM(molecule,map);
    else if (group == groups().mm())
        return this->addToMM(molecule,map);
    else
    {
        throw SireFF::missing_group( QObject::tr(
            "There is no group in the MolproFF forcefield."),
                CODELOC );

        return isDirty();
    }
}

/** Add the molecules in 'molecules' to the group 'group'

    \throw SireFF::missing_group
*/
bool MolproFF::addTo(const FFBase::Group &group,
                     const QList<PartialMolecule> &molecules,
                     const ParameterMap &map)
{
    if (group == groups().qm())
        return this->addToQM(molecules,map);
    else if (group == groups().mm())
        return this->addToMM(molecules,map);
    else
    {
        throw SireFF::missing_group( QObject::tr(
            "There is no group in the MolproFF forcefield."),
                CODELOC );

        return isDirty();
    }
}

/** Change the molecule 'molecule' */
bool MolproFF::_pvt_change(const PartialMolecule &molecule)
{
    MoleculeID molid = molecule.ID();

    if (qm_mols.contains(molid))
    {
        //the object exists in the QM region - make the change
        if (qm_mols[molid].change(molecule))
        {
            //the molecule changed, so we need to rebuild
            //all of the arrays...
            this->mustNowRecalulateQM();

            return true;
        }
    }
    else if (mm_mols.contains(molid))
    {
        //the object exists in the MM region
        if (mm_mols[molid].change(molecule))
        {
            this->mustNowRecalculateMM(molid);
            return true;
        }
    }

    return false;
}

/** Change the molecule 'molecule'. Does nothing if this
    molecule is not in this forcefield. */
bool MolproFF::change(const PartialMolecule &molecule)
{
    if (this->_pvt_change(molecule))
        this->incrementMinorVersion();

    return isDirty();
}

/** Change a whole load of molecules */
bool MolproFF::change(const QList<PartialMolecule> &molecules)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->change(molecules.first());
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        ForceField ffield(*this);
        MolproFF &copy = ffield.asA<MolproFF>();

        bool changed = false;

        for (QHash<MoleculeID,PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool this_changed = copy._pvt_change(it.value());

            changed = changed or this_changed;
        }

        if (changed)
        {
            copy.incrementMinorVersion();
            *this = ffield;
        }

        return isDirty();
    }
}

/** Change a whole load of molecules */
bool MolproFF::change(const QHash<MoleculeID,PartialMolecule> &molecules)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->change( *(molecules.begin()) );
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        ForceField ffield(*this);
        MolproFF &copy = ffield.asA<MolproFF>();

        bool changed = false;

        for (QHash<MoleculeID,PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool this_changed = copy._pvt_change(it.value());

            changed = changed or this_changed;
        }

        if (changed)
        {
            copy.incrementMinorVersion();
            *this = ffield;
        }

        return isDirty();
    }
}

bool MolproFF::_pvt_removeFromQM(const PartialMolecule &molecule)
{
    MoleculeID molid = molecule.ID();

    if (qm_mols.contains(molid))
    {
        PartialMolecule &qm_mol = qm_mols[molid];

        if (qm_mol.remove(molecule.selectedAtoms()))
        {
            if (qm_mol.selectedNone())
                //the entire molecule has been removed
                qm_mols.remove(molid);

            this->mustNowRecalculateFromScratch();

            return true;
        }
    }

    return false;
}

bool MolproFF::_pvt_removeFromMM(const PartialMolecule &molecule)
{
    MoleculeID molid = molecule.ID();

    if (mm_mols.contains(molid))
    {
        PartialMolecule &mm_mol = mm_mols[molid];

        if (mm_mol.remove(molecule.selectedAtoms()))
        {
            if (mm_mol.selectedNone())
                //the entire molecule has been removed
                mm_mols.remove(molid);

            if (mm_mol.nAtomsInArray() > 0)
            {
                //we need to rebuild everything as this atom
                //was in the MM array
                this->mustNowRebuildMM();
                return true;
            }
        }
    }

    return false;
}

/** Remove the molecule 'molecule' from the QM region. This
    does nothing if this molecule is not in this region. */
bool MolproFF::removeFromQM(const PartialMolecule &molecule)
{
    if (this->_pvt_removeFromQM(molecule))
        this->incrementMajorVersion();

    return isDirty();
}

/** Remove the molecule 'molecule' from the MM region. This
    does nothing if this molecule is not in this region. */
bool MolproFF::removeFromMM(const PartialMolecule &molecule)
{
    if (this->_pvt_removeFromMM(molecule))
        this->incrementMajorVersion();

    return isDirty();
}

/** Remove the molecules in 'molecules' from the QM region */
bool MolproFF::removeFromQM(const QList<PartialMolecule> &molecules)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->removeFromQM(molecules.first());
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        ForceField ffield(*this);
        MolproFF &copy = ffield.asA<MolproFF>();

        bool changed = false;

        for (QList<PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool removed = copy._pvt_removeFromQM(*it);
            changed = changed or removed;
        }

        if (changed)
        {
            copy.incrementMajorVersion();
            *this = ffield;
        }

        return isDirty();
    }
}

/** Remove the molecules in 'molecules' from the MM region */
bool MolproFF::removeFromQM(const QList<PartialMolecule> &molecules)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->removeFromMM(molecules.first());
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        ForceField ffield(*this);
        MolproFF &copy = ffield.asA<MolproFF>();

        bool changed = false;

        for (QList<PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool removed = copy._pvt_removeFromMM(*it);
            changed = changed or removed;
        }

        if (changed)
        {
            copy.incrementMajorVersion();
            *this = ffield;
        }

        return isDirty();
    }
}

bool MolproFF::_pvt_remove(const PartialMolecule &molecule)
{
    bool removed_from_qm = this->_pvt_removeFromQM(molecule);
    bool removed_from_mm = this->_pvt_removeFromMM(molecule);

    return removed_from_qm or removed_from_mm;
}

/** Remove the molecule 'mol'. Does nothing if this
    molecule is not in this forcefield */
bool MolproFF::remove(const PartialMolecule &molecule)
{
    if (this->_pvt_remove(molecule))
    {
        this->incrementMajorVersion();
    }

    return isDirty();
}

/** Remove all of the molecules in 'molecules' from this forcefield */
bool MolproFF::remove(const QList<PartialMolecule> &molecules)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->remove(molecules.first());
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        ForceField ffield(*this);
        MolproFF &copy = ffield.asA<MolproFF>();

        bool changed = false;

        for (QList<PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool removed = copy._pvt_remove(*it);
            changed = changed or removed;
        }

        if (changed)
        {
            copy.incrementMajorVersion();
            *this = ffield;
        }

        return isDirty();
    }
}

/** Completely rebuild the QM CoordGroup which is used to work out
    which MM atoms are within the non-bonded cutoff */
void MolproFF::rebuildQMCoordGroup()
{
    if (qm_coords.isEmpty() or qm_mols.isEmpty())
    {
        //there are no QM molecules!
        qm_coordgroup = CoordGroup();
    }
    else if (qm_mols.count() == 1 and
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

/** Completely rebuild the QM coordinates array from scratch */
void MolproFF::rebuildQMArray()
{
    if (qm_mols.isEmpty())
    {
        //there are no QM molecules!
        qm_coords.clear();
    }
    else if (qm_mols.count() == 1)
    {
        const QMMolecule &qm_mol = *(qm_mols.constBegin());

        int nats = qm_mol.nAtomsInArray();

        if (nats > 0)
        {
            qm_coords.resize( 3*nats );
            qm_mol.update(qm_coords, 0);
        }
        else
            qm_coords.clear();
    }
    else
    {
        //work out how many atoms should be in the array...
        int nats = 0;

        for (QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.constBegin();
             it != qm_mols.constEnd();
             ++it)
        {
            nats += it->nAtomsInArray();
        }

        if (nats > 0)
        {
            //resize the array for this number of atoms (3 times as x,y,z coordinates)
            qm_coords.resize( 3*nats );

            //now tell each QM molecule to place its coordinates into this array
            int i = 0;

            for (QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.constBegin();
                 it != qm_mols.constEnd();
                 ++it)
            {
                i = it->update(qm_coords, i);
            }
        }
        else
            qm_coords.clear();
    }

    //rebuild the QM CoordGroup
    this->rebuildQMCoordGroup();

    //we don't need to update nor rebuild the QM array
    this->clearStatus( NEED_REBUILD_QM | NEED_UPDATE_QM );
}

/** Update the existing QM coordinates array */
void MolproFF::updateQMArray()
{
    if (not qm_coords.isEmpty())
    {
        //tell each QM molecule to place its new coordinates into the
        //existing array
        int i = 0;

        for (QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.constBegin();
             it != qm_mols.constEnd();
             ++it)
        {
            i = it->update(qm_coords, i);
        }

        //rebuild the QM CoordGroup
        this->rebuildQMCoordGroup();
    }

    //we don't need to update the QM array
    this->clearStatus( NEED_UPDATE_QM );
}

/** Rebuild the MM coordinates and charges array */
void MolproFF::rebuildMMArray()
{
    if (mm_mols.count() == 0 or qm_coords.isEmpty() or qm_coordgroup.isEmpty())
    {
        mm_coords_and_charges.clear();
        mm_array_index.clear();
    }
    else
    {
        //run through each molecule and tell it where
        //the QM atoms are - this allows the molecule to
        //see whether or not it is within the cutoff
        int nats = 0;

        for (QHash<MoleculeID,MMMolecule>::iterator it = mm_mols.begin();
             it != mm_mols.end();
            ++it)
        {
            nats += it->update(qm_coordgroup, this->space(),
                               this->switchingFunction());
        }

        //resize the MM array (four times as x,y,z + charge)
        mm_coords_and_charges.resize( 4 * nats );

        //add all of the MM atoms to this array - recording the index
        //of the first atom for each molecule
        int i = 0;

        mm_array_index.clear();
        mm_array_index.reserve(mm_mols.count());

        for (QHash<MoleculeID,MMMolecule>::const_iterator it = mm_mols.constBegin();
             it != mm_mols.constEnd();
             ++it)
        {
            mm_array_index.insert(it.key(),i);
            i = it->update(mm_coords_and_charges, i);
        }
    }

    this->clearStatus( NEED_REBUILD_MM | NEED_UPDATE_MM );
    rebuild_mm.clear();
}

/** Update the MM arrays */
void MolproFF::updateMMArray()
{
    if (rebuild_mm.isEmpty() or mm_mols.isEmpty() or
        qm_coords.isEmpty() or qm_coordgroup.isEmpty())
    {
        this->clearStatus( NEED_REBUILD_MM );
        return;
    }

    //update the MM array for the specified Molecules in rebuild_mm

    int n_additional_atoms = 0;

    bool something_changed = false;

    //update them all and see if there has been any change in the number
    //of atoms that need to be in the MM region
    foreach (MoleculeID molid, rebuild_mm)
    {
        BOOST_ASSERT( mm_mols.contains(molid) );

        MMMolecule &mm_mol = mm_mols[molid];

        //see if the number of atoms in the array changes
        int old_nats = mm_mol.nAtomsInArray();

        int new_nats = mm_mol.update(qm_coordgroup);

        if (new_nats != old_nats)
        {
            if (old_nats == 0)
            {
                //this molecule must be added
                n_additional_atoms += new_nats;
            }
            else
            {
                //some MM atoms have been removed - just rebuild the
                //entire MM array from scratch
                this->rebuildMMArray();
                return;
            }
        }
        else if (new_nats != 0)
        {
            //there were some atoms in the array before, and still some
            //atoms - this means that moving this molecule has changed
            //the QM/MM calculation
            something_changed = true;
        }
    }

    if (something_changed)
    {
        //some atoms within the cutoff of the QM region have
        //changed - we must therefore rebuild the MM array
        //and we will need to recalculate the QM/MM energy

        if (n_additional_atoms != 0)
        {
            //how big is the array now?
            int i = mm_coords_and_charges.count();

            mm_coords_and_charges.resize( i + 4*n_additional_atoms );

            //add or update the molecules
            foreach (MoleculeID molid, rebuild_mm)
            {
                BOOST_ASSERT( mm_mols.contains(molid) );

                const MMMolecule &mm_mol = *(mm_mols.constFind(molid));

                QHash<MoleculeID,quint32>::const_iterator it =
                                              mm_array_index.constFind(molid);

                if (it != mm_array_index.constEnd())
                    //this molecule is exists in the array and is being updated
                    mm_mol.update(mm_coords_and_charges, *it);
                else
                {
                    //this molecule is being added to the array
                    mm_array_index.insert(molid, i);
                    i = mm_mol.update(mm_coords_and_charges, i);
                }
            }
        }
        else
        {
            //there is no change in the number of atoms from each molecule
            foreach (MoleculeID molid, rebuild_mm)
            {
                BOOST_ASSERT( mm_mols.contains(molid) );

                const MMMolecule &mm_mol = *(mm_mols.constFind(molid));

                QHash<MoleculeID,quint32>::const_iterator it =
                                              mm_array_index.constFind(molid);

                BOOST_ASSERT( it != mm_array_index.constEnd() );

                mm_mol.update(mm_coords_and_charges, *it);
            }
        }
    }
    else
    {
        //nothing in the QM cutoff region changed - there
        //is therefore no need to recalculate the QM/MM energy
        ff_status = UP2DATE;
    }

    this->clearStatus( NEED_UPDATE_MM );
    rebuild_mm.clear();
}

/** Update the QM and MM arrays. */
void MolproFF::updateArrays()
{
    if (this->needRebuildQM())
    {
        this->rebuildQMArray();
        this->rebuildMMArray();
    }
    else if (this->needUpdateQM())
    {
        this->updateQMArray();
        this->rebuildMMArray();
    }
    else if (this->needRebuildMM())
    {
        this->rebuildMMArray();
    }
    else
    {
        QSet<MoleculeID> molids = this->needUpdateMM();

        if (not molids.isEmpty())
            this->updateMMArray(molids);
    }
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
int MolproFF::nMMAtomsInArray()
{
    //must rebuild the arrays to make sure this is right
    this->updateArrays();

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
int MolproFF::nAtomsInArray()
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

    //update the arrays - do this first as we may find out
    //that there is no need for an energy calculation
    this->updateArrays();

    //update the QM and MM arrays to account for any moves
    if (this->needEnergyCalc())
    {
        double qmmm_nrg = 0;

        //if there are no QM atoms, then there is no QM/MM energy!
        if (not qm_coords.isEmpty())
        {
            session.setArrays(qm_coords, mm_coords_and_charges);

            //calculate the energy of the system
            qmmm_nrg = session.calculateEnergy(this->energyCmdString());
        }

        qmmm_nrg = (qmmm_nrg - zero_nrg) * hartree;

        setComponent( components().total(), hf_nrg );
        setComponent( components().qm(), hf_nrg );

        ff_status = UP2DATE;
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
    //update the arrays - do this first as we may find out
    //that there is no need for an energy calculation
    this->updateArrays();

    if (this->needEnergyCalc())
    {
        double qmmm_nrg = 0;

        //if there are no QM atoms, then there is no QM/MM energy!
        if (not qm_coords.isEmpty())
        {
            //start a new molpro session for this forcefield
            //(this calculates the energy)
            MolproSession session(*this);

            //obtain the calculated energy from molpro
            qmmm_nrg = session.calculateEnergy(this->energyCmdString());
        }

        //subtract the zero energy and convert to kcal mol-1
        qmmm_nrg = (qmmm_nrg - zero_nrg) * hartree;

        //save the energy in the right components
        setComponent( components().total(), hf_nrg );
        setComponent( components().qm(), hf_nrg );

        //the forcefield is now completely clean
        ff_status = UP2DATE;

        //exiting destroys 'session', which closes the
        //Molpro process
    }
}
