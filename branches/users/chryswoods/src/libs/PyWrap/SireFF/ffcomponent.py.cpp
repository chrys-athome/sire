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

#include "SireCAS/qhash_sirecas.h"

#include "SireFF/ffcomponent.h"

#include "SireFF/ffbase.h"

using namespace boost::python;

using namespace SireCAS;

namespace SireFF
{

void SIREFF_EXPORT export_FFComponent()
{
    class_<FFComponent, bases<Function> >("FFComponent", init<>())

        .def( init<const FFBase&,
                   optional<const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&>
                  >() )
        .def( init<const FFBase&, const Symbols&>() )

        .def( init<const FFBase&, const QString&,
                   optional<const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&>
                  >() )
        .def( init<const FFBase&, const QString&, const Symbols&>() )

        .def( init<const Function&>() )
        .def( init<const FFComponent&>() )

        .def( "forceFieldName", &FFComponent::forceFieldName )
        .def( "forceFieldComponent", &FFComponent::forceFieldComponent )

        .def( "forceFieldID", &FFComponent::forceFieldID )
    ;

    implicitly_convertible<Function, FFComponent>();
}

}
