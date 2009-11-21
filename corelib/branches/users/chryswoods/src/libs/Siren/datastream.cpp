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

#include <QHash>
#include <QMutex>

#include "datastream.h"
#include "hanref.h"
#include "tester.h"
#include "logger.h"

#include "Siren/errors.h"

using namespace Siren;

typedef QHash<const QDataStream*,WeakHandle> DSRegistryType;

Q_GLOBAL_STATIC( QMutex, registryMutex )
Q_GLOBAL_STATIC( DSRegistryType, datastreamRegistry );

void DataStream::registerDataStream()
{
    ///// Register this DataStream against a global registry
    /////  This allows easy interconversion with QDataStream
    QMutexLocker lkr( registryMutex() );

    QHash<const QDataStream*,WeakHandle> &registry = *(datastreamRegistry());
    
    //does this exist?
    bool found_ds = false;
    
    if (registry.contains(ds))
    {
        HanRef registered_ds = registry.value(ds).lock();
        
        if (not registered_ds.isNull())
        {
            this->operator=( registered_ds.asA<DataStream>() );
            found_ds = true;
        }
    }

    //clean out all expired entries
    {
        QMutableHashIterator<const QDataStream*,WeakHandle> it(registry);
    
        while (it.hasNext())
        {
            it.next();
        
            if (it.value().expired())
                it.remove();
        }
    }
    
    if (not found_ds)
    {
        //ok - this is a new DataStream
        Stream::saveNeedsDecoration(false);

        //we will use Qt version 4.2
        ds->setVersion( QDataStream::Qt_4_2 );

        //here's a magic number to check we are ok...
        const qint64 magic_number = 4506264543405497370UL;
        ds_version = 1;

        if (ds->device()->isReadable())
        {
            qint64 test_magic;
            *ds >> test_magic;
            
            if (test_magic != magic_number)
                throw Siren::corrupted_data( QObject::tr(
                        "Cannot read this binary stream as the magic numbers "
                        "don't match. Should be %1, but got %2.")
                            .arg(magic_number).arg(test_magic), CODELOC );
                            
            *ds >> ds_version;
        
            Stream::saveLoadingMode();
        }
        else
        {
            //save a magic number and version number
            *ds << magic_number << ds_version;
        
            Stream::saveSavingMode();
        }

        registry.insert( ds, WeakHandle(*this) );
    }
}

/** Null constructor */
DataStream::DataStream() : ImplementsHandle<DataStream,Stream>(), ds(0)
{}

/** Construct the stream for the device 'd' */
DataStream::DataStream(QIODevice *d) 
           : ImplementsHandle<DataStream,Stream>(true),
             ds_ptr( new QDataStream(d) ), ds(ds_ptr.get())
{
    registerDataStream();
    
    if (d == 0)
        DataStream::operator=( DataStream() );
}

/** Construct the stream for the data 'a', opened using the passed mode */
DataStream::DataStream(QByteArray *a, QIODevice::OpenMode mode) 
           : ImplementsHandle<DataStream,Stream>(true),
             ds_ptr( new QDataStream(a, mode) ), ds(ds_ptr.get())
{
    registerDataStream();
}

/** Construct the stream to read from the passed data 'a' */
DataStream::DataStream(const QByteArray &a) 
           : ImplementsHandle<DataStream,Stream>(true),
             ds_ptr( new QDataStream(a) ), ds(ds_ptr.get())
{
    registerDataStream();
}

/** Construct the stream for the passed QDataStream */
DataStream::DataStream(QDataStream &datastream) 
           : ImplementsHandle<DataStream,Stream>(true),
             ds(&datastream)
{
    registerDataStream();
}

/** Copy constructor */
DataStream::DataStream(const DataStream &other)
           : ImplementsHandle<DataStream,Stream>(other),
             ds_ptr(other.ds_ptr), ds(other.ds)
{}

/** Destructor */
DataStream::~DataStream()
{}

/** Copy assignment operator */
DataStream& DataStream::operator=(const DataStream &other)
{
    if (this != &other)
    {
        Stream::operator=(other);
        ds = other.ds;
        ds_ptr = other.ds_ptr;
    }
    
    return *this;
}

/** Comparison operator */
bool DataStream::operator==(const DataStream &other) const
{
    return ds == other.ds and Stream::operator==(other);
}

/** Comparison operator */
bool DataStream::operator!=(const DataStream &other) const
{
    return not DataStream::operator==(other);
}

uint DataStream::hashCode() const
{
    return qHash( DataStream::typeName() );
}

QString DataStream::toString() const
{
    if (isNull())
        return QObject::tr("DataStream::null");
    else if (isSaving())
        return QObject::tr("DataStream(=saving=)");
    else
        return QObject::tr("DataStream(=loading=)");
}

bool DataStream::test(Logger &logger) const
{
    return false;
}

/** Stream a logical value */
DataStream& DataStream::operator&(bool &b)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << b;
        else
            *ds >> b;
    }
    
    return *this;
}

/** Stream an integer */
DataStream& DataStream::operator&(qint8 &i)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << i;
        else
            *ds >> i;
    }
    
    return *this;
}

/** Stream an integer */
DataStream& DataStream::operator&(quint8 &i)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << i;
        else
            *ds >> i;
    }
    
    return *this;
}

/** Stream an integer */
DataStream& DataStream::operator&(quint16 &i)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << i;
        else
            *ds >> i;
    }
    
    return *this;
}

/** Stream an integer */
DataStream& DataStream::operator&(qint16 &i)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << i;
        else
            *ds >> i;
    }
    
    return *this;
}

/** Stream an integer */
DataStream& DataStream::operator&(qint32 &i)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << i;
        else
            *ds >> i;
    }
    
    return *this;
}

/** Stream an integer */
DataStream& DataStream::operator&(quint64 &i)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << i;
        else
            *ds >> i;
    }
    
    return *this;
}

/** Stream an integer */
DataStream& DataStream::operator&(qint64 &i)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << i;
        else
            *ds >> i;
    }
    
    return *this;
}

/** Stream an integer */
DataStream& DataStream::operator&(quint32 &i)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << i;
        else
            *ds >> i;
    }
    
    return *this;
}

/** Stream a floating point number */
DataStream& DataStream::operator&(float &f)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << f;
        else
            *ds >> f;
    }
    
    return *this;
}

/** Stream a floating point number */
DataStream& DataStream::operator&(double &f)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << f;
        else
            *ds >> f;
    }
    
    return *this;
}

void DataStream::startItem(const QString&)
{}

int DataStream::startArray(const QString&, int count)
{
    if (not isNull())
    {
        if (isSaving())
        {
            *ds << qint32(count);
            return count;
        }
        else
        {
            qint32 sz;
            *ds >> sz;
            return sz;
        }
    }
    else
        return 0;
}

int DataStream::startSet(const QString&, int count)
{
    if (not isNull())
    {
        if (isSaving())
        {
            *ds << qint32(count);
            return count;
        }
        else
        {
            qint32 sz;
            *ds >> sz;
            return sz;
        }
    }
    else
        return 0;
}

int DataStream::startMap(const QString&, const QString&, int count, bool)
{
    if (not isNull())
    {
        if (isSaving())
        {
            *ds << qint32(count);
            return count;
        }
        else
        {
            qint32 sz;
            *ds >> sz;
            return sz;
        }
    }
    else
        return 0;
}

void DataStream::nextData(const char*)
{}

void DataStream::nextBase()
{}

void DataStream::nextIndex(int)
{}

void DataStream::nextEntry()
{}

void DataStream::nextKey()
{}

void DataStream::nextValue()
{}

void DataStream::endItem(const QString&)
{}

void DataStream::endArray(const QString&)
{}

void DataStream::endSet(const QString&)
{}

void DataStream::endMap(const QString&, const QString&)
{}

int DataStream::readReference()
{
    if (not isNull())
    {
        if (isSaving())
            return 0;
        else
        {
            qint32 ref;
            *ds >> ref;
            return ref;
        }
    }
    else
        return 0;
}

void DataStream::createReference(int id)
{
    if (not isNull())
    {
        if (isSaving())
        {
            *ds << qint32(id);
        }
    }
}

void DataStream::startTarget(int)
{}

void DataStream::endTarget(int)
{}

int DataStream::readStringReference()
{
    return DataStream::readReference();
}

void DataStream::createStringReference(int id)
{
    DataStream::createReference(id);
}

QString DataStream::readString(int)
{
    if (not isNull())
    {
        if (isLoading())
        {
            QString s;
            *ds >> s;
            return s;
        }
    }
    
    return QString();
}

void DataStream::writeString(int, const QString &text)
{
    if (not isNull())
    {
        if (isSaving())
            *ds << text;
    }
}

int DataStream::readBlobReference()
{
    return DataStream::readReference();
}

void DataStream::createBlobReference(int id)
{
    DataStream::createReference(id);
}

QByteArray DataStream::readBlob(int)
{
    if (not isNull())
    {
        if (isLoading())
        {
            QByteArray data;
            *ds >> data;
            return data;
        }
    }
    
    return QByteArray();
}

void DataStream::writeBlob(int, const QByteArray &blob)
{
    if (not isNull())
    {
        if (isSaving())
        {
            *ds << blob;
        }
    }
}

int DataStream::readClassID(const QString &type_name, int &version)
{
    if (not isNull())
    {
        if (isLoading())
        {
            QString loaded_type;
            qint32 string_size;
            qint32 class_id, class_version;
            
            *ds >> class_id >> string_size >> loaded_type >> class_version;
            
            if (type_name != loaded_type)
                throw Siren::corrupted_data( QObject::tr(
                    "Something has gone wrong with the mapping of ID numbers "
                    "to class names in this stream. The stream says that the "
                    "class associated with ID %1 (version %2) is %3, but the "
                    "code expects it to be class %4.")
                        .arg(class_id).arg(class_version)
                        .arg(loaded_type, type_name), CODELOC );
            
            version = class_version;
            return class_id;
        }
    }
    
    version = 0;
    return 0;
}

void DataStream::writeClassID(const QString &type_name, int id, int version)
{
    if (not isNull())
    {
        if (isSaving())
        {
            *ds << qint32(id) << qint32(type_name.size())
                << type_name << qint32(version);
        }
    }
}

QString DataStream::peekNextType()
{
    if (isNull() or isSaving())
        return QString::null;
        
    //read the magic - this should always exist...
    QByteArray peek = ds->device()->peek(8);
    
    if (peek.isEmpty())
        throw Siren::corrupted_data( QObject::tr(
                "Could not peek ahead to read the magic number "
                "of the next object."), CODELOC );

    QDataStream ds2(peek);
        
    qint32 magic;
    ds2 >> magic;
        
    QString type_name = this->getClassType(magic);
        
    if (not type_name.isEmpty())
        return type_name;
        
    //we don't know of any type with this ID number. This means
    //that this must be the first object of this type, so the 
    //next four bytes should be the length of the type name, 
    //and then the name of the type itself
    qint32 sz;
    ds2 >> sz;
    
    //qstring is an array of 16bit qchars (plus 8 to guarantee have enough)
    peek = ds->device()->peek( 8 + 2*sz + 8 );
    
    QDataStream ds3(peek);
    
    ds3 >> magic >> sz >> type_name;
    
    if (type_name.isEmpty())
        throw Siren::corrupted_data( QObject::tr(
            "Could not read the type name of the object with magic %1. "
            "The length of the type name should be %2x2 bytes...")
                .arg(magic).arg(sz), CODELOC );

    return type_name;
}

int DataStream::readMagic()
{
    if (not isNull())
    {
        if (isLoading())
        {
            qint32 magic;
            *ds >> magic;
            return magic;
        }
    }
    
    return 0;
}

void DataStream::writeMagic(int magic)
{
    if (not isNull())
    {
        if (isSaving())
        {
            *ds << qint32(magic);
        }
    }
}
