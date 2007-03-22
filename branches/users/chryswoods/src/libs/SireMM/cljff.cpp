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

#include "cljff.h"
#include "ljpair.h"

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
///////////// Implementation of CLJFF::Components
/////////////

/** Constructor */
CLJFF::Components::Components() : FFBase::Components()
{}

/** Construct using the supplied forcefield */
CLJFF::Components::Components(const FFBase &ffbase, const Symbols &symbols)
      : FFBase::Components( ffbase, addToSymbols(symbols, x(), y(), z()) ),
        e_coul(ffbase, "coul", x(), y(), z()),
        e_lj(ffbase, "lj", x(), y(), z())
{
    this->registerComponent(e_coul);
    this->registerComponent(e_lj);
}

/** Copy constructor */
CLJFF::Components::Components(const CLJFF::Components &other)
      : FFBase::Components(other),
        e_coul(other.e_coul),
        e_lj(other.e_lj)
{}

/** Destructor */
CLJFF::Components::~Components()
{}

/** Assignment operator */
CLJFF::Components& CLJFF::Components::operator=(const CLJFF::Components &other)
{
    e_coul = other.e_coul;
    e_lj = other.e_lj;

    FFBase::Components::operator=(other);

    return *this;
}

/** Set the forcefield */
void CLJFF::Components::setForceField(const FFBase &ffbase)
{
    *this = CLJFF::Components(ffbase);
}

/** Describe the coulomb component */
QString CLJFF::Components::describe_coulomb()
{
    return QObject::tr("The total coulomb (electrostatic) energy of the forcefield.");
}

/** Describe the LJ component */
QString CLJFF::Components::describe_lj()
{
    return QObject::tr("The total Lennard-Jones (van der waals) energy of the forcefield.");
}

/////////////
///////////// Implementation of CLJFF::Parameters
/////////////

/** Constructor - by default the LJ parameters
    come from the 'ljs' property */
CLJFF::Parameters::Parameters()
      : FFBase::Parameters(), chg_params("coulomb", "charges"), lj_params("lj", "ljs")
{}

/** Copy constructor */
CLJFF::Parameters::Parameters(const CLJFF::Parameters &other)
      : FFBase::Parameters(other),
        chg_params(other.chg_params), lj_params(other.lj_params)
{}

/** Destructor */
CLJFF::Parameters::~Parameters()
{}

/** Static object returned by CCLJFF::parameters() */
CLJFF::Parameters CLJFF::Parameters::default_sources;

/////////////
///////////// Implementation of CLJFF::Groups
/////////////

CLJFF::Groups::Groups() : FFBase::Groups()
{}

CLJFF::Groups::Groups(const CLJFF::Groups &other)
     : FFBase::Groups(other)
{}

CLJFF::Groups::~Groups()
{}

/////////////
///////////// Implementation of CLJFF::CLJMoleculeData
/////////////

namespace SireMM
{

/** Private class used to hold the data of CLJFF::CLJMolecule */
class CLJFF::CLJMoleculeData : public QSharedData
{
public:
    CLJMoleculeData();

    CLJMoleculeData(const Molecule &mol,
                    const QString &chgproperty, const QString &ljproperty);
    CLJMoleculeData(const Residue &residue,
                    const QString &chgproperty, const QString &ljproperty);
    CLJMoleculeData(const NewAtom &atom,
                    const QString &chgproperty, const QString &ljproperty);
    CLJMoleculeData(const Molecule &mol, const AtomSelection &selectedatoms,
                    const QString &chgproperty, const QString &ljproperty);

    CLJMoleculeData(const CLJMoleculeData &other);

    ~CLJMoleculeData();

    CLJMoleculeData& operator=(const CLJMoleculeData &other);

    bool operator==(const CLJMoleculeData &other) const;
    bool operator!=(const CLJMoleculeData &other) const;

    void rebuildAll();
    void rebuildCoordinates();

    bool rebuildAll(const QSet<CutGroupID> &cgids);
    bool rebuildCoordinates(const QSet<CutGroupID> &cgids);

    /** The actual molecule */
    Molecule molecule;

    /** The name of the property associated with the atomic partial charges */
    QString chg_property;

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

    /** The partial charges of the atoms, in the same order
        as coords. Atoms that are not selected for this forcefield
        have a zero charge */
    AtomicCharges chgs;

    /** The LJ parameters of the atoms, in the same order as
        coords. Atoms that are not selected for this forcefield
        have a zero LJ parameter. */
    AtomicLJs ljs;

    static QSharedDataPointer<CLJMoleculeData> shared_null;
};

/** Constructor */
CLJFF::CLJMoleculeData::CLJMoleculeData() : QSharedData()
{}

/** Construct to represent all of the molecule 'mol' */
CLJFF::CLJMoleculeData::CLJMoleculeData(const Molecule &mol,
                                        const QString &chgproperty,
                                        const QString &ljproperty)
                     : QSharedData(),
                       molecule(mol),
                       chg_property(chgproperty),
                       lj_property(ljproperty),
                       selected_atoms(mol)
{
    this->rebuildAll();
}

/** Construct to represent all of the residue 'residue' */
CLJFF::CLJMoleculeData::CLJMoleculeData(const Residue &residue,
                                        const QString &chgproperty,
                                        const QString &ljproperty)
                     : QSharedData(),
                       molecule(residue.molecule()),
                       chg_property(chgproperty),
                       lj_property(ljproperty),
                       selected_atoms(residue)
{
    this->rebuildAll();
}

/** Construct to represent the atom 'atom' */
CLJFF::CLJMoleculeData::CLJMoleculeData(const NewAtom &atom,
                                        const QString &chgproperty,
                                        const QString &ljproperty)
                     : QSharedData(),
                       molecule(atom.molecule()),
                       chg_property(chgproperty),
                       lj_property(ljproperty),
                       selected_atoms(atom)
{
    this->rebuildAll();
}

/** Construct to represent the selected atoms from 'selectatoms' that are
    in the molecule 'mol' */
CLJFF::CLJMoleculeData::CLJMoleculeData(const Molecule &mol,
                                        const AtomSelection &selectatoms,
                                        const QString &chgproperty,
                                        const QString &ljproperty)
                     : QSharedData(),
                       molecule(mol),
                       chg_property(chgproperty),
                       lj_property(ljproperty),
                       selected_atoms(selectatoms)
{
    //ensure that the selected atoms and the molecule are compatible
    selected_atoms.assertCompatibleWith(molecule);

    this->rebuildAll();
}

/** Copy constructor */
CLJFF::CLJMoleculeData::CLJMoleculeData(const CLJFF::CLJMoleculeData &other)
                       : QSharedData(),
                         molecule(other.molecule),
                         chg_property(other.chg_property),
                         lj_property(other.lj_property),
                         selected_atoms(other.selected_atoms),
                         cg_index(other.cg_index),
                         coords(other.coords),
                         chgs(other.chgs),
                         ljs(other.ljs)
{}

/** Destructor */
CLJFF::CLJMoleculeData::~CLJMoleculeData()
{}

/** Assignment operator */
CLJFF::CLJMoleculeData& CLJFF::CLJMoleculeData::operator=(const CLJFF::CLJMoleculeData &other)
{
    if (this != &other)
    {
        molecule = other.molecule;
        lj_property = other.lj_property;
        chg_property = other.chg_property;
        selected_atoms = other.selected_atoms;
        cg_index = other.cg_index;
        coords = other.coords;
        chgs = other.chgs;
        ljs = other.ljs;
    }

    return *this;
}

/** Comparison operator */
bool CLJFF::CLJMoleculeData::operator==(const CLJFF::CLJMoleculeData &other) const
{
    return molecule == other.molecule and
           chg_property == other.chg_property and
           lj_property == other.lj_property and
           selected_atoms == other.selected_atoms and
           cg_index == other.cg_index;
}

/** Comparison operator */
bool CLJFF::CLJMoleculeData::operator!=(const CLJFF::CLJMoleculeData &other) const
{
    return molecule != other.molecule or
           chg_property != other.chg_property or
           lj_property != other.lj_property or
           selected_atoms != other.selected_atoms or
           cg_index != other.cg_index;
}

/** Rebuild all of the coordinate and LJ data from scratch

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
void CLJFF::CLJMoleculeData::rebuildAll()
{
    if (selected_atoms.selectedAll())
    {
        //all atoms are selected - we can therefore short-circuit everything!
        chgs = molecule.getProperty(chg_property);
        ljs = molecule.getProperty(lj_property);
        cg_index.clear();
        coords = molecule.coordGroups();

        return;
    }

    if (selected_atoms.isEmpty())
    {
        //there has been nothing selected!
        coords.clear();
        chgs.clear();
        ljs.clear();
        cg_index.clear();

        return;
    }

    uint nselectedgroups = selected_atoms.nSelectedCutGroups();
    uint ngroups = molecule.nCutGroups();

    if (nselectedgroups == ngroups)
    {
        coords = molecule.coordGroups();
        chgs = molecule.getProperty(chg_property);
        ljs = molecule.getProperty(lj_property);
        cg_index.clear();

        //loop through all CutGroups and zero the parameters of any
        //atoms that are not selected
        for (CutGroupID i(0); i<ngroups; ++i)
        {
            if ( not selected_atoms.selectedAll(i) )
            {
                QVector<ChargeParameter> &chgparams = chgs[i];
                QVector<LJParameter> &ljparams = ljs[i];

                //there are some missing parameters to zero!
                uint nats = molecule.nAtoms(i);

                for (AtomID j(0); j<nats; ++j)
                {
                    if ( not selected_atoms.selected(CGAtomID(i,j)) )
                    {
                        //this atom has not been selected
                        // - zero its charge and LJ parameter
                        chgparams[j] = ChargeParameter::dummy();
                        ljparams[j] = LJParameter::dummy();
                    }
                }
            }
        }

        return;
    }

    const QVector<CoordGroup> &all_coords = molecule.coordGroups();
    AtomicCharges all_chgs = molecule.getProperty(chg_property);
    AtomicLJs all_ljs = molecule.getProperty(lj_property);

    coords = QVector<CoordGroup>( nselectedgroups );
    chgs = AtomicCharges( QVector<ChargeParameter>( nselectedgroups ) );
    ljs = AtomicLJs( QVector<LJParameter>( nselectedgroups ) );

    cg_index.clear();
    cg_index.reserve(nselectedgroups);

    int idx = 0;

    for (CutGroupID i(0); i<ngroups; ++i)
    {
        if (not selected_atoms.selectedNone(i))
        {
            coords[idx] = all_coords[i];
            chgs[idx] = all_chgs[i];
            ljs[idx] = all_ljs[i];

            cg_index.insert(i, idx);
            ++idx;

            if (not selected_atoms.selectedAll(i))
            {
                uint nats = molecule.nAtoms(i);

                QVector<ChargeParameter> &chgparams = chgs[idx];
                QVector<LJParameter> &ljparams = ljs[idx];

                for (AtomID j(0); j<nats; ++j)
                {
                    if ( not selected_atoms.selected(CGAtomID(i,j)) )
                    {
                        chgparams[j] = ChargeParameter::dummy();
                        ljparams[j] = LJParameter::dummy();
                    }
                }
            }
        }
    }
}

/** Rebuild all of the coordinates and LJ data for the CutGroups whose
    IDs are in 'cgids'. Return whether or not there are any atoms
    from these CutGroups that are in the molecule selection */
bool CLJFF::CLJMoleculeData::rebuildAll(const QSet<CutGroupID>&)
{
    //This can be heavily optimised - I just don't have the time now!
    this->rebuildAll();
    return true;
}

/** Rebuild all of the coordinate data from scratch */
void CLJFF::CLJMoleculeData::rebuildCoordinates()
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
bool CLJFF::CLJMoleculeData::rebuildCoordinates(const QSet<CutGroupID> &cgids)
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

/** Shared null CLJMoleculeData */
QSharedDataPointer<CLJFF::CLJMoleculeData> CLJFF::CLJMoleculeData::shared_null(
                                                    new CLJFF::CLJMoleculeData() );

} // end of namespace SireMM


static const RegisterMetaType<CLJFF::CLJMoleculeData> r_cljmoldata(MAGIC_ONLY,
                                                      "SireMM::CLJFF::CLJMoleculeData");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const CLJFF::CLJMoleculeData &cljmoldata)
{
    writeHeader(ds, r_cljmoldata, 1);

    SharedDataStream sds(ds);

    sds << cljmoldata.molecule
        << cljmoldata.chg_property
        << cljmoldata.lj_property
        << cljmoldata.selected_atoms;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      CLJFF::CLJMoleculeData &cljmoldata)
{
    VersionID v = readHeader(ds, r_cljmoldata);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        Molecule mol;
        QString chg_property;
        QString lj_property;
        AtomSelection selected_atoms;

        sds >> mol >> chg_property >> lj_property >> selected_atoms;

        cljmoldata = CLJFF::CLJMoleculeData(mol, selected_atoms,
                                            chg_property, lj_property);
    }
    else
        throw version_error(v, "1", r_cljmoldata, CODELOC);

    return ds;
}

/////////////
///////////// Implementation of CLJFF::CLJMolecule
/////////////

static const RegisterMetaType<CLJFF::CLJMolecule> r_cljmol;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const CLJFF::CLJMolecule &cljmol)
{
    writeHeader(ds, r_cljmol, 1);

    SharedDataStream sds(ds);
    sds << cljmol.d;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJFF::CLJMolecule &cljmol)
{
    VersionID v = readHeader(ds, r_cljmol);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> cljmol.d;
    }
    else
        throw version_error(v, "1", r_cljmol, CODELOC);

    return ds;
}

/** Null constructor - creates an empty CLJMolecule */
CLJFF::CLJMolecule::CLJMolecule() : d(CLJFF::CLJMoleculeData::shared_null)
{}

/** Construct from the passed molecule, using 'ljproperty' to find the
    molecule's LJ parameters.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
CLJFF::CLJMolecule::CLJMolecule(const Molecule &molecule,
                                const QString &chgproperty,
                                const QString &ljproperty)
                   : d( new CLJFF::CLJMoleculeData(molecule,
                                                   chgproperty,
                                                   ljproperty) )
{}

/** Construct from the passed residue, using 'ljproperty' to find the
    residue's LJ parameters.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
CLJFF::CLJMolecule::CLJMolecule(const Residue &residue,
                                const QString &chgproperty,
                                const QString &ljproperty)
                   : d( new CLJFF::CLJMoleculeData(residue,
                                                   chgproperty,
                                                   ljproperty) )
{}

/** Construct from the passed atom, using 'ljproperty' to find the
    atom's LJ parameters.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
CLJFF::CLJMolecule::CLJMolecule(const NewAtom &atom,
                                const QString &chgproperty,
                                const QString &ljproperty)
                   : d( new CLJFF::CLJMoleculeData(atom,
                                                   chgproperty,
                                                   ljproperty) )
{}

/** Construct from the selected atoms of the passed molecule, using
    'ljproperty' to find the atoms' LJ parameters.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
CLJFF::CLJMolecule::CLJMolecule(const Molecule &molecule,
                                const AtomSelection &selected_atoms,
                                const QString &chgproperty,
                                const QString &ljproperty)
                   : d( new CLJFF::CLJMoleculeData(molecule,
                                                   selected_atoms,
                                                   chgproperty,
                                                   ljproperty) )
{}

/** Construct a copy of 'other', but masked by the CutGroups in 'groups' */
CLJFF::CLJMolecule::CLJMolecule(const CLJMolecule &other,
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
        const AtomicLJs &ljs = other.d->ljs;
        const QVector<CoordGroup> &coords = other.d->coords;

        int ngroups = groups.count();

        QVector< QVector<ChargeParameter> > new_chgs(ngroups);
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
            new_chgs[i] = chgs[*it];
            new_ljs[i] = ljs[*it];

            new_index.insert(*it,i);
            ++i;
        }

        d->coords = new_coords;
        d->chgs = AtomicCharges(new_chgs);
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
            d->chgs.remove(idx);
            d->ljs.remove(idx);
        }
    }
}

/** Copy constructor */
CLJFF::CLJMolecule::CLJMolecule(const CLJMolecule &other)
                   : d( other.d )
{}

/** Destructor */
CLJFF::CLJMolecule::~CLJMolecule()
{}

/** Assignment operator */
CLJFF::CLJMolecule& CLJFF::CLJMolecule::operator=(const CLJMolecule &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool CLJFF::CLJMolecule::operator==(const CLJFF::CLJMolecule &other) const
{
    return d == other.d or (*d == *(other.d));
}

/** Comparison operator */
bool CLJFF::CLJMolecule::operator!=(const CLJFF::CLJMolecule &other) const
{
    return d != other.d and (*d != *(other.d));
}

/** Return whether or not this is empty (has no atoms selected */
bool CLJFF::CLJMolecule::isEmpty() const
{
    return d->selected_atoms.isEmpty();
}

/** Return the actual molecule */
const Molecule& CLJFF::CLJMolecule::molecule() const
{
    return d->molecule;
}

/** Return a ChangedCLJMolecule that represents the change from the
    CLJMolecule in its current state to 'molecule', where only the
    CutGroups whose IDs are in 'cgids' have changed. (if cgids is
    empty then all CutGroups have changed)

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule
CLJFF::CLJMolecule::_pvt_change(const Molecule &molecule,
                                const QSet<CutGroupID> &cgids,
                                const QString &chgproperty,
                                const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        //this is a null Molecule
        return ChangedCLJMolecule();

    //assert that this is the same molecule
    d->molecule.assertSameMolecule(molecule);

    if ( (chgproperty.isNull() or chgproperty == d->chg_property) and
         (ljproperty.isNull() or ljproperty == d->lj_property) )
    {
        //has there been a change?
        if ( d->molecule.version() == molecule.version() )
            //no - there hasn't!
            return ChangedCLJMolecule();

        //the molecule may have changed
        CLJMolecule newmol(*this);

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

            return ChangedCLJMolecule(*this, newmol);
        }
        else
        {
            //only part of the molecule has changed
            if (molecule.version().major() != d->molecule.version().major())
            {
                //there has been a major change

                if (newmol.d->rebuildAll(cgids))
                    //the part of the molecule in this forcefield has changed!
                    return ChangedCLJMolecule(*this, newmol, cgids);
                else
                    //no part of the molecule that was in this forcefield has changed
                    return ChangedCLJMolecule();
            }
            else
            {
                //only the coordinates have changed

                if (newmol.d->rebuildCoordinates(cgids))
                    //the part of the molecule in this forcefield has moved
                    return ChangedCLJMolecule(*this, newmol, cgids);
                else
                    //no part of the molecule in this forcefield has moved
                    return ChangedCLJMolecule();
            }
        }
    }
    else
    {
        //there has been a change of property - the entire molecule
        //needs to be rebuilt
        CLJMolecule newmol(*this);

        newmol.d->molecule = molecule;
        newmol.d->chg_property = chgproperty;
        newmol.d->lj_property = ljproperty;

        newmol.d->rebuildAll();

        return ChangedCLJMolecule(*this, newmol);
    }
}

/** Return a ChangedCLJMolecule that represents the change from the
    CLJMolecule in its current state to 'molecule'. Note that
    the new state must have the same MoleculeID as the current state!

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule
CLJFF::CLJMolecule::change(const Molecule &molecule) const
{
    return this->_pvt_change(molecule, QSet<CutGroupID>());
}

/** Return a ChangedCLJMolecule that represents the change from the
    CLJMolecule in its current state to include the changes in 'residue'

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule
CLJFF::CLJMolecule::change(const Residue &residue) const
{
    return this->_pvt_change(residue.molecule(), residue.info().cutGroupIDs());
}

/** Return a ChangedCLJMolecule that represents the change from
    the CLJMolecule in its current state to include the changes in 'atom'

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule
CLJFF::CLJMolecule::change(const NewAtom &atom) const
{
    QSet<CutGroupID> cgid;
    cgid.insert(atom.cgAtomID().cutGroupID());

    return this->_pvt_change(atom.molecule(), cgid);
}

/** Private function used to return the ChangedCLJMolecule that represents
    the change from the CLJMolecule in its current state to 'molecule', with
    the new selection 'selected_atoms', with the guarantee that only the CutGroups
    whose IDs are in 'cgids' have changed. (if cgids is empty then all of the
    CutGroups have changed)

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule
CLJFF::CLJMolecule::_pvt_change(const Molecule &molecule,
                                const QSet<CutGroupID> &cgids,
                                const AtomSelection &selected_atoms,
                                const QString &chgproperty,
                                const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedCLJMolecule();

    if (selected_atoms == d->selected_atoms)
        //there has been no change in the atom selections
        return this->_pvt_change(molecule, cgids, chgproperty, ljproperty);

    d->molecule.assertSameMolecule(molecule);

    CLJMolecule newmol(*this);

    newmol.d->molecule = molecule;
    newmol.d->selected_atoms = selected_atoms;

    if ( (chgproperty.isNull() or chgproperty == d->chg_property) and
         (ljproperty.isNull() or ljproperty == d->lj_property) )
    {
        //there is no change in the property used to get the LJ parameters
        if (cgids.isEmpty())
        {
            //the entire molecule has changed
            newmol.d->rebuildAll();

            return ChangedCLJMolecule(*this, newmol);
        }
        else
        {
            //only parts of the molecule have changed
            newmol.d->rebuildAll(cgids);

            return ChangedCLJMolecule(*this, newmol, cgids);
        }
    }
    else
    {
        //there has been a change of property - we need to rebuild
        //the entire molecule
        newmol.d->chg_property = chgproperty;
        newmol.d->lj_property = ljproperty;
        newmol.d->rebuildAll();

        return ChangedCLJMolecule(*this, newmol);
    }
}

/** Return a ChangedCLJMolecule that represents the change from the CLJMolecule
    in its current state to include the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule
CLJFF::CLJMolecule::add(const Molecule &molecule,
                        const QString &chgproperty,
                        const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedCLJMolecule(*this, CLJMolecule(molecule,
                                                     chgproperty,
                                                     ljproperty));
    else
        return this->_pvt_change(molecule, QSet<CutGroupID>(),
                                 AtomSelection(molecule),
                                 chgproperty,
                                 ljproperty);
}

/** Return a ChangedCLJMolecule that represents the change from the CLJMolecule
    in its current state to include as well the passed residue, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule
CLJFF::CLJMolecule::add(const Residue &residue,
                        const QString &chgproperty,
                        const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedCLJMolecule(*this, CLJMolecule(residue,
                                                     chgproperty,
                                                     ljproperty));
    else
    {
        AtomSelection new_selection = d->selected_atoms;

        new_selection.selectAll(residue.number());

        return this->_pvt_change(residue.molecule(), residue.info().cutGroupIDs(),
                                 new_selection, chgproperty, ljproperty);
    }
}

/** Return a ChangedCLJMolecule that represents the change from the CLJMolecule
    in its current state to include as well the passed atom, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule
CLJFF::CLJMolecule::add(const NewAtom &atom,
                        const QString &chgproperty,
                        const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        return ChangedCLJMolecule(*this, CLJMolecule(atom,
                                                     chgproperty,
                                                     ljproperty));
    else
    {
        AtomSelection new_selection = d->selected_atoms;

        new_selection.select(atom.cgAtomID());

        QSet<CutGroupID> cgid;
        cgid.insert(atom.cgAtomID().cutGroupID());

        return this->_pvt_change(atom.molecule(), cgid, new_selection,
                                 chgproperty, ljproperty);
    }
}

/** Return a ChangedCLJMolecule that represents the change from the CLJMolecule
    in its current state to include as well the passed selection

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule
CLJFF::CLJMolecule::add(const AtomSelection &selected_atoms,
                        const QString &chgproperty,
                        const QString &ljproperty) const
{
    AtomSelection new_selection = d->selected_atoms;

    new_selection.selectAll(selected_atoms);

    Molecule oldmol = d->molecule;

    return this->_pvt_change(oldmol, selected_atoms.selectedCutGroups(),
                             new_selection, chgproperty, ljproperty);
}

/** Return a ChangedCLJMolecule that represents the change from the CLJMolecule
    in its current state to remove the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule CLJFF::CLJMolecule::remove(const Molecule &molecule) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselectAll();

    return this->_pvt_change(molecule, QSet<CutGroupID>(), new_selection);
}

/** Return a ChangedCLJMolecule that represents the change from the CLJMolecule
    in its current state to remove the passed residue, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule CLJFF::CLJMolecule::remove(const Residue &residue) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselectAll(residue.number());

    return this->_pvt_change(residue.molecule(), residue.info().cutGroupIDs(), new_selection);
}

/** Return a ChangedCLJMolecule that represents the change from the CLJMolecule
    in its current state to remove the passed atom, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule CLJFF::CLJMolecule::remove(const NewAtom &atom) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselect(atom.cgAtomID());

    QSet<CutGroupID> cgid;
    cgid.insert(atom.cgAtomID().cutGroupID());

    return this->_pvt_change(atom.molecule(), cgid, new_selection);
}

/** Return a ChangedCLJMolecule that represents the change from the CLJMolecule
    in its current state to remove the passed atom selection

    \throw SireError::incompatible_error
*/
CLJFF::ChangedCLJMolecule CLJFF::CLJMolecule::remove(const AtomSelection &selected_atoms) const
{
    AtomSelection new_selection = d->selected_atoms;
    new_selection.deselectAll(selected_atoms);

    Molecule oldmol = d->molecule;

    return this->_pvt_change(oldmol, selected_atoms.selectedCutGroups(),
                             new_selection);
}

/** Return the name of the property used to get the partial charges */
const QString& CLJFF::CLJMolecule::chgProperty() const
{
    return d->chg_property;
}

/** Return the name of the property used to get the LJ parameters */
const QString& CLJFF::CLJMolecule::ljProperty() const
{
    return d->lj_property;
}

/** Return the CoordGroups of the CutGroups that have atoms that are
    in this forcefield. Note that there is no defined order to the
    array of CoordGroups, other than it is the same order as the
    array of LJ parameter groups returned by ljParameters(). */
const QVector<CoordGroup>& CLJFF::CLJMolecule::coordinates() const
{
    return d->coords;
}

/** Return the partial charges of all of the atoms that
    are in this forcefield. The parameters are arranged
    by CutGroups and are in the same order as the coordinates
    as returned by coordinates(). Atoms which are not in this
    forcefield have zero charge. */
const AtomicCharges& CLJFF::CLJMolecule::charges() const
{
    return d->chgs;
}

/** Return the LJ parameters of all of the atoms that are in
    this forcefield. The parameters are arranged by CutGroup and
    are in the same order as the coordinates as returned by
    coordinates(). Atoms which are not in this forcefield
    have zero LJ parameters. */
const AtomicLJs& CLJFF::CLJMolecule::ljParameters() const
{
    return d->ljs;
}

/** Return whether or not this CLJMolecule represents the whole
    of the molecule */
bool CLJFF::CLJMolecule::isWholeMolecule() const
{
    return d->selected_atoms.selectedAll();
}

/** Return the selection of atoms that are present from this
    molecule in the forcefield */
const AtomSelection& CLJFF::CLJMolecule::selectedAtoms() const
{
    return d->selected_atoms;
}

/////////////
///////////// Implementation of CLJFF::ChangedCLJMolecule
/////////////

static const RegisterMetaType<CLJFF::ChangedCLJMolecule> r_changedmol;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const CLJFF::ChangedCLJMolecule &changedmol)
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
                                      CLJFF::ChangedCLJMolecule &changedmol)
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
CLJFF::ChangedCLJMolecule::ChangedCLJMolecule()
{}

/** Construct a zero change - e.g. from 'mol' to 'mol' - this
    is useful when constructing changes that will later on move
    from 'mol' to 'newmol', e.g. ChangedCLJMolecule(mol).change(newmol) */
CLJFF::ChangedCLJMolecule::ChangedCLJMolecule(const CLJMolecule &molecule)
                          : oldmol(molecule),
                            newmol(molecule)
{
    //oldparts, newparts and changed_cgids are empty as there is no change!
}

/** Construct the change of all of 'old_molecule' to all of 'new_molecule' */
CLJFF::ChangedCLJMolecule::ChangedCLJMolecule(const CLJMolecule &old_molecule,
                                             const CLJMolecule &new_molecule)
                          : oldmol(old_molecule),
                            newmol(new_molecule),
                            oldparts(old_molecule),
                            newparts(new_molecule)
{}

/** Construct the change of 'old_molecule' to 'new_molecule', where only the
    CutGroups whose IDs are in 'cgids' are known to change */
CLJFF::ChangedCLJMolecule::ChangedCLJMolecule(const CLJMolecule &old_molecule,
                                              const CLJMolecule &new_molecule,
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
        oldparts = CLJMolecule(oldmol, changed_cgids);
        newparts = CLJMolecule(newmol, changed_cgids);
    }
}

/** Copy constructor */
CLJFF::ChangedCLJMolecule::ChangedCLJMolecule(const CLJFF::ChangedCLJMolecule &other)
                          : oldmol(other.oldmol),
                            newmol(other.newmol),
                            oldparts(other.oldparts),
                            newparts(other.newparts),
                            changed_cgids(other.changed_cgids)
{}

/** Destructor */
CLJFF::ChangedCLJMolecule::~ChangedCLJMolecule()
{}

/** Assignment operator */
CLJFF::ChangedCLJMolecule&
CLJFF::ChangedCLJMolecule::operator=(const CLJFF::ChangedCLJMolecule &other)
{
    oldmol = other.oldmol;
    newmol = other.newmol;
    oldparts = other.oldparts;
    newparts = other.newparts;
    changed_cgids = other.changed_cgids;

    return *this;
}

/** Comparison operator */
bool CLJFF::ChangedCLJMolecule::operator==(const CLJFF::ChangedCLJMolecule &other) const
{
    return oldmol == other.oldmol and
           newmol == other.newmol and
           changed_cgids == other.changed_cgids;
}

/** Comparison operator */
bool CLJFF::ChangedCLJMolecule::operator!=(const CLJFF::ChangedCLJMolecule &other) const
{
    return oldmol != other.oldmol or
           newmol != other.newmol or
           changed_cgids != other.changed_cgids;
}

/** Return whether or not this is empty (both old and new are empty) */
bool CLJFF::ChangedCLJMolecule::isEmpty() const
{
    return oldmol.isEmpty() and newmol.isEmpty();
}

/** Return whether the entire molecule has changed */
bool CLJFF::ChangedCLJMolecule::changedAll() const
{
    //everything has changed if changed_cgids is empty
    return changed_cgids.isEmpty();
}

/** Return the CutGroupIDs of all CutGroups that have changed since the
    last energy evaluation. */
const QSet<CutGroupID>& CLJFF::ChangedCLJMolecule::changedGroups() const
{
    return changed_cgids;
}

/** Return the whole CLJMolecule as it was in the old state */
const CLJFF::CLJMolecule& CLJFF::ChangedCLJMolecule::oldMolecule() const
{
    return oldmol;
}

/** Return the whole CLJMolecule as it is in the new state */
const CLJFF::CLJMolecule& CLJFF::ChangedCLJMolecule::newMolecule() const
{
    return newmol;
}

/** Return the parts of the CLJMolecule in the old state that
    have changed compared to the new state */
const CLJFF::CLJMolecule& CLJFF::ChangedCLJMolecule::oldParts() const
{
    return oldparts;
}

/** Return the parts of the CLJMolecule in the new state that
    have changed compared to the old state */
const CLJFF::CLJMolecule& CLJFF::ChangedCLJMolecule::newParts() const
{
    return newparts;
}

/** Return the ChangedCLJMolecule that represents the change from the old molecule
    to 'molecule' */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::change(const Molecule &molecule) const
{
    //calculate the change from newmol to molecule...
    ChangedCLJMolecule next_change = newmol.change(molecule);

    if (next_change.isEmpty())
        //there was no further change
        return ChangedCLJMolecule();
    else
        //now return the change from oldmol to newmol
        return ChangedCLJMolecule(oldmol, next_change.newMolecule());
}

static QSet<CutGroupID> operator+(const QSet<CutGroupID> &set0, const QSet<CutGroupID> &set1)
{
    QSet<CutGroupID> ret(set0);
    ret.unite(set1);

    return ret;
}

/** Return the ChangedCLJMolecule that represents the change from the old molecule
    to 'residue' */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::change(const Residue &residue) const
{
    if (this->changedAll())
        return this->change(residue.molecule());

    //calculate the change from newmol to 'residue'...
    ChangedCLJMolecule next_change = newmol.change(residue);

    if (next_change.isEmpty())
        return ChangedCLJMolecule();
    else
        return ChangedCLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCLJMolecule that represents the change from the old molecule
    to 'residue' */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::change(const NewAtom &atom) const
{
    if (this->changedAll())
        return this->change(atom.molecule());

    ChangedCLJMolecule next_change = newmol.change(atom);

    if (next_change.isEmpty())
        return ChangedCLJMolecule();
    else
        return ChangedCLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCLJMolecule that represents the addition of all atoms
    in the molecule 'molecule' */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::add(const Molecule &molecule,
                               const QString &chgproperty,
                               const QString &ljproperty) const
{
    ChangedCLJMolecule next_change = newmol.add(molecule,
                                                chgproperty,
                                                ljproperty);

    if (next_change.isEmpty())
        return ChangedCLJMolecule();
    else
        return ChangedCLJMolecule(oldmol, next_change.newMolecule());
}

/** Return the ChangedCLJMolecule that represents the addition of all atoms
    in the residue 'residue' */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::add(const Residue &residue,
                               const QString &chgproperty,
                               const QString &ljproperty) const
{
    ChangedCLJMolecule next_change = newmol.add(residue,
                                                chgproperty,
                                                ljproperty);

    if (next_change.isEmpty())
        return ChangedCLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCLJMolecule that represents the addition of the atom 'atom' */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::add(const NewAtom &atom,
                               const QString &chgproperty,
                               const QString &ljproperty) const
{
    ChangedCLJMolecule next_change = newmol.add(atom,
                                                chgproperty,
                                                ljproperty);

    if (next_change.isEmpty())
        return ChangedCLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCLJMolecule that represents the addition of the selected atoms */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::add(const AtomSelection &selected_atoms,
                               const QString &chgproperty,
                               const QString &ljproperty) const
{
    ChangedCLJMolecule next_change = newmol.add(selected_atoms,
                                                chgproperty,
                                                ljproperty);

    if (next_change.isEmpty())
        return ChangedCLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCLJMolecule that represents the removal of the entire molecule! */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::remove(const Molecule &molecule) const
{
    ChangedCLJMolecule next_change = newmol.remove(molecule);

    if (next_change.isEmpty())
        return ChangedCLJMolecule();
    else
        return ChangedCLJMolecule(oldmol, next_change.newMolecule());
}

/** Return the ChangedCLJMolecule that represents the removal of the residue 'residue' */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::remove(const Residue &residue) const
{
    ChangedCLJMolecule next_change = newmol.remove(residue);

    if (next_change.isEmpty())
        return ChangedCLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCLJMolecule(oldmol, next_change.newMolecule(),
                                  changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCLJMolecule that represents the removal of 'atom' */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::remove(const NewAtom &atom) const
{
    ChangedCLJMolecule next_change = newmol.remove(atom);

    if (next_change.isEmpty())
        return ChangedCLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCLJMolecule(oldmol, next_change.newMolecule(),
                                  changed_cgids + next_change.changed_cgids);
}

/** Return the ChangedCLJMolecule that represents the removal of the
    selected atoms. */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::remove(const AtomSelection &selected_atoms) const
{
    ChangedCLJMolecule next_change = newmol.remove(selected_atoms);

    if (next_change.isEmpty())
        return ChangedCLJMolecule();
    else if (this->changedAll() or next_change.changedAll())
        return ChangedCLJMolecule(oldmol, next_change.newMolecule());
    else
        return ChangedCLJMolecule(oldmol, next_change.newMolecule(),
                                 changed_cgids + next_change.changed_cgids);
}

/////////////
///////////// Implementation of CLJFF
/////////////

/** This function returns the CLJ energy of two groups based on the
    inter-atomic inverse-square-distances stored in 'distmatrix'
    and using the CLJ parameters in 'cljmatrix'. */
CLJFF::CLJEnergy CLJFF::calculatePairEnergy(DistMatrix &distmatrix,
                                            CLJMatrix &cljmatrix)
{
    uint nats0 = distmatrix.nOuter();
    uint nats1 = distmatrix.nInner();

    BOOST_ASSERT( cljmatrix.nOuter() == nats0 );
    BOOST_ASSERT( cljmatrix.nInner() == nats1 );

    double cnrg = 0;
    double ljnrg = 0;

    //loop over all pairs of atoms
    for (uint i=0; i<nats0; ++i)
    {
        distmatrix.setOuterIndex(i);
        cljmatrix.setOuterIndex(i);

        for (uint j=0; j<nats1; ++j)
        {
            //get the distance and LJPair for this atom pair
            double invdist2 = distmatrix[j];
            const CLJPair &cljpair = cljmatrix[j];

            double sig2_over_dist2 = SireMaths::pow_2(cljpair.sigma()) * invdist2;
            double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
            double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

            //LJ energy
            ljnrg += double(4) * cljpair.epsilon() *
                                    ( sig12_over_dist12 - sig6_over_dist6 );

            //Coulomb energy
            cnrg += cljpair.charge2() * std::sqrt(invdist2);
        }
    }

    return CLJEnergy(cnrg,ljnrg);
}

/** This function is used to calculate and return the self-energy of a group,
    using the inverse-square-distances and parameters stored
    in the passed distance and CLJ matricies. */
CLJFF::CLJEnergy CLJFF::calculateSelfEnergy(DistMatrix &distmatrix,
                                            CLJMatrix &cljmatrix)
{
    uint nats = distmatrix.nOuter();

    BOOST_ASSERT( distmatrix.nInner() == nats );
    BOOST_ASSERT( cljmatrix.nInner() == nats );
    BOOST_ASSERT( cljmatrix.nOuter() == nats );

    double cnrg = 0;
    double ljnrg = 0;

    for (uint i=0; i<nats-1; ++i)
    {
        distmatrix.setOuterIndex(i);
        cljmatrix.setOuterIndex(i);

        for (uint j=i+1; j<nats; ++j)
        {
            //get the distance and LJPair for this atom pair
            double invdist2 = distmatrix[j];
            const CLJPair &cljpair = cljmatrix[j];

            double sig2_over_dist2 = SireMaths::pow_2(cljpair.sigma()) * invdist2;
            double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
            double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

            //LJ energy
            ljnrg += double(4) * cljpair.epsilon() *
                                    ( sig12_over_dist12 - sig6_over_dist6 );

            //coulomb energy
            cnrg += cljpair.charge2() * std::sqrt(invdist2);
        }
    }

    return CLJEnergy(cnrg,ljnrg);
}

/** Calculate and return the LJ energy of interaction of group0,
    with partial charges in 'chgs0' and with LJ parameters
    in 'lj0' and group1, with partial charges in 'chgs1' and
    LJ parameters in 'lj1', using the space 'space', and using
    the provided distance and CLJ matricies as temporary workspace. */
CLJFF::CLJEnergy CLJFF::calculateEnergy(const CoordGroup &group0,
                                        const QVector<ChargeParameter> &chg0,
                                        const QVector<LJParameter> &lj0,
                                        const CoordGroup &group1,
                                        const QVector<ChargeParameter> &chg1,
                                        const QVector<LJParameter> &lj1,
                                        const Space &space,
                                        const SwitchingFunction &switchfunc,
                                        DistMatrix &distmatrix,
                                        CLJMatrix &cljmatrix)
{
    if ( not space.beyond(switchfunc.cutoffDistance(), group0, group1) )
    {
        double mindist = space.calcInvDist2(group0, group1, distmatrix);

        double sclcoul = switchfunc.electrostaticScaleFactor(mindist);
        double scllj = switchfunc.vdwScaleFactor(mindist);

        if (sclcoul != 0 or scllj != 0)
        {
            BOOST_ASSERT( chg0.count() == lj0.count() );
            BOOST_ASSERT( chg1.count() == lj1.count() );

            //combine the charge and LJ parameters together
            int nats0 = lj0.count();
            int nats1 = lj1.count();

            BOOST_ASSERT( group0.count() == nats0 );
            BOOST_ASSERT( group1.count() == nats1 );

            cljmatrix.redimension(nats0, nats1);

            const ChargeParameter *chg0_array = chg0.constData();
            const LJParameter *lj0_array = lj0.constData();

            const ChargeParameter *chg1_array = chg1.constData();
            const LJParameter *lj1_array = lj1.constData();

            for (int i=0; i<nats0; ++i)
            {
                cljmatrix.setOuterIndex(i);

                double chg0_param = SireUnits::one_over_four_pi_eps0
                                              * chg0_array[i].charge();

                const LJParameter &lj0param = lj0_array[i];

                for (int j=0; j<nats1; ++j)
                {
                    LJPair combined_lj = LJPair::geometric( lj1_array[j], lj0param );

                    cljmatrix[j] = CLJPair( chg0_param * chg1_array[j].charge(),
                                            combined_lj.sigma(),
                                            combined_lj.epsilon() );
                }
            }

            CLJEnergy nrg = CLJFF::calculatePairEnergy(distmatrix, cljmatrix);

            return CLJEnergy( sclcoul * nrg.coulomb(),
                              scllj * nrg.lj() );
        }
    }

    return CLJEnergy(0,0);
}

/** Calculate the LJ energy of interaction of the atoms within the
    group 'group', with partial charges 'chgs', LJ parameters in 'ljs',
    using the space 'space', and working in the temporary
    workspace provided by the passed distance and CLJ matricies */
CLJFF::CLJEnergy CLJFF::calculateEnergy(const CoordGroup &group,
                                        const QVector<ChargeParameter> &chgs,
                                        const QVector<LJParameter> &ljs,
                                        const Space &space,
                                        DistMatrix &distmatrix,
                                        CLJMatrix &cljmatrix)
{
    space.calcInvDist2(group, distmatrix);

    //combine together the LJ parameters
    int nats = ljs.count();

    BOOST_ASSERT(group.count() == nats);
    BOOST_ASSERT(chgs.count() == nats);

    cljmatrix.redimension(nats,nats);

    const ChargeParameter *chg_array = chgs.constData();
    const LJParameter *lj_array = ljs.constData();

    //we only need to fill in the top left diagonal
    //(and we don't need to do matrix(i,i) as we never
    // calculate a self-interaction!)
    for (int i=0; i<nats-1; ++i)
    {
        cljmatrix.setOuterIndex(i);

        double chg0_param = SireUnits::one_over_four_pi_eps0 * chg_array[i].charge();
        const LJParameter &lj0param = lj_array[i];

        for (int j=i+1; j<nats; ++j)
        {
            LJPair combined_lj = LJPair::geometric( lj_array[j], lj0param );

            cljmatrix[j] = CLJPair( chg0_param * chg_array[j].charge(),
                                    combined_lj.sigma(),
                                    combined_lj.epsilon() );
        }
    }

    return CLJFF::calculateSelfEnergy(distmatrix, cljmatrix);
}

/** Calculate and return the LJ energy of interaction between two
    CLJMolecules, using the space 'space', combining rules 'combrules'
    and switching function 'switchfunc'. The calculation will use
    the provided distance and CLJ matricies as a temporary workspace */
CLJFF::CLJEnergy CLJFF::calculateEnergy(const CLJFF::CLJMolecule &mol0,
                                        const CLJFF::CLJMolecule &mol1,
                                        const Space &space,
                                        const SwitchingFunction &switchfunc,
                                        DistMatrix &distmatrix,
                                        CLJMatrix &cljmatrix)
{
    int ncg0 = mol0.coordinates().count();
    int ncg1 = mol1.coordinates().count();

    if (ncg0 == 1 and ncg1 == 1)
    {
        return calculateEnergy( mol0.coordinates().constData()[0],
                                mol0.charges().constData()[0],
                                mol0.ljParameters().constData()[0],
                                mol1.coordinates().constData()[0],
                                mol1.charges().constData()[0],
                                mol1.ljParameters().constData()[0],

                                space, switchfunc,
                                distmatrix, cljmatrix);
    }
    else if (ncg0 > 0 and ncg1 > 0)
    {
        CLJEnergy cljnrg;

        const CoordGroup *cg0array = mol0.coordinates().constData();
        const CoordGroup *cg1array = mol1.coordinates().constData();

        const QVector<ChargeParameter> *chg0array = mol0.charges().constData();
        const QVector<LJParameter> *lj0array = mol0.ljParameters().constData();

        const QVector<ChargeParameter> *chg1array = mol1.charges().constData();
        const QVector<LJParameter> *lj1array = mol1.ljParameters().constData();

        for (int i=0; i<ncg0; ++i)
        {
            const CoordGroup &group0 = cg0array[i];

            const QVector<ChargeParameter> &chg0 = chg0array[i];
            const QVector<LJParameter> &lj0 = lj0array[i];

            for (int j=0; j<ncg1; ++j)
            {
                const CoordGroup &group1 = cg1array[j];

                const QVector<ChargeParameter> &chg1 = chg1array[j];
                const QVector<LJParameter> &lj1 = lj1array[j];

                cljnrg += calculateEnergy(group0, chg0, lj0,
                                          group1, chg1, lj1,
                                          space, switchfunc,
                                          distmatrix, cljmatrix);
            }
        }

        return cljnrg;
    }
    else
        return CLJEnergy(0,0);
}

/** Calculate and return the LJ energy of interaction between the pairs
    of atoms within a single CLJMolecule, using the space 'space',
    combining rules 'combrules' and switching function
    'switchfunc'. The calculation will use the provided
    distance and LJ matricies as a temporary workspace  */
CLJFF::CLJEnergy CLJFF::calculateEnergy(const CLJFF::CLJMolecule &mol,
                                        const Space &space,
                                        const SwitchingFunction &switchfunc,
                                        DistMatrix &distmatrix,
                                        CLJMatrix &cljmatrix)
{
    int ncg = mol.coordinates().count();

    if (ncg == 1)
    {
        //calculate only the self-energy of the first CutGroup
        return calculateEnergy(mol.coordinates().constData()[0],
                               mol.charges().constData()[0],
                               mol.ljParameters().constData()[0],
                               space, distmatrix, cljmatrix);
    }
    else if (ncg > 1)
    {
        CLJEnergy cljnrg(0,0);

        const CoordGroup *cgarray = mol.coordinates().constData();

        const QVector<ChargeParameter> *chgarray = mol.charges().constData();
        const QVector<LJParameter> *ljarray = mol.ljParameters().constData();

        for (int i=0; i<ncg-1; ++i)
        {
            const CoordGroup &group0 = cgarray[i];

            const QVector<ChargeParameter> &chg0 = chgarray[i];
            const QVector<LJParameter> &lj0 = ljarray[i];

            //add on the self-energy
            cljnrg += calculateEnergy(group0, chg0, lj0, space,
                                      distmatrix, cljmatrix);

            for (int j=i+1; j<ncg; ++j)
            {
                //calculate the group-group energy
                const CoordGroup &group1 = cgarray[j];

                const QVector<ChargeParameter> &chg1 = chgarray[j];
                const QVector<LJParameter> &lj1 = ljarray[j];

                cljnrg += calculateEnergy(group0, chg0, lj0,
                                          group1, chg1, lj1,
                                          space, switchfunc,
                                          distmatrix, cljmatrix);
            }
        }

        //have to add on the self-energy of the last CutGroup of the molecule
        const CoordGroup &group = cgarray[ncg-1];

        const QVector<ChargeParameter> &chg = chgarray[ncg-1];
        const QVector<LJParameter> &lj = ljarray[ncg-1];

        cljnrg += calculateEnergy(group, chg, lj, space, distmatrix, cljmatrix);

        return cljnrg;
    }
    else
        return CLJEnergy(0,0);
}

static const RegisterMetaType<CLJFF> r_cljff(MAGIC_ONLY, "SireMM::CLJFF");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJFF &cljff)
{
    writeHeader(ds, r_cljff, 1);

    SharedDataStream sds(ds);

    sds << cljff.spce << cljff.switchfunc
        << static_cast<const FFBase&>(cljff);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJFF &cljff)
{
    VersionID v = readHeader(ds, r_cljff);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> cljff.spce >> cljff.switchfunc
            >> static_cast<FFBase&>(cljff);
    }
    else
        throw version_error(v, "1", r_cljff, CODELOC);

    return ds;
}

/** Null constructor */
CLJFF::CLJFF() : FFBase()
{
    this->registerComponents();
}

/** Construct a CLJFF using the specified space and switching function */
CLJFF::CLJFF(const Space &space, const SwitchingFunction &switchingfunction)
     : FFBase(),
       spce(space), switchfunc(switchingfunction)
{
    this->registerComponents();
}

/** Copy constructor */
CLJFF::CLJFF(const CLJFF &other)
     : FFBase(other),
       spce(other.spce), switchfunc(other.switchfunc)
{
    //get the pointer from the base class...
    components_ptr = dynamic_cast<const CLJFF::Components*>( &(FFBase::components()) );
    BOOST_ASSERT( components_ptr != 0 );
}

/** Copy assignment function used by derived classes */
CLJFF& CLJFF::operator=(const CLJFF &other)
{
    FFBase::copy(other);

    spce = other.spce;
    switchfunc = other.switchfunc;

    components_ptr = dynamic_cast<const CLJFF::Components*>( &(FFBase::components()) );
    BOOST_ASSERT( components_ptr != 0 );

    return *this;
}

/** Destructor */
CLJFF::~CLJFF()
{}

/** Set the property 'name' to the value 'value'. This
    returns whether or not this changes the forcefield,
    and therefore the energy of the forcefield will need
    to be recalculated

    Note that you can only set pre-defined properties
    of forcefields - an exception will be thrown if
    you try to set the value of a property that does
    not exist in this forcefield.

    \throw SireMol::missing_property
*/
bool CLJFF::setProperty(const QString &name, const Property &property)
{
    if ( name == QLatin1String("space") )
    {
        this->setSpace(property);
        return this->isDirty();
    }
    else if ( name == QLatin1String("switching function") )
    {
        this->setSwitchingFunction(property);
        return this->isDirty();
    }
    else
        return FFBase::setProperty(name, property);
}

/** Return the property associated with the name 'name'

    \throw SireMol::missing_property
*/
Property CLJFF::getProperty(const QString &name) const
{
    if ( name == QLatin1String("space") )
    {
        return this->space();
    }
    else if ( name == QLatin1String("switching function") )
    {
        return this->switchingFunction();
    }
    else
        return FFBase::getProperty(name);
}

/** Return whether or not this contains a property with the name 'name' */
bool CLJFF::containsProperty(const QString &name) const
{
    return ( name == QLatin1String("space") ) or
           ( name == QLatin1String("switching function") ) or
           FFBase::containsProperty(name);
}

/** Register the energy components associated with this forcefield */
void CLJFF::registerComponents()
{
    std::auto_ptr<CLJFF::Components> ptr( new CLJFF::Components(*this) );

    FFBase::registerComponents(ptr.get());

    components_ptr = ptr.release();
}
