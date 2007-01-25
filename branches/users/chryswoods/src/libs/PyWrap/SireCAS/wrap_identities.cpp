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
  * This file contains the boost::python wrapping of Identities
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireCAS/symbol.h"
#include "SireCAS/symbols.h"
#include "SireCAS/symbolvalue.h"
#include "SireCAS/identities.h"
#include "SireCAS/symbolexpression.h"
#include "SireCAS/values.h"

#include "SirePy/converttuple.hpp"

using namespace boost::python;

namespace SireCAS
{

void  
SIRECAS_EXPORT
export_Identities()
{
    void (Identities::*wrap_iadd0)(const SymbolExpression&)
                      = &Identities::add;
    void (Identities::*wrap_iadd1)(const SymbolExpression&, const SymbolExpression&)
                      = &Identities::add;
    void (Identities::*wrap_iadd2)(const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&)
                      = &Identities::add;
    void (Identities::*wrap_iadd3)(const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&)
                      = &Identities::add;
    void (Identities::*wrap_iadd4)(const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&)
                      = &Identities::add;
    void (Identities::*wrap_iadd5)(const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&)
                      = &Identities::add;
    void (Identities::*wrap_iadd6)(const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&)
                      = &Identities::add;
    void (Identities::*wrap_iadd7)(const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&)
                      = &Identities::add;
    void (Identities::*wrap_iadd8)(const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&)
                      = &Identities::add;
    void (Identities::*wrap_iadd9)(const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&,
                                   const SymbolExpression&, const SymbolExpression&)
                      = &Identities::add;

    class_<Identities>("Identities", init<>())
        .def( init<const QList<SymbolExpression>&>() )
        .def( init<const SymbolExpression&>() )
        .def( init<const SymbolExpression&, const SymbolExpression&>() )
        .def( init<const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&>() )
        .def( init<const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&>() )
        .def( init<const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&>() )
        .def( init<const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&>() )
        .def( init<const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&>() )
        .def( init<const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&>() )
        .def( init<const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&>() )
        .def( init<const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&,
                   const SymbolExpression&, const SymbolExpression&>() )
        .def( "expression", &Identities::expression )
        .def( "set", &Identities::set )
        .def( "add", wrap_iadd0 )
        .def( "add", wrap_iadd1 )
        .def( "add", wrap_iadd2 )
        .def( "add", wrap_iadd3 )
        .def( "add", wrap_iadd4 )
        .def( "add", wrap_iadd5 )
        .def( "add", wrap_iadd6 )
        .def( "add", wrap_iadd7 )
        .def( "add", wrap_iadd8 )
        .def( "add", wrap_iadd9 )
    ;

    //allow implicit conversion from tuple<SymbolExpression> to QList<SymbolExpression>
    SirePy::convert_python_tuple_to_container< QList<SymbolExpression> >();
    
    //allow implicit conversion from QList<SymbolExpression> to Identities
    implicitly_convertible<QList<SymbolExpression>,Identities>();
}

}
