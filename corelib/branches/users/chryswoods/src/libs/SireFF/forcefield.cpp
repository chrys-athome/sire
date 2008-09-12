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

#include "forcefield.h"

#include "SireMol/molecule.h"
#include "SireMol/mover.hpp"
#include "SireMol/viewsofmol.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/moleculegroup.h"
#include "SireMol/mgnum.h"

#include "SireError/errors.h"
#include "SireBase/errors.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include <QDebug>

using namespace SireFF;
using namespace SireBase;
using namespace SireStream;

///////////
/////////// Implementation of NullFF
///////////

static const RegisterMetaType<NullFF> r_nullff;

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const NullFF &nullff)
{
    writeHeader(ds, r_nullff, 0);
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, NullFF &nullff)
{
    VersionID v = readHeader(ds, r_nullff);
    
    if (v != 0)
        throw version_error(v, "0", r_nullff, CODELOC);
        
    return ds;
}

static NullFF *shared_null_ff;

NullFF& NullFF::getSharedNullFF()
{
    if (shared_null_ff == 0)
    {
        shared_null_ff = new NullFF(false);
    }
    
    return *shared_null_ff;
}

/** Private constructor */
NullFF::NullFF(bool) : ConcreteProperty<NullFF,FF>()
{}

/** Constructor */
NullFF::NullFF() : ConcreteProperty<NullFF,FF>( NullFF::getSharedNullFF() )
{}

/** Copy constructor */
NullFF::NullFF(const NullFF &other) : ConcreteProperty<NullFF,FF>(other)
{}

/** Destructor */
NullFF::~NullFF()
{}

/** Copy assignment */
NullFF& NullFF::operator=(const NullFF &other)
{
    FF::operator=(other);
    return *this;
}

/** All null forcefields are equal */
bool NullFF::operator==(const NullFF &other) const
{
    return true;
}

/** All null forcefields are equal */
bool NullFF::operator!=(const NullFF &other) const
{
    return false;
}

QString NullFF::toString() const
{
    return "NullFF()";
}

bool NullFF::setProperty(const QString &name, const Property &value)
{
    throw SireBase::missing_property( QObject::tr(
        "The null forcefield has no properties!"), CODELOC );

    return false;
}

const Property& NullFF::property(const QString &name) const
{
    throw SireBase::missing_property( QObject::tr(
        "The null forcefield has no properties!"), CODELOC );

    return props.property(name);
}

bool NullFF::containsProperty(const QString &name) const
{
    return false;
}

const Properties& NullFF::properties() const
{
    return props;
}

void NullFF::mustNowRecalculateFromScratch()
{}

const FFComponent& NullFF::_pvt_components() const
{
    throw SireError::invalid_state( QObject::tr(
        "The null forcefield has no components!"), CODELOC );
}

void NullFF::recalculateEnergy()
{}

static void throwNoAdd()
{
    throw SireError::invalid_state( QObject::tr(
        "You cannot add anything to the null forcefield."), CODELOC );
}

void NullFF::group_add(quint32 i, const MoleculeView &molview,
                       const PropertyMap &map)
{
    throwNoAdd();
}

void NullFF::group_add(quint32 i, const ViewsOfMol &molviews, 
                       const PropertyMap &map)
{
    throwNoAdd();
}

void NullFF::group_add(quint32 i, const Molecules &molecules, 
                       const PropertyMap &map)
{
    throwNoAdd();
}

void NullFF::group_add(quint32 i, const MolGroup &molgroup, 
                       const PropertyMap &map)
{
    throwNoAdd();
}

bool NullFF::group_addIfUnique(quint32 i, const MoleculeView &molview, 
                               const PropertyMap &map)
{
    throwNoAdd();
    return false;
}

ViewsOfMol NullFF::group_addIfUnique(quint32 i, const ViewsOfMol &molviews, 
                                     const PropertyMap &map)
{
    throwNoAdd();
    return ViewsOfMol();
}

QList<ViewsOfMol> NullFF::group_addIfUnique(quint32 i, const Molecules &molecules, 
                                            const PropertyMap &map)
{
    throwNoAdd();
    throw QList<ViewsOfMol>();
}

QList<ViewsOfMol> NullFF::group_addIfUnique(quint32 i, const MolGroup &molgroup, 
                                            const PropertyMap &map)
{
    throwNoAdd();
    return QList<ViewsOfMol>();
}

static void throwNoRemove()
{
    throw SireError::invalid_state( QObject::tr(
        "You cannot remove anything from the null forcefield."), CODELOC );
}

bool NullFF::group_remove(quint32 i, const MoleculeView &molview)
{
    throwNoRemove();
    return false;
}

ViewsOfMol NullFF::group_remove(quint32 i, const ViewsOfMol &molviews)
{
    throwNoRemove();
    return ViewsOfMol();
}

QList<ViewsOfMol> NullFF::group_remove(quint32 i, const Molecules &molecules)
{
    throwNoRemove();
    return QList<ViewsOfMol>();
}

QList<ViewsOfMol> NullFF::group_remove(quint32 i, const MolGroup &molgroup)
{
    throwNoRemove();
    return QList<ViewsOfMol>();
}

bool NullFF::group_removeAll(quint32 i, const MoleculeView &molview)
{
    throwNoRemove();
    return false;
}

ViewsOfMol NullFF::group_removeAll(quint32 i, const ViewsOfMol &molviews)
{
    throwNoRemove();
    return ViewsOfMol();
}

QList<ViewsOfMol> NullFF::group_removeAll(quint32 i, const Molecules &molecules)
{
    throwNoRemove();
    return QList<ViewsOfMol>();
}

QList<ViewsOfMol> NullFF::group_removeAll(quint32 i, const MolGroup &molgroup)
{
    throwNoRemove();
    return QList<ViewsOfMol>();
}

ViewsOfMol NullFF::group_remove(quint32 i, MolNum molnum)
{
    throwNoRemove();
    return ViewsOfMol();
}

QList<ViewsOfMol> NullFF::group_remove(quint32 i, const QSet<MolNum> &molnums)
{
    throwNoRemove();
    return QList<ViewsOfMol>();
}

void NullFF::group_removeAll(quint32 i)
{
    throwNoRemove();
}

bool NullFF::group_update(quint32 i, const MoleculeData &moldata)
{
    return false;
}

QList<Molecule> NullFF::group_update(quint32 i, const Molecules &molecules)
{
    return QList<Molecule>();
}

QList<Molecule> NullFF::group_update(quint32 i, const MolGroup &molgroup)
{
    return QList<Molecule>();
}

static void throwNoSetContents()
{
    throw SireError::invalid_state( QObject::tr(
        "You cannot set the contents of a null forcefield!"), CODELOC );
}

bool NullFF::group_setContents(quint32 i, const MoleculeView &molview, 
                               const PropertyMap &map)
{
    throwNoSetContents();
    return false;
}

bool NullFF::group_setContents(quint32 i, const ViewsOfMol &molviews, 
                               const PropertyMap &map)
{
    throwNoSetContents();
    return false;
}

bool NullFF::group_setContents(quint32 i, const Molecules &molecules, 
                               const PropertyMap &map)
{
    throwNoSetContents();
    return false;
}

bool NullFF::group_setContents(quint32 i, const MolGroup &molgroup, 
                               const PropertyMap &map)
{
    throwNoSetContents();
    return false;
}

void NullFF::_pvt_updateName()
{
    throw SireError::invalid_state( QObject::tr(
        "You cannot change the name of a null forcefield."), CODELOC );
}

void NullFF::_pvt_restore(const ForceField &ffield)
{
    if (not ffield->isA<NullFF>())
        throw SireError::invalid_cast( QObject::tr(
            "You cannot restore a NullFF using a %1.")
                .arg(ffield->what()), CODELOC );
}

static void throwNoGroups()
{
    throw SireMol::missing_group( QObject::tr(
        "There are no molecule groups in the null forcefield!"), CODELOC );
}

const MolGroup& NullFF::at(MGNum mgnum) const
{
    throwNoGroups();
    return this->getGroup(mgnum);
}

const MolGroup& NullFF::getGroup(MGNum mgnum) const
{
    return NullFF::at(mgnum);
}

void NullFF::getGroups(const QList<MGNum> &mgnums,
                       QVarLengthArray<const MolGroup*,10> &groups) const
{
    throwNoGroups();
}

QHash<MGNum,const MolGroup*> NullFF::getGroups() const
{
    return QHash<MGNum,const MolGroup*>();
}

void NullFF::group_setName(quint32 i, const QString &new_name)
{
    throwNoGroups();
}

///////////
/////////// Implementation of ForceField
///////////

static const RegisterMetaType<ForceField> r_ff;

/** Serialise a ForceField to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds,
                                      const ForceField &ff)
{
    writeHeader(ds, r_ff, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const Property&>(ff);

    #warning Need to save and load ff equations!
    
    return ds;
}

/** Deserialise a ForceField from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds,
                                      ForceField &ff)
{
    VersionID v = readHeader(ds, r_ff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> static_cast<Property&>(ff);

        #warning Need to save and load ff equations!
    }
    else
        throw version_error(v, "1", r_ff, CODELOC);
        
    return ds;
}

static ForceField *_pvt_shared_null = 0;

const ForceField& ForceField::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new ForceField( NullFF() );
        
    return *_pvt_shared_null;
}

/** Null constructor - constructs a simple, NullFF() object */
ForceField::ForceField() : Property(ForceField::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
ForceField::ForceField(const PropertyBase &property)
           : Property(property.asA<FF>())
{}

/** Construct from passed FF */
ForceField::ForceField(const FF &ff) : Property(ff)
{} 

/** Copy constructor */
ForceField::ForceField(const ForceField &other)
           : Property(other)
{}

/** Destructor */
ForceField::~ForceField()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
ForceField& ForceField::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<FF>());
    return *this;
}

/** Copy assignment operator from another FF */
ForceField& ForceField::operator=(const FF &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const FF* ForceField::operator->() const
{
    return static_cast<const FF*>(&(d()));
}

/** Dereference this pointer */
const FF& ForceField::operator*() const
{
    return static_cast<const FF&>(d());
}

/** Return a read-only reference to the contained object */
const FF& ForceField::read() const
{
    return static_cast<const FF&>(d());
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
FF& ForceField::edit()
{
    return static_cast<FF&>(d());
}
    
/** Return a raw pointer to the FF object */
const FF* ForceField::data() const
{
    return static_cast<const FF*>(&(d()));
}

/** Return a raw pointer to the FF object */
const FF* ForceField::constData() const
{
    return static_cast<const FF*>(&(d()));
}
    
/** Return a raw pointer to the FF object */
FF* ForceField::data()
{
    return static_cast<FF*>(&(d()));
}

/** Automatic casting operator */
ForceField::operator const FF&() const
{
    return static_cast<const FF&>(d());
}
