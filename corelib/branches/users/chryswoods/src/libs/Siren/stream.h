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

#ifndef SIREN_STREAM_H
#define SIREN_STREAM_H

#include <QHash>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include "handle.h"

SIREN_BEGIN_HEADER

namespace Siren
{

class Stream;
class Class;
class ObjRef;
class Object;

template<class T> class Primitive;

/** This is the base class of all schemas */
class SIREN_EXPORT SchemaBase
{
public:
    ~SchemaBase();

    bool isNull() const;
    
    void assertNotNull() const;

protected:
    SchemaBase();
    SchemaBase(Stream *s, bool needs_decoration);
    SchemaBase(const SchemaBase &other);

    SchemaBase& operator=(const SchemaBase &other);
    
    Stream *stream;
    bool needs_decoration;
};

/** This schema class is used to help with the streaming
    of objects - it provides object version checks, 
    allows naming of types and also helps provide
    any necessary closure (as the schema notifies the
    stream when it starts, and when it goes out of scope)
    
    @author Christopher Woods
*/
class SIREN_EXPORT Schema : public SchemaBase
{
public:
    Schema();
    Schema(const Schema &other);

    ~Schema();

    Schema& operator=(const Schema &other);

    QString name() const;

    Stream& data(const char *data_name);
    Stream& base();

    void end();

private:
    friend class Stream;

    Schema(Stream *s, const QString &type_name, bool needs_decoration);

    /** The type name of the type managed by this schema
        (this will be the reduced type name, not the full typename) */
    QString type_name;
};

/** This schema is used to help stream shared data types */
class SIREN_EXPORT SharedSchema : public SchemaBase
{
public:
    SharedSchema();
    SharedSchema(const SharedSchema &other);
    
    ~SharedSchema();
    
    SharedSchema& operator=(const SharedSchema &other);
    
    bool mustStream() const;

    Stream& data(const char *data_name);
    Stream& base();
    
    void end();

private:
    friend class Stream;
    SharedSchema(Stream *s, int id, bool must_stream, bool needs_decoration);

    /** The ID number of the target */
    int id;
    
    /** Whether or not the shared object needs to be streamed */
    bool must_stream;
};

/** This is the base class of all container schemas */
class SIREN_EXPORT ContainerSchema : public SchemaBase
{
public:
    ~ContainerSchema();
    
    int count() const;
    
protected:
    ContainerSchema();

    ContainerSchema(Stream *s, const QString &type_name,
                    int count, bool needs_decoration);

    ContainerSchema(const ContainerSchema &other);

    ContainerSchema& operator=(const ContainerSchema &other);

    void nextItem();
    void throwFullContainerError() const;
    void assertFullContainer() const;
                    
    /** The type name of the contained type */
    QString type_name;
    
    /** The number of items in this container */
    int n_to_stream;
    
    /** The number of items already streamed */
    int n_streamed;
};

/** This is the schema used to help stream arrays */
class SIREN_EXPORT ArraySchema : public ContainerSchema
{
public:
    ArraySchema();
    ArraySchema(const ArraySchema &other);
    ~ArraySchema();

    ArraySchema& operator=(const ArraySchema &other);

    QString name() const;

    Stream& index(int i);
    
    void end();
    
private:
    friend class Stream;
    ArraySchema(Stream *s, const QString &type_name, 
                int count, bool needs_decoration);
};

/** This is the schema used to help stream sets */
class SIREN_EXPORT SetSchema : public ContainerSchema
{
public:
    SetSchema();
    SetSchema(const SetSchema &other);
    ~SetSchema();
    
    SetSchema& operator=(const SetSchema &other);
    
    QString name() const;
    
    Stream& entry();
    
    void end();
    
private:
    friend class Stream;
    SetSchema(Stream *s, const QString &type_name,
              int count, bool needs_decoration);
};

/** This is the schema used to help stream maps */
class SIREN_EXPORT MapSchema : public ContainerSchema
{
public:
    MapSchema();
    MapSchema(const MapSchema &other);
    ~MapSchema();
    
    MapSchema& operator=(const MapSchema &other);
    
    QString name() const;
    
    template<class Key>
    Stream& entry(Key &key);
    
    template<class Key>
    Stream& entry(const Key &key);
    
    void end();
    
private:
    friend class Stream;
    MapSchema(Stream *s, const QString &key_type, const QString &value_type,
              int count, bool allow_duplicates, bool needs_decoration);
              
    /** The type name for the key */
    QString key_type;
    
    /** Whether or not duplicates are allowed */
    bool allow_duplicates;
};

namespace detail
{

/** This class provides the means by which we can create type names
    for the types to be streamed, get a key to identify the object,
    and create new, null objects */
template<class T>
struct StreamHelper
{
    static QString typeName()
    {
        return T::typeName();
    }
    
    static const void* getKey(const T &object)
    {
        return object.constData();
    }
    
    static T null()
    {
        return T();
    }
};

template<>
struct StreamHelper<quint8>
{
    static QString typeName(){ return "uint8"; }
    static quint8 null(){ return 0; }
};

template<>
struct StreamHelper<qint8>
{
    static QString typeName(){ return "int8"; }
    static qint8 null(){ return 0; }
};

template<>
struct StreamHelper<quint32>
{
    static QString typeName(){ return "uint32"; }
    static quint32 null(){ return 0; }
};

template<>
struct StreamHelper<qint32>
{
    static QString typeName(){ return "int32"; }
    static qint32 null(){ return 0; }
};

template<>
struct StreamHelper<quint64>
{
    static QString typeName(){ return "uint64"; }
    static quint64 null(){ return 0; }
};

template<>
struct StreamHelper<qint64>
{
    static QString typeName(){ return "int64"; }
    static qint64 null(){ return 0; }
};

template<>
struct StreamHelper<float>
{
    static QString typeName(){ return "flaot"; }
    static float null(){ return 0; }
};

template<>
struct StreamHelper<double>
{
    static QString typeName(){ return "double"; }
    static double null(){ return 0; }
};

template<class T>
struct StreamHelper< boost::shared_ptr<T> >
{
    static QString typeName()
    {
        return QString("shared_ptr< %1 >").arg(T::typeName());
    }
    
    static const void* getKey(const boost::shared_ptr<T> &object)
    {
        return object.get();
    }
    
    static boost::shared_ptr<T> null()
    {
        return boost::shared_ptr<T>();
    }
};

/** This is the virtual base class that helps with streaming
    shared objects */
class SIREN_EXPORT SharedHelperBase
{
public:
    SharedHelperBase();
    
    virtual ~SharedHelperBase();
    
    virtual const char* what() const=0;
    
    virtual void finalise()=0;
    
    template<class T>
    const T& asA() const;

protected:
    void throwCastingError(const char* this_type, const char* want_type) const;
    void throwStillInConstructionError(const char* this_type) const;
};

/** This class provides help with streaming shared objects */
template<class T>
class SIREN_EXPORT SharedHelper : public SharedHelperBase
{
public:
    SharedHelper();
    SharedHelper(const T *object);
    SharedHelper(const SharedHelper &other);
    
    ~SharedHelper();
    
    SharedHelper& operator=(const SharedHelper &other);
    
    static const char* typeName();
    
    const char* what() const;
    
    void finalise();
    
    const T& object() const;

private:
    /** The actual shared object */
    T shared_object;
    
    /** Pointer to the shared object while it is being constructed */
    const T *constructing_object;
};

/** This class holds the registry of shared streamed objects */
class SIREN_EXPORT StreamRegistry : public boost::noncopyable
{
public:
    StreamRegistry();
    ~StreamRegistry();

    /** Index of all of the class names streamed so far, together
        with their ID number */
    QHash<QString,int> types_by_name;
    
    /** Index of all of the class names streamed so far, index
        by their ID number */
    QHash<int,QString> types_by_id;
    
    /** Index of all of the class versions */
    QHash<int,int> type_versions_by_id;

    /** Actual pointers to the copies of shared data stored in 
        this stream - indexed by ID */
    QHash< int, boost::shared_ptr<detail::SharedHelperBase> > sdata_by_id;

    /** The keys for each piece of shared data, mapped to the 
        index of that data in the stream. The key is normally the 
        pointer to the actual underlying piece of shared data */
    QHash<const void*,int> sdata_by_key;

    /** Shared string stored in this stream, indexed by ID */
    QHash<int,QString> strings_by_id;
    
    /** The ID of each string stored in this stream */
    QHash<QString,int> strings_by_key;

    /** Shared binary blobs stored in this stream, indexed by ID */
    QHash<int,QByteArray> blobs_by_id;
    
    /** The pointer for each piece of binary data mapped to ID number */
    QHash<const void*,int> blobs_by_key;
};

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

///////
/////// Implementation of SharedHelperBase
///////

/** Return the object held in this helper, cast as a type 'T' */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T& SharedHelperBase::asA() const
{
    const SharedHelper<T> *ptr = dynamic_cast<const SharedHelper<T>*>(this);
    
    if (not ptr)
    {
        if (QLatin1String(this->what()) != 
            QLatin1String(SharedHelper<T>::typeName()))
        {
            throwCastingError( this->what(), SharedHelper<T>::typeName() );
        }
        
        ptr = static_cast<const SharedHelper<T>*>(this);
    }
    
    return ptr->object();
}

///////
/////// Implementation of SharedHelper
///////

template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedHelper<T>::SharedHelper() 
                : SharedHelperBase(), 
                  shared_object( StreamHelper<T>::null() ),
                  constructing_object(0)
{}

template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedHelper<T>::SharedHelper(const T *object)
                : SharedHelperBase(), 
                  shared_object( StreamHelper<T>::null() ),
                  constructing_object(object)
{}

template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedHelper<T>::SharedHelper(const SharedHelper &other)
                : SharedHelperBase(other), shared_object(other.shared_object),
                  constructing_object(other.constructing_object)
{}

template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedHelper<T>::~SharedHelper()
{}

template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedHelper<T>& SharedHelper<T>::operator=(const SharedHelper<T> &other)
{
    shared_object = other.shared_object;
    constructing_object = other.constructing_object;
    
    return *this;
}

/** Take a copy of the shared object once it has finished construction */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
void SharedHelper<T>::finalise()
{
    if (constructing_object)
    {
        shared_object = *constructing_object;
        constructing_object = 0;
    }
}

template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T& SharedHelper<T>::object() const
{
    if (constructing_object)
        SharedHelperBase::throwStillInConstructionError(this->what());
    
    return shared_object;
}

template<class T>
SIREN_OUTOFLINE_TEMPLATE
const char* SharedHelper<T>::typeName()
{
    return typeid( SharedHelper<T> ).name();
}

template<class T>
SIREN_OUTOFLINE_TEMPLATE
const char* SharedHelper<T>::what() const
{
    return SharedHelper<T>::typeName();
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

} // end of namespace detail

/** This is the base class of all stream objects. A virtual 
    base class is used to provide a common interface that
    the other derived classes can use for streaming
    (so that we don't end up with a profusion of different
    streaming functions!)
    
    A hyperthetical object could be streamed using this function;
    
    void Person::stream(Stream &s)
    {
        int version = s.checkVersion<Person>(2);
    
        Schema schema = s.item<Person>();
        
        if (version == 1)
        {
            schema.data("name") & name;
            schema.data("address") & addrs;
            schema.data("age") & age_years;
            
            Obj::stream( schema.base() );
            
            if (schema.isLoading())
            {
                //clear the current 'friend' array, as this
                //doesn't exist with this version of the data
                friends.clear();
            }
        }
        else if (version == 2)
        {
            schema.data("name") & name;
            schema.data("address") & addrs;
            schema.data("age") & age_years;
            schema.data("friends") & friends;
            
            Obj::stream( schema.base() );
        }
        else
        {
            s.throwUnsupportedError<Person>(version, "1,2");
        }
    }
    
    In this case, "friends" is an array of People, which is streamed via
    the function
    
    void People::stream(Stream &s)
    {
        s.assertVersion<People>(1);
    
        //we are an array of "PersonPtr" instances
        int n = this->count();
        
        ArraySchema schema = s.array<PeoplePtr>(n);
        
        if (schema.isLoading())
        {
            this->resize(n);
        
            PeoplePtr *array = this->data();
        
            for (int i=0; i<n; ++i)
            {
                schema.index(i) >> array[i];
            }
        }
        else
        {
            const PeoplePtr *array = this->constData();
            
            for (int i=0; i<n; ++i)
            {
                schema.index(i) << array[i];
            }
        }
    }
    
    In this case, "PeoplePtr" is just a SharedPolyPointer to a Person object,
    which is streamed directly as a basic type
    
    Stream& operator&(Stream &s, SharedPolyPointer<Person> &ptr)
    {
        s.assertVersion("Siren::SharedPolyPointer", 1);
        
        SharedSchema shared = s.shared(ptr);
        
        if (shared.mustStream())
        {
            if (s.isLoading())
            {
                ObjRef object = s.loadNextObject();
                ptr = object.asA<Person>();
            }
            else
            {
                ptr->stream(s);
            }
        }
    }
    
    @author Christopher Woods
*/
class SIREN_EXPORT Stream 
        : public ExtendsHandle< Stream, Handles<detail::StreamRegistry> >
{
public:
    virtual ~Stream();

    static QString typeName();

    virtual Stream& operator&(bool &b)=0; 
    
    virtual Stream& operator&(qint8 &i)=0;
    virtual Stream& operator&(quint8 &i)=0;
    virtual Stream& operator&(quint16 &i)=0;
    virtual Stream& operator&(qint16 &i)=0;
    virtual Stream& operator&(qint32 &i)=0;
    virtual Stream& operator&(quint64 &i)=0;
    virtual Stream& operator&(qint64 &i)=0;
    virtual Stream& operator&(quint32 &i)=0;

    virtual Stream& operator&(float &f)=0;
    virtual Stream& operator&(double &f)=0;

    Stream& operator&(QString &s);
    Stream& operator&(QByteArray &b);
    
    Stream& operator&(Object &object);
    
    template<class T>
    Stream& operator&(Primitive<T> &primitive);
    
    ObjRef loadNextObject();

    bool isSaving() const;
    bool isLoading() const;

    void assertIsSaving() const;
    void assertIsLoading() const;

    int checkVersion(const QString &class_name, int version);
    void assertVersion(const QString &class_name, int version);

    template<class T>
    int checkVersion(int version);
    
    template<class T>
    void assertVersion(int version);

    Schema item(const Class &c, int version=1);

    template<class T>
    Schema item(int version=1);
    
    template<class T>
    ArraySchema array(int count);
    
    template<class T>
    SetSchema set(int count);
    
    template<class Key, class Value>
    MapSchema map(int count, bool allow_duplicates=false);

    template<class T>
    SharedSchema shared(T &value);

    template<class T>
    SharedSchema shared(const T &value);

    template<class T>
    Stream& operator<<(const T &data);
    
    template<class T>
    Stream& operator>>(T &data);

protected:
    Stream();
    Stream(bool);
    
    Stream(const Stream &other);
    
    Stream& operator=(const Stream &other);
    
    bool operator==(const Stream &other) const;
    bool operator!=(const Stream &other) const;

    void saveNeedsDecoration(bool needs_decoration);
    
    void saveSavingMode();
    void saveLoadingMode();

    /** Called to signify the start of a new item (scalar) of type 'type_name'.
        If this is a decorated stream, then it is up to you to verify that
        the type in the stream is the same as the type.
    */
    virtual void startItem(const QString &type_name)=0;
    
    /** Called to signify the start of a new array of 'count' elements, returning
        the number of elements */
    virtual int startArray(const QString &type_name, int count)=0;
    
    /** Called to signify the start of a new set of 'count' elements, returning
        the number of elements */
    virtual int startSet(const QString &type_name, int count)=0;
    
    /** Called to signify the start of a new map of 'count' elements, returning
        the number of elements */
    virtual int startMap(const QString &key_type, const QString &value_type, 
                         int count, bool allow_duplicates)=0;

    /** Called to signify the next data item, with field name 'data_name' */
    virtual void nextData(const char *data_name)=0;
    
    /** Called to signify the next base class item */
    virtual void nextBase()=0;

    /** Called to signify the next item at index 'i' in an array */
    virtual void nextIndex(int i)=0;
    
    /** Called to signify the next entry in a set */
    virtual void nextEntry()=0;
    
    /** Called to signify the next key in a map */
    virtual void nextKey()=0;
    /** Called to signify the next value in a map */
    virtual void nextValue()=0;

    /** Called to signify the end of an item of type 'type_name' */
    virtual void endItem(const QString &type_name)=0;

    /** Called to signify the end of an array of type 'type_name' */
    virtual void endArray(const QString &type_name)=0;

    /** Called to signify the end of a set of type 'type_name' */
    virtual void endSet(const QString &type_name)=0;

    /** Called to signify the end of a map of type 'type_name' */
    virtual void endMap(const QString &key_type, const QString &value_type)=0;

    /** Read a reference number and return it */
    virtual int readReference()=0;
    /** Write the reference number 'id' */
    virtual void createReference(int id)=0;
    
    /** Called to signify the start of the reference target with ID 'id' */
    virtual void startTarget(int id)=0;
    /** Called to signify the end of the reference target with ID 'id' */
    virtual void endTarget(int id)=0;

    /** Read a string reference number and return it */
    virtual int readStringReference()=0;
    /** Write the string reference number ID 'id' */
    virtual void createStringReference(int id)=0;
    
    /** Read the string with reference ID 'id' and return it */
    virtual QString readString(int id)=0;
    /** Write the string with reference 'ID' and value 'text' */
    virtual void writeString(int id, const QString &text)=0;

    /** Read a binary large object (Blob) reference and return it */
    virtual int readBlobReference()=0;
    /** Write the Blob reference ID 'id' */
    virtual void createBlobReference(int id)=0;

    /** Read and return the Blob with reference ID 'id' */
    virtual QByteArray readBlob(int id)=0;
    /** Write the Blob 'blob' with reference ID 'id' */
    virtual void writeBlob(int id, const QByteArray &blob)=0;

    /** Read the class ID and data format version for the class with
        name 'class_name' - the class ID is returned and 'version' updated */
    virtual int readClassID(const QString &class_name, int &version)=0;
    
    /** Write the class ID 'id' and data format version 'version' for the class 
        with type name 'class_name' */
    virtual void writeClassID(const QString &class_name, int id, int version)=0;
    
    /** Peek into the stream and return the class name of the type of
        the next object. This does not advance the stream */
    virtual QString peekNextType()=0;

    /** For non-decorated streams (which rely on magic numbers), read and
        return the next magic number. This number will always be 
        less than 2^30, so will fit into a 32 bit signed integer. */
    virtual int readMagic()=0;
    
    /** For non-decorated streams, write the magic number 'magic'.
        This number will always be less than 2^30, so will fit into a 
        32 bit signed integer. */
    virtual void writeMagic(int magic)=0;

    QString getClassType(int magic) const;

private:
    friend class Schema;
    friend class ArraySchema;
    friend class SetSchema;
    friend class MapSchema;
    friend class SharedSchema;

    bool haveStreamed(const void *key) const;
    bool haveStreamed(int id) const;
    
    int getID(const void *key);
    
    const detail::SharedHelperBase& getReference(int id) const;
    
    void startTarget(int id, detail::SharedHelperBase *helper);
    void finaliseObject(int id);

    void throwNotSavingError() const;
    void throwNotLoadingError() const;

    void throwUnsupportedError(const QString &type_name, int got_version,
                               const QString &supported_versions) const;

    /** Whether or not this stream requires decorators */
    bool needs_decoration;

    /** Whether or not this stream is saving or loading */
    bool is_saving;
};

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

//////////
////////// Implementation of Schema - inlining should
////////// hopefully speed up calls when method naming 
////////// is not used
////////// 

/** Tell the stream that the next object will be 
    a member variable, with name 'name' */
inline Stream& Schema::data(const char *name)
{
    if (needs_decoration)
        stream->nextData(name);
        
    return *stream;
}

/** Tell the stream that the next object will be
    a base class of this object */
inline Stream& Schema::base()
{
    if (needs_decoration)
        stream->nextBase();
        
    return *stream;
}

//////////
////////// Implementation of SharedSchema
//////////

/** Return whether or not the shared object must now
    be streamed - if this is false then the object
    has been streamed already. */
inline bool SharedSchema::mustStream() const
{
    return must_stream;
}

/** Tell the stream that the next object will be 
    a member variable, with name 'name' */
inline Stream& SharedSchema::data(const char *name)
{
    if (needs_decoration)
        stream->nextData(name);
        
    return *stream;
}

/** Tell the stream that the next object will be
    a base class of this object */
inline Stream& SharedSchema::base()
{
    if (needs_decoration)
        stream->nextBase();
        
    return *stream;
}

//////////
////////// Implementation of ContainerSchema - inlining should
////////// hopefully speed up calls when method naming 
////////// is not used
////////// 

/** Return the number of items to stream for this container */
inline int ContainerSchema::count() const
{
    return n_to_stream;
}

/** Tell that container that the next item in the container
    is about to be streamed */
inline void ContainerSchema::nextItem()
{
    if (n_streamed == n_to_stream)
        ContainerSchema::throwFullContainerError();

    n_streamed += 1;
}

//////////
////////// Implementation of ArraySchema - inlining should
////////// hopefully speed up calls when method naming 
////////// is not used
////////// 

/** Tell the stream that the next object will be
    the array item at index 'i' */
inline Stream& ArraySchema::index(int i)
{
    ContainerSchema::nextItem();

    if (needs_decoration)
        stream->nextIndex(i);
        
    return *stream;
}

//////////
////////// Implementation of SetSchema - inlining should
////////// hopefully speed up calls when method naming 
////////// is not used
////////// 

/** Tell the stream that the next object will
    be an entry into a set */
inline Stream& SetSchema::entry()
{
    ContainerSchema::nextItem();

    if (needs_decoration)
        stream->nextEntry();
        
    return *stream;
}

//////////
////////// Implementation of MapSchema - inlining should
////////// hopefully speed up calls when method naming 
////////// is not used
////////// 

/** Tell the stream that the next object will
    be an entry into a map at the key 'key' */
template<class Key>
SIREN_INLINE_TEMPLATE
Stream& MapSchema::entry(Key &key)
{
    ContainerSchema::nextItem();

    if (needs_decoration)
        stream->nextKey();
        
    (*stream) & key;
    
    if (needs_decoration)
        stream->nextValue();
        
    return *stream;
}

/** Tell the stream that the next object will
    be an entry into a map at the key 'key' */
template<class Key>
SIREN_INLINE_TEMPLATE
Stream& MapSchema::entry(const Key &key)
{
    ContainerSchema::nextItem();

    if (needs_decoration)
        stream->nextKey();
        
    (*stream) << key;
    
    if (needs_decoration)
        stream->nextValue();

    return *stream;
}

//////////
////////// Implementation of Stream
//////////

/** Return whether or not this stream is being used to save data */
inline bool Stream::isSaving() const
{
    return is_saving;
}

/** Return whether or not this stream is being used to load data */
inline bool Stream::isLoading() const
{
    return not is_saving;
}

/** Assert that this stream is in saving mode

    \throw Siren::invalid_state
*/
inline void Stream::assertIsSaving() const
{
    if (not is_saving)
        Stream::throwNotSavingError();
}

/** Assert that this stream is in loading mode

    \throw Siren::invalid_state
*/
inline void Stream::assertIsLoading() const
{
    if (is_saving)
        Stream::throwNotLoadingError();
}

/** Check the version of the object of type 'T', suggesting the 
    desired writing version, and returning the actual version */
template<class T>
int Stream::checkVersion(int version)
{
    return this->checkVersion( detail::StreamHelper<T>::typeName(), version );
}

/** Assert that the version of the object of type 'T' being
    streamed is 'version' 
    
    \throw Siren::version_error
*/
template<class T>
void Stream::assertVersion(int version)
{
    this->assertVersion( detail::StreamHelper<T>::typeName(), version );
}

/** Return whether or not this item has been shared streamed before */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedSchema Stream::shared(const T &value)
{
    this->assertIsSaving();
    
    const void* key = detail::StreamHelper<T>::getKey(value);

    if (key == 0)
    {
        this->createReference(0);
        return SharedSchema(this, 0, false, needs_decoration);
    }
    else if (this->haveStreamed(key))
    {
        int id = this->getID(key);
        this->createReference(id);
        return SharedSchema(this, id, false, needs_decoration);
    }
    else
    {
        int new_id = this->getID(key);
        this->createReference( new_id );
        
        this->startTarget(new_id);
        
        return SharedSchema(this, new_id, true, needs_decoration);
    }
}

/** Return whether or not this item has been shared streamed before,
    and it is has, and we are loading, then update the passed reference 
    so that it is equal to the shared object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedSchema Stream::shared(T &value)
{
    if (this->isSaving())
    {
        const T &const_value = value;
        return this->shared<T>(const_value);
    }
    else
    {
        int id = this->readReference();
        
        if (id == 0)
        {
            //this is a null object
            value = detail::StreamHelper<T>::null();
            return SharedSchema(this, id, false, needs_decoration);
        }
        else if (this->haveStreamed(id))
        {
            value = this->getReference(id).asA<T>();
            return SharedSchema(this, id, false, needs_decoration);
        }
        else
        {
            this->startTarget(id, new detail::SharedHelper<T>(&value));
            return SharedSchema(this, id, true, needs_decoration);
        }
    }
}

/** Return the schema for a Siren::Object of type 'T', saying either that
    we will write version 'version', or that we will support
    the version up to a maximum of 'version'
    
    \throw Siren::corrupted_data
    \throw Siren::version_error
*/
template<class T>
SIREN_OUTOFLINE_TEMPLATE
Schema Stream::item(int version)
{
    const QString type_name = detail::StreamHelper<T>::typeName();
    
    this->startItem(type_name);
    return Schema(this, type_name, this->needs_decoration);
}
    
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ArraySchema Stream::array(int count)
{
    const QString type_name = detail::StreamHelper<T>::typeName();
    count = this->startArray( type_name, count );
    return ArraySchema(this, type_name, count, this->needs_decoration);
}

template<class T>
SIREN_OUTOFLINE_TEMPLATE
SetSchema Stream::set(int count)
{
    const QString type_name = detail::StreamHelper<T>::typeName();
    count = this->startSet( type_name, count );
    return SetSchema(this, type_name, count, this->needs_decoration);
}

template<class Key, class Value>
SIREN_OUTOFLINE_TEMPLATE
MapSchema Stream::map(int count, bool allow_duplicates)
{
    const QString key_type = detail::StreamHelper<Key>::typeName();
    const QString value_type = detail::StreamHelper<Value>::typeName();
    
    count = this->startMap( key_type, value_type, count, allow_duplicates );
    
    return MapSchema(this, key_type, value_type, count, this->needs_decoration);
}

/** Explicitly request saving of an object */
template<class T>
SIREN_INLINE_TEMPLATE
Stream& Stream::operator<<(const T &data)
{
    if (not this->is_saving)
        this->throwNotSavingError();

    return this->operator&( const_cast<T&>(data) );
}

/** Explicitly request loading of an object */
template<class T>
SIREN_INLINE_TEMPLATE
Stream& Stream::operator>>(T &data)
{
    if (this->is_saving)
        this->throwNotLoadingError();

    return this->operator&( data );
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

}

SIREN_EXPOSE_CLASS( Siren::Stream )

SIREN_END_HEADER

#endif
