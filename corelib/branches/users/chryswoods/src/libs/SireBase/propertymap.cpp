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

#include "propertymap.h"

#include "Siren/objref.h"
#include "Siren/stream.h"
#include "Siren/streamqt.h"

#include <QDebug>

using namespace SireBase;
using namespace Siren;

////////////
//////////// Implementation of PropertyName
////////////

static const RegisterObject<PropertyName> r_propname;

/** Null constructor */
PropertyName::PropertyName() 
             : Implements<PropertyName,Object>(), value_is_default(false)
{}

/** Construct a PropertyName that searches for the
    property using the source 'source' */
PropertyName::PropertyName(const char *source)
             : Implements<PropertyName,Object>(), src(source), value_is_default(false)
{}

/** Construct a PropertyName that searches for the 
    property using the source 'source' */
PropertyName::PropertyName(const QString &source)
             : Implements<PropertyName,Object>(), src(source), value_is_default(false)
{}

/** Construct a PropertyName that uses the supplied
    value, rather than searching for the property */
PropertyName::PropertyName(const Object &value)
             : Implements<PropertyName,Object>(), val(value)
{}

/** Construct a PropertyName that searches for the property
    using the source 'source', but only if that source is
    specifically provided - otherwise the supplied default
    value of the property is used instead */
PropertyName::PropertyName(const QString &source, 
                           const Object &default_value)
             : Implements<PropertyName,Object>(),
               src(source), val(default_value), value_is_default(true)
{
    BOOST_ASSERT(not source.isEmpty());
}

/** Copy constructor */
PropertyName::PropertyName(const PropertyName &other)
             : Implements<PropertyName,Object>(other),
               src(other.src), val(other.val), value_is_default(other.value_is_default)
{}

/** Destructor */
PropertyName::~PropertyName()
{}

/** Copy assignment operator */
PropertyName& PropertyName::operator=(const PropertyName &other)
{
    src = other.src;
    val = other.val;
    value_is_default = other.value_is_default;
    
    return *this;
}

/** Comparison operator */
bool PropertyName::operator==(const PropertyName &other) const
{
    return src == other.src and val == other.val and
           value_is_default == other.value_is_default;
}

/** Comparison operator */
bool PropertyName::operator!=(const PropertyName &other) const
{
    return src != other.src or val != other.val or 
           value_is_default != other.value_is_default;
}

/** Return a PropertyName that says that this property is not set */
PropertyName PropertyName::none()
{
    return PropertyName();
}

/** Return whether or not the source has been set */
bool PropertyName::hasSource() const
{
    return not src.isEmpty();
}

/** Return whether or not the value has been set */
bool PropertyName::hasValue() const
{
    return not val.isNull();
}

/** Return whether or not this has a default value */
bool PropertyName::hasDefaultValue() const
{
    return value_is_default;
}

/** Return whether this property is null */
bool PropertyName::isNull() const
{
    return src.isEmpty() and val.isNull();
}

/** Return the source of the property - this is only valid
    if .hasSource() is true */
const QString& PropertyName::source() const
{
    return src;
}

static None none;

/** Return the value of the property - this is only valid
    if .hasValue() is true */
const Object& PropertyName::value() const
{
    if (val.isNull())
        return ::none;
    else
        return *val;
}

/** Return a string representation of this propertyname */
QString PropertyName::toString() const
{
    if (this->hasSource())
    {
        if (value_is_default)
            return QString("%1 {default: %2}").arg(src).arg(val->what());
        else
            return src;
    }
    else if (this->hasValue())
        return val->what();
    else
        return "NULL";
}

uint PropertyName::hashCode() const
{
    return qHash(PropertyName::typeName()) + qHash(src);
}

void PropertyName::stream(Stream &s)
{
    s.assertVersion<PropertyName>(1);
    
    Schema schema = s.item<PropertyName>();
    
    schema.data("source") & src;
    schema.data("value") & val;
    schema.data("value_is_default") & value_is_default;
    
    Object::stream( schema.base() );
}

////////////
//////////// Implementation of PropertyMap
////////////

static const RegisterObject<PropertyMap> r_propmap;

/** Null constructor */
PropertyMap::PropertyMap() : Implements<PropertyMap,Object>()
{}

/** Construct a map that holds just a single PropertyName */
PropertyMap::PropertyMap(const QString &property, const PropertyName &propname)
            : Implements<PropertyMap,Object>()
{
    propmap.insert(property, propname);
}

/** Construct a map that holds lots of PropertyNames */
PropertyMap::PropertyMap(const QHash<QString,PropertyName> &propnames)
            : Implements<PropertyMap,Object>(), propmap(propnames)
{}

/** Copy constructor */
PropertyMap::PropertyMap(const PropertyMap &other)
            : Implements<PropertyMap,Object>(other), propmap(other.propmap)
{}

/** Destructor */
PropertyMap::~PropertyMap()
{}

/** Copy assignment operator */
PropertyMap& PropertyMap::operator=(const PropertyMap &other)
{
    propmap = other.propmap;
    return *this;
}

/** Add two PropertyMaps together. This copies the properties of
    other and adds them to this, replacing any in this that 
    have the same name */
PropertyMap PropertyMap::operator+(const PropertyMap &other) const
{
    PropertyMap ret(*this);
    
    for (QHash<QString,PropertyName>::const_iterator it = other.propmap.begin();
         it != other.propmap.end();
         ++it)
    {
        ret.propmap.insert(it.key(), it.value());
    }
    
    return ret;
}

/** Comparison operator */
bool PropertyMap::operator==(const PropertyMap &other) const
{
    return propmap == other.propmap;
}

/** Comparison operator */
bool PropertyMap::operator!=(const PropertyMap &other) const
{
    return propmap != other.propmap;
}

/** Map the property called 'name' to the source or value of 
    that property */
PropertyName PropertyMap::operator[](const QString &name) const
{
    QHash<QString,PropertyName>::const_iterator
                                    it = propmap.constFind(name);
                                    
    if (it == propmap.constEnd())
    {
        return PropertyName(name);
    }
    else
    {
        return it.value();
    }
}

/** Map the property called 'name' to the source or value of 
    that property */
PropertyName PropertyMap::operator[](const char *name) const
{
    return this->operator[](QLatin1String(name));
}

/** Map the property in 'propname' to the source or value of
    that property. */
PropertyName PropertyMap::operator[](const PropertyName &propname) const
{
    if (propname.hasSource())
    {
        QHash<QString,PropertyName>::const_iterator 
                                        it = propmap.constFind(propname.source());
                                        
        if (it != propmap.constEnd())
            return it.value();
        else
            return propname;
    }
    else
        return propname;
}

/** Return whether or not this map is default - if it is,
    then it doesn't specify any properties */
bool PropertyMap::isDefault() const
{
    return propmap.isEmpty();
}

/** Return whether or not this map specifies the source or value
    of the property called 'name' */
bool PropertyMap::specified(const QString &name) const
{
    return propmap.contains(name);
}

/** Return whether or not this map specifies the source or value
    of the property called 'name' */
bool PropertyMap::specified(const char *name) const
{
    return propmap.contains(name);
}

/** Return whether or not this map specifies the source or value
    of the property called 'name' */
bool PropertyMap::specified(const PropertyName &propname) const
{
    if (propname.hasSource())
        return propmap.contains(propname.source());
    else
        return propname.hasValue();
}

/** Set the property called 'name' to have the source or value
    in 'source'. This replaces any existing source or value
    for any existing property of this name in this map */
void PropertyMap::set(const QString &name, const PropertyName &source)
{
    propmap.insert(name, source);
}

/** Return a string representation of this PropertyMap */
QString PropertyMap::toString() const
{
    QStringList items;
    
    for (QHash<QString,PropertyName>::const_iterator it = propmap.constBegin();
         it != propmap.constEnd();
         ++it)
    {
        items.append( QString("%1 == %2").arg( it.key(), it.value().toString() ) );
    }
    
    return QString("[ %1 ]").arg( items.join(", ") );
}

uint PropertyMap::hashCode() const
{
    return qHash( PropertyMap::typeName() ) + qHash( propmap.count() );
}

void PropertyMap::stream(Stream &s)
{
    s.assertVersion<PropertyMap>(1);
    
    Schema schema = s.item<PropertyMap>();
    
    schema.data("map") & propmap;
    
    Object::stream( schema.base() );
}
