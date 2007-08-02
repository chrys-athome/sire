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
bool PropertyBase::operator==(const PropertyBase &other) const
{
    return true;
}

/** Comparison operator */
bool PropertyBase::operator!=(const PropertyBase &other) const
{
    return false;
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

static SharedPolyPointer<PropertyBase> shared_null( new NullProperty() );

/** Return a shared null property */
Property PropertyBase::null_property()
{
    return Property(shared_null);
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
                : PropertyBase(), QVariant()
{}

/** Construct a property equal to 'value' */
VariantProperty::VariantProperty(const QVariant &value)
                : PropertyBase(), QVariant(value)
{}

/** Construct from a 'Property' - the property must be able to
    be cast to a VariantProperty

    \throw SireError::invalid_cast
*/
VariantProperty::VariantProperty(const Property &property)
                : PropertyBase(), QVariant()
{
    *this = property;
}

/** Copy constructor */
VariantProperty::VariantProperty(const VariantProperty &other)
                : PropertyBase(other), QVariant(other)
{}

/** Destructor */
VariantProperty::~VariantProperty()
{}

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

/** Assignment from a Property

    \throw SireError::invalid_cast
*/
VariantProperty& VariantProperty::operator=(const Property &other)
{
    if (not other.isA<VariantProperty>())
        throw SireError::invalid_cast( QObject::tr(
                "Cannot cast from a \"%1\" to a VariantProperty!")
                    .arg(other.what()), CODELOC );

    return this->operator=(other.asA<VariantProperty>());
}

/** Comparison function */
bool VariantProperty::_pvt_isEqual(const PropertyBase &other) const
{
    BOOST_ASSERT(other.isA<VariantProperty>());

    return QVariant::operator==( other.asA<VariantProperty>() );
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
    sds << property.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, Property &property)
{
    VersionID v = readHeader(ds, r_prop);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> property.d;
    }
    else
        throw version_error(v, "1", r_prop, CODELOC);

    return ds;
}

/** Null constructor - construct a null property */
Property::Property() : d( PropertyBase::null_property().d )
{}

/** Construct from the passed pointer - this must not be null! This
    takes over ownership of the passed pointer and may delete it
    whenever it wishes.

    \throw SireError::nullptr_error
*/
Property::Property(PropertyBase *ptr) : d(ptr)
{
    if (not ptr)
        throw SireError::nullptr_error( QObject::tr(
                  "Cannot construct a Property from a null pointer!"), CODELOC );
}

/** Construct from the passed property */
Property::Property(const PropertyBase &property)
         : d(property)
{}

/** Construct from a QVariant */
Property::Property(const QVariant &value)
         : d(new VariantProperty(value))
{}

/** Copy constructor */
Property::Property(const Property &other)
         : d(other.d)
{}

/** Destructor */
Property::~Property()
{}

/** Copy assignment */
Property& Property::operator=(const Property &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool Property::operator==(const Property &other) const
{
    return (d == other.d) and *d == *(other.d);
}

/** Comparison operator */
bool Property::operator!=(const Property &other) const
{
    return (d != other.d) or *d != *(other.d);
}
