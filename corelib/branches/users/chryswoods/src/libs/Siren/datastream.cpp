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

#include <QDataStream>

#include "datastream.h"

#include "version_error.h"
#include "magic_error.h"

#include "detail/sharestrings.h"

#include "Siren/errors.h"

#include <QtEndian>

#include <QMutex>

#include <boost/weak_ptr.hpp>

#include <QDebug>

using namespace Siren;
using namespace Siren::detail;

using boost::shared_ptr;
using boost::weak_ptr;

////////
//////// Implementation of SharedDataRegistry
////////

SharedDataHolder::SharedDataHolder()
{}

SharedDataHolder::~SharedDataHolder()
{}

void SharedDataHolder::throwCastingError(const char *trycast,
                                         const char *iscast) const
{
    throw Siren::program_bug( QObject::tr(
        "DataStream has got confused over types and is attempting "
        "an invalid cast! (from %1 to %2)")
            .arg(iscast).arg(trycast), CODELOC );
}

////////
//////// Implementation of SharedDataRegistry
////////

/** Constructor */
SharedDataRegistry::SharedDataRegistry() : ds(0), version_number(0)
{}

/** Assert that this is a valid ID */
void SharedDataRegistry::assertValidID(quint32 id) const
{
    if (not objects_by_id.contains(id))
    {
        QList<quint32> keys = objects_by_id.keys();
        qSort(keys);
    
        throw Siren::program_bug( QObject::tr(
            "The DataStream has encountered an invalid object ID (%1). "
            "The maximum ID available is %2. This is sometimes caused "
            "by the programmer forgetting to detach a shared_ptr<T> before "
            "streaming into it.")
                .arg(id).arg( keys.last() ), CODELOC );
    }
}

/** Something went wrong with the ID system... */
void SharedDataRegistry::throwIDError(quint32 registered_id) const
{
    throw Siren::program_bug( QObject::tr(
        "The DataStream has got confused and messed up its ID numbers. "
        "The object with ID %1 has already been registered!")
            .arg(registered_id), CODELOC );
}

typedef QHash< QDataStream*, weak_ptr<SharedDataRegistry> > GlobalRegistry;

Q_GLOBAL_STATIC( QMutex, registryMutex );
Q_GLOBAL_STATIC( GlobalRegistry, globalRegistry );

/** Construct and return the shared registry for the passed QDataStream */
shared_ptr<SharedDataRegistry> SharedDataRegistry::construct(QDataStream &ds)
{
    QMutexLocker lkr( registryMutex() );
   
    if (globalRegistry()->contains(&ds))
    {
        shared_ptr<SharedDataRegistry> reg = globalRegistry()->value(&ds).lock();
        
        if (reg.get() != 0)
            return reg;
    }
    
    shared_ptr<SharedDataRegistry> reg( new SharedDataRegistry() );
    
    reg->ds = &ds;
    
    globalRegistry()->insert( &ds, reg );
    
    return reg;
}

/** Destructor */
SharedDataRegistry::~SharedDataRegistry()
{
    //remove this registry from the global_registry
    QMutexLocker lkr( registryMutex() );
    
    if (ds)
    {
        if (globalRegistry())
            globalRegistry()->remove(ds);
    }
}

/** Return whether or not the registry contains an object with key 'key' */
bool SharedDataRegistry::containsKey(quint32 key) const
{
    return objects_by_id.contains(key);
}

/** Return the version number of the shared data stream */
quint32 SharedDataRegistry::version() const
{
    if (version_number == 0)
        //the version number has not been set - use the latest version
        return 2;
    else
        return version_number;
}

static quint64 SHARED_DATASTREAM_MAGIC = 9840821822734431312LLU;

quint64 SharedDataRegistry::magic()
{
    return SHARED_DATASTREAM_MAGIC;
}

/** Try to find the magic number in the next 64bits of the datastream. This
    only peeks ahead - it does not advance the datastream at all.
    This returns true if the next 64bits contain the magic number */
bool SharedDataRegistry::peekMagic()
{
    if (ds == 0)
        return false;

    //peek to see if the 64bit magic number is next in the stream
    char magic_number_data[8];

    int nbytes_read = ds->device()->peek( magic_number_data, 8 );
    
    if (nbytes_read < 8)
    {
        //there is not enough data to contain the magic number
        return false;
    }

    //now read the number
    quint64 magic_number = qFromBigEndian<quint64>( *((quint64*)magic_number_data) );
    
    return (magic_number == SHARED_DATASTREAM_MAGIC);
}

/** This function is used to read the version number from the data stream
    (if it has not already been read) */
void SharedDataRegistry::readVersion()
{
    if (ds == 0)
        return;

    if (version_number != 0)
        return;
        
    if (not this->peekMagic())
    {
        //there is no magic - we must be at version 1
        version_number = 1;
        return;
    }
    
    //read the magic number and version number
    quint64 magic_number;
    
    (*ds) >> magic_number >> version_number;
    
    if (magic_number != SHARED_DATASTREAM_MAGIC)
        throw Siren::program_bug( QObject::tr(
            "Something went wrong when reading the DataStream version! "
            "The magic number changed (from %1 to %2)")
                .arg(SHARED_DATASTREAM_MAGIC).arg(magic_number), CODELOC );
                
    if (version_number == 0)
        throw Siren::program_bug( QObject::tr(
            "There was no version number, even though there should have been one!"),
                CODELOC );

    if (version_number > 2)
    {
        qWarning() << "Reading a DataStream written using a newer version ("
                   << version_number << ") than this version (2) - this could lead "
                   << "to problems...";
    }
}

/** This function writes the version number of the DataStream into the 
    stream - it only does this once, before any of the objects are written */
void SharedDataRegistry::writeVersion()
{
    if (ds == 0)
        return;

    if (version_number != 0)
        return;
        
    //we are at version 2
    version_number = 2;
    
    (*ds) << SHARED_DATASTREAM_MAGIC << version_number;
}

////////
//////// Implementation of DataStream
////////

/** Construct a DataStream that uses its own QDataStream
    object to serialise and deseralise the objects */
DataStream::DataStream()
           : boost::noncopyable(), 
             local_ds( new QDataStream() ),
             ds( *local_ds )
{
    //set the datastream to use the Qt 4.2 format
    ds.setVersion(QDataStream::Qt_4_2);

    registry = SharedDataRegistry::construct( *local_ds );
}

/** Construct a DataStream that uses the IO device 'd' */
DataStream::DataStream(QIODevice *d)
           : boost::noncopyable(),
             local_ds( new QDataStream(d) ),
             ds( *local_ds )
{
    //set the datastream to use the Qt 4.2 format
    ds.setVersion(QDataStream::Qt_4_2);

    registry = SharedDataRegistry::construct( *local_ds );
}

/** Construct a DataStream that operates on a byte array, a. 
    The mode describes how the device is to be used. */
DataStream::DataStream(QByteArray *a, QIODevice::OpenMode mode)
           : boost::noncopyable(),
             local_ds( new QDataStream(a, mode) ),
             ds( *local_ds )
{
    //set the datastream to use the Qt 4.2 format
    ds.setVersion(QDataStream::Qt_4_2);

    registry = SharedDataRegistry::construct( *local_ds );
}

/** Construct a read-only DataStream that reads from the 
    byte array 'a' */
DataStream::DataStream(const QByteArray &a)
           : boost::noncopyable(),
             local_ds( new QDataStream(a) ),
             ds( *local_ds )
{
    //set the datastream to use the Qt 4.2 format
    ds.setVersion(QDataStream::Qt_4_2);

    registry = SharedDataRegistry::construct( *local_ds );
}

/** Construct a DataStream that uses the QDataStream 'datastream'
    to serialise and unserialise the objects. */
DataStream::DataStream(QDataStream &qdatastream)
           : boost::noncopyable(), local_ds(0), ds(qdatastream)
{
    //set the datastream to use the Qt 4.2 format
    ds.setVersion(QDataStream::Qt_4_2);

    registry = SharedDataRegistry::construct(qdatastream);
}

/** Destructor */
DataStream::~DataStream()
{
    delete local_ds;
}

/** Return the version number of the shared data stream */
quint32 DataStream::version() const
{
    return registry->version();
}

quint64 DataStream::magic()
{
    return SharedDataRegistry::magic();
}

/** Try to find the magic number in the next 64bits of the datastream. This
    only peeks ahead - it does not advance the datastream at all.
    This returns true if the next 64bits contain the magic number */
bool DataStream::peekMagic()
{
    return registry->peekMagic();
}

/** Read the ID number of the next shared object from the stream */
quint32 DataStream::loadID()
{
    quint32 id;
    
    if (this->version() == 1)
    {
        bool already_streamed;
        ds >> id >> already_streamed;
    }
    else if (this->version() == 2)
    {
        quint64 magic;
        ds >> magic >> id;
        
        if (magic != SHARED_DATASTREAM_MAGIC)
            throw Siren::corrupted_data( QObject::tr(
                "There was a problem reading the magic number of the "
                "DataStream - read %1, but expected %2.")
                    .arg(magic).arg(SHARED_DATASTREAM_MAGIC), CODELOC );
    }
    else
        throw Siren::version_error( QObject::tr(
            "Reading version %1 of the DataStream format, but this "
            "program only supports versions 1 and 2.").arg(this->version()),
                CODELOC );

    return id;
}

/** This internal function is used to read the version number from the data stream
    (if it has not already been read) */
void DataStream::readVersion()
{
    registry->readVersion();
}

/** This function writes the version number of the DataStream into the 
    stream - it only does this once, before any of the objects are written */
void DataStream::writeVersion()
{
    registry->writeVersion();
}

namespace Siren
{

namespace detail
{

/** This is a helper class that helps the loading and saving of a shared
    pointer */
struct GetQStringPointer
{
    static bool isEmpty(const QString &string)
    {
        return string.isEmpty();
    }

    static const void* value(const QString &string)
    {
        return string.constData();
    }
    
    static void load(QDataStream &ds, QString &string)
    {
        ds >> string;
    }
    
    static void save(QDataStream &ds, const QString &string)
    {
        ds << string;
    }
};

} // end of namespace detail

DataStream SIREN_EXPORT &operator<<(DataStream &sds, 
                                    const QString &string)
{
    sds.sharedSave<QString, detail::GetQStringPointer>( shareString(string) );
    return sds;
}

DataStream SIREN_EXPORT &operator>>(DataStream &sds, QString &string)
{
    sds.sharedLoad<QString, detail::GetQStringPointer>(string);
    string = shareString(string);
    
    return sds;
}

/** Write a header to the data stream that describes the type and version
    of the object that is about to be written */
DataStream SIREN_EXPORT &writeHeader(DataStream &ds,
                                     const detail::RegisterMetaTypeBase &r_type)
{
    ds << r_type.UID() << r_type.version();
    return ds;
}

/** Read the header of the binary object to check that the type is correct
    and to obtain the binary data version */
VERSION_ID SIREN_EXPORT readHeader(DataStream &ds,
                                   const detail::RegisterMetaTypeBase &r_type)
{
    CLASS_UID uid;
    VERSION_ID v;

    ds >> uid >> v;

    if (uid != r_type.UID())
        throw magic_error( uid, r_type, CODELOC );

    return v;
}

} //end of namespace Siren


