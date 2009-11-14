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
#include "magic_error.h"

using namespace Siren;

static const RegisterMetaType<magic_error> r_magic_error( 189316926129508445, 1 );

magic_error::magic_error() : ImplementsException<magic_error,exception>()
{}

magic_error::magic_error(QString err, QString place)
            : ImplementsException<magic_error,exception>(err, place)
{}

static QString errorString(QString type_name, CLASS_UID wrongid, CLASS_UID rightid)
{
    return QObject::tr( "The magic number (%1) is not the correct magic number "
                        "for the class %2 (the correct number is %3).")
                            .arg(type_name).arg(wrongid).arg(rightid);
}

magic_error::magic_error(CLASS_UID wrongid, const detail::RegisterMetaTypeBase &info,
                         QString place)
            : ImplementsException<magic_error,exception>(
                    ::errorString(info.typeName(), wrongid, info.UID()), place )
{}

magic_error::magic_error(CLASS_UID wrongid, const Class &c, QString place)
            : ImplementsException<magic_error,exception>(
                    ::errorString(c.name(), wrongid, c.UID()), place )
{}
            
magic_error::magic_error(const magic_error &other)
            : ImplementsException<magic_error,exception>(other)
{}

magic_error::~magic_error() throw()
{}
