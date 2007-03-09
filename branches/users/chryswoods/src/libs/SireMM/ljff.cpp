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
#include "SireCAS/qhash_sirecas.h"

#include <QSharedData>

#include "ljff.h"

#include "SireMol/residueinfo.h"
#include "SireMol/cgatomid.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireFF;
using namespace SireMM;
using namespace SireMol;
using namespace SireStream;

/////////////
///////////// Implementation of LJFF::Components
/////////////

/** Constructor */
LJFF::Components::Components() : FFBase::Components()
{}

/** Construct using the supplied forcefield */
LJFF::Components::Components(const FFBase &ffbase, const Symbols &symbols)
     : FFBase::Components( ffbase, addToSymbols(symbols, x(), y(), z()) ),
       e_lj(ffbase, "lj", x(), y(), z())
{
    this->registerComponent(e_lj);
}

/** Copy constructor */
LJFF::Components::Components(const LJFF::Components &other)
     : FFBase::Components(other),
       e_lj(other.e_lj)
{}

/** Destructor */
LJFF::Components::~Components()
{}

/** Assignment operator */
LJFF::Components& LJFF::Components::operator=(const LJFF::Components &other)
{
    e_lj = other.e_lj;

    FFBase::Components::operator=(other);

    return *this;
}

/** Set the forcefield */
void LJFF::Components::setForceField(const FFBase &ffbase)
{
    *this = LJFF::Components(ffbase);
}

/** Describe the LJ component */
QString LJFF::Components::describe_lj()
{
    return QObject::tr("The total Lennard-Jones (van der waals) energy of the forcefield.");
}

/////////////
///////////// Implementation of LJFF::Parameters
/////////////

/** Constructor - by default the LJ parameters
    come from the 'ljs' property */
LJFF::Parameters::Parameters()
     : FFBase::Parameters(), lj_params("lj", "ljs")
{}

/** Copy constructor */
LJFF::Parameters::Parameters(const LJFF::Parameters &other)
     : FFBase::Parameters(other), lj_params(other.lj_params)
{}

/** Destructor */
LJFF::Parameters::~Parameters()
{}

/** Static object returned by CLJFF::parameters() */
LJFF::Parameters LJFF::Parameters::default_sources;

/////////////
///////////// Implementation of LJFF::Groups
/////////////

LJFF::Groups::Groups() : FFBase::Groups()
{}

LJFF::Groups::Groups(const LJFF::Groups &other)
     : FFBase::Groups(other)
{}

LJFF::Groups::~Groups()
{}

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

    LJMoleculeData& operator=(const LJMoleculeData &other);

    bool operator==(const LJMoleculeData &other) const;
    bool operator!=(const LJMoleculeData &other) const;

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

    static QSharedDataPointer<LJMoleculeData> shared_null;
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

/** Assignment operator */
LJFF::LJMoleculeData& LJFF::LJMoleculeData::operator=(const LJFF::LJMoleculeData &other)
{
    if (this != &other)
    {
        molecule = other.molecule;
        lj_property = other.lj_property;
        selected_atoms = other.selected_atoms;
        cg_index = other.cg_index;
        coords = other.coords;
        ljs = other.ljs;
    }

    return *this;
}

/** Comparison operator */
bool LJFF::LJMoleculeData::operator==(const LJFF::LJMoleculeData &other) const
{
    return molecule == other.molecule and
           lj_property == other.lj_property and
           selected_atoms == other.selected_atoms and
           cg_index == other.cg_index;
}

/** Comparison operator */
bool LJFF::LJMoleculeData::operator!=(const LJFF::LJMoleculeData &other) const
{
    return molecule != other.molecule or
           lj_property != other.lj_property or
           selected_atoms != other.selected_atoms or
           cg_index != other.cg_index;
}

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
        ljs = molecule.getProperty(lj_property);
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

    BOOST_ASSERT( nselectedgroups == uint(cg_index.count()) );
    BOOST_ASSERT( nselectedgroups == uint(coords.count()) );
    BOOST_ASSERT( nselectedgroups == uint(ljs.count()) );

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

    BOOST_ASSERT( nselectedgroups == uint(cg_index.count()) );
    BOOST_ASSERT( nselectedgroups == uint(coords.count()) );
    BOOST_ASSERT( nselectedgroups == uint(ljs.count()) );

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

/** Shared null LJMoleculeData */
QSharedDataPointer<LJFF::LJMoleculeData> LJFF::LJMoleculeData::shared_null(
                                                    new LJFF::LJMoleculeData() );

} // end of namespace SireMM


static const RegisterMetaType<LJFF::LJMoleculeData> r_ljmoldata(MAGIC_ONLY,
                                                      "SireMM::LJFF::LJMoleculeData");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const LJFF::LJMoleculeData &ljmoldata)
{
    writeHeader(ds, r_ljmoldata, 1);

    SharedDataStream sds(ds);

    sds << ljmoldata.molecule
        << ljmoldata.lj_property
        << ljmoldata.selected_atoms;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, LJFF::LJMoleculeData &ljmoldata)
{
    VersionID v = readHeader(ds, r_ljmoldata);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        Molecule mol;
        QString lj_property;
        AtomSelection selected_atoms;

        sds >> mol >> lj_property >> selected_atoms;

        ljmoldata = LJFF::LJMoleculeData(mol, selected_atoms, lj_property);
    }
    else
        throw version_error(v, "1", r_ljmoldata, CODELOC);

    return ds;
}

/////////////
///////////// Implementation of LJFF::LJMolecule
/////////////

static const RegisterMetaType<LJFF::LJMolecule> r_ljmol;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const LJFF::LJMolecule &ljmol)
{
    writeHeader(ds, r_ljmol, 1);

    SharedDataStream sds(ds);
    sds << ljmol.d;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, LJFF::LJMolecule &ljmol)
{
    VersionID v = readHeader(ds, r_ljmol);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> ljmol.d;
    }
    else
        throw version_error(v, "1", r_ljmol, CODELOC);

    return ds;
}

/** Null constructor - creates an empty LJMolecule */
LJFF::LJMolecule::LJMolecule() : d(LJFF::LJMoleculeData::shared_null)
{}

/** Construct from the passed molecule, using 'ljproperty' to find the
    molecule's LJ parameters.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
LJFF::LJMolecule::LJMolecule(const Molecule &molecule, const QString &ljproperty)
                 : d( new LJFF::LJMoleculeData(molecule,ljproperty) )
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

        QVector< QVector<LJParameter> > new_ljs(ngroups);
        QVector<CoordGroup> new_coords(ngroups);

        QHash<CutGroupID,int> new_index;
        new_index.reserve(ngroups);

        int i = 0;

        for (QSet<CutGroupID>::const_iterator it = groups.begin();
             it != groups.end();
             ++it)
        {
            new_coords[i] = coords[*it];
            new_ljs[i] = ljs[*it];

            new_index.insert(*it,i);
            ++i;
        }

        d->coords = new_coords;
        d->ljs = AtomicLJs(new_ljs);
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
                indicies_to_be_removed.append(it.value());
            }
        }

        //remove the identified CutGroups (from back to front)
        qSort(indicies_to_be_removed.begin(), indicies_to_be_removed.end(),
              qGreater<int>());

        foreach (int idx, indicies_to_be_removed)
        {
            d->coords.remove(idx);
            d->ljs.remove(idx);
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
LJFF::LJMolecule& LJFF::LJMolecule::operator=(const LJMolecule &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool LJFF::LJMolecule::operator==(const LJFF::LJMolecule &other) const
{
    return d == other.d or (*d == *(other.d));
}

/** Comparison operator */
bool LJFF::LJMolecule::operator!=(const LJFF::LJMolecule &other) const
{
    return d != other.d and (*d != *(other.d));
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
LJFF::ChangedLJMolecule LJFF::LJMolecule::_pvt_change(const Molecule &molecule,
                                                      const QSet<CutGroupID> &cgids,
                                                      const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        //this is a null Molecule
        return ChangedLJMolecule();

    //assert that this is the same molecule
    d->molecule.assertSameMolecule(molecule);

    if (ljproperty.isNull() or ljproperty == d->lj_property)
    {
        //has there been a change?
        if ( d->molecule.version() == molecule.version() )
            //no - there hasn't!
            return ChangedLJMolecule();

        //the molecule may have changed
        LJMolecule newmol(*this);

        newmol.d->molecule = molecule;

        if (cgids.isEmpty())
        {
            //the whole molecule has changed

            if (molecule.version().major() != d->molecule.version().major())
            {
                //there has been a major change
                newmol.d->rebuildAll();
            }
            else
            {
                //only the coordinates have changed
                newmol.d->rebuildCoordinates();
            }

            return ChangedLJMolecule(*this, newmol);
        }
        else
        {
            //only part of the molecule has changed
            if (molecule.version().major() != d->molecule.version().major())
            {
                //there has been a major change

                if (newmol.d->rebuildAll(cgids))
                    //the part of the molecule in this forcefield has changed!
                    return ChangedLJMolecule(*this, newmol, cgids);
                else
                    //no part of the molecule that was in this forcefield has changed
                    return ChangedLJMolecule();
            }
            else
            {
                //only the coordinates have changed

                if (newmol.d->rebuildCoordinates(cgids))
                    //the part of the molecule in this forcefield has moved
                    return ChangedLJMolecule(*this, newmol, cgids);
                else
                    //no part of the molecule in this forcefield has moved
                    return ChangedLJMolecule();
            }
        }
    }
    else
    {
        //there has been a change of property - the entire molecule
        //needs to be rebuilt
        LJMolecule newmol(*this);

        newmol.d->molecule = molecule;
        newmol.d->lj_property = ljproperty;

        newmol.d->rebuildAll();

        return ChangedLJMolecule(*this, newmol);
    }
}

/** Return a ChangedLJMolecule that represents the change from the
    LJMolecule in its current state to 'molecule'. Note that
    the new state must have the same MoleculeID as the current state!

    \throw SireError::incompatible_error
*/
LJFF::ChangedLJMolecule LJFF::LJMolecule::change(const Molecule &molecule) const
{
    return this->_pvt_change(molecule, QSet<CutGroupID>());
}

/** Return a ChangedLJMolecule that represents the change from the
    LJMolecule in its current state to include the changes in 'residue'

    \throw SireError::incompatible_error
*/
LJFF::ChangedLJMolecule LJFF::LJMolecule::change(const Residue &residue) const
{
    return this->_pvt_change(residue.molecule(), residue.info().cutGroupIDs());
}

/** Return a ChangedLJMolecule that represents the change from
    the LJMolecule in its current state to include the changes in 'atom'

    \throw SireError::incompatible_error
*/
LJFF::ChangedLJMolecule LJFF::LJMolecule::change(const NewAtom &atom) const
{
    QSet<CutGroupID> cgid;
    cgid.insert(atom.cgAtomID().cutGroupID());

    return this->_pvt_change(atom.molecule(), cgid);
}

/** Private function used to return the ChangedLJMolecule that represents
    the change from the LJMolecule in its current state to 'molecule', with
    the new selection 'selected_atoms', with the guarantee that only the CutGroups
    whose IDs are in 'cgids' have changed. (if cgids is empty then all of the
    CutGroups have changed)

    \throw SireError::incompatible_error
*/
LJFF::ChangedLJMolecule LJFF::LJMolecule::_pvt_change(const Molecule &molecule,
                                                const QSet<CutGroupID> &cgids,
                                                const AtomSelection &selected_atoms,
                                                const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedLJMolecule();

    if (selected_atoms == d->selected_atoms)
        //there has been no change in the atom selections
        return this->_pvt_change(molecule, cgids, ljproperty);

    d->molecule.assertSameMolecule(molecule);

    LJMolecule newmol(*this);

    newmol.d->molecule = molecule;
    newmol.d->selected_atoms = selected_atoms;

    if (ljproperty.isNull() or ljproperty == d->lj_property)
    {
        //there is no change in the property used to get the LJ parameters
        if (cgids.isEmpty())
        {
            //the entire molecule has changed
            newmol.d->rebuildAll();

            return ChangedLJMolecule(*this, newmol);
        }
        else
        {
            //only parts of the molecule have changed
            newmol.d->rebuildAll(cgids);

            return ChangedLJMolecule(*this, newmol, cgids);
        }
    }
    else
    {
        //there has been a change of property - we need to rebuild
        //the entire molecule
        newmol.d->lj_property = ljproperty;
        newmol.d->rebuildAll();

        return ChangedLJMolecule(*this, newmol);
    }
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to include the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
LJFF::ChangedLJMolecule LJFF::LJMolecule::add(const Molecule &molecule,
                                              const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedLJMolecule(*this, LJMolecule(molecule,ljproperty));
    else
        return this->_pvt_change(molecule, QSet<CutGroupID>(),
                                 AtomSelection(molecule), ljproperty);
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to include as well the passed residue, which itself may
    have changed

    \throw SireError::incompatible_error
*/
LJFF::ChangedLJMolecule LJFF::LJMolecule::add(const Residue &residue,
                                              const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedLJMolecule(*this, LJMolecule(residue,ljproperty));
    else
    {
        AtomSelection new_selection = d->selected_atoms;

        new_selection.selectAll(residue.number());

        return this->_pvt_change(residue.molecule(), residue.info().cutGroupIDs(),
                                 new_selection, ljproperty);
    }
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to include as well the passed atom, which itself may
    have changed

    \throw SireError::incompatible_error
*/
LJFF::ChangedLJMolecule LJFF::LJMolecule::add(const NewAtom &atom,
                                              const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedLJMolecule(*this, LJMolecule(atom,ljproperty));
    else
    {
        AtomSelection new_selection = d->selected_atoms;

        new_selection.select(atom.cgAtomID());

        QSet<CutGroupID> cgid;
        cgid.insert(atom.cgAtomID().cutGroupID());

        return this->_pvt_change(atom.molecule(), cgid, new_selection,
                                 ljproperty);
    }
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to include as well the passed selection

    \throw SireError::incompatible_error
*/
LJFF::ChangedLJMolecule LJFF::LJMolecule::add(const AtomSelection &selected_atoms,
                                              const QString &ljproperty) const
{
    AtomSelection new_selection = d->selected_atoms;

    new_selection.selectAll(selected_atoms);

    Molecule oldmol = d->molecule;

    return this->_pvt_change(oldmol, selected_atoms.selectedCutGroups(),
                             new_selection, ljproperty);
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to remove the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
LJFF::ChangedLJMolecule LJFF::LJMolecule::remove(const Molecule &molecule) const
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
LJFF::ChangedLJMolecule LJFF::LJMolecule::remove(const Residue &residue) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselectAll(residue.number());

    return this->_pvt_change(residue.molecule(), residue.info().cutGroupIDs(), new_selection);
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to remove the passed atom, which itself may
    have changed

    \throw SireError::incompatible_error
*/
LJFF::ChangedLJMolecule LJFF::LJMolecule::remove(const NewAtom &atom) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselect(atom.cgAtomID());

    QSet<CutGroupID> cgid;
    cgid.insert(atom.cgAtomID().cutGroupID());

    return this->_pvt_change(atom.molecule(), cgid, new_selection);
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to remove the passed atom selection

    \throw SireError::incompatible_error
*/
LJFF::ChangedLJMolecule LJFF::LJMolecule::remove(const AtomSelection &selected_atoms) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselectAll(selected_atoms);

    Molecule oldmol = d->molecule;

    return this->_pvt_change(oldmol, selected_atoms.selectedCutGroups(),
                             new_selection);
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

static const RegisterMetaType<LJFF::ChangedLJMolecule> r_changedmol;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const LJFF::ChangedLJMolecule &changedmol)
{
    writeHeader(ds, r_changedmol, 1);

    SharedDataStream sds(ds);

    sds << changedmol.oldmol << changedmol.newmol
        << changedmol.oldparts << changedmol.newparts
        << changedmol.changed_cgids;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      LJFF::ChangedLJMolecule &changedmol)
{
    VersionID v = readHeader(ds, r_changedmol);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> changedmol.oldmol >> changedmol.newmol
            >> changedmol.oldparts >> changedmol.newparts
            >> changedmol.changed_cgids;
    }
    else
        throw version_error(v, "1", r_changedmol, CODELOC);

    return ds;
}

/** Empty constructor - this represents no change */
LJFF::ChangedLJMolecule::ChangedLJMolecule()
{}

/** Construct a zero change - e.g. from 'mol' to 'mol' - this
    is useful when constructing changes that will later on move
    from 'mol' to 'newmol', e.g. ChangedLJMolecule(mol).change(newmol) */
LJFF::ChangedLJMolecule::ChangedLJMolecule(const LJMolecule &molecule)
                        : oldmol(molecule),
                          newmol(molecule)
{
    //oldparts, newparts and changed_cgids are empty as there is no change!
}

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
    if (changed_groups.count() == old_molecule.molecule().nCutGroups())
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
LJFF::ChangedLJMolecule&
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
const LJFF::LJMolecule& LJFF::ChangedLJMolecule::oldMolecule() const
{
    return oldmol;
}

/** Return the whole LJMolecule as it is in the new state */
const LJFF::LJMolecule& LJFF::ChangedLJMolecule::newMolecule() const
{
    return newmol;
}

/** Return the parts of the LJMolecule in the old state that
    have changed compared to the new state */
const LJFF::LJMolecule& LJFF::ChangedLJMolecule::oldParts() const
{
    return oldparts;
}

/** Return the parts of the LJMolecule in the new state that
    have changed compared to the old state */
const LJFF::LJMolecule& LJFF::ChangedLJMolecule::newParts() const
{
    return newparts;
}

/** Return the ChangedLJMolecule that represents the change from the old molecule
    to 'molecule' */
LJFF::ChangedLJMolecule LJFF::ChangedLJMolecule::change(const Molecule &molecule) const
{
    //calculate the change from newmol to molecule...
    ChangedLJMolecule next_change = newmol.change(molecule);

    if (next_change.isEmpty())
        //there was no further change
        return ChangedLJMolecule();
    else
        //now return the change from oldmol to newmol
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
}

QSet<CutGroupID> operator+(const QSet<CutGroupID> &set0, const QSet<CutGroupID> &set1)
{
    QSet<CutGroupID> ret(set0);
    ret.unite(set1);

    return ret;
}

/** Return the ChangedLJMolecule that represents the change from the old molecule
    to 'residue' */
LJFF::ChangedLJMolecule LJFF::ChangedLJMolecule::change(const Residue &residue) const
{
    if (this->changedAll())
        return this->change(residue.molecule());

    //calculate the change from newmol to 'residue'...
    ChangedLJMolecule next_change = newmol.change(residue);

    if (next_change.isEmpty())
        return ChangedLJMolecule();
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the change from the old molecule
    to 'residue' */
LJFF::ChangedLJMolecule LJFF::ChangedLJMolecule::change(const NewAtom &atom) const
{
    if (this->changedAll())
        return this->change(atom.molecule());

    ChangedLJMolecule next_change = newmol.change(atom);

    if (next_change.isEmpty())
        return ChangedLJMolecule();
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the addition of all atoms
    in the molecule 'molecule' */
LJFF::ChangedLJMolecule LJFF::ChangedLJMolecule::add(const Molecule &molecule,
                                                     const QString &ljproperty) const
{
    ChangedLJMolecule next_change = newmol.add(molecule, ljproperty);

    if (next_change.isEmpty())
        return ChangedLJMolecule();
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
}

/** Return the ChangedLJMolecule that represents the addition of all atoms
    in the residue 'residue' */
LJFF::ChangedLJMolecule LJFF::ChangedLJMolecule::add(const Residue &residue,
                                                     const QString &ljproperty) const
{
    ChangedLJMolecule next_change = newmol.add(residue, ljproperty);

    if (next_change.isEmpty())
        return ChangedLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the addition of the atom 'atom' */
LJFF::ChangedLJMolecule LJFF::ChangedLJMolecule::add(const NewAtom &atom,
                                                     const QString &ljproperty) const
{
    ChangedLJMolecule next_change = newmol.add(atom, ljproperty);

    if (next_change.isEmpty())
        return ChangedLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the addition of the selected atoms */
LJFF::ChangedLJMolecule LJFF::ChangedLJMolecule::add(
                                      const AtomSelection &selected_atoms,
                                      const QString &ljproperty) const
{
    ChangedLJMolecule next_change = newmol.add(selected_atoms, ljproperty);

    if (next_change.isEmpty())
        return ChangedLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the removal of the entire molecule! */
LJFF::ChangedLJMolecule LJFF::ChangedLJMolecule::remove(const Molecule &molecule) const
{
    ChangedLJMolecule next_change = newmol.remove(molecule);

    if (next_change.isEmpty())
        return ChangedLJMolecule();
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
}

/** Return the ChangedLJMolecule that represents the removal of the residue 'residue' */
LJFF::ChangedLJMolecule LJFF::ChangedLJMolecule::remove(const Residue &residue) const
{
    ChangedLJMolecule next_change = newmol.remove(residue);

    if (next_change.isEmpty())
        return ChangedLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the removal of 'atom' */
LJFF::ChangedLJMolecule LJFF::ChangedLJMolecule::remove(const NewAtom &atom) const
{
    ChangedLJMolecule next_change = newmol.remove(atom);

    if (next_change.isEmpty())
        return ChangedLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedLJMolecule that represents the removal of the
    selected atoms. */
LJFF::ChangedLJMolecule LJFF::ChangedLJMolecule::remove(
                                      const AtomSelection &selected_atoms) const
{
    ChangedLJMolecule next_change = newmol.remove(selected_atoms);

    if (next_change.isEmpty())
        return ChangedLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/////////////
///////////// Implementation of LJFF
/////////////

/** This function returns the LJ energy of two groups based on the
    inter-atomic inverse-square-distances stored in 'distmatrix'
    and using the LJ parameters 'ljmatrix'. */
double LJFF::calculatePairEnergy(DistMatrix &distmatrix,
                                 LJMatrix &ljmatrix)
{
    uint nats0 = distmatrix.nOuter();
    uint nats1 = distmatrix.nInner();

    BOOST_ASSERT( ljmatrix.nOuter() == nats0 );
    BOOST_ASSERT( ljmatrix.nInner() == nats1 );

    double ljnrg = 0;

    //loop over all pairs of atoms
    for (uint i=0; i<nats0; ++i)
    {
        distmatrix.setOuterIndex(i);
        ljmatrix.setOuterIndex(i);

        for (uint j=0; j<nats1; ++j)
        {
            //get the distance and LJPair for this atom pair
            double invdist2 = distmatrix[j];
            const LJPair &ljpair = ljmatrix[j];

            double sig2_over_dist2 = SireMaths::pow_2(ljpair.sigma()) * invdist2;
            double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
            double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

            //LJ energy
            ljnrg += double(4) * ljpair.epsilon() *
                                    ( sig12_over_dist12 - sig6_over_dist6 );
        }
    }

    return ljnrg;
}

/** This function is used to calculate and return the self-energy of a group,
    using the inverse-square-distances and parameters stored
    in the passed distance and LJ matricies. */
double LJFF::calculateSelfEnergy(DistMatrix &distmatrix,
                                 LJMatrix &ljmatrix)
{
    uint nats = distmatrix.nOuter();

    BOOST_ASSERT( distmatrix.nInner() == nats );
    BOOST_ASSERT( ljmatrix.nInner() == nats );
    BOOST_ASSERT( ljmatrix.nOuter() == nats );

    double ljnrg = 0;

    for (uint i=0; i<nats-1; ++i)
    {
        distmatrix.setOuterIndex(i);
        ljmatrix.setOuterIndex(i);

        for (uint j=i+1; j<nats; ++j)
        {
            //get the distance and LJPair for this atom pair
            double invdist2 = distmatrix[j];
            const LJPair &ljpair = ljmatrix[j];

            double sig2_over_dist2 = SireMaths::pow_2(ljpair.sigma()) * invdist2;
            double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
            double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

            //LJ energy
            ljnrg += double(4) * ljpair.epsilon() *
                                    ( sig12_over_dist12 - sig6_over_dist6 );
        }
    }

    return ljnrg;
}

/** Calculate and return the LJ energy of interaction of group0, with LJ parameters
    in 'lj0' and group1, with LJ parameters in 'lj1', using the
    space 'space', and using the provided distance and LJ matricies as
    temporary workspace. */
double LJFF::calculateEnergy(const CoordGroup &group0,
                             const QVector<LJParameter> &lj0,
                             const CoordGroup &group1,
                             const QVector<LJParameter> &lj1,
                             const Space &space,
                             const SwitchingFunction &switchfunc,
                             DistMatrix &distmatrix,
                             LJMatrix &ljmatrix)
{
    if ( not space.beyond(switchfunc.cutoffDistance(), group0, group1) )
    {
        double mindist = space.calcInvDist2(group0, group1, distmatrix);

        double scllj = switchfunc.vdwScaleFactor(mindist);

        if (scllj != 0)
        {
            //combine the LJ parameters together
            int nats0 = lj0.count();
            int nats1 = lj1.count();

            BOOST_ASSERT( group0.count() == nats0 );
            BOOST_ASSERT( group1.count() == nats1 );

            ljmatrix.redimension(nats0, nats1);

            const LJParameter *lj0_array = lj0.constData();
            const LJParameter *lj1_array = lj1.constData();

            for (int i=0; i<nats0; ++i)
            {
                ljmatrix.setOuterIndex(i);

                const LJParameter &lj0param = lj0_array[i];

                for (int j=0; j<nats1; ++j)
                {
                    ljmatrix[j] = LJPair::geometric( lj1_array[j], lj0param );
                }
            }

            return scllj * LJFF::calculatePairEnergy(distmatrix, ljmatrix);
        }
    }

    return 0;
}

/** Calculate the LJ energy of interaction of the atoms within the
    group 'group', with LJ parameters in 'ljs', using the space 'space',
    combining rules in 'combrules', and working in the temporary
    workspace provided by the passed distance and LJ matricies */
double LJFF::calculateEnergy(const CoordGroup &group,
                             const QVector<LJParameter> &ljs,
                             const Space &space,
                             DistMatrix &distmatrix,
                             LJMatrix &ljmatrix)
{
    space.calcInvDist2(group, distmatrix);

    //combine together the LJ parameters
    int nats = ljs.count();

    BOOST_ASSERT(group.count() == nats);

    ljmatrix.redimension(nats,nats);

    const LJParameter *lj_array = ljs.constData();

    //we only need to fill in the top left diagonal
    //(and we don't need to do matrix(i,i) as we never
    // calculate a self-interaction!)
    for (int i=0; i<nats-1; ++i)
    {
        ljmatrix.setOuterIndex(i);

        const LJParameter &lj0param = lj_array[i];

        for (int j=i+1; j<nats; ++j)
        {
            ljmatrix[j] = LJPair::geometric( lj_array[j], lj0param );
        }
    }

    return LJFF::calculateSelfEnergy(distmatrix, ljmatrix);
}

/** Calculate and return the LJ energy of interaction between two
    LJMolecules, using the space 'space', combining rules 'combrules'
    and switching function 'switchfunc'. The calculation will use
    the provided distance and LJ matricies as a temporary workspace */
double LJFF::calculateEnergy(const LJFF::LJMolecule &mol0,
                             const LJFF::LJMolecule &mol1,
                             const Space &space,
                             const SwitchingFunction &switchfunc,
                             DistMatrix &distmatrix,
                             LJMatrix &ljmatrix)
{
    int ncg0 = mol0.coordinates().count();
    int ncg1 = mol1.coordinates().count();

    if (ncg0 == 1 and ncg1 == 1)
    {
        return calculateEnergy( mol0.coordinates().constData()[0],
                                mol0.ljParameters().constData()[0],
                                mol1.coordinates().constData()[0],
                                mol1.ljParameters().constData()[0],

                                space, switchfunc,
                                distmatrix, ljmatrix);
    }
    else if (ncg0 > 0 and ncg1 > 0)
    {
        double ljnrg = 0;

        const CoordGroup *cg0array = mol0.coordinates().constData();
        const CoordGroup *cg1array = mol1.coordinates().constData();

        const QVector<LJParameter> *lj0array = mol0.ljParameters().constData();
        const QVector<LJParameter> *lj1array = mol1.ljParameters().constData();

        for (int i=0; i<ncg0; ++i)
        {
            const CoordGroup &group0 = cg0array[i];
            const QVector<LJParameter> &lj0 = lj0array[i];

            for (int j=0; j<ncg1; ++j)
            {
                const CoordGroup &group1 = cg1array[j];
                const QVector<LJParameter> &lj1 = lj1array[j];

                ljnrg += calculateEnergy(group0, lj0, group1, lj1,
                                         space, switchfunc,
                                         distmatrix, ljmatrix);
            }
        }

        return ljnrg;
    }
    else
        return 0;
}

/** Calculate and return the LJ energy of interaction between the pairs
    of atoms within a single LJMolecule, using the space 'space',
    combining rules 'combrules' and switching function
    'switchfunc'. The calculation will use the provided
    distance and LJ matricies as a temporary workspace  */
double LJFF::calculateEnergy(const LJFF::LJMolecule &mol,
                             const Space &space,
                             const SwitchingFunction &switchfunc,
                             DistMatrix &distmatrix,
                             LJMatrix &ljmatrix)
{
    int ncg = mol.coordinates().count();

    if (ncg == 1)
    {
        //calculate only the self-energy of the first CutGroup
        return calculateEnergy(mol.coordinates().constData()[0],
                               mol.ljParameters().constData()[0],
                               space, distmatrix, ljmatrix);
    }
    else if (ncg > 1)
    {
        double ljnrg = 0;

        const CoordGroup *cgarray = mol.coordinates().constData();
        const QVector<LJParameter> *ljarray = mol.ljParameters().constData();

        for (int i=0; i<ncg-1; ++i)
        {
            const CoordGroup &group0 = cgarray[i];
            const QVector<LJParameter> &lj0 = ljarray[i];

            //add on the self-energy
            ljnrg += calculateEnergy(group0, lj0, space, distmatrix, ljmatrix);

            for (int j=i+1; j<ncg; ++j)
            {
                //calculate the group-group energy
                const CoordGroup &group1 = cgarray[j];
                const QVector<LJParameter> &lj1 = ljarray[j];

                ljnrg += calculateEnergy(group0, lj0, group1, lj1,
                                         space, switchfunc,
                                         distmatrix, ljmatrix);
            }
        }

        //have to add on the self-energy of the last CutGroup of the molecule
        const CoordGroup &group = cgarray[ncg-1];
        const QVector<LJParameter> &lj = ljarray[ncg-1];

        ljnrg += calculateEnergy(group, lj, space, distmatrix, ljmatrix);

        return ljnrg;
    }
    else
        return 0;
}

static const RegisterMetaType<LJFF> r_ljff(MAGIC_ONLY, "SireMM::LJFF");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const LJFF &ljff)
{
    writeHeader(ds, r_ljff, 1);

    SharedDataStream sds(ds);

    sds << ljff.spce << ljff.switchfunc
        << static_cast<const FFBase&>(ljff);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, LJFF &ljff)
{
    VersionID v = readHeader(ds, r_ljff);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> ljff.spce >> ljff.switchfunc
            >> static_cast<FFBase&>(ljff);
    }
    else
        throw version_error(v, "1", r_ljff, CODELOC);

    return ds;
}

/** Null constructor */
LJFF::LJFF() : FFBase()
{
    this->registerComponents();
}

/** Construct a LJFF using the specified space and switching function */
LJFF::LJFF(const Space &space, const SwitchingFunction &switchingfunction)
     : FFBase(),
       spce(space), switchfunc(switchingfunction)
{
    this->registerComponents();
}

/** Copy constructor */
LJFF::LJFF(const LJFF &other)
     : FFBase(other),
       spce(other.spce), switchfunc(other.switchfunc)
{
    //get the pointer from the base class...
    components_ptr = dynamic_cast<const LJFF::Components*>( &(FFBase::components()) );
    BOOST_ASSERT( components_ptr != 0 );
}

/** Destructor */
LJFF::~LJFF()
{}

/** Register the energy components associated with this forcefield */
void LJFF::registerComponents()
{
    std::auto_ptr<LJFF::Components> ptr( new LJFF::Components(*this) );

    FFBase::registerComponents(ptr.get());

    components_ptr = ptr.release();
}
