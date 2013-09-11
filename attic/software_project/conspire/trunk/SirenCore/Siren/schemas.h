#ifndef SIREN_STREAM_H
#define SIREN_STREAM_H
/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/siren.h"

SIREN_BEGIN_HEADER

namespace Siren
{

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
