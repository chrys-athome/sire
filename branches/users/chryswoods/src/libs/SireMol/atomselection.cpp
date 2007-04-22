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

#include "atomselection.h"

#include "molecule.h"
#include "residue.h"
#include "newatom.h"
#include "atominfo.h"
#include "moleculeinfo.h"
#include "residueinfo.h"
#include "resnum.h"
#include "residatomid.h"
#include "cgatomid.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<AtomSelection> r_atomselec;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AtomSelection &atomselec)
{
    writeHeader(ds, r_atomselec, 1)
            << atomselec.selected_atoms
            << atomselec.molinfo
            << atomselec.nselected;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomSelection &atomselec)
{
    VersionID v = readHeader(ds, r_atomselec);

    if (v == 1)
    {
        ds >> atomselec.selected_atoms
           >> atomselec.molinfo
           >> atomselec.nselected;
    }
    else
        throw version_error(v, "1", r_atomselec, CODELOC);

    return ds;
}

/** Null constructor */
AtomSelection::AtomSelection() : nselected(0)
{}

/** Copy constructor */
AtomSelection::AtomSelection(const AtomSelection &other)
              : selected_atoms(other.selected_atoms),
                molinfo(other.molinfo),
                nselected(other.nselected)
{}

/** Copy assignment operator */
AtomSelection& AtomSelection::operator=(const AtomSelection &other)
{
    if (this != &other)
    {
        selected_atoms = other.selected_atoms;
        molinfo = other.molinfo;
        nselected = other.nselected;
    }

    return *this;
}

/** Construct a selection which initially represents the selection
    of all of the atoms in 'molecule' */
AtomSelection::AtomSelection(const MoleculeView &molecule)
{
    *this = molecule.selectedAtoms();
}

/** Construct a selection which initially represents the selection
    of all of the atoms in the molecule described by 'molecule_info' */
AtomSelection::AtomSelection(const MoleculeInfo &molecule_info)
              : molinfo(molecule_info),
                nselected(molecule_info.nAtoms())
{}

/** Destructor */
AtomSelection::~AtomSelection()
{}

/** Comparison operator */
bool AtomSelection::operator==(const AtomSelection &other) const
{
    return nselected == other.nselected and
           molinfo == other.molinfo and
           selected_atoms == other.selected_atoms;
}

/** Comparison operator */
bool AtomSelection::operator!=(const AtomSelection &other) const
{
    return nselected != other.nselected or
           molinfo != other.molinfo or
           selected_atoms != other.selected_atoms;
}

/** Is this an empty selection (selected no atoms, nSelected() == 0) */
bool AtomSelection::isEmpty() const
{
    return nselected == 0;
}

/** Return the total number of selected atoms */
int AtomSelection::nSelected() const
{
    return nselected;
}

/** Return whether or not all atoms in the molecule have
    been selected. */
bool AtomSelection::selectedAll() const
{
    return nselected != 0 and selected_atoms.isEmpty();
}

/** Return the total number of CutGroups that contain at least
    one selected atom */
int AtomSelection::nSelectedCutGroups() const
{
    if (this->selectedAll())
        return molinfo.nCutGroups();
    else
        return selected_atoms.count();
}

/** Return the total number of residues that contain at least
    one selected atom */
int AtomSelection::nSelectedResidues() const
{
    if (this->selectedAll())
        return molinfo.nResidues();
    else
    {
        int nselected = 0;

        uint nres = molinfo.nResidues();

        for (ResID i(0); i<nres; ++i)
        {
            uint nats = molinfo.nAtoms(i);

            for (AtomID j(0); j<nats; ++j)
            {
                if ( this->selected(ResIDAtomID(i,j)) )
                {
                    ++nselected;
                    break;
                }
            }
        }

        return nselected;
    }
}

/** Return whether all CutGroups contain at least one selected atom */
bool AtomSelection::selectedAllCutGroups() const
{
    return this->selectedAll() or
           this->nSelectedCutGroups() == molinfo.nCutGroups();
}

/** Return whether all Residues contain at least one selected atom */
bool AtomSelection::selectedAllResidues() const
{
    if (this->selectedAll())
        return true;
    else
    {
        uint nres = molinfo.nResidues();

        for (ResID i(0); i<nres; ++i)
        {
            uint nats = molinfo.nAtoms(i);

            bool selected_this_residue = false;

            for (AtomID j(0); j<nats; ++j)
            {
                if ( this->selected(ResIDAtomID(i,j)) )
                {
                    selected_this_residue = true;
                    break;
                }
            }

            if (not selected_this_residue)
                return false;
        }

        return true;
    }
}

/** Return whether or not no atoms have been selected */
bool AtomSelection::selectedNone() const
{
    return nselected == 0;
}

/** Return the total number of selected atoms in the
    CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
int AtomSelection::nSelected(CutGroupID cgid) const
{
    molinfo.assertCutGroupExists(cgid);

    if (this->selectedAll())
        return molinfo.nAtoms(cgid);
    else
        return selected_atoms.value(cgid).count();
}

/** Internal function used to see if the atom with index
    'cgatomid' has been selected. This doesn't check whether
    or not the index is valid! */
bool AtomSelection::_pvt_selected(const CGAtomID &cgatomid) const
{
    if (selectedAll())
        return true;
    else
    {
        QHash< CutGroupID,QSet<AtomID> >::const_iterator it =
                                        selected_atoms.find(cgatomid.cutGroupID());

        return it != selected_atoms.end() and
               ( it->isEmpty() or it->contains(cgatomid.atomID()) );
    }
}

/** Return the total number of atoms selected in the
    residue with number 'resnum'

    \throw SireMol::missing_residue
*/
int AtomSelection::nSelected(ResNum resnum) const
{
    molinfo.assertResidueExists(resnum);

    if (this->selectedAll())
        return molinfo.nAtoms(resnum);
    else
    {
        ResidueInfo resinfo = molinfo[resnum];

        uint nats = resinfo.nAtoms();

        int nselected = 0;

        for (AtomID i(0); i<nats; ++i)
        {
            if (this->_pvt_selected(resinfo[i]))
                ++nselected;
        }

        return nselected;
    }
}

/** Return the number of atoms from 'selection' that are
    selected in this selection

    \throw SireError::incompatible_error
*/
int AtomSelection::nSelected(const AtomSelection &selection) const
{
    return this->intersect(selection).nSelected();
}

/** Return the number of atoms from 'selection' that
    are selected in this selection

    \throw SireError::incompatible_error
*/
int AtomSelection::nSelected(const SelectionFromMol &selection) const
{
    return this->intersect( this->setSelection(selection) ).nSelected();
}

/** Return whether or not all atoms in the CutGroup with
    ID == cgid have been selected.

    \throw SireError::invalid_index
*/
bool AtomSelection::selectedAll(CutGroupID cgid) const
{
    return nSelected(cgid) == molinfo.nAtoms(cgid);
}

/** Return whether or not all of the atoms in the Residue with
    number 'resnum' have been selected.

    \throw SireMol::missing_residue
*/
bool AtomSelection::selectedAll(ResNum resnum) const
{
    return nSelected(resnum) == molinfo.nAtoms(resnum);
}

/** Return whether or not all of the atoms in 'selection'
    have been selected.

    \throw SireError::incompatible_error
*/
bool AtomSelection::selectedAll(const AtomSelection &other) const
{
    return this->contains(other);
}

/** Return whether or not all of the atoms in 'selection'
    have been selected

    \throw SireError::incompatible_error
*/
bool AtomSelection::selectedAll(const SelectionFromMol &selection) const
{
    return this->contains(selection);
}

/** Return whether or not no atoms in the CutGroup with ID == cgid
    have been selected

    \throw SireError::invalid_index
*/
bool AtomSelection::selectedNone(CutGroupID cgid) const
{
    return nSelected(cgid) == 0;
}

/** Return whether or not no atoms in the residue with
    number 'resnum' have been selected

    \throw SireMol::missing_residue
*/
bool AtomSelection::selectedNone(ResNum resnum) const
{
    return nSelected(resnum) == 0;
}

/** Return whether or not no atoms from 'selection'
    have been selected

    \throw SireError::incompatible_error
*/
bool AtomSelection::selectedNone(const AtomSelection &selection) const
{
    return not this->intersects(selection);
}

/** Return whether or not no atoms from 'selection'
    have been selected

    \throw SireError::incompatible_error
*/
bool AtomSelection::selectedNone(const SelectionFromMol &selection) const
{
    return not this->intersects(selection);
}

/** Return whether or not the atom with the index 'cgatomid'
    has been selected.

    \throw SireError::invalid_index
*/
bool AtomSelection::selected(const CGAtomID &cgatomid) const
{
    molinfo.assertAtomExists(cgatomid);

    return _pvt_selected(cgatomid);
}

/** Return whether or not the atom with index 'atomid' is selected */
bool AtomSelection::selected(const IDMolAtom &atomid) const
{
    return _pvt_selected( atomid.index(molinfo) );
}

/** Select all of the atoms in the molecule */
void AtomSelection::_pvt_selectAll()
{
    selected_atoms.clear();
    nselected = molinfo.nAtoms();
}

/** Select all of the atoms in the molecule */
AtomSelection AtomSelection::selectAll() const
{
    AtomSelection retval = *this;
    retval._pvt_selectAll();

    return retval;
}

/** Deselect all of the atoms in the molecule */
void AtomSelection::_pvt_deselectAll()
{
    selected_atoms.clear();
    nselected = 0;
}

/** Deselect all of the atoms in the molecule */
AtomSelection AtomSelection::deselectAll() const
{
    AtomSelection retval = *this;
    retval._pvt_deselectAll();

    return retval;
}

/** Synonym for deselectAll() */
AtomSelection AtomSelection::selectNone() const
{
    return this->deselectAll();
}

/** Select all of the atoms in the CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
void AtomSelection::_pvt_select(CutGroupID cgid)
{
    molinfo.assertCutGroupExists(cgid);

    if (not this->selectedAll())
    {
        //how many atoms from this CutGroup have already been selected?
        int nats = selected_atoms.value(cgid).count();

        //calculate the number of atoms selected after this change
        nselected += (molinfo.nAtoms(cgid) - nats);

        if ( nselected == quint32(molinfo.nAtoms()) )
            //all of the atoms have now been selected!
            selected_atoms.clear();
        else
            //replace any existing set of AtomIDs with an empty
            //set (the empty set implies all atoms are selected)
            selected_atoms.insert( cgid, QSet<AtomID>() );
    }
}

/** Select all of the atoms in the CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectAll(CutGroupID cgid) const
{
    AtomSelection retval = *this;
    retval._pvt_select(cgid);

    return retval;
}

/** Deselect all of the atoms in the CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
void AtomSelection::_pvt_deselect(CutGroupID cgid)
{
    //how many atoms from this CutGroup have been selected already?
    int nats = this->nSelected(cgid);

    if (nats == 0)
        //none of the atoms in this CutGroup are selected
        return;

    //subtract this number from the total
    nselected -= nats;

    if (nselected == 0)
    {
        //there are now no atoms selected!
        selected_atoms.clear();
    }
    else if (selected_atoms.isEmpty())
    {
        //these are the first atoms that are being deselected
        // - we need to populate selected_atoms with empty sets
        //   for each CutGroup
        uint ncg = molinfo.nCutGroups();

        selected_atoms.reserve(ncg-1);

        for (CutGroupID i(0); i<ncg; ++i)
        {
            if (i != cgid)
                selected_atoms.insert(i, QSet<AtomID>());
        }
    }
    else
    {
        selected_atoms.remove(cgid);
    }
}

/** Deselect all of the atoms in the CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselectAll(CutGroupID cgid) const
{
    AtomSelection retval = *this;
    retval._pvt_deselect(cgid);

    return *this;
}

/** Select all of the atoms in the CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(CutGroupID cgid) const
{
    return this->selectAll(cgid);
}

/** Deselect all of the atoms in the CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(CutGroupID cgid) const
{
    return this->deselectAll(cgid);
}

/** Select only the atoms in the CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::setSelection(CutGroupID cgid) const
{
    return this->selectNone().select(cgid);
}

/** Internal function used to select the atom at index 'cgatomid'
    - this does not check whether this index is valid. */
void AtomSelection::_unsafe_select(const CGAtomID &cgatomid)
{
    if (not this->selectedAll())
    {
        int ncgatoms = molinfo.nAtoms(cgatomid.cutGroupID());

        QHash< CutGroupID,QSet<AtomID> >::iterator it =
                            selected_atoms.find(cgatomid.cutGroupID());

        if (it != selected_atoms.end())
        {
            //atoms from this CutGroup have already been selected

            if ( not (it->isEmpty() or it->contains(cgatomid.atomID())) )
            {
                //the atom in this CutGroup has not yet been selected

                if (it->count() == ncgatoms - 1)
                {
                    //adding this atom will complete the set of all
                    //atoms in this selected
                    this->selectAll(cgatomid.cutGroupID());
                    return;
                }
                else
                {
                    it->insert(cgatomid.atomID());
                    ++nselected;
                }
            }
        }
        else
        {
            //none of the atoms in this CutGroup have been selected

            if (ncgatoms == 1)
            {
                //there is only one atom in this CutGroup, and it has
                //just been selected!
                this->selectAll(cgatomid.cutGroupID());
                return;
            }
            else
            {
                // add a hash containing just this atom
                QSet<AtomID> atms;
                atms.reserve(ncgatoms);

                atms.insert(cgatomid.atomID());

                selected_atoms.insert( cgatomid.cutGroupID(), atms );

                ++nselected;
            }
        }
    }
}

void AtomSelection::_pvt_select(const CGAtomID &cgatomid)
{
    info().assertAtomExists(cgatomid);
    this->_unsafe_select(cgatomid);
}

/** Internal function used to deselect the atom at index 'cgatomid'
    - this does not check whether this index is valid. */
void AtomSelection::_unsafe_deselect(const CGAtomID &cgatomid)
{
    if (this->selected(cgatomid))
    {
        if (this->nSelected() == 1)
        {
            //we are deselecting the only selected atom!
            this->deselectAll();
            return;
        }
        else if (this->selectedAll())
        {
            //all atoms have been selected  - we now need to populate
            //the selected_atoms hash...
            uint ncg = molinfo.nCutGroups();

            selected_atoms.reserve(ncg);

            for (CutGroupID i(0); i<ncg; ++i)
            {
                if (i != cgatomid.cutGroupID())
                {
                    //all atoms from this group are selected,
                    //this is represented by an empty hash
                    selected_atoms.insert( i, QSet<AtomID>() );
                }
            }

            //now insert the info for this CutGroup
            uint ncgatoms = molinfo.nAtoms(cgatomid.cutGroupID());

            if (ncgatoms > 1)
            {
                QSet<AtomID> atms;
                atms.reserve(ncgatoms-1);

                for (AtomID i(0); i<ncgatoms; ++i)
                {
                    if (i != cgatomid.atomID())
                        atms.insert(i);
                }

                selected_atoms.insert( cgatomid.cutGroupID(), atms );
            }

            --nselected;
            return;
        }
        else
        {
            if (this->nSelected(cgatomid.cutGroupID()) - 1 == 0)
            {
                //deselecting this atom will remove all atoms from
                //this CutGroup
                this->deselectAll(cgatomid.cutGroupID());
                return;
            }
            else
            {
                QHash< CutGroupID,QSet<AtomID> >::iterator it =
                                selected_atoms.find(cgatomid.cutGroupID());

                BOOST_ASSERT( it != selected_atoms.end() );

                if (it->isEmpty())
                {
                    uint ncgatoms = molinfo.nAtoms(cgatomid.cutGroupID());

                    //replace the empty hash (representing all selected atoms)
                    //with a hash containing all but the deselected atom
                    it->reserve(ncgatoms-1);

                    for (AtomID i(0); i<ncgatoms; ++i)
                    {
                        if (i != cgatomid.atomID())
                            it->insert(i);
                    }

                    --nselected;
                }
                else
                {
                    it->remove(cgatomid.atomID());
                    --nselected;
                }
            }
        }
    }
}

void AtomSelection::_pvt_deselect(const CGAtomID &cgatomid)
{
    info().assertAtomExists(cgatomid);
    this->_unsafe_deselect(cgatomid);
}

/** Select all of the atoms that are in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
void AtomSelection::_pvt_select(ResNum resnum)
{
    if ( not (this->selectedAll() or this->selectedAll(resnum)) )
    {
        ResidueInfo resinfo = molinfo[resnum];

        if (molinfo.nResidues() == 1)
        {
            //we selecting all of the atoms of the only residue!
            this->_pvt_selectAll();
            return;
        }
        else
        {
            uint nats = resinfo.nAtoms();

            for (AtomID i(0); i<nats; ++i)
            {
                this->_pvt_select( resinfo[i] );
            }
        }
    }
}

/** Select all of the atoms that are in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
AtomSelection AtomSelection::selectAll(ResNum resnum) const
{
    AtomSelection retval = *this;
    retval._pvt_select(resnum);

    return retval;
}

/** Deselect all of the atoms that are in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
void AtomSelection::_pvt_deselect(ResNum resnum)
{
    if ( not (this->isEmpty() or this->selectedNone(resnum)) )
    {
        ResidueInfo resinfo = molinfo[resnum];

        if (molinfo.nResidues() == 1)
        {
            //we are deselecting the only residue in the molecule!
            this->_pvt_deselectAll();
            return;
        }
        else
        {
            uint nats = resinfo.nAtoms();

            for (AtomID i(0); i<nats; ++i)
            {
                this->_pvt_deselect( resinfo[i] );
            }
        }
    }
}

/** Deselect all of the atoms that are in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
AtomSelection AtomSelection::deselectAll(ResNum resnum) const
{
    AtomSelection retval = *this;
    retval._pvt_deselect(resnum);

    return retval;
}

/** Select all of the atoms that are in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
AtomSelection AtomSelection::select(ResNum resnum) const
{
    return this->selectAll(resnum);
}

/** Deselect all of the atoms that are in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
AtomSelection AtomSelection::deselect(ResNum resnum) const
{
    return this->deselectAll(resnum);
}

/** Select only the atoms in the Residue with number 'resnum'

    \throw SireMol::missing_residue
*/
AtomSelection AtomSelection::setSelection(ResNum resnum) const
{
    return this->selectNone().select(resnum);
}

/** Select all of the atoms selected in 'selection'

    \throw SireError::incompatible_error
*/
void AtomSelection::_pvt_select(const AtomSelection &selection)
{
    this->assertCompatibleWith( selection.molinfo );

    if (selection.selectedAll())
    {
        this->_pvt_selectAll();
        return;
    }
    else if (selection.selectedNone())
    {
        return;
    }

    uint ncg = molinfo.nCutGroups();

    for (CutGroupID i(0); i<ncg; ++i)
    {
        if (selection.selectedAll(i))
            this->_pvt_select(i);
        else
        {
            uint nats = molinfo.nAtoms(i);

            for (AtomID j(0); j<nats; ++j)
            {
                CGAtomID cgatomid(i,j);

                if (selection.selected(cgatomid))
                    this->_unsafe_select(cgatomid);
            }
        }
    }
}

/** Select all of the atoms selected in 'selection'

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::selectAll(const AtomSelection &selection) const
{
    AtomSelection retval = *this;
    retval._pvt_select(selection);

    return retval;
}

/** Deselect all of the atoms selected in 'selection'

    \throw SireError::incompatible_error
*/
void AtomSelection::_pvt_deselect(const AtomSelection &selection)
{
    if (selection.selectedAll() or selection.contains(*this))
    {
        this->_pvt_deselectAll();
        return;
    }
    else if (selection.selectedNone() or this->selectedNone())
    {
        return;
    }

    uint ncg = molinfo.nCutGroups();

    for (CutGroupID i(0); i<ncg; ++i)
    {
        if (selection.selectedAll(i))
            this->_pvt_deselect(i);
        else if (not this->selectedNone(i))
        {
            uint nats = molinfo.nAtoms(i);

            for (AtomID j(0); j<nats; ++j)
            {
                CGAtomID cgatomid(i,j);

                if (selection.selected(cgatomid))
                    this->_unsafe_deselect(cgatomid);
            }
        }
    }
}

/** Deselect all of the atoms selected in 'selection'

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::deselectAll(const AtomSelection &selection) const
{
    AtomSelection retval = *this;
    retval._pvt_deselect(selection);

    return retval;
}

/** Select all of the atoms selected in 'selection'

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::select(const AtomSelection &selection) const
{
    return this->selectAll(selection);
}

/** Deselect all of the atoms selected in 'selection'

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::deselect(const AtomSelection &selection) const
{
    return this->deselectAll(selection);
}

/** Select only the atoms in the selection 'selection'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::setSelection(const AtomSelection &selection) const
{
    return this->selectNone().select(selection);
}

/** Select all of the atoms listed in 'selection'

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::selectAll(const SelectionFromMol &selection) const
{
    return selection.selectAllFrom(*this);
}

/** Deselect all of the atoms listed in 'selection'

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::deselectAll(const SelectionFromMol &selection) const
{
    return selection.deselectAllIn(*this);
}

/** Select all of the atoms listed in 'selection'

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::select(const SelectionFromMol &selection) const
{
    return this->selectAll(selection);
}

/** Deselect all of the atoms listed in 'selection'

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::deselect(const SelectionFromMol &selection) const
{
    return this->deselectAll(selection);
}

/** Select only the atoms in the selection 'selection'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::setSelection(const SelectionFromMol &selection) const
{
    return this->selectNone().select(selection);
}

void AtomSelection::_pvt_select(const SelectionFromMol &selection)
{
    *this = this->selectAll(selection);
}

void AtomSelection::_pvt_deselect(const SelectionFromMol &selection)
{
    *this = this->deselectAll(selection);
}

/** Select the atom at index 'cgatomid'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const CGAtomID &cgatomid) const
{
    AtomSelection retval = *this;
    retval._pvt_select(cgatomid);

    return retval;
}

void AtomSelection::_pvt_select(const IDMolAtom &atomid)
{
    this->_unsafe_select( atomid.index(molinfo) );
}

/** Select the atom with index 'atomid' */
AtomSelection AtomSelection::select(const IDMolAtom &atomid) const
{
    AtomSelection retval = *this;
    retval._pvt_select(atomid);

    return retval;
}

/** Deselect the atom at index 'cgatomid'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const CGAtomID &cgatomid) const
{
    AtomSelection retval = *this;
    retval._pvt_deselect(cgatomid);

    return retval;
}

/** Select only the atom with ID == cgatomid

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::setSelection(const CGAtomID &cgatomid) const
{
    return this->selectNone().select(cgatomid);
}

void AtomSelection::_pvt_deselect(const IDMolAtom &atomid)
{
    this->_unsafe_deselect( atomid.index(molinfo) );
}

/** Deselect the atom at index 'atomid' */
AtomSelection AtomSelection::deselect(const IDMolAtom &atomid) const
{
    AtomSelection retval = *this;
    retval._pvt_deselect(atomid);

    return retval;
}

/** Select only the atom with ID == atomid

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::setSelection(const IDMolAtom &atomid) const
{
    return this->selectNone().select(atomid);
}

/** Invert the current selection */
void AtomSelection::_pvt_invert()
{
    if (this->selectedAll())
    {
        this->deselectAll();
        return;
    }

    if (this->selectedNone())
    {
        this->selectAll();
        return;
    }

    uint ncg = molinfo.nCutGroups();

    for (CutGroupID i(0); i<ncg; ++i)
    {
        if (this->selectedAll(i))
            this->deselectAll(i);
        else if (this->selectedNone(i))
            this->selectAll(i);
        else
        {
            uint nats = molinfo.nAtoms(i);

            for (AtomID j(0); j<nats; ++j)
            {
                CGAtomID cgatomid(i,j);

                if (this->selected(cgatomid))
                    this->deselect(cgatomid);
                else
                    this->select(cgatomid);
            }
        }
    }
}

/** Return the inverse of the current selection */
AtomSelection AtomSelection::invert() const
{
    AtomSelection retval = *this;
    retval._pvt_invert();

    return retval;
}

/** Apply the mask of CutGroupIDs to this selection - this deselects
    every CutGroup that does not have its ID in 'cgids' */
void AtomSelection::_pvt_applyMask(const QSet<CutGroupID> &cgids)
{
    uint ncg = molinfo.nCutGroups();

    for (CutGroupID i(0); i<ncg; ++i)
    {
        if (not cgids.contains(i))
            this->deselectAll(i);
    }
}

/** Apply the mask of CutGroupIDs to this selection - this deselects
    every CutGroup that does not have its ID in 'cgids' */
AtomSelection AtomSelection::applyMask(const QSet<CutGroupID> &cgids) const
{
    AtomSelection retval = *this;
    retval._pvt_applyMask(cgids);

    return retval;
}

/** Apply the mask of residue numbers to this selection - this
    deselects every residue that does not have its number in 'resnums' */
void AtomSelection::_pvt_applyMask(const QSet<ResNum> &resnums)
{
    foreach (ResNum resnum, molinfo.residueNumbers())
    {
        if (not resnums.contains(resnum))
            this->deselectAll(resnum);
    }
}

/** Apply the mask of residue numbers to this selection - this
    deselects every residue that does not have its number in 'resnums' */
AtomSelection AtomSelection::applyMask(const QSet<ResNum> &resnums) const
{
    AtomSelection retval = *this;
    retval._pvt_applyMask(resnums);

    return retval;
}

/** Apply the mask of one AtomSelection to the other - this creates
    the intersection of the two AtomSelections

    \throw SireError::incompatible_error
*/
void AtomSelection::_pvt_applyMask(const AtomSelection &other)
{
    this->assertCompatibleWith(other.molinfo);

    if (other.selectedAll())
        return;

    if (other.selectedNone())
        this->deselectAll();

    uint ncg = molinfo.nCutGroups();

    for (CutGroupID i(0); i<ncg; ++i)
    {
        if (other.selectedAll(i))
            continue;
        else if (other.selectedNone(i))
            this->deselectAll(i);
        else
        {
            uint nats = molinfo.nAtoms(i);

            for (AtomID j(0); j<nats; ++j)
            {
                CGAtomID cgatomid(i,j);

                if (not other.selected(cgatomid))
                    this->deselect(cgatomid);
            }
        }
    }
}

/** Apply the mask of one AtomSelection to the other - this creates
    the intersection of the two AtomSelections

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::applyMask(const AtomSelection &selection) const
{
    AtomSelection retval = *this;
    retval._pvt_applyMask(selection);

    return retval;
}

/** Apply the mask from the selected atoms in 'selection'

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::applyMask(const SelectionFromMol &selection) const
{
    return this->applyMask( this->setSelection(selection) );
}

/** Return whether or not this selection contains any of the atoms
    selected in 'selection'

    \throw SireError::incompatible_error
*/
bool AtomSelection::intersects(const AtomSelection &selection) const
{
    this->assertCompatibleWith(selection);

    if (this->selectedAll() or selection.selectedAll())
        return true;
    else if (this->selectedNone() or selection.selectedNone())
        return false;
    else
    {
        QSet<CutGroupID> cgids = this->selectedCutGroups();

        foreach (CutGroupID cgid, cgids)
        {
            if (selection.selectedAll(cgid))
            {
                if (not selection.selectedNone(cgid))
                    return true;
            }
            else if (not selection.selectedNone(cgid))
            {
                uint nats = molinfo.nAtoms(cgid);

                for (AtomID i(0); i<nats; ++i)
                {
                    CGAtomID cgatomid(cgid,i);

                    if (selection.selected(cgatomid) and this->selected(cgatomid))
                        return true;
                }
            }
        }

        return false;
    }
}

/** Return whether or not this selection contains any of the atoms
    selected in 'selection'

    \throw SireError::incompatible_error
*/
bool AtomSelection::intersects(const SelectionFromMol &selection) const
{
    return this->intersects( this->setSelection(selection) );
}

/** Return whether or not this selection contains all of the atoms that
    are contained in 'selection'

    \throw SireError::incompatible_error
*/
bool AtomSelection::contains(const AtomSelection &selection) const
{
    this->assertCompatibleWith(selection);

    if (this->selectedAll())
        return true;
    else if (this->selectedNone())
        return selection.selectedNone();
    else if (this->nSelected() < selection.nSelected())
        return false;
    else
    {
        QSet<CutGroupID> cgids = selection.selectedCutGroups();

        foreach (CutGroupID cgid, cgids)
        {
            if (selection.selectedAll(cgid))
            {
                if (not this->selectedAll(cgid))
                    return false;
            }
            else
            {
                uint nats = molinfo.nAtoms(cgid);

                for (AtomID j(0); j<nats; ++j)
                {
                    CGAtomID cgatomid(cgid,j);

                    if (selection.selected(cgatomid))
                    {
                        if (not this->selected(cgatomid))
                            return false;
                    }
                }
            }
        }

        return true;
    }
}

/** Return whether or not this selection contains all of the atoms that
    are contained in 'selection'

    \throw SireError::incompatible_error
*/
bool AtomSelection::contains(const SelectionFromMol &selection) const
{
    return this->contains( this->setSelection(selection) );
}

/** Return the intersection of this selection with 'selection'. The
    returned selection will contain all atoms that are selected
    by both groups.

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::intersect(const AtomSelection &selection) const
{
    return this->applyMask(selection);
}

/** Return the intersection of this selection with 'selection'. The
    returned selection will contain all atoms that are selected
    by both groups.

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::intersect(const SelectionFromMol &selection) const
{
    return this->intersect( this->setSelection(selection) );
}

/** Return the union of this selection with 'selection'. The
    returned selection will contain all atoms that are in
    either or the two selections.

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::unite(const AtomSelection &selection) const
{
    return this->selectAll(selection);
}

/** Return the union of this selection with 'selection'. The
    returned selection will contain all atoms that are in
    either or the two selections.

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::unite(const SelectionFromMol &selection) const
{
    return this->unite( this->setSelection(selection) );
}

/** Return this selection minus 'selection' - this will return
    the atoms that are in this selection that are not in 'other'.

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::subtract(const AtomSelection &selection) const
{
    return this->deselectAll(selection);
}

/** Return this selection minus 'selection' - this will return
    the atoms that are in this selection that are not in 'other'.

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::subtract(const SelectionFromMol &selection) const
{
    return this->subtract( this->setSelection(selection) );
}

/** Assert that 'info' is compatible with this selection. This
    will throw an exception unless info == molinfo

    \throw SireError::incompatible_error
*/
void AtomSelection::assertCompatibleWith(const MoleculeInfo &info) const
{
    if (info != molinfo)
        throw SireError::incompatible_error( QObject::tr(
                "The passed Molecule is not compatible with this selection!"),
                      CODELOC );
}

/** Assert that 'molecule' is compatible with this selection */
void AtomSelection::assertCompatibleWith(const Molecule &molecule) const
{
    this->assertCompatibleWith(molecule.info());
}

/** Assert that 'selection' is compatible with this selection */
void AtomSelection::assertCompatibleWith(const AtomSelection &selection) const
{
    if (selection.molinfo != molinfo)
        throw SireError::incompatible_error( QObject::tr(
                "The passed selection is not compatible with this selection!"),
                      CODELOC );
}

/** Assert that 'selection' is compatible with this selection */
void AtomSelection::assertCompatibleWith(const SelectionFromMol &selection) const
{
    try
    {
        this->setSelection(selection);
    }
    catch(...)
    {
        throw SireError::incompatible_error( QObject::tr(
                "The passed selection is not compatible with this selection!"),
                      CODELOC );
    }
}

/** Return a list of all of the atoms that are contained in the this selection */
QSet<AtomIndex> AtomSelection::selected() const
{
    QSet<AtomIndex> all_atoms;

    //do this in residue/atom order - this is more natural for the user
    uint nres = molinfo.nResidues();

    for (ResID i(0); i<nres; ++i)
    {
        uint nats = molinfo.nAtoms(i);

        for (AtomID j(0); j<nats; ++j)
        {
            CGAtomID cgatomid = molinfo[ ResIDAtomID(i,j) ];

            if (this->selected(cgatomid))
                all_atoms.insert( molinfo.atom(cgatomid) );
        }
    }

    return all_atoms;
}

/** Return the CutGroupIDs of any CutGroups that have at least one
    selected atom */
QSet<CutGroupID> AtomSelection::selectedCutGroups() const
{
    if (this->selectedAll())
    {
        QSet<CutGroupID> cgids;

        uint ncg = molinfo.nCutGroups();

        if (ncg > 0)
        {
            cgids.reserve(ncg);

            for (CutGroupID i(0); i<ncg; ++i)
                cgids.insert(i);
        }

        return cgids;
    }
    else if (this->selectedNone())
    {
        return QSet<CutGroupID>();
    }
    else
        return selected_atoms.keys().toSet();
}

/** Return the residue numbers of any residues that have at least
    one selected atom */
QSet<ResNum> AtomSelection::selectedResidues() const
{
    if (this->selectedAll())
    {
        return QList<ResNum>::fromVector(molinfo.residueNumbers()).toSet();
    }
    else if (this->selectedNone())
    {
        return QSet<ResNum>();
    }
    else
    {
        QVector<ResNum> resnums = molinfo.residueNumbers();

        QSet<ResNum> selected_res;

        foreach (ResNum resnum, resnums)
        {
            if (this->nSelected(resnum) > 0)
                selected_res.insert(resnum);
        }

        return selected_res;
    }
}

/** Return whether or not only a single atom is selected */
bool AtomSelection::isSingleAtom() const
{
    return nSelected() == 1;
}

/** Return whether or not all atoms of only a single CutGroup is selected */
bool AtomSelection::isSingleCutGroup() const
{
    QSet<CutGroupID> cgids = this->selectedCutGroups();

    if (cgids.count() == 1)
        return this->selectedAll( *(cgids.constBegin()) );
    else
        return false;
}

/** Return whether or not all atoms of only a single residue is selected */
bool AtomSelection::isSingleResidue() const
{
    QSet<ResNum> resnums = this->selectedResidues();

    if (resnums.count() == 1)
        return this->selectedAll( *(resnums.constBegin()) );
    else
        return false;
}

/** Return the ID of the single selected atom, if this
    selection only represents a single atom. This
    will throw an exception unless isSingleAtom() returns
    true.

    \throw SireError::incompatible_error
*/
CGAtomID AtomSelection::asSingleAtom() const
{
    if (nSelected() != 1)
        throw SireError::incompatible_error( QObject::tr(
            "More than just a single atom is selected!"),
                CODELOC );

    if (selectedAll())
        //there is only a single atom in the molecule, and it
        //is selected!
        return CGAtomID( CutGroupID(0), AtomID(0) );
    else
    {
        QHash< CutGroupID,QSet<AtomID> >::const_iterator
                                  it = selected_atoms.constBegin();

        return CGAtomID( it.key(), *(it->constBegin()) );
    }
}

/** Return the ID of the single selected CutGroup, if this
    selection only represents a single CutGroup. This
    will throw an exception unless isSingleCutGroup() returns
    true.

    \throw SireError::incompatible_error
*/
CutGroupID AtomSelection::asSingleCutGroup() const
{
    if (not isSingleCutGroup())
        throw SireError::incompatible_error( QObject::tr(
            "All of the atoms of just one CutGroup have not been selected."),
                CODELOC );

    if (selected_atoms.isEmpty())
        //there is only one CutGroup in the molecule
        return CutGroupID(0);
    else
        return selected_atoms.constBegin().key();
}

/** Return the ID of the single selected Residue, if this
    selection only represents a single Residue. This
    will throw an exception unless isSingleResidue() returns
    true.

    \throw SireError::incompatible_error
*/
ResNum AtomSelection::asSingleResidue() const
{
    if (not isSingleResidue())
        throw SireError::incompatible_error( QObject::tr(
            "All of the atoms of just one Residue have not been selected."),
                CODELOC );

    if (selected_atoms.isEmpty())
        //there is only one Residue in the molecule
        return info().residueNumber( ResID(0) );
    else
        return this->selected().constBegin()->resNum();
}
