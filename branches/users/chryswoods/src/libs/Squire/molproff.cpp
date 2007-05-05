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
#include "SireMol/molecules.h"

#include "SireMol/residue.h"
#include "SireMol/residueinfo.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"
#include "SireMol/element.h"
#include "SireMol/propertyextractor.h"
#include "SireMol/atomselector.h"

#include "SireMM/atomiccharges.h"

#include "SireUnits/convert.h"
#include "SireUnits/units.h"

#include "SireMol/errors.h"
#include "SireFF/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QFile>
#include <QDebug>

using namespace Squire;
using namespace SireMM;
using namespace SireFF;
using namespace SireMol;
using namespace SireMaths;
using namespace SireBase;
using namespace SireUnits;
using namespace SireStream;

Q_DECLARE_OPERATORS_FOR_FLAGS(Squire::MolproFF::QMMMStatus);

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
    int natoms = 0;

    //count up the number of non-dummy atoms
    int ngroups = elements.count();

    for (int i=0; i<ngroups; ++i)
    {
        const QVector<Element> &group_elements = elements.at(i);

        int nats = group_elements.count();
        const Element *elements_array = group_elements.constData();

        for (int j=0; j<nats; ++j)
        {
            if (elements_array[j].nProtons() > 0)
            {
                ++natoms;
            }
        }
    }

    return natoms;
}

/** Construct a QM molecule from the passed Molecule */
MolproFF::QMMolecule::QMMolecule(const PartialMolecule &molecule)
         : mol(molecule)
{
    coords = mol.extract().coordGroups();
    elements = mol.extract().elements();
    nats = countElements(elements);
}

/** Copy constructor */
MolproFF::QMMolecule::QMMolecule(const MolproFF::QMMolecule &other)
         : mol(other.mol), coords(other.coords),
           elements(other.elements), nats(other.nats)
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
const PartialMolecule& MolproFF::QMMolecule::molecule() const
{
    return mol;
}

/** Change the molecule - return whether it changed */
bool MolproFF::QMMolecule::change(const PartialMolecule &molecule)
{
    if (molecule.version() != mol.version())
    {
        //have the coordinates of the atoms changed?
        QVector<CoordGroup> new_coords = molecule.extract().coordGroups();

        bool something_changed = new_coords.count() != coords.count();

        if (not something_changed)
        {
            int ngroups = coords.count();
            const CoordGroup *old_coords_array = coords.constData();
            const CoordGroup *new_coords_array = new_coords.constData();

            if (old_coords_array != new_coords_array)
            {
                for (int i=0; i<ngroups; ++i)
                {
                    if (old_coords_array[i] != new_coords_array[i])
                    {
                        something_changed = true;
                        break;
                    }
                }
            }
        }

        mol = mol.change(molecule);
        coords = new_coords;

        return something_changed;
    }
    else
        return false;
}

/** Add parts to the molecule - record whether it changed */
bool MolproFF::QMMolecule::add(const PartialMolecule &molecule)
{
    if (mol.selectedAtoms().contains(molecule.selectedAtoms()))
        //there is nothing to add - maybe something to change
        return this->change(molecule);
    else
    {
        mol = mol.change(molecule).selection().add(molecule.selectedAtoms());

        coords = mol.extract().coordGroups();
        elements = mol.extract().elements();
        nats = countElements(elements);

        return true;
    }
}

/** Remove some atoms from the molecule */
bool MolproFF::QMMolecule::remove(const AtomSelection &selected_atoms)
{
    if (mol.selectedAtoms().intersects(selected_atoms))
    {
        mol = mol.selection().remove(selected_atoms);

        coords = mol.extract().coordGroups();
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

    int ngroups = coords.count();

    BOOST_ASSERT(ngroups == elements.count());

    const CoordGroup *groups_array = coords.constData();
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

    int ncg = coords.count();

    BOOST_ASSERT(ncg == elements.count());

    const CoordGroup *groups_array = coords.constData();
    const QVector<Element> *elements_array = elements.constData();

    double *qm_array_array = qm_array.data();
    int maxcoords = qm_array.count();

    for (int i=0; i<ncg; ++i)
    {
        int natms = groups_array[i].count();
        BOOST_ASSERT(natms == elements_array[i].count());

        const Vector *coords_array = groups_array[i].constData();
        const Element *elem_array = elements_array[i].constData();

        for (int j=0; j<natms; ++j)
        {
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
MolproFF::MMMolecule::MMMolecule(const PartialMolecule &molecule,
                                 const QString &charge_property)
         : mol(molecule), chg_property(charge_property),
           nats(0), rebuild_all(true)
{
    //get the atomic charges...
    chgs = mol.extract().property(chg_property);
    coords = mol.extract().coordGroups();
}

/** Copy constructor */
MolproFF::MMMolecule::MMMolecule(const MMMolecule &other)
         : mol(other.mol), chg_property(other.chg_property),
           coords(other.coords), chgs(other.chgs),
           coords_and_chgs(other.coords_and_chgs),
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
        coords = other.coords;
        chgs = other.chgs;
        coords_and_chgs = other.coords_and_chgs;
        cgids_to_be_rebuilt = other.cgids_to_be_rebuilt;
        nats = other.nats;
        rebuild_all = other.rebuild_all;
    }

    return *this;
}

/** Return the actual molecule */
const PartialMolecule& MolproFF::MMMolecule::molecule() const
{
    return mol;
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

/** Change the molecule */
bool MolproFF::MMMolecule::change(const PartialMolecule &molecule,
                                  const QString &chgproperty)
{
    if (rebuild_all or mol.selectedAtoms().isEmpty())
    {
        //just update the molecule
        MoleculeVersion oldversion = mol.version();

        mol = mol.change(molecule);
        coords = mol.extract().coordGroups();

        if ( not chgproperty.isNull() and chgproperty != chg_property )
        {
            chg_property = chgproperty;
            chgs = mol.extract().property(chg_property);
        }
        else if (oldversion.major() != molecule.version().major())
        {
            chgs = mol.extract().property(chg_property);
        }

        return true;
    }

    //save the old coordinates and charges
    QVector<CoordGroup> old_coords = coords;
    AtomicCharges old_chgs = chgs;

    //record whether there has been any change
    bool changed = false;
    bool major_change = mol.version().major() != molecule.version().major();

    if ( not chgproperty.isNull() and chgproperty != chg_property )
    {
        chg_property = chgproperty;
        major_change = true;
    }

    //update the molecule, coords and charges
    if (mol.version() != molecule.version())
    {
        mol = mol.change(molecule);
        coords = mol.extract().coordGroups();
        changed = true;
    }

    if (major_change)
    {
        chgs = mol.extract().property(chg_property);
        changed = true;
    }

    if (not changed)
        //nothing at all has changed
        return rebuild_all or not cgids_to_be_rebuilt.isEmpty();

    //lets see what has changed...
    uint ngroups = mol.info().nCutGroups();
    int ngroups_minus_one = ngroups - 1;

    //get raw pointers to the old and new coordinate and charge arrays
    const CoordGroup *old_coords_array = old_coords.constData();
    const CoordGroup *new_coords_array = coords.constData();

    const QVector<ChargeParameter> *old_chgs_array = old_chgs.constData();
    const QVector<ChargeParameter> *new_chgs_array = chgs.constData();

    if (mol.selectedAtoms().selectedAllCutGroups())
    {
        //this is the easy case - all CutGroups have been selected,
        //so the coords and charges arrays are for all groups, and
        //are ordered by CutGroupID

        for (CutGroupID i(0); i<ngroups; ++i)
        {
            if ( (not cgids_to_be_rebuilt.contains(i)) and
                 (
                    ( old_coords_array != new_coords_array and
                      old_coords_array[i] != new_coords_array[i] ) or

                    ( old_chgs_array != new_chgs_array and
                      old_chgs_array[i] != new_chgs_array[i] )
                 )
               )
            {
                //there is some difference with this CutGroup
                if (cgids_to_be_rebuilt.count() == ngroups_minus_one)
                {
                    //all of the CutGroups have changed
                    //therefore the entire molecule has changed
                    rebuild_all = true;
                    cgids_to_be_rebuilt.clear();
                    break;
                }

                cgids_to_be_rebuilt.insert(i);
                changed = true;
            }
        }
    }
    else
    {
        //this is the harder case - only some of the CutGroups have been
        //selected - get the ones that have...
        QHash<CutGroupID,quint32> cg_index = mol.extract().cutGroupIndex();

        for (QHash<CutGroupID,quint32>::const_iterator it = cg_index.constBegin();
             it != cg_index.constEnd();
             ++it)
        {
            CutGroupID cgid = it.key();
            int i = *it;

            if ( (not cgids_to_be_rebuilt.contains(cgid)) and
                 (
                    ( old_coords_array != new_coords_array and
                      old_coords_array[i] != new_coords_array[i] ) or

                    ( old_chgs_array != new_chgs_array and
                      old_chgs_array[i] != new_chgs_array[i] )
                 )
               )
            {
                //there is some difference with this CutGroup
                if (cgids_to_be_rebuilt.count() == ngroups_minus_one)
                {
                    //all of the CutGroups have changed
                    //therefore the entire molecule has changed
                    rebuild_all = true;
                    cgids_to_be_rebuilt.clear();
                    break;
                }

                cgids_to_be_rebuilt.insert(cgid);
                changed = true;
            }
        }
    }

    return changed or rebuild_all or not cgids_to_be_rebuilt.isEmpty();
}

/** Add parts to this molecule */
bool MolproFF::MMMolecule::add(const PartialMolecule &molecule,
                               const QString &chgproperty)
{
    //first see if the molecule needs to be changed...
    bool changed = this->change(molecule,chgproperty);

    if ( not mol.selectedAtoms().contains(molecule.selectedAtoms()) )
    {
        mol = mol.selection().add(molecule.selectedAtoms());

        coords = mol.extract().coordGroups();
        chgs = mol.extract().property(chg_property);

        if (not rebuild_all)
        {
            cgids_to_be_rebuilt.unite( molecule.selectedAtoms().selectedCutGroups() );

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
    if (mol.selectedAtoms().intersects(selected_atoms))
    {
        mol = mol.selection().remove(selected_atoms);

        coords = mol.extract().coordGroups();
        chgs = mol.extract().property(chg_property);

        if (not rebuild_all)
        {
            cgids_to_be_rebuilt.unite(selected_atoms.selectedCutGroups());

            if (cgids_to_be_rebuilt.count() == mol.info().nCutGroups())
            {
                rebuild_all = true;
                cgids_to_be_rebuilt.clear();
            }
        }

        return true;
    }
    else
        return rebuild_all or not cgids_to_be_rebuilt.isEmpty();
}

/** Return a list of all of the copies of 'group' that are close to 'center',
    together with the non-bonded scale factor for that group */
static QList< tuple<double,CoordGroup> >
getCloseGroups(const CoordGroup &group, const CoordGroup &center,
               const Space &space, const SwitchingFunction &switchfunc)
{
    QList< tuple<double,CoordGroup> > groups
                    = space.getCopiesWithin(group, center,
                                            switchfunc.cutoffDistance());

    //loop through each copy and set its scale factor
    QMutableListIterator< tuple<double,CoordGroup> > it(groups);

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

    return groups;
}

/** Count the number of non-zero charges in the array of charges */
static int nCharges(const QVector<ChargeParameter> &chgs)
{
    int nchgs = 0;

    int n = chgs.count();
    const ChargeParameter *chgs_array = chgs.constData();

    for (int i=0; i<n; ++i)
    {
        if (chgs_array[i] != 0)
            ++nchgs;
    }

    return nchgs;
}

/** Convert the list of CoordGroup copies and charges into a single array
    containing the coordinates and charges together */
static QVector<double>
getCoordsAndCharges( const QList< tuple<double,CoordGroup> > &close_groups,
                     const QVector<ChargeParameter> &group_chgs )
{
    if (close_groups.isEmpty())
        return QVector<double>();

    //get the indicies of groups that have a non-zero charge
    QHash<int, ChargeParameter> non_zero_charges;

    int nchgs = group_chgs.count();
    const ChargeParameter *group_chgs_array = group_chgs.constData();

    for (int i=0; i<nchgs; ++i)
    {
        const ChargeParameter &chg = group_chgs_array[i];

        if (chg != 0)
            non_zero_charges.insert(i, chg);
    }

    //count the number of atoms - equals the number of copies of the
    //group times the number atoms in the group that have a non-zero charge
    int nats = close_groups.count() * non_zero_charges.count();

    if (nats == 0)
        return QVector<double>();

    //reserve sufficient space for the coordinates and charges
    QVector<double> coords_and_chgs(4 * nats);

    double *coords_and_chgs_array = coords_and_chgs.data();

    int i = 0;

    for ( QList< tuple<double,CoordGroup> >::const_iterator it = close_groups.begin();
          it != close_groups.end();
          ++it )
    {
        double scale_fac = it->get<0>();
        const Vector *group_coords = it->get<1>().constData();

        for (QHash<int, ChargeParameter>::const_iterator
                                          it2 = non_zero_charges.constBegin();
             it2 != non_zero_charges.constEnd();
             ++it2)
        {
            int idx = it2.key();
            const Vector &coord = group_coords[idx];

            //store the x, y and z coordinates (in bohr radii)
            coords_and_chgs_array[i] = convertTo(coord.x(), bohr_radii);
            ++i;

            coords_and_chgs_array[i] = convertTo(coord.y(), bohr_radii);
            ++i;

            coords_and_chgs_array[i] = convertTo(coord.z(), bohr_radii);
            ++i;

            //also store the charge (in absolute electron charges)
            coords_and_chgs_array[i] = convertTo(scale_fac * it2->charge(),
                                                 mod_electrons);
            ++i;
        }
    }

    BOOST_ASSERT( i == 4*nats );

    return coords_and_chgs;
}

/** Update the current state of the CutGroup with ID == cgid to
    have the coordinates from 'group_coords', charges from 'group_chgs',
    and to be in the space 'space' with the QM atoms from 'qm_coordgroup',
    using the cutoff defined in 'switchfunc' */
void MolproFF::MMMolecule::updateGroup(CutGroupID cgid,
                                       const CoordGroup &group_coords,
                                       const QVector<ChargeParameter> &group_chgs,
                                       const CoordGroup &qm_coordgroup,
                                       const Space &space,
                                       const SwitchingFunction &switchfunc)
{
    //remove the curent entry for this CutGroup
    QVector<double> group_coords_and_chgs = coords_and_chgs.take(cgid);
    int old_nats_times_four = group_coords_and_chgs.count();

    //count up how many atoms have a non-zero charge
    int ncharges = nCharges(group_chgs);

    if (ncharges == 0 or qm_coordgroup.count() == 0)
    {
        //there are no QM or MM atoms
        nats -= (old_nats_times_four / 4);
        return;
    }

    //get copies of this CoordGroup that are within the cutoff distance
    //of the QM CoordGroup
    QList< tuple<double,CoordGroup> > close_groups
                               = getCloseGroups( group_coords,
                                                 qm_coordgroup,
                                                 space, switchfunc );

    //convert these groups into the array of coordinates and charges
    group_coords_and_chgs = getCoordsAndCharges( close_groups, group_chgs );

    int new_nats_times_four = group_coords_and_chgs.count();

    if (new_nats_times_four > 0)
    {
        //save these coordinates and charges for this group
        coords_and_chgs.insert(cgid, group_coords_and_chgs);
    }

    //update the number of atoms
    nats += ( (new_nats_times_four - old_nats_times_four) / 4 );
}

/** Update the MMMolecule - this finds all of the CoordGroups from
    this molecule that are within the cutoff of the coordgroup
    'qm_coordgroup', based on the space 'space' using the
    switching function 'switchfunc' */
int MolproFF::MMMolecule::update(const CoordGroup &qm_coordgroup,
                                 const Space &space,
                                 const SwitchingFunction &switchfunc,
                                 bool must_rebuild_all)
{
    if (coords.isEmpty())
    {
        //nothing to do with an empty molecule!
        coords_and_chgs.clear();
        nats = 0;
        return 0;
    }

    if (rebuild_all or must_rebuild_all)
    {
        //the entire MM molecule must be rebuilt...
        coords_and_chgs.clear();

        //zero the number of atoms of this molecule in the array...
        nats = 0;

        //loop over all of the CoordGroups in this molecule and
        //tell them to update their array of coords and charges
        if (mol.selectedAtoms().selectedAllCutGroups())
        {
            uint ncg = mol.info().nCutGroups();

            for (CutGroupID i(0); i<ncg; ++i)
            {
                this->updateGroup(i, coords.at(i),
                                     chgs.at(i),
                                     qm_coordgroup,
                                     space, switchfunc);
            }
        }
        else
        {
            //get the index that maps from CutGroupID to index...
            QHash<CutGroupID,quint32> cg_index = mol.extract().cutGroupIndex();

            //loop over all of the CoordGroups in this molecule and
            //tell them to update their array of coords and charges
            for (QHash<CutGroupID,quint32>::const_iterator it = cg_index.constBegin();
                 it != cg_index.constEnd();
                 ++it)
            {
                CutGroupID cgid = it.key();
                quint32 idx = *it;

                this->updateGroup(cgid, coords.at(idx),
                                        chgs.at(idx),
                                        qm_coordgroup,
                                        space, switchfunc);
            }
        }
    }
    else
    {
        if (mol.selectedAtoms().selectedAllCutGroups())
        {
            //loop over all of the CoordGroups in this molecule and
            //tell them to update their array of coords and charges
            foreach (CutGroupID cgid, cgids_to_be_rebuilt)
            {
                this->updateGroup(cgid, coords.at(cgid),
                                        chgs.at(cgid),
                                        qm_coordgroup,
                                        space, switchfunc);
            }
        }
        else
        {
            //get the mapping from CutGroupID to array index
            QHash<CutGroupID,quint32> cg_index = mol.extract().cutGroupIndex();

            //we need to rebuild the specified CoordGroups...
            foreach (CutGroupID cgid, cgids_to_be_rebuilt)
            {
                if (cg_index.contains(cgid))
                {
                    quint32 idx = cg_index.value(cgid);

                    this->updateGroup(cgid, coords.at(idx),
                                            chgs.at(idx),
                                            qm_coordgroup,
                                            space, switchfunc);
                }
                else
                {
                    //this CutGroup has been removed
                    QVector<double> gcoords = coords_and_chgs.take(cgid);

                    nats -= (gcoords.count() / 4);
                }
            }
        }
    }

    rebuild_all = false;
    cgids_to_be_rebuilt.clear();

    //return the number of atoms in the array
    return nats;
}

/** Update the mm_coords_and_charges array with the coordinates
    and charges of atoms in this molecule. This will either
    replace the existing information about this molecule in
    mm_coords_and_charges, or it will add its information
    onto the end of the array. */
int MolproFF::MMMolecule::update(QVector<double> &mm_coords_and_charges,
                                 int i) const
{
    if (nats == 0)
        //There is nothing to do :-)
        return i;

    int new_i = i + (4 * nats);

    //make sure that there is sufficient space in the array
    if (new_i > mm_coords_and_charges.count())
    {
        throw SireError::program_bug( QObject::tr(
            "There is insufficient space for this MM molecule in the "
            "mm_coords_and_charges array! (Have %1 but need %2) (%3, %4)")
                  .arg(mm_coords_and_charges.count())
                  .arg(new_i)
                  .arg(i).arg(nats), CODELOC );
    }

    //copy in each CutGroup in turn
    double *mm_coords_and_charges_array = mm_coords_and_charges.data();

    for (QHash< CutGroupID,QVector<double> >::const_iterator
                                            it = coords_and_chgs.constBegin();
         it != coords_and_chgs.constEnd();
         ++it)
    {
        double *start = &(mm_coords_and_charges_array[i]);

        void *output = qMemCopy(start, it->constData(), it->count() * sizeof(double));
        BOOST_ASSERT( output == start );

        i += it->count();
    }

    BOOST_ASSERT(i == new_i);

    return i;
}

/** Return a Molpro format string providing the coordinates and charges of atoms
    in this molecule that are within the electrostatic cutoff of the QM atoms. */
QString MolproFF::MMMolecule::coordAndChargesString() const
{
    if (nats == 0)
        //There is nothing to do :-)
        return QString::null;

    //create a string list containing each atom from coords_and_chgs
    //(which has the coordinates in bohr_radii and charges in mod electrons)
    QStringList lines;

    for (QHash< CutGroupID,QVector<double> >::const_iterator
                                            it = coords_and_chgs.constBegin();
         it != coords_and_chgs.constEnd();
         ++it)
    {
        const QVector<double> &group_coords_chgs = it.value();

        int ngroup_atoms = group_coords_chgs.count();
        const double *group_coords_chgs_array = group_coords_chgs.constData();

        for (int i=0; i<ngroup_atoms; i+=4)
        {
            QString atomline;
            QTextStream ts(&atomline, QIODevice::WriteOnly);

            ts.setRealNumberNotation(QTextStream::FixedNotation);
            ts.setRealNumberPrecision(10);

            ts << group_coords_chgs_array[i] << ", "
               << group_coords_chgs_array[i+1] << ", "
               << group_coords_chgs_array[i+2] << ", "
               << group_coords_chgs_array[i+3];

            lines.append(atomline);
        }
    }

    return lines.join("\n");
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

                qm_mols.insert(qm_mol.ID(), MolproFF::QMMolecule(qm_mol));
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

        //have to now do the entire calculation from scratch
        molproff.mustNowRecalculateFromScratch();

        //read in the base class...
        sds >> static_cast<FFBase&>(molproff);
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

/** Clear the status represented by 'stat' */
void MolproFF::clearStatus(QMMMStatus stat)
{
    //set ff_status to '0' wherever 'stat' is 1
    ff_status = ff_status & (~stat);
}

/** Whether or not we need to recalculate the energy */
bool MolproFF::needEnergyCalc() const
{
    #if QT_VERSION >= 0x040200
        return ff_status.testFlag(NEED_ENERGY_CALC);
    #else
        return (ff_status & NEED_ENERGY_CALC);
    #endif
}

/** Whether or not we need to rebuild the QM array */
bool MolproFF::needRebuildQM() const
{
    #if QT_VERSION >= 0x040200
        return ff_status.testFlag(NEED_REBUILD_QM);
    #else
        return (ff_status & NEED_REBUILD_QM);
    #endif
}

/** Whether or not we need to rebuild all of the MM array */
bool MolproFF::needRebuildMM() const
{
    #if QT_VERSION >= 0x040200
        return ff_status.testFlag(NEED_REBUILD_MM);
    #else
        return (ff_status & NEED_REBUILD_MM);
    #endif
}

/** Whether or not we need to update the QM array */
bool MolproFF::needUpdateQM() const
{
    #if QT_VERSION >= 0x040200
        return ff_status.testFlag(NEED_UPDATE_QM);
    #else
        return (ff_status & NEED_UPDATE_QM);
    #endif
}

/** Whether or not we need to update the MM array */
bool MolproFF::needUpdateMM() const
{
    #if QT_VERSION >= 0x040200
        return ff_status.testFlag(NEED_UPDATE_MM);
    #else
        return (ff_status & NEED_UPDATE_MM);
    #endif
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
                      CODELOC );

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
                      CODELOC );

        const VariantProperty &varprop = value.asA<VariantProperty>();

        if (varprop.canConvert<QString>())
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
                      CODELOC );

        const VariantProperty &varprop = value.asA<VariantProperty>();

        if (varprop.canConvert<QString>())
            this->setMolproTempDir( QDir( varprop.value<QString>() ) );
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
        return QVariant::fromValue(this->energyOrigin());
    }
    else if ( name == QLatin1String("molpro") )
    {
        return QVariant::fromValue(this->molproExe().absoluteFilePath());
    }
    else if ( name == QLatin1String("molpro temporary directory") )
    {
        return QVariant::fromValue(this->molproTempDir().absolutePath());
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
                    QVariant::fromValue(this->energyOrigin()) );
    props.insert( QLatin1String("molpro"),
                    QVariant::fromValue(this->molproExe().absoluteFilePath()) );
    props.insert( QLatin1String("molpro temporary directory"),
                    QVariant::fromValue(this->molproTempDir().absolutePath()) );

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

    //check that parts of this molecule are not in the MM region
    if (mm_mols.contains(molid))
    {
        if (mm_mols.constFind(molid)->molecule()
                        .selectedAtoms().contains(molecule.selectedAtoms()))
            throw SireMol::duplicate_molecule( QObject::tr(
                "Cannot add the molecule %1 to the QM region of the MolproFF "
                "\"%2\" (%3 %4) as parts of this molecule already exist "
                "in the MM region.")
                    .arg(molecule.ID()).arg(this->name())
                    .arg(this->ID()).arg(this->version().toString()),
                        CODELOC );
    }

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

        return true;
    }
}

/** Add a molecule to the MM region.

    \throw SireMol::duplicate_molecule
    \throw SireError::incompatible_error
*/
bool MolproFF::_pvt_addToMM(const PartialMolecule &molecule,
                            const ParameterMap &map)
{
    MoleculeID molid = molecule.ID();

    //check that parts of this molecule are not in the QM region
    if (qm_mols.contains(molid))
    {
        if (qm_mols.constFind(molid)->molecule()
                      .selectedAtoms().contains(molecule.selectedAtoms()))
            throw SireMol::duplicate_molecule( QObject::tr(
                "Cannot add the molecule %1 to the MM region of the MolproFF "
                "\"%2\" (%3 %4) as parts of this molecule already exist "
                "in the QM region.")
                    .arg(molecule.ID()).arg(this->name())
                    .arg(this->ID()).arg(this->version().toString()),
                        CODELOC );
    }

    //do we already contain this molecule?
    if (mm_mols.contains(molid))
    {
        MMMolecule &mm_mol = mm_mols[molid];

        if (mm_mol.add(molecule,
                       map.source(parameters().coulomb())))
        {
            this->mustNowUpdateMM(molid);
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

        this->mustNowUpdateMM(molid);
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
bool MolproFF::addToQM(const Molecules &molecules,
                       const ParameterMap &map)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->addToQM(*(molecules.begin()),map);
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        SharedPolyPointer<MolproFF> ffield(*this);
        MolproFF &copy = *ffield;

        bool changed = false;

        for (Molecules::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            bool added = copy._pvt_addToQM(*it, map);

            changed = changed or added;
        }

        if (changed)
        {
            copy.incrementMajorVersion();

            //everything's ok - copy back to the original
            FFBase::operator=(copy);
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
bool MolproFF::addToMM(const Molecules &molecules,
                       const ParameterMap &map)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->addToMM(*(molecules.begin()),map);
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        SharedPolyPointer<MolproFF> ffield(*this);
        MolproFF &copy = *ffield;

        bool changed = false;

        for (Molecules::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool added = copy._pvt_addToMM(*it, map);

            changed = changed or added;
        }

        if (changed)
        {
            copy.incrementMajorVersion();

            //everything's ok - copy back to the original
            FFBase::operator=(copy);
        }

        return isDirty();
    }
}

bool MolproFF::_pvt_add(const PartialMolecule &molecule,
                        const ParameterMap &map)
{
    if (qm_mols.contains(molecule.ID()))
    {
        if (mm_mols.contains(molecule.ID()))
            throw SireMol::duplicate_molecule( QObject::tr(
                "You need to specify a group when adding the molecule with "
                "ID == %1, as it exists in both the QM and MM regions!")
                    .arg(molecule.ID()), CODELOC );

        return this->addToQM(molecule,map);
    }
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

/** Add a molecule to the forcefield - this molecule must already
    exist in just one group in this forcefield
*/
bool MolproFF::add(const PartialMolecule &molecule,
                   const ParameterMap &map)
{
    return this->_pvt_add(molecule,map);
}

/** Add lots of molecules to this forcefield - they must all already
    exist in this forcefield

    \throw SireMol::missing_molecule
*/
bool MolproFF::add(const Molecules &molecules,
                   const ParameterMap &map)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->add(*(molecules.begin()),map);
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        SharedPolyPointer<MolproFF> ffield(*this);
        MolproFF &copy = *ffield;

        bool changed = false;

        for (Molecules::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool mol_changed = this->_pvt_add(*it, map);
            changed = changed or mol_changed;
        }

        if (changed)
        {
            copy.incrementMajorVersion();
            FFBase::operator=(copy);
        }

        return isDirty();
    }
}

/** Add the molecule 'molecule' to the group 'group'

    \throw SireFF::invalid_group
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
        throw SireFF::invalid_group( QObject::tr(
            "There is no group in the MolproFF forcefield."),
                CODELOC );

        return isDirty();
    }
}

/** Add the molecules in 'molecules' to the group 'group'

    \throw SireFF::invalid_group
*/
bool MolproFF::addTo(const FFBase::Group &group,
                     const Molecules &molecules,
                     const ParameterMap &map)
{
    if (group == groups().qm())
        return this->addToQM(molecules,map);
    else if (group == groups().mm())
        return this->addToMM(molecules,map);
    else
    {
        throw SireFF::invalid_group( QObject::tr(
            "There is no group in the MolproFF forcefield."),
                CODELOC );

        return isDirty();
    }
}

/** Change the molecule 'molecule' */
bool MolproFF::_pvt_change(const PartialMolecule &molecule)
{
    MoleculeID molid = molecule.ID();

    bool changed = false;

    if (qm_mols.contains(molid))
    {
        //the object exists in the QM region - make the change
        if (qm_mols[molid].change(molecule))
        {
            //the molecule changed, so we need to rebuild
            //all of the arrays...
            this->mustNowUpdateQM();

            changed = true;
        }
    }

    if (mm_mols.contains(molid))
    {
        //the object exists in the MM region
        if (mm_mols[molid].change(molecule))
        {
            this->mustNowUpdateMM(molid);
            changed = true;
        }
    }

    return changed;
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
bool MolproFF::change(const Molecules &molecules)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->change( *(molecules.begin()) );
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        SharedPolyPointer<MolproFF> ffield(*this);
        MolproFF &copy = *ffield;

        bool changed = false;

        for (Molecules::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool this_changed = copy._pvt_change(*it);

            changed = changed or this_changed;
        }

        if (changed)
        {
            copy.incrementMinorVersion();
            FFBase::operator=(copy);
        }

        return isDirty();
    }
}

bool MolproFF::_pvt_removeFromQM(const PartialMolecule &molecule)
{
    MoleculeID molid = molecule.ID();

    if (qm_mols.contains(molid))
    {
        QMMolecule &qm_mol = qm_mols[molid];

        if (qm_mol.remove(molecule.selectedAtoms()))
        {
            if (qm_mol.molecule().selectedAtoms().selectedNone())
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
        MMMolecule &mm_mol = mm_mols[molid];

        if (mm_mol.remove(molecule.selectedAtoms()))
        {
            int nats = mm_mol.nAtomsInArray();

            if (mm_mol.molecule().selectedAtoms().selectedNone())
                //the entire molecule has been removed
                mm_mols.remove(molid);

            if (nats > 0)
            {
                //we need to rebuild everything as this atom
                //was in the MM array
                this->mustNowUpdateMM(molid);
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
bool MolproFF::removeFromQM(const Molecules &molecules)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->removeFromQM( *(molecules.begin()) );
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        SharedPolyPointer<MolproFF> ffield(*this);
        MolproFF &copy = *ffield;

        bool changed = false;

        for (Molecules::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool removed = copy._pvt_removeFromQM(*it);
            changed = changed or removed;
        }

        if (changed)
        {
            copy.incrementMajorVersion();
            FFBase::operator=(copy);
        }

        return isDirty();
    }
}

/** Remove the molecules in 'molecules' from the MM region */
bool MolproFF::removeFromMM(const Molecules &molecules)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->removeFromMM( *(molecules.begin()) );
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        SharedPolyPointer<MolproFF> ffield(*this);
        MolproFF &copy = *ffield;

        bool changed = false;

        for (Molecules::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool removed = copy._pvt_removeFromMM(*it);
            changed = changed or removed;
        }

        if (changed)
        {
            copy.incrementMajorVersion();
            FFBase::operator=(copy);
        }

        return isDirty();
    }
}

/** Remove the molecule 'molecule' from the group 'group'

    \throw SireFF::invalid_group
*/
bool MolproFF::removeFrom(const FFBase::Group &group,
                          const PartialMolecule &molecule)
{
    if (group == groups().qm())
        return this->removeFromQM(molecule);
    else if (group == groups().mm())
        return this->removeFromMM(molecule);
    else
    {
        throw SireFF::invalid_group( QObject::tr(
            "There is no group in the MolproFF forcefield."),
                CODELOC );

        return isDirty();
    }
}

/** Remove the molecules in 'molecules' from the group 'group'

    \throw SireFF::invalid_group
*/
bool MolproFF::removeFrom(const FFBase::Group &group,
                          const Molecules &molecules)
{
    if (group == groups().qm())
        return this->removeFromQM(molecules);
    else if (group == groups().mm())
        return this->removeFromMM(molecules);
    else
    {
        throw SireFF::invalid_group( QObject::tr(
            "There is no group in the MolproFF forcefield."),
                CODELOC );

        return isDirty();
    }
}

/** Completely remove the selected atoms of any version of
    the molecule 'molecule' from this forcefield */
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
bool MolproFF::remove(const Molecules &molecules)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
        return this->remove( *(molecules.begin()) );
    else
    {
        //work on a detached copy of this forcefield, so that
        //we maintain the invariant
        SharedPolyPointer<MolproFF> ffield(*this);
        MolproFF &copy = *ffield;

        bool changed = false;

        for (Molecules::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool removed = copy._pvt_remove(*it);
            changed = changed or removed;
        }

        if (changed)
        {
            copy.incrementMajorVersion();
            FFBase::operator=(copy);
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
             qm_mols.constBegin().value().molecule().info().nCutGroups() == 1)
    {
        //take the easy route!
        qm_coordgroup = qm_mols.constBegin()->molecule().extract().coordGroups().at(0);
    }
    else
    {
        //how many QM atoms are there?
        int natms = 0;

        for (QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.constBegin();
             it != qm_mols.constEnd();
             ++it)
        {
            natms += it->molecule().info().nAtoms();
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
            const QVector<CoordGroup> &groups = it->molecule().extract().coordGroups();

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
                               this->switchingFunction(), true);  //true as will
                                                                  //force update
        }

        //resize the MM array (four times as x,y,z + charge)
        mm_coords_and_charges = QVector<double>(4 * nats);

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
    if (mm_mols.isEmpty() or
        qm_coords.isEmpty() or qm_coordgroup.isEmpty())
    {
        //remove the MM array as there is no QM/MM energy
        //(though may still be a pure QM energy)
        mm_coords_and_charges.clear();
        mm_array_index.clear();

        this->clearStatus( NEED_REBUILD_MM );
        return;
    }
    else if (rebuild_mm.isEmpty())
    {
        this->clearStatus( NEED_REBUILD_MM );
        return;
    }

    bool recopy_all = false;
    bool recount_atoms = false;
    bool something_changed = false;

    int n_additional_atoms = 0;

    foreach (MoleculeID molid, rebuild_mm)
    {
        QHash<MoleculeID,MMMolecule>::iterator it = mm_mols.find(molid);

        if (it != mm_mols.end())
        {
            MMMolecule &mm_mol = *it;

            //see if the number of atoms in the array changes
            int old_natms = mm_mol.nAtomsInArray();

            int new_natms = mm_mol.update(qm_coordgroup,
                                          space(), switchingFunction());

            if (new_natms != old_natms)
            {
                recopy_all = true;
                n_additional_atoms = new_natms - old_natms;
            }
            else if (new_natms != 0)
            {
                //there were some atoms in the array before, and still some
                //atoms - this means that moving this molecule has changed
                //the QM/MM calculation
                something_changed = true;
            }
        }
        else
        {
            //this molecule has been removed since the last update
            recopy_all = true;
            recount_atoms = true;
        }
    }

    if (recount_atoms)
    {
        //we have to count up how many atoms there are in the arrays...
        int old_natms = mm_coords_and_charges.count() / 4;

        int new_natms = 0;

        for (QHash<MoleculeID,MMMolecule>::const_iterator it = mm_mols.constBegin();
             it != mm_mols.constEnd();
             ++it)
        {
            new_natms += it->nAtomsInArray();
        }

        n_additional_atoms = new_natms - old_natms;
    }

    if (recopy_all)
    {
        //need to recopy the array from scratch as the
        //locations of atoms in the array has changed
        if (n_additional_atoms != 0)
        {
            int new_size = mm_coords_and_charges.count() + (4*n_additional_atoms);

            if (new_size == 0)
            {
                //there are now no atoms in the MM cutoff region
                mm_coords_and_charges.clear();
                mm_array_index.clear();
                this->clearStatus(NEED_REBUILD_MM);
                rebuild_mm.clear();
                return;
            }

            //there are some new or missing atoms - we need a new array!
            mm_coords_and_charges.clear();
            mm_coords_and_charges = QVector<double>(new_size);
        }

        //copy in each of the MM molecules, recording the index of the
        //first atom of each molecule
        int i = 0;
        mm_array_index.clear();
        mm_array_index.reserve(mm_mols.count());

        for (QHash<MoleculeID,MMMolecule>::const_iterator it = mm_mols.constBegin();
             it != mm_mols.constEnd();
             ++it)
        {
            if (it->nAtomsInArray() > 0)
            {
                mm_array_index.insert(it.key(),i);
                i = it->update(mm_coords_and_charges, i);
            }
        }
    }
    else if (something_changed)
    {
        //only individual molecules in the array have changed
        foreach (MoleculeID molid, rebuild_mm)
        {
            BOOST_ASSERT( mm_array_index.contains(molid) );
            BOOST_ASSERT( mm_mols.contains(molid) );

            int i = mm_array_index.value(molid);
            mm_mols.constFind(molid)->update(mm_coords_and_charges, i);
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
    if (ff_status == UP2DATE)
        return;
    else if (this->needRebuildQM())
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
    else if (this->needUpdateMM())
    {
        this->updateMMArray();
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

    QStringList mmcoords;

    for (QHash<MoleculeID,MMMolecule>::const_iterator it = mm_mols.begin();
         it != mm_mols.end();
         ++it)
    {
        QString mol_string = it->coordAndChargesString();

        if (not mol_string.isEmpty())
            mmcoords.append(mol_string);
    }

    return mmcoords.join("\n");
}

/** Return the number of QM atoms in the array */
int MolproFF::nQMAtomsInArray()
{
    int nats = 0;

    for (QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.constBegin();
         it != qm_mols.constEnd();
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
    this->updateArrays();

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

        setComponent( components().total(), qmmm_nrg );
        setComponent( components().qm(), qmmm_nrg );

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
        setComponent( components().total(), qmmm_nrg );
        setComponent( components().qm(), qmmm_nrg );

        //the forcefield is now completely clean
        ff_status = UP2DATE;

        //exiting destroys 'session', which closes the
        //Molpro process
    }
}

/** Return whether or not this forcefield refers to the molecule
    with ID == molid */
bool MolproFF::refersTo(MoleculeID molid) const
{
    return qm_mols.contains(molid) or mm_mols.contains(molid);
}

/** Return whether or not the group 'group' in this forcefield
    refers to the molecule with ID == molid

    \throw SireFF::invalid_group
*/
bool MolproFF::refersTo(MoleculeID molid,
                        const FFBase::Group &group) const
{
    if (group == groups().qm())
        return qm_mols.contains(molid);
    else if (group == groups().mm())
        return mm_mols.contains(molid);
    else
    {
        throw SireFF::invalid_group( QObject::tr(
            "There is no such group in the MolproFF forcefield!"),
                CODELOC );

        return false;
    }
}

/** Return the groups referring to the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
QSet<FFBase::Group> MolproFF::groupsReferringTo(MoleculeID molid) const
{
    QSet<FFBase::Group> molgroups;

    if (qm_mols.contains(molid))
        molgroups.insert(groups().qm());

    if (mm_mols.contains(molid))
        molgroups.insert(groups().mm());

    if (molgroups.isEmpty())
        throw SireMol::missing_molecule( QObject::tr(
            "There is no molecule with ID == %1 in the forcefield "
            "%2 (%3 : %4).")
                .arg(molid).arg(this->name())
                .arg(this->ID()).arg(this->version().toString()),
                    CODELOC );

    return molgroups;
}

/** Return the IDs of all of the molecules that are in this forcefield */
QSet<MoleculeID> MolproFF::moleculeIDs() const
{
    return (qm_mols.keys() + mm_mols.keys()).toSet();
}

/** Return the IDs of the molecules that are in the group 'group'

    \throw SireFF::invalid_group
*/
QSet<MoleculeID> MolproFF::moleculeIDs(const FFBase::Group &group) const
{
    if (group == groups().qm())
        return qm_mols.keys().toSet();
    else if (group == groups().mm())
        return mm_mols.keys().toSet();
    else
    {
        throw SireFF::invalid_group( QObject::tr(
            "There is no such group in the MolproFF forcefield!"),
                CODELOC );

        return QSet<MoleculeID>();
    }
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
PartialMolecule MolproFF::molecule(MoleculeID molid) const
{
    if (qm_mols.contains(molid))
    {
        PartialMolecule mol = qm_mols.constFind(molid)->molecule();

        if (mm_mols.contains(molid))
            mol = mol.selection().add(
                      mm_mols.constFind(molid)->molecule().selectedAtoms());

        return mol;
    }
    else if (mm_mols.contains(molid))
        return mm_mols.constFind(molid)->molecule();
    else
    {
        throw SireMol::missing_molecule( QObject::tr(
            "There is no molecule with ID == %1 in the forcefield "
            "%2 (%3 : %4).")
                .arg(molid).arg(this->name())
                .arg(this->ID()).arg(this->version().toString()),
                    CODELOC );

        return PartialMolecule();
    }
}

/** Return the molecule in the group 'group' that has the ID == molid

    \throw SireMol::missing_molecule
    \throw SireFF::invalid_group
*/
PartialMolecule MolproFF::molecule(MoleculeID molid,
                                   const FFBase::Group &group) const
{
    bool foundmol = false;
    PartialMolecule mol;

    if (group == groups().qm())
    {
        if (qm_mols.contains(molid))
        {
            mol = qm_mols.constFind(molid)->molecule();
            foundmol = true;
        }
    }
    else if (group == groups().mm())
    {
        if (mm_mols.contains(molid))
        {
            mol = mm_mols.constFind(molid)->molecule();
            foundmol = true;
        }
    }
    else
        throw SireFF::invalid_group( QObject::tr(
            "There is no such group in the MolproFF forcefield!"),
                CODELOC );

    if (not foundmol)
        throw SireMol::missing_molecule( QObject::tr(
            "There is no molecule with ID == %1 in the group %2 "
            "in the forcefield %3 (%4 : %5).")
                .arg(molid).arg(this->name())
                .arg(group)
                .arg(this->ID()).arg(this->version().toString()),
                    CODELOC );

    return mol;
}

/** Return whether or not this forcefield contains all of the atoms
    of any version of the molecule 'molecule' */
bool MolproFF::contains(const PartialMolecule &mol) const
{
    return this->molecule(mol.ID())
                  .selectedAtoms().contains(mol.selectedAtoms());
}

/** Return whether or not this forcefield contains all of the
    atoms of any version of the molecule 'molecule'

    \throw SireFF::invalid_group
*/
bool MolproFF::contains(const PartialMolecule &mol,
                        const FFBase::Group &group) const
{
    return this->molecule(mol.ID(),group)
                .selectedAtoms().contains(mol.selectedAtoms());
}

/** Return copies of all of the molecules that are in this forcefield */
Molecules MolproFF::contents() const
{
    Molecules mols;

    int nmols = qm_mols.count() + mm_mols.count();

    if (nmols == 0)
        return mols;

    mols.reserve(nmols);

    for (QHash<MoleculeID,MMMolecule>::const_iterator it = mm_mols.constBegin();
         it != mm_mols.constEnd();
         ++it)
    {
        mols.add( it->molecule() );
    }

    for (QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.constBegin();
         it != qm_mols.constEnd();
         ++it)
    {
        mols.add( it->molecule() );
    }

    return mols;
}

/** Return copies of all of the QM molecules in this forcefield */
Molecules MolproFF::qmMolecules() const
{
    Molecules mols;

    int nmols = qm_mols.count();

    if (nmols > 0)
    {
        mols.reserve(nmols);

        for (QHash<MoleculeID,QMMolecule>::const_iterator it = qm_mols.constBegin();
             it != qm_mols.constEnd();
             ++it)
        {
            mols.add(it->molecule());
        }
    }

    return mols;
}

/** Return copies of all of the MM molecules in this forcefield */
Molecules MolproFF::mmMolecules() const
{
    Molecules mols;

    int nmols = mm_mols.count();

    if (nmols > 0)
    {
        mols.reserve(nmols);

        for (QHash<MoleculeID,MMMolecule>::const_iterator it = mm_mols.constBegin();
             it != mm_mols.constEnd();
             ++it)
        {
            mols.add(it->molecule());
        }
    }

    return mols;
}

/** Return copies of all of the molecules that are in the group 'group'

    \throw SireFF::invalid_group
*/
Molecules MolproFF::contents(const FFBase::Group &group) const
{
    if (group == groups().qm())
        return this->qmMolecules();
    else if (group == groups().mm())
        return this->mmMolecules();
    else
    {
        throw SireFF::invalid_group( QObject::tr(
            "There is no such group in the MolproFF forcefield!"),
                CODELOC );

        return Molecules();
    }
}
