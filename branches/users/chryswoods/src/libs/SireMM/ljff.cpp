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

#include "qhash_siremol.h"
#include "qhash_sirecas.h"

#include <QSharedData>

#include "ljff.h"

using namespace SireFF;

/////////////
///////////// Implementation of LJFF::Components
/////////////

/////////////
///////////// Implementation of LJFF::Parameters
/////////////

/////////////
///////////// Implementation of LJFF::Groups
/////////////

/////////////
///////////// Implementation of LJFF::LJMoleculeData
/////////////

namespace SireMM
{

/** Private class used to hold the data of LJFF::LJMolecule */
class LJFF::LJMoleculeData : public QSharedData
{
public:
    LJMoleculeData();

    LJMoleculeData(const Molecule &mol, const QString &ljproperty);
    LJMoleculeData(const Residue &residue, const QString &ljproperty);
    LJMoleculeData(const NewAtom &atom, const QString &ljproperty);
    LJMoleculeData(const Molecule &mol, const AtomSelection &selectedatoms,
                   const QString &ljproperty);

    LJMoleculeData(const LJMoleculeData &other);

    ~LJMoleculeData();

    void rebuildAll();
    void rebuildCoordinates();

    bool rebuildAll(const QSet<CutGroupID> &cgids);
    bool rebuildCoordinates(const QSet<CutGroupID> &cgids);

    /** The actual molecule */
    Molecule molecule;

    /** The name of the property associated with the LJ parameters */
    QString lj_property;

    /** The atoms selected for inclusion in this forcefield */
    AtomSelection selected_atoms;

    /** Hash mapping the CutGroupID of a selected cutgroup to
        the index in the coords and ljs array for its coordinates
        and LJ parameters. If this is empty then all of the
        CutGroups in the molecule have been selected. */
    QHash<CutGroupID,int> cg_index;

    /** The coordinates of the CutGroups that contain atoms that
        are selected for inclusion in the LJ forcefield.  */
    QVector<CoordGroup> coords;

    /** The LJ parameters of the atoms, in the same order as
        coords. Atoms that are not selected for this forcefield
        have a zero LJ parameter. */
    AtomicLJs ljs;
};

/** Constructor */
LJFF::LJMoleculeData::LJMoleculeData() : QSharedData()
{}

/** Construct to represent all of the molecule 'mol' */
LJFF::LJMoleculeData::LJMoleculeData(const Molecule &mol,
                                     const QString &ljproperty)
                     : QSharedData(),
                       molecule(mol),
                       lj_property(ljproperty),
                       selected_atoms(mol)
{
    this->rebuildAll();
}

/** Construct to represent all of the residue 'residue' */
LJFF::LJMoleculeData::LJMoleculeData(const Residue &residue,
                                     const QString &ljproperty)
                     : QSharedData(),
                       molecule(residue.molecule()),
                       lj_property(ljproperty),
                       selected_atoms(residue)
{
    this->rebuildAll();
}

/** Construct to represent the atom 'atom' */
LJFF::LJMoleculeData::LJMoleculeData(const NewAtom &atom,
                                     const QString &ljproperty)
                     : QSharedData(),
                       molecule(atom.molecule()),
                       lj_property(ljproperty),
                       selected_atoms(atom)
{
    this->rebuildAll();
}

/** Construct to represent the selected atoms from 'selectatoms' that are
    in the molecule 'mol' */
LJFF::LJMoleculeData::LJMoleculeData(const Molecule &mol,
                                     const AtomSelection &selectatoms,
                                     const QString &ljproperty)
                     : QSharedData(),
                       molecule(mol),
                       lj_property(ljproperty),
                       selected_atoms(selectatoms)
{
    //ensure that the selected atoms and the molecule are compatible
    selected_atoms.assertCompatibleWith(molecule);

    this->rebuildAll();
}

/** Copy constructor */
LJFF::LJMoleculeData::LJMoleculeData(const LJFF::LJMoleculeData &other)
                     : QSharedData(),
                       molecule(other.molecule),
                       lj_property(other.lj_property),
                       selected_atoms(other.selected_atoms),
                       cg_index(other.cg_index),
                       coords(other.coords),
                       ljs(other.ljs)
{}

/** Destructor */
LJFF::LJMoleculeData::~LJMoleculeData()
{}

/** Rebuild all of the coordinate and LJ data from scratch

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
void LJFF::LJMoleculeData::rebuildAll()
{
    if (selected_atoms.selectedAll())
    {
        //all atoms are selected - we can therefore short-circuit everything!
        ljs = molecule.getProperty(lj_property);
        cg_index.clear();
        coords = molecule.coordGroups();

        return;
    }

    if (selected_atoms.isEmpty())
    {
        //there has been nothing selected!
        coords.clear();
        ljs.clear();
        cg_index.clear();

        return;
    }

    uint nselectedgroups = selected_atoms.nSelectedCutGroups();
    uint ngroups = molecule.nCutGroups();

    if (nselectedgroups == ngroups)
    {
        coords = molecule.coordGroups();
        all_ljs = molecule.getProperty(lj_property);
        cg_index.clear();

        //loop through all CutGroups and zero the parameters of any
        //atoms that are not selected
        for (CutGroupID i(0); i<ngroups; ++i)
        {
            if ( not selected_atoms.selectedAll(i) )
            {
                QVector<LJParameter> &ljparams = ljs[i];

                //there are some missing parameters to zero!
                uint nats = molecule.nAtoms(i);

                for (AtomID j(0); j<nats; ++j)
                {
                    if ( not selected_atoms.selected(CGAtomID(i,j)) )
                        //this atom has not been selected - zero its LJ parameter
                        ljparams[j] = LJParameter::dummy();
                }
            }
        }

        return;
    }

    const QVector<CoordGroup> &all_coords = molecule.coordGroups();
    AtomicLJs all_ljs = molecule.getProperty(lj_property);

    coords = QVector<CoordGroup>( nselectedgroups );
    ljs = AtomicLJs( QVector<LJParameter>( nselectedgroups ) );

    cg_index.clear();
    cg_index.reserve(nselectedgroups);

    int idx = 0;

    for (CutGroupID i(0); i<ngroups; ++i)
    {
        if (not selected_atoms.selectedNone(i))
        {
            coords[idx] = all_coords[i];
            ljs[idx] = all_ljs[i];

            cg_index.insert(i, idx);
            ++idx;

            if (not selected_atoms.selectedAll(i))
            {
                uint nats = molecule.nAtoms(i);

                QVector<LJParameter> &ljparams = ljs[idx];

                for (AtomID j(0); j<nats; ++j)
                {
                    if ( not selected_atoms.selected(CGAtomID(i,j)) )
                        ljparams[j] = LJParameter::dummy();
                }
            }
        }
    }
}

/** Rebuild all of the coordinates and LJ data for the CutGroups whose
    IDs are in 'cgids'. Return whether or not there are any atoms
    from these CutGroups that are in the molecule selection */
bool LJFF::LJMoleculeData::rebuildAll(const QSet<CutGroupID>&)
{
    //This can be heavily optimised - I just don't have the time now!
    this->rebuildAll();
    return true;
}

/** Rebuild all of the coordinate data from scratch */
void LJFF::LJMoleculeData::rebuildCoordinates()
{
    if (selected_atoms.selectedAll())
    {
        coords = molecule.coordGroups();
        return;
    }

    uint nselectedgroups = selected_atoms.nSelectedCutGroups();
    uint ngroups = molecule.nCutGroups();

    if (nselectedgroups == ngroups)
    {
        coords = molecule.coordGroups();
        return;
    }

    BOOST_ASSERT( nselectedgroups == cg_index.count() );
    BOOST_ASSERT( nselectedgroups == coords.count() );
    BOOST_ASSERT( nselectedgroups == ljs.count() );

    const QVector<CoordGroup> &new_coords = molecule.coordGroups();

    for (QHash<CutGroupID,int>::const_iterator it = cg_index.constBegin();
         it != cg_index.constEnd();
         ++it)
    {
        coords[ it.value() ] = new_coords[it.key()];
    }
}

/** Rebuild all of the coordinate data for the CutGroups whose IDs are
    in 'cgids'. Return whether or not the passed CutGroups are actually
    part of the selected parts of the molecule. */
bool LJFF::LJMoleculeData::rebuildCoordinates(const QSet<CutGroupID> &cgids)
{
    if (selected_atoms.selectedAll())
    {
        coords = molecule.coordGroups();
        return true;
    }

    uint nselectedgroups = selected_atoms.nSelectedCutGroups();
    uint ngroups = molecule.nCutGroups();

    if (nselectedgroups == ngroups)
    {
        coords = molecule.coordGroups();
        return true;
    }

    BOOST_ASSERT( nselectedgroups == cg_index.count() );
    BOOST_ASSERT( nselectedgroups == coords.count() );
    BOOST_ASSERT( nselectedgroups == ljs.count() );

    const QVector<CoordGroup> &new_coords = molecule.coordGroups();

    bool cutgroups_in_selection = false;

    for (QSet<CutGroupID>::const_iterator it = cgids.begin();
         it != cgids.end();
         ++it)
    {
        QHash<CutGroupID,int>::const_iterator idx = cg_index.constFind(*it);

        if (idx != cg_index.constEnd())
        {
            cutgroups_in_selection = true;
            coords[idx.value()] = new_coords[idx.key()];
        }
    }

    return cutgroups_in_selection;
}


/////////////
///////////// Implementation of LJFF::LJMolecule
/////////////

/** Shared null LJMoleculeData */
static QSharedDataPointer<LJFF::LJMoleculeData> shared_null(
                                                    new LJFF::LJMoleculeData() );

/** Null constructor - creates an empty LJMolecule */
LJFF::LJMolecule::LJMolecule() : d(shared_null)
{}

/** Construct from the passed molecule, using 'ljproperty' to find the
    molecule's LJ parameters.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
LJFF::LJMolecule::LJMolecule(const Molecule &molecule, const QString &ljproperty)
                 : d( new LJFF:LJMoleculeData(molecule,ljproperty) )
{}

/** Construct from the passed residue, using 'ljproperty' to find the
    residue's LJ parameters.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
LJFF::LJMolecule::LJMolecule(const Residue &residue, const QString &ljproperty)
                 : d( new LJFF::LJMoleculeData(residue,ljproperty) )
{}

/** Construct from the passed atom, using 'ljproperty' to find the
    atom's LJ parameters.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
LJFF::LJMolecule::LJMolecule(const NewAtom &atom, const QString &ljproperty)
                 : d( new LJFF::LJMoleculeData(atom,ljproperty) )
{}

/** Construct from the selected atoms of the passed molecule, using
    'ljproperty' to find the atoms' LJ parameters.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
LJFF::LJMolecule::LJMolecule(const Molecule &molecule,
                             const AtomSelection &selected_atoms,
                             const QString &ljproperty)
                 : d( new LJFF::LJMoleculeData(molecule,selected_atoms,ljproperty) )
{}

/** Construct a copy of 'other', but masked by the CutGroups in 'groups' */
LJFF::LJMolecule::LJMolecule(const LJMolecule &other, const QSet<CutGroupID> &groups)
                 : d( other.d )
{
    //get the index of which CutGroups are already selected
    const QHash<CutGroupID,int> index = other.d->cg_index;

    if (index.isEmpty())
    {
        //an empty index implies that the entire molecule is selected
        // - we therefore need only select the CutGroups in 'groups'
        const AtomicLJs &ljs = other.d->ljs;
        const QVector<CoordGroup> &coords = other.d->coords;

        int ngroups = groups.count();

        AtomicLJs new_ljs(ngroups);
        QVector<CoordGroup> new_coords(ngroups);

        QHash<CutGroupID,int> new_index;
        new_index.reserve(ngroups);

        int i = 0;

        foreach (CutGroupID cgid, groups)
        {
            int idx = index[cgid];

            new_coords[i] = coords[idx];
            new_ljs[i] = ljs[i];

            new_index.insert(cgid,i);
            ++i;
        }

        d->coords = new_coords;
        d->ljs = new_ljs;
        d->cg_index = new_index;

        d->selected_atoms.applyMask(groups);
    }
    else
    {
        //run through the CutGroups in the molecule and see which ones need to
        //be removed
        QList<int> indicies_to_be_removed;

        for (QHash<CutGroupID,int>::const_iterator it = index.constBegin();
             it != index.constEnd();
             ++it)
        {
            if ( not groups.contains(it.key()) )
            {
                //this CutGroup will be masked - deselect it and
                //record the index of its coordinates and LJ parameters
                d->selected_atoms.deselectAll(it.key());
                indicies_to_be_removed.insert(it.value());
            }
        }

        //remove the identified CutGroups (from back to front)
        qSort(indicies_to_be_removed.begin(), indicies_to_be_removed.end(),
              qGreater<int>());

        foreach (idx, indicies_to_be_removed)
        {
            d->coords.removeAt(idx);
            d->ljs.removeAt(idx);
        }
    }
}

/** Copy constructor */
LJFF::LJMolecule::LJMolecule(const LJMolecule &other)
                 : d( other.d )
{}

/** Destructor */
LJFF::LJMolecule::~LJMolecule()
{}

/** Assignment operator */
LJMolecule& LJFF::LJMolecule::operator=(const LJMolecule &other)
{
    d = other.d;
    return *this;
}

/** Return whether or not this is empty (has no atoms selected */
bool LJFF::LJMolecule::isEmpty() const
{
    return d->selected_atoms.isEmpty();
}

/** Return the actual molecule */
const Molecule& LJFF::LJMolecule::molecule() const
{
    return d->molecule;
}

/** Return a ChangedLJMolecule that represents the change from the
    LJMolecule in its current state to 'molecule', where only the
    CutGroups whose IDs are in 'cgids' have changed. (if cgids is
    empty then all CutGroups have changed)

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::_pvt_change(const Molecule &molecule,
                                                const QSet<CutGroupID> &cgids) const
{
    //assert that this is the same molecule
    d->molecule.assertSameMolecule(molecule);

    //has there been a change?
    if ( d->molecule.version() == molecule.version() )
        //no - there hasn't!
        return ChangedLJMolecule();

    //the molecule may have changed
    LJMolecule newmol(*this);

    newmol.molecule = molecule;

    if (cgids.isEmpty())
    {
        //the whole molecule has changed

        if (molecule.majorVersion() != d->molecule.majorVersion())
        {
            //there has been a major change
            newmol.rebuildAll();
        }
        else
        {
            //only the coordinates have changed
            newmol.rebuildCoordinates();
        }

        return ChangedLJMolecule(*this, newmol);
    }
    else
    {
        //only part of the molecule has changed
        if (molecule.majorVersion() != d->molecule.majorVersion())
        {
            //there has been a major change

            if (newmol.rebuildAll(cgids))
                //the part of the molecule in this forcefield has changed!
                return ChangedLJMolecule(*this, newmol, cgids);
            else
                //no part of the molecule that was in this forcefield has changed
                return ChangedLJMolecule();
        }
        else
        {
            //only the coordinates have changed

            if (newmol.rebuildCoordinates(cgids))
                //the part of the molecule in this forcefield has moved
                return ChangedLJMolecule(*this, newmol, cgids);
            else
                //no part of the molecule in this forcefield has moved
                return ChangedLJMolecule();
        }
    }
}

/** Return a ChangedLJMolecule that represents the change from the
    LJMolecule in its current state to 'molecule'. Note that
    the new state must have the same MoleculeID as the current state!

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::change(const Molecule &molecule) const
{
    return this->_pvt_change(molecule, QSet<CutGroupID>());
}

/** Return a ChangedLJMolecule that represents the change from the
    LJMolecule in its current state to include the changes in 'residue'

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::change(const Residue &residue) const
{
    return this->_pvt_change(residue.molecule(), residue.cutGroupIDs());
}

/** Return a ChangedLJMolecule that represents the change from
    the LJMolecule in its current state to include the changes in 'atom'

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::change(const NewAtom &newatom) const
{
    QSet<CutGroupID> cgid;
    cgid.insert(newatom.cutGroupID());

    return this->_pvt_change(atom.molecule(), cgid);
}

/** Private function used to return the ChangedLJMolecule that represents
    the change from the LJMolecule in its current state to 'molecule', with
    the new selection 'selected_atoms', with the guarantee that only the CutGroups
    whose IDs are in 'cgids' have changed. (if cgids is empty then all of the
    CutGroups have changed)

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::_pvt_change(const Molecule &molecule,
                                                const QSet<CutGroupID> &cgids,
                                                const AtomSelection &selected_atoms) const
{
    if (selected_atoms == d->selected_atoms)
        //there has been no change in the atom selections
        return this->_pvt_change(molecule, cgids);

    d->molecule.assertSameMolecule(molecule);

    LJMolecule newmol(*this);

    newmol.molecule = molecule;
    newmol.selected_atoms = selected_atoms;

    if (cgids.isEmpty())
    {
        //the entire molecule has changed
        newmol.rebuildAll();

        return ChangedLJMolecule(*this, newmol);
    }
    else
    {
        //only parts of the molecule have changed
        newmol.rebuildAll(cgids);

        return ChangedLJMolecule(*this, newmol, cgids);
    }
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to include the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::add(const Molecule &molecule) const
{
    return this->_pvt_change(molecule, QSet<CutGroupID>(), AtomSelection(molecule));
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to include as well the passed residue, which itself may
    have changed

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::add(const Residue &residue) const
{
    AtomSelection new_selection = d->selected_atoms;

    new_selection.selectAll(residue.number());

    return this->_pvt_change(residue.molecule(), residue.cutGroupIDs(), new_selection);
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to include as well the passed atom, which itself may
    have changed

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::add(const NewAtom &newatom) const
{
    AtomSelection new_selection = d->selected_atoms;

    new_selection.select(newatom.cgAtomID());

    QSet<CutGroupID> cgid;
    cgid.insert(newatom.cutGroupID());

    return this->_pvt_change(atom.molecule(), cgid, new_selection);
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to include as well the passed selection

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::add(const AtomSelection &selected_atoms) const
{
    AtomSelection new_selection = d->selected_atoms;

    new_selection.selectAll(selected_atoms);

    Molecule oldmol = d->molecule;

    return this->_pvt_change(oldmol, QSet<CutGroupID>(), new_selection);
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to remove the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::remove(const Molecule &molecule) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselectAll();

    return this->_pvt_change(molecule, QSet<CutGroupID>(), new_selection);
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to remove the passed residue, which itself may
    have changed

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::remove(const Residue &residue) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselectAll(residue.number());

    return this->_pvt_change(residue.molecule(), residue.cutGroupIDs(), new_selection);
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to remove the passed atom, which itself may
    have changed

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::remove(const NewAtom &atom) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselect(atom.cgAtomID());

    QSet<CutGroupID> cgid;
    cgid.insert(atom.cutGroupID());

    return this->_pvt_change(atom.molecule(), cgid, new_selection);
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to remove the passed atom selection

    \throw SireError::incompatible_error
*/
ChangedLJMolecule LJFF::LJMolecule::remove(const AtomSelection &selected_atoms) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselectAll(selected_atoms);

    Molecule oldmol = d->molecule;

    return this->_pvt_change(oldmol, QSet<CutGroupID>(), new_selection);
}

/** Return the name of the property used to get the LJ parameters */
const QString& LJFF::LJMolecule::ljProperty() const
{
    return d->lj_property;
}

/** Return the CoordGroups of the CutGroups that have atoms that are
    in this forcefield. Note that there is no defined order to the
    array of CoordGroups, other than it is the same order as the
    array of LJ parameter groups returned by ljParameters(). */
const QVector<CoordGroup>& LJFF::LJMolecule::coordinates() const
{
    return d->coords;
}

/** Return the LJ parameters of all of the atoms that are in
    this forcefield. The parameters are arranged by CutGroup and
    are in the same order as the coordinates as returned by
    coordinates(). Atoms which are not in this forcefield
    have zero LJ parameters. */
const AtomicLJs& LJFF::LJMolecule::ljParameters() const
{
    return d->ljs;
}

/** Return whether or not this LJMolecule represents the whole
    of the molecule */
bool LJFF::LJMolecule::isWholeMolecule() const
{
    return d->selected_atoms.selectedAll();
}

/** Return the selection of atoms that are present from this
    molecule in the forcefield */
const AtomSelection& LJFF::LJMolecule::selectedAtoms() const
{
    return d->selected_atoms;
}

/////////////
///////////// Implementation of LJFF::ChangedLJMolecule
/////////////

/** Empty constructor - this represents no change */
LJFF::ChangedLJMolecule::ChangedLJMolecule()
{}

/** Construct the change of all of 'old_molecule' to all of 'new_molecule' */
LJFF::ChangedLJMolecule::ChangedLJMolecule(const LJMolecule &old_molecule,
                                           const LJMolecule &new_molecule)
                        : oldmol(old_molecule),
                          newmol(new_molecule),
                          oldparts(old_molecule),
                          newparts(new_molecule)
{}

/** Construct the change of 'old_molecule' to 'new_molecule', where only the
    CutGroups whose IDs are in 'cgids' are known to change */
LJFF::ChangedLJMolecule::ChangedLJMolecule(const LJMolecule &old_molecule,
                                           const LJMolecule &new_molecule,
                                           const QSet<CutGroupID> &changed_groups)
                        : oldmol(old_molecule),
                          newmol(new_molecule),
                          changed_cgids(changed_groups)
{
    if (changed_groups.count() == old_molecule.nCutGroups())
    {
        //the entire molecule has changed
        oldparts = oldmol;
        newparts = newmol;
        changed_cgids.clear();
    }
    else
    {
        oldparts = LJMolecule(oldmol, changed_cgids);
        newparts = LJMolecule(newmol, changed_cgids);
    }
}

/** Copy constructor */
LJFF::ChangedLJMolecule::ChangedLJMolecule(const LJFF::ChangedLJMolecule &other)
                        : oldmol(other.oldmol),
                          newmol(other.newmol),
                          oldparts(other.oldparts),
                          newparts(other.newparts),
                          changed_cgids(other.changed_cgids)
{}

/** Destructor */
LJFF::ChangedLJMolecule::~ChangedLJMolecule()
{}

/** Assignment operator */
ChangedLJMolecule&
LJFF::ChangedLJMolecule::operator=(const LJFF::ChangedLJMolecule &other)
{
    oldmol = other.oldmol;
    newmol = other.newmol;
    oldparts = other.oldparts;
    newparts = other.newparts;
    changed_cgids = other.changed_cgids;

    return *this;
}

/** Comparison operator */
bool LJFF::ChangedLJMolecule::operator==(const LJFF::ChangedLJMolecule &other) const
{
    return oldmol == other.oldmol and
           newmol == other.newmol and
           changed_cgids == other.changed_cgids;
}

/** Comparison operator */
bool LJFF::ChangedLJMolecule::operator!=(const LJFF::ChangedLJMolecule &other) const
{
    return oldmol != other.oldmol or
           newmol != other.newmol or
           changed_cgids != other.changed_cgids;
}

/** Return whether or not this is empty (both old and new are empty) */
bool LJFF::ChangedLJMolecule::isEmpty() const
{
    return oldmol.isEmpty() and newmol.isEmpty();
}

/** Return whether the entire molecule has changed */
bool LJFF::ChangedLJMolecule::changedAll() const
{
    //everything has changed if changed_cgids is empty
    return changed_cgids.isEmpty();
}

/** Return the CutGroupIDs of all CutGroups that have changed since the
    last energy evaluation. */
const QSet<CutGroupID>& LJFF::ChangedLJMolecule::changedGroups() const
{
    return changed_cgids;
}

/** Return the whole LJMolecule as it was in the old state */
const LJMolecule& LJFF::ChangedLJMolecule::oldMolecule() const
{
    return oldmol;
}

/** Return the whole LJMolecule as it is in the new state */
const LJMolecule& LJFF::ChangedLJMolecule::newMolecule() const
{
    return newmol;
}

/** Return the parts of the LJMolecule in the old state that
    have changed compared to the new state */
const LJMolecule& LJFF::ChangedLJMolecule::oldParts() const
{
    return oldparts;
}

/** Return the parts of the LJMolecule in the new state that
    have changed compared to the old state */
const LJMolecule& LJFF::ChangedLJMolecule::newParts() const
{
    return newparts;
}

/** Return the ChangedLJMolecule that represents the change from the old molecule
    to 'molecule' */
ChangedLJMolecule LJFF::ChangedLJMolecule::change(const Molecule &molecule) const
{
    //calculate the change from newmol to molecule...
    ChangedLJMolecule next_change = newmol.change(molecule);

    if (next_change.isEmpty())
        //there was no further change
        return *this;
    else
        //now return the change from oldmol to newmol
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
}

/** Return the ChangedLJMolecule that represents the change from the old molecule
    to 'residue' */
ChangedLJMolecule LJFF::ChangedLJMolecule::change(const Residue &residue) const
{
    if (this->changedAll())
        return this->change(residue.molecule());

    //calculate the change from newmol to 'residue'...
    ChangedLJMolecule next_change = newmol.change(residue);

    if (next_change.isEmpty())
        return *this;
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the change from the old molecule
    to 'residue' */
ChangedLJMolecule LJFF::ChangedLJMolecule::change(const NewAtom &atom) const
{
    if (this->changedAll())
        return this->change(atom.molecule());

    ChangedLJMolecule next_change = newmol.change(atom);

    if (next_change.isEmpty())
        return *this;
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the addition of all atoms
    in the molecule 'molecule' */
ChangedLJMolecule LJFF::ChangedLJMolecule::add(const Molecule &molecule) const
{
    ChangedLJMolecule next_change = newmol.add(molecule);

    if (next_change.isEmpty())
        return *this;
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
}

/** Return the ChangedLJMolecule that represents the addition of all atoms
    in the residue 'residue' */
ChangedLJMolecule LJFF::ChangedLJMolecule::add(const Residue &residue) const
{
    ChangedLJMolecule next_change = newmol.add(residue);

    if (next_change.isEmpty())
        return *this;
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the addition of the atom 'atom' */
ChangedLJMolecule LJFF::ChangedLJMolecule::add(const NewAtom &atom) const
{
    ChangedLJMolecule next_change = newmol.add(atom);

    if (next_change.isEmpty())
        return *this;
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the addition of the selected atoms */
ChangedLJMolecule LJFF::ChangedLJMolecule::add(
                                      const AtomSelection &selected_atoms) const
{
    ChangedLJMolecule next_change = newmol.add(selected_atoms);

    if (next_change.isEmpty())
        return *this;
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the removal of the entire molecule! */
ChangedLJMolecule LJFF::ChangedLJMolecule::remove(const Molecule &molecule) const
{
    ChangedLJMolecule next_change = newmol.remove(molecule);

    if (next_change.isEmpty())
        return *this;
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
}

/** Return the ChangedLJMolecule that represents the removal of the residue 'residue' */
ChangedLJMolecule LJFF::ChangedLJMolecule::remove(const Residue &residue) const
{
    ChangedLJMolecule next_change = newmol.remove(residue);

    if (next_change.isEmpty())
        return *this;
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the removal of 'atom' */
ChangedLJMolecule LJFF::ChangedLJMolecule::remove(const NewAtom &atom) const
{
    ChangedLJMolecule next_change = newmol.remove(atom);

    if (next_change.isEmpty())
        return *this;
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.changedMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the removal of the
    selected atoms. */
ChangedLJMolecule LJFF::ChangedLJMolecule::remove(
                                      const AtomSelection &selected_atoms) const
{
    ChangedLJMolecule next_change = newmol.remove(selected_atoms);

    if (next_change.isEmpty())
        return *this;
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.changedMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/////////////
///////////// Implementation of LJFF
/////////////
