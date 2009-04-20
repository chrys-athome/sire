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

#include "shareddatastream.h"

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include "tostring.h"

#include <QMutex>

#include <boost/weak_ptr.hpp>

#include <QDebug>

using namespace SireStream;
using namespace SireStream::private_detail;

using boost::shared_ptr;
using boost::weak_ptr;

////////
//////// Implementation of SharedDataRegistry
////////

SharedDataHolder::SharedDataHolder()
{}

SharedDataHolder::~SharedDataHolder()
{}

void SharedDataHolder::throwCastingError() const
{
    throw SireError::program_bug( QObject::tr(
        "SharedDataStream has got confused over types and is attempting "
        "an invalid cast!"), CODELOC );
}

////////
//////// Implementation of SharedDataRegistry
////////

/** Constructor */
SharedDataRegistry::SharedDataRegistry() : ds(0)
{}

/** Assert that this is a valid ID */
void SharedDataRegistry::assertValidID(quint32 id) const
{
    if (not objects_by_id.contains(id))
    {
        QList<quint32> keys = objects_by_id.keys();
        qSort(keys);
    
        throw SireError::program_bug( QObject::tr(
            "The SharedDataStream has encountered an invalid object ID (%1). "
            "The maximum ID available is %2. This is sometimes caused "
            "by the programmer forgetting to detach a shared_ptr<T> before "
            "streaming into it.")
                .arg(id).arg( keys.last() ), CODELOC );
    }
}

/** Something went wrong with the ID system... */
void SharedDataRegistry::throwIDError(quint32 registered_id) const
{
    throw SireError::program_bug( QObject::tr(
        "The SharedDataStream has got confused and messed up its ID numbers. "
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
    
//    qDebug() << SireError::getPIDString()
//             << "Creating a SharedDataRegistry for the QDataStream" << &ds;
    
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
    
//    qDebug() << SireError::getPIDString() 
//             << "Destroying the registry for the QDataStream" << ds
//             << "\n   (" << objects_by_key.count() << "shared object(s) and"
//             << strings_by_key.count() << "shared string(s))";
    
    if (ds)
    {
        if (globalRegistry())
            globalRegistry()->remove(ds);
    }
}

/** This is version 1 of the SharedDataStream format */
int SharedDataRegistry::version() const
{
    return 1;
}

void SharedDataRegistry::versionError(const QString &supported_versions) const
{
    throw version_error( QObject::tr("This SharedDataStream (version %1) can only "
            "read streams written by the following versions: %2.")
                .arg(this->version()).arg(supported_versions), CODELOC );
}

const QString& SharedDataRegistry::getString(quint32 id) const
{
    this->assertValidID(id);

    return objects_by_id[id]->sharedData<QString>();
}

quint32 SharedDataRegistry::getStringID(const QString &str, bool *first_copy)
{
    if (strings_by_key.contains(str))
    {
        *first_copy = false;
    }
    else
    {
        *first_copy = true;
        objects_by_id.insert( quint32(objects_by_id.count()),
                               boost::shared_ptr<SharedDataHolder>(
                                     new SharedDataHolderT<QString>(str)) );
                                     
        strings_by_key.insert(str, objects_by_id.count() - 1);
    }

    return strings_by_key[str];
}

void SharedDataRegistry::loadedString(quint32 id, const QString &str)
{
    if (not strings_by_key.contains(str))
    {
        if (objects_by_id.contains(id))
            this->throwIDError(id);
    
        objects_by_id.insert( id, boost::shared_ptr<SharedDataHolder>(
                                     new SharedDataHolderT<QString>(str)) );
                                     
        strings_by_key.insert(str, id);
    }
}

////////
//////// Implementation of SharedDataStream
////////

/** Construct a SharedDataStream that uses the QDataStream 'datastream'
    to serialise and unserialise the objects. */
SharedDataStream::SharedDataStream(QDataStream &qdatastream)
                 : ds(qdatastream), version_number(0)
{
    registry = SharedDataRegistry::construct(qdatastream);
}

/** Destructor */
SharedDataStream::~SharedDataStream()
{}

/** Return the version number of the shared data stream */
quint32 SharedDataStream::version() const
{
    if (version_number == 0)
        //the version number has not been set - use the latest version
        return 1;
    else
        return version_number;
}

static quint32 SHARED_DATASTREAM_MAGIC = 3640980511;

/** This internal function is used to read the version number from the data stream
    (if it has not already been read) */
void SharedDataStream::readVersion()
{
    if (version_number != 0)
        return;
        
    //try to read the magic number (4 bytes) from the datastream
    QByteArray magic = ds.device()->peek(4);
    
    if (magic.isEmpty())
    {
        //there is no magic - we must be at version 1
        version_number = 1;
        return;
    }
    
    QDataStream ds2(magic);
    
    quint32 magic_number;
    ds2 >> magic_number;
    
    if (magic_number != SHARED_DATASTREAM_MAGIC)
    {
        //this is not the magic number - it is missing, which implies
        //that this is version 1 of the format
        version_number = 1;
        return;
    }
    
    //read the magic number and version number
    ds >> magic_number >> version_number;
    
    if (magic_number != SHARED_DATASTREAM_MAGIC)
        throw SireError::program_bug( QObject::tr(
            "Something went wrong when reading the SharedDataStream version! "
            "The magic number changed (from %1 to %2)")
                .arg(SHARED_DATASTREAM_MAGIC).arg(magic_number), CODELOC );
                
    if (version_number == 0)
        throw SireError::program_bug( QObject::tr(
            "There was no version number, even though there should have been one!"),
                CODELOC );
}

/** This function writes the version number of the SharedDataStream into the 
    stream - it only does this once, before any of the objects are written */
void SharedDataStream::writeVersion()
{
    if (version_number == 0)
        return;
        
    //we are at version 1
    version_number = 1;
    
    ds << SHARED_DATASTREAM_MAGIC << version_number;
}

/** Specialisation of the serialisation function implicitly shared
    QString strings. This will stream the string in a way that ensures 
    that only a single copy of the string is passed to the stream, 
    with multiple copies being merely references to the first copy. */
template<>
SharedDataStream& SharedDataStream::operator<<(const QString &str)
{
    this->writeVersion();

    //get the ID of this string in the registry
    bool first_copy;
    quint32 id = registry->getStringID(str, &first_copy);

    ds << id << first_copy;

    if (first_copy)
    {
        //this is the first copy of this string and must be streamed
        ds << str;
    }

    return *this;
}

/** Specialisation of the deserialisation function for implicitly shared
    QString strings. This will destream the string in a way that ensures 
    that the implicitly shared nature of the string is preserved. */
template<>
SharedDataStream& SharedDataStream::operator>>(QString &str)
{
    this->readVersion();

    if (registry->version() == 1)
    {
        quint32 id;
        bool first_copy;
        
        ds >> id >> first_copy;
        
        if (first_copy)
        {
            //this is the first copy of this string
            ds >> str;
            
            //register the copy
            registry->loadedString(id, str);
        }
        else
            str = registry->getString(id);
    }
    else
        registry->versionError("1");

    return *this;
}
