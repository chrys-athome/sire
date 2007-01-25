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

#include <QFile>
#include <QByteArray>
#include <QString>

#include "sireglobal.h"

using namespace boost::python;

void  
SIREQT_EXPORT
export_QFile()
{
    scope QFile_scope = 
    class_<QFile, bases<QIODevice>, boost::noncopyable>("QFile", init<>())
        .def( init<const QString&>() )
        .def( "copy", (bool (QFile::*)(const QString&))
                      &QFile::copy )
        .def( "copy", (bool (*)(const QString&, const QString&))
                      &QFile::copy ).staticmethod("copy")
        .def( "error", &QFile::error )
        .def( "exists", (bool (QFile::*)() const)
                      &QFile::exists )
        .def( "exists", (bool (*)(const QString&))
                      &QFile::exists ).staticmethod("exists")
        .def( "fileName", &QFile::fileName )
        .def( "flush", &QFile::flush )
        .def( "handle", &QFile::handle )
        .def( "isSequential", &QFile::isSequential )
        .def( "link", (bool (QFile::*)(const QString&))
                      &QFile::link )
        .def( "link", (bool (*)(const QString&, const QString&))
                      &QFile::link ).staticmethod("link")
        .def( "permissions", (QFile::Permissions (QFile::*)() const)
                      &QFile::permissions )
        .def( "permissions", (QFile::Permissions (*)(const QString&))
                      &QFile::permissions ).staticmethod("permissions")
        .def( "readLink", (QString (QFile::*)() const)
                      &QFile::readLink )
        .def( "readLink", (QString (*)(const QString&))
                      &QFile::readLink ).staticmethod("readLink")
        .def( "remove", (bool (QFile::*)())
                      &QFile::remove )
        .def( "remove", (bool (*)(const QString&))
                      &QFile::remove ).staticmethod("remove")
        .def( "rename", (bool (QFile::*)(const QString&))
                      &QFile::rename )
        .def( "rename", (bool (*)(const QString&, const QString&))
                      &QFile::rename ).staticmethod("rename")
        .def( "resize", (bool (QFile::*)(qint64))
                      &QFile::resize )
        .def( "resize", (bool (*)(const QString&, qint64))
                      &QFile::resize ).staticmethod("resize")
        .def( "setFileName", &QFile::setFileName )
        .def( "setPermissions", (bool (QFile::*)(QFile::Permissions))
                      &QFile::setPermissions )
        .def( "setPermissions", (bool (*)(const QString&, QFile::Permissions))
                      &QFile::setPermissions ).staticmethod("setPermissions")
        .def( "size", &QFile::size )
        .def( "unsetError", &QFile::unsetError )
        
        .def( "decodeName", (QString (*)(const QByteArray&))
                     &QFile::decodeName )
        .def( "decodeName", (QString (*)(const char*))
                     &QFile::decodeName ).staticmethod("decodeName")
        .def( "encodeName", &QFile::encodeName ).staticmethod("encodeName")
    ;

    enum_<QFile::FileError>("FileError")
        .value( "NoError", QFile::NoError )
        .value( "ReadError", QFile::ReadError )
        .value( "WriteError", QFile::WriteError )
        .value( "FatalError", QFile::FatalError )
        .value( "ResourceError", QFile::ResourceError )
        .value( "OpenError", QFile::OpenError )
        .value( "AbortError", QFile::AbortError )
        .value( "TimeOutError", QFile::TimeOutError )
        .value( "UnspecifiedError", QFile::UnspecifiedError )
        .value( "RemoveError", QFile::RemoveError )
        .value( "RenameError", QFile::RenameError )
        .value( "PositionError", QFile::PositionError )
        .value( "ResizeError", QFile::ResizeError )
        .value( "PermissionsError", QFile::PermissionsError )
        .value( "CopyError", QFile::CopyError )
    ;

    enum_<QFile::Permission>("Permissions")
        .value( "ReadOwner", QFile::ReadOwner )
        .value( "WriteOwner", QFile::WriteOwner )
        .value( "ExeOwner", QFile::ExeOwner )
        .value( "ReadUser", QFile::ReadUser )
        .value( "WriteUser", QFile::WriteUser )
        .value( "ExeUser", QFile::ExeUser )
        .value( "ReadGroup", QFile::ReadGroup )
        .value( "WriteGroup", QFile::WriteGroup )
        .value( "ExeGroup", QFile::ExeGroup )
        .value( "ReadOther", QFile::ReadOther )
        .value( "WriteOther", QFile::WriteOther )
        .value( "ExeOther", QFile::ExeOther )
    ;
    
    implicitly_convertible<int, QFile::Permissions>();
    implicitly_convertible<QFile::Permission, QFile::Permissions>();

}
