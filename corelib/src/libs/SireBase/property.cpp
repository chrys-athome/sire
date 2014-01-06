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

#include <QMutex>

#include "property.h"

#include <QDebug>

#include "SireError/getbacktrace.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireBase;
using namespace SireBase;
using namespace SireStream;

Q_GLOBAL_STATIC_WITH_ARGS( QMutex, getGlobalMutex, ((QMutex::Recursive)) );

namespace SireBase
{

/** Return a pointer to a global mutex */
QMutex SIREBASE_EXPORT *globalLock()
{
    return getGlobalMutex();
}

} // end of namespace SireBase

///////////////
/////////////// Implementation of Property
///////////////

/** Constructor */
Property::Property() : QSharedData()
{}

/** Copy constructor */
Property::Property(const Property&) : QSharedData()
{}

/** Destructor */
Property::~Property()
{}

/** Assignment operator */
Property& Property::operator=(const Property&)
{
    return *this;
}

/** Comparison operator */
bool Property::operator==(const Property&) const
{
    return true;
}

/** Comparison operator */
bool Property::operator!=(const Property&) const
{
    return false;
}

/** Default 'toString()' function for properties - it would
    help if all properties output something more sensible */
QString Property::toString() const
{
    return QString("%1()").arg( this->what() );
}

/** Throw an invalid cast!

    \throw SireError::invalid_cast
*/
void Property::throwInvalidCast(const Property &other) const
{
    throw SireError::invalid_cast( QObject::tr(
            "Cannot cast from an object of class \"%1\" to an object "
            "of class \"%2\".")
                .arg(other.what()).arg(this->what()), CODELOC );
}

/** Throw an invalid cast!

    \throw SireError::invalid_cast
*/
void Property::throwInvalidCast(const char *typenam) const
{
    throw SireError::invalid_cast( QObject::tr(
            "Cannot cast from an object of class \"%1\" to an object "
            "of class \"%2\".")
                .arg(this->what()).arg(typenam), CODELOC );
}

static const RegisterMetaType<Property> r_propbase(MAGIC_ONLY,
                                                       "SireBase::Property");

/** Serialise to a binary data stream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const Property&)
{
    writeHeader(ds, r_propbase, 0);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, Property&)
{
    VersionID v = readHeader(ds, r_propbase);

    if (v != 0)
        throw version_error(v, "0", r_propbase, CODELOC);

    return ds;
}

///////////////
/////////////// Implementation of VariantProperty
///////////////

static const RegisterMetaType<VariantProperty> r_varprop;

/** Serialise to a binary data stream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const VariantProperty &varprop)
{
    writeHeader(ds, r_varprop, 1)
          << static_cast<const Property&>(varprop)
          << static_cast<const QVariant&>(varprop);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, VariantProperty &varprop)
{
    VersionID v = readHeader(ds, r_varprop);

    if (v == 1)
    {
        ds >> static_cast<Property&>(varprop)
           >> static_cast<QVariant&>(varprop);
    }
    else
        throw version_error(v, "1", r_varprop, CODELOC);

    return ds;
}

/** Null constructor */
VariantProperty::VariantProperty()
                : ConcreteProperty<VariantProperty,Property>(), QVariant()
{}

/** Construct a property equal to 'value' */
VariantProperty::VariantProperty(const QVariant &value)
                : ConcreteProperty<VariantProperty,Property>(), QVariant(value)
{}

/** Construct from a 'Property' - the property must be able to
    be cast to a VariantProperty

    \throw SireError::invalid_cast
*/
VariantProperty::VariantProperty(const Property &property)
                : ConcreteProperty<VariantProperty,Property>(), QVariant()
{
    *this = property;
}

/** Copy constructor */
VariantProperty::VariantProperty(const VariantProperty &other)
                : ConcreteProperty<VariantProperty,Property>(other), QVariant(other)
{}

/** Destructor */
VariantProperty::~VariantProperty()
{}

/** Throw an invalid cast error */
void VariantProperty::throwInvalidCast(const QString &typname) const
{
    throw SireError::invalid_cast( QObject::tr(
        "Cannot convert an object of type %1 to an object of type %2.")
            .arg(QVariant::typeName()).arg(typname), CODELOC );
}

/** Assignment operator from a QVariant */
VariantProperty& VariantProperty::operator=(const QVariant &other)
{
    QVariant::operator=(other);
    return *this;
}

/** Assignment operator from a VariantProperty */
VariantProperty& VariantProperty::operator=(const VariantProperty &other)
{
    QVariant::operator=(other);
    Property::operator=(other);

    return *this;
}

/** Comparison operator */
bool VariantProperty::operator==(const VariantProperty &other) const
{
    return QVariant::operator==(other);
}

/** Comparison operator */
bool VariantProperty::operator!=(const VariantProperty &other) const
{
    return QVariant::operator!=(other);
}

const char* VariantProperty::typeName()
{
    return QMetaType::typeName( qMetaTypeId<VariantProperty>() );
}

/** String operator */
QString VariantProperty::toString() const
{
    if (this->canConvert<QString>())
        return this->value<QString>();
    else
    {
        return QString("VariantProperty( %1() )")
                    .arg(this->typeName());
    }
}

///////////////
/////////////// Implementation of NullProperty
///////////////

static const RegisterMetaType<NullProperty> r_nullprop;

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const NullProperty &property)
{
    writeHeader(ds, r_nullprop, 1);

    ds << static_cast<const Property&>(property);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, NullProperty &property)
{
    VersionID v = readHeader(ds, r_nullprop);

    if (v == 1)
    {
        ds >> static_cast<Property&>(property);
    }
    else
        throw version_error(v, "1", r_nullprop, CODELOC);

    return ds;
}

NullProperty::NullProperty()
             : ConcreteProperty<NullProperty,Property>()
{}

NullProperty::NullProperty(const NullProperty &other)
             : ConcreteProperty<NullProperty,Property>(other)
{}

NullProperty::~NullProperty()
{}

const char* NullProperty::typeName()
{
    return QMetaType::typeName( qMetaTypeId<NullProperty>() );
}

static SharedPolyPointer<NullProperty> global_null;

/** Return the global null property */
const NullProperty& Property::null()
{
    if (global_null.constData() == 0)
    {
        QMutexLocker lkr( globalLock() );
        
        if (global_null.constData() == 0)
            global_null = new NullProperty();
    }
    
    return *(global_null.constData());
}

QString NullProperty::toString() const
{
    return QObject::tr("NULL");
}

///////////////
/////////////// Implementation of PropPtrBase
///////////////

static const RegisterMetaType<PropPtrBase> r_propptr( MAGIC_ONLY, NO_ROOT,
                                                      "SireBase::PropPtrBase" );

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, 
                                        const PropPtrBase &propptr)
{
    writeHeader(ds, r_propptr, 1);

    SharedDataStream sds(ds);

    sds << propptr.ptr;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, PropPtrBase &propptr)
{
    VersionID v = readHeader(ds, r_propptr);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> propptr.ptr;
    }
    else
        throw version_error(v, "1", r_propptr, CODELOC);

    return ds;
}

/** Construct to hold a pointer to 'property' */
PropPtrBase::PropPtrBase(const Property &property)
                : ptr(property)
{}

/** Construct to hold a pointer to 'property' - this takes over
    ownership of the pointer */
PropPtrBase::PropPtrBase(Property *property)
            : ptr(property)
{}

/** Copy constructor */
PropPtrBase::PropPtrBase(const PropPtrBase &other)
                : ptr(other.ptr)
{}

/** Destructor */
PropPtrBase::~PropPtrBase()
{}

/** Copy assignment operator */
PropPtrBase& PropPtrBase::operator=(const PropPtrBase &other)
{
    ptr = other.ptr;
    return *this;
}

/** Comparison operator */
bool PropPtrBase::operator==(const PropPtrBase &other) const
{
    return ptr.constData() == other.ptr.constData() or
           ptr->equals(*(other.ptr));
}

/** Comparison operator */
bool PropPtrBase::operator!=(const PropPtrBase &other) const
{
    return ptr.constData() != other.ptr.constData() and
           not ptr->equals(*(other.ptr));
}

/** Comparison operator */
bool PropPtrBase::operator==(const Property &other) const
{
    return ptr.constData() == &other or
           ptr->equals(other);
}

/** Comparison operator */
bool PropPtrBase::operator!=(const Property &other) const
{
    return ptr.constData() != &other and
           not ptr->equals(other);
}

/** Is this a unique pointer to the object? */
bool PropPtrBase::unique() const
{
    return ptr.unique();
}

/** Detach this pointer from shared storage */
void PropPtrBase::detach()
{
    ptr.detach();
}

/** Allow automatic casting to a Property */
PropPtrBase::operator const Property&() const
{
    return *ptr;
}

/** Return a read-only reference to the object */
const Property& PropPtrBase::read() const
{
    BOOST_ASSERT( ptr.constData() != 0 );
    return *ptr;
}

/** Return a writable reference to the object. This performs
    a copy-on-write test and action */
Property& PropPtrBase::edit()
{
    BOOST_ASSERT( ptr.constData() != 0 );
    return *ptr;
}

/** Return a writable reference to the object. This performs
    a copy-on-write test and action - this is a synonym for PropPtr::edit */
Property& PropPtrBase::write()
{
    return PropPtrBase::edit();
}

/** Throw an error as we can't cast 'got_type' into 'want_type'

    \throw SireError::invalid_cast
*/
void PropPtrBase::throwCastingError(const char *got_type, const char *want_type)
{
    throw SireError::invalid_cast( QObject::tr(
        "Cannot cast from a %1 into a %2.")
            .arg(got_type).arg(want_type), CODELOC );
}

///////////////
/////////////// Implementation of GlobalPropPtrBase
///////////////

static const RegisterMetaType<GlobalPropPtrBase> r_globalpropptr( MAGIC_ONLY, NO_ROOT,
                                                      "SireBase::GlobalPropPtrBase" );

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, 
                                        const GlobalPropPtrBase &propptr)
{
    writeHeader(ds, r_globalpropptr, 1);

    SharedDataStream sds(ds);

    sds << propptr.ptr;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, GlobalPropPtrBase &propptr)
{
    VersionID v = readHeader(ds, r_globalpropptr);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> propptr.ptr;
    }
    else
        throw version_error(v, "1", r_globalpropptr, CODELOC);

    return ds;
}

/** Construct to hold a pointer to 'property' */
GlobalPropPtrBase::GlobalPropPtrBase(const Property &property)
                  : ptr(property)
{}

/** Construct to hold a pointer to 'property' - this takes over
    ownership of the pointer */
GlobalPropPtrBase::GlobalPropPtrBase(Property *property)
                  : ptr(property)
{}

/** Copy constructor */
GlobalPropPtrBase::GlobalPropPtrBase(const GlobalPropPtrBase &other)
                  : ptr(other.ptr)
{}

/** Destructor */
GlobalPropPtrBase::~GlobalPropPtrBase()
{}

/** Copy assignment operator */
GlobalPropPtrBase& GlobalPropPtrBase::operator=(const GlobalPropPtrBase &other)
{
    ptr = other.ptr;
    return *this;
}

/** Comparison operator */
bool GlobalPropPtrBase::operator==(const GlobalPropPtrBase &other) const
{
    return ptr.constData() == other.ptr.constData();
}

/** Comparison operator */
bool GlobalPropPtrBase::operator!=(const GlobalPropPtrBase &other) const
{
    return ptr.constData() != other.ptr.constData();
}

/** Comparison operator */
bool GlobalPropPtrBase::operator==(const Property &other) const
{
    return ptr.constData() == &other or
           ptr->equals(other);
}

/** Comparison operator */
bool GlobalPropPtrBase::operator!=(const Property &other) const
{
    return ptr.constData() != &other and
           not ptr->equals(other);
}

/** Is this a unique pointer to the object? */
bool GlobalPropPtrBase::unique() const
{
    return ptr.unique();
}

/** Allow automatic casting to a Property */
GlobalPropPtrBase::operator const Property&() const
{
    return *ptr;
}

/** Return a read-only reference to the object */
const Property& GlobalPropPtrBase::read() const
{
    BOOST_ASSERT( ptr.constData() != 0 );
    return *ptr;
}

/** Throw an error as we can't cast 'got_type' into 'want_type'

    \throw SireError::invalid_cast
*/
void GlobalPropPtrBase::throwCastingError(const char *got_type, const char *want_type)
{
    throw SireError::invalid_cast( QObject::tr(
        "Cannot cast from a %1 into a %2.")
            .arg(got_type).arg(want_type), CODELOC );
}

////////
//////// Full instantiation of PropPtr<Property>
////////

PropPtr<Property>::PropPtr() : PropPtrBase( Property::null() )
{}

PropPtr<Property>::PropPtr(const Property &property)
                  : PropPtrBase(property)
{}

PropPtr<Property>::PropPtr(Property *property)
                  : PropPtrBase(property)
{}

PropPtr<Property>::PropPtr(const PropPtrBase &other)
                  : PropPtrBase(other)
{}

PropPtr<Property>::PropPtr(const PropPtr<Property> &other)
                  : PropPtrBase(other)
{}

PropPtr<Property>::~PropPtr()
{}

PropPtr<Property>& PropPtr<Property>::operator=(const PropPtr<Property> &other)
{
    PropPtrBase::operator=(other);
    return *this;
}

PropPtr<Property>& PropPtr<Property>::operator=(const Property &property)
{
    return this->operator=( PropPtr<Property>(property) );
}

PropPtr<Property>& PropPtr<Property>::operator=(Property *property)
{
    return this->operator=( PropPtr<Property>(property) );
}

PropPtr<Property>& PropPtr<Property>::operator=(const PropPtrBase &property)
{
    return this->operator=( PropPtr<Property>(property) );
}

const Property* PropPtr<Property>::operator->() const
{
    return &(PropPtrBase::read());
}

const Property& PropPtr<Property>::operator*() const
{
    return PropPtrBase::read();
}

const Property& PropPtr<Property>::read() const
{
    return PropPtrBase::read();
}

Property& PropPtr<Property>::edit()
{
    return PropPtrBase::edit();
}

const Property* PropPtr<Property>::data() const
{
    return &(PropPtrBase::read());
}

const Property* PropPtr<Property>::constData() const
{
    return &(PropPtrBase::read());
}

Property* PropPtr<Property>::data()
{
    return &(PropPtrBase::edit());
}

PropPtr<Property>::operator const Property&() const
{
    return PropPtrBase::read();
}

bool PropPtr<Property>::isNull() const
{
    return PropPtrBase::operator==( Property::null() );
}

PropPtr<Property> PropPtr<Property>::null()
{
    return PropPtr<Property>();
}

////////
//////// Full instantiation of GlobalPropPtr<Property>
////////

GlobalPropPtr<Property>::GlobalPropPtr() : GlobalPropPtrBase( Property::null() )
{}

GlobalPropPtr<Property>::GlobalPropPtr(const Property &property)
                        : GlobalPropPtrBase(property)
{}

GlobalPropPtr<Property>::GlobalPropPtr(Property *property)
                        : GlobalPropPtrBase(property)
{}

GlobalPropPtr<Property>::GlobalPropPtr(const GlobalPropPtrBase &other)
                        : GlobalPropPtrBase(other)
{}

GlobalPropPtr<Property>::GlobalPropPtr(const GlobalPropPtr<Property> &other)
                        : GlobalPropPtrBase(other)
{}

GlobalPropPtr<Property>::~GlobalPropPtr()
{}

GlobalPropPtr<Property>& GlobalPropPtr<Property>::operator=(
                                            const GlobalPropPtr<Property> &other)
{
    GlobalPropPtrBase::operator=(other);
    return *this;
}

GlobalPropPtr<Property>& GlobalPropPtr<Property>::operator=(const Property &property)
{
    return this->operator=( GlobalPropPtr<Property>(property) );
}

GlobalPropPtr<Property>& GlobalPropPtr<Property>::operator=(Property *property)
{
    return this->operator=( GlobalPropPtr<Property>(property) );
}

GlobalPropPtr<Property>& GlobalPropPtr<Property>::operator=(
                                                   const GlobalPropPtrBase &property)
{
    return this->operator=( GlobalPropPtr<Property>(property) );
}

const Property* GlobalPropPtr<Property>::operator->() const
{
    return &(GlobalPropPtrBase::read());
}

const Property& GlobalPropPtr<Property>::operator*() const
{
    return GlobalPropPtrBase::read();
}

const Property& GlobalPropPtr<Property>::read() const
{
    return GlobalPropPtrBase::read();
}

const Property* GlobalPropPtr<Property>::data() const
{
    return &(GlobalPropPtrBase::read());
}

const Property* GlobalPropPtr<Property>::constData() const
{
    return &(GlobalPropPtrBase::read());
}

GlobalPropPtr<Property>::operator const Property&() const
{
    return GlobalPropPtrBase::read();
}

bool GlobalPropPtr<Property>::isNull() const
{
    return GlobalPropPtrBase::operator==( Property::null() );
}

GlobalPropPtr<Property> GlobalPropPtr<Property>::null()
{
    return GlobalPropPtr<Property>();
}
