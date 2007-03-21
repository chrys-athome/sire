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
#include "SireMol/atomselection.h"

#include "SireFF/errors.h"
#include "SireMol/errors.h"

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
FFBase::Groups::Groups() : n(0)
{
    mainid = this->getUniqueID();
}

/** Copy constructor */
FFBase::Groups::Groups( const FFBase::Groups &other )
       : mainid(other.mainid), n(other.n)
{}

/** Destructor */
FFBase::Groups::~Groups()
{}

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

/** Copy assignment from a forcefield */
FFBase& FFBase::operator=(const ForceField &ffield)
{
    return this->operator=( *(ffield.constData()) );
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

    Values all_vals = this->energies();

    Values specified_vals;
    specified_vals.reserve(components.count());

    for (QSet<FFComponent>::const_iterator it = components.begin();
         it != components.end();
         ++it)
    {
        this->components().assertContains(*it);
        specified_vals.set(*it, all_vals.value(*it));
    }

    return specified_vals;
}

template<class T>
bool FFBase::_pvt_add(const QList<T> &objs, const ParameterMap &map)
{
    if (objs.count() == 0)
        return false;
    else if (objs.count() == 1)
        return this->add(objs.at(0), map);
    else
    {
        //maintain the invariant
        ForceField ffield(*this);

        bool changed = false;

        for (typename QList<T>::const_iterator it = objs.begin();
             it != objs.end();
             ++it)
        {
            bool added = ffield.add(*it, map);
            changed = changed or added;
        }

        //everything went well!
        if (changed)
            *this = ffield;

        return changed;
    }
}

/** Add lots of molecules */
bool FFBase::add(const QList<Molecule> &molecules, const ParameterMap &map)
{
    return this->_pvt_add<Molecule>(molecules, map);
}

/** Add lots of residues */
bool FFBase::add(const QList<Residue> &residues, const ParameterMap &map)
{
    return this->_pvt_add<Residue>(residues, map);
}

/** Add lots of atoms */
bool FFBase::add(const QList<NewAtom> &atoms, const ParameterMap &map)
{
    return this->_pvt_add<NewAtom>(atoms, map);
}

template<class T>
bool FFBase::_pvt_addTo(const FFBase::Group &group,
                        const T &obj, const ParameterMap &map)
{
    if (group == this->groups().main())
        return this->add(obj, map);
    else
    {
        throw SireFF::invalid_group( QObject::tr(
                "Cannot add to %1(%2) (ID == %3) as there is no such group!")
                    .arg(this->what()).arg(this->name())
                    .arg(this->ID()), CODELOC );

        return false;
    }
}

/** Add the selected atoms 'selected_atoms' to this forcefield using the
    optional parameter map to find any necessay parameters
    from properties of the atom. This will replace any
    existing copy of the atom that already exists in
    this forcefield. This returns whether or not the
    forcefield has been changed by this addition, and therefore
    whether its energy needs recalculating.

    This will throw an exception if this forcefield doens't
    support partial molecules.

    \throw SireError::invalid_operation
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool FFBase::addTo(const FFBase::Group &group, const Molecule &molecule,
                   const AtomSelection &selected_atoms, const ParameterMap &map)
{
    if (group == groups().main())
        return this->add(molecule, selected_atoms, map);
    else
    {
        throw SireFF::invalid_group( QObject::tr(
                "Cannot add to %1(%2) (ID == %3) as there is no such group!")
                    .arg(this->what()).arg(this->name())
                    .arg(this->ID()), CODELOC );

        return false;
    }
}

/** Add the molecule 'molecule' to the group 'group' in this forcefield using
    the optional parameter map to find any necessary parameters
    from properties of the molecule. This will replace any
    existing copy of the molecule that already exists in
    this forcefield. This returns whether or not the
    forcefield has been changed by this addition, and therefore
    whether its energy needs recalculating.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool FFBase::addTo(const FFBase::Group &group,
                   const Molecule &molecule, const ParameterMap &map)
{
    return this->_pvt_addTo<Molecule>(group, molecule, map);
}

/** Add the residue 'residue' to the group 'group'
    in this forcefield using
    the optional parameter map to find any necessary parameters
    from properties of the residue. This will replace any
    existing copy of the residue that already exists in
    this forcefield. This returns whether or not the
    forcefield has been changed by this addition, and therefore
    whether its energy needs recalculating.

    This will throw an exception if this forcefield does not
    support partial molecules.

    \throw SireError::invalid_operation
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool FFBase::addTo(const FFBase::Group &group,
                   const Residue &residue, const ParameterMap &map)
{
    return this->_pvt_addTo<Residue>(group, residue, map);
}

/** Add the atom 'atom' to the group 'group'
    in this forcefield using the
    optional parameter map to find any necessay parameters
    from properties of the atom. This will replace any
    existing copy of the atom that already exists in
    this forcefield. This returns whether or not the
    forcefield has been changed by this addition, and therefore
    whether its energy needs recalculating.

    This will throw an exception if this forcefield doens't
    support partial molecules.

    \throw SireError::invalid_operation
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool FFBase::addTo(const FFBase::Group &group,
                   const NewAtom &atom, const ParameterMap &map)
{
    return this->_pvt_addTo<NewAtom>(group, atom, map);
}

template<class T>
bool FFBase::_pvt_addTo(const FFBase::Group &group,
                        const QList<T> &objs,
                        const ParameterMap &map)
{
    if (objs.count() == 0)
        return false;
    else if (objs.count() == 1)
        return this->addTo(group, objs.at(0), map);
    else
    {
        //maintain the invariant
        ForceField ffield(*this);

        bool changed = false;

        for (typename QList<T>::const_iterator it = objs.begin();
             it != objs.end();
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

/** Adds lots of molecules to the group 'group' */
bool FFBase::addTo(const FFBase::Group &group,
                   const QList<Molecule> &molecules,
                   const ParameterMap &map)
{
    return this->_pvt_addTo<Molecule>(group, molecules, map);
}

/** Adds lots of residues to the group 'group' */
bool FFBase::addTo(const FFBase::Group &group,
                   const QList<Residue> &residues,
                   const ParameterMap &map)
{
    return this->_pvt_addTo<Residue>(group, residues, map);
}

/** Adds lots of atoms to the group 'group' */
bool FFBase::addTo(const FFBase::Group &group,
                   const QList<NewAtom> &atoms,
                   const ParameterMap &map)
{
    return this->_pvt_addTo<NewAtom>(group, atoms, map);
}

template<class T>
bool FFBase::_pvt_remove(const QList<T> &objs)
{
    if (objs.count() == 0)
        return false;
    else if (objs.count() == 1)
        return this->remove(objs.at(0));
    else
    {
        //maintain the invariant
        ForceField ffield(*this);

        bool changed = false;

        for (typename QList<T>::const_iterator it = objs.begin();
             it != objs.end();
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

/** Remove a whole load of molecules */
bool FFBase::remove(const QList<Molecule> &molecules)
{
    return this->_pvt_remove<Molecule>(molecules);
}

/** Remove a whole load of residues */
bool FFBase::remove(const QList<Residue> &residues)
{
    return this->_pvt_remove<Residue>(residues);
}

/** Remove a whole load of atoms */
bool FFBase::remove(const QList<NewAtom> &atoms)
{
    return this->_pvt_remove<NewAtom>(atoms);
}

/** Return the copy of the residue in this forcefield that
    is in the molecule with ID == molid and with residue number
    'resnum'

    \throw SireMol::missing_molecule
    \throw SireMol::missing_residue
*/
Residue FFBase::residue(MoleculeID molid, ResNum resnum) const
{
    return this->molecule(molid).residue(resnum);
}

/** Return the copy of the residue in this forcefield that
    is in the molecule with ID == molid and with residue index
    'resid'

    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
Residue FFBase::residue(MoleculeID molid, ResID resid) const
{
    return this->molecule(molid).residue(resid);
}

/** Return the copy of the residue in this forcefield that
    is in the molecule with ID == molid and with residue
    called 'resname'

    \throw SireMol::missing_molecule
    \throw SireMol::missing_residue
*/
Residue FFBase::residue(MoleculeID molid, const QString &resname) const
{
    return this->molecule(molid).residue(resname);
}

/** Return a copy of the atom in this forcefield that
    in the molecule with ID == molid and with index 'atomid'

    \throw SireMol::missing_molecule
    \throw SireMol::missing_residue
    \throw SireMol::missing_cutgroup
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
NewAtom FFBase::atom(MoleculeID molid, const IDMolAtom &atomid) const
{
    return NewAtom(atomid, this->molecule(molid));
}

/** Return the copy of the molecule 'mol' that is in this forcefield

    \throw SireMol::missing_molecule
*/
Molecule FFBase::molecule(const Molecule &mol) const
{
    return this->molecule(mol.ID());
}

/** Return the copy of the residue 'res' that is in this forcefield

    \throw SireMol::missing_molecule
    \throw SireMol::missing_residue
*/
Residue FFBase::residue(const Residue &res) const
{
    return this->residue(res.molecule().ID(), res.resNum());
}

/** Return a copy of the atom 'atom' that is in this forcefield */
NewAtom FFBase::atom(const NewAtom &atom) const
{
    return this->atom(atom.molecule().ID(), atom.cgAtomID());
}
