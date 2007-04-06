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

#include "forcefield.h"
#include "ffbase.h"

#include "SireBase/property.h"

#include "SireMol/molecule.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireBase;
using namespace SireFF;

static const RegisterMetaType<ForceField> r_forcefield;

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ForceField&)
{
    writeHeader(ds, r_forcefield, 0);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ForceField&)
{
    VersionID v = readHeader(ds, r_forcefield);

    if (v != 0)
        throw version_error(v, "1", r_forcefield, CODELOC);

    return ds;
}

namespace SireFF
{
namespace detail
{
class NullFF;
}
}

QDataStream& operator<<(QDataStream&, const SireFF::detail::NullFF&);
QDataStream& operator>>(QDataStream&, SireFF::detail::NullFF&);

namespace SireFF
{
namespace detail
{

/** This is a null forcefield, which is used to provide a null object
    to prevent segfaults when using the null ForceField class.

    @author Christopher Woods
*/
class NullFF : public FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const NullFF&);
friend QDataStream& ::operator>>(QDataStream&, NullFF&);

public:
    NullFF() : FFBase()
    {}

    NullFF(const NullFF &other) : FFBase(other)
    {}

    ~NullFF()
    {}

    class SIREFF_EXPORT Parameters : public FFBase::Parameters
    {
    public:
        Parameters()
        {}

        ~Parameters()
        {}

        static Parameters default_parameter;
    };

    const Parameters& parameters() const
    {
        return Parameters::default_parameter;
    }

    static const char* typeName()
    {
        return "SireFF::detail::NullFF";
    }

    const char* what() const
    {
        return NullFF::typeName();
    }

    NullFF* clone() const
    {
        return new NullFF(*this);
    }

    bool change(const PartialMolecule&)
    {
        return false;
    }
    
    bool add(const PartialMolecule&, const ParameterMap&)
    {
        return false;
    }

    bool remove(const PartialMolecule&)
    {
        return false;
    }
    
    bool contains(const PartialMolecule&) const
    {
        return false;
    }

    bool refersTo(MoleculeID) const
    {
        return false;
    }

    QSet<MoleculeID> moleculeIDs() const
    {
        return QSet<MoleculeID>();
    }

    PartialMolecule molecule(MoleculeID molid) const
    {
        throw SireMol::missing_molecule( QObject::tr(
                  "There is no molecule with ID == %1 in the Null ForceField!")
                        .arg(molid), CODELOC );
                        
        return PartialMolecule();
    }

    QHash<MoleculeID,PartialMolecule> contents() const
    {
        return QHash<MoleculeID,PartialMolecule>();
    }

protected:
    void recalculateEnergy()
    {}

    void _pvt_copy(const FFBase&)
    {}

    /** Null molecule returned by the null forcefield */
    static Molecule null_molecule;
};

NullFF::Parameters NullFF::Parameters::default_parameter;

} // detail
} // SireFF

////////////
//////////// Implementation of NullFF
////////////

Q_DECLARE_METATYPE(SireFF::detail::NullFF)

using namespace SireFF::detail;

static const RegisterMetaType<NullFF> r_nullff;

/** Serialise to a binary datastream */
QDataStream &operator<<(QDataStream &ds, const NullFF &nullff)
{
    writeHeader(ds, r_nullff, 1)
                  << static_cast<const FFBase&>(nullff);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream &operator>>(QDataStream &ds, NullFF &nullff)
{
    VersionID v = readHeader(ds, r_nullff);

    if (v == 1)
    {
        ds >> static_cast<FFBase&>(nullff);
    }
    else
        throw version_error(v, "1", r_nullff, CODELOC);

    return ds;
}

Molecule NullFF::null_molecule;

////////////
//////////// Implementation of ForceField
////////////

static const SharedPolyPointer<FFBase> shared_null( new NullFF() );

/** Constructor */
ForceField::ForceField() : d(shared_null)
{}

/** Construct from the passed FFBase forcefield */
ForceField::ForceField(const FFBase &ffield) : d(ffield)
{}

/** Construct from a shared pointer to a forcefield */
ForceField::ForceField(const SharedPolyPointer<FFBase> &ffptr)
           : d(ffptr)
{
    if (not d.constData())
        d = shared_null;
}

/** Copy constructor */
ForceField::ForceField(const ForceField &other)
           : d(other.d)
{}

/** Destructor */
ForceField::~ForceField()
{}

/** Assignment operator */
ForceField& ForceField::operator=(const ForceField &other)
{
    d = other.d;
    return *this;
}

/** Assignment operator */
ForceField& ForceField::operator=(const FFBase &other)
{
    d = other;
    return *this;
}

/** Return the actual FFBase object that contains the  
    implementation of this forcefield */
const FFBase& ForceField::base() const
{
    return *d;
}

/** Return the class name of the forcefield */
const char* ForceField::what() const
{
    return d->what();
}

/** Return the name of this forcefield - this is used as the root of all of
    the components of the forcefield, e.g. if the name was "Ligand CLJ", then
    the "coul" component would have the function: E_{Ligand CLJ}_{coul}(x,y,z) */
const QString& ForceField::name() const
{
    return d->name();
}

/** Set the name of this forcefield - this will change the names of all
    of the forcefield components, so beware if you are already using functions
    that are derived from components of this forcefield */
void ForceField::setName(const QString& name)
{
    d->setName(name);
}

/** Return the object describing the components of this
    forcefield */
const FFBase::Components& ForceField::components() const
{
    return d->components();
}

/** Return the object containing the names of the parameters
    used by this forcefield */
const FFBase::Parameters& ForceField::parameters() const
{
    return d->parameters();
}

/** Return the object containing the names of the 
    groups used by this forcefield */
const FFBase::Groups& ForceField::groups() const
{
    return d->groups();
}

/** Return the total energy of this forcefield. This will return the cached
    value if nothing has changed, or will recalculate if something has changed. */
double ForceField::energy()
{
    return d->energy();
}

/** Return the energy of the component represented by the function
    'component'

    \throw SireFF::missing_component
*/
double ForceField::energy(const FFComponent &component)
{
    return d->energy(component);
}

/** Return the values of all of the energy components of this forcefield */
Values ForceField::energies()
{
    return d->energies();
}

/** Return the values of all of the specified energy components in this
    forcefield

    \throw SireFF::missing_component
*/
Values ForceField::energies(const QSet<FFComponent> &components)
{
    return d->energies(components);
}

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
bool ForceField::setProperty(const QString &name, const Property &value)
{
    return d->setProperty(name,value);
}

/** Return the property associated with the name 'name'

    \throw SireMol::missing_property
*/
Property ForceField::getProperty(const QString &name) const
{
    return d->getProperty(name);
}

/** Return whether or not this contains a property with the name 'name' */
bool ForceField::containsProperty(const QString &name) const
{
    return d->containsProperty(name);
}

/** Return all of the properties of this forcefield, indexed by name */
QHash<QString,Property> ForceField::properties() const
{
    return d->properties();
}

/** Change the molecule 'molecule' (e.g. move it, or change its
    parameters). This does nothing if the molecule is not
    in this forcefield. Returns whether or not the forcefield
    has been changed by this change, and thus whether the
    energy needs to be recalculated. The same parameter map
    that was used when this molecule was added will be used
    to extract any necessary parameters from the molecule's
    properties

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool ForceField::change(const PartialMolecule &molecule)
{
    return d->change(molecule);
}

/** Change a whole load of partial molecules

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool ForceField::change(const QHash<MoleculeID,PartialMolecule> &molecules)
{
    return d->change(molecules);
}

/** Add the molecule 'molecule' to this forcefield using the
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
bool ForceField::add(const PartialMolecule &molecule, const ParameterMap &map)
{
    return d->add(molecule, map);
}

/** Add lots of molecules 

    \throw SireError::invalid_operation
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceField::add(const QList<PartialMolecule> &molecules,
                     const ParameterMap &map)
{
    return d->add(molecules, map);
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
    \throw SireFF::invalid_group
*/
bool ForceField::addTo(const FFBase::Group &group, const PartialMolecule &molecule,
                       const ParameterMap &map)
{
    return d->addTo(group, molecule, map);
}

/** Adds lots of molecules to the group 'group' 

    \throw SireMol::missing_property
    \throw SireMol::invalid_cast
    \throw SireMol::invalid_operation
    \throw SireFF::invalid_group
*/
bool ForceField::addTo(const FFBase::Group &group,
                       const QList<PartialMolecule> &molecules,
                       const ParameterMap &map)
{
    return d->addTo(group, molecules, map);
}

/** Remove the molecule 'molecule' from this forcefield - this
    does nothing if the molecule is not in this forcefield. This
    returns whether this has changed the forcefield (therefore
    necessitating a recalculation of the energy)

    \throw SireError::invalid_operation
*/
bool ForceField::remove(const PartialMolecule &molecule)
{
    return d->remove(molecule);
}

/** Remove a whole load of molecules */
bool ForceField::remove(const QList<PartialMolecule> &molecules)
{
    return d->remove(molecules);
}

/** Remove the molecule 'molecule' from the group 'group'
    from this forcefield - this does nothing if the molecule
    is not in this forcefield. This returns whether or not
    this has changed the forcefield (therefore necessitating
    a recalculation of the energy)

    \throw SireFF::invalid_group
*/
bool ForceField::removeFrom(const FFBase::Group &group,
                            const PartialMolecule &molecule)
{
    return d->removeFrom(group, molecule);
}

/** Remove lots of molecules from the group 'group' 

    \throw SireFF::invalid_group
*/
bool ForceField::removeFrom(const FFBase::Group &group,
                            const QList<PartialMolecule> &molecules)
{
    return d->removeFrom(group, molecules);
}

/** Return whether this forcefield contains a complete copy of
    any version of the partial molecule 'molecule' */
bool ForceField::contains(const PartialMolecule &molecule) const
{
    return d->contains(molecule);
}

/** Return whether this forcefield contains a complete copy of
    any version of the partial molecule 'molecule' in the group 'group' 
    
    \throw SireFF::invalid_group
*/
bool ForceField::contains(const PartialMolecule &molecule, 
                          const FFBase::Group &group) const
{
    return d->contains(molecule, group);
}

/** Return whether or not this forcefield contains *any part* of
    any version of the molecule with ID 'molid' */
bool ForceField::refersTo(MoleculeID molid) const
{
    return d->refersTo(molid);
}

/** Return whether or not the group 'group' in this forcefield
    contains *any part* of any version of the molecule with ID
    'molid' 
    
    \throw SireFF::invalid_group
*/
bool ForceField::refersTo(MoleculeID molid, const FFBase::Group &group) const
{
    return d->refersTo(molid, group);
}

/** Return the set of all of the ID numbers of all of the
    molecules that are referred to by this forcefield
    (i.e. all molecules that have at least some part
    in this forcefield) */
QSet<MoleculeID> ForceField::moleculeIDs() const
{
    return d->moleculeIDs();
}

/** Return the set of all of the ID numbers of all of the
    molecules that are referred to by group 'group' in
    this forcefield (i.e. all molecules that have at least
    some part in this group in this forcefield) 
    
    \throw SireFF::invalid_group
*/
QSet<MoleculeID> ForceField::moleculeIDs(const FFBase::Group &group) const
{
    return d->moleculeIDs(group);
}

/** Return the copy of the molecule in this forcefield that
    has the ID == molid

    \throw SireMol::missing_molecule
*/
PartialMolecule ForceField::molecule(MoleculeID molid) const
{
    return d->molecule(molid);
}

/** Return copies of the molecules in this forcefield whose IDs 
    are in 'molids'
    
    \throw SireMol::missing_molecule
*/
QHash<MoleculeID,PartialMolecule> ForceField::molecules(
                                          const QSet<MoleculeID> &molids) const
{
    return d->molecules(molids);
}

/** Return the contents of the group 'group' in this forcefield 

    \throw SireFF::invalid_group
*/
QHash<MoleculeID,PartialMolecule> 
ForceField::contents(const FFBase::Group &group) const
{
    return d->contents(group);
}

/** Return all of the molecules (and parts of molecules) that
    are in this forcefield */
QHash<MoleculeID,PartialMolecule> ForceField::contents() const
{
    return d->contents();
}

/** Return whether or not the forcefield is dirty (the energy
    needs to be recalculated) */
bool ForceField::isDirty() const
{
    return d->isDirty();
}

/** Return whether or not the forcefield is clean (the energy
    does not need to be recalculated) */
bool ForceField::isClean() const
{
    return d->isClean();
}

/** Return the ID number of the forcefield */
ForceFieldID ForceField::ID() const
{
    return d->ID();
}

/** Return the version number of the forcefield */
const Version& ForceField::version() const
{
    return d->version();
}

/** Assert that this forcefield contains the component 'component'

    \throw SireFF::missing_component
*/
void ForceField::assertContains(const FFComponent &component) const
{
    d->assertContains(component);
}
