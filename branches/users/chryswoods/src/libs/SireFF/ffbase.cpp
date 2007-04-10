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

#include "SireCAS/qhash_sirecas.h"

#include "ffbase.h"
#include "forcefield.h"

#include "SireMol/molecule.h"
#include "SireMol/moleculeid.h"
#include "SireMol/residue.h"
#include "SireMol/resnum.h"
#include "SireMol/resid.h"
#include "SireMol/newatom.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/atomselection.h"

#include "SireBase/property.h"

#include "SireFF/errors.h"
#include "SireMol/errors.h"
#include "SireBase/errors.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireFF;
using namespace SireBase;
using namespace SireCAS;
using namespace SireMol;
using namespace SireStream;

///////////
/////////// Implementation of FFBase::Groups
///////////

/** Constructor */
FFBase::Groups::Groups() : n(1)
{}

/** Copy constructor */
FFBase::Groups::Groups( const FFBase::Groups &other )
       : n(other.n)
{}

/** Destructor */
FFBase::Groups::~Groups()
{}

FFBase::Group FFBase::Groups::mainid(0);

/** Return a new, unique ID for a group in the forcefield */
FFBase::Group FFBase::Groups::getUniqueID()
{
    ++n;
    return FFBase::Group(n);
}

FFBase::Groups FFBase::Groups::default_group;

///////////
/////////// Implementation of FFBase::Parameters
///////////

/** Constructor */
FFBase::Parameters::Parameters()
{}

/** Copy constructor */
FFBase::Parameters::Parameters( const FFBase::Parameters& )
{}

/** Destructor */
FFBase::Parameters::~Parameters()
{}

///////////
/////////// Implementation of FFBase::Components
///////////

/** The symbol representing the x coordinate */
Symbol FFBase::Components::_x("x");

/** The symbol representing the y coordinate */
Symbol FFBase::Components::_y("y");

/** The symbol representing the z coordinate */
Symbol FFBase::Components::_z("z");

/** Add the passed symbols to 'symbols' and return the result */
Symbols FFBase::Components::addToSymbols(Symbols symbols,
                          const Symbol &sym0)
{
    symbols.insert(sym0);

    return symbols;
}

/** Add the passed symbols to 'symbols' and return the result */
Symbols FFBase::Components::addToSymbols(Symbols symbols,
                          const Symbol &sym0, const Symbol &sym1)
{
    symbols.insert(sym0);
    symbols.insert(sym1);

    return symbols;
}

/** Add the passed symbols to 'symbols' and return the result */
Symbols FFBase::Components::addToSymbols(Symbols symbols,
                          const Symbol &sym0, const Symbol &sym1, const Symbol &sym2)
{
    symbols.insert(sym0);
    symbols.insert(sym1);
    symbols.insert(sym2);

    return symbols;
}

/** Add the passed symbols to 'symbols' and return the result */
Symbols FFBase::Components::addToSymbols(Symbols symbols,
                          const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                          const Symbol &sym3)
{
    symbols.insert(sym0);
    symbols.insert(sym1);
    symbols.insert(sym2);
    symbols.insert(sym3);

    return symbols;
}

/** Add the passed symbols to 'symbols' and return the result */
Symbols FFBase::Components::addToSymbols(Symbols symbols,
                          const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                          const Symbol &sym3, const Symbol &sym4)
{
    symbols.insert(sym0);
    symbols.insert(sym1);
    symbols.insert(sym2);
    symbols.insert(sym3);
    symbols.insert(sym4);

    return symbols;
}

/** Add the passed symbols to 'symbols' and return the result */
Symbols FFBase::Components::addToSymbols(Symbols symbols,
                          const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                          const Symbol &sym3, const Symbol &sym4, const Symbol &sym5)
{
    symbols.insert(sym0);
    symbols.insert(sym1);
    symbols.insert(sym2);
    symbols.insert(sym3);
    symbols.insert(sym4);
    symbols.insert(sym5);

    return symbols;
}

/** Add the passed symbols to 'symbols' and return the result */
Symbols FFBase::Components::addToSymbols(Symbols symbols,
                          const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                          const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                          const Symbol &sym6)
{
    symbols.insert(sym0);
    symbols.insert(sym1);
    symbols.insert(sym2);
    symbols.insert(sym3);
    symbols.insert(sym4);
    symbols.insert(sym5);
    symbols.insert(sym6);

    return symbols;
}

/** Add the passed symbols to 'symbols' and return the result */
Symbols FFBase::Components::addToSymbols(Symbols symbols,
                          const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                          const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                          const Symbol &sym6, const Symbol &sym7)
{
    symbols.insert(sym0);
    symbols.insert(sym1);
    symbols.insert(sym2);
    symbols.insert(sym3);
    symbols.insert(sym4);
    symbols.insert(sym5);
    symbols.insert(sym6);
    symbols.insert(sym7);

    return symbols;
}

/** Add the passed symbols to 'symbols' and return the result */
Symbols FFBase::Components::addToSymbols(Symbols symbols,
                          const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                          const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                          const Symbol &sym6, const Symbol &sym7, const Symbol &sym8)
{
    symbols.insert(sym0);
    symbols.insert(sym1);
    symbols.insert(sym2);
    symbols.insert(sym3);
    symbols.insert(sym4);
    symbols.insert(sym5);
    symbols.insert(sym6);
    symbols.insert(sym7);
    symbols.insert(sym8);

    return symbols;
}

/** Add the passed symbols to 'symbols' and return the result */
Symbols FFBase::Components::addToSymbols(Symbols symbols,
                          const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                          const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                          const Symbol &sym6, const Symbol &sym7, const Symbol &sym8,
                          const Symbol &sym9)
{
    symbols.insert(sym0);
    symbols.insert(sym1);
    symbols.insert(sym2);
    symbols.insert(sym3);
    symbols.insert(sym4);
    symbols.insert(sym5);
    symbols.insert(sym6);
    symbols.insert(sym7);
    symbols.insert(sym8);
    symbols.insert(sym9);

    return symbols;
}

/** Constructor */
FFBase::Components::Components()
{}

/** Construct based on the passed FFBase */
FFBase::Components::Components(const FFBase &ffbase, const Symbols &symbols)
       : e_total(ffbase, symbols)
{
    this->registerComponent(e_total);
}

/** Copy constructor */
FFBase::Components::Components(const FFBase::Components &other)
       : e_total(other.e_total), symbolids(other.symbolids)
{}

/** Destructor */
FFBase::Components::~Components()
{}

/** Assignment operator */
FFBase::Components& FFBase::Components::operator=(const FFBase::Components &other)
{
    e_total = other.e_total;
    symbolids = other.symbolids;

    return *this;
}

/** Describe the 'total' component */
QString FFBase::Components::describe_total()
{
    return QObject::tr("The total energy of the forcefield.");
}

/** Set the names of the functions from the passed base name */
void FFBase::Components::setForceField(const FFBase &ffbase)
{
    *this = Components(ffbase);
}

/** Register the component 'component' */
void FFBase::Components::registerComponent(const FFComponent &component)
{
    if (component.symbols().isEmpty())
        throw SireError::program_bug( QObject::tr(
              "You cannot create a forcefield function that isn't a function "
              "of any symbol! (add symbols to %1)")
                  .arg(component.name()), CODELOC );

    symbolids.insert( component.ID() );
}

/** Assert that this contains the component 'component'

    \throw SireFF::missing_component
*/
void FFBase::Components::assertContains(const FFComponent &component) const
{
    if (not this->contains(component))
        throw SireFF::missing_component( QObject::tr(
                "The forcefield does not contain a component represented by the "
                "function \"%1\"").arg(component.toString()), CODELOC );
}

///////////
/////////// Implementation of FFBase
///////////

static const RegisterMetaType<FFBase> r_ffbase(MAGIC_ONLY, "SireFF::FFBase");

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const FFBase &ffbase)
{
    writeHeader(ds, r_ffbase, 1)
          << ffbase.ffname
          << ffbase.id_and_version
          << ffbase.nrg_components
          << ffbase.isdirty;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, FFBase &ffbase)
{
    VersionID v = readHeader(ds, r_ffbase);

    if (v == 1)
    {
        QString newname;

        ds >> newname
           >> ffbase.id_and_version
           >> ffbase.nrg_components
           >> ffbase.isdirty;

        //tell the forcefield about the change in name
        ffbase.setName(newname);
    }
    else
        throw version_error(v, "1", r_ffbase, CODELOC);

    return ds;
}

/** This is the Incremint that is used to increment
    forcefield ID numbers */
static Incremint forcefield_incremint;

/** Null constructor */
FFBase::FFBase() : QSharedData(),
                   ffname(QObject::tr("Unnamed")),
                   id_and_version(&forcefield_incremint),
                   isdirty(true)
{}

/** Construct a forcefield called 'name' */
FFBase::FFBase(const QString &name)
       : QSharedData(),
         ffname(name),
         id_and_version(&forcefield_incremint),
         isdirty(true)
{}

/** Copy constructor */
FFBase::FFBase(const FFBase &other)
       : QSharedData(),
         ffname(other.ffname),
         id_and_version(other.id_and_version),
         nrg_components(other.nrg_components),
         components_ptr( other.components_ptr->clone() ),
         isdirty(other.isdirty)
{}

/** Destructor */
FFBase::~FFBase()
{}

/** Protected function used to copy one FFBase to another - this
    is used by the assignment operator of derived classes. */
FFBase& FFBase::copy(const FFBase &other)
{
    if (this != &other)
    {
        ffname = other.ffname;
        id_and_version = other.id_and_version;
        nrg_components = other.nrg_components;
        components_ptr.reset(other.components_ptr->clone());
        isdirty = other.isdirty;
    }

    return *this;
}

/** Copy assignment from another forcefield */
FFBase& FFBase::operator=(const FFBase &other)
{
    if (&other == this)
        //they are the same!
        return *this;

    //assert that these are of the same type
    if ( QLatin1String(typeid(*this).name()) != QLatin1String(typeid(other).name()) )
    {
        throw SireError::invalid_cast( QObject::tr(
                "Cannot set a forcefield of type \"%1\" equal to a forcefield of "
                "type \"%2\".")
                    .arg(this->what()).arg(other.what()), CODELOC );
    }
    
    this->_pvt_copy(other);
    
    return *this;
}

/** Copy assignment from a forcefield */
FFBase& FFBase::operator=(const ForceField &ffield)
{
    return this->operator=( ffield.base() );
}

/** Comparison operator - two forcefields are equal if they 
    have the same ID and version numbers */
bool FFBase::operator==(const FFBase &other) const
{
    return id_and_version == other.id_and_version;
}

/** Comparison operator - two forcefields are equal if they 
    have the same ID and version numbers */
bool FFBase::operator!=(const FFBase &other) const
{
    return id_and_version != other.id_and_version;
}

/** Return a string representation of this forcefield */
QString FFBase::toString() const
{
    return QString("%1(%2, %3:%4)")
              .arg(this->what())
              .arg(this->name())
              .arg(this->ID())
              .arg(this->version().toString());
}

/** Register the passed components */
void FFBase::registerComponents(FFBase::Components *components)
{
    BOOST_ASSERT( components != 0 );
    components_ptr.reset( components );
}

/** Label the forcefield as dirty (in need of an energy recalculation) */
void FFBase::setDirty()
{
    isdirty = true;
}

/** Label the forcefield as clean (no need to recalculate the energy) */
void FFBase::setClean()
{
    isdirty = false;
}

/** Return the current values of the energy components - these may be
    out of date if the state of the forcefield has been changed
    (and therefore isDirty() is true) */
Values FFBase::currentEnergies() const
{
    return nrg_components;
}

/** Return the name of this forcefield - this is used as the root of all of
    the components of the forcefield, e.g. if the name was "Ligand CLJ", then
    the "coul" component would have the function: E_{Ligand CLJ}_{coul}(x,y,z) */
const QString& FFBase::name() const
{
    return ffname;
}

/** Set the name of this forcefield - this will change the names of all
    of the forcefield components, so beware if you are already using functions
    that are derived from components of this forcefield */
void FFBase::setName(const QString &name)
{
    ffname = name;

    //changing the name also changes the ID number of the forcefield
    id_and_version.incrementID();

    BOOST_ASSERT( components_ptr.get() != 0 );
    components_ptr->setForceField(*this);
}

/** Return the energy of the component represented by the function
    'component'

    \throw SireFF::missing_component
*/
double FFBase::energy(const Function &component)
{
    components().assertContains(component);

    if (isDirty())
    {
        this->recalculateEnergy();
        this->setClean();
    }

    return nrg_components.value(component);
}

/** Return the total energy of this forcefield. This will return the cached
    value if nothing has changed, or will recalculate if something has changed. */
double FFBase::energy()
{
    return energy( components().total() );
}

/** Return the values of all of the energy components of this forcefield */
Values FFBase::energies()
{
    if (isDirty())
    {
        this->recalculateEnergy();
        this->setClean();
    }

    return nrg_components;
}

/** Return the values of all of the specified energy components in this
    forcefield

    \throw SireFF::missing_component
*/
Values FFBase::energies(const QSet<FFComponent> &components)
{
    if (components.isEmpty())
        return Values();

    for (QSet<FFComponent>::const_iterator it = components.begin();
         it != components.end();
         ++it)
    {
        this->components().assertContains(*it);
    }

    Values all_vals = this->energies();

    Values specified_vals;
    specified_vals.reserve(components.count());

    for (QSet<FFComponent>::const_iterator it = components.begin();
         it != components.end();
         ++it)
    {
        specified_vals.set(*it, all_vals.value(*it));
    }

    return specified_vals;
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
bool FFBase::setProperty(const QString &name, const Property&)
{
    //if we are here then we are in error!
    throw SireBase::missing_property( QObject::tr(
                "There is no property called \"%1\" in the forcefield "
                "%2(%3 : %4)")
                    .arg(name).arg(this->what())
                    .arg(this->name()).arg(this->ID()), CODELOC );

    return false;
}

/** Return the property associated with the name 'name'

    \throw SireBase::missing_property
*/
Property FFBase::getProperty(const QString &name) const
{
    //if we are here then we are in error!
    throw SireBase::missing_property( QObject::tr(
                "There is no property called \"%1\" in the forcefield "
                "%2(%3 : %4)")
                    .arg(name).arg(this->what())
                    .arg(this->name()).arg(this->ID()), CODELOC );

    return false;
}

/** Return whether or not this contains a property with the name 'name' */
bool FFBase::containsProperty(const QString &name) const
{
    return false;
}

/** Return all of the properties of this forcefield, indexed by name */
QHash<QString,Property> FFBase::properties() const
{
    return QHash<QString,Property>();
}

/** Change a whole load of partial molecules

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool FFBase::change(const QHash<MoleculeID,PartialMolecule> &molecules)
{
    if (molecules.count() == 0)
        return false;
    else if (molecules.count() == 1)
    {
        QHash<MoleculeID,PartialMolecule>::const_iterator it = molecules.begin();
        BOOST_ASSERT( it.key() == it->ID() );
        
        return this->change(*it);
    }
    else
    {
        //maintain the invariant
        ForceField ffield(*this);
    
        for (QHash<MoleculeID,PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            BOOST_ASSERT( it.key() == it->ID());
            
            ffield.change(*it);
        }
        
        //everything went well
        *this = ffield;
        
        return isDirty();
    }
}

/** Change a whole load of partial molecules

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool FFBase::change(const QList<PartialMolecule> &molecules)
{
    if (molecules.count() == 0)
        return false;
    else if (molecules.count() == 1)
    {
        return this->change(molecules[0]);
    }
    else
    {
        //maintain the invariant
        ForceField ffield(*this);
    
        for (QList<PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            ffield.change(*it);
        }
        
        //everything went well
        *this = ffield;
        
        return isDirty();
    }
}

/** Add lots of molecules 

    \throw SireError::invalid_operation
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
bool FFBase::add(const QList<PartialMolecule> &molecules, const ParameterMap &map)
{
    if (molecules.count() == 0)
        return false;
    else if (molecules.count() == 1)
        return this->add(molecules.at(0), map);
    else
    {
        //maintain the invariant
        ForceField ffield(*this);

        for (QList<PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            ffield.add(*it, map);
        }

        //everything went well!
        *this = ffield;

        return isDirty();
    }
}

/** Add the molecule 'molecule' to the group 'group' in this forcefield using
    the optional parameter map to find any necessary parameters
    from properties of the molecule. This will replace any
    existing copy of the molecule that already exists in
    this forcefield. This returns whether or not the
    forcefield has been changed by this addition, and therefore
    whether its energy needs recalculating.

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
    \throw SireFF::invalid_group
*/
bool FFBase::addTo(const FFBase::Group &group,
                   const PartialMolecule &molecule, const ParameterMap &map)
{
    if (group == this->groups().main())
        return this->add(molecule, map);
    else
    {
        throw SireFF::invalid_group( QObject::tr(
                "Cannot add to %1(%2) (ID == %3) as there is no such group!")
                    .arg(this->what()).arg(this->name())
                    .arg(this->ID()), CODELOC );

        return false;
    }
}

/** Adds lots of molecules to the group 'group' 

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
    \throw SireMol::invalid_operation
    \throw SireFF::invalid_group
*/
bool FFBase::addTo(const FFBase::Group &group,
                   const QList<PartialMolecule> &molecules,
                   const ParameterMap &map)
{
    if (molecules.count() == 0)
        return false;
    else if (molecules.count() == 1)
        return this->addTo(group, molecules.at(0), map);
    else
    {
        //maintain the invariant
        ForceField ffield(*this);

        bool changed = false;

        for (QList<PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool added = ffield.addTo(group, *it, map);
            changed = changed or added;
        }

        //everything went well!
        if (changed)
            *this = ffield;

        return changed;
    }
}

/** Remove a whole load of molecules */
bool FFBase::remove(const QList<PartialMolecule> &molecules)
{
    if (molecules.count() == 0)
        return false;
    else if (molecules.count() == 1)
        return this->remove(molecules.at(0));
    else
    {
        //maintain the invariant
        ForceField ffield(*this);

        bool changed = false;

        for (QList<PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool removed = ffield.remove(*it);
            changed = changed or removed;
        }

        if (changed)
            *this = ffield;

        return changed;
    }
}

/** Remove the molecule 'molecule' from the group 'group'
    from this forcefield - this does nothing if the molecule
    is not in this forcefield. This returns whether or not
    this has changed the forcefield (therefore necessitating
    a recalculation of the energy)

    \throw SireFF::invalid_group
*/
bool FFBase::removeFrom(const FFBase::Group &group,
                        const PartialMolecule &molecule)
{
    if (group == this->groups().main())
        return this->remove(molecule);
    else
    {
        throw SireFF::invalid_group( QObject::tr(
                "Cannot remove from %1(%2) (ID == %3) as there is no such group!")
                    .arg(this->what()).arg(this->name())
                    .arg(this->ID()), CODELOC );

        return false;
    }
}

/** Remove lots of molecules from the group 'group' 

    \throw SireFF::invalid_group
*/
bool FFBase::removeFrom(const FFBase::Group &group,
                        const QList<PartialMolecule> &molecules)
{
    if (molecules.count() == 0)
        return false;
    else if (molecules.count() == 1)
        return this->removeFrom(group, molecules.at(0));
    else
    {
        ForceField ffield(*this);

        bool changed = false;

        for (QList<PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            bool removed = this->removeFrom(group, *it);
            changed = changed or removed;
        }

        if (changed)
            *this = ffield;

        return changed;
    }
}

/** Return whether this forcefield contains a complete copy of
    any version of the partial molecule 'molecule' in the group 'group' 
    
    \throw SireFF::invalid_group
*/
bool FFBase::contains(const PartialMolecule &molecule, const FFBase::Group &group) const
{
    if (group == this->groups().main())
        return this->contains(molecule);
    else
    {
        throw SireFF::invalid_group( QObject::tr(
                "Cannot query %1(%2) (ID == %3) as there is no such group!")
                    .arg(this->what()).arg(this->name())
                    .arg(this->ID()), CODELOC );

        return false;
    }
}

/** Return whether or not the group 'group' in this forcefield
    contains *any part* of any version of the molecule with ID
    'molid' 
    
    \throw SireFF::invalid_group
*/
bool FFBase::refersTo(MoleculeID molid, const FFBase::Group &group) const
{
    if (group == this->groups().main())
        return this->refersTo(molid);
    else
    {
        throw SireFF::invalid_group( QObject::tr(
                "Cannot query %1(%2) (ID == %3) as there is no such group!")
                    .arg(this->what()).arg(this->name())
                    .arg(this->ID()), CODELOC );

        return false;
    }
}

/** Return the set of all of the ID numbers of all of the
    molecules that are referred to by group 'group' in
    this forcefield (i.e. all molecules that have at least
    some part in this group in this forcefield) 
    
    \throw SireFF::invalid_group
*/
QSet<MoleculeID> FFBase::moleculeIDs(const FFBase::Group &group) const
{
    if (group == this->groups().main())
        return this->moleculeIDs();
    else
    {
        throw SireFF::invalid_group( QObject::tr(
                "Cannot query %1(%2) (ID == %3) as there is no such group!")
                    .arg(this->what()).arg(this->name())
                    .arg(this->ID()), CODELOC );

        return QSet<MoleculeID>();
    }
}

/** Return a copy of the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
PartialMolecule FFBase::molecule(MoleculeID molid) const
{
    return this->molecule(molid, FFBase::Groups::main());
}

/** Return copies of the molecules in this forcefield whose IDs 
    are in 'molids'
    
    \throw SireMol::missing_molecule
*/
QHash<MoleculeID,PartialMolecule> FFBase::molecules(
                                          const QSet<MoleculeID> &molids) const
{
    QHash<MoleculeID,PartialMolecule> mols;
    
    mols.reserve(molids.count());
    
    foreach (MoleculeID molid, molids)
    {
        mols.insert( molid, this->molecule(molid) );
    }
    
    return mols;
}

/** Return the contents of the group 'group' in this forcefield 

    \throw SireFF::invalid_group
*/
QHash<MoleculeID,PartialMolecule> FFBase::contents(const FFBase::Group &group) const
{
    if (group == this->groups().main())
        return this->contents();
    else
    {
        throw SireFF::invalid_group( QObject::tr(
                "Cannot query %1(%2) (ID == %3) as there is no such group!")
                    .arg(this->what()).arg(this->name())
                    .arg(this->ID()), CODELOC );

        return QHash<MoleculeID,PartialMolecule>();
    }
}

/** Return copies of all of the molecules that are in this forcefield */
QHash<MoleculeID,PartialMolecule> FFBase::molecules() const
{
    return this->contents();
}

/** Return copies of all of the molecules that are in the group 'group'
    in this forcefield 
    
    \throw SireFF::missing_group
*/
QHash<MoleculeID,PartialMolecule> FFBase::molecules(const FFBase::Group &group) const
{
    return this->contents(group);
}
