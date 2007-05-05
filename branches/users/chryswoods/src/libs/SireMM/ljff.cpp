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

#include "SireMol/cgatomid.h"
#include "SireMol/moleculeversion.h"
#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/propertyextractor.h"
#include "SireMol/atomselector.h"

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

    LJMoleculeData(const PartialMolecule &mol, const QString &ljproperty);

    LJMoleculeData(const LJMoleculeData &other);

    ~LJMoleculeData();

    LJMoleculeData& operator=(const LJMoleculeData &other);

    bool operator==(const LJMoleculeData &other) const;
    bool operator!=(const LJMoleculeData &other) const;

    void rebuildAll();
    void rebuildCoordinates();

    /** The actual molecule */
    PartialMolecule molecule;

    /** The name of the property associated with the LJ parameters */
    QString lj_property;

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
LJFF::LJMoleculeData::LJMoleculeData(const PartialMolecule &mol,
                                     const QString &ljproperty)
                     : QSharedData(),
                       molecule(mol),
                       lj_property(ljproperty)
{
    this->rebuildAll();
}

/** Copy constructor */
LJFF::LJMoleculeData::LJMoleculeData(const LJFF::LJMoleculeData &other)
                     : QSharedData(),
                       molecule(other.molecule),
                       lj_property(other.lj_property),
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
        coords = other.coords;
        ljs = other.ljs;
    }

    return *this;
}

/** Comparison operator */
bool LJFF::LJMoleculeData::operator==(const LJFF::LJMoleculeData &other) const
{
    return molecule == other.molecule and
           lj_property == other.lj_property;
}

/** Comparison operator */
bool LJFF::LJMoleculeData::operator!=(const LJFF::LJMoleculeData &other) const
{
    return molecule != other.molecule or
           lj_property != other.lj_property;
}

/** Rebuild all of the coordinate and LJ data from scratch

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
void LJFF::LJMoleculeData::rebuildAll()
{
    ljs = molecule.extract().property(lj_property);
    coords = molecule.extract().coordGroups();
}

/** Rebuild all of the coordinate data from scratch */
void LJFF::LJMoleculeData::rebuildCoordinates()
{
    coords = molecule.extract().coordGroups();
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
        << ljmoldata.lj_property;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, LJFF::LJMoleculeData &ljmoldata)
{
    VersionID v = readHeader(ds, r_ljmoldata);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        PartialMolecule mol;
        QString lj_property;

        sds >> mol >> lj_property;

        ljmoldata = LJFF::LJMoleculeData(mol, lj_property);
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

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
LJFF::LJMolecule::LJMolecule(const PartialMolecule &molecule,
                             const QString &ljproperty)
                 : d( new LJFF::LJMoleculeData(molecule,ljproperty) )
{}

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
    return d->molecule.selectedAtoms().isEmpty();
}

/** Return the actual molecule */
const PartialMolecule& LJFF::LJMolecule::molecule() const
{
    return d->molecule;
}

/** Return a ChangedLJMolecule that represents the change from the
    LJMolecule in its current state to 'molecule'

    \throw SireError::incompatible_error
*/
LJFF::LJMolecule LJFF::LJMolecule::change(const PartialMolecule &molecule,
                                          const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        //this is a null Molecule
        return LJMolecule();

    //assert that this is the same molecule
    d->molecule.assertSameMolecule(molecule);

    if ( ljproperty.isNull() or ljproperty == d->lj_property )
    {
        if ( d->molecule.version() == molecule.version() )
            //there has been no change
            return *this;
        else
        {
            LJMolecule newmol(*this);

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
        LJMolecule newmol(*this);

        PartialMolecule &mol = newmol.d->molecule;
        mol = mol.change(molecule);

        newmol.d->lj_property = ljproperty;

        newmol.d->rebuildAll();

        return newmol;
    }
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to include the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
LJFF::LJMolecule LJFF::LJMolecule::add(const PartialMolecule &mol,
                                       const QString &ljproperty) const
{
    if (d->molecule.ID() == 0)
        return LJMolecule(mol, ljproperty);

    else if (molecule().selectedAtoms().contains(mol.selectedAtoms()))
        //there will be no change in the atom selections
        return this->change(mol, ljproperty);
    else
    {
        PartialMolecule mol = molecule().change(mol)
                                  .selection().add(mol.selectedAtoms());

        QString lj_property = ljproperty;

        if (ljproperty.isNull())
            lj_property = ljProperty();

        return LJMolecule(mol, lj_property);
    }
}

/** Return a ChangedLJMolecule that represents the change from the LJMolecule
    in its current state to remove the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
LJFF::LJMolecule LJFF::LJMolecule::remove(const PartialMolecule &mol) const
{
    if (d->molecule.ID() == 0)
    {
        return LJMolecule();
    }
    else if (molecule().selectedAtoms().intersects(mol.selectedAtoms()))
    {
        LJMolecule newmol(*this);

        PartialMolecule &mol = newmol.d->molecule;
        mol = mol.selection().remove(mol.selectedAtoms());

        newmol.d->rebuildAll();

        return newmol;
    }
    else
        //there has been no change
        return *this;
}

/** Return a LJMolecule that contains the CutGroups of 'oldmol'
    that appear to have changed in 'newmol' */
LJFF::LJMolecule LJFF::LJMolecule::getDifferences(const LJFF::LJMolecule &newmol) const
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
            if (old_ljs_array != new_ljs_array)
            {
                //potentially the coordinates or LJ parameters
                //may have changed

                for (CutGroupID i(0); i<ngroups; ++i)
                {
                    if (old_coords_array[i] != new_coords_array[i] or
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
        else if (old_ljs_array != new_ljs_array)
        {
            //only the charges or LJ parameters may have changed
            for (CutGroupID i(0); i<ngroups; ++i)
            {
                if (old_ljs_array[i] != new_ljs_array[i])
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
        return LJMolecule();
    else
        //cgids_that_change contains the IDs of all of the CutGroups
        //that change - create a CLJMolecule to represent this change
        return LJMolecule( old_molecule.selection().applyMask(cgids_that_change),
                           ljProperty() );
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
    return d->molecule.selectedAtoms().selectedAll();
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
        << changedmol.oldparts << changedmol.newparts;

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
            >> changedmol.oldparts >> changedmol.newparts;
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
{}

/** Construct the change of all of 'old_molecule' to all of 'new_molecule' */
LJFF::ChangedLJMolecule::ChangedLJMolecule(const LJMolecule &old_molecule,
                                           const LJMolecule &new_molecule)
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
LJFF::ChangedLJMolecule::ChangedLJMolecule(const LJFF::ChangedLJMolecule &other)
                        : oldmol(other.oldmol),
                          newmol(other.newmol),
                          oldparts(other.oldparts),
                          newparts(other.newparts)
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

    return *this;
}

/** Comparison operator */
bool LJFF::ChangedLJMolecule::operator==(const LJFF::ChangedLJMolecule &other) const
{
    return oldmol == other.oldmol and
           newmol == other.newmol and
           oldparts == other.oldparts and
           newparts == other.newparts;
}

/** Comparison operator */
bool LJFF::ChangedLJMolecule::operator!=(const LJFF::ChangedLJMolecule &other) const
{
    return oldmol != other.oldmol or
           newmol != other.newmol or
           oldparts != other.oldparts or
           newparts != other.newparts;
}

/** Return whether or not this is empty (both old and new are empty) */
bool LJFF::ChangedLJMolecule::isEmpty() const
{
    return oldmol.isEmpty() and newmol.isEmpty();
}

/** Return whether nothing has changed - this will be when oldmol == newmol */
bool LJFF::ChangedLJMolecule::nothingChanged() const
{
    return oldmol == newmol;
}

/** Return whether the entire molecule has changed */
bool LJFF::ChangedLJMolecule::changedAll() const
{
    return oldparts == oldmol or newparts == newmol;
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
LJFF::ChangedLJMolecule
LJFF::ChangedLJMolecule::change(const PartialMolecule &molecule,
                                const QString &ljproperty) const
{
    return ChangedLJMolecule( oldmol, newmol.change(molecule, ljproperty) );
}

/** Return the ChangedLJMolecule that represents the addition of all atoms
    in the molecule 'molecule' */
LJFF::ChangedLJMolecule
LJFF::ChangedLJMolecule::add(const PartialMolecule &molecule,
                             const QString &ljproperty) const
{
    return ChangedLJMolecule( oldmol, newmol.add(molecule, ljproperty) );
}

/** Return the ChangedLJMolecule that represents the removal the molecule! */
LJFF::ChangedLJMolecule
LJFF::ChangedLJMolecule::remove(const PartialMolecule &molecule) const
{
    return ChangedLJMolecule( oldmol, newmol.remove(molecule) );
}

/////////////
///////////// Implementation of LJFF
/////////////

/** This function returns the LJ energy of two groups based on the
    inter-atomic inverse-square-distances stored in 'distmatrix'
    and using the LJ parameters 'ljmatrix'. */
double LJFF::calculatePairEnergy(DistMatrix &distmatrix,
                                 LJPairMatrix &ljmatrix)
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
                                 LJPairMatrix &ljmatrix)
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
                             LJPairMatrix &ljmatrix)
{
    if ( not space.beyond(switchfunc.cutoffDistance(), group0, group1) )
    {
        double mindist = space.calcInvDist2(group0, group1, distmatrix);

        double scllj = switchfunc.vdwScaleFactor(mindist);

        if (scllj != 0)
        {
            //combine the LJ parameters together
            uint nats0 = lj0.count();
            uint nats1 = lj1.count();

            BOOST_ASSERT( group0.count() == nats0 );
            BOOST_ASSERT( group1.count() == nats1 );

            ljmatrix.redimension(nats0, nats1);

            const LJParameter *lj0_array = lj0.constData();
            const LJParameter *lj1_array = lj1.constData();

            for (uint i=0; i<nats0; ++i)
            {
                ljmatrix.setOuterIndex(i);

                const LJParameter &lj0param = lj0_array[i];

                for (uint j=0; j<nats1; ++j)
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
                             LJPairMatrix &ljmatrix)
{
    space.calcInvDist2(group, distmatrix);

    //combine together the LJ parameters
    uint nats = ljs.count();

    BOOST_ASSERT(group.count() == nats);

    ljmatrix.redimension(nats,nats);

    const LJParameter *lj_array = ljs.constData();

    //we only need to fill in the top left diagonal
    //(and we don't need to do matrix(i,i) as we never
    // calculate a self-interaction!)
    for (uint i=0; i<nats-1; ++i)
    {
        ljmatrix.setOuterIndex(i);

        const LJParameter &lj0param = lj_array[i];

        for (uint j=i+1; j<nats; ++j)
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
                             LJPairMatrix &ljmatrix)
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
                             LJPairMatrix &ljmatrix)
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

/** Copy assignment function used by derived classes */
void LJFF::_pvt_copy(const FFBase &ffbase)
{
    const LJFF &ljff = dynamic_cast<const LJFF&>(ffbase);

    spce = ljff.spce;
    switchfunc = ljff.switchfunc;

    components_ptr =
          dynamic_cast<const LJFF::Components*>( &(FFBase::components()) );

    BOOST_ASSERT( components_ptr != 0 );
}

/** Set the space within which the molecules interact */
bool LJFF::setSpace(const Space &space)
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
bool LJFF::setSwitchingFunction(const SwitchingFunction &sfunc)
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
bool LJFF::setProperty(const QString &name, const Property &property)
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

    \throw SireBase::missing_property
*/
Property LJFF::getProperty(const QString &name) const
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
bool LJFF::containsProperty(const QString &name) const
{
    return ( name == QLatin1String("space") ) or
           ( name == QLatin1String("switching function") ) or
           FFBase::containsProperty(name);
}

/** Return all of the properties of this forcefield, indexed by name */
QHash<QString,Property> LJFF::properties() const
{
    QHash<QString,Property> props;

    props.insert( QLatin1String("space"), this->space() );
    props.insert( QLatin1String("switching function"), this->switchingFunction() );

    props.unite( FFBase::properties() );

    return props;
}

/** Register the energy components associated with this forcefield */
void LJFF::registerComponents()
{
    std::auto_ptr<LJFF::Components> ptr( new LJFF::Components(*this) );

    FFBase::registerComponents(ptr.get());

    components_ptr = ptr.release();
}
