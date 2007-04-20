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

#include "atomselector.h"
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

/** Copy assignment operator */
AtomSelector& AtomSelector::operator=(const AtomSelector &other)
{
    MolDataView::operator=(other);
    return *this;
}

/** Destructor */
AtomSelector::~AtomSelector()
{}

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

/** Invert the current selection */
AtomSelector AtomSelector::invert() const
{
    AtomSelector retval = *this;
    retval._pvt_selection() = selectedAtoms().invert();
    return *this;
}
