/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIRESTREAM_SHAREDDATASTREAM_H
#define SIRESTREAM_SHAREDDATASTREAM_H

#include "sireglobal.h"

#include <QDataStream>
#include <QSharedDataPointer>

#include <QList>
#include <QLinkedList>
#include <QHash>
#include <QVector>
#include <QMultiHash>
#include <QMap>
#include <QMultiMap>
#include <QSet>

#include <QString>

#include <boost/shared_ptr.hpp>

#include "SireBase/shareddatapointer.hpp"
#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireStream
{

using SireBase::SharedDataPointer;
using SireBase::SharedPolyPointer;

namespace detail
{

/** This is a holder class for all types of shared data. A virtual class
    is needed to ensure that the destructors of the shared copies in the 
    registry are called properly */
class SIRESTREAM_EXPORT SharedDataHolder
{
public:
    SharedDataHolder();
    virtual ~SharedDataHolder();
    
    template<class T>
    const T& sharedData() const;
    
protected:
    void throwCastingError() const;
};

/** This is the type-specialised version of SharedDataHolder */
template<class T>
class SIRESTREAM_EXPORT SharedDataHolderT : public SharedDataHolder
{
public:
    SharedDataHolderT() : SharedDataHolder()
    {}
    
    SharedDataHolderT(const T &shared_data) : SharedDataHolder(), data(shared_data)
    {}
    
    ~SharedDataHolderT()
    {}
    
    const T& sharedData() const
    {
        return data;
    }

private:
    /** Copy of the shared data */
    T data;
};

template<class T>
const T& SharedDataHolder::sharedData() const
{
    const SharedDataHolderT<T> *this_ptr 
                    = dynamic_cast<const SharedDataHolderT<T>*>(this);
                    
    if (not this_ptr)
        this->throwCastingError();
        
    return this_ptr->sharedData();
}

/** This is a helper class that helps the loading and saving of a shared
    pointer */
template<class T, class S>
struct GetSharedDataPointer
{
    static bool isEmpty(const T &shared_pointer)
    {
        return shared_pointer.constData() == 0;
    }

    static const void* value(const T &shared_pointer)
    {
        return shared_pointer.constData();
    }
    
    static void load(QDataStream &ds, T &shared_pointer)
    {
        if (shared_pointer.constData() == 0)
        {
            shared_pointer = T( new S() );
        }
        
        //null pointers will have been caught before here
        ds >> *(shared_pointer.data());
    }

    static void load_v1(QDataStream &ds, T &shared_pointer)
    {
        //load using the old format
        bool null_pointer;
        
        ds >> null_pointer;
        
        if (null_pointer)
        {
            shared_pointer = T();
        }
        else
        {
            GetSharedDataPointer<T,S>::load(ds, shared_pointer);
        }
    }
    
    static void save(QDataStream &ds, const T &shared_pointer)
    {
        //null pointers will have been caught before here
        ds << *(shared_pointer.constData());
    }
};

/** This is a helper class that helps the loading and saving of shared containers */
template<class T>
struct GetSharedContainerPointer
{
    static bool isEmpty(const T &container)
    {
        return (container.constBegin() == container.constEnd());
    }

    static const void* value(const T &container)
    {
        if (container.constBegin() == container.constEnd())
            return 0;
        else
            return &(*(container.constBegin()));
    }

    static void load(QDataStream &ds, T &container)
    {
        ds >> container;
    }
    
    static void save(QDataStream &ds, const T &container)
    {
        ds << container;
    }
};

/** This class is used by SharedDataStream to provide a registry of shared
    objects that have either been read or written already (and so don't
    need reading or writing again)
    
    @author Christopher Woods
*/
class SIRESTREAM_EXPORT SharedDataRegistry
{
public:
    static boost::shared_ptr<SharedDataRegistry> construct(QDataStream &ds);

    ~SharedDataRegistry();

    template<class T>
    const T& getSharedObject(quint32 id) const;
    
    template<class T, class GETPOINTER>
    quint32 getID(const T &shared_object);
    
    template<class T, class GETPOINTER>
    void loadedSharedObject(quint32 id, const T &shared_object);
    
    template<class T, class GETPOINTER>
    bool contains(const T &shared_object) const;
    
    bool containsKey(quint32 id) const;
    
private:
    SharedDataRegistry();

    void assertValidID(quint32 id) const;
    
    void throwIDError(quint32 registered_id) const;

    /** Actual pointers to the copies of shared data stored in 
        this registry - indexed by registry ID */
    QHash< quint32, boost::shared_ptr<SharedDataHolder> > objects_by_id;

    /** The keys for each piece of shared data, mapped to the 
        index of that data in the registry. The key is normally the 
        pointer to the actual underlying piece of shared data */
    QHash<const void*,quint32> objects_by_key;
    
    /** Pointer to the QDataStream object associated with this registry */
    QDataStream *ds;
};

/** Return a reference to the shared object with ID 'id'

    \throw SireError::invalid_key
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& SharedDataRegistry::getSharedObject(quint32 id) const
{
    this->assertValidID(id);
    return objects_by_id[id]->sharedData<T>();
}

/** Return the ID of the object 'shared_object'. This adds the object
    to the registry with a new ID number if it is not currently
    in the registry. This uses the template function 'GETPOINTER' to
    get the pointer to the shared data for the object */
template<class T, class GETPOINTER>
SIRE_OUTOFLINE_TEMPLATE
quint32 SharedDataRegistry::getID(const T &shared_object)
{
    const void *ptr = GETPOINTER::value(shared_object);

    if (not objects_by_key.contains(ptr))
    {
        objects_by_id.insert( quint32(objects_by_id.count()),
                              boost::shared_ptr<SharedDataHolder>(
                                     new SharedDataHolderT<T>(shared_object)) );
                                     
        objects_by_key.insert(ptr, objects_by_id.count() - 1);
    }

    return objects_by_key.value(ptr);
}

/** Update the registry with the object 'shared_object' which has just
    been read for the ID 'id' */
template<class T, class GETPOINTER>
SIRE_OUTOFLINE_TEMPLATE
void SharedDataRegistry::loadedSharedObject(quint32 id, const T &shared_object)
{
    const void *ptr = GETPOINTER::value(shared_object);

    if (not objects_by_key.contains(ptr))
    {
        if (objects_by_id.contains(id))
            this->throwIDError(id);

        objects_by_id.insert( id, boost::shared_ptr<SharedDataHolder>(
                                     new SharedDataHolderT<T>(shared_object)) );
                                     
        objects_by_key.insert(ptr, id);
    }
}

/** Return whether or not the registry contains the object 'shared_object' */
template<class T, class GETPOINTER>
SIRE_OUTOFLINE_TEMPLATE
bool SharedDataRegistry::contains(const T &shared_object) const
{
    const void *ptr = GETPOINTER::value(shared_object);
    return objects_by_key.contains(ptr);
}

}

/**
This is a streaming class that is used to help stream implicitly shared data. 
This class ensures that only a single copy of the implicitly shared data is actually 
written to the stream, thus reducing data bloat, and also preserving the shared data
of the objects when they are read back into the program.

@author
*/
class SIRESTREAM_EXPORT SharedDataStream
{
public:
    SharedDataStream(QDataStream &ds);

    ~SharedDataStream();

    template<class T>
    SharedDataStream& operator<<(const T &value);
    
    template<class T>
    SharedDataStream& operator>>(T &value);

    template<class T, class GETPOINTER>
    void sharedSave(const T &value);

    template<class T, class GETPOINTER>
    void sharedSavePointer(const T &pointer);
    
    template<class T, class GETPOINTER>
    void sharedLoad(T &value);

    template<class T, class GETPOINTER>
    void sharedLoad(T &value, const T &null_value);

    template<class T, class GETPOINTER>
    void sharedLoadPointer(T &pointer);

    quint32 version() const;

private:
    void readVersion();
    void writeVersion();

    static quint64 magic();

    quint32 loadID();

    bool peekMagic();
    quint32 readObjectID();

    /** Shared pointer to the registry of shared objects that 
        have already been streamed */
    boost::shared_ptr<detail::SharedDataRegistry> registry;

    /** Reference to the actual QDataStream that is used to
        stream the data */
    QDataStream &ds;
    
    /** The version number of this shared datastream - this is used
        to indicate the range of shared objects that are shared-streamed
        rather than normally streamed, and to control the format
        of the shared stream */
    quint32 version_number;
};

/** This function is used to save a shared object. If multiple copies of 
    the shared object are streamed, then only the first copy is sent to 
    the stream - with references to the first copy sent for any other copies. */
template<class T, class GETPOINTER>
void SharedDataStream::sharedSave(const T &value)
{
    //ensure that the version number of the shared stream is written
    if (this->version_number == 0)
        this->writeVersion();

    //is this a null object - if so, just write a zero
    if (GETPOINTER::isEmpty(value))
    {
        ds << this->magic() << quint32(0);
    }
    else
    {
        //have we seen this object before?
        bool already_streamed = this->registry->contains<T,GETPOINTER>(value);

        //stream the magic, so we know that this is a shared object,
        //and the stream ID number of the object
        ds << this->magic() << this->registry->getID<T,GETPOINTER>(value);

        //now stream the object, if it hasn't been already
        if (not already_streamed)
            GETPOINTER::save(ds, value);
    }
    
    return *this;
}

/** This function is used to load a shared object from the stream */
template<class T, class GETPOINTER>
void SharedDataStream::sharedLoad(T &value, const T &default_value)
{
    if (this->version_number == 0)
        this->readVersion();

    if (this->version() > 1)
    {
        //version 1 did not check to see if the object was shared
        
        //Try to read the magic number (8 bytes) from the datastream
        if (not this->peekMagic())
        {
            //this object is not shared-streamed - just load it up normally
            ds >> value;
            return;
        }
    }

    quint32 id = this->loadID();
    
    if (id == 0)
    {
        //this is a null object
        value = default_value;
    }
    else if (this->registry->containsKey(id))
    {
        //this object has already been read - get it from the database
        value = this->registry->getSharedObject<T>(id);
    }
    else
    {
        //this object has not been read, so we have to get it from the stream
        GETPOINTER::load(ds, value);
        
        //inform the database of the value of this object
        this->registry->loadedSharedObject<T>(id, value);
    }
}

/** This function is used to load a shared object from the stream */
template<class T, class GETPOINTER>
void SharedDataStream::sharedLoad(T &value)
{
    this->sharedLoad<T,GETPOINTER>(value, T());
}

/** This function is used to load a shared pointer from the stream */
template<class T, class GETPOINTER>
void SharedDataStream::sharedLoadPointer(T &pointer)
{
    if (this->version_number == 0)
        this->readVersion();

    if (this->version() == 1)
    {
        //version 1 used a different format for shared pointers
        
        quint32 id = this->loadID();
        
        if (id == 0)
        {
            //this is a null object
            pointer = T();
        }
        else if (this->registry->containsKey(id))
        {
            //this object has already been read - get it from the database
            pointer = this->registry->getSharedObject<T>(id);
        }
        else
        {
            //this object has not been read, so we have to get it from the stream
            GETPOINTER::load_v1(ds, pointer);
            
            //inform the database of the value of this object
            this->registry->loadedSharedObject<T>(id, pointer);
        }
    }
    else
        this->sharedLoad<T,GETPOINTER>(pointer);

    return *this;
}

/** This function is used to save a shared pointer to the stream */
template<class T, class GETPOINTER>
SIRE_OUTOFLINE_TEMPLATE
void SharedDataStream::sharedSavePointer(const T &pointer)
{
    this->sharedSave<T,GETPOINTER>(pointer);
}

//////////
////////// SharedDataStream serialisation operators
//////////

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator<<(SharedDataStream &sds, 
                             const QSharedDataPointer<T> &objptr)
{
    sds.sharedSavePointer< QSharedDataPointer<T>,
                       detail::GetSharedDataPointer<QSharedDataPointer<T>,T> >(objptr);
    return sds;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator>>(SharedDataStream &sds, 
                             QSharedDataPointer<T> &objptr)
{
    sds.sharedLoadPointer< QSharedDataPointer<T>,
                       detail::GetSharedDataPointer<QSharedDataPointer<T>,T> >(objptr);

    return sds;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator<<(SharedDataStream &sds,
                             const SharedDataPointer<T> &objptr)
{
    sds.sharedSavePointer< SharedDataPointer<T>,
                       detail::GetSharedDataPointer<SharedDataPointer<T>,T> >(objptr);

    return sds;
}

/** Specialisation of the deserialisation function for implicitly shared
    objects handled by SharedDataPointer. This will destream the object
    in a way that ensures that the implicitly shared nature of the
    data is preserved. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator>>(SharedDataStream &sds,
                             SharedDataPointer<T> &objptr)
{
    sds.sharedLoadPointer< SharedDataPointer<T>,
                      detail::GetSharedDataPointer<SharedDataPointer<T>,T> >(objptr);

    return sds;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator<<(SharedDataStream &sds, 
                             const SharedPolyPointer<T> &objptr)
{
    sds.sharedSave< SharedPolyPointer<T>,
                    detail::GetSharedDataPointer<SharedPolyPointer<T>,T> >(objptr);

    return sds;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator>>(SharedDataStream &sds, 
                             SharedPolyPointer<T> &objptr)
{
    sds.sharedLoad< SharedPolyPointer<T>,
                    detail::GetSharedDataPointer<SharedPolyPointer<T>,T> >(objptr);

    return sds;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator<<(SharedDataStream &sds, 
                             const boost::shared_ptr<T> &objptr)
{
    sds.sharedSave< boost::shared_ptr<T>,
                    detail::GetSharedDataPointer<boost::shared_ptr<T>,T> >(objptr);

    return sds;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator>>(SharedDataStream &sds,
                             boost::shared_ptr<T> &objptr)
{
    sds.sharedLoad< boost::shared_ptr<T>,
                    detail::GetSharedDataPointer<boost::shared_ptr<T>,T> >(objptr);

    return sds;
}

SharedDataStream& operator<<(SharedDataStream &sds, const QString &str);
SharedDataStream& operator>>(SharedDataStream &sds, QString &str);

namespace detail
{

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& sharedSaveContainer(SharedDataStream &sds, const T &container)
{
    sds.sharedSave< T, detail::GetSharedContainerPointer<T> >(container);
    return sds;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& sharedLoadContainer(SharedDataStream &sds, T &container)
{
    sds.sharedLoad< T, detail::GetSharedContainerPointer<T> >(container);
    return sds;
}

} // end of namespace detail

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator<<(SharedDataStream &sds, const QList<T> &list)
{
    return detail::sharedSaveContainer(sds, list);
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator>>(SharedDataStream &sds, QList<T> &list)
{
    return detail::sharedLoadContainer(sds, list);
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator<<(SharedDataStream &sds, const QVector<T> &vector)
{
    return detail::sharedSaveContainer(sds, vector);
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator>>(SharedDataStream &sds, QVector<T> &vector)
{
    return detail::sharedLoadContainer(sds, vector);
}

template<class Key, class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator<<(SharedDataStream &sds, const QHash<Key,T> &hash)
{
    return detail::sharedSaveContainer(sds, hash);
}

template<class Key, class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator>>(SharedDataStream &sds, QHash<Key,T> &hash)
{
    return detail::sharedLoadContainer(sds, hash);
}

template<class Key, class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator<<(SharedDataStream &sds, const QMap<Key,T> &map)
{
    return detail::sharedSaveContainer(sds, map);
}

template<class Key, class T>
SIRE_OUTOFLINE_TEMPLATE
SharedDataStream& operator>>(SharedDataStream &sds, QMap<Key,T> &map)
{
    return detail::sharedLoadContainer(sds, map);
}

}

SIRE_END_HEADER

#endif
