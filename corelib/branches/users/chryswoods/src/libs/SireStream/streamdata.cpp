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
#include <QDataStream>

#include "streamdata.hpp"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireStream;

using boost::tuple;
using boost::shared_ptr;

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

QByteArray streamDataSave( const void *object, const char *type_name )
{
    //get the ID number of this type
    int id = QMetaType::type( type_name );

    if ( id == 0 or not QMetaType::isRegistered(id) )
        throw SireError::unknown_type(QObject::tr(
            "The object with type \"%1\" does not appear to have been "
            "registered with QMetaType. It cannot be streamed! (%2, %3)")
                .arg(type_name).arg(id).arg(QMetaType::isRegistered(id)), CODELOC);

    QByteArray data;
    QDataStream ds(&data, QIODevice::WriteOnly);
    
    //save the object type name
    ds << QString(type_name);

    //use the QMetaType streaming function to save this table
    if (not QMetaType::save(ds, id, object))
        throw SireError::program_bug(QObject::tr(
            "There was an error saving the object of type \"%1\". "
            "Has the programmer remembered to add a RegisterMetaType for this class?")
                .arg(type_name), CODELOC);

    return data;
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
        qDebug() << "Deleting the pointer" << ptr << "with ID" << id
                 << QMetaType::typeName(id);
                 
        QMetaType::destroy(id, const_cast<void*>(ptr));
    }

private:
    int id;
};

} // end of namespace detail
} // end of namespace SireStream

namespace SireStream
{

/** This loads an object from the passed blob of binary data. This binary
    data *must* have been created by the "save" function below. */
tuple<shared_ptr<void>,QString> SIRESTREAM_EXPORT load(const QByteArray &data)
{
    QDataStream ds(data);
    
    QString type_name;
    ds >> type_name;
    
    if (type_name.isEmpty())
        //this is a null pointer!
        return tuple<shared_ptr<void>,QString>( shared_ptr<void>(), QString::null );
        
    //get the type that represents this name
    int id = QMetaType::type( type_name.toLatin1().constData() );

    if ( id == 0 or not QMetaType::isRegistered(id) )
        throw SireError::unknown_type( QObject::tr(
              "Cannot deserialise an object of type \"%1\". "
              "Ensure that the library or module containing "
              "this type has been loaded and that it has been registered "
              "with QMetaType.").arg(type_name), CODELOC );
    
    //create a default-constructed object of this type
    shared_ptr<void> ptr( QMetaType::construct(id,0), detail::void_deleter(id) );

    if (ptr.get() == 0)
        throw SireError::program_bug( QObject::tr(
                "Could not create an object of type \"%1\" despite "
                "this type having been registered with QMetaType. This is "
                "a program bug!!!").arg(type_name), CODELOC );
    
    //load the object from the datastream
    if ( not QMetaType::load(ds, id, ptr.get()) )
        throw SireError::program_bug(QObject::tr(
            "There was an error loading the object of type \"%1\"")
                 .arg(type_name), CODELOC);

    return tuple<shared_ptr<void>,QString>( ptr, type_name );
}

}
