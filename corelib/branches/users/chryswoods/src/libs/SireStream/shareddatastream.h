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
#include <QHash>

#include <boost/shared_ptr.hpp>

#include "SireBase/shareddatapointer.hpp"
#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireStream
{

using SireBase::SharedDataPointer;
using SireBase::SharedPolyPointer;

namespace private_detail
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

    int version() const;
    
    void versionError(const QString &supported_versions) const;

    template<class T>
    const T& getSharedObject(quint32 id) const;
    
    const QString& getString(quint32 id) const;
    
    template<class T>
    quint32 getID(const T &shared_object, const void *ptr, bool *first_copy);

    quint32 getStringID(const QString &str, bool *first_copy);
    
    template<class T>
    void loadedObject(quint32 id, const T &shared_object, const void *ptr);
    
    void loadedString(quint32 id, const QString &str);
    
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
    
    /** The locations of all of the shared strings */
    QHash<QString,quint32> strings_by_key;
    
    /** Pointer to the QDataStream object associated with this registry */
    QDataStream *ds;
};

template<class T>
const T& SharedDataRegistry::getSharedObject(quint32 id) const
{
    this->assertValidID(id);
    
    return objects_by_id[id]->sharedData<T>();
}

template<class T>
quint32 SharedDataRegistry::getID(const T &shared_object, const void *ptr, 
                                  bool *first_copy)
{
    if (objects_by_key.contains(ptr))
    {
        *first_copy = false;
    }
    else
    {
        *first_copy = true;
        objects_by_id.insert( quint32(objects_by_id.count()),
                              boost::shared_ptr<SharedDataHolder>(
                                     new SharedDataHolderT<T>(shared_object)) );
                                     
        objects_by_key.insert(ptr, objects_by_id.count() - 1);
    }

    return objects_by_key[ptr];
}

template<class T>
void SharedDataRegistry::loadedObject(quint32 id, const T &shared_object, 
                                      const void *ptr)
{
    if (not objects_by_key.contains(ptr))
    {
        if (objects_by_id.contains(id))
            this->throwIDError(id);

        objects_by_id.insert( id, boost::shared_ptr<SharedDataHolder>(
                                     new SharedDataHolderT<T>(shared_object)) );
                                     
        objects_by_key.insert(ptr, id);
    }
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
    SharedDataStream& operator<<(const T &obj);

    template<class T>
    SharedDataStream& operator<<(const QSharedDataPointer<T> &obj);

    template<class T>
    SharedDataStream& operator<<(const SharedDataPointer<T> &obj);
    
    template<class T>
    SharedDataStream& operator<<(const SharedPolyPointer<T> &obj);

    template<class T>
    SharedDataStream& operator<<(const boost::shared_ptr<T> &obj);

    template<class T>
    SharedDataStream& operator<<(const QVector<T> &vec);

    template<class T>
    SharedDataStream& operator>>(T &obj);

    template<class T>
    SharedDataStream& operator>>(QSharedDataPointer<T> &obj);

    template<class T>
    SharedDataStream& operator>>(SharedDataPointer<T> &obj);
    
    template<class T>
    SharedDataStream& operator>>(SharedPolyPointer<T> &obj);

    template<class T>
    SharedDataStream& operator>>(boost::shared_ptr<T> &obj);
    
    template<class T>
    SharedDataStream& operator>>(QVector<T> &vec);

    quint32 version() const;

private:
    void readVersion();
    void writeVersion();

    /** Shared pointer to the registry of shared objects that 
        have already been streamed */
    boost::shared_ptr<private_detail::SharedDataRegistry> registry;

    /** Reference to the actual QDataStream that is used to
        stream the data */
    QDataStream &ds;
    
    /** The version number of this shared datastream - this is used
        to indicate the range of shared objects that are shared-streamed
        rather than normally streamed, and to control the format
        of the shared stream */
    quint32 version_number;
};

/** Default serialisation function used as a wrapper around the standard
    QDataStream functions. This is called for any types that are
    not implicitly shared. */
template<class T>
SharedDataStream& SharedDataStream::operator<<(const T &obj)
{
    if (this->version_number == 0)
        this->writeVersion();

    ds << obj;
    return *this;
}

/** Default deserialisation function used as a wrapper around the standard
    QDataStream functions. This is called for any types that are
    not implicitly shared. */
template<class T>
SharedDataStream& SharedDataStream::operator>>(T &obj)
{
    if (this->version_number == 0)
        this->readVersion();
    
    ds >> obj;
    return *this;
}

/** Specialisation of the serialisation function for implicitly shared
    objects handled by QSharedDataPointer. This will stream the object
    in a way that ensures that only a single copy of the data is passed
    to the stream, with multiple copies being merely references to the
    first copy. */
template<class T>
SharedDataStream& SharedDataStream::operator<<(const QSharedDataPointer<T> &objptr)
{
    if (this->version_number == 0)
        this->writeVersion();

    //get the ID of this string in the registry
    bool first_copy;
    quint32 id = registry->getID(objptr, objptr.constData(), &first_copy);

    ds << id << first_copy;

    if (first_copy)
    {
        if (objptr.constData() == 0)
            //this is a null pointer
            ds << false;
        else
            //this is the first copy of this object and must be streamed
            ds << true << *objptr;
    }

    return *this;
}

/** Specialisation of the deserialisation function for implicitly shared
    objects handled by QSharedDataPointer. This will destream the object
    in a way that ensures that the implicitly shared nature of the
    data is preserved. */
template<class T>
SharedDataStream& SharedDataStream::operator>>(QSharedDataPointer<T> &objptr)
{
    if (this->version_number == 0)
        this->readVersion();

    if (registry->version() == 1)
    {
        quint32 id;
        bool first_copy;
        
        ds >> id >> first_copy;
        
        if (first_copy)
        {
            //this is the first copy of this object
            bool non_null;
            
            ds >> non_null;
            
            if (non_null)
            {
                if (objptr.constData() == 0)
                    objptr = new T();
                    
                ds >> *objptr;
            }
            else
                objptr = 0;
            
            //register the copy
            registry->loadedObject(id, objptr, objptr.constData());
        }
        else
            objptr = registry->getSharedObject< QSharedDataPointer<T> >(id);
    }
    else
        registry->versionError("1");

    return *this;
}

/** Specialisation of the serialisation function for implicitly shared
    objects handled by SharedDataPointer. This will stream the object
    in a way that ensures that only a single copy of the data is passed
    to the stream, with multiple copies being merely references to the
    first copy. */
template<class T>
SharedDataStream& SharedDataStream::operator<<(const SharedDataPointer<T> &objptr)
{
    if (this->version_number == 0)
        this->writeVersion();

    //get the ID of this string in the registry
    bool first_copy;
    quint32 id = registry->getID(objptr, objptr.constData(), &first_copy);

    ds << id << first_copy;

    if (first_copy)
    {
        //this is the first copy of this string and must be streamed
        if (objptr.constData() == 0)
            ds << false;
        else
            ds << true << *(objptr.constData());
    }

    return *this;
}

/** Specialisation of the deserialisation function for implicitly shared
    objects handled by SharedDataPointer. This will destream the object
    in a way that ensures that the implicitly shared nature of the
    data is preserved. */
template<class T>
SharedDataStream& SharedDataStream::operator>>(SharedDataPointer<T> &objptr)
{
    if (this->version_number == 0)
        this->readVersion();

    if (registry->version() == 1)
    {
        quint32 id;
        bool first_copy;
        
        ds >> id >> first_copy;

        if (first_copy)
        {
            //this is the first copy of this object
            bool non_null;
            ds >> non_null;
            
            if (non_null)
            {
                if (objptr.constData() == 0)
                    objptr = new T();
                    
                ds >> *(objptr.data());
            }
            else
                objptr = 0;
            
            //register the copy
            registry->loadedObject(id, objptr, objptr.constData());
        }
        else
            objptr = registry->getSharedObject< SharedDataPointer<T> >(id);
    }
    else
        registry->versionError("1");

    return *this;
}

/** Specialisation of the serialisation function for implicitly shared
    objects handled by SharedPolyPointer. This will stream the object
    in a way that ensures that only a single copy of the data is passed
    to the stream, with multiple copies being merely references to the
    first copy. */
template<class T>
SharedDataStream& SharedDataStream::operator<<(const SharedPolyPointer<T> &objptr)
{
    if (this->version_number == 0)
        this->writeVersion();

    //get the ID of this string in the registry
    bool first_copy;
    quint32 id = registry->getID(objptr, objptr.constData(), &first_copy);

    ds << id << first_copy;

    if (first_copy)
    {
        //this is the first copy of this string and must be streamed
        ds << objptr;
    }

    return *this;
}

/** Specialisation of the deserialisation function for implicitly shared
    objects handled by SharedPolyPointer. This will destream the object
    in a way that ensures that the implicitly shared nature of the
    data is preserved. */
template<class T>
SharedDataStream& SharedDataStream::operator>>(SharedPolyPointer<T> &objptr)
{
    if (this->version_number == 0)
        this->readVersion();

    if (registry->version() == 1)
    {
        quint32 id;
        bool first_copy;
        
        ds >> id >> first_copy;
        
        if (first_copy)
        {
            //this is the first copy of this object
            ds >> objptr;
            
            //register the copy
            registry->loadedObject(id, objptr, objptr.constData());
        }
        else
            objptr = registry->getSharedObject< SharedPolyPointer<T> >(id);
    }
    else
        registry->versionError("1");

    return *this;
}

/** Specialisation of the serialisation function for explicitly shared
    objects handled by boost::shared_ptr. This will stream the object
    in a way that ensures that only a single copy of the data is passed
    to the stream, with multiple copies being merely references to the
    first copy. */
template<class T>
SharedDataStream& SharedDataStream::operator<<(const boost::shared_ptr<T> &objptr)
{
    if (this->version_number == 0)
        this->writeVersion();

    //get the ID of this string in the registry
    bool first_copy;
    quint32 id = registry->getID(objptr, objptr.get(), &first_copy);

    ds << id << first_copy;

    if (first_copy)
    {
        //this is the first copy of this string and must be streamed
        ds << *objptr;
    }

    return *this;
}

/** Specialisation of the deserialisation function for explicitly shared
    objects handled by boost::shared_ptr. This will destream the object
    in a way that ensures that the explicitly shared nature of the
    data is preserved. */
template<class T>
SharedDataStream& SharedDataStream::operator>>(boost::shared_ptr<T> &objptr)
{
    if (this->version_number == 0)
        this->readVersion();

    if (registry->version() == 1)
    {
        quint32 id;
        bool first_copy;
        
        ds >> id >> first_copy;
        
        if (first_copy)
        {
            //this is the first copy of this object
            ds >> *objptr;
            
            //register the copy
            registry->loadedObject(id, objptr, objptr.get());
        }
        else
            objptr = registry->getSharedObject< boost::shared_ptr<T> >(id);
    }
    else
        registry->versionError("1");

    return *this;
}

template<>
SharedDataStream& SharedDataStream::operator<<(const QString &str);

template<>
SharedDataStream& SharedDataStream::operator>>(QString &str);

}

SIRE_END_HEADER

#endif
