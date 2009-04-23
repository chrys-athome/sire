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
        return 2;
    else
        return version_number;
}

static quint64 SHARED_DATASTREAM_MAGIC = 9840821820734431312LLU;

quint64 SharedDataStream::magic()
{
    return SHARED_DATASTREAM_MAGIC;
}

/** Try to find the magic number in the next 64bits of the datastream. This
    only peeks ahead - it does not advance the datastream at all.
    This returns true if the next 64bits contain the magic number */
bool SharedDataStream::peekMagic()
{
    QByteArray magic = ds.device()->peek(8);
    
    if (magic.count() < 8)
    {
        //there is not enough remaining data to contain the magic number
        return false;
    }
    
    QDataStream ds2(magic);
    quint64 magic_number;
    
    ds2 >> magic_number;
    
    return magic_number == SHARED_DATASTREAM_MAGIC;
}

/** Read the ID number of the next shared object from the stream */
quint32 SharedDataStream::loadID()
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
        
        this->assertCorrectMagic(magic);
    }
    else
        this->throwVersionError();

    return id;
}

/** This internal function is used to read the version number from the data stream
    (if it has not already been read) */
void SharedDataStream::readVersion()
{
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

    if (version_number > 2)
    {
        qDebug() << "Reading a SharedDataStream written using a newer version ("
                 << version_number << ") than this version (2) - this could lead "
                 << "to problems...";
    }
}

/** This function writes the version number of the SharedDataStream into the 
    stream - it only does this once, before any of the objects are written */
void SharedDataStream::writeVersion()
{
    if (version_number == 0)
        return;
        
    //we are at version 2
    version_number = 2;
    
    ds << SHARED_DATASTREAM_MAGIC << version_number;
}

namespace SireStream
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
    
    static void save(QDataStream &ds, QString &string)
    {
        ds << string;
    }
};

} // end of namespace detail

SharedDataStream& operator<<(SharedDataStream &sds, const QString &string)
{
    sds.saveShared<QString, GetQStringPointer>(string);
    return sds;
}

SharedDataStream& operator>>(SharedDataStream &sds, QString &string)
{
    sds.loadShared<QString, GetQStringPointer>(string);
    return sds;
}
