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

#include <QHash>

#include "properties.h"

#include "SireBase/errors.h"
#include "Siren/errors.h"

#include "Siren/objptr.hpp"
#include "Siren/objref.h"
#include "Siren/stream.h"
#include "Siren/streamqt.h"

#include <QDebug>

using namespace SireBase;
using namespace Siren;

namespace Siren { namespace detail {

template<>
struct StreamHelper<Properties>
{
    static QString typeName()
    {
        return Properties::typeName();
    }
    
    static const void* getKey(const Properties &object)
    {
        return object.shareKey();
    }
    
    static Properties null()
    {
        return Properties();
    }
};

}}

namespace SireBase
{

namespace detail
{

/** This is the data for a Properties object */
class PropertiesData : public QSharedData
{
public:
    PropertiesData();
    PropertiesData(const PropertiesData &other);

    ~PropertiesData();

    PropertiesData& operator=(const PropertiesData &other);

    bool operator==(const PropertiesData &other) const;
    bool operator!=(const PropertiesData &other) const;

    /** The metadata for this set of properties itself */
    Properties metadata;

    /** The collection of properties, indexed by name */
    QHash<QString, ObjectPtr> properties;

    /** The metadata for each property, indexed by name */
    QHash<QString, Properties> props_metadata;

    static const QSharedDataPointer<PropertiesData>& getNullData();

private:
    /** Constructor used only once to create the global-null PropertiesData */
    PropertiesData(bool) : metadata(false)
    {}
};

} // namespace detail

} // namespace SireBase

using namespace SireBase::detail;

static QSharedDataPointer<PropertiesData> nulldata_ptr;

/** Return a shared pointer to the null object - this is not
    as trivial as it sounds, as there is a circular reference! */
const QSharedDataPointer<PropertiesData>& PropertiesData::getNullData()
{
    if (nulldata_ptr.constData() == 0)
    {
        QMutexLocker lkr( &(Siren::globalRegistrationLock()) );
        
        if (nulldata_ptr.constData() != 0)
            return nulldata_ptr;
    
        //use a constructor where the circular reference is broken
        PropertiesData *ptr = new PropertiesData(false);
        
        nulldata_ptr = ptr;

        //fix the reference
        ptr->metadata = Properties();
    }

    return nulldata_ptr;
}

/** Null constructor */
PropertiesData::PropertiesData() : QSharedData()
{}

/** Copy constructor */
PropertiesData::PropertiesData(const PropertiesData &other)
               : QSharedData(),
                 metadata(other.metadata), properties(other.properties),
                 props_metadata(other.props_metadata)
{}

/** Destructor */
PropertiesData::~PropertiesData()
{}

/** Assignment operator */
PropertiesData& PropertiesData::operator=(const PropertiesData &other)
{
    if (this != &other)
    {
        metadata = other.metadata;
        properties = other.properties;
        props_metadata = other.props_metadata;
    }

    return *this;
}

/** Comparison operator */
bool PropertiesData::operator==(const PropertiesData &other) const
{
    return metadata == other.metadata and properties == other.properties 
                and props_metadata == other.props_metadata;
}

/** Comparison operator */
bool PropertiesData::operator!=(const PropertiesData &other) const
{
    return metadata != other.metadata or properties != other.properties 
                or props_metadata != other.props_metadata;
}

/////////////
///////////// Implementation of Properties
/////////////

static const RegisterObject<Properties> r_props;

/** Private constructor used to avoid the problem of the circular reference! */
Properties::Properties(bool) : Implements<Properties,Object>(), d(0)
{}

/** Null constructor - construct an empty set of properties */
Properties::Properties() : Implements<Properties,Object>(),
                           d( PropertiesData::getNullData() )
{}

/** Copy constructor */
Properties::Properties(const Properties &other) 
           : Implements<Properties,Object>(other), d(other.d)
{}

/** Destructor */
Properties::~Properties()
{}

/** Assignment operator */
Properties& Properties::operator=(const Properties &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool Properties::operator==(const Properties &other) const
{
    return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool Properties::operator!=(const Properties &other) const
{
    return d != other.d and *d != *(other.d);
}

const void* Properties::shareKey() const
{
    return d.constData();
}

uint Properties::hashCode() const
{
    return qHash(Properties::typeName());
}

QString Properties::toString() const
{
    return Properties::typeName();
}

void Properties::stream(Stream &s)
{
    s.assertVersion<Properties>(1);
    
    SharedSchema shared = s.shared(*this);
    
    if (shared.mustStream())
    {
        PropertiesData *p;
        
        if (s.isSaving())
            p = const_cast<PropertiesData*>(d.constData());
        else
            p = d.data();
            
        shared.data("metadata") & p->metadata;
        shared.data("properties") & p->properties;
        shared.data("property_metadata") & p->props_metadata;
        
        Object::stream( shared.base() );
    }
}

/** Return whether this is empty (has no values) */
bool Properties::isEmpty() const
{
    return d->properties.isEmpty();
}

/** Return the keys for all of the properties in this set */
QStringList Properties::propertyKeys() const
{
    return d->properties.keys();
}

/** Return an iterator pointing to the first property in this set */
Properties::const_iterator Properties::begin() const
{
    return d->properties.begin();
}

/** Return an iterator pointing to the first property in this set */
Properties::const_iterator Properties::constBegin() const
{
    return d->properties.constBegin();
}

/** Return an iterator pointing to the property with key 'key', or
    Properties::end() if there is no such property */
Properties::const_iterator Properties::find(const QString &key) const
{
    return d->properties.find(key);
}

/** Return an iterator pointing to the property with key 'key', or
    Properties::end() if there is no such property */
Properties::const_iterator Properties::constFind(const QString &key) const
{
    return d->properties.constFind(key);
}

/** Return an iterator pointing one beyond the last property in this set */
Properties::const_iterator Properties::end() const
{
    return d->properties.end();
}

/** Return an iterator pointing one beyond the last property in this set */
Properties::const_iterator Properties::constEnd() const
{
    return d->properties.end();
}

/** Assert that this set contains a property with key 'key'

    \throw SireBase::missing_property
*/
void Properties::assertContainsProperty(const PropertyName &key) const
{
    if (key.hasSource() and not d->properties.contains(key.source())
                        and not key.hasDefaultValue())
    {
        throw SireBase::missing_property( QObject::tr(
            "There is no property with key \"%1\". Available keys are ( %2 ).")
                .arg(key.source(), this->propertyKeys().join(", ")), CODELOC );
    }
}

/** Return the list of metadata keys */
QStringList Properties::metadataKeys() const
{
    return d->metadata.propertyKeys();
}

/** Assert that this contains the metadata at metakey 'metakey' */
void Properties::assertContainsMetadata(const PropertyName &metakey) const
{
    return d->metadata.assertContainsProperty(metakey);
}

/** Return the list of metadata keys for the property with key 'key'

    \throw SireBase::missing_property
*/
QStringList Properties::metadataKeys(const PropertyName &key) const
{
    return this->allMetadata(key).propertyKeys();
}

/** Assert that this set contains the metadata property for the
    property 'key' with metakey 'metakey'
    
    \throw SireBase::missing_property
*/
void Properties::assertContainsMetadata(const PropertyName &key, 
                                        const PropertyName &metakey) const
{
    this->assertContainsProperty(key);
    
    if (metakey.hasSource() and 
        not this->allMetadata(key).hasProperty(metakey))
    {
        throw SireBase::missing_property( QObject::tr(
            "There is no metadata with metakey \"%1\" for the property \"%2\". "
            "Available metakeys are ( %3 ).")
                .arg(metakey.source(), 
                     key.toString(), metadataKeys(key).join(", ")), CODELOC );
    }
}

/** Return whether or not this contains a property with key 'key' */
bool Properties::hasProperty(const PropertyName &key) const
{
    return key.hasValue() or d->properties.contains(key.source())
                          or key.hasDefaultValue();
}

/** Return whether or not this contains the metadata with metakey 'metakey' */
bool Properties::hasMetadata(const PropertyName &metakey) const
{
    return d->metadata.hasProperty(metakey);
}

/** Return the property with key 'key'

    \throw SireBase::missing_property
*/
const Object& Properties::operator[](const PropertyName &key) const
{
    if (key.hasValue())
        return key.value();
    else
    {
        QHash<QString,ObjectPtr>::const_iterator 
                            it = d->properties.constFind(key.source());

        if (it == d->properties.constEnd())
        {
            if (key.hasDefaultValue())
                return key.value();
            else
                throw SireBase::missing_property( QObject::tr(
                    "There is no property with name \"%1\". "
                    "Available properties are [ %2 ].")
                        .arg(key.source(), this->propertyKeys().join(", ")), CODELOC );
        }

        return *it;
    }
}

/** Return all of the metadata associated with this properties object */
const Properties& Properties::allMetadata() const
{
    return d->metadata;
}

Q_GLOBAL_STATIC( Properties, nullProperties )

/** Return the metadata for the property with key 'key'

    \throw SireBase::missing_property
*/
const Properties& Properties::allMetadata(const PropertyName &key) const
{
    if (key.hasValue())
        return *nullProperties();
    else
    {
        QHash<QString,Properties>::const_iterator 
                                it = d->props_metadata.constFind(key.source());

        if (it == d->props_metadata.constEnd())
        {
            if (key.hasDefaultValue())  
                return *nullProperties();
            else
                throw SireBase::missing_property( QObject::tr(
                    "There is no property with name \"%1\". "
                    "Available properties are [ %2 ].")
                        .arg(key.source(), propertyKeys().join(", ")), CODELOC );
        }

        return *it;
    }
}

/** Return whether or not the property with key 'key' contains some
    metadata with the metakey 'metakey'
    
    \throw SireBase::missing_property
*/
bool Properties::hasMetadata(const PropertyName &key, 
                             const PropertyName &metakey) const
{
    this->assertContainsProperty(key);
    
    return metakey.hasValue() or 
           this->allMetadata(key).hasProperty(metakey);
}

/** Return the property with key 'key' - note that if 
    'key' specifies a value rather than a source, then the
    value contained in the key is returned

    \throw SireBase::missing_property
*/
const Object& Properties::property(const PropertyName &key) const
{
    return this->operator[](key);
}

/** Return the property with key 'key' - note that if 
    'key' specifies a value rather than a source, then the
    value contained in the key is returned. If no such source
    exists, and there is no value in the key, then 
    'default_value' is returned */
const Object& Properties::property(const PropertyName &key,
                                   const Object &default_value) const
{
    if (key.hasValue())
    {
        return key.value();
    }
    else
    {
        QHash<QString,Properties>::const_iterator 
                                it = d->props_metadata.constFind(key.source());

        if (it != d->props_metadata.constEnd())
            return *it;
        else
            return default_value;
    }
}

/** Return the metadata at metakey 'metakey' - note that if 'metakey'
    specifies a value rather than a source, then the value contained
    in the metakey is returned.
    
    \throw SireBase::missing_property
*/
const Object& Properties::metadata(const PropertyName &metakey) const
{
    return d->metadata.property(metakey);
}

/** Return the metadata at metakey 'metakey' - note that if 'metakey'
    specifies a value rather than a source, then the value contained
    in the metakey is returned. If there is no such metadata, and no
    value is contained in the metakey, then 'default_value' is 
    returned */
const Object& Properties::metadata(const PropertyName &metakey,
                                   const Object &default_value) const
{
    return d->metadata.property(metakey, default_value);
}

/** Return the metadata at metakey 'metakey' that is associated with
    the property at key 'key'.
    
    \throw SireBase::missing_property
*/
const Object& Properties::metadata(const PropertyName &key,
                                   const PropertyName &metakey) const
{
    return this->allMetadata(key).property(metakey);
}

/** Return the metadata at metakey 'metakey' that is associated with
    the property at key 'key', or 'default_value' if there is no
    such metadata.
*/
const Object& Properties::metadata(const PropertyName &key,
                                   const PropertyName &metakey,
                                   const Object &default_value) const
{
    return this->allMetadata(key).property(metakey,default_value);
}

/** Set the property at key 'key' to have the value 'value'. This 
    replaces any existing property at this key, and removes any
    existing metadata is 'clear_metadata' is true */
void Properties::setProperty(const QString &key, const Object &value,
                             bool clear_metadata)
{
    if (key.isEmpty())
        throw Siren::invalid_arg( QObject::tr(
            "You cannot insert a property with an empty key!"), CODELOC );

    d->properties.insert(key, value);

    if (clear_metadata or not d->props_metadata.contains(key))
        d->props_metadata.insert(key, Properties());
}

void Properties::setProperty(const QString &key, const Object &value)
{
    this->setProperty(key, value, false);
}

/** Set the metadata at metakey 'metakey' to have the value 'value'.
    This replaces any existing metadata with this metakey */
void Properties::setMetadata(const QString &metakey, const Object &value)
{
    d->metadata.setProperty(metakey, value);
}

/** Set the metadata at metakey 'metakey' for the property at key 'key'.
    This replaces any existing metadata for this key/metakey pair */
void Properties::setMetadata(const QString &key, const QString &metakey,
                             const Object &value)
{
    this->assertContainsProperty(key);
    d->props_metadata.find(key)->setProperty(metakey, value);
}

/** Remove the property with key 'key' and all of its metadata */
void Properties::removeProperty(const QString &key)
{
    if (this->hasProperty(key))
    {
        d->properties.remove(key);
        d->props_metadata.remove(key);
    }
}

/** Remove the metadata at metakey 'metakey' */
void Properties::removeMetadata(const QString &metakey)
{
    if (this->hasMetadata(metakey))
    {
        d->metadata.removeProperty(metakey);
    }
}

/** Remove all of the top-level metadata */
void Properties::removeAllMetadata()
{
    d->metadata.clear();
}

/** Remove the metadata at metakey 'metakey' for the 
    property at key 'key' */
void Properties::removeMetadata(const QString &key, const QString &metakey)
{
    if (this->hasMetadata(key, metakey))
    {
        d->props_metadata.find(key)->removeProperty(metakey);
    }
}

/** Remove all of the metadata associated with the property at 
    key 'key' */
void Properties::removeAllMetadata(const QString &key)
{
    if (this->hasProperty(key))
        d->props_metadata.insert(key, Properties());
}

/** Completely clear this object of all properties and metadata */
void Properties::clear()
{
    this->operator=( Properties() );
}

/** Return the type name of the property at key 'key' 

    \throw SireBase::missing_property
*/
QString Properties::propertyType(const PropertyName &key) const
{
    return this->property(key).what();
}

/** Return the type name of the metadata at metakey 'metakey'

    \throw SireBase::missing_property
*/
QString Properties::metadataType(const PropertyName &metakey) const
{
    return this->metadata(metakey).what();
}

/** Return the type name of the metadata at metakey 'metakey'
    for the property at key 'key' 
    
    \throw SireBase::missing_property
*/
QString Properties::metadataType(const PropertyName &key,
                                 const PropertyName &metakey) const
{
    return this->metadata(key, metakey).what();
}
