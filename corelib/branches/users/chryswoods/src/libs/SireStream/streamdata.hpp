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

#ifndef SIRESTREAM_STREAMHELPER_HPP
#define SIRESTREAM_STREAMHELPER_HPP

#include <QByteArray>

#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>

#include "sireglobal.h"

#include <QDebug>

SIRE_BEGIN_HEADER

namespace SireStream
{

boost::tuple<boost::shared_ptr<void>,QString> load(const QByteArray &data);
boost::tuple<boost::shared_ptr<void>,QString> load(const QString &filename);

QString getTypeName(const QByteArray &data);
QString getTypeName(const QString &filename);

QStringList getRequiredLibraries(const QByteArray &data);
QStringList getRequiredLibraries(const QString &filename);

void registerLibrary(const QString &library,
                     quint32 version, quint32 min_supported_version);

quint32 getLibraryVersion(const QString &library);
quint32 getMinimumSupportedVersion(const QString &library);

class SIRESTREAM_EXPORT RegisterLibrary
{
public:
    RegisterLibrary()
    {}

    RegisterLibrary(const QString &library_name,
                    quint32 version, quint32 min_supported_version)
    {
        registerLibrary(library_name, version, min_supported_version);
    }
    
    ~RegisterLibrary()
    {}
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

namespace detail
{
QByteArray streamDataSave( const void *object, const char *type_name );

void streamDataSave( const void *object, const char *type_name, 
                     const QString &filename );

void throwStreamDataInvalidCast(const QString &load_type, 
                                const QString &cast_type);
}

/** This loads an object of type T from the passed blob of binary
    data. Note that this data *must* have been created by the "save"
    function below, and T must match the type of the object saved 
    in this data. Also note that this type must have been registered
    with the metatype system (via RegisterMetaType)
    
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T loadType(const QByteArray &data)
{
    boost::tuple<boost::shared_ptr<void>,QString> new_obj
            = SireStream::load(data);

    if ( QLatin1String(T::typeName()) != new_obj.get<1>() )
    {
        detail::throwStreamDataInvalidCast(new_obj.get<1>(), T::typeName());
    }

    return T( *(static_cast<const T*>(new_obj.get<0>().get())) );
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
T loadType(const QString &filename)
{
    boost::tuple<boost::shared_ptr<void>,QString> new_obj
            = SireStream::load(filename);

    if ( QLatin1String(T::typeName()) != new_obj.get<1>() )
    {
        detail::throwStreamDataInvalidCast(new_obj.get<1>(), T::typeName());
    }

    return T( *(static_cast<const T*>(new_obj.get<0>().get())) );
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
QByteArray save(const T &old_obj)
{
    return detail::streamDataSave( &old_obj, old_obj.what() );
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
void save(const T &old_obj, const QString &filename)
{
    detail::streamDataSave( &old_obj, old_obj.what(), filename );
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

SIRE_EXPOSE_FUNCTION( SireStream::getTypeName )
SIRE_EXPOSE_FUNCTION( SireStream::getRequiredLibraries )
SIRE_EXPOSE_FUNCTION( SireStream::getLibraryVersion )
SIRE_EXPOSE_FUNCTION( SireStream::getMinimumSupportedVersion )

SIRE_END_HEADER

#endif
