/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include <QByteArray>
#include <QFile>
#include <QDataStream>
#include <QList>
#include <QMutex>

#include <cstdlib>

#include "streamdata.hpp"

#include "tostring.h"

#include "sire_version.h"        // CONDITIONAL_INCLUDE

#include "SireStream/version_error.h"

#include "SireStream/errors.h"
#include "SireError/errors.h"

#include <QDebug>

using namespace SireStream;

using boost::tuple;
using boost::shared_ptr;

namespace SireStream
{
namespace detail
{

/** This internal class is used to record the names and versions of the 
    libraries that are linked in the executable. This information is attached
    to the binary data saved for an object as an aid to ensure compatibility
    when reading this data back.
    
    @author Christopher Woods
*/
class LibraryInfo
{
public:
    ~LibraryInfo();

    static void registerLibrary(const QString &library, 
                                quint32 version, quint32 minversion);
    
    static QString getSupportReport(const QList< tuple<QString,quint32> > &libraries);
                           
    static void assertSupported(const QString &library, quint32 version);

    static QByteArray getLibraryHeader();
    
    static QList< tuple<QString,quint32> > readLibraryHeader(const QByteArray &header);

    static void checkLibraryHeader(const QByteArray &header);

    static quint32 getLibraryVersion(const QString &library);
    static quint32 getMinimumSupportedVersion(const QString &library);

private:
    LibraryInfo();

    static LibraryInfo& libraryInfo();
    
    static QMutex data_mutex;
    static LibraryInfo *global_library;
    
    QHash< QString, tuple<quint32,quint32> > library_info;
    QByteArray library_header;
};

/////////
///////// Implementation of LibraryInfo
/////////

LibraryInfo::LibraryInfo()
{}

LibraryInfo::~LibraryInfo()
{}

QMutex LibraryInfo::data_mutex;

LibraryInfo* LibraryInfo::global_library(0);

LibraryInfo& LibraryInfo::libraryInfo()
{
    if (not global_library)
    {
        global_library = new LibraryInfo();
        
        //while we are here, register the SireStream and SireError libraries.
        //This has to be done here to prevent crashes caused by registration
        //being attempted before the static data in this file is initialised.
        global_library->library_info.insert( "SireError", tuple<quint32,quint32>(1,1) );
        global_library->library_info.insert( "SireStream", tuple<quint32,quint32>(1,1) );
    }
        
    return *global_library;
}

/** This function is used to register libraries as they are loaded */
void LibraryInfo::registerLibrary(const QString &library, 
                                  quint32 version, quint32 minversion)
{
    QMutexLocker lkr(&data_mutex);
    
    if (minversion > version)
        minversion = version;
    
    libraryInfo().library_info.insert( library, 
                                       tuple<quint32,quint32>(version,minversion) );
                                       
    libraryInfo().library_header.clear();
}

/** This returns a report about whether or not the provided list of libraries
    are supported. This returns an empty string if they are all supported,
    or a string containing every detail of the lack of support */
QString LibraryInfo::getSupportReport(const QList< tuple<QString,quint32> > &libraries)
{
    QMutexLocker lkr(&data_mutex);

    QStringList problems;

    for (QList< tuple<QString,quint32> >::const_iterator it = libraries.constBegin();
         it != libraries.constEnd();
         ++it)
    {
        QString library = it->get<0>();
        quint32 version = it->get<1>();
    
        if (not libraryInfo().library_info.contains(library))
        {
            problems.append( QObject::tr(
                "  (%1) The required library \"%2\" is missing.")
                    .arg(problems.count() + 1).arg(library) );
        }
        else
        {
            quint32 max_version = libraryInfo().library_info[library].get<0>();
            quint32 min_version = libraryInfo().library_info[library].get<1>();
            
            if (version > max_version)
            {
                problems.append( QObject::tr(
                    "  (%1) We need a newer version of the library \"%2\" (%3) "
                    "than the one available (%4).")
                        .arg(problems.count() + 1)
                        .arg(library).arg(version).arg(max_version) );
            }
            else if (version < min_version)
            {
                problems.append( QObject::tr(
                    "   (%1) We need an older version of the library \"%2\" (%3) "
                    "than the one available (%4).")
                        .arg(problems.count() + 1)
                        .arg(library).arg(version).arg(min_version) );
            }
        }
    }
    
    if (problems.isEmpty())
        return QString();
    else
        return problems.join("\n");
}
                       
/** Assert that the library 'library' with version 'version' is supported 

    \throw SireStream::version_error
*/
void LibraryInfo::assertSupported(const QString &library, quint32 version)
{
    QMutexLocker lkr(&data_mutex);
    
    if (not libraryInfo().library_info.contains(library))
    {
        throw version_error( QObject::tr(
            "The required library (%1) is not available.")
                .arg(library), CODELOC );
    }
    else
    {
        quint32 max_version = libraryInfo().library_info[library].get<0>();
        quint32 min_version = libraryInfo().library_info[library].get<1>();
        
        if (version > max_version)
        {
            throw version_error( QObject::tr(
                    "We need a newer version of the library \"%1\" (%2) "
                    "than the one available (%3).")
                        .arg(library).arg(version).arg(max_version), CODELOC );
        }
        else if (version < min_version)
        {
            throw version_error( QObject::tr(
                    "We need an older version of the library \"%1\" (%2) "
                    "than the one available (%3).")
                        .arg(library).arg(version).arg(min_version), CODELOC );
        }
    }
}

/** Return the header data that describes the libraries linked with 
    this executable */
QByteArray LibraryInfo::getLibraryHeader()
{
    QMutexLocker lkr(&data_mutex);
    
    if (libraryInfo().library_header.isEmpty())
    {
        QByteArray header;
    
        QDataStream ds( &header, QIODevice::WriteOnly );
        
        //this is version 1
        ds << quint32(1);
        
        //write the number of libraries
        ds << quint32( libraryInfo().library_info.count() );
        
        for (QHash< QString,tuple<quint32,quint32> >::const_iterator 
                                         it = libraryInfo().library_info.constBegin();
             it != libraryInfo().library_info.constEnd();
             ++it)
        {
            ds << it.key() << it.value().get<0>();
        }
        
        libraryInfo().library_header = qCompress(header, 9);
    }
    
    return libraryInfo().library_header;
}

/** This reads the library header and returns the libraries and versions
    used when writing the binary data */
QList< tuple<QString,quint32> > LibraryInfo::readLibraryHeader(const QByteArray &header)
{
    QByteArray unpacked_header = qUncompress(header);

    QDataStream ds(unpacked_header);
    
    quint32 version;
    
    ds >> version;
    
    QList< tuple<QString,quint32> > libraries;
    
    if (version == 1)
    {
        quint32 nlibs;
        ds >> nlibs;
        
        for (quint32 i=0; i<nlibs; ++i)
        {
            QString library;
            quint32 lib_version;
            
            ds >> library >> lib_version;
            
            libraries.append( tuple<QString,quint32>(library, lib_version) );
        }
    }
    else
        throw version_error( QObject::tr(
            "Cannot even read the information about supported libraries, as not "
            "even that data is supported. This can only read version 1, but "
            "the data is version %1.").arg(version), CODELOC );
    
    return libraries;
}

/** This reads a library header and checks that all of the requirements are
    satisfied 
    
    \throw version_error
*/
void LibraryInfo::checkLibraryHeader(const QByteArray &header)
{
    QList< tuple<QString,quint32> > libraries = readLibraryHeader(header);
    
    QString report = getSupportReport(libraries);
    
    if (not report.isEmpty())
        throw version_error( QObject::tr(
            "There are incompatibilities between the libraries required "
            "to read this data and the libraries available to this program.\n%1")
                .arg(report), CODELOC );
}

quint32 LibraryInfo::getLibraryVersion(const QString &library)
{
    QMutexLocker lkr(&data_mutex);
    
    if (not libraryInfo().library_info.contains(library))
    {
        throw SireError::unsupported( QObject::tr(
            "The library %1 is not available to this program. Available libraries "
            "are %2.")
                .arg(library)
                .arg( Sire::toString(libraryInfo().library_info.keys()) ),
                    CODELOC );
    }
    
    return libraryInfo().library_info.value(library).get<0>();
}

quint32 LibraryInfo::getMinimumSupportedVersion(const QString &library)
{
    QMutexLocker lkr(&data_mutex);
    
    if (not libraryInfo().library_info.contains(library))
    {
        throw SireError::unsupported( QObject::tr(
            "The library %1 is not available to this program. Available libraries "
            "are %2.")
                .arg(library)
                .arg( Sire::toString(libraryInfo().library_info.keys()) ),
                    CODELOC );
    }
    
    return libraryInfo().library_info.value(library).get<1>();
}

} // end of namespace detail
} // end of namespace SireStream

/////////
///////// Implementation of FileHeader
/////////

/** Write the header to the data stream */
QDataStream SIRESTREAM_EXPORT &operator<<(QDataStream &ds,
                                          const FileHeader &header)
{
    ds << quint32(1);

    QByteArray data;
    QDataStream ds2(&data, QIODevice::WriteOnly);
    
    ds2 << header.created_by
        << header.created_when
        << header.created_where
        << header.type_name
        << header.build_repository
        << header.build_version
        << header.required_libraries
        << header.system_locale
        << header.data_digest
        << header.compressed_size
        << header.uncompressed_size;
        
    ds << qCompress(data, 9);
    
    return ds;
}

/** Read the header from the data stream */
QDataStream SIRESTREAM_EXPORT &operator>>(QDataStream &ds,
                                          FileHeader &header)
{
    quint32 version;
    ds >> version;
    
    if (version == 1)
    {
        QByteArray data;
        ds >> data;

        data = qUncompress(data);
    
        QDataStream ds2(data);
    
        ds2 >> header.created_by
            >> header.created_when
            >> header.created_where
            >> header.type_name
            >> header.build_repository
            >> header.build_version
            >> header.required_libraries
            >> header.system_locale
            >> header.data_digest
            >> header.compressed_size
            >> header.uncompressed_size;
    }
    else
        throw version_error( QObject::tr(
            "The header version (%1) is not recognised. Only header version "
            "1 is supported in this program.")
                .arg(version), CODELOC );
        
    return ds;
}

/** Null constructor */
FileHeader::FileHeader() : compressed_size(0), uncompressed_size(0)
{}

/** Internal constructor used by streamDataSave */
FileHeader::FileHeader(const QString &typ_name,
                       const QByteArray &compressed_data,
                       const QByteArray &raw_data)
{
    //these two may be UNIX only...
    created_by = std::getenv("USER");
    created_where = std::getenv("HOSTNAME");

    created_when = QDateTime::currentDateTime();
    
    type_name = typ_name;
    
    build_repository = svn_repository_url;
    build_version = svn_version_number;
    
    required_libraries = detail::LibraryInfo::getLibraryHeader();
    
    data_digest = MD5Sum(compressed_data);
    
    compressed_size = compressed_data.count();
    uncompressed_size = raw_data.count();
}

/** Copy constructor */
FileHeader::FileHeader(const FileHeader &other)
           : created_by(other.created_by), created_when(other.created_when),
             created_where(other.created_where), type_name(other.type_name),
             build_repository(other.build_repository),
             build_version(other.build_version),
             required_libraries(other.required_libraries),
             system_locale(other.system_locale),
             data_digest(other.data_digest),
             compressed_size(other.compressed_size),
             uncompressed_size(other.uncompressed_size)
{}

/** Destructor */
FileHeader::~FileHeader()
{}

/** Copy assignment operator */
FileHeader& FileHeader::operator=(const FileHeader &other)
{
    if (this != &other)
    {
        created_by = other.created_by;
        created_when = other.created_when;
        created_where = other.created_where;
        type_name = other.type_name;
        build_repository = other.build_repository;
        build_version = other.build_version;
        system_locale = other.system_locale;
        required_libraries = other.required_libraries;
        data_digest = other.data_digest;
        compressed_size = other.compressed_size;
        uncompressed_size = other.uncompressed_size;
    }
    
    return *this;
}

/** Return the username of whoever created this data */
const QString& FileHeader::createdBy() const
{
    return created_by;
}

/** Return the date and time when this was created */
const QDateTime& FileHeader::createdWhen() const
{
    return created_when;
}

/** Where this file was created (the name of the machine) */
const QString& FileHeader::createdWhere() const
{
    return created_where;
}

/** Return the minimum memory the will be necessary to read the file */
quint32 FileHeader::requiredMemory() const
{
    return 2*uncompressed_size + compressed_size;
}

/** Return the compression ratio of the file */
double FileHeader::compressionRatio() const
{
    if (uncompressed_size == 0)
        return 1;
    else
    {
        return double(compressed_size) / uncompressed_size;
    }
}

/** Return the name of the top-level data type in this data */
const QString& FileHeader::dataType() const
{
    return type_name;
}

/** Return the locale in which this data was written. This is useful
    as it can help with the support of multiple languages
    (as the person who saved the file may not necessarily be using
     English ;-) */
const QLocale& FileHeader::locale() const
{
    return system_locale;
}

/** Return the list of libraries required to load this data */
QStringList FileHeader::requiredLibraries() const
{
    QList< tuple<QString,quint32> > libs = 
                    detail::LibraryInfo::readLibraryHeader(required_libraries);
                    
    QStringList libraries;
    
    for (QList< tuple<QString,quint32> >::const_iterator it = libs.constBegin();
         it != libs.constEnd();
         ++it)
    {
        libraries.append( it->get<0>() );
    }
    
    return libraries;
}

/** Does this data require that the library 'library' be loaded? */
bool FileHeader::requireLibrary(const QString &library) const
{
    QStringList libraries = this->requiredLibraries();
    
    foreach (const QString &lib, libraries)
    {
        if (lib == library)
            return true;
    }
    
    return false;
}

/** Return the version number required of the library 'library'. This
    returns 0 if this library isn't required. */
quint32 FileHeader::requiredVersion(const QString &library) const
{
    QList< tuple<QString,quint32> > libs = 
                    detail::LibraryInfo::readLibraryHeader(required_libraries);
                    
    for (QList< tuple<QString,quint32> >::const_iterator it = libs.constBegin();
         it != libs.constEnd();
         ++it)
    {
        if (it->get<0>() == library)
            return it->get<1>();
    }
    
    return 0;
}

/** Return the digest of the data - this is used to check 
    for any data corruption */
const MD5Sum& FileHeader::digest() const
{
    return data_digest;
}

/** Return the repository from which this source code was downloaded */
QString FileHeader::repository() const
{
    return build_repository;
}

/** Return the version of the source code from the repository */
QString FileHeader::buildVersion() const
{
    return build_version;
}

/** Assert that the libraries required are compatible with what has
    been loaded */
void FileHeader::assertCompatible() const
{
    detail::LibraryInfo::checkLibraryHeader(required_libraries);
}

/** Assert that the data in 'compressed_data' is not corrupt */
void FileHeader::assertNotCorrupted(const QByteArray &compressed_data) const
{
    if (quint32(compressed_data.size()) != compressed_size)
        throw SireStream::corrupted_data( QObject::tr(
            "The data for the object %1 appears to be corrupt as it "
            "is the wrong size (%2 bytes vs. the expected %3 bytes)")
                .arg(type_name).arg(compressed_data.size())
                .arg(compressed_size), CODELOC );
                
    MD5Sum new_digest(compressed_data);
    
    if (data_digest != new_digest)
        throw SireStream::corrupted_data( QObject::tr(
            "The data for the object %1 appears to be corrupt as the "
            "digests don't match (%2 vs. %3)")
                .arg(type_name)
                .arg(new_digest.toString(), data_digest.toString()),
                    CODELOC );
}

/////////
///////// Implementation of free functions
/////////

namespace SireStream
{
namespace detail
{

void throwStreamDataInvalidCast(const QString &load_type,
                                const QString &cast_type)
{
    throw SireError::invalid_cast( QObject::tr(
        "Cannot load the binary data for the object of type %1 into "
        "an object of type %2.")
            .arg(load_type).arg(cast_type), CODELOC );
}

static quint32 SIRE_MAGIC_NUMBER(251785387);

QByteArray streamDataSave( const void *object, const char *type_name )
{
    //get the ID number of this type
    int id = QMetaType::type( type_name );

    if ( id == 0 or not QMetaType::isRegistered(id) )
        throw SireError::unknown_type(QObject::tr(
            "The object with type \"%1\" does not appear to have been "
            "registered with QMetaType. It cannot be streamed! (%2, %3)")
                .arg(type_name).arg(id).arg(QMetaType::isRegistered(id)), CODELOC);

    //use the QMetaType streaming function to save this object
    QByteArray object_data;
    QDataStream ds2(&object_data, QIODevice::WriteOnly);

    if (not QMetaType::save(ds2, id, object))
        throw SireError::program_bug(QObject::tr(
            "There was an error saving the object of type \"%1\". "
            "Has the programmer remembered to add a RegisterMetaType for this class?")
                .arg(type_name), CODELOC);

    //compress the object data (level 3 compression seems best, giving
    //about a ten-fold reduction for only a 30% increase in serialisation time)
    QByteArray compressed_object_data = qCompress(object_data, 3);

    //now write a header for the object
    FileHeader header( type_name, compressed_object_data, object_data );
    
    //clear the uncompressed data to save space
    object_data = QByteArray();
    
    QByteArray data;
    QDataStream ds(&data, QIODevice::WriteOnly);
    
    //write a magic number - then the header
    ds << SIRE_MAGIC_NUMBER;
    ds << header;

    //now write the object data
    ds << compressed_object_data;

    return data;
}

void streamDataSave( const void *object, const char *type_name, const QString &filename )
{
    QFile f(filename);
    
    if (not f.open(QIODevice::WriteOnly))
        throw SireError::file_error(f, CODELOC);

    QByteArray data = streamDataSave(object, type_name);

    if (f.write(data) == -1)
        throw SireError::file_error( QObject::tr(
            "There was an error writing to the file %1. Is there enough space to "
            "to write a file of %d bytes?").arg(filename).arg(data.count()),
                CODELOC );
}

struct void_deleter
{
public:
    void_deleter(int type_id) : id(type_id)
    {}
    
    ~void_deleter()
    {}
    
    void operator()(void const *ptr) const
    {
        QMetaType::destroy(id, const_cast<void*>(ptr));
    }

private:
    int id;
};

} // end of namespace detail
} // end of namespace SireStream

namespace SireStream
{

/** This function is called by each Sire library when it is loaded
    to register the library with the streaming system. You should
    not call this function yourself! */
void SIRESTREAM_EXPORT registerLibrary(const QString &library,
                                       quint32 version, 
                                       quint32 min_supported_version)
{
    detail::LibraryInfo::registerLibrary(library, version, min_supported_version);
}

/** Return the version of the loaded library called 'library'

    \throw SireError::unsupported
*/
quint32 SIRESTREAM_EXPORT getLibraryVersion(const QString &library)
{
    return detail::LibraryInfo::getLibraryVersion(library);
}

/** Return the minimum data version that the library 'library' is capable
    of reading
    
    \throw SireError::unsupported
*/
quint32 SIRESTREAM_EXPORT getMinimumSupportedVersion(const QString &library)
{
    return detail::LibraryInfo::getMinimumSupportedVersion(library);
}

using namespace SireStream::detail;

/** This loads an object from the passed blob of binary data. This binary
    data *must* have been created by the "save" function below. */
tuple<shared_ptr<void>,QString> SIRESTREAM_EXPORT load(const QByteArray &data)
{
    QDataStream ds(data);
    
    //read the magic
    quint32 magic;
    ds >> magic;
    
    if (magic != SIRE_MAGIC_NUMBER)
        throw version_error( QObject::tr(
            "This data does not appear to have been written by the SireStream::save() "
            "function."), CODELOC );
    
    //read the version and the libraries and versions
    //(die if the required libraries aren't loaded!)
    FileHeader header;
    ds >> header;
    
    header.assertCompatible();
    
    if (header.dataType().isEmpty())
        //this is a null pointer!
        return tuple<shared_ptr<void>,QString>( shared_ptr<void>(), QString::null );
        
    //get the type that represents this name
    int id = QMetaType::type( header.dataType().toLatin1().constData() );

    if ( id == 0 or not QMetaType::isRegistered(id) )
        throw SireError::unknown_type( QObject::tr(
              "Cannot deserialise an object of type \"%1\". "
              "Ensure that the library or module containing "
              "this type has been loaded and that it has been registered "
              "with QMetaType.").arg(header.dataType()), CODELOC );
    
    //now read the data
    QByteArray object_data;
    ds >> object_data;
    
    //validate that the data is correct
    header.assertNotCorrupted(object_data);
    
    //uncompress the data
    object_data = qUncompress(object_data);
    
    QDataStream ds2(object_data);
    
    //create a default-constructed object of this type
    shared_ptr<void> ptr( QMetaType::construct(id,0), detail::void_deleter(id) );

    if (ptr.get() == 0)
        throw SireError::program_bug( QObject::tr(
                "Could not create an object of type \"%1\" despite "
                "this type having been registered with QMetaType. This is "
                "a program bug!!!").arg(header.dataType()), CODELOC );
    
    //load the object from the datastream
    if ( not QMetaType::load(ds2, id, ptr.get()) )
        throw SireError::program_bug(QObject::tr(
            "There was an error loading the object of type \"%1\"")
                 .arg(header.dataType()), CODELOC);

    return tuple<shared_ptr<void>,QString>( ptr, header.dataType() );
}

/** This loads an object from the specified file. This binary
    data *must* have been created by the "save" function below. */
tuple<shared_ptr<void>,QString> SIRESTREAM_EXPORT load(const QString &filename)
{
    QFile f(filename);
    
    if (not f.open( QIODevice::ReadOnly) )
        throw SireError::file_error(f, CODELOC);
        
    QByteArray data = f.readAll();
    
    if (data.isEmpty())
        throw SireError::file_error( QObject::tr(
            "There was an error reading data from the file %1. Either "
            "the file is empty, or some read error has occured.")
                .arg(filename), CODELOC );

    return load(data);
}

/** Return the header for the data */
FileHeader SIRESTREAM_EXPORT getDataHeader(const QByteArray &data)
{
    QDataStream ds(data);
    
    //read the magic
    quint32 magic;
    ds >> magic;
    
    if (magic != SIRE_MAGIC_NUMBER)
        throw version_error( QObject::tr(
            "This data does not appear to have been written by the SireStream::save() "
            "function."), CODELOC );
    
    //read the version and the libraries and versions
    //(die if the required libraries aren't loaded!)
    FileHeader header;
    ds >> header;

    return header;
}

/** Return the header of the file */
FileHeader SIRESTREAM_EXPORT getDataHeader(const QString &filename)
{
    QFile f(filename);
    
    if (not f.open( QIODevice::ReadOnly) )
        throw SireError::file_error(f, CODELOC);

    QDataStream ds(&f);
    
    //read the magic
    quint32 magic;
    ds >> magic;
    
    if (magic != SIRE_MAGIC_NUMBER)
        throw version_error( QObject::tr(
            "This data does not appear to have been written by the SireStream::save() "
            "function."), CODELOC );
    
    //read the version and the libraries and versions
    //(die if the required libraries aren't loaded!)
    FileHeader header;
    ds >> header;

    return header;
}

} //end of namespace SireStream
