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

#include <QSharedData>

#include "harmonicff.h"

#include "SireBase/property.h"

#include "SireMol/molecule.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/molecules.h"
#include "SireMol/propertyextractor.h"
#include "SireMol/atomselector.h"
#include "SireMol/atomselection.h"

#include "SireError/errors.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireFF;
using namespace SireStream;

////////////
//////////// Implementation of HarmonicFF::HarmonicMoleculeData
////////////

QDataStream& operator<<(QDataStream&, const SireMM::HarmonicFF::HarmonicMoleculeData&);
QDataStream& operator>>(QDataStream&, SireMM::HarmonicFF::HarmonicMoleculeData&);

namespace SireMM
{

/** This is the private implementation of the data for HarmonicMolecule */
class SIREMM_EXPORT HarmonicFF::HarmonicMoleculeData : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const HarmonicFF::HarmonicMoleculeData&);
friend QDataStream& ::operator>>(QDataStream&, HarmonicFF::HarmonicMoleculeData&);

public:
    HarmonicMoleculeData();

    HarmonicMoleculeData(const PartialMolecule &molecule,
                         const QString &r0_property,
                         const QString &k_property);

    HarmonicMoleculeData(const HarmonicMoleculeData &other);

    ~HarmonicMoleculeData();

    HarmonicMoleculeData& operator=(const HarmonicMoleculeData &other);

    bool operator==(const HarmonicMoleculeData &other) const;
    bool operator!=(const HarmonicMoleculeData &other) const;

    const PartialMolecule& molecule() const;

    const Vector& r() const;
    const Vector& r0() const;

    double k() const;

    double energy(const Space &space) const;

    void change(const PartialMolecule &mol);

private:
    void extractHarmonicProperties();

    /** The molecule itself */
    PartialMolecule mol;

    /** The centre of this molecule */
    Vector mol_center;

    /** The equilibrium point for the restraint */
    Vector _r0;

    /** The force constant for the harmonic restraint */
    double _k;

    /** The property used to extract the force constant
        for the restraint */
    QString r0_prop;

    /** The property used to extract the equilibrium point
        for the restraint */
    QString k_prop;
};

}

Q_DECLARE_METATYPE(SireMM::HarmonicFF::HarmonicMoleculeData);

using namespace SireMM;

static const RegisterMetaType<HarmonicFF::HarmonicMoleculeData> r_moldata;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const HarmonicFF::HarmonicMoleculeData &moldata)
{
    writeHeader(ds, r_moldata, 1);

    SharedDataStream sds(ds);
    sds << moldata.mol << moldata.mol_center << moldata._r0
        << moldata._k << moldata.r0_prop << moldata.k_prop;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      HarmonicFF::HarmonicMoleculeData &moldata)
{
    VersionID v = readHeader(ds, r_moldata);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> moldata.mol >> moldata.mol_center >> moldata._r0
            >> moldata._k >> moldata.r0_prop >> moldata.k_prop;
    }
    else
        throw version_error(v, "1", r_moldata, CODELOC);

    return ds;
}

/** Null constructor */
HarmonicFF::HarmonicMoleculeData::HarmonicMoleculeData()
           : QSharedData(),
             mol_center(0), _r0(0), _k(0)
{}

/** Extract the harmonic restraint properties from the molecule

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
void HarmonicFF::HarmonicMoleculeData::extractHarmonicProperties()
{
    //get the force constant
    VariantProperty prop = mol.extract().property(r0_prop);

    if (not prop.canConvert<Vector>())
        throw SireError::invalid_cast( QObject::tr(
            "Cannot convert the property \"%1\" into a Vector!")
                .arg(r0_prop), CODELOC );

    _r0 = prop.value<Vector>();

    prop = mol.extract().property(k_prop);

    if (not prop.canConvert<double>())
        throw SireError::invalid_cast( QObject::tr(
            "Cannot convert the property \"%1\" into a double!")
                .arg(k_prop), CODELOC );

    _k = prop.value<double>();
}

/** Construct for the molecule 'molecule' using the passed properties to
    find the equilibrium point and force constant for the restraint */
HarmonicFF::HarmonicMoleculeData::HarmonicMoleculeData(
                                 const PartialMolecule &molecule,
                                 const QString &r0_property,
                                 const QString &k_property)
           : QSharedData(),
             mol(molecule), _r0(0), _k(0),
             r0_prop(r0_property), k_prop(k_property)
{
    mol_center = mol.extract().geometricCenter();
    extractHarmonicProperties();
}

/** Copy constructor */
HarmonicFF::HarmonicMoleculeData::HarmonicMoleculeData(
                                          const HarmonicMoleculeData &other)
           : QSharedData(),
             mol(other.mol), mol_center(other.mol_center),
             _r0(other._r0), _k(other._k),
             r0_prop(other.r0_prop), k_prop(other.k_prop)
{}

/** Destructor */
HarmonicFF::HarmonicMoleculeData::~HarmonicMoleculeData()
{}

/** Copy assignment operator */
HarmonicFF::HarmonicMoleculeData& HarmonicFF::HarmonicMoleculeData::operator=(
                                            const HarmonicMoleculeData &other)
{
    if (this != &other)
    {
        mol = other.mol;
        mol_center = other.mol_center;
        _r0 = other._r0;
        _k = other._k;
        r0_prop = other.r0_prop;
        k_prop = other.k_prop;
    }

    return *this;
}

/** Comparison operator */
bool HarmonicFF::HarmonicMoleculeData::operator==(
                                      const HarmonicMoleculeData &other) const
{
    return mol == other.mol and
           r0_prop == other.r0_prop and k_prop == other.k_prop;
}

/** Comparison operator */
bool HarmonicFF::HarmonicMoleculeData::operator!=(
                                      const HarmonicMoleculeData &other) const
{
    return mol != other.mol or
           r0_prop != other.r0_prop or k_prop != other.k_prop;
}

/** Return the actual molecule */
const PartialMolecule& HarmonicFF::HarmonicMoleculeData::molecule() const
{
    return mol;
}

/** Return the current value of 'r' (the center of the molecule) */
const Vector& HarmonicFF::HarmonicMoleculeData::r() const
{
    return mol_center;
}

/** Return the equilibrium point for the harmonic restraint */
const Vector& HarmonicFF::HarmonicMoleculeData::r0() const
{
    return _r0;
}

/** Return the force constant of the harmonic restraint */
double HarmonicFF::HarmonicMoleculeData::k() const
{
    return _k;
}

/** Return the energy of the restraint in the passed space */
double HarmonicFF::HarmonicMoleculeData::energy(const Space &space) const
{
    return _k * space.calcDist2(mol_center, _r0);
}

/** Change the molecule */
void HarmonicFF::HarmonicMoleculeData::change(const PartialMolecule &molecule)
{
    if (molecule.version().major() != mol.version().major())
    {
        mol = molecule;
        mol_center = mol.extract().geometricCenter();

        extractHarmonicProperties();
    }
    else if (molecule.version().minor() != mol.version().minor())
    {
        mol = molecule;
        mol_center = mol.extract().geometricCenter();
    }
}

////////////
//////////// Implementation of HarmonicFF::HarmonicMolecule
////////////

static const RegisterMetaType<HarmonicFF::HarmonicMolecule> r_harmmol;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const HarmonicFF::HarmonicMolecule &harmmol)
{
    writeHeader(ds, r_harmmol, 1);

    SharedDataStream sds(ds);
    sds << harmmol.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      HarmonicFF::HarmonicMolecule &harmmol)
{
    VersionID v = readHeader(ds, r_harmmol);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> harmmol.d;
    }
    else
        throw version_error(v, "1", r_harmmol, CODELOC);

    return ds;
}

static QSharedDataPointer<HarmonicFF::HarmonicMoleculeData> shared_null;

static QSharedDataPointer<HarmonicFF::HarmonicMoleculeData> getSharedNull()
{
    if (shared_null.constData() == 0)
    {
        shared_null = new HarmonicFF::HarmonicMoleculeData();
    }

    return shared_null;
}

/** Create a null molecule */
HarmonicFF::HarmonicMolecule::HarmonicMolecule()
           : d( getSharedNull() )
{}

/** Construct from the passed molecule, using the passed properties
    to get the harmonic restraint parameters */
HarmonicFF::HarmonicMolecule::HarmonicMolecule(const PartialMolecule &mol,
                                               const QString &r0_property,
                                               const QString &k_property)
           : d( new HarmonicFF::HarmonicMoleculeData(mol, r0_property, k_property) )
{}

/** Copy constructor */
HarmonicFF::HarmonicMolecule::HarmonicMolecule(const HarmonicMolecule &other)
           : d(other.d)
{}

/** Destructor */
HarmonicFF::HarmonicMolecule::~HarmonicMolecule()
{}

/** Copy assignment operator */
HarmonicFF::HarmonicMolecule& HarmonicFF::HarmonicMolecule::operator=(
                                                      const HarmonicMolecule &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool HarmonicFF::HarmonicMolecule::operator==(const HarmonicMolecule &other) const
{
    return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool HarmonicFF::HarmonicMolecule::operator!=(const HarmonicMolecule &other) const
{
    return d != other.d and *d != *(other.d);
}

/** Return whether or not this is an empty molecule */
bool HarmonicFF::HarmonicMolecule::isEmpty() const
{
    return d->molecule().selectedAtoms().isEmpty();
}

/** Return whether or not this represents the whole molecule */
bool HarmonicFF::HarmonicMolecule::isWholeMolecule() const
{
    return d->molecule().selectedAtoms().selectedAll();
}

/** Return the actual molecule */
const PartialMolecule& HarmonicFF::HarmonicMolecule::molecule() const
{
    return d->molecule();
}

/** Return the version of the molecule */
const Version& HarmonicFF::HarmonicMolecule::version() const
{
    return d->molecule().version();
}

/** Return the current value of 'r' (the center of the molecule) */
const Vector& HarmonicFF::HarmonicMolecule::r() const
{
    return d->r();
}

/** Return the equilibrium point for the harmonic restraint */
const Vector& HarmonicFF::HarmonicMolecule::r0() const
{
    return d->r0();
}

/** Return the force constant of the harmonic restraint */
double HarmonicFF::HarmonicMolecule::k() const
{
    return d->k();
}

/** Return the current energy of the restraint */
double HarmonicFF::HarmonicMolecule::energy(const Space &space) const
{
    return d->energy(space);
}

/** Change the molecule */
void HarmonicFF::HarmonicMolecule::change(const PartialMolecule &molecule)
{
    d->change(molecule);
}

/////////////
///////////// Implementation of HarmonicFF::Components
/////////////

/** Constructor */
HarmonicFF::Components::Components() : FFBase::Components()
{}

/** Construct using the supplied forcefield */
HarmonicFF::Components::Components(const FFBase &ffbase, const Symbols &symbols)
      : FFBase::Components( ffbase, addToSymbols(symbols, x(), y(), z()) )
{}

/** Copy constructor */
HarmonicFF::Components::Components(const HarmonicFF::Components &other)
      : FFBase::Components(other)
{}

/** Destructor */
HarmonicFF::Components::~Components()
{}

/** Assignment operator */
HarmonicFF::Components& HarmonicFF::Components::operator=(
                                      const HarmonicFF::Components &other)
{
    FFBase::Components::operator=(other);
    return *this;
}

/** Set the forcefield */
void HarmonicFF::Components::setForceField(const FFBase &ffbase)
{
    *this = HarmonicFF::Components(ffbase);
}

////////////
//////////// Implementation of HarmonicFF::Parameters
////////////

HarmonicFF::Parameters::Parameters()
           : FFBase::Parameters(),
             _r0("r0"), _k("k")
{}

HarmonicFF::Parameters::Parameters(const HarmonicFF::Parameters &other)
           : FFBase::Parameters(other),
             _r0(other._r0), _k(other._k)
{}

HarmonicFF::Parameters::~Parameters()
{}

HarmonicFF::Parameters HarmonicFF::Parameters::default_sources;

////////////
//////////// Implementation of HarmonicFF
////////////

static const RegisterMetaType<HarmonicFF> r_harmff;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const HarmonicFF &harmff)
{
    writeHeader(ds, r_harmff, 1);

    SharedDataStream sds(ds);

    sds << harmff.mols << harmff.changed_mols << harmff.spce
        << harmff.delta_nrg << harmff.need_total_recalc;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, HarmonicFF &harmff)
{
    VersionID v = readHeader(ds, r_harmff);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> harmff.mols >> harmff.changed_mols >> harmff.spce
            >> harmff.delta_nrg >> harmff.need_total_recalc;

        harmff.reindex();
    }
    else
        throw version_error(v, "1", r_harmff, CODELOC);

    return ds;
}

/** Null constructor */
HarmonicFF::HarmonicFF() : FFBase(), delta_nrg(0), need_total_recalc(true)
{
    FFBase::registerComponents(new HarmonicFF::Components(*this));
}

/** Construct the forcefield using the supplied space to
    calculate distances */
HarmonicFF::HarmonicFF(const Space &space)
           : FFBase(), spce(space), delta_nrg(0), need_total_recalc(true)
{
    FFBase::registerComponents(new HarmonicFF::Components(*this));
}

/** Copy constructor */
HarmonicFF::HarmonicFF(const HarmonicFF &other)
           : FFBase(other),
             mols(other.mols), molid_to_index(other.molid_to_index),
             changed_mols(other.changed_mols), spce(other.spce),
             delta_nrg(other.delta_nrg),
             need_total_recalc(other.need_total_recalc)
{}

/** Destructor */
HarmonicFF::~HarmonicFF()
{}

/** Copy assignment function */
void HarmonicFF::_pvt_copy(const FFBase &ffbase)
{
    const HarmonicFF &other = dynamic_cast<const HarmonicFF&>(ffbase);

    mols = other.mols;
    molid_to_index = other.molid_to_index;
    changed_mols = other.changed_mols;
    spce = other.spce;
    delta_nrg = other.delta_nrg;
    need_total_recalc = other.need_total_recalc;
}

/** Return the space used to calculate distances */
const Space& HarmonicFF::space() const
{
    return spce;
}

/** Tell the forcefield that the energy must be recalculated
    from scratch */
void HarmonicFF::mustNowRecalculateFromScratch()
{
    need_total_recalc = true;
    delta_nrg = 0;
}

/** Set the space that will be used to calculate distances */
bool HarmonicFF::setSpace(const Space &space)
{
    if (spce != space)
    {
        spce = space;
        incrementMajorVersion();
        mustNowRecalculateFromScratch();
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
    \throw SireError::invalid_cast
*/
bool HarmonicFF::setProperty(const QString &name, const Property &value)
{
    if ( name == QLatin1String("space") )
    {
        this->setSpace(value);
        return this->isDirty();
    }
    else
        return FFBase::setProperty(name, value);
}

/** Return the property associated with the name 'name'

    \throw SireBase::missing_property
*/
Property HarmonicFF::getProperty(const QString &name) const
{
    if ( name == QLatin1String("space") )
    {
        return this->space();
    }
    else
        return FFBase::getProperty(name);
}

/** Return whether or not this contains a property with the name 'name' */
bool HarmonicFF::containsProperty(const QString &name) const
{
    return ( name == QLatin1String("space") ) or
           FFBase::containsProperty(name);
}

/** Return all of the properties of this forcefield, indexed by name */
QHash<QString,Property> HarmonicFF::properties() const
{
    QHash<QString,Property> props;

    props.insert( QLatin1String("space"), this->space() );

    props.unite( FFBase::properties() );

    return props;
}

/** Change the molecule 'molecule' - return whether or not
    this changes the forcefield */
bool HarmonicFF::_pvt_change(const PartialMolecule &molecule)
{
    MoleculeID molid = molecule.ID();

    if (molid_to_index.contains(molid))
    {
        //has this molecule already been changed
        QHash<MoleculeID,Molecule>::const_iterator it = changed_mols.constFind(molid);

        if (it != changed_mols.constEnd())
        {
            if (it->version() != molecule.version())
            {
                changed_mols.insert(molid, molecule);
                return true;
            }
            else
                return false;
        }

        uint idx = *(molid_to_index.constFind(molid)->begin());

        if (mols.constData()[idx].version() != molecule.molecule().version())
        {
            //the molecule has changed
            changed_mols.insert(molid, molecule);
            return true;
        }
    }

    return false;
}

/** Change the molecule 'molecule' - return whether or not
    the forcefield is now dirty */
bool HarmonicFF::change(const PartialMolecule &molecule)
{
    if (this->_pvt_change(molecule))
    {
        if (changed_mols.count() == molid_to_index.count())
            need_total_recalc = true;

        incrementMinorVersion();
    }

    return isDirty();
}

/** Change lots of molecules */
bool HarmonicFF::change(const Molecules &molecules)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
    {
        return this->change( *(molecules.begin()) );
    }
    else
    {
        HarmonicFF old_ff = *this;

        bool changed = false;

        try
        {
            for (Molecules::const_iterator it = molecules.begin();
                 it != molecules.end();
                 ++it)
            {
                bool this_changed = this->_pvt_change(*it);
                changed = changed or this_changed;
            }

            if (changed)
            {
                if (changed_mols.count() == molid_to_index.count())
                    need_total_recalc = true;

                incrementMinorVersion();
            }
        }
        catch(...)
        {
            *this = old_ff;
            throw;
        }

        return isDirty();
    }
}

/** Add the molecule 'molecule' to this forcefield, using
    the supplied map to extract the harmonic restraint properties.

    This molecule can appear multiple times in this forcefield,
    with each time involving a different set of selected atoms.
*/
bool HarmonicFF::_pvt_add(const PartialMolecule &molecule,
                          const ParameterMap &map)
{
    //create a holder for this molecule
    HarmonicMolecule mol( molecule, map.source(parameters().r0()),
                                    map.source(parameters().k()) );

    //change the molecule
    bool changed = this->_pvt_change(molecule);

    MoleculeID molid = molecule.ID();

    //now see if we have to add the molecule
    QHash< MoleculeID, QHash<AtomSelection,uint> >::const_iterator it
                                                    = molid_to_index.constFind(molid);

    if ( it == molid_to_index.constEnd() or
         not it->contains(molecule.selectedAtoms()) )
    {
        //we need to add this molecule
        uint idx = mols.count();
        mols.append(mol);
        molid_to_index[molid].insert(molecule.selectedAtoms(),idx);

        if (not need_total_recalc)
            delta_nrg += mol.energy(spce);

        return true;
    }
    else
        return changed;
}

/** Add the molecule 'molecule' to this forcefield, using
    the supplied map to extract the harmonic restraint properties.

    This molecule can appear multiple times in this forcefield,
    with each time involving a different set of selected atoms.
*/
bool HarmonicFF::add(const PartialMolecule &molecule, const ParameterMap &map)
{
    if (this->_pvt_add(molecule,map))
        incrementMajorVersion();

    return isDirty();
}

/** Add lots of molecules to this forcefield */
bool HarmonicFF::add(const Molecules &molecules, const ParameterMap &map)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
    {
        return this->add( *(molecules.begin()), map );
    }
    else
    {
        HarmonicFF orig_ff = *this;
        bool changed = false;

        try
        {
            for (Molecules::const_iterator it = molecules.begin();
                 it != molecules.end();
                 ++it)
            {
                bool this_changed = this->_pvt_add(*it, map);
                changed = changed or this_changed;
            }

            if (changed)
                incrementMajorVersion();
        }
        catch(...)
        {
            *this = orig_ff;
            throw;
        }

        return isDirty();
    }
}

/** Reindex the molecules array */
void HarmonicFF::reindex()
{
    molid_to_index.clear();

    uint nmols = mols.count();
    const HarmonicMolecule *mols_array = mols.constData();

    for (uint i=0; i<nmols; ++i)
    {
        const PartialMolecule &mol = mols_array[i].molecule();

        molid_to_index[mol.ID()].insert(mol.selectedAtoms(),i);
    }
}

/** Remove the molecule 'molecule' from this forcefield */
bool HarmonicFF::_pvt_remove(const PartialMolecule &molecule)
{
    QHash< MoleculeID,QHash<AtomSelection,uint> >::const_iterator
                                       it = molid_to_index.constFind(molecule.ID());

    if (it != molid_to_index.constEnd())
    {
        QHash<AtomSelection,uint>::const_iterator it2 =
                                            it->constFind(molecule.selectedAtoms());

        if (it2 != it->constEnd())
        {
            //the molecule does exist in this forcefield
            if (not need_total_recalc)
                delta_nrg -= mols.constData()[*it2].energy(spce);

            mols.remove(*it2);

            if (it->count() == 1)
                //we have just removed the last part of this molecule
                changed_mols.remove(molecule.ID());

            return true;
        }
    }

    return false;
}

/** Remove the molecule 'molecule' from this forcefield */
bool HarmonicFF::remove(const PartialMolecule &molecule)
{
    if (this->_pvt_remove(molecule))
    {
        reindex();
        incrementMajorVersion();
    }

    return isDirty();
}

/** Remove lots of molecules */
bool HarmonicFF::remove(const Molecules &molecules)
{
    if (molecules.count() == 0)
        return isDirty();
    else if (molecules.count() == 1)
    {
        return this->remove( *(molecules.begin()) );
    }
    else
    {
        HarmonicFF orig_ff = *this;
        bool changed = false;

        try
        {
            for (Molecules::const_iterator it = molecules.begin();
                 it != molecules.end();
                 ++it)
            {
                bool this_changed = this->_pvt_remove(*it);
                changed = changed or this_changed;
            }

            if (changed)
            {
                reindex();
                incrementMajorVersion();
            }
        }
        catch(...)
        {
            *this = orig_ff;
            throw;
        }

        return isDirty();
    }
}

/** Return whether or not this forcefield refers to the molecule
    with ID == molid */
bool HarmonicFF::refersTo(MoleculeID molid) const
{
    return molid_to_index.contains(molid);
}

/** Return the forcefield groups that refer to the molecule with
    ID == molid */
QSet<FFBase::Group> HarmonicFF::groupsReferringTo(MoleculeID molid) const
{
    if (this->refersTo(molid))
    {
        QSet<FFBase::Group> groups;
        groups.insert(this->groups().main());
        return groups;
    }
    else
        return QSet<FFBase::Group>();
}

/** Return the complete set of MoleculeIDs of all of the molecules
    in this forcefield */
QSet<MoleculeID> HarmonicFF::moleculeIDs() const
{
    return molid_to_index.keys().toSet();
}

/** Return the molecule in this forcefield with ID == molid

    \throw SireMol::missing_molecule
*/
PartialMolecule HarmonicFF::molecule(MoleculeID molid) const
{
    if (not molid_to_index.contains(molid))
        throw SireMol::missing_molecule( QObject::tr(
            "The forcefield \"%1\" (%2) does not contain the molecule with "
            "ID == %3.")
                .arg(name()).arg(ID()).arg(molid), CODELOC );

    QHash<AtomSelection,uint> parts = molid_to_index.value(molid);

    BOOST_ASSERT( not parts.isEmpty() );

    //unite the selections together
    QHash<AtomSelection,uint>::const_iterator it = parts.constBegin();
    AtomSelection selection = it.key();
    ++it;

    while (it != parts.constEnd() and not selection.selectedAll())
    {
        selection = selection.unite(it.key());
        ++it;
    }

    return mols[ parts.constBegin().value() ].molecule().selection().set(selection);
}

/** Return whether or not this forcefield contains the passed molecule. */
bool HarmonicFF::contains(const PartialMolecule &molecule) const
{
    if (this->refersTo(molecule.ID()))
    {
        return this->molecule(molecule.ID()).selectedAtoms()
                                  .contains(molecule.selectedAtoms());
    }
    else
        return false;
}

/** Return all of the molecules within this forcefield */
Molecules HarmonicFF::contents() const
{
    Molecules all_mols;

    foreach (MoleculeID molid, this->moleculeIDs())
    {
        all_mols.append( this->molecule(molid) );
    }

    return all_mols;
}

/** Recalculate the total energy from scratch */
void HarmonicFF::recalculateTotalEnergy()
{
    double nrg = 0;

    int nmols = mols.count();
    const HarmonicMolecule *mols_array = mols.constData();

    if (not changed_mols.isEmpty())
    {
        QVector<HarmonicMolecule> orig_mols = mols;

        try
        {
            //change all of the molecules
            for (QHash<MoleculeID,Molecule>::const_iterator
                                                    it = changed_mols.constBegin();
                 it != changed_mols.constEnd();
                 ++it)
            {
                BOOST_ASSERT( molid_to_index.contains(it.key()) );

                const QHash<AtomSelection,uint> &parts
                                      = *(molid_to_index.constFind(it.key()));

                for (QHash<AtomSelection,uint>::const_iterator it2 = parts.begin();
                     it2 != parts.end();
                     ++it2)
                {
                    mols[*it2].change(*it);
                }
            }
        }
        catch(...)
        {
            mols = orig_mols;
            throw;
        }
    }

    for (int i=0; i<nmols; ++i)
    {
        nrg += mols_array[i].energy(spce);
    }

    setComponent( components().total(), nrg );
    need_total_recalc = false;
    delta_nrg = 0;
    changed_mols.clear();
}

/** Recalculate the energy of the restraints */
void HarmonicFF::recalculateEnergy()
{
    if (need_total_recalc)
    {
        this->recalculateTotalEnergy();
    }
    else
    {
        QVector<HarmonicMolecule> orig_mols = mols;

        try
        {
            const HarmonicMolecule *mols_array = mols.constData();

            //change all of the molecules
            for (QHash<MoleculeID,Molecule>::const_iterator it = changed_mols.constBegin();
                 it != changed_mols.constEnd();
                 ++it)
            {
                BOOST_ASSERT( molid_to_index.contains(it.key()) );

                const QHash<AtomSelection,uint> &parts
                                      = *(molid_to_index.constFind(it.key()));

                for (QHash<AtomSelection,uint>::const_iterator it2 = parts.begin();
                     it2 != parts.end();
                     ++it2)
                {
                    BOOST_ASSERT( *it2 < uint(mols.count()) );

                    HarmonicMolecule mol = mols_array[*it2];

                    delta_nrg -= mol.energy(spce);
                    mol.change(*it);
                    delta_nrg += mol.energy(spce);
                }
            }
        }
        catch(...)
        {
            mols = orig_mols;
            throw;
        }

        changeComponent( components().total(), delta_nrg );
        need_total_recalc = false;
        delta_nrg = 0;
        changed_mols.clear();
    }
}
