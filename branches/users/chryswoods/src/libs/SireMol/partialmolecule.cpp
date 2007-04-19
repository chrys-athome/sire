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

#include "partialmolecule.h"

#include "moleculeview_inlines.h"
#include "molecule.h"
#include "residue.h"
#include "newatom.h"

#include "propertyextractor.h"

#include "atom.h"
#include "resid.h"
#include "resnum.h"
#include "cgatomid.h"
#include "moleculeproperty.h"

#include "SireBase/property.h"

#include "SireVol/coordgroup.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<PartialMolecule> r_partialmol;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const PartialMolecule &partialmol)
{
    writeHeader(ds, r_partialmol, 1);

    SharedDataStream sds(ds);

    sds << partialmol.selected_atoms
        << static_cast<const MoleculeView&>(partialmol);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       PartialMolecule &partialmol)
{
    VersionID v = readHeader(ds, r_partialmol);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> partialmol.selected_atoms
            >> static_cast<MoleculeView&>(partialmol);
    }
    else
        throw version_error(v, "1", r_partialmol, CODELOC);

    return ds;
}

/** Empty constructor */
PartialMolecule::PartialMolecule() : MoleculeView()
{}

/** Construct a PartialMolecule that represents the entire
    Molecule 'molecule' */
PartialMolecule::PartialMolecule(const Molecule &molecule)
                : MoleculeView(molecule),
                  selected_atoms(molecule)
{}

/** Construct a PartialMolecule that represents the
    selected atoms in 'selection' that are part of the
    molecule 'molecule' - the selected atoms must be
    compatible with 'molecule'

    \throw SireError::incompatible_error
*/
PartialMolecule::PartialMolecule(const Molecule &molecule,
                                 const AtomSelection &selection)
                : MoleculeView(molecule),
                  selected_atoms(selection)
{
    selected_atoms.assertCompatibleWith(molecule);
}

/** Construct a PartialMolecule that represents the whole
    of the residue 'residue' */
PartialMolecule::PartialMolecule(const Residue &residue)
                : MoleculeView(residue),
                  selected_atoms(residue)
{}

/** Construct a PartialMolecule that represents the atom 'atom' */
PartialMolecule::PartialMolecule(const NewAtom &atom)
                : MoleculeView(atom),
                  selected_atoms(atom)
{}

/** Copy constructor */
PartialMolecule::PartialMolecule(const PartialMolecule &other)
                : MoleculeView(other),
                  selected_atoms(other.selected_atoms)
{}

/** Destructor */
PartialMolecule::~PartialMolecule()
{}

/** Copy assignment - make equal to a Molecule */
PartialMolecule& PartialMolecule::operator=(const PartialMolecule &molecule)
{
    MoleculeView::operator=(molecule);
    selected_atoms = molecule.selected_atoms;

    return *this;
}

/** Comparison operator */
bool PartialMolecule::operator==(const PartialMolecule &other) const
{
    return MoleculeView::operator==(other) and
           selected_atoms == other.selected_atoms;
}

/** Comparison operator */
bool PartialMolecule::operator!=(const PartialMolecule &other) const
{
    return MoleculeView::operator!=(other) or
           selected_atoms != other.selected_atoms;
}

/** Return a PropertyExtractor object that can be used to
    extract various properties from this molecule
    (e.g. arrays containing all of the atom elements of
     selected atoms) */
PropertyExtractor PartialMolecule::extract() const
{
    return PropertyExtractor(*this);
}

/** Change this PartialMolecule to match 'molecule'.

    This returns whether any of the selected atoms in 'molecule'
    are also selected in this PartialMolecule.

    \throw SireError::incompatible_error
*/
bool PartialMolecule::change(const PartialMolecule &molecule)
{
    selected_atoms.assertCompatibleWith(molecule.selected_atoms);

    MoleculeView::operator=(molecule);

    return selected_atoms.intersects(molecule.selected_atoms);
}

/** Add the selected atoms in 'selection' to this PartialMolecule.
    This returns whether or not the selection is changed.

    \throw SireError::incompatible_error
*/
bool PartialMolecule::add(const AtomSelection &selection)
{
    selected_atoms.assertCompatibleWith(selection);

    if (not selected_atoms.contains(selection))
    {
        selected_atoms = selected_atoms.unite(selection);
        return true;
    }
    else
        return false;
}

/** Remove the selected atoms in 'selection' from this molecule.
    Returns whether or not the selection has been changed.

    \throw SireError::incompatible_error
*/
bool PartialMolecule::remove(const AtomSelection &selection)
{
    selected_atoms.assertCompatibleWith(selection);

    if (selected_atoms.intersects(selection))
    {
        selected_atoms = selected_atoms.subtract(selection);
        return true;
    }
    else
        return false;
}

/** Return whether this PartialMolecule is empty (has no atoms selected) */
bool PartialMolecule::isEmpty() const
{
    return selected_atoms.isEmpty();
}

/** Return the number of selected atoms */
int PartialMolecule::nSelected() const
{
    return selected_atoms.nSelected();
}

/** Return the number of selected atoms in the CutGroup with
    ID == cgid

    \throw SireError::invalid_index
*/
int PartialMolecule::nSelected(CutGroupID cgid) const
{
    return selected_atoms.nSelected(cgid);
}

/** Return the number of selected atoms in the residue with
    number 'resnum'

    \throw SireMol::missing_residue
*/
int PartialMolecule::nSelected(ResNum resnum) const
{
    return selected_atoms.nSelected(resnum);
}

/** Return the total number of CutGroups that contain at least
    one selected atom */
int PartialMolecule::nSelectedCutGroups() const
{
    return selected_atoms.nSelectedCutGroups();
}

/** Return the total number of residues that contain at least
    one selected atom */
int PartialMolecule::nSelectedResidues() const
{
    return selected_atoms.nSelectedResidues();
}

/** Return whether all of the CutGroups contain at least
    one selected atom */
bool PartialMolecule::selectedAllCutGroups() const
{
    return selected_atoms.selectedAllCutGroups();
}

/** Return whether all of the Residues contain at least
    one selected atom */
bool PartialMolecule::selectedAllResidues() const
{
    return selected_atoms.selectedAllResidues();
}

/** Return whether or not the atom with the index 'cgatomid'
    has been selected.

    \throw SireError::invalid_index
*/
bool PartialMolecule::selected(const CGAtomID &cgatomid) const
{
    return selected_atoms.selected(cgatomid);
}

/** Return whether or not the atom with index 'atomid' is selected */
bool PartialMolecule::selected(const IDMolAtom &atomid) const
{
    return selected_atoms.selected(atomid);
}

/** Return whether or not all atoms in the molecule have
    been selected. */
bool PartialMolecule::selectedAll() const
{
    return selected_atoms.selectedAll();
}

/** Return whether or not all atoms in the CutGroup with
    ID == cgid have been selected.

    \throw SireError::invalid_index
*/
bool PartialMolecule::selectedAll(CutGroupID cgid) const
{
    return selected_atoms.selectedAll(cgid);
}

/** Return whether or not all of the atoms in the Residue with
    number 'resnum' have been selected.

    \throw SireMol::missing_residue
*/
bool PartialMolecule::selectedAll(ResNum resnum) const
{
    return selected_atoms.selectedAll(resnum);
}

/** Return whether or not no atoms have been selected */
bool PartialMolecule::selectedNone() const
{
    return selected_atoms.selectedNone();
}

/** Return whether or not no atoms in the CutGroup with ID == cgid
    have been selected

    \throw SireError::invalid_index
*/
bool PartialMolecule::selectedNone(CutGroupID cgid) const
{
    return selected_atoms.selectedNone(cgid);
}

/** Return whether or not no atoms in the residue with
    number 'resnum' have been selected

    \throw SireMol::missing_residue
*/
bool PartialMolecule::selectedNone(ResNum resnum) const
{
    return selected_atoms.selectedNone(resnum);
}

/** Select all of the atoms in the molecule */
void PartialMolecule::selectAll()
{
    selected_atoms.selectAll();
}

/** Deselect all of the atoms in the molecule */
void PartialMolecule::deselectAll()
{
    selected_atoms.deselectAll();
}

/** Select all of the atoms in the CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
void PartialMolecule::selectAll(CutGroupID cgid)
{
    selected_atoms.selectAll(cgid);
}

/** Deselect all of the atoms in the CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
void PartialMolecule::deselectAll(CutGroupID cgid)
{
    selected_atoms.deselectAll(cgid);
}

/** Select all of the atoms that are in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
void PartialMolecule::selectAll(ResNum resnum)
{
    selected_atoms.selectAll(resnum);
}

/** Deselect all of the atoms that are in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
void PartialMolecule::deselectAll(ResNum resnum)
{
    selected_atoms.deselectAll(resnum);
}

/** Select all of the atoms selected in 'selection'

    \throw SireError::incompatible_error
*/
void PartialMolecule::selectAll(const AtomSelection &selection)
{
    selected_atoms.selectAll(selection);
}

/** Deselect all of the atoms selected in 'selection'

    \throw SireError::incompatible_error
*/
void PartialMolecule::deselectAll(const AtomSelection &selection)
{
    selected_atoms.deselectAll(selection);
}

/** Select the atom at index 'cgatomid'

    \throw SireError::invalid_index
*/
void PartialMolecule::select(const CGAtomID &cgatomid)
{
    selected_atoms.select(cgatomid);
}

/** Deselect the atom at index 'cgatomid'

    \throw SireError::invalid_index
*/
void PartialMolecule::deselect(const CGAtomID &cgatomid)
{
    selected_atoms.deselect(cgatomid);
}

/** Select the atom with index 'atomid' */
void PartialMolecule::select(const IDMolAtom &atomid)
{
    selected_atoms.select(atomid);
}

/** Deselect the atom at index 'atomid' */
void PartialMolecule::deselect(const IDMolAtom &atomid)
{
    selected_atoms.deselect(atomid);
}

/** Invert the current selection */
void PartialMolecule::invert()
{
    selected_atoms.invert();
}

/** Return whether or not this selection contains any of the atoms
    selected in 'other'

    \throw SireError::incompatible_error
*/
bool PartialMolecule::intersects(const AtomSelection &other) const
{
    return selected_atoms.intersects(other);
}

/** Return whether or not this selection contains all of the atoms that
    are contained in the 'other' selection

    \throw SireError::incompatible_error
*/
bool PartialMolecule::contains(const AtomSelection &other) const
{
    return selected_atoms.contains(other);
}

/** Return the intersection of this selection with 'other'. The
    returned selection will contain all atoms that are selected
    by both groups.

    \throw SireError::incompatible_error
*/
PartialMolecule PartialMolecule::intersect(const AtomSelection &other) const
{
    PartialMolecule i(*this);

    i.selected_atoms = selected_atoms.intersect(other);

    return i;
}

/** Return the union of this selection with 'other'. The
    returned selection will contain all atoms that are in
    either or the two selections.

    \throw SireError::incompatible_error
*/
PartialMolecule PartialMolecule::unite(const AtomSelection &other) const
{
    PartialMolecule u(*this);

    u.selected_atoms = selected_atoms.unite(other);

    return u;
}

/** Return this selection minus 'other' - this will return
    the atoms that are in this selection that are not in 'other'.

    \throw SireError::incompatible_error
*/
PartialMolecule PartialMolecule::subtract(const AtomSelection &other) const
{
    PartialMolecule s(*this);

    s.selected_atoms = selected_atoms.subtract(other);

    return s;
}

/** Apply the mask of CutGroupIDs to this selection - this deselects
    every CutGroup that does not have its ID in 'cgids' */
void PartialMolecule::applyMask(const QSet<CutGroupID> &cgids)
{
    selected_atoms.applyMask(cgids);
}

/** Apply the mask of residue numbers to this selection - this
    deselects every residue that does not have its number in 'resnums' */
void PartialMolecule::applyMask(const QSet<ResNum> &resnums)
{
    selected_atoms.applyMask(resnums);
}

/** Apply the mask of one AtomSelection to the other - this creates
    the intersection of the two AtomSelections

    \throw SireError::incompatible_error
*/
void PartialMolecule::applyMask(const AtomSelection &other)
{
    selected_atoms.applyMask(other);
}

/** Set the selection from 'selection'

    \throw SireError::incompatible_error
*/
void PartialMolecule::setSelection(const AtomSelection &selection)
{
    selected_atoms.assertCompatibleWith(selection);
    selected_atoms = selection;
}

/** Return a list of all of the atoms that are contained in the this selection */
QList<AtomIndex> PartialMolecule::selected() const
{
    return selected_atoms.selected();
}

/** Return the MoleculeInfo that holds the metainfo for this
    molecule */
const MoleculeInfo& PartialMolecule::info() const
{
    return data().info();
}

/** Return the name of the molecule. */
QString PartialMolecule::name() const
{
    return info().name();
}

/** Return the ID number of this molecule */
MoleculeID PartialMolecule::ID() const
{
    return data().ID();
}

/** Return the version number of this molecule. */
const MoleculeVersion& PartialMolecule::version() const
{
    return data().version();
}

/** Return the CutGroupIDs of any CutGroups that have at least one
    selected atom */
QSet<CutGroupID> PartialMolecule::selectedCutGroups() const
{
    return selected_atoms.selectedCutGroups();
}

/** Return the residue numbers of any residues that have at least
    one selected atom */
QSet<ResNum> PartialMolecule::selectedResidues() const
{
    return selected_atoms.selectedResidues();
}

/** Return the property with the name 'name'. If this is a molecular
    property then this property will be masked by the atom selection
    (e.g. AtomicProperties will only be returned for selected atoms,
     with the order the same as that returned by coordGroups() )

    \throw SireBase::missing_property
*/
Property PartialMolecule::getProperty(const QString &name) const
{
    Property property = data().getProperty(name);

    if (this->selectedAll() or not property.isA<MoleculeProperty>())
    {
        return property;
    }

    //mask this property by the current selection
    return property.asA<MoleculeProperty>().mask(selected_atoms);
}

/** Return the CoordGroups that contain the coordinates of atoms that
    have been selected. Only CoordGroups that contain selected atoms
    will be returned. */
QVector<CoordGroup> PartialMolecule::coordGroups() const
{
    QVector<CoordGroup> coords = data().coordGroups();

    if (this->selectedAllCutGroups())
        return coords;
    else
    {
        QSet<CutGroupID> cgids = selected_atoms.selectedCutGroups();
        QVector<CoordGroup> selected_coords( cgids.count() );

        const CoordGroup *coords_array = coords.constData();
        CoordGroup *selected_coords_array = selected_coords.data();

        int i=0;

        foreach (CutGroupID cgid, cgids)
        {
            selected_coords_array[i] = coords_array[cgid];
            ++i;
        }

        return selected_coords;
    }
}

/** Return the index of CutGroupID into the CoordGroup, AtomicProperty
    or CutGroup arrays. This is necessary as while CutGroupID corresponds
    exactly to the array index for a whole molecule, it does not
    correspond exactly for a partial molecule (as CutGroups that don't contain
    any selected atoms are left out, and the order of the CutGroups in the
    array may change). This function returns the index that maps CutGroupID
    to the location of the CutGroup in the arrays returned by coordGroups(),
    getProperty() and all of the extract() functions. */
QHash<CutGroupID,quint32> PartialMolecule::cutGroupIndex() const
{
    if (this->selectedAllCutGroups())
    {
        //all CutGroups are selected, so CutGroupID => index into array
        // - this is a pretty pointless index!
        QHash<CutGroupID,quint32> index;
        uint ncg = info().nCutGroups();
        index.reserve(ncg);

        for (CutGroupID i(0); i<ncg; ++i)
        {
            index.insert( i, i );
        }

        return index;
    }
    else
    {
        //there are some missing CutGroups - the arrays are ordered
        //using the iterator order of selectedCutGroups()
        QSet<CutGroupID> cgids = selected_atoms.selectedCutGroups();

        QHash<CutGroupID,quint32> index;

        if (not cgids.isEmpty())
        {
            index.reserve(cgids.count());

            quint32 i = 0;

            foreach (CutGroupID cgid, cgids)
            {
                index.insert( cgid, i );
                ++i;
            }
        }

        return index;
    }
}
