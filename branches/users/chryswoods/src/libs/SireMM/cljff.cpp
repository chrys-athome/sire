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

#include "SireMol/partialmolecule.h"
#include "SireMol/atomselector.h"
#include "SireMol/propertyextractor.h"
#include "SireMol/molecule.h"
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

    CLJMoleculeData(const PartialMolecule &molecule,
                    const QString &chgproperty, const QString &ljproperty);

    CLJMoleculeData(const CLJMoleculeData &other);

    ~CLJMoleculeData();

    CLJMoleculeData& operator=(const CLJMoleculeData &other);

    bool operator==(const CLJMoleculeData &other) const;
    bool operator!=(const CLJMoleculeData &other) const;

    void rebuildAll();
    void rebuildCoordinates();

    /** The actual molecule */
    PartialMolecule molecule;

    /** The name of the property associated with the atomic partial charges */
    QString chg_property;

    /** The name of the property associated with the LJ parameters */
    QString lj_property;

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
CLJFF::CLJMoleculeData::CLJMoleculeData(const PartialMolecule &mol,
                                        const QString &chgproperty,
                                        const QString &ljproperty)
                     : QSharedData(),
                       molecule(mol),
                       chg_property(chgproperty),
                       lj_property(ljproperty)
{
    this->rebuildAll();
}

/** Copy constructor */
CLJFF::CLJMoleculeData::CLJMoleculeData(const CLJFF::CLJMoleculeData &other)
                       : QSharedData(),
                         molecule(other.molecule),
                         chg_property(other.chg_property),
                         lj_property(other.lj_property),
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
           lj_property == other.lj_property;
}

/** Comparison operator */
bool CLJFF::CLJMoleculeData::operator!=(const CLJFF::CLJMoleculeData &other) const
{
    return molecule != other.molecule or
           chg_property != other.chg_property or
           lj_property != other.lj_property;
}

/** Rebuild all of the coordinate and LJ data from scratch

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
void CLJFF::CLJMoleculeData::rebuildAll()
{
    chgs = molecule.extract().property(chg_property);
    ljs = molecule.extract().property(lj_property);
    coords = molecule.extract().coordGroups();
}

/** Rebuild all of the coordinate data from scratch */
void CLJFF::CLJMoleculeData::rebuildCoordinates()
{
    coords = molecule.extract().coordGroups();
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
        << cljmoldata.lj_property;

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

        PartialMolecule molecule;
        QString chg_property;
        QString lj_property;

        sds >> molecule >> chg_property >> lj_property;

        cljmoldata = CLJFF::CLJMoleculeData(molecule,
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

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CLJFF::CLJMolecule::CLJMolecule(const PartialMolecule &molecule,
                                const QString &chgproperty,
                                const QString &ljproperty)
                   : d( new CLJFF::CLJMoleculeData(molecule,
                                                   chgproperty,
                                                   ljproperty) )
{}

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
    return d->molecule.selectedAtoms().isEmpty();
}

/** Return the actual molecule */
const PartialMolecule& CLJFF::CLJMolecule::molecule() const
{
    return d->molecule;
}

/** Return a CLJMolecule that represents this molecule changed
    to 'molecule'. Note that the new state must have the same
    MoleculeID as the current state!

    \throw SireError::incompatible_error
*/
CLJFF::CLJMolecule CLJFF::CLJMolecule::change(const PartialMolecule &molecule,
                                              const QString &chgproperty,
                                              const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        //this is a null Molecule
        return CLJMolecule();

    //assert that this is the same molecule
    d->molecule.assertSameMolecule(molecule);

    if ( (chgproperty.isNull() or chgproperty == d->chg_property) and
         (ljproperty.isNull() or ljproperty == d->lj_property) )
    {
        if ( d->molecule.version() == molecule.version() )
            //there has been no change
            return *this;
        else
        {
            CLJMolecule newmol(*this);

            PartialMolecule &mol = newmol.d->molecule;
            mol = mol.change(molecule);

            if ( d->molecule.version().major() != molecule.version().major() )
                newmol.d->rebuildAll();
            else
                newmol.d->rebuildCoordinates();

            return newmol;
        }
    }
    else
    {
        //there has been a change of property - the entire molecule
        //needs to be rebuilt
        CLJMolecule newmol(*this);

        PartialMolecule &mol = newmol.d->molecule;
        mol = mol.change(molecule);

        newmol.d->chg_property = chgproperty;
        newmol.d->lj_property = ljproperty;

        newmol.d->rebuildAll();

        return newmol;
    }
}

/** Return a CLJMolecule that represents the change from the CLJMolecule
    in its current state to include the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CLJFF::CLJMolecule CLJFF::CLJMolecule::add(const PartialMolecule &mol,
                                           const QString &chgproperty,
                                           const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        return CLJMolecule(mol, chgproperty, ljproperty);

    else if (molecule().selectedAtoms().contains(mol.selectedAtoms()))
        //there will be no change in the atom selections
        return this->change(mol, chgproperty, ljproperty);
    else
    {
        PartialMolecule mol = molecule().change(mol)
                                  .selection().add(mol.selectedAtoms());

        QString chg_property = chgproperty;
        QString lj_property = ljproperty;

        if (chgproperty.isNull())
            chg_property = chargeProperty();

        if (ljproperty.isNull())
            lj_property = ljProperty();

        return CLJMolecule(mol, chg_property, lj_property);
    }
}

/** Return a CLJMolecule that represents the change from the CLJMolecule
    in its current state to remove the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CLJFF::CLJMolecule CLJFF::CLJMolecule::remove(const PartialMolecule &mol) const
{
    if (d->molecule.ID() == 0)
    {
        return CLJMolecule();
    }
    else if (molecule().selectedAtoms().intersects(mol.selectedAtoms()))
    {
        CLJMolecule newmol(*this);

        PartialMolecule &mol = newmol.d->molecule;
        mol = mol.selection().remove(mol.selectedAtoms());

        newmol.d->rebuildAll();

        return newmol;
    }
    else
        //there has been no change
        return *this;
}

/** Return a CLJMolecule that contains the CutGroups of 'oldmol'
    that appear to have changed in 'newmol' */
CLJFF::CLJMolecule
CLJFF::CLJMolecule::getDifferences(const CLJFF::CLJMolecule &newmol) const
{
    const PartialMolecule &old_molecule = this->molecule();
    const PartialMolecule &new_molecule = newmol.molecule();

    old_molecule.assertSameMolecule(new_molecule);

    uint ngroups = old_molecule.selectedAtoms().nSelectedCutGroups();
    int ngroups_minus_one = ngroups - 1;

    if (ngroups == 0)
        //the molecule is empty - all of none of it has changed :-)
        return *this;

    QSet<CutGroupID> cgids_that_change;

    const CoordGroup* old_coords_array = this->coordinates().constData();
    const CoordGroup* new_coords_array = newmol.coordinates().constData();

    const QVector<ChargeParameter>* old_chgs_array = this->charges().constData();
    const QVector<ChargeParameter>* new_chgs_array = newmol.charges().constData();

    const QVector<LJParameter>* old_ljs_array = this->ljParameters().constData();
    const QVector<LJParameter>* new_ljs_array = newmol.ljParameters().constData();

    if (old_molecule.selectedAtoms().selectedAllCutGroups() and
        new_molecule.selectedAtoms().selectedAllCutGroups())
    {
        //easy comparison - each CutGroup is represented and
        //they are in the same order in each state of the molecule

        //see if the coordinates have changed
        if (old_coords_array != new_coords_array)
        {
            if (old_chgs_array != new_chgs_array or
                old_ljs_array != new_ljs_array)
            {
                //potentially the coordinates, charges or LJ parameters
                //may have changed

                for (CutGroupID i(0); i<ngroups; ++i)
                {
                    if (old_coords_array[i] != new_coords_array[i] or
                        old_chgs_array[i] != new_chgs_array[i] or
                        old_ljs_array[i] != new_ljs_array[i])
                    {
                        if (cgids_that_change.count() == ngroups_minus_one)
                            //all of the CutGroups have changed - therefore all
                            //of the molecule has changed
                            return *this;

                        cgids_that_change.insert(i);
                    }
                }
            }
            else
            {
                //only the coordinates may have changed
                for (CutGroupID i(0); i<ngroups; ++i)
                {
                    if (old_coords_array[i] != new_coords_array[i])
                    {
                        if (cgids_that_change.count() == ngroups_minus_one)
                            //all of the CutGroups have changed - therefore all
                            //of the molecule has changed
                            return *this;

                        cgids_that_change.insert(i);
                    }
                }
            }
        }
        else if (old_chgs_array != new_chgs_array or
                 old_ljs_array != new_ljs_array)
        {
            //only the charges or LJ parameters may have changed
            for (CutGroupID i(0); i<ngroups; ++i)
            {
                if (old_chgs_array[i] != new_chgs_array[i] or
                    old_ljs_array[i] != new_ljs_array[i])
                {
                    if (cgids_that_change.count() == ngroups_minus_one)
                        //all of the CutGroups have now changed
                        return *this;

                    cgids_that_change.insert(i);
                }
            }
        }
    }
    else if (old_molecule.selectedAtoms().selectedAllCutGroups())
    {
        //next most easy - all CutGroups in this molecule have been selected

        //get the index of the coordgroups in newmol
        QHash<CutGroupID,quint32> new_index = new_molecule.extract().cutGroupIndex();

        for (CutGroupID i(0); i<ngroups; ++i)
        {
            QHash<CutGroupID,quint32>::const_iterator it = new_index.constFind(i);

            if (it == new_index.constEnd())
            {
                //this CutGroup is missing in newmol - add it to the list of
                //things that have changed
                if (cgids_that_change.count() == ngroups_minus_one)
                    //all groups have now changed, so the entire molecule
                    //has changed
                    return *this;

                cgids_that_change.insert(i);
            }
            else
            {
                quint32 newidx = *it;

                if (old_coords_array[i] != new_coords_array[newidx] or
                    old_chgs_array[i] != new_chgs_array[newidx] or
                    old_ljs_array[i] != new_ljs_array[newidx])
                {
                    if (cgids_that_change.count() == ngroups_minus_one)
                        //all of the groups have changed, so the entire
                        //molecule has changed
                        return *this;

                    cgids_that_change.insert(i);
                }
            }
        }
    }
    else
    {
        //all CutGroups are not selected - get the index that maps the
        //ID of the CutGroup to the location in the coords, chgs and ljs arrays
        QHash<CutGroupID,quint32> old_index = old_molecule.extract().cutGroupIndex();
        QHash<CutGroupID,quint32> new_index = new_molecule.extract().cutGroupIndex();

        for (QHash<CutGroupID,quint32>::const_iterator it = old_index.constBegin();
             it != old_index.constEnd();
             ++it)
        {
            if (not new_index.contains(it.key()))
            {
                //this CutGroup is missing in newmol - add it to the list of
                //things that have changed
                if (cgids_that_change.count() == ngroups_minus_one)
                    //all groups have now changed, so the entire molecule
                    //has changed
                    return *this;

                cgids_that_change.insert(it.key());
            }
            else
            {
                quint32 oldidx = *it;
                quint32 newidx = new_index.value(it.key());

                if (old_coords_array[oldidx] != new_coords_array[newidx] or
                    old_chgs_array[oldidx] != new_chgs_array[newidx] or
                    old_ljs_array[oldidx] != new_ljs_array[newidx])
                {
                    if (cgids_that_change.count() == ngroups_minus_one)
                        //all of the groups have changed, so the entire
                        //molecule has changed
                        return *this;

                    cgids_that_change.insert(it.key());
                }
            }
        }
    }

    if (cgids_that_change.isEmpty())
        //none of oldmol has changed!
        return CLJMolecule();
    else
        //cgids_that_change contains the IDs of all of the CutGroups
        //that change - create a CLJMolecule to represent this change
        return CLJMolecule( old_molecule.selection().applyMask(cgids_that_change),
                            chargeProperty(), ljProperty() );
}

/** Return the name of the property used to get the partial charges */
const QString& CLJFF::CLJMolecule::chargeProperty() const
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
    return d->molecule.selectedAtoms().selectedAll();
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
        << changedmol.oldparts << changedmol.newparts;

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
            >> changedmol.oldparts >> changedmol.newparts;
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
{}

/** Construct the change of all of 'old_molecule' to all of 'new_molecule' */
CLJFF::ChangedCLJMolecule::ChangedCLJMolecule(const CLJMolecule &old_molecule,
                                              const CLJMolecule &new_molecule)
                          : oldmol(old_molecule),
                            newmol(new_molecule)
{
    //work out the different CutGroups between the two states
    if (oldmol.isEmpty() or newmol.isEmpty())
    {
        oldparts = oldmol;
        newparts = newmol;
    }
    else
    {
        oldparts = oldmol.getDifferences(newmol);
        newparts = newmol.getDifferences(oldmol);
    }
}

/** Copy constructor */
CLJFF::ChangedCLJMolecule::ChangedCLJMolecule(const CLJFF::ChangedCLJMolecule &other)
                          : oldmol(other.oldmol),
                            newmol(other.newmol),
                            oldparts(other.oldparts),
                            newparts(other.newparts)
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

    return *this;
}

/** Comparison operator */
bool CLJFF::ChangedCLJMolecule::operator==(const CLJFF::ChangedCLJMolecule &other) const
{
    return oldmol == other.oldmol and
           newmol == other.newmol and
           newparts == other.newparts and
           oldparts == other.oldparts;
}

/** Comparison operator */
bool CLJFF::ChangedCLJMolecule::operator!=(const CLJFF::ChangedCLJMolecule &other) const
{
    return oldmol != other.oldmol or
           newmol != other.newmol or
           newparts != other.newparts or
           oldparts != other.oldparts;
}

/** Return whether or not this is empty (both old and new are empty) */
bool CLJFF::ChangedCLJMolecule::isEmpty() const
{
    return oldmol.isEmpty() and newmol.isEmpty();
}

/** Return whether nothing has changed - this will be when oldmol == newmol */
bool CLJFF::ChangedCLJMolecule::nothingChanged() const
{
    return (oldmol.isEmpty() and newmol.isEmpty()) or oldmol == newmol;
}

/** Return whether the entire molecule has changed */
bool CLJFF::ChangedCLJMolecule::changedAll() const
{
    return newparts == newmol or oldparts == oldmol;
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
CLJFF::ChangedCLJMolecule::change(const PartialMolecule &molecule,
                                  const QString &chgproperty,
                                  const QString &ljproperty) const
{
    return ChangedCLJMolecule( oldmol, newmol.change(molecule,
                                                     chgproperty,
                                                     ljproperty) );
}

/** Return the ChangedCLJMolecule that represents the addition of all atoms
    in the molecule 'molecule' */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::add(const PartialMolecule &molecule,
                               const QString &chgproperty,
                               const QString &ljproperty) const
{
    return ChangedCLJMolecule( oldmol, newmol.add(molecule,
                                                  chgproperty,
                                                  ljproperty) );
}

/** Return the ChangedCLJMolecule that represents the removal of the entire molecule! */
CLJFF::ChangedCLJMolecule
CLJFF::ChangedCLJMolecule::remove(const PartialMolecule &molecule) const
{
    return ChangedCLJMolecule( oldmol, newmol.remove(molecule) );
}

/////////////
///////////// Implementation of CLJFF
/////////////

/** This function returns the CLJ energy of two groups based on the
    inter-atomic inverse-square-distances stored in 'distmatrix'
    and using the CLJ parameters in 'cljmatrix'. */
CLJFF::CLJEnergy CLJFF::calculatePairEnergy(DistMatrix &distmatrix,
                                            CLJPairMatrix &cljmatrix)
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
                                            CLJPairMatrix &cljmatrix)
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
                                        CLJPairMatrix &cljmatrix)
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
            uint nats0 = lj0.count();
            uint nats1 = lj1.count();

            BOOST_ASSERT( group0.count() == nats0 );
            BOOST_ASSERT( group1.count() == nats1 );

            cljmatrix.redimension(nats0, nats1);

            const ChargeParameter *chg0_array = chg0.constData();
            const LJParameter *lj0_array = lj0.constData();

            const ChargeParameter *chg1_array = chg1.constData();
            const LJParameter *lj1_array = lj1.constData();

            for (uint i=0; i<nats0; ++i)
            {
                cljmatrix.setOuterIndex(i);

                double chg0_param = SireUnits::one_over_four_pi_eps0
                                              * chg0_array[i].charge();

                const LJParameter &lj0param = lj0_array[i];

                for (uint j=0; j<nats1; ++j)
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
                                        CLJPairMatrix &cljmatrix)
{
    space.calcInvDist2(group, distmatrix);

    //combine together the LJ parameters
    uint nats = ljs.count();

    BOOST_ASSERT(group.count() == nats);
    BOOST_ASSERT(chgs.count() == int(nats));

    cljmatrix.redimension(nats,nats);

    const ChargeParameter *chg_array = chgs.constData();
    const LJParameter *lj_array = ljs.constData();

    //we only need to fill in the top left diagonal
    //(and we don't need to do matrix(i,i) as we never
    // calculate a self-interaction!)
    for (uint i=0; i<nats-1; ++i)
    {
        cljmatrix.setOuterIndex(i);

        double chg0_param = SireUnits::one_over_four_pi_eps0 * chg_array[i].charge();
        const LJParameter &lj0param = lj_array[i];

        for (uint j=i+1; j<nats; ++j)
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
                                        CLJPairMatrix &cljmatrix)
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
                                        CLJPairMatrix &cljmatrix)
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

/** Destructor */
CLJFF::~CLJFF()
{}

/** Copy assignment function used by derived classes */
void CLJFF::_pvt_copy(const FFBase &ffbase)
{
    const CLJFF &cljff = dynamic_cast<const CLJFF&>(ffbase);

    spce = cljff.spce;
    switchfunc = cljff.switchfunc;

    components_ptr = dynamic_cast<const CLJFF::Components*>( &(FFBase::components()) );
    BOOST_ASSERT( components_ptr != 0 );
}

/** Set the space within which the molecules interact */
bool CLJFF::setSpace(const Space &space)
{
    if (space != spce)
    {
        spce = space;
        this->incrementMajorVersion();
        this->mustNowRecalculateFromScratch();
    }

    return isDirty();
}

/** Set the switching function used to apply the non-bonded cutoff */
bool CLJFF::setSwitchingFunction(const SwitchingFunction &sfunc)
{
    if (sfunc != switchfunc)
    {
        switchfunc = sfunc;
        this->incrementMajorVersion();
        this->mustNowRecalculateFromScratch();
    }

    return isDirty();
}

/** Set the property 'name' to the value 'value'. This
    returns whether or not this changes the forcefield,
    and therefore the energy of the forcefield will need
    to be recalculated

    Note that you can only set pre-defined properties
    of forcefields - an exception will be thrown if
    you try to set the value of a property that does
    not exist in this forcefield.

    \throw SireBase::missing_property
*/
bool CLJFF::setProperty(const QString &name, const Property &value)
{
    if ( name == QLatin1String("space") )
    {
        this->setSpace(value);
        return this->isDirty();
    }
    else if ( name == QLatin1String("switching function") )
    {
        this->setSwitchingFunction(value);
        return this->isDirty();
    }
    else
        return FFBase::setProperty(name, value);
}

/** Return the property associated with the name 'name'

    \throw SireBase::missing_property
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

/** Return all of the properties of this forcefield, indexed by name */
QHash<QString,Property> CLJFF::properties() const
{
    QHash<QString,Property> props;

    props.insert( QLatin1String("space"), this->space() );
    props.insert( QLatin1String("switching function"), this->switchingFunction() );

    props.unite( FFBase::properties() );

    return props;
}

/** Register the energy components associated with this forcefield */
void CLJFF::registerComponents()
{
    std::auto_ptr<CLJFF::Components> ptr( new CLJFF::Components(*this) );

    FFBase::registerComponents(ptr.get());

    components_ptr = ptr.release();
}
