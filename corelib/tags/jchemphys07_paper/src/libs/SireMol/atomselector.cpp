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

#include "SireMol/qhash_siremol.h"

#include "atomselector.h"
#include "atomindex.h"
#include "moleculeview_inlines.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<AtomSelector> r_atomselector;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const AtomSelector &atomselector)
{
    writeHeader(ds, r_atomselector, 1)
          << static_cast<const MolDataView&>(atomselector);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       AtomSelector &atomselector)
{
    VersionID v = readHeader(ds, r_atomselector);

    if (v == 1)
    {
        ds >> static_cast<MolDataView&>(atomselector);
    }
    else
        throw version_error(v, "1", r_atomselector, CODELOC);

    return ds;
}

/** Null constructor */
AtomSelector::AtomSelector() : MolDataView()
{}

/** Construct the extractor to extract properties from 'molecule' */
AtomSelector::AtomSelector(const MoleculeView &molecule)
             : MolDataView(molecule)
{}

/** Construct the selector to select the atoms in 'molecule'
    restricted to the selection in 'selected_atoms'

    \throw SireError::incompatible_error
*/
AtomSelector::AtomSelector(const MoleculeView &molecule,
                           const SelectionFromMol &selected_atoms)
             : MolDataView(molecule)
{
    _pvt_selection() = selectedAtoms().intersect(selected_atoms);
}

/** Copy constructor */
AtomSelector::AtomSelector(const AtomSelector &other)
             : MolDataView(other)
{}

/** Destructor */
AtomSelector::~AtomSelector()
{}

/** Copy assignment operator */
AtomSelector& AtomSelector::operator=(const AtomSelector &other)
{
    MolDataView::operator=(other);
    return *this;
}

/** Return the selector that represents the entire molecule */
AtomSelector AtomSelector::selectAll() const
{
    if (selectedAtoms().selectedAll())
        return *this;
    else
    {
        AtomSelector retval = *this;
        retval._pvt_selection() = selectedAtoms().selectAll();
        return retval;
    }
}

/** Return the selector that represents the entire molecule,
    but with no selected atoms! */
AtomSelector AtomSelector::deselectAll() const
{
    if (selectedAtoms().selectedNone())
        return *this;
    else
    {
        AtomSelector retval = *this;
        retval._pvt_selection() = selectedAtoms().deselectAll();
        return retval;
    }
}

/** Return the selector that represents the entire molecule,
    but with no selected atoms! */
AtomSelector AtomSelector::selectNone() const
{
    return this->deselectAll();
}

/** Return the selector with a completely cleared selection */
AtomSelector AtomSelector::clear() const
{
    return this->deselectAll();
}

/** Add the selection 'selection' to this molecule

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::add(const SelectionFromMol &selection) const
{
    AtomSelector retval = *this;
    retval._pvt_selection() = selectedAtoms().select(selection);
    return retval;
}

/** Remove the selected atoms in 'selection' from this molecule

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::remove(const SelectionFromMol &selection) const
{
    AtomSelector retval = *this;
    retval._pvt_selection() = selectedAtoms().deselect(selection);
    return retval;
}

/** Return the selector with all of the atoms in 'selection' additionally
    selected

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::selectAll(const SelectionFromMol &selection) const
{
    return this->add(selection);
}

/** Return a selector with all of the atoms in 'selection' deselected

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::deselectAll(const SelectionFromMol &selection) const
{
    return this->remove(selection);
}

/** Return a selector with none of the atoms in 'selection' selected

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::selectNone(const SelectionFromMol &selection) const
{
    return this->remove(selection);
}

/** Return a selector with the selection of the atoms in 'selection'
    removed.

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::clear(const SelectionFromMol &selection) const
{
    return this->remove(selection);
}

/** Return a selector with the selection set equal to 'selection'

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::set(const SelectionFromMol &selection) const
{
    AtomSelector retval = *this;

    retval._pvt_selection() = selectedAtoms().setSelection(selection);

    return retval;
}

/** Return a selector with the selection set equal to 'selection'

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::setSelection(const SelectionFromMol &selection) const
{
    return this->set(selection);
}

/** Return a selector with the atoms in 'selection' also selected

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::select(const SelectionFromMol &selection) const
{
    return this->add(selection);
}

/** Return a selector with the atoms in 'selection' removed from
    the selection

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::deselect(const SelectionFromMol &selection) const
{
    return this->remove(selection);
}

/** Invert the current selection */
AtomSelector AtomSelector::invert() const
{
    AtomSelector retval = *this;
    retval._pvt_selection() = selectedAtoms().invert();
    return *this;
}

/** Return the selector that represents the intersection of this
    selection and 'selection'

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::intersect(const SelectionFromMol &selection) const
{
    AtomSelector retval = *this;
    retval._pvt_selection() = selectedAtoms().intersect(selection);

    return *this;
}

/** Return the selector that represents the union of this
    selection and 'selection'

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::unite(const SelectionFromMol &selection) const
{
    AtomSelector retval = *this;
    retval._pvt_selection() = selectedAtoms().unite(selection);

    return *this;
}

/** Return the selector that represents the difference of this
    selection and 'selection'

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::subtract(const SelectionFromMol &selection) const
{
    AtomSelector retval = *this;
    retval._pvt_selection() = selectedAtoms().subtract(selection);

    return *this;
}

/** Return the mask of 'selection' with this selector - this returns
    the intersection of the two selections

    \throw SireError::incompatible_error
*/
AtomSelector AtomSelector::applyMask(const SelectionFromMol &selection) const
{
    AtomSelector retval = *this;
    retval._pvt_selection() = selectedAtoms().applyMask(selection);

    return *this;
}

/** Return whether no atoms are selected */
bool AtomSelector::isEmpty() const
{
    return selectedAtoms().isEmpty();
}

/** Return whether all atoms are selected */
bool AtomSelector::isComplete() const
{
    return selectedAtoms().selectedAll();
}

/** Return the total number of selected atoms */
int AtomSelector::nSelected() const
{
    return selectedAtoms().nSelected();
}

/** Return the total number of atoms from 'selection' which have been
    selected

    \throw SireError::incompatible_error
*/
int AtomSelector::nSelected(const SelectionFromMol &selection) const
{
    return selectedAtoms().nSelected(selection);
}

/** Return the number of CutGroups that contain at least one
    selected atom */
int AtomSelector::nSelectedCutGroups() const
{
    return selectedAtoms().nSelectedCutGroups();
}

/** Return the number of Residues that contain at least one
    selected atom */
int AtomSelector::nSelectedResidues() const
{
    return selectedAtoms().nSelectedResidues();
}

/** Return whether or not all CutGroups contain at least one
    selected atom */
bool AtomSelector::selectedAllCutGroups() const
{
    return selectedAtoms().selectedAllCutGroups();
}

/** Return whether or not all CutGroups contain at least one
    selected atom */
bool AtomSelector::selectedAllResidues() const
{
    return selectedAtoms().selectedAllResidues();
}

/** Return whether all of the atoms in 'selection' are selected

    \throw SireError::incompatible_error
*/
bool AtomSelector::selected(const SelectionFromMol &selection) const
{
    return selectedAtoms().selectedAll(selection);
}

/** Return whether all of the atoms in 'selection' are selected

    \throw SireError::incompatible_error
*/
bool AtomSelector::selectedAll(const SelectionFromMol &selection) const
{
    return selectedAtoms().selectedAll(selection);
}

/** Return whether all atoms are selected */
bool AtomSelector::selectedAll() const
{
    return selectedAtoms().selectedAll();
}

/** Return whether none of the atoms are selected */
bool AtomSelector::selectedNone() const
{
    return selectedAtoms().selectedNone();
}

/** Return whether none of the atoms from 'selection' are selected

    \throw SireError::incompatible_error
*/
bool AtomSelector::selectedNone(const SelectionFromMol &selection) const
{
    return selectedAtoms().selectedNone(selection);
}

/** Return whether this selection contains any of the atoms
    in the selection 'selection'

    \throw SireError::incompatible_error
*/
bool AtomSelector::intersects(const SelectionFromMol &selection) const
{
    return selectedAtoms().intersects(selection);
}

/** Return whether this selection contains all of the atoms
    in 'selection'

    \throw SireError::incompatible_error
*/
bool AtomSelector::contains(const SelectionFromMol &selection) const
{
    return selectedAtoms().contains(selection);
}

/** Return the AtomIndex objects of all of the selected atoms */
QSet<AtomIndex> AtomSelector::atomIndexes() const
{
    return selectedAtoms().selectedAtoms();
}

/** Return the CutGroupIDs of all CutGroups that contain at least
    one selected atom */
QSet<CutGroupID> AtomSelector::cutGroupIDs() const
{
    return selectedAtoms().selectedCutGroups();
}

/** Return the residue numbers of all residues that contain
    at least one selected atom */
QSet<ResNum> AtomSelector::residueNumbers() const
{
    return selectedAtoms().selectedResidues();
}
