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

#include <Python.h>
#include <boost/python.hpp>

#include <QDataStream>
#include <QIODevice>

#include "sireglobal.h"

using namespace boost::python;

template<typename T>
QDataStream& __lshift__(QDataStream &ds, const T &value)
{
    ds << value;
    return ds;
}

template<typename T>
QDataStream& __rshift__(QDataStream &ds, T &value)
{
    ds >> value;
    return ds;
}

void  
SIREQT_EXPORT
export_QDataStream()
{
    scope QDataStream_scope = 
    class_<QDataStream, boost::noncopyable>("QDataStream", init<>())
        .def( init<QIODevice*>() )
        .def( init<QByteArray*, QIODevice::OpenMode>() )
        .def( init<const QByteArray&>() )
        
        .def( "__lshift__", &__lshift__<qint8>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<qint8>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<bool>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<bool>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<quint8>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<quint8>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<quint16>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<quint16>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<qint16>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<qint16>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<quint64>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<quint64>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<qint64>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<qint64>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<float>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<float>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<double>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<double>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<const char*>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<char*>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "atEnd", &QDataStream::atEnd )
        .def( "byteOrder", &QDataStream::byteOrder )
        .def( "device", &QDataStream::device,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "readBytes", &QDataStream::readBytes,
                                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "readRawData", &QDataStream::readRawData )
        .def( "resetStatus", &QDataStream::resetStatus )
        .def( "setByteOrder", &QDataStream::setByteOrder )
        .def( "setDevice", &QDataStream::setDevice )
        .def( "setStatus",  &QDataStream::setStatus )
        .def( "setVersion", &QDataStream::setVersion )
        .def( "skipRawData", &QDataStream::skipRawData )
        .def( "status", &QDataStream::status )
        .def( "unsetDevice", &QDataStream::unsetDevice )
        .def( "version", &QDataStream::version )
        .def( "writeBytes", &QDataStream::writeBytes,
                                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "writeRawData", &QDataStream::writeRawData )
    ;

    enum_<QDataStream::ByteOrder>("ByteOrder")
        .value( "BigEndian", QDataStream::BigEndian )
        .value( "LittleEndian", QDataStream::LittleEndian )
    ;
    
    enum_<QDataStream::Status>("Status")
        .value( "Ok", QDataStream::Ok )
        .value( "ReadPastEnd", QDataStream::ReadPastEnd )
        .value( "ReadCorruptData", QDataStream::ReadCorruptData )
    ;
    
    enum_<QDataStream::Version>("Version")
        .value( "Qt_1_0", QDataStream::Qt_1_0 )
        .value( "Qt_2_0", QDataStream::Qt_2_0 )
        .value( "Qt_2_1", QDataStream::Qt_2_1 )
        .value( "Qt_3_0", QDataStream::Qt_3_0 )
        .value( "Qt_3_1", QDataStream::Qt_3_1 )
        .value( "Qt_3_3", QDataStream::Qt_3_3 )
        .value( "Qt_4_0", QDataStream::Qt_4_0 )
        .value( "Qt_4_1", QDataStream::Qt_4_1 )
    ;
}
