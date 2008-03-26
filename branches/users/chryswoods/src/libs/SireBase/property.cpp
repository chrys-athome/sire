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

#include "property.h"

#include <QDebug>

#include "SireError/errors.h"
#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

///////////////
/////////////// Implementation of NullProperty
///////////////

namespace SireBase
{

/** This is a null property */
class SIREBASE_EXPORT NullProperty
              : public ConcreteProperty<NullProperty,PropertyBase>
{
public:
    NullProperty() : ConcreteProperty<NullProperty,PropertyBase>()
    {}

    NullProperty(const NullProperty &other)
            : ConcreteProperty<NullProperty,PropertyBase>(other)
    {}

    ~NullProperty()
    {}

    static const char* typeName()
    {
        return "SireBase::NullProperty";
    }
};

}

using namespace SireBase;
using namespace SireBase;
using namespace SireStream;

///////////////
/////////////// Implementation of PropertyBase
///////////////

/** Constructor */
PropertyBase::PropertyBase() : QSharedData()
{}

/** Copy constructor */
PropertyBase::PropertyBase(const PropertyBase&) : QSharedData()
{}

/** Destructor */
PropertyBase::~PropertyBase()
{}

/** Assignment operator */
PropertyBase& PropertyBase::operator=(const PropertyBase&)
{
    return *this;
}

/** Comparison operator */
bool PropertyBase::operator==(const PropertyBase&) const
{
    return true;
}

/** Comparison operator */
bool PropertyBase::operator!=(const PropertyBase&) const
{
    return false;
}

/** Assignment operator */
PropertyBase& PropertyBase::operator=(const Property &property)
{
    return this->operator=(*property);
}

/** Comparison operator */
bool PropertyBase::operator==(const Property &property) const
{
    return this->operator==(*property);
}

/** Comparison operator */
bool PropertyBase::operator!=(const Property &property) const
{
    return this->operator!=(*property);
}

/** Throw an invalid cast!

    \throw SireError::invalid_cast
*/
void PropertyBase::throwInvalidCast(const PropertyBase &other) const
{
    throw SireError::invalid_cast( QObject::tr(
            "Cannot cast from an object of class \"%1\" to an object "
            "of class \"%2\".")
                .arg(other.what()).arg(this->what()), CODELOC );
}

/** Throw an invalid cast!

    \throw SireError::invalid_cast
*/
void PropertyBase::throwInvalidCast(const char *typenam) const
{
    throw SireError::invalid_cast( QObject::tr(
            "Cannot cast from an object of class \"%1\" to an object "
            "of class \"%2\".")
                .arg(typenam).arg(this->what()), CODELOC );
}

static const RegisterMetaType<PropertyBase> r_propbase(MAGIC_ONLY,
                                                       "SireBase::PropertyBase");

/** Serialise to a binary data stream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const PropertyBase&)
{
    writeHeader(ds, r_propbase, 0);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, PropertyBase&)
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
          << static_cast<const PropertyBase&>(varprop)
          << static_cast<const QVariant&>(varprop);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, VariantProperty &varprop)
{
    VersionID v = readHeader(ds, r_varprop);

    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(varprop)
           >> static_cast<QVariant&>(varprop);
    }
    else
        throw version_error(v, "1", r_varprop, CODELOC);

    return ds;
}

/** Null constructor */
VariantProperty::VariantProperty()
                : ConcreteProperty<VariantProperty,PropertyBase>(), QVariant()
{}

/** Construct a property equal to 'value' */
VariantProperty::VariantProperty(const QVariant &value)
                : ConcreteProperty<VariantProperty,PropertyBase>(), QVariant(value)
{}

/** Construct from a 'Property' - the property must be able to
    be cast to a VariantProperty

    \throw SireError::invalid_cast
*/
VariantProperty::VariantProperty(const Property &property)
                : ConcreteProperty<VariantProperty,PropertyBase>(), QVariant()
{
    *this = property;
}

/** Copy constructor */
VariantProperty::VariantProperty(const VariantProperty &other)
                : ConcreteProperty<VariantProperty,PropertyBase>(other), QVariant(other)
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
    PropertyBase::operator=(other);

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

///////////////
/////////////// Implementation of Property
///////////////

static const RegisterMetaType<Property> r_prop;

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const Property &property)
{
    writeHeader(ds, r_prop, 1);

    SharedDataStream sds(ds);
    sds << property.ptr;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, Property &property)
{
    VersionID v = readHeader(ds, r_prop);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> property.ptr;
    }
    else
        throw version_error(v, "1", r_prop, CODELOC);

    return ds;
}

static SharedPolyPointer<PropertyBase> shared_null( new NullProperty() );

/** Null constructor - constructs a null property */
Property::Property() : ptr(shared_null)
{}

/** Construct from the passed property */
Property::Property(const PropertyBase &property)
         : ptr(property)
{}

/** Copy constructor */
Property::Property(const Property &other)
         : ptr(other.ptr)
{}

/** Destructor */
Property::~Property()
{}

/** Return whether this is a null property */
bool Property::isNull() const
{
    return ptr->isA<NullProperty>();
}

/** Copy assignment operator */
Property& Property::operator=(const PropertyBase &property)
{
    ptr = property;
    return *this;
}

/** Comparison operator */
bool Property::operator==(const Property &other) const
{
    return ptr == other.ptr or *ptr == *(other.ptr);
}

/** Comparison operator */
bool Property::operator!=(const Property &other) const
{
    return ptr != other.ptr and *ptr != *(other.ptr);
}

/** Return a const reference to the object */
const PropertyBase& Property::d() const
{
    return *ptr;
}

/** Return a modifiable reference to the object */
PropertyBase& Property::d()
{
    return *ptr;
}

/** Dereferencing operator */
const PropertyBase* Property::operator->() const
{
    return &(d());
}

/** Dereferencing operator */
const PropertyBase& Property::operator*() const
{
    return d();
}

/** Return a const reference to this property */
const PropertyBase& Property::read() const
{
    return d();
}

/** Return an editable reference to the property */
PropertyBase& Property::edit()
{
    return d();
}

/** Return a const pointer to the property */
const PropertyBase* Property::data() const
{
    return &(d());
}

/** Return a const pointer to the property */
const PropertyBase* Property::constData() const
{
    return &(d());
}

/** Return a modifiable pointer to the property */
PropertyBase* Property::data()
{
    return &(d());
}

/** Detach this property from shared storage */
void Property::detach()
{
    ptr.detach();
}

/** Allow implicit casting to a PropertyBase object */
Property::operator const PropertyBase&() const
{
    return d();
}
