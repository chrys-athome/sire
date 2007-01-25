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

#include <boost/python.hpp>

#include "SireFF/qhash_sireff.h"

#include "SireFF/parametermap.h"

#include "SireQt/qlist.hpp"

using namespace boost::python;

using namespace SireQt;

namespace SireFF
{

void SIREFF_EXPORT export_ParameterMap()
{
    class_<ParameterName>( "ParameterName", init<>() )
        .def( init<const QString&>() )
        .def( init<const QString&,const QString&>() )
        .def( init<const ParameterName&>() )

        .def( self == other<QString>() )

        .def( "isValid", &ParameterName::isValid )
        .def( "name", &ParameterName::name,
                          return_value_policy<copy_const_reference>() )

        .def( "defaultSource", &ParameterName::defaultSource,
                          return_value_policy<copy_const_reference>() )
    ;

    class_<ParameterSource, bases<ParameterName> >( "ParameterSource", init<>() )

        .def( init<const ParameterSource&>() )

        .def( "isDefault", &ParameterSource::isDefault )
        .def( "source", &ParameterSource::source,
                          return_value_policy<copy_const_reference>() )
    ;

    wrap_QList<ParameterSource>("QList_ParameterSource_");

    class_<ParameterMap>( "ParameterMap", init<>() )

        .def( init<const ParameterSource&>() )
        .def( init<const QList<ParameterSource>&>() )

        .def( init<const ParameterMap&>() )

        .def( "__and__", &ParameterMap::operator&& )

        .def( "source", &ParameterMap::source )
    ;

    implicitly_convertible<ParameterSource, ParameterMap>();
    implicitly_convertible<QList<ParameterSource>, ParameterMap>();
}

}
