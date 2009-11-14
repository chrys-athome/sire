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

#include "class.h"
#include "Siren/version_error.h"

using namespace Siren;

static const RegisterMetaType<version_error> r_verror( 17568356463543514470UL, 1 );

version_error::version_error() : ImplementsException<version_error,exception>()
{}

version_error::version_error(const QString &error, const QString place)
              : ImplementsException<version_error,exception>(error, place)
{}

static QString errorString(VERSION_ID wrongid,
                           QString supported_ids,
                           QString type_name)
{
    return QObject::tr("Version %1 of the class %2 is not supported. "
                       "Only version numbers [ %3 ] are supported.")
                    .arg(wrongid).arg(type_name, supported_ids);
}

static QString errorString(VERSION_ID wrongid, VERSION_ID supported_id, 
                           QString type_name)
{
    return ::errorString(wrongid, QString::number(supported_id), type_name);
}

version_error::version_error(VERSION_ID wrongid, 
                             const detail::RegisterMetaTypeBase &info,
                             QString place)
              : ImplementsException<version_error,exception>(
                        ::errorString(wrongid, info.version(), info.typeName()),
                        place )
{}

version_error::version_error(VERSION_ID wrongid, QString supported_ids,
                             const Class &c, QString place)
              : ImplementsException<version_error,exception>(
                        ::errorString(wrongid, supported_ids, c.name()), place )
{}

version_error::version_error(VERSION_ID wrongid, 
                             const Class &c, QString place)
              : ImplementsException<version_error,exception>(
                        ::errorString(wrongid, c.version(), c.name()), place )
{}

version_error::version_error(VERSION_ID wrongid, QString supported_ids,
                             const detail::RegisterMetaTypeBase &info,
                             QString place)
              : ImplementsException<version_error,exception>(
                        ::errorString(wrongid, supported_ids, info.typeName()),
                        place )
{}

version_error::version_error(const version_error &other)
              : ImplementsException<version_error,exception>(other)
{}

version_error::~version_error() throw()
{}
