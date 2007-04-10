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

#include "coulombff.h"

#include "SireMol/residueinfo.h"
#include "SireMol/cgatomid.h"

#include "SireUnits/units.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireFF;
using namespace SireMM;
using namespace SireMol;
using namespace SireStream;

/////////////
///////////// Implementation of CoulombFF::Components
/////////////

/** Constructor */
CoulombFF::Components::Components() : FFBase::Components()
{}

/** Construct using the supplied forcefield */
CoulombFF::Components::Components(const FFBase &ffbase, const Symbols &symbols)
          : FFBase::Components( ffbase, addToSymbols(symbols, x(), y(), z()) ),
            e_coulomb(ffbase, "coulomb", x(), y(), z())
{
    this->registerComponent(e_coulomb);
}

/** Copy constructor */
CoulombFF::Components::Components(const CoulombFF::Components &other)
          : FFBase::Components(other),
            e_coulomb(other.e_coulomb)
{}

/** Destructor */
CoulombFF::Components::~Components()
{}

/** Assignment operator */
CoulombFF::Components& CoulombFF::Components::operator=(const CoulombFF::Components &other)
{
    e_coulomb = other.e_coulomb;

    FFBase::Components::operator=(other);

    return *this;
}

/** Set the forcefield */
void CoulombFF::Components::setForceField(const FFBase &ffbase)
{
    *this = CoulombFF::Components(ffbase);
}

/** Describe the Coulomb component */
QString CoulombFF::Components::describe_coulomb()
{
    return QObject::tr("The total Coulomb (electrostatic) energy of the forcefield.");
}

/////////////
///////////// Implementation of CoulombFF::Parameters
/////////////

/** Constructor - by default the Coulomb parameters
    come from the 'charges' property */
CoulombFF::Parameters::Parameters()
          : FFBase::Parameters(), chg_params("coulomb", "charges")
{}

/** Copy constructor */
CoulombFF::Parameters::Parameters(const CoulombFF::Parameters &other)
          : FFBase::Parameters(other), chg_params(other.chg_params)
{}

/** Destructor */
CoulombFF::Parameters::~Parameters()
{}

/** Static object returned by CCoulombFF::parameters() */
CoulombFF::Parameters CoulombFF::Parameters::default_sources;

/////////////
///////////// Implementation of CoulombFF::Groups
/////////////

CoulombFF::Groups::Groups() : FFBase::Groups()
{}

CoulombFF::Groups::Groups(const CoulombFF::Groups &other)
          : FFBase::Groups(other)
{}

CoulombFF::Groups::~Groups()
{}

/////////////
///////////// Implementation of CoulombFF::CoulombMoleculeData
/////////////

namespace SireMM
{

/** Private class used to hold the data of CoulombFF::CoulombMolecule */
class CoulombFF::CoulombMoleculeData : public QSharedData
{
public:
    CoulombMoleculeData();

    CoulombMoleculeData(const Molecule &mol, const QString &chgproperty);
    CoulombMoleculeData(const Residue &residue, const QString &chgproperty);
    CoulombMoleculeData(const NewAtom &atom, const QString &chgproperty);
    CoulombMoleculeData(const Molecule &mol, const AtomSelection &selectedatoms,
                        const QString &chgproperty);

    CoulombMoleculeData(const CoulombMoleculeData &other);

    ~CoulombMoleculeData();

    CoulombMoleculeData& operator=(const CoulombMoleculeData &other);

    bool operator==(const CoulombMoleculeData &other) const;
    bool operator!=(const CoulombMoleculeData &other) const;

    void rebuildAll();
    void rebuildCoordinates();

    bool rebuildAll(const QSet<CutGroupID> &cgids);
    bool rebuildCoordinates(const QSet<CutGroupID> &cgids);

    /** The actual molecule */
    Molecule molecule;

    /** The name of the property associated with the charge parameters */
    QString chg_property;

    /** The atoms selected for inclusion in this forcefield */
    AtomSelection selected_atoms;

    /** Hash mapping the CutGroupID of a selected cutgroup to
        the index in the coords and ljs array for its coordinates
        and Coulomb parameters. If this is empty then all of the
        CutGroups in the molecule have been selected. */
    QHash<CutGroupID,int> cg_index;

    /** The coordinates of the CutGroups that contain atoms that
        are selected for inclusion in the Coulomb forcefield.  */
    QVector<CoordGroup> coords;

    /** The partial charges of the atoms, in the same order as
        coords. Atoms that are not selected for this forcefield
        have a zero charge. */
    AtomicCharges chgs;

    static QSharedDataPointer<CoulombMoleculeData> shared_null;
};

/** Constructor */
CoulombFF::CoulombMoleculeData::CoulombMoleculeData() : QSharedData()
{}

/** Construct to represent all of the molecule 'mol' */
CoulombFF::CoulombMoleculeData::CoulombMoleculeData(const Molecule &mol,
                                                    const QString &chgproperty)
                     : QSharedData(),
                       molecule(mol),
                       chg_property(chgproperty),
                       selected_atoms(mol)
{
    this->rebuildAll();
}

/** Construct to represent all of the residue 'residue' */
CoulombFF::CoulombMoleculeData::CoulombMoleculeData(const Residue &residue,
                                                    const QString &chgproperty)
                     : QSharedData(),
                       molecule(residue.molecule()),
                       chg_property(chgproperty),
                       selected_atoms(residue)
{
    this->rebuildAll();
}

/** Construct to represent the atom 'atom' */
CoulombFF::CoulombMoleculeData::CoulombMoleculeData(const NewAtom &atom,
                                                    const QString &chgproperty)
                     : QSharedData(),
                       molecule(atom.molecule()),
                       chg_property(chgproperty),
                       selected_atoms(atom)
{
    this->rebuildAll();
}

/** Construct to represent the selected atoms from 'selectatoms' that are
    in the molecule 'mol' */
CoulombFF::CoulombMoleculeData::CoulombMoleculeData(const Molecule &mol,
                                                    const AtomSelection &selectatoms,
                                                    const QString &chgproperty)
                     : QSharedData(),
                       molecule(mol),
                       chg_property(chgproperty),
                       selected_atoms(selectatoms)
{
    //ensure that the selected atoms and the molecule are compatible
    selected_atoms.assertCompatibleWith(molecule);

    this->rebuildAll();
}

/** Copy constructor */
CoulombFF::CoulombMoleculeData::CoulombMoleculeData(
                            const CoulombFF::CoulombMoleculeData &other)
                     : QSharedData(),
                       molecule(other.molecule),
                       chg_property(other.chg_property),
                       selected_atoms(other.selected_atoms),
                       cg_index(other.cg_index),
                       coords(other.coords),
                       chgs(other.chgs)
{}

/** Destructor */
CoulombFF::CoulombMoleculeData::~CoulombMoleculeData()
{}

/** Assignment operator */
CoulombFF::CoulombMoleculeData&
CoulombFF::CoulombMoleculeData::operator=(const CoulombFF::CoulombMoleculeData &other)
{
    if (this != &other)
    {
        molecule = other.molecule;
        chg_property = other.chg_property;
        selected_atoms = other.selected_atoms;
        cg_index = other.cg_index;
        coords = other.coords;
        chgs = other.chgs;
    }

    return *this;
}

/** Comparison operator */
bool CoulombFF::CoulombMoleculeData::operator==(
                            const CoulombFF::CoulombMoleculeData &other) const
{
    return molecule == other.molecule and
           chg_property == other.chg_property and
           selected_atoms == other.selected_atoms and
           cg_index == other.cg_index;
}

/** Comparison operator */
bool CoulombFF::CoulombMoleculeData::operator!=(
                            const CoulombFF::CoulombMoleculeData &other) const
{
    return molecule != other.molecule or
           chg_property != other.chg_property or
           selected_atoms != other.selected_atoms or
           cg_index != other.cg_index;
}

/** Rebuild all of the coordinate and Coulomb data from scratch

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
void CoulombFF::CoulombMoleculeData::rebuildAll()
{
    if (selected_atoms.selectedAll())
    {
        //all atoms are selected - we can therefore short-circuit everything!
        chgs = molecule.getProperty(chg_property);
        cg_index.clear();
        coords = molecule.coordGroups();

        return;
    }

    if (selected_atoms.isEmpty())
    {
        //there has been nothing selected!
        coords.clear();
        chgs.clear();
        cg_index.clear();

        return;
    }

    uint nselectedgroups = selected_atoms.nSelectedCutGroups();
    uint ngroups = molecule.nCutGroups();

    if (nselectedgroups == ngroups)
    {
        coords = molecule.coordGroups();
        chgs = molecule.getProperty(chg_property);
        cg_index.clear();

        //loop through all CutGroups and zero the parameters of any
        //atoms that are not selected
        for (CutGroupID i(0); i<ngroups; ++i)
        {
            if ( not selected_atoms.selectedAll(i) )
            {
                QVector<ChargeParameter> &chgparams = chgs[i];

                //there are some missing parameters to zero!
                uint nats = molecule.nAtoms(i);

                for (AtomID j(0); j<nats; ++j)
                {
                    if ( not selected_atoms.selected(CGAtomID(i,j)) )
                        //this atom has not been selected - zero its Coulomb parameter
                        chgparams[j] = ChargeParameter::dummy();
                }
            }
        }

        return;
    }

    const QVector<CoordGroup> &all_coords = molecule.coordGroups();
    AtomicCharges all_chgs = molecule.getProperty(chg_property);

    coords = QVector<CoordGroup>( nselectedgroups );
    chgs = AtomicCharges( QVector<ChargeParameter>( nselectedgroups ) );

    cg_index.clear();
    cg_index.reserve(nselectedgroups);

    int idx = 0;

    for (CutGroupID i(0); i<ngroups; ++i)
    {
        if (not selected_atoms.selectedNone(i))
        {
            coords[idx] = all_coords[i];
            chgs[idx] = all_chgs[i];

            cg_index.insert(i, idx);
            ++idx;

            if (not selected_atoms.selectedAll(i))
            {
                uint nats = molecule.nAtoms(i);

                QVector<ChargeParameter> &chgparams = chgs[idx];

                for (AtomID j(0); j<nats; ++j)
                {
                    if ( not selected_atoms.selected(CGAtomID(i,j)) )
                        chgparams[j] = ChargeParameter::dummy();
                }
            }
        }
    }
}

/** Rebuild all of the coordinates and Coulomb data for the CutGroups whose
    IDs are in 'cgids'. Return whether or not there are any atoms
    from these CutGroups that are in the molecule selection */
bool CoulombFF::CoulombMoleculeData::rebuildAll(const QSet<CutGroupID>&)
{
    //This can be heavily optimised - I just don't have the time now!
    this->rebuildAll();
    return true;
}

/** Rebuild all of the coordinate data from scratch */
void CoulombFF::CoulombMoleculeData::rebuildCoordinates()
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
    BOOST_ASSERT( nselectedgroups == uint(chgs.count()) );

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
bool CoulombFF::CoulombMoleculeData::rebuildCoordinates(const QSet<CutGroupID> &cgids)
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
    BOOST_ASSERT( nselectedgroups == uint(chgs.count()) );

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

/** Shared null CoulombMoleculeData */
QSharedDataPointer<CoulombFF::CoulombMoleculeData>
CoulombFF::CoulombMoleculeData::shared_null( new CoulombFF::CoulombMoleculeData() );

} // end of namespace SireMM


static const RegisterMetaType<CoulombFF::CoulombMoleculeData>
                          r_coulmoldata(MAGIC_ONLY,"SireMM::CoulombFF::CoulombMoleculeData");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const CoulombFF::CoulombMoleculeData &coulmoldata)
{
    writeHeader(ds, r_coulmoldata, 1);

    SharedDataStream sds(ds);

    sds << coulmoldata.molecule
        << coulmoldata.chg_property
        << coulmoldata.selected_atoms;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      CoulombFF::CoulombMoleculeData &coulmoldata)
{
    VersionID v = readHeader(ds, r_coulmoldata);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        Molecule mol;
        QString chg_property;
        AtomSelection selected_atoms;

        sds >> mol >> chg_property >> selected_atoms;

        coulmoldata = CoulombFF::CoulombMoleculeData(mol, selected_atoms,
                                                     chg_property);
    }
    else
        throw version_error(v, "1", r_coulmoldata, CODELOC);

    return ds;
}

/////////////
///////////// Implementation of CoulombFF::CoulombMolecule
/////////////

static const RegisterMetaType<CoulombFF::CoulombMolecule> r_coulmol;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const CoulombFF::CoulombMolecule &coulmol)
{
    writeHeader(ds, r_coulmol, 1);

    SharedDataStream sds(ds);
    sds << coulmol.d;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      CoulombFF::CoulombMolecule &coulmol)
{
    VersionID v = readHeader(ds, r_coulmol);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> coulmol.d;
    }
    else
        throw version_error(v, "1", r_coulmol, CODELOC);

    return ds;
}

/** Null constructor - creates an empty CoulombMolecule */
CoulombFF::CoulombMolecule::CoulombMolecule()
          : d(CoulombFF::CoulombMoleculeData::shared_null)
{}

/** Construct from the passed molecule, using 'chgproperty' to find the
    molecule's partial charges.

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CoulombFF::CoulombMolecule::CoulombMolecule(const Molecule &molecule,
                                            const QString &chgproperty)
          : d( new CoulombFF::CoulombMoleculeData(molecule,chgproperty) )
{}

/** Construct from the passed residue, using 'chgproperty' to find the
    molecule's partial charges.

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CoulombFF::CoulombMolecule::CoulombMolecule(const Residue &residue,
                                            const QString &chgproperty)
          : d( new CoulombFF::CoulombMoleculeData(residue,chgproperty) )
{}

/** Construct from the passed atom, using 'chgproperty' to find the
    molecule's partial charges.

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CoulombFF::CoulombMolecule::CoulombMolecule(const NewAtom &atom,
                                            const QString &chgproperty)
          : d( new CoulombFF::CoulombMoleculeData(atom,chgproperty) )
{}

/** Construct from the selected atoms of the passed molecule, using
    'chgproperty' to find the molecule's partial charges.

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CoulombFF::CoulombMolecule::CoulombMolecule(const Molecule &molecule,
                                            const AtomSelection &selected_atoms,
                                            const QString &chgproperty)
          : d( new CoulombFF::CoulombMoleculeData(molecule,
                                                  selected_atoms,
                                                  chgproperty) )
{}

/** Construct a copy of 'other', but masked by the CutGroups in 'groups' */
CoulombFF::CoulombMolecule::CoulombMolecule(const CoulombMolecule &other,
                                            const QSet<CutGroupID> &groups)
          : d( other.d )
{
    //get the index of which CutGroups are already selected
    const QHash<CutGroupID,int> index = other.d->cg_index;

    if (index.isEmpty())
    {
        //an empty index implies that the entire molecule is selected
        // - we therefore need only select the CutGroups in 'groups'
        const AtomicCharges &chgs = other.d->chgs;
        const QVector<CoordGroup> &coords = other.d->coords;

        int ngroups = groups.count();

        QVector< QVector<ChargeParameter> > new_chgs(ngroups);
        QVector<CoordGroup> new_coords(ngroups);

        QHash<CutGroupID,int> new_index;
        new_index.reserve(ngroups);

        int i = 0;

        for (QSet<CutGroupID>::const_iterator it = groups.begin();
             it != groups.end();
             ++it)
        {
            new_coords[i] = coords[*it];
            new_chgs[i] = chgs[*it];

            new_index.insert(*it,i);
            ++i;
        }

        d->coords = new_coords;
        d->chgs = AtomicCharges(new_chgs);
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
                //record the index of its coordinates and Coulomb parameters
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
            d->chgs.remove(idx);
        }
    }
}

/** Copy constructor */
CoulombFF::CoulombMolecule::CoulombMolecule(const CoulombMolecule &other)
          : d( other.d )
{}

/** Destructor */
CoulombFF::CoulombMolecule::~CoulombMolecule()
{}

/** Assignment operator */
CoulombFF::CoulombMolecule&
CoulombFF::CoulombMolecule::operator=(const CoulombMolecule &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool CoulombFF::CoulombMolecule::operator==(
                            const CoulombFF::CoulombMolecule &other) const
{
    return d == other.d or (*d == *(other.d));
}

/** Comparison operator */
bool CoulombFF::CoulombMolecule::operator!=(
                            const CoulombFF::CoulombMolecule &other) const
{
    return d != other.d and (*d != *(other.d));
}

/** Return whether or not this is empty (has no atoms selected */
bool CoulombFF::CoulombMolecule::isEmpty() const
{
    return d->selected_atoms.isEmpty();
}

/** Return the actual molecule */
const Molecule& CoulombFF::CoulombMolecule::molecule() const
{
    return d->molecule;
}

/** Return a ChangedCoulombMolecule that represents the change from the
    CoulombMolecule in its current state to 'molecule', where only the
    CutGroups whose IDs are in 'cgids' have changed. (if cgids is
    empty then all CutGroups have changed)

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::_pvt_change(const Molecule &molecule,
                                        const QSet<CutGroupID> &cgids,
                                        const QString &chgproperty) const
{
    if (d->molecule.ID() == 0)
        //this is a null Molecule
        return ChangedCoulombMolecule();

    //assert that this is the same molecule
    d->molecule.assertSameMolecule(molecule);

    if (chgproperty.isNull() or chgproperty == d->chg_property)
    {
        //has there been a change?
        if ( d->molecule.version() == molecule.version() )
            //no - there hasn't!
            return ChangedCoulombMolecule();

        //the molecule may have changed
        CoulombMolecule newmol(*this);

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

            return ChangedCoulombMolecule(*this, newmol);
        }
        else
        {
            //only part of the molecule has changed
            if (molecule.version().major() != d->molecule.version().major())
            {
                //there has been a major change

                if (newmol.d->rebuildAll(cgids))
                    //the part of the molecule in this forcefield has changed!
                    return ChangedCoulombMolecule(*this, newmol, cgids);
                else
                    //no part of the molecule that was in this forcefield has changed
                    return ChangedCoulombMolecule();
            }
            else
            {
                //only the coordinates have changed

                if (newmol.d->rebuildCoordinates(cgids))
                    //the part of the molecule in this forcefield has moved
                    return ChangedCoulombMolecule(*this, newmol, cgids);
                else
                    //no part of the molecule in this forcefield has moved
                    return ChangedCoulombMolecule();
            }
        }
    }
    else
    {
        //there has been a change of property - the entire molecule
        //needs to be rebuilt
        CoulombMolecule newmol(*this);

        newmol.d->molecule = molecule;
        newmol.d->chg_property = chgproperty;

        newmol.d->rebuildAll();

        return ChangedCoulombMolecule(*this, newmol);
    }
}

/** Return a ChangedCoulombMolecule that represents the change from the
    CoulombMolecule in its current state to 'molecule'. Note that
    the new state must have the same MoleculeID as the current state!

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::change(const Molecule &molecule) const
{
    return this->_pvt_change(molecule, QSet<CutGroupID>());
}

/** Return a ChangedCoulombMolecule that represents the change from the
    CoulombMolecule in its current state to include the changes in 'residue'

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::change(const Residue &residue) const
{
    return this->_pvt_change(residue.molecule(), residue.info().cutGroupIDs());
}

/** Return a ChangedCoulombMolecule that represents the change from
    the CoulombMolecule in its current state to include the changes in 'atom'

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::change(const NewAtom &atom) const
{
    QSet<CutGroupID> cgid;
    cgid.insert(atom.cgAtomID().cutGroupID());

    return this->_pvt_change(atom.molecule(), cgid);
}

/** Private function used to return the ChangedCoulombMolecule that represents
    the change from the CoulombMolecule in its current state to 'molecule', with
    the new selection 'selected_atoms', with the guarantee that only the CutGroups
    whose IDs are in 'cgids' have changed. (if cgids is empty then all of the
    CutGroups have changed)

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::_pvt_change(const Molecule &molecule,
                                        const QSet<CutGroupID> &cgids,
                                        const AtomSelection &selected_atoms,
                                        const QString &chgproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedCoulombMolecule();

    if (selected_atoms == d->selected_atoms)
        //there has been no change in the atom selections
        return this->_pvt_change(molecule, cgids, chgproperty);

    d->molecule.assertSameMolecule(molecule);

    CoulombMolecule newmol(*this);

    newmol.d->molecule = molecule;
    newmol.d->selected_atoms = selected_atoms;

    if (chgproperty.isNull() or chgproperty == d->chg_property)
    {
        //there is no change in the property used to get the Coulomb parameters
        if (cgids.isEmpty())
        {
            //the entire molecule has changed
            newmol.d->rebuildAll();

            return ChangedCoulombMolecule(*this, newmol);
        }
        else
        {
            //only parts of the molecule have changed
            newmol.d->rebuildAll(cgids);

            return ChangedCoulombMolecule(*this, newmol, cgids);
        }
    }
    else
    {
        //there has been a change of property - we need to rebuild
        //the entire molecule
        newmol.d->chg_property = chgproperty;
        newmol.d->rebuildAll();

        return ChangedCoulombMolecule(*this, newmol);
    }
}

/** Return a ChangedCoulombMolecule that represents the change from the CoulombMolecule
    in its current state to include the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::add(const Molecule &molecule,
                                const QString &chgproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedCoulombMolecule(*this,
                                      CoulombMolecule(molecule,chgproperty));
    else
        return this->_pvt_change(molecule, QSet<CutGroupID>(),
                                 AtomSelection(molecule), chgproperty);
}

/** Return a ChangedCoulombMolecule that represents the change from the CoulombMolecule
    in its current state to include as well the passed residue, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::add(const Residue &residue,
                                const QString &chgproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedCoulombMolecule(*this,
                                      CoulombMolecule(residue,chgproperty));
    else
    {
        AtomSelection new_selection = d->selected_atoms;

        new_selection.selectAll(residue.number());

        return this->_pvt_change(residue.molecule(), residue.info().cutGroupIDs(),
                                 new_selection, chgproperty);
    }
}

/** Return a ChangedCoulombMolecule that represents the change from the CoulombMolecule
    in its current state to include as well the passed atom, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::add(const NewAtom &atom,
                                const QString &chgproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedCoulombMolecule(*this, CoulombMolecule(atom,chgproperty));
    else
    {
        AtomSelection new_selection = d->selected_atoms;

        new_selection.select(atom.cgAtomID());

        QSet<CutGroupID> cgid;
        cgid.insert(atom.cgAtomID().cutGroupID());

        return this->_pvt_change(atom.molecule(), cgid, new_selection,
                                 chgproperty);
    }
}

/** Return a ChangedCoulombMolecule that represents the change from the CoulombMolecule
    in its current state to include as well the passed selection

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::add(const AtomSelection &selected_atoms,
                                const QString &chgproperty) const
{
    AtomSelection new_selection = d->selected_atoms;

    new_selection.selectAll(selected_atoms);

    Molecule oldmol = d->molecule;

    return this->_pvt_change(oldmol, selected_atoms.selectedCutGroups(),
                             new_selection, chgproperty);
}

/** Return a ChangedCoulombMolecule that represents the change from the CoulombMolecule
    in its current state to remove the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::remove(const Molecule &molecule) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselectAll();

    return this->_pvt_change(molecule, QSet<CutGroupID>(), new_selection);
}

/** Return a ChangedCoulombMolecule that represents the change from the CoulombMolecule
    in its current state to remove the passed residue, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::remove(const Residue &residue) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselectAll(residue.number());

    return this->_pvt_change(residue.molecule(), residue.info().cutGroupIDs(), new_selection);
}

/** Return a ChangedCoulombMolecule that represents the change from the CoulombMolecule
    in its current state to remove the passed atom, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::remove(const NewAtom &atom) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselect(atom.cgAtomID());

    QSet<CutGroupID> cgid;
    cgid.insert(atom.cgAtomID().cutGroupID());

    return this->_pvt_change(atom.molecule(), cgid, new_selection);
}

/** Return a ChangedCoulombMolecule that represents the change from the CoulombMolecule
    in its current state to remove the passed atom selection

    \throw SireError::incompatible_error
*/
CoulombFF::ChangedCoulombMolecule
CoulombFF::CoulombMolecule::remove(const AtomSelection &selected_atoms) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselectAll(selected_atoms);

    Molecule oldmol = d->molecule;

    return this->_pvt_change(oldmol, selected_atoms.selectedCutGroups(),
                             new_selection);
}

/** Return the name of the property used to get the partial charges */
const QString& CoulombFF::CoulombMolecule::chargeProperty() const
{
    return d->chg_property;
}

/** Return the CoordGroups of the CutGroups that have atoms that are
    in this forcefield. Note that there is no defined order to the
    array of CoordGroups, other than it is the same order as the
    array of Coulomb parameter groups returned by ljParameters(). */
const QVector<CoordGroup>& CoulombFF::CoulombMolecule::coordinates() const
{
    return d->coords;
}

/** Return the partial charges of all of the atoms that are in
    this forcefield. The parameters are arranged by CutGroup and
    are in the same order as the coordinates as returned by
    coordinates(). Atoms which are not in this forcefield
    have zero charge. */
const AtomicCharges& CoulombFF::CoulombMolecule::charges() const
{
    return d->chgs;
}

/** Return whether or not this CoulombMolecule represents the whole
    of the molecule */
bool CoulombFF::CoulombMolecule::isWholeMolecule() const
{
    return d->selected_atoms.selectedAll();
}

/** Return the selection of atoms that are present from this
    molecule in the forcefield */
const AtomSelection& CoulombFF::CoulombMolecule::selectedAtoms() const
{
    return d->selected_atoms;
}

/////////////
///////////// Implementation of CoulombFF::ChangedCoulombMolecule
/////////////

static const RegisterMetaType<CoulombFF::ChangedCoulombMolecule> r_changedmol;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const CoulombFF::ChangedCoulombMolecule &changedmol)
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
                                      CoulombFF::ChangedCoulombMolecule &changedmol)
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
CoulombFF::ChangedCoulombMolecule::ChangedCoulombMolecule()
{}

/** Construct a zero change - e.g. from 'mol' to 'mol' - this
    is useful when constructing changes that will later on move
    from 'mol' to 'newmol', e.g. ChangedCoulombMolecule(mol).change(newmol) */
CoulombFF::ChangedCoulombMolecule::ChangedCoulombMolecule(
                                          const CoulombMolecule &molecule)
          : oldmol(molecule),
            newmol(molecule)
{
    //oldparts, newparts and changed_cgids are empty as there is no change!
}

/** Construct the change of all of 'old_molecule' to all of 'new_molecule' */
CoulombFF::ChangedCoulombMolecule::ChangedCoulombMolecule(
                                          const CoulombMolecule &old_molecule,
                                          const CoulombMolecule &new_molecule)
          : oldmol(old_molecule),
            newmol(new_molecule),
            oldparts(old_molecule),
            newparts(new_molecule)
{}

/** Construct the change of 'old_molecule' to 'new_molecule', where only the
    CutGroups whose IDs are in 'cgids' are known to change */
CoulombFF::ChangedCoulombMolecule::ChangedCoulombMolecule(
                                          const CoulombMolecule &old_molecule,
                                          const CoulombMolecule &new_molecule,
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
        oldparts = CoulombMolecule(oldmol, changed_cgids);
        newparts = CoulombMolecule(newmol, changed_cgids);
    }
}

/** Copy constructor */
CoulombFF::ChangedCoulombMolecule::ChangedCoulombMolecule(
                                    const CoulombFF::ChangedCoulombMolecule &other)
          : oldmol(other.oldmol),
            newmol(other.newmol),
            oldparts(other.oldparts),
            newparts(other.newparts),
            changed_cgids(other.changed_cgids)
{}

/** Destructor */
CoulombFF::ChangedCoulombMolecule::~ChangedCoulombMolecule()
{}

/** Assignment operator */
CoulombFF::ChangedCoulombMolecule&
CoulombFF::ChangedCoulombMolecule::operator=(const CoulombFF::ChangedCoulombMolecule &other)
{
    oldmol = other.oldmol;
    newmol = other.newmol;
    oldparts = other.oldparts;
    newparts = other.newparts;
    changed_cgids = other.changed_cgids;

    return *this;
}

/** Comparison operator */
bool CoulombFF::ChangedCoulombMolecule::operator==(
                              const CoulombFF::ChangedCoulombMolecule &other) const
{
    return oldmol == other.oldmol and
           newmol == other.newmol and
           changed_cgids == other.changed_cgids;
}

/** Comparison operator */
bool CoulombFF::ChangedCoulombMolecule::operator!=(
                              const CoulombFF::ChangedCoulombMolecule &other) const
{
    return oldmol != other.oldmol or
           newmol != other.newmol or
           changed_cgids != other.changed_cgids;
}

/** Return whether or not this is empty (both old and new are empty) */
bool CoulombFF::ChangedCoulombMolecule::isEmpty() const
{
    return oldmol.isEmpty() and newmol.isEmpty();
}

/** Return whether the entire molecule has changed */
bool CoulombFF::ChangedCoulombMolecule::changedAll() const
{
    //everything has changed if changed_cgids is empty
    return changed_cgids.isEmpty();
}

/** Return the CutGroupIDs of all CutGroups that have changed since the
    last energy evaluation. */
const QSet<CutGroupID>&
CoulombFF::ChangedCoulombMolecule::changedGroups() const
{
    return changed_cgids;
}

/** Return the whole CoulombMolecule as it was in the old state */
const CoulombFF::CoulombMolecule&
CoulombFF::ChangedCoulombMolecule::oldMolecule() const
{
    return oldmol;
}

/** Return the whole CoulombMolecule as it is in the new state */
const CoulombFF::CoulombMolecule&
CoulombFF::ChangedCoulombMolecule::newMolecule() const
{
    return newmol;
}

/** Return the parts of the CoulombMolecule in the old state that
    have changed compared to the new state */
const CoulombFF::CoulombMolecule&
CoulombFF::ChangedCoulombMolecule::oldParts() const
{
    return oldparts;
}

/** Return the parts of the CoulombMolecule in the new state that
    have changed compared to the old state */
const CoulombFF::CoulombMolecule&
CoulombFF::ChangedCoulombMolecule::newParts() const
{
    return newparts;
}

/** Return the ChangedCoulombMolecule that represents the change from the old molecule
    to 'molecule' */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::change(const Molecule &molecule) const
{
    //calculate the change from newmol to molecule...
    ChangedCoulombMolecule next_change = newmol.change(molecule);

    if (next_change.isEmpty())
        //there was no further change
        return ChangedCoulombMolecule();
    else
        //now return the change from oldmol to newmol
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule());
}

static QSet<CutGroupID> operator+(const QSet<CutGroupID> &set0, const QSet<CutGroupID> &set1)
{
    QSet<CutGroupID> ret(set0);
    ret.unite(set1);

    return ret;
}

/** Return the ChangedCoulombMolecule that represents the change from the old molecule
    to 'residue' */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::change(const Residue &residue) const
{
    if (this->changedAll())
        return this->change(residue.molecule());

    //calculate the change from newmol to 'residue'...
    ChangedCoulombMolecule next_change = newmol.change(residue);

    if (next_change.isEmpty())
        return ChangedCoulombMolecule();
    else
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule(),
                                      changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCoulombMolecule that represents the change from the old molecule
    to 'residue' */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::change(const NewAtom &atom) const
{
    if (this->changedAll())
        return this->change(atom.molecule());

    ChangedCoulombMolecule next_change = newmol.change(atom);

    if (next_change.isEmpty())
        return ChangedCoulombMolecule();
    else
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule(),
                                      changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCoulombMolecule that represents the addition of all atoms
    in the molecule 'molecule' */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::add(const Molecule &molecule,
                                       const QString &chgproperty) const
{
    ChangedCoulombMolecule next_change = newmol.add(molecule, chgproperty);

    if (next_change.isEmpty())
        return ChangedCoulombMolecule();
    else
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule());
}

/** Return the ChangedCoulombMolecule that represents the addition of all atoms
    in the residue 'residue' */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::add(const Residue &residue,
                                       const QString &chgproperty) const
{
    ChangedCoulombMolecule next_change = newmol.add(residue, chgproperty);

    if (next_change.isEmpty())
        return ChangedCoulombMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule(),
                                      changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCoulombMolecule that represents the addition of the atom 'atom' */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::add(const NewAtom &atom,
                                       const QString &chgproperty) const
{
    ChangedCoulombMolecule next_change = newmol.add(atom, chgproperty);

    if (next_change.isEmpty())
        return ChangedCoulombMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule(),
                                      changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCoulombMolecule that represents the addition of the selected atoms */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::add(const AtomSelection &selected_atoms,
                                       const QString &chgproperty) const
{
    ChangedCoulombMolecule next_change = newmol.add(selected_atoms, chgproperty);

    if (next_change.isEmpty())
        return ChangedCoulombMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule(),
                                      changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCoulombMolecule that represents the removal of the entire molecule! */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::remove(const Molecule &molecule) const
{
    ChangedCoulombMolecule next_change = newmol.remove(molecule);

    if (next_change.isEmpty())
        return ChangedCoulombMolecule();
    else
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule());
}

/** Return the ChangedCoulombMolecule that represents the removal of the residue 'residue' */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::remove(const Residue &residue) const
{
    ChangedCoulombMolecule next_change = newmol.remove(residue);

    if (next_change.isEmpty())
        return ChangedCoulombMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule(),
                                      changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCoulombMolecule that represents the removal of 'atom' */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::remove(const NewAtom &atom) const
{
    ChangedCoulombMolecule next_change = newmol.remove(atom);

    if (next_change.isEmpty())
        return ChangedCoulombMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule(),
                                      changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCoulombMolecule that represents the removal of the
    selected atoms. */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::remove(const AtomSelection &selected_atoms) const
{
    ChangedCoulombMolecule next_change = newmol.remove(selected_atoms);

    if (next_change.isEmpty())
        return ChangedCoulombMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCoulombMolecule(oldmol, next_change.newMolecule(),
                                      changed_cgids + next_change.changed_cgids);
}

/////////////
///////////// Implementation of CoulombFF
/////////////

/** This function returns the Coulomb energy of two groups based on the
    inter-atomic inverse-distances stored in 'distmatrix'
    and using the Coulomb parameters in chgs0 and chgs1 */
double CoulombFF::calculatePairEnergy(DistMatrix &distmatrix,
                                      const QVector<ChargeParameter> &chgs0,
                                      const QVector<ChargeParameter> &chgs1)
{
    int nats0 = distmatrix.nOuter();
    int nats1 = distmatrix.nInner();

    BOOST_ASSERT( chgs0.count() == nats0 );
    BOOST_ASSERT( chgs1.count() == nats1 );

    double cnrg = 0;

    const ChargeParameter *chgs0_array = chgs0.constData();
    const ChargeParameter *chgs1_array = chgs1.constData();

    //loop over all pairs of atoms
    for (int i=0; i<nats0; ++i)
    {
        distmatrix.setOuterIndex(i);
        double chg0 = SireUnits::one_over_four_pi_eps0 * chgs0_array[i].charge();

        if (chg0 != 0)
        {
            for (int j=0; j<nats1; ++j)
            {
                //Coulomb energy
                cnrg += chg0 * chgs1_array[j].charge() * distmatrix[j];
            }
        }
    }

    return cnrg;
}

/** This function is used to calculate and return the self-energy of a group,
    using the inverse-square-distances and parameters stored
    in the passed distance matrix with passed partial charges. */
double CoulombFF::calculateSelfEnergy(DistMatrix &distmatrix,
                                      const QVector<ChargeParameter> &chgs)
{
    int nats = distmatrix.nOuter();

    BOOST_ASSERT( distmatrix.nInner() == distmatrix.nOuter() );
    BOOST_ASSERT( chgs.count() == nats );

    double cnrg = 0;

    const ChargeParameter *chgs_array = chgs.constData();

    for (int i=0; i<nats-1; ++i)
    {
        distmatrix.setOuterIndex(i);

        double chg0 = SireUnits::one_over_four_pi_eps0 * chgs_array[i].charge();

        if (chg0 != 0)
        {
            for (int j=i+1; j<nats; ++j)
            {
                cnrg += chg0 * chgs_array[j].charge() * distmatrix[j];
            }
        }
    }

    return cnrg;
}

/** Calculate and return the Coulomb energy of interaction of group0,
    with partial charges in 'chgs0' and group1, with partial charges
    in 'chgs1', using the space 'space', and using the provided
    distance matrix as a temporary workspace. */
double CoulombFF::calculateEnergy(const CoordGroup &group0,
                                  const QVector<ChargeParameter> &chgs0,
                                  const CoordGroup &group1,
                                  const QVector<ChargeParameter> &chgs1,
                                  const Space &space,
                                  const SwitchingFunction &switchfunc,
                                  DistMatrix &distmatrix)
{
    if ( not space.beyond(switchfunc.cutoffDistance(), group0, group1) )
    {
        double mindist = space.calcInvDist(group0, group1, distmatrix);

        double sclcoul = switchfunc.electrostaticScaleFactor(mindist);

        if (sclcoul != 0)
        {
            return sclcoul * CoulombFF::calculatePairEnergy(distmatrix,
                                                            chgs0, chgs1);
        }
    }

    return 0;
}

/** Calculate the Coulomb energy of interaction of the atoms within the
    group 'group', with partial charges in 'chgs', using the space 'space',
    and working in the temporary workspace provided by the passed distance
    matrix */
double CoulombFF::calculateEnergy(const CoordGroup &group,
                                  const QVector<ChargeParameter> &chgs,
                                  const Space &space,
                                  DistMatrix &distmatrix)
{
    space.calcInvDist(group, distmatrix);

    return CoulombFF::calculateSelfEnergy(distmatrix, chgs);
}

/** Calculate and return the Coulomb energy of interaction between two
    CoulombMolecules, using the space 'space', combining rules 'combrules'
    and switching function 'switchfunc'. The calculation will use
    the provided distance matrix as a temporary workspace */
double CoulombFF::calculateEnergy(const CoulombFF::CoulombMolecule &mol0,
                                  const CoulombFF::CoulombMolecule &mol1,
                                  const Space &space,
                                  const SwitchingFunction &switchfunc,
                                  DistMatrix &distmatrix)
{
    int ncg0 = mol0.coordinates().count();
    int ncg1 = mol1.coordinates().count();

    if (ncg0 == 1 and ncg1 == 1)
    {
        return calculateEnergy( mol0.coordinates().constData()[0],
                                mol0.charges().constData()[0],
                                mol1.coordinates().constData()[0],
                                mol1.charges().constData()[0],

                                space, switchfunc, distmatrix);
    }
    else if (ncg0 > 0 and ncg1 > 0)
    {
        double cnrg = 0;

        const CoordGroup *cg0array = mol0.coordinates().constData();
        const CoordGroup *cg1array = mol1.coordinates().constData();

        const QVector<ChargeParameter> *chg0array = mol0.charges().constData();
        const QVector<ChargeParameter> *chg1array = mol1.charges().constData();

        for (int i=0; i<ncg0; ++i)
        {
            const CoordGroup &group0 = cg0array[i];
            const QVector<ChargeParameter> &chgs0 = chg0array[i];

            for (int j=0; j<ncg1; ++j)
            {
                const CoordGroup &group1 = cg1array[j];
                const QVector<ChargeParameter> &chgs1 = chg1array[j];

                cnrg += calculateEnergy(group0, chgs0, group1, chgs1,
                                        space, switchfunc, distmatrix);
            }
        }

        return cnrg;
    }
    else
        return 0;
}

/** Calculate and return the Coulomb energy of interaction between the pairs
    of atoms within a single CoulombMolecule, using the space 'space',
    and switching function 'switchfunc'. The calculation will use the provided
    distance matrix as a temporary workspace  */
double CoulombFF::calculateEnergy(const CoulombFF::CoulombMolecule &mol,
                                  const Space &space,
                                  const SwitchingFunction &switchfunc,
                                  DistMatrix &distmatrix)
{
    int ncg = mol.coordinates().count();

    if (ncg == 1)
    {
        //calculate only the self-energy of the first CutGroup
        return calculateEnergy(mol.coordinates().constData()[0],
                               mol.charges().constData()[0],
                               space, distmatrix);
    }
    else if (ncg > 1)
    {
        double cnrg = 0;

        const CoordGroup *cgarray = mol.coordinates().constData();
        const QVector<ChargeParameter> *chgarray = mol.charges().constData();

        for (int i=0; i<ncg-1; ++i)
        {
            const CoordGroup &group0 = cgarray[i];
            const QVector<ChargeParameter> &chgs0 = chgarray[i];

            //add on the self-energy
            cnrg += calculateEnergy(group0, chgs0, space, distmatrix);

            for (int j=i+1; j<ncg; ++j)
            {
                //calculate the group-group energy
                const CoordGroup &group1 = cgarray[j];
                const QVector<ChargeParameter> &chgs1 = chgarray[j];

                cnrg += calculateEnergy(group0, chgs0, group1, chgs1,
                                        space, switchfunc, distmatrix);
            }
        }

        //have to add on the self-energy of the last CutGroup of the molecule
        const CoordGroup &group = cgarray[ncg-1];
        const QVector<ChargeParameter> &chgs = chgarray[ncg-1];

        cnrg += calculateEnergy(group, chgs, space, distmatrix);

        return cnrg;
    }
    else
        return 0;
}

static const RegisterMetaType<CoulombFF> r_coulff(MAGIC_ONLY, "SireMM::CoulombFF");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CoulombFF &coulff)
{
    writeHeader(ds, r_coulff, 1);

    SharedDataStream sds(ds);

    sds << coulff.spce << coulff.switchfunc
        << static_cast<const FFBase&>(coulff);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CoulombFF &coulff)
{
    VersionID v = readHeader(ds, r_coulff);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> coulff.spce >> coulff.switchfunc
            >> static_cast<FFBase&>(coulff);
    }
    else
        throw version_error(v, "1", r_coulff, CODELOC);

    return ds;
}

/** Null constructor */
CoulombFF::CoulombFF() : FFBase()
{
    this->registerComponents();
}

/** Construct a CoulombFF using the specified space and switching function */
CoulombFF::CoulombFF(const Space &space, const SwitchingFunction &switchingfunction)
          : FFBase(),
            spce(space), switchfunc(switchingfunction)
{
    this->registerComponents();
}

/** Copy constructor */
CoulombFF::CoulombFF(const CoulombFF &other)
          : FFBase(other),
            spce(other.spce), switchfunc(other.switchfunc)
{
    //get the pointer from the base class...
    components_ptr = dynamic_cast<const CoulombFF::Components*>( &(FFBase::components()) );
    BOOST_ASSERT( components_ptr != 0 );
}

/** Destructor */
CoulombFF::~CoulombFF()
{}

/** Register the energy components associated with this forcefield */
void CoulombFF::registerComponents()
{
    std::auto_ptr<CoulombFF::Components> ptr( new CoulombFF::Components(*this) );

    FFBase::registerComponents(ptr.get());

    components_ptr = ptr.release();
}
