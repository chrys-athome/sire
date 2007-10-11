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

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireBase;
using namespace SireStream;

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
    QHash<QString, Property> properties;

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
        //use a constructor where the circular reference is broken
        PropertiesData *ptr = new PropertiesData(false);
        
        nulldata_ptr = ptr;

        //fix the reference
        ptr->metadata = Properties();
    }

    return nulldata_ptr;
}

/** Serialise to a binary data stream */
QDataStream& operator<<(QDataStream &ds, const PropertiesData &props)
{
    SharedDataStream sds(ds);
    
    sds << props.metadata << props.properties << props.props_metadata;
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream& operator>>(QDataStream &ds, PropertiesData &props)
{
    SharedDataStream sds(ds);
    
    sds >> props.metadata >> props.properties >> props.props_metadata;
    
    return ds;
}

/** Null constructor */
PropertiesData::PropertiesData()
{}

/** Copy constructor */
PropertiesData::PropertiesData(const PropertiesData &other)
               : metadata(other.metadata), properties(other.properties),
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

static const RegisterMetaType<Properties> r_props;

/** Serialise to a binary data stream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const Properties &props)
{
    writeHeader(ds, r_props, 1);
    
    SharedDataStream sds(ds);
    
    sds << props.d
        << static_cast<const PropertyBase&>(props);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, Properties &props)
{
    VersionID v = readHeader(ds, r_props);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> props.d
            >> static_cast<PropertyBase&>(props);
    }
    else
        throw version_error(v, "1", r_props, CODELOC);
    
    return ds;
}

/** Private constructor used to avoid the problem of the circular reference! */
Properties::Properties(bool) : d(0)
{}

/** Null constructor - construct an empty set of properties */
Properties::Properties() : d( PropertiesData::getNullData() )
{}

/** Copy constructor */
Properties::Properties(const Properties &other) : d(other.d)
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

/** Return whether this is empty (has no values) */
bool Properties::isEmpty() const
{
    return d->properties.isEmpty();
}

/** Return the keys for all of the properties in this set */
QStringList Properties::keys() const
{
    return d->properties.keys();
}

/** Assert that this set contains a property with key 'key'

    \throw SireBase::missing_property
*/
void Properties::assertContainsProperty(const PropertyName &key) const
{
    if (key.hasSource() and not d->properties.contains(key))
    {
        throw SireBase::missing_property( QObject::tr(
            "There is no property with key \"%1\". Available keys are ( %2 ).")
                .arg(key, keys().join(", ")), CODELOC );
    }
}

/** Return the list of metadata keys for the property with key 'key'

    \throw SireBase::missing_property
*/
QStringList Properties::metadataKeys(const PropertyName &key) const
{
    return this->metadata(key).keys();
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
        not this->metadata(key).contains(metakey))
    {
        throw SireBase::missing_property( QObject::tr(
            "There is no metadata with metakey \"%1\" for the property \"%2\". "
            "Available metakeys are ( %3 ).")
                .arg(metakey, key, metadataKeys(key).join(", ")), CODELOC );
    }
}

/** Return whether or not this contains a property with key 'key' */
bool Properties::contains(const PropertyName &key) const
{
    return key.hasValue() or d->properties.contains(key);
}

/** Return whether or not the property with key 'key' contains some
    metadata with the metakey 'metakey'
    
    \throw SireBase::missing_property
*/
bool Properties::contains(const PropertyName &key, 
                          const PropertyName &metakey) const
{
    this->assertContainsProperty(key);
    
    return metakey.hasValue() or 
           this->metadata(key).contains(metakey);
}

/** Return the property with key 'key'

    \throw SireBase::missing_property
*/
const Property& Properties::operator[](const PropertyName &key) const
{
    if (key.hasValue())
        return key.value();
    else
    {
        QHash<QString,Property>::const_iterator it = d->properties.constFind(key);

        if (it == d->properties.constEnd())
        {
            throw SireBase::missing_property( QObject::tr(
                "There is no property with name \"%1\". "
                "Available properties are [ %2 ].")
                    .arg(key, keys().join(", ")), CODELOC );
        }

        return *it;
    }
}

/** Return the metadata associated with these properties */
const Properties& Properties::metadata() const
{
    return d->metadata;
}

static Properties null_properties;

/** Return the metadata for the property with key 'key'

    \throw SireBase::missing_property
*/
const Properties& Properties::metadata(const PropertyName &key) const
{
    if (key.hasValue())
        return null_properties;
    else
    {
        QHash<QString,Properties>::const_iterator it = d->props_metadata.constFind(key);

        if (it == d->props_metadata.constEnd())
        {
            throw SireBase::missing_property( QObject::tr(
                "There is no property with name \"%1\". "
                "Available properties are [ %2 ].")
                    .arg(key, keys().join(", ")), CODELOC );
        }

        return *it;
    }
}

/** Return the property with key 'key' - note that if 
    'key' specifies a value rather than a source, then the
    value contained in the key is returned

    \throw SireBase::missing_property
*/
const Property& Properties::value(const PropertyName &key) const
{
    return this->operator[](key);
}

/** Return the property with key 'key' - note that if 
    'key' specifies a value rather than a source, then the
    value contained in the key is returned. If no such source
    exists, and there is no value in the key, then 
    'default_value' is returned */
const Property& Properties::value(const PropertyName &key,
                                  const Property &default_value) const
{
    if (key.hasValue())
        return key.value();
    else
    {
        QHash<QString,Properties>::const_iterator it = d->props_metadata.constFind(key);

        if (it != d->props_metadata.constEnd())
            return it.value();
        else
            return default_value;
    }
}

/** Insert the properties 'properties' into this set - any properties
    with the same name are replaced */
void Properties::insert(const Properties &properties)
{
    if (this->isEmpty())
    {
        d = properties.d;
        return;
    }
    
    QHash<QString,Property> &props = d->properties;
    
    for (QHash<QString,Property>::const_iterator 
                          it = properties.d->properties.constBegin();
         it != properties.d->properties.constEnd();
         ++it)
    {
        props.insert(it.key(), it.value());
    }
    
    QHash<QString,Properties> &metaprops = d->props_metadata;
    
    for (QHash<QString,Properties>::const_iterator
                          it = properties.d->props_metadata.constBegin();
         it != properties.d->props_metadata.constEnd();
         ++it)
    {
        metaprops.insert(it.key(), it.value());
    }
}

/** Insert the property with key 'key' and value 'value' and metadata
    'metadata' into this set - any existing property with this key
    is replaced */
void Properties::insert(const QString &key, const Property &value,
                        const Properties &metadata)
{
    d->properties.insert(key, value);
    d->props_metadata.insert(key, metadata);
}

/** Insert the property with key 'key' and value 'value'
    into this set - any existing property with this key
    is replaced */
void Properties::insert(const QString &key, const Property &value)
{
    this->insert(key, value, Properties());
}

/** Update the value of the property with key 'key' to have value 
    'value'. If 'clear_metadata' is true, then all of the metadata
    for this property is removed. If there is no property with this
    key, then it is added, with no metadata.
*/
void Properties::update(const QString &key, const Property &value,
                        bool clear_metadata)
{
    if (this->contains(key))
    {
        d->properties.insert(key, value);
    
        if (clear_metadata)
            d->props_metadata.clear();
    }
    else
    {
        this->insert(key, value);
    }
}

/** Update the value of the property with key 'key' to have the 
    value 'value', and update the metadata as well to have the 
    values contained in 'metadata'. If 'clear_metadata' is true,
    then any existing metadata is first cleared before being updated
    (i.e. the metadata is set equal to 'metadata'). If there
    is no property with this key, then it is inserted, together
    with the supplied metadata.
*/
void Properties::update(const QString &key, const Property &value,
                        const Properties &metadata, bool clear_metadata)
{
    if (this->contains(key))
    {
        d->properties.insert(key, value);
    
        if (clear_metadata)
            d->props_metadata.insert(key, metadata);
        else
            d->props_metadata[key].insert(metadata);
    }
    else
    {
        this->insert(key, value, metadata);
    }
}

/** Insert the metadata for the property with key 'key' - this will
    replace any existing metadata with the same metakeys
    
    \throw SireBase::missing_property
*/
void Properties::insertMetadata(const QString &key, const Properties &metadata)
{
    this->assertContainsProperty(key);
    
    d->props_metadata[key].insert(metadata);
}

/** Set the metadata for the property with key 'key' and metadata with
    metakey 'metakey' to the value 'metavalue', setting the metametadata
    for this piece of metadata to 'metametadata'
    
    \throw SireBase::missing_property
*/
void Properties::insertMetadata(const QString &key, const QString &metakey,
                                const Property &metavalue,
                                const Properties &metametadata)
{
    this->assertContainsProperty(key);
    
    d->props_metadata[key].insert(metakey, metavalue, metametadata);
}

/** Set the metadata for the property with key 'key' and metadata with
    metakey 'metakey' to the value 'metavalue'
    
    \throw SireBase::missing_property
*/
void Properties::insertMetadata(const QString &key, const QString &metakey,
                                const Property &metavalue)
{
    this->insertMetadata(key, metakey, metavalue, Properties());
}

/** Update the metadata for the property with key 'key' and metadata with
    metakey 'metakey' to equal 'metavalue'. If 'clear_metametadata' is
    true, then all of the existing metametadata for this piece of 
    metadata is removed.
    
    \throw SireBase::missing_property
*/
void Properties::updateMetadata(const QString &key, const QString &metakey,
                                const Property &metavalue, bool clear_metametadata)
{
    this->assertContainsProperty(key);
    
    d->props_metadata[key].insert(metakey, metavalue, clear_metametadata);
}

/** Update the metadata for the property with key 'key' and metadata with
    metakey 'metakey' to equal 'metavalue', also inserting all of 
    the metametadata in 'metametadata'. If 'clear_metametadata' is true,
    the any existing metametadata is removed before 'metametadata' 
    is added.
    
    \throw SireBase::missing_property
*/
void Properties::updateMetadata(const QString &key, const QString &metakey,
                                const Property &metavalue,
                                const Properties &metametadata,
                                bool clear_metametadata)
{
    this->assertContainsProperty(key);
    
    d->props_metadata[key].update(metakey, metavalue, 
                                  metametadata, clear_metametadata);
}

/** Remove the property with key 'key' and all of its metadata */
void Properties::remove(const QString &key)
{
    if (this->contains(key))
    {
        d->properties.remove(key);
        d->props_metadata.remove(key);
    }
}

/** Remove all of the metadata associated with the property with 
    key 'key'
    
    \throw SireBase::missing_property
*/
void Properties::remoteMetaData(const QString &key)
{
    this->assertContainsProperty(key);
    
    d->props_metadata[key].clear();
}

/** Remove the metadata with metakey 'metakey' from the property 
    with key 'key' 
    
    \throw SireBase::missing_property
*/
void Properties::removeMetaData(const QString &key, const QString &metakey)
{
    this->assertContainsProperty(key);
    
    d->props_metadata[key].remove(metakey);
}

/** Completely remove all properties */
void Properties::clear()
{
    this->operator=(Properties());
}
