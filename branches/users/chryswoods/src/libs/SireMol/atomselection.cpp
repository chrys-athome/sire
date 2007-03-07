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

#include "atomselection.h"

#include "molecule.h"
#include "residue.h"
#include "newatom.h"
#include "moleculeinfo.h"
#include "residueinfo.h"
#include "resnum.h"
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

/** Construct a selection which initially represents the selection
    of all of the atoms in 'molecule' */
AtomSelection::AtomSelection(const Molecule &molecule)
              : molinfo(molecule.info()),
                nselected(molecule.nAtoms())
{}

/** Construct a selection which initially represents the selection
    of all of the atoms in 'residue' */
AtomSelection::AtomSelection(const Residue &residue)
              : molinfo(residue.molecule().info()),
                nselected(0)

{
    this->deselectAll();
    this->selectAll(residue.number());
}

/** Construct a selection which initially represents the selection
    of the atom 'atom' */
AtomSelection::AtomSelection(const NewAtom &atom)
              : molinfo(atom.molecule().info()),
                nselected(0)
{
    this->deselectAll();
    this->select(atom.cgAtomID());
}

/** Copy constructor */
AtomSelection::AtomSelection(const AtomSelection &other)
              : selected_atoms(other.selected_atoms),
                molinfo(other.molinfo),
                nselected(other.nselected)
{}

/** Destructor */
AtomSelection::~AtomSelection()
{}

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
            QSet<CutGroupID> cgids = molinfo[i].cutGroupIDs();

            foreach (CutGroupID cgid, cgids)
            {
                if (selected_atoms.contains(cgid))
                {
                    ++nres;
                    break;
                }
            }
        }

        return nres;
    }
}

/** Return whether or not all atoms in the molecule have
    been selected. */
bool AtomSelection::selectedAll() const
{
    return nselected != 0 and selected_atoms.isEmpty();
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

/** Return whether or not the atom with the index 'cgatomid'
    has been selected.

    \throw SireError::invalid_index
*/
bool AtomSelection::selected(const CGAtomID &cgatomid) const
{
    molinfo.assertAtomExists(cgatomid);

    return _pvt_selected(cgatomid);
}

/** Select all of the atoms in the molecule */
void AtomSelection::selectAll()
{
    selected_atoms.clear();
    nselected = molinfo.nAtoms();
}

/** Deselect all of the atoms in the molecule */
void AtomSelection::deselectAll()
{
    selected_atoms.clear();
    nselected = 0;
}

/** Select all of the atoms in the CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
void AtomSelection::selectAll(CutGroupID cgid)
{
    molinfo.assertCutGroupExists(cgid);

    if (not this->selectedAll())
    {
        //how many atoms from this CutGroup have already been selected?
        int nats = selected_atoms.value(cgid).count();

        //calculate the number of atoms selected after this change
        nselected += (molinfo.nAtoms(cgid) - nats);

        if (nselected == molinfo.nAtoms())
            //all of the atoms have now been selected!
            selected_atoms.clear();
        else
            //replace any existing set of AtomIDs with an empty
            //set (the empty set implies all atoms are selected)
            selected_atoms.insert( cgid, QSet<AtomID>() );
    }
}

/** Deselect all of the atoms in the CutGroup with ID == cgid

    \throw SireError::invalid_index
*/
void AtomSelection::deselectAll(CutGroupID cgid)
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

/** Internal function used to select the atom at index 'cgatomid'
    - this does not check whether this index is valid. */
void AtomSelection::_pvt_select(const CGAtomID &cgatomid)
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

/** Internal function used to deselect the atom at index 'cgatomid'
    - this does not check whether this index is valid. */
void AtomSelection::_pvt_deselect(const CGAtomID &cgatomid)
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

/** Select all of the atoms that are in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
void AtomSelection::selectAll(ResNum resnum)
{
    ResidueInfo resinfo = molinfo[resnum];

    if (not this->selectedAll())
    {
        if (molinfo.nResidues() == 1)
        {
            //we selecting all of the atoms of the only residue!
            this->selectAll();
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

/** Deselect all of the atoms that are in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
void AtomSelection::deselectAll(ResNum resnum)
{
    ResidueInfo resinfo = molinfo[resnum];

    if (not this->isEmpty())
    {
        if (molinfo.nResidues() == 1)
        {
            //we are deselecting the only residue in the molecule!
            this->deselectAll();
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

/** Select the atom at index 'cgatomid'

    \throw SireError::invalid_index
*/
void AtomSelection::select(const CGAtomID &cgatomid)
{
    molinfo.assertAtomExists(cgatomid);
    this->_pvt_select(cgatomid);
}

/** Deselect the atom at index 'cgatomid'

    \throw SireError::invalid_index
*/
void AtomSelection::deselect(const CGAtomID &cgatomid)
{
    molinfo.assertAtomExists(cgatomid);
    this->_pvt_deselect(cgatomid);
}

/** Invert the current selection */
void AtomSelection::invert()
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

/** Apply the mask of CutGroupIDs to this selection - this deselects
    every CutGroup that does not have its ID in 'cgids' */
void AtomSelection::applyMask(const QSet<CutGroupID> &cgids)
{
    uint ncg = molinfo.nCutGroups();

    for (CutGroupID i(0); i<ncg; ++i)
    {
        if (not cgids.contains(i))
            this->deselectAll(i);
    }
}

/** Apply the mask of residue numbers to this selection - this
    deselects every residue that does not have its number in 'resnums' */
void AtomSelection::applyMask(const QSet<ResNum> &resnums)
{
    foreach (ResNum resnum, molinfo.residueNumbers())
    {
        if (not resnums.contains(resnum))
            this->deselectAll(resnum);
    }
}

/** Apply the mask of one AtomSelection to the other - this creates
    the union of the two AtomSelections

    \throw SireError::incompatible_error
*/
void AtomSelection::applyMask(const AtomSelection &other)
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

/** Assert that 'info' is compatible with this selection. This
    will throw an exception unless info == molinfo

    \throw SireError::incompatible_error
*/
void AtomSelection::assertCompatibleWith(const MoleculeInfo &info) const
{
    if (info != molinfo)
        throw SireError::incompatible_error( QObject::tr(
                "The passed Molecule is not compatible with this selection!"), CODELOC );
}

/** Assert that 'molecule' is compatible with this selection */
void AtomSelection::assertCompatibleWith(const Molecule &molecule) const
{
    this->assertCompatibleWith(molecule.info());
}
