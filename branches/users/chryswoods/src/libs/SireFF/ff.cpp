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

#include "ff.h"
#include "ffcomponent.h"

#include "tostring.h"

#include "SireFF/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<FF> r_ff( MAGIC_ONLY, FF::typeName() );

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const FF &ff)
{
    writeHeader(ds, r_ff, 1);
    
    SharedDataStream sds(ds);
    
    sds << ff.uid << ff.versn << ff.ffname << ff.nrg_components
        << ff.isdirty
        << static_cast<const MolGroupsBase&>(ff);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, FF &ff)
{
    VersionID v = readHeader(ds, r_ff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> ff.uid >> ff.versn >> ff.ffname >> ff.nrg_components
            >> ff.isdirty
            >> static_cast<MolGroupsBase&>(ff);
    }
    else
        throw version_error(v, "1", r_ff, CODELOC);
        
    return ds;
}

/** Constructor */
FF::FF() : MolGroupsBase(), versn(0), 
                            version_ptr( new Incremint(0) ),
                            ffname( QObject::tr("unnamed") ),
                            isdirty(true)
{
    uid = QUuid::createUuid();
}

/** Construct a forcefield, and also give it a name */
FF::FF(const QString &name) : MolGroupsBase(), 
                              versn(0), 
                              version_ptr( new Incremint(0) ),
                              ffname(name),
                              isdirty(true)
{
    uid = QUuid::createUuid();
}

/** Copy constructor */
FF::FF(const FF &other) : MolGroupsBase(other),
                          uid(other.uid), versn(other.versn),
                          version_ptr(other.version_ptr),
                          ffname(other.ffname),
                          nrg_components(other.nrg_components),
                          isdirty(other.isdirty)
{}

/** Destructor */
FF::~FF()
{}

/** Copy assignment operator */
FF& FF::operator=(const FF &other)
{
    if (this != &other)
    {
        uid = other.uid;
        versn = other.versn;
        version_ptr = other.version_ptr;
        ffname = other.ffname;
        nrg_components = other.nrg_components;
        isdirty = other.isdirty;
        
        MolGroupsBase::operator=(other);
    }
    
    return *this;
}

/** Comparison operator - two forcefields are only identical if
    they have the same UID and version */
bool FF::operator==(const FF &other) const
{
    return uid == other.uid and versn == other.versn;
}

/** Comparison operator - two forcefields are only identical if
    they have the same UID and version */
bool FF::operator!=(const FF &other) const
{
    return uid != other.uid or versn != other.versn;
}

/** Return the current values of the energy components - note that
    these will be invalid if this->isDirty() is true */
const Values& FF::currentEnergies() const
{
    return nrg_components;
}

/** Return the unique ID for this forcefield */
const QUuid& FF::UID() const
{
    return uid;
}

/** Return the version number of this copy of the forcefield */
quint64 FF::version() const
{
    return versn;
}

/** Return a string representation of this forcefield */
QString FF::toString() const
{
    return QObject::tr("%1(\"%2\", version=%3)")
                    .arg(this->what())
                    .arg(this->name())
                    .arg(this->version());
}

/** Increment the version number of this forcefield */
void FF::incrementVersion()
{
    versn = version_ptr->increment();
}

/** Return the name of this forcefield */
const FFName& FF::name() const
{
    return ffname;
}

/** Set the name of this forcefield */
void FF::setName(const FFName &name)
{
    if (ffname != name)
    {
        ffname = name;
        this->incrementVersion();
    }
}

/** Return the energy of the forcefield component represented
    by the passed symbol
    
    \throw SireFF::missing_component
*/
SireUnits::Dimension::Energy FF::energy(const Symbol &component)
{
    if (this->isDirty())
        this->recalculateEnergy();
        
    if (not nrg_components.values().contains(component.ID()))
        throw SireFF::missing_component( QObject::tr(
            "There is no component in this forcefield represented by "
            "the symbol %1. Available components are %2.")
                .arg(component.toString())
                .arg( Sire::toString( nrg_components.values().keys() ) ), CODELOC );

    return SireUnits::Dimension::Energy( nrg_components.value(component) );
}

/** Return the energy of this forcefield in its current state */
SireUnits::Dimension::Energy FF::energy()
{
    return this->energy( this->components().total() );
}

/** Return the values of the specified energy components */
Values FF::energies(const QSet<Symbol> &components)
{
    if (this->isDirty())
        this->recalculateEnergy();
        
    Values vals;
    
    foreach (const Symbol &component, components)
    {
        if (not nrg_components.values().contains(component.ID()))
            throw SireFF::missing_component( QObject::tr(
                "There is no component in this forcefield represented by "
                "the symbol %1. Available components are %2.")
                    .arg(component.toString())
                    .arg( Sire::toString( nrg_components.values().keys() ) ), CODELOC );
    
        vals.set( component, nrg_components.value(component) );
    }
    
    return vals;
}

/** Return the values of all of the energy components of this forcefield */
Values FF::energies()
{
    if (this->isDirty())
        this->recalculateEnergy();
        
    return nrg_components;
}

/** Add the passed view of the molecule to the molecule groups 
    identified by 'mgid' using the default properties to
    find the parameters needed by this forcefield
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::add(const MoleculeView &molview, const MGID &mgid)
{
    this->add(molview, mgid, PropertyMap());
}

/** Add the passed views of the molecule to the molecule groups 
    identified by 'mgid' using the default properties to
    find the parameters needed by this forcefield
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::add(const ViewsOfMol &molviews, const MGID &mgid)
{
    this->add(molviews, mgid, PropertyMap());
}

/** Add the passed molecules to the molecule groups 
    identified by 'mgid' using the default properties to
    find the parameters needed by this forcefield
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::add(const Molecules &molecules, const MGID &mgid)
{
    this->add(molecules, mgid, PropertyMap());
}

/** Add the molecules in the passed MolGroup to the molecule groups 
    identified by 'mgid' using the default properties to
    find the parameters needed by this forcefield
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::add(const MolGroup &molgroup, const MGID &mgid)
{
    this->add(molgroup, mgid, PropertyMap());
}

/** Add the passed view of the molecule to the molecule groups 
    identified by 'mgid' using the default properties to
    find the parameters needed by this forcefield
    
    Only add this view to groups that don't already contain
    this view (the whole view, not part of it)
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::addIfUnique(const MoleculeView &molview, const MGID &mgid)
{
    this->addIfUnique(molview, mgid, PropertyMap());
}

/** Add the passed views of the molecule to the molecule groups 
    identified by 'mgid' using the default properties to
    find the parameters needed by this forcefield
    
    Only add views to groups that don't already contain
    them (the whole view, not part of it, and can add some views)
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::addIfUnique(const ViewsOfMol &molviews, const MGID &mgid)
{
    this->addIfUnique(molviews, mgid, PropertyMap());
}

/** Add the passed molecules to the molecule groups 
    identified by 'mgid' using the default properties to
    find the parameters needed by this forcefield
    
    Only add the views of molecules to groups that don't already contain
    them (the whole view, not part of it)
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::addIfUnique(const Molecules &molecules, const MGID &mgid)
{
    this->addIfUnique(molecules, mgid, PropertyMap());
}

/** Add the molecules in the passed MolGroup to the molecule groups 
    identified by 'mgid' using the default properties to
    find the parameters needed by this forcefield
    
    Only add the views of molecules to groups that don't already contain
    them (the whole view, not part of it)
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::addIfUnique(const MolGroup &molgroup, const MGID &mgid)
{
    this->addIfUnique(molgroup, mgid, PropertyMap());
}

/** Set the contents of this forcefield to just contain 'molview', 
    using the default locations to find the properties that contain
    the forcefield parameters for this molecule
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::setContents(const MGID &mgid, const MoleculeView &molview)
{
    this->setContents(mgid, molview, PropertyMap());
}

/** Set the contents of this forcefield to the molecule views in 'molviews', 
    using the default locations to find the properties that contain
    the forcefield parameters for this molecule
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::setContents(const MGID &mgid, const ViewsOfMol &molviews)
{
    this->setContents(mgid, molviews, PropertyMap());
}

/** Set the contents of this forcefield to the molecules 'molecules', 
    using the default locations to find the properties that contain
    the forcefield parameters for this molecule
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::setContents(const MGID &mgid, const Molecules &molecules)
{
    this->setContents(mgid, molecules, PropertyMap());
}
                         
/** Set the contents of this forcefield to contains the molecules in 'molgroup', 
    using the default locations to find the properties that contain
    the forcefield parameters for this molecule
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void FF::setContents(const MGID &mgid, const MolGroup &molgroup)
{
    this->setContents(mgid, molgroup, PropertyMap());
}
