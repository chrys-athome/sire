/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "stream.h"
#include "class.h"
#include "objref.h"
#include "detail/sharestrings.h"

#include "Siren/errors.h"

using namespace Siren;
using namespace Siren::detail;

////////
//////// Implementation of SchemaBase
////////

/** Null constructor */
SchemaBase::SchemaBase() : stream(0), needs_decoration(false)
{}

/** Internal constructor */
SchemaBase::SchemaBase(Stream *s, bool decorate)
           : stream(s), needs_decoration(decorate)
{}

/** Copy constructor - we can only have one valid SchemaBase,
    so this takes the identity of 'other'! */
SchemaBase::SchemaBase(const SchemaBase &other)
           : stream(other.stream), needs_decoration(other.needs_decoration)
{
    const_cast<SchemaBase*>(&other)->stream = 0;
}

/** Destructor */
SchemaBase::~SchemaBase()
{}

/** Copy assignment operator - we can only have one valid
    SchemaBase, so this takes the identity of 'other'! */
SchemaBase& SchemaBase::operator=(const SchemaBase &other)
{
    if (this != &other)
    {
        stream = other.stream;
        needs_decoration = other.needs_decoration;
        
        const_cast<SchemaBase*>(&other)->stream = 0;
    }
    
    return *this;
}

/** Return whether or not this schema is null */
bool SchemaBase::isNull() const
{
    return stream == 0;
}

/** Assert that this schema is not null */
void SchemaBase::assertNotNull() const
{
    if (this->isNull())
        throw Siren::nullptr_error( QObject::tr("The schema is null!"), CODELOC );
}

////////
//////// Implementation of ContainerSchema
////////

ContainerSchema::ContainerSchema()
                : SchemaBase(), n_to_stream(0), n_streamed(0)
{}

ContainerSchema::ContainerSchema(Stream *s, const QString &name,
                                 int count, bool needs_decoration)
                : SchemaBase(s, needs_decoration),
                  type_name(name), n_to_stream(count), n_streamed(0)
{
    BOOST_ASSERT( count >= 0 );
}

ContainerSchema::ContainerSchema(const ContainerSchema &other)
                : SchemaBase(other), 
                  type_name(other.type_name),
                  n_to_stream(other.n_to_stream),
                  n_streamed(other.n_streamed)
{}

ContainerSchema& ContainerSchema::operator=(const ContainerSchema &other)
{
    if (this != &other)
    {
        SchemaBase::operator=(other);
        type_name = other.type_name;
        n_to_stream = other.n_to_stream;
        n_streamed = other.n_streamed;
    }
    
    return *this;
}

ContainerSchema::~ContainerSchema()
{}

void ContainerSchema::assertFullContainer() const
{
    if (stream)
    {
        if (n_streamed != n_to_stream)
        {
            //cannot throw an exception as this may be triggered
            //by premature calling of destructor if a deeper
            //exception has been thrown.
        
            Siren::printError( QObject::tr(
                    "There was an error streaming a container. The container "
                    "was listed as having a size of %1, but the number of "
                    "elements streamed (%2) has not reached this value.")
                        .arg(n_to_stream).arg(n_streamed) );
        }
    }
}

void ContainerSchema::throwFullContainerError() const
{
    throw Siren::corrupted_data( QObject::tr(
                "There was an error streaming a container. The "
                "number of items available to stream is about to be "
                "exceeded (container.count() == %1).")
                    .arg(n_to_stream), CODELOC );
}

////////
//////// Implementation of Schema
////////

/** Null constructor */
Schema::Schema() : SchemaBase()
{}

Schema::Schema(Stream *s, const QString &name, bool decorate)
       : SchemaBase(s, decorate), type_name(name)
{}

Schema::Schema(const Schema &other)
       : SchemaBase(other), type_name(other.type_name)
{}

Schema::~Schema()
{
    if (stream and needs_decoration)
        stream->endItem(type_name);
        
    stream = 0;
}

Schema& Schema::operator=(const Schema &other)
{
    if (this != &other)
    {
        SchemaBase::operator=(other);
        type_name = other.type_name;
    }
    
    return *this;
}

/** Return the name of the type being streamed */
QString Schema::name() const
{
    return type_name;
}

/** End the object being streamed prematurely. This will make this
    schema null */
void Schema::end()
{
    if (stream and needs_decoration)
        stream->endItem(type_name);
        
    stream = 0;
}

////////
//////// Implementation of SharedSchema
////////

SharedSchema::SharedSchema() : SchemaBase(), id(0), must_stream(false)
{}

SharedSchema::SharedSchema(Stream *s, int target_id, bool stream, bool decorate)
             : SchemaBase(s, decorate), id(target_id), must_stream(stream)
{}

SharedSchema::SharedSchema(const SharedSchema &other)
             : SchemaBase(other), id(other.id), must_stream(other.must_stream)
{}

SharedSchema::~SharedSchema()
{
    if (stream and must_stream)
    {
        stream->finaliseObject(id);
        stream->endTarget(id);
        stream = 0;
    }
}

SharedSchema& SharedSchema::operator=(const SharedSchema &other)
{
    if (this != &other)
    {
        SchemaBase::operator=(other);
        id = other.id;
        must_stream = other.must_stream;
    }
    
    return *this;
}

void SharedSchema::end()
{
    if (stream and must_stream)
    {
        stream->finaliseObject(id);
        stream->endTarget(id);
        stream = 0;
    }
}

////////
//////// Implementation of ArraySchema
////////

ArraySchema::ArraySchema() : ContainerSchema()
{}

ArraySchema::ArraySchema(Stream *s, const QString &type_name, 
                         int count, bool needs_decoration)
            : ContainerSchema(s, type_name, count, needs_decoration)
{}

ArraySchema::ArraySchema(const ArraySchema &other) : ContainerSchema(other)
{}

ArraySchema::~ArraySchema()
{
    assertFullContainer();

    if (stream and needs_decoration)
        stream->endArray(type_name);
    
    stream = 0;
}

ArraySchema& ArraySchema::operator=(const ArraySchema &other)
{
    ContainerSchema::operator=(other);
    return *this;
}

QString ArraySchema::name() const
{
    return QString("array{%1}").arg(type_name);
}

void ArraySchema::end()
{
    assertFullContainer();

    if (stream and needs_decoration)
        stream->endArray(type_name);
        
    stream = 0;
}

////////
//////// Implementation of SetSchema
////////

SetSchema::SetSchema() : ContainerSchema()
{}

SetSchema::SetSchema(Stream *s, const QString &type_name, 
                     int count, bool needs_decoration)
          : ContainerSchema(s, type_name, count, needs_decoration)
{}

SetSchema::SetSchema(const SetSchema &other) : ContainerSchema(other)
{}

SetSchema::~SetSchema()
{
    assertFullContainer();

    if (stream and needs_decoration)
        stream->endSet(type_name);
        
    stream = 0;
}

SetSchema& SetSchema::operator=(const SetSchema &other)
{
    ContainerSchema::operator=(other);
    return *this;
}

QString SetSchema::name() const
{
    return QString("set{%1}").arg(type_name);
}

void SetSchema::end()
{
    assertFullContainer();

    if (stream and needs_decoration)
        stream->endSet(type_name);
        
    stream = 0;
}

////////
//////// Implementation of MapSchema
////////

MapSchema::MapSchema() : ContainerSchema()
{}

MapSchema::MapSchema(Stream *s, const QString &key, const QString &value_type,
                     int count, bool duplicates, bool needs_decoration)
            : ContainerSchema(s, value_type, count, needs_decoration),
              key_type(key), allow_duplicates(duplicates)
{}

MapSchema::MapSchema(const MapSchema &other) 
          : ContainerSchema(other), key_type(other.key_type),
            allow_duplicates(other.allow_duplicates)
{}

MapSchema::~MapSchema()
{
    assertFullContainer();

    if (stream and needs_decoration)
        stream->endMap(key_type, type_name);
        
    stream = 0;
}

MapSchema& MapSchema::operator=(const MapSchema &other)
{   
    if (this != &other)
    {
        ContainerSchema::operator=(other);
        key_type = other.key_type;
        allow_duplicates = other.allow_duplicates;
    }
    
    return *this;
}

QString MapSchema::name() const
{
    return QString("map{%1,%2}").arg(key_type, type_name);
}

void MapSchema::end()
{
    assertFullContainer();

    if (stream and needs_decoration)
        stream->endMap(key_type, type_name);
        
    stream = 0;
}

////////
//////// Implementation of SharedHelperBase
////////

SharedHelperBase::SharedHelperBase()
{}
    
SharedHelperBase::~SharedHelperBase()
{}

void SharedHelperBase::throwCastingError(const char* this_type, 
                                         const char* want_type) const
{
    throw Siren::corrupted_data( QObject::tr(
            "Problem when streaming shared data. We can't cast the "
            "existing shared object of type %1 to the desired type %2.")
                .arg(this_type).arg(want_type), CODELOC );
}

void SharedHelperBase::throwStillInConstructionError(const char* this_type) const
{
    throw Siren::corrupted_data( QObject::tr(
            "Problem when loading some shared data of type %1. It appears that this "
            "shared object is needed even though it hasn't been read fully yet. "
            "Is there a circular reference (does the object contain itself?).")
                .arg(this_type), CODELOC );
}

////////
//////// Implementation of Stream
////////

/** Constructor */
Stream::Stream() : boost::noncopyable(), needs_decoration(false), is_saving(true)
{}

/** Destructor */
Stream::~Stream()
{}

/** Check that the next thing to read/write really is an object
    of type 'type_name', and return/set the version number of the class */
int Stream::checkVersion(const QString &type_name, int version)
{
    int id = types_by_name.value(type_name, -1);

    if (this->isLoading())
    {
        if (id == -1)
        {
            //this type hasn't been loaded before
            int actual_version = 0;
            
            id = this->readClassID(type_name, actual_version);
            
            types_by_name.insert(type_name, id);
            type_versions_by_id.insert(id, actual_version);
            
            return actual_version;
        }
        else
        {
            if (not needs_decoration)
            {
                //add in a magic check here, as the lack of decoration
                //may lead to us jumping over object boundaries
                int magic = this->readMagic();
                
                if (magic != id)
                    throw Siren::corrupted_data( QObject::tr(
                        "Could not read the magic value for %1. Read %2 "
                        "but we expected %3. This suggests that there was "
                        "a bug while writing the data, or it has become corrupted.")
                            .arg(type_name).arg(magic).arg(id), CODELOC );
            }
            
            return type_versions_by_id.value(id);
        }
    }
    else
    {
        //saving the object
        if (id == -1)
        {
            //we haven't written an object of this type yet
            id = types_by_name.count() + 1;
            
            //write this data to the stream
            this->writeClassID(type_name, id, version);
            
            types_by_name.insert(type_name, id);
            type_versions_by_id.insert(id, version);
            
            return version;
        }
        else
        {
            if (not needs_decoration)
                //write a magic number so that we can ensure that
                //we don't read through objects
                this->writeMagic(id);

            return type_versions_by_id.value(id);
        }
    }
}

/** Load the next Siren::Object from the stream. All streamable
    polymorphic types are derived from Siren::Object, so this 
    should work...
    
    \throw Siren::invalid_state
    \throw Siren::corrupted_data
    \throw Siren::version_error
    \throw Siren::unknown_type
*/
ObjRef Stream::loadNextObject()
{
    //get the type name of the next object
    QString next_type = this->peekNextType();
    
    //create a Class for this type
    Class c( next_type );
    
    return c.createObject();
}

/** Stream the passed string */
Stream& Stream::operator&(QString &text)
{
    if (this->isSaving())
    {
        if (text.isEmpty())
        {
            this->createStringReference(0);
            return *this;
        }
    
        QHash<QString,int>::const_iterator it = strings_by_key.constFind(text);
        
        if (it == strings_by_key.constEnd())
        {
            text = ::detail::shareString(text);
        
            //this is a new string
            int new_id = strings_by_key.count() + 1;
            
            this->createStringReference(new_id);
            this->writeString(new_id, text);
            
            strings_by_key.insert(text, new_id);
            strings_by_id.insert(new_id, text);
        }
        else
        {
            this->createStringReference( it.value() );
        }
    }
    else
    {
        int id = this->readStringReference();
        
        if (strings_by_id.contains(id))
        {
            text = strings_by_id.value(id);
        }
        else
        {
            text = ::detail::shareString( this->readString(id) );
            strings_by_key.insert(text, id);
            strings_by_id.insert(id, text);
        }
    }
    
    return *this;
}

/** Stream the passed blob of binary data */
Stream& Stream::operator&(QByteArray &blob)
{
    if (this->isSaving())
    {
        if (blob.isEmpty())
        {
            this->createBlobReference(0);
            return *this;
        }
    
        QHash<const void*,int>::const_iterator 
                        it = blobs_by_key.constFind(blob.constData());
        
        if (it == blobs_by_key.constEnd())
        {
            //this is a new blob
            int new_id = blobs_by_key.count() + 1;
            
            this->createBlobReference(new_id);
            this->writeBlob(new_id, blob);
            
            blobs_by_key.insert(blob.constData(), new_id);
            blobs_by_id.insert(new_id, blob);
        }
        else
        {
            this->createBlobReference( it.value() );
        }
    }
    else
    {
        int id = this->readBlobReference();
        
        if (blobs_by_id.contains(id))
        {
            blob = blobs_by_id.value(id);
        }
        else
        {
            blob = this->readBlob(id);
            blobs_by_key.insert(blob.constData(), id);
            blobs_by_id.insert(id, blob);
        }
    }
    
    return *this;
}

/** Stream an object */
Stream& Stream::operator&(Object &object)
{
    object.stream(*this);
    return *this;
}

/** Return the schema for the object with Class 'c', saying either that
    we will write version 'version', or that we will support
    the version up to a maximum of 'version'
    
    \throw Siren::corrupted_data
    \throw Siren::version_error
*/
Schema Stream::item(const Class &c, int version)
{
    const QString type_name = c.name();
    
    this->startItem(type_name);
    return Schema(this, type_name, this->needs_decoration);
}

/** Called by the derived stream to say whether or not it 
    needs data to be decorated */
void Stream::saveNeedsDecoration(bool decorate)
{
    needs_decoration = decorate;
}

/** Called by the derived stream to say that we are saving
    data to the stream */
void Stream::saveSavingMode()
{
    is_saving = true;
}

/** Called by the derived stream to say that we are loading
    data from the stream */
void Stream::saveLoadingMode()
{
    is_saving = false;
}

/** Return whether or not the object with key 'key' has been streamed */
bool Stream::haveStreamed(const void *key) const
{
    return sdata_by_key.contains(key);
}

/** Return whether or not the object with ID 'id' has been streamed */
bool Stream::haveStreamed(int id) const
{
    return sdata_by_id.contains(id);
}

/** Return the ID reference number for the object with key 'key' */
int Stream::getID(const void *key)
{
    if (not sdata_by_key.contains(key))
    {
        int new_id = sdata_by_key.count() + 1;
        sdata_by_key.insert( key, new_id );
        
        return new_id;
    }
    else
        return sdata_by_key.value(key);
}

/** Return a reference to the helper that contains a copy of the 
    shared object with ID 'id'
    
    \throw Siren::corrupted_data
*/
const SharedHelperBase& Stream::getReference(int id) const
{
    QHash< int, boost::shared_ptr<SharedHelperBase> >::const_iterator 
                    it = sdata_by_id.constFind(id);
                    
    if (it == sdata_by_id.constEnd())
        throw Siren::corrupted_data( QObject::tr(
                "There is a problem as the reference with ID %1 does not "
                "appear to exist in the archive?")
                    .arg(id), CODELOC );
                    
    return *(it.value());
}

/** Internal function called to start the target with ID 'id', using the 
    passed helper to save the object */ 
void Stream::startTarget(int id, SharedHelperBase *helper)
{
    this->assertIsLoading();
    sdata_by_id.insert( id, boost::shared_ptr<SharedHelperBase>(helper) );

    this->startTarget(id);
}

/** Internal function called to finalise the target with ID 'id' */
void Stream::finaliseObject(int id)
{
    QHash< int, boost::shared_ptr<SharedHelperBase> >::iterator 
                    it = sdata_by_id.find(id);
                    
    if (it != sdata_by_id.end())
        it.value()->finalise();
}

void Stream::throwNotSavingError() const
{
    throw Siren::invalid_state( QObject::tr(
            "You cannot use this stream for saving when it is opened for loading!"),    
                CODELOC );
}

void Stream::throwNotLoadingError() const
{
    throw Siren::invalid_state( QObject::tr(
            "You cannot use this stream for loading when it is opened for saving!"),    
                CODELOC );
}

/** Throw a version_error saying that only the passed versions are supported */
void Stream::throwUnsupportedError(const QString &type_name, int version,
                                   const QString &supported_versions) const
{
    throw Siren::version_error( QObject::tr(
            "Cannot stream an object of type %1 as its version (%2) is not "
            "one of the supported versions for this program (%3).")
                .arg(type_name).arg(version)
                .arg(supported_versions), CODELOC );
}

/** Assert that the version of the object is 'version' */
void Stream::assertVersion(const QString &type_name, int version)
{
    int actual_version = this->checkVersion(type_name, version);
    
    if (version != actual_version)
        throwUnsupportedError(type_name, version, QString::number(actual_version));
}
