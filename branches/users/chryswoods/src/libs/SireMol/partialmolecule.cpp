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

#include "molecule.h"
#include "residue.h"
#include "newatom.h"

#include "atom.h"
#include "resid.h"
#include "resnum.h"
#include "cgatomid.h"

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

    sds << partialmol.d << partialmol.selected_atoms;

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

        sds >> partialmol.d >> partialmol.selected_atoms;
    }
    else
        throw version_error(v, "1", r_partialmol, CODELOC);

    return ds;
}

/** Empty constructor */
PartialMolecule::PartialMolecule() : d(MoleculeData::null())
{}

/** Construct a PartialMolecule that represents the entire
    Molecule 'molecule' */
PartialMolecule::PartialMolecule(const Molecule &molecule)
                : d(molecule.d),
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
                : d(molecule.d),
                  selected_atoms(selection)
{
    selected_atoms.assertCompatibleWith(molecule);
}

/** Construct a PartialMolecule that represents the whole
    of the residue 'residue' */
PartialMolecule::PartialMolecule(const Residue &residue)
                : d(residue.d),
                  selected_atoms(residue)
{}

/** Construct a PartialMolecule that represents the atom 'atom' */
PartialMolecule::PartialMolecule(const NewAtom &atom)
                : d(atom.d),
                  selected_atoms(atom)
{}

/** Copy constructor */
PartialMolecule::PartialMolecule(const PartialMolecule &other)
                : d(other.d),
                  selected_atoms(other.selected_atoms)
{}

/** Destructor */
PartialMolecule::~PartialMolecule()
{}

/** Copy assignment - make equal to a Molecule */
PartialMolecule& PartialMolecule::operator=(const Molecule &molecule)
{
    d = molecule.d;
    selected_atoms = AtomSelection(molecule);

    return *this;
}

/** Copy assignment - make equal to a Residue */
PartialMolecule& PartialMolecule::operator=(const Residue &residue)
{
    d = residue.d;
    selected_atoms = AtomSelection(residue);

    return *this;
}

/** Copy assignment - make equal to a Atom */
PartialMolecule& PartialMolecule::operator=(const NewAtom &atom)
{
    d = atom.d;
    selected_atoms = AtomSelection(atom);

    return *this;
}

/** Copy assignment */
PartialMolecule& PartialMolecule::operator=(const PartialMolecule &other)
{
    d = other.d;
    selected_atoms = other.selected_atoms;

    return *this;
}

/** Comparison operator */
bool PartialMolecule::operator==(const PartialMolecule &other) const
{
    return ( d == other.d or *d == *(other.d) ) and
           selected_atoms == other.selected_atoms;
}

/** Comparison operator */
bool PartialMolecule::operator!=(const PartialMolecule &other) const
{
    return ( d != other.d and *d != *(other.d) ) or
           selected_atoms != other.selected_atoms;
}

/** Return the ID of the molecule */
MoleculeID PartialMolecule::ID() const
{
    return d->ID();
}

/** Return the molecule's version */
const MoleculeVersion& PartialMolecule::version() const
{
    return d->version();
}

/** Return the info object describing the data-structure of the molecule */
const MoleculeInfo& PartialMolecule::info() const
{
    return d->info();
}

/** Return the info object describing the data-structure of the
    residue with number 'resnum'

    \throw SireMol::missing_residue
*/
const ResidueInfo& PartialMolecule::info(ResNum resnum) const
{
    return d->info().residue(resnum);
}

/** Return the info object describing the data-structure of the
    residue at index 'resid'

    \throw SireError::invalid_index
*/
const ResidueInfo& PartialMolecule::info(ResID resid) const
{
    return d->info().residue(resid);
}

/** Return the PartialMolecule as a Molecule */
Molecule PartialMolecule::molecule() const
{
    Molecule mol;
    mol.d = d;
    return mol;
}

/** Return the PartialMolecule as a Residue

    \throw SireMol::missing_residue
*/
Residue PartialMolecule::residue(ResNum resnum) const
{
    return molecule().residue(resnum);
}

/** Return the PartialMolecule as a Residue

    \throw SireError::invalid_index
*/
Residue PartialMolecule::residue(ResID resid) const
{
    return molecule().residue(resid);
}

/** Return the PartialMolecule as a Residue

    \throw SireMol::missing_residue
*/
Residue PartialMolecule::residue(const QString &resname) const
{
    return molecule().residue(resname);
}

/** Return the PartialMolecule as an Atom

    \throw SireMol::missing_atom
*/
NewAtom PartialMolecule::atom(const IDMolAtom &atomid) const
{
    return NewAtom( atomid, this->molecule() );
}

/** Return the selection representing the atoms that are
    part of this PartialMolecule */
const AtomSelection& PartialMolecule::selection() const
{
    return selected_atoms;
}

/** Change this PartialMolecule to match 'molecule'.

    This returns whether any of the selected atoms in 'molecule'
    are also selected in this PartialMolecule.

    \throw SireError::incompatible_error
*/
bool PartialMolecule::change(const PartialMolecule &molecule)
{
    selected_atoms.assertCompatibleWith(molecule.selected_atoms);

    d = molecule.d;

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

/** Return a list of all of the atoms that are contained in the this selection */
QList<AtomIndex> PartialMolecule::selected() const
{
    return selected_atoms.selected();
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
