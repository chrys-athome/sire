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

#include "SireMol/cgatomid.h"
#include "SireMol/moleculeversion.h"
#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/propertyextractor.h"
#include "SireMol/atomselector.h"

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

    CoulombMoleculeData(const PartialMolecule &mol, const QString &chgproperty);

    CoulombMoleculeData(const CoulombMoleculeData &other);

    ~CoulombMoleculeData();

    CoulombMoleculeData& operator=(const CoulombMoleculeData &other);

    bool operator==(const CoulombMoleculeData &other) const;
    bool operator!=(const CoulombMoleculeData &other) const;

    void rebuildAll();
    void rebuildCoordinates();

    /** The actual molecule */
    PartialMolecule molecule;

    /** The name of the property associated with the charge parameters */
    QString chg_property;

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
CoulombFF::CoulombMoleculeData::CoulombMoleculeData(const PartialMolecule &mol,
                                                    const QString &chgproperty)
                     : QSharedData(),
                       molecule(mol),
                       chg_property(chgproperty)
{
    this->rebuildAll();
}

/** Copy constructor */
CoulombFF::CoulombMoleculeData::CoulombMoleculeData(
                            const CoulombFF::CoulombMoleculeData &other)
                     : QSharedData(),
                       molecule(other.molecule),
                       chg_property(other.chg_property),
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
           chg_property == other.chg_property;
}

/** Comparison operator */
bool CoulombFF::CoulombMoleculeData::operator!=(
                            const CoulombFF::CoulombMoleculeData &other) const
{
    return molecule != other.molecule or
           chg_property != other.chg_property;
}

/** Rebuild all of the coordinate and Coulomb data from scratch

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
void CoulombFF::CoulombMoleculeData::rebuildAll()
{
    chgs = molecule.extract().property(chg_property);
    coords = molecule.extract().coordGroups();
}

/** Rebuild all of the coordinate data from scratch */
void CoulombFF::CoulombMoleculeData::rebuildCoordinates()
{
    coords = molecule.extract().coordGroups();
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
        << coulmoldata.chg_property;

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

        PartialMolecule mol;
        QString chg_property;

        sds >> mol >> chg_property;

        coulmoldata = CoulombFF::CoulombMoleculeData(mol,chg_property);
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
CoulombFF::CoulombMolecule::CoulombMolecule(const PartialMolecule &molecule,
                                            const QString &chgproperty)
          : d( new CoulombFF::CoulombMoleculeData(molecule,chgproperty) )
{}

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
    return d->molecule.selectedAtoms().isEmpty();
}

/** Return the actual molecule */
const PartialMolecule& CoulombFF::CoulombMolecule::molecule() const
{
    return d->molecule;
}

/** Return a ChangedCoulombMolecule that represents the change from the
    CoulombMolecule in its current state to 'molecule', where only the
    CutGroups whose IDs are in 'cgids' have changed. (if cgids is
    empty then all CutGroups have changed)

    \throw SireError::incompatible_error
*/
CoulombFF::CoulombMolecule
CoulombFF::CoulombMolecule::change(const PartialMolecule &molecule,
                                   const QString &chgproperty) const
{
    if (d->molecule.ID() == 0)
        //this is a null Molecule
        return CoulombMolecule();

    //assert that this is the same molecule
    d->molecule.assertSameMolecule(molecule);

    if ( chgproperty.isNull() or chgproperty == d->chg_property )
    {
        if ( d->molecule.version() == molecule.version() )
            //there has been no change
            return *this;
        else
        {
            CoulombMolecule newmol(*this);

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
        CoulombMolecule newmol(*this);

        PartialMolecule &mol = newmol.d->molecule;
        mol = mol.change(molecule);

        newmol.d->chg_property = chgproperty;
        newmol.d->rebuildAll();

        return newmol;
    }
}

/** Return a ChangedCoulombMolecule that represents the change from the CoulombMolecule
    in its current state to include the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CoulombFF::CoulombMolecule
CoulombFF::CoulombMolecule::add(const PartialMolecule &mol,
                                const QString &chgproperty) const
{
    if (d->molecule.ID() == 0)
        return CoulombMolecule(mol, chgproperty);

    else if (molecule().selectedAtoms().contains(mol.selectedAtoms()))
        //there will be no change in the atom selections
        return this->change(mol, chgproperty);
    else
    {
        PartialMolecule mol = molecule().change(mol)
                                  .selection().add(mol.selectedAtoms());

        QString chg_property = chgproperty;

        if (chgproperty.isNull())
            chg_property = chargeProperty();

        return CoulombMolecule(mol, chg_property);
    }
}

/** Return a ChangedCoulombMolecule that represents the change from the CoulombMolecule
    in its current state to remove the entire molecule, which itself may
    have changed

    \throw SireError::incompatible_error
*/
CoulombFF::CoulombMolecule
CoulombFF::CoulombMolecule::remove(const PartialMolecule &mol) const
{
    if (d->molecule.ID() == 0)
    {
        return CoulombMolecule();
    }
    else if (molecule().selectedAtoms().intersects(mol.selectedAtoms()))
    {
        CoulombMolecule newmol(*this);

        PartialMolecule &mol = newmol.d->molecule;
        mol = mol.selection().remove(mol.selectedAtoms());

        newmol.d->rebuildAll();

        return newmol;
    }
    else
        //there has been no change
        return *this;
}

/** Return a CoulombMolecule that contains the CutGroups of 'oldmol'
    that appear to have changed in 'newmol' */
CoulombFF::CoulombMolecule
CoulombFF::CoulombMolecule::getDifferences(
                          const CoulombFF::CoulombMolecule &newmol) const
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

    if (old_molecule.selectedAtoms().selectedAllCutGroups() and
        new_molecule.selectedAtoms().selectedAllCutGroups())
    {
        //easy comparison - each CutGroup is represented and
        //they are in the same order in each state of the molecule

        //see if the coordinates have changed
        if (old_coords_array != new_coords_array)
        {
            if (old_chgs_array != new_chgs_array)
            {
                //potentially the coordinates, charges or LJ parameters
                //may have changed

                for (CutGroupID i(0); i<ngroups; ++i)
                {
                    if (old_coords_array[i] != new_coords_array[i] or
                        old_chgs_array[i] != new_chgs_array[i])
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
        else if (old_chgs_array != new_chgs_array)
        {
            //only the charges or LJ parameters may have changed
            for (CutGroupID i(0); i<ngroups; ++i)
            {
                if (old_chgs_array[i] != new_chgs_array[i])
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
                    old_chgs_array[i] != new_chgs_array[newidx])
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
                    old_chgs_array[oldidx] != new_chgs_array[newidx])
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
        return CoulombMolecule();
    else
        //cgids_that_change contains the IDs of all of the CutGroups
        //that change - create a CLJMolecule to represent this change
        return CoulombMolecule( old_molecule.selection().applyMask(cgids_that_change),
                                chargeProperty() );
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
    return d->molecule.selectedAtoms().selectedAll();
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
        << changedmol.oldparts << changedmol.newparts;

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
            >> changedmol.oldparts >> changedmol.newparts;
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
{}

/** Construct the change of all of 'old_molecule' to all of 'new_molecule' */
CoulombFF::ChangedCoulombMolecule::ChangedCoulombMolecule(
                                          const CoulombMolecule &old_molecule,
                                          const CoulombMolecule &new_molecule)
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
CoulombFF::ChangedCoulombMolecule::ChangedCoulombMolecule(
                                    const CoulombFF::ChangedCoulombMolecule &other)
          : oldmol(other.oldmol),
            newmol(other.newmol),
            oldparts(other.oldparts),
            newparts(other.newparts)
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

    return *this;
}

/** Comparison operator */
bool CoulombFF::ChangedCoulombMolecule::operator==(
                              const CoulombFF::ChangedCoulombMolecule &other) const
{
    return oldmol == other.oldmol and
           newmol == other.newmol and
           oldparts == other.oldparts and
           newparts == other.newparts;
}

/** Comparison operator */
bool CoulombFF::ChangedCoulombMolecule::operator!=(
                              const CoulombFF::ChangedCoulombMolecule &other) const
{
    return oldmol != other.oldmol or
           newmol != other.newmol or
           oldparts != other.oldparts or
           newparts != other.newparts;
}

/** Return whether or not this is empty (both old and new are empty) */
bool CoulombFF::ChangedCoulombMolecule::isEmpty() const
{
    return oldmol.isEmpty() and newmol.isEmpty();
}

/** Return whether nothing has changed - this will be when oldmol == newmol */
bool CoulombFF::ChangedCoulombMolecule::nothingChanged() const
{
    return oldmol == newmol;
}

/** Return whether the entire molecule has changed */
bool CoulombFF::ChangedCoulombMolecule::changedAll() const
{
    return newparts == newmol or oldparts == oldmol;
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
CoulombFF::ChangedCoulombMolecule::change(const PartialMolecule &molecule,
                                          const QString &chgproperty) const
{
    return ChangedCoulombMolecule( oldmol,
                                   newmol.change(molecule, chgproperty) );
}

/** Return the ChangedCoulombMolecule that represents the addition of all atoms
    in the molecule 'molecule' */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::add(const PartialMolecule &molecule,
                                       const QString &chgproperty) const
{
    return ChangedCoulombMolecule( oldmol,
                                   newmol.add(molecule, chgproperty) );
}

/** Return the ChangedCoulombMolecule that represents the removal of the entire molecule! */
CoulombFF::ChangedCoulombMolecule
CoulombFF::ChangedCoulombMolecule::remove(const PartialMolecule &molecule) const
{
    return ChangedCoulombMolecule( oldmol,
                                   newmol.remove(molecule) );
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

/** Copy assignment function used by derived classes */
void CoulombFF::_pvt_copy(const FFBase &ffbase)
{
    const CoulombFF &coulff = dynamic_cast<const CoulombFF&>(ffbase);

    spce = coulff.spce;
    switchfunc = coulff.switchfunc;

    components_ptr =
          dynamic_cast<const CoulombFF::Components*>( &(FFBase::components()) );

    BOOST_ASSERT( components_ptr != 0 );
}

/** Set the space within which the molecules interact */
bool CoulombFF::setSpace(const Space &space)
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
bool CoulombFF::setSwitchingFunction(const SwitchingFunction &sfunc)
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
bool CoulombFF::setProperty(const QString &name, const Property &property)
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
Property CoulombFF::getProperty(const QString &name) const
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
bool CoulombFF::containsProperty(const QString &name) const
{
    return ( name == QLatin1String("space") ) or
           ( name == QLatin1String("switching function") ) or
           FFBase::containsProperty(name);
}

/** Return all of the properties of this forcefield, indexed by name */
QHash<QString,Property> CoulombFF::properties() const
{
    QHash<QString,Property> props;

    props.insert( QLatin1String("space"), this->space() );
    props.insert( QLatin1String("switching function"), this->switchingFunction() );

    props.unite( FFBase::properties() );

    return props;
}

/** Register the energy components associated with this forcefield */
void CoulombFF::registerComponents()
{
    std::auto_ptr<CoulombFF::Components> ptr( new CoulombFF::Components(*this) );

    FFBase::registerComponents(ptr.get());

    components_ptr = ptr.release();
}
