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

/**
  * This file contains the boost::python wrapping of the Function class
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireCAS/qhash_sirecas.h"

#include <QString>

#include "SireCAS/function.h"
#include "SireCAS/symbols.h"
#include "SireCAS/functions.h"
#include "SireCAS/symbolvalue.h"
#include "SireCAS/symbolcomplex.h"
#include "SireCAS/identities.h"
#include "SireCAS/symbolexpression.h"
#include "SireCAS/values.h"
#include "SireCAS/integrationconstant.h"
#include "SireCAS/complexvalues.h"

using namespace boost::python;

namespace SireCAS
{

void
SIRECAS_EXPORT
export_Function()
{
    Function (Function::*wrap_call)(const Symbols&) const = &Function::operator();

    Function (Function::*wrap_call0)(const Symbol&
                                    ) const = &Function::operator();
    Function (Function::*wrap_call1)(const Symbol&, const Symbol&
                                    ) const = &Function::operator();
    Function (Function::*wrap_call2)(const Symbol&, const Symbol&, const Symbol&
                                    ) const = &Function::operator();
    Function (Function::*wrap_call3)(const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&
                                    ) const = &Function::operator();
    Function (Function::*wrap_call4)(const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&, const Symbol&
                                    ) const = &Function::operator();
    Function (Function::*wrap_call5)(const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&, const Symbol&, const Symbol&
                                    ) const = &Function::operator();
    Function (Function::*wrap_call6)(const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&
                                    ) const = &Function::operator();
    Function (Function::*wrap_call7)(const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&, const Symbol&
                                    ) const = &Function::operator();
    Function (Function::*wrap_call8)(const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&, const Symbol&, const Symbol&
                                    ) const = &Function::operator();
    Function (Function::*wrap_call9)(const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&, const Symbol&, const Symbol&,
                                     const Symbol&
                                    ) const = &Function::operator();

    class_<Function, bases<Symbol> >("Function", init<>())
        .def( init<const QString&,
                   optional<const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&>
                  >() )
        .def( init<const QString&, const Symbols&>() )
        .def( "name", &Function::name )
        .def( "__call__", wrap_call )
        .def( "__call__", wrap_call0 )
        .def( "__call__", wrap_call1 )
        .def( "__call__", wrap_call2 )
        .def( "__call__", wrap_call3 )
        .def( "__call__", wrap_call4 )
        .def( "__call__", wrap_call5 )
        .def( "__call__", wrap_call6 )
        .def( "__call__", wrap_call7 )
        .def( "__call__", wrap_call8 )
        .def( "__call__", wrap_call9 )
    ;

    void (Functions::*wrap_insert1)(const Function&) = &Functions::insert;
    void (Functions::*wrap_insert2)(const Functions&) = &Functions::insert;

    class_<Functions>("Functions", init<>())
        .def( init<const Function&>() )
        .def( "insert", wrap_insert1 )
        .def( "insert", wrap_insert2 )
        .def( "functionNames", &Functions::functionNames )
    ;
}

}
