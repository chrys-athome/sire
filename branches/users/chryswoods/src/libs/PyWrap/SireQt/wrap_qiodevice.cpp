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

#include <QIODevice>

#include "sireglobal.h"

using namespace boost::python;

/** Provide a basic wrapping of QIODevice so that it can be seen as the common
    base of the other IO device classes. */
void  
SIREQT_EXPORT
export_QIODevice()
{
    scope QIODevice_scope = 
    class_<QIODevice, boost::noncopyable>("QIODevice", no_init)
        .def( "atEnd", &QIODevice::atEnd )
        .def( "bytesAvailable", &QIODevice::bytesAvailable )
        .def( "bytesToWrite", &QIODevice::bytesToWrite )
        .def( "canReadLine", &QIODevice::canReadLine )
        .def( "close", &QIODevice::close )
        .def( "errorString", &QIODevice::errorString )
        .def( "getChar", &QIODevice::getChar )
        .def( "isOpen", &QIODevice::isOpen )
        .def( "isReadable", &QIODevice::isReadable )
        .def( "isSequential", &QIODevice::isSequential )
        .def( "isTextModeEnabled", &QIODevice::isTextModeEnabled )
        .def( "isWritable", &QIODevice::isWritable )
        .def( "open", &QIODevice::open )
        .def( "openMode", &QIODevice::openMode )
        .def( "peek", (qint64 (QIODevice::*)(char*, qint64))
                          &QIODevice::peek )
        .def( "peek", (QByteArray (QIODevice::*)(qint64))
                          &QIODevice::peek )
        .def( "pos", &QIODevice::pos )
        .def( "putChar", &QIODevice::putChar )
        .def( "read", (qint64 (QIODevice::*)(char*, qint64))
                          &QIODevice::read )
        .def( "read", (QByteArray (QIODevice::*)(qint64))
                          &QIODevice::read )
        .def( "readAll", &QIODevice::readAll )
        .def( "readLine", (qint64 (QIODevice::*)(char*, qint64))
                          &QIODevice::readLine )
        .def( "readLine", (QByteArray (QIODevice::*)(qint64 maxSize))
                          &QIODevice::readLine )
        .def( "reset", &QIODevice::reset )
        .def( "seek", &QIODevice::seek )
        .def( "setTextModeEnabled", &QIODevice::setTextModeEnabled )
        .def( "size", &QIODevice::size )
        .def( "ungetChar", &QIODevice::ungetChar )
        .def( "waitForBytesWritten", &QIODevice::waitForBytesWritten )
        .def( "waitForReadyRead", &QIODevice::waitForReadyRead )
        .def( "write", (qint64 (QIODevice::*)(const char*, qint64))
                        &QIODevice::write )
        .def( "write", (qint64 (QIODevice::*)(const QByteArray&))
                        &QIODevice::write )
    ;

    enum_<QIODevice::OpenModeFlag>("OpenMode")
        .value( "NotOpen", QIODevice::NotOpen )
        .value( "ReadOnly", QIODevice::ReadOnly )
        .value( "WriteOnly", QIODevice::WriteOnly )
        .value( "ReadWrite", QIODevice::ReadWrite )
        .value( "Append", QIODevice::Append )
        .value( "Truncate", QIODevice::Truncate )
        .value( "Text", QIODevice::Text )
        .value( "Unbuffered", QIODevice::Unbuffered )
    ;

    implicitly_convertible< int, QFlags<QIODevice::OpenModeFlag> >();
    implicitly_convertible< QIODevice::OpenModeFlag, QFlags<QIODevice::OpenModeFlag> >();

}
