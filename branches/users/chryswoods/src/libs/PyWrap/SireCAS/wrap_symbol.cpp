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
  * This file contains the boost::python wrapping of the Symbol classes 
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
#include "SireCAS/symbolcomplex.h"
#include "SireCAS/identities.h"
#include "SireCAS/symbolexpression.h"
#include "SireCAS/values.h"
#include "SireCAS/integrationconstant.h"
#include "SireCAS/complexvalues.h"
#include "SireCAS/function.h"

using namespace boost::python;

namespace SireCAS
{

void  
SIRECAS_EXPORT
export_Symbol()
{
    class_<Symbol, bases<ExBase> >("Symbol", init<>())
        .def( init<const QString&>() )
        .def( "ID", &Symbol::ID )
        .def( self == double() )
        .def( self == Complex() )
        .def( self == int() )
        .def( self == Expression() )
    ;
    
    class_<IntegrationConstant, bases<Symbol> >("IntegrationConstant", init<>())
    ;
    
    void (Symbols::*wrap_insert1)(const Symbol&) = &Symbols::insert;
    void (Symbols::*wrap_insert2)(const Symbols&) = &Symbols::insert;
    
    class_<Symbols>("Symbols", init<>())
        .def( init<const Symbol&>() )
        .def( "insert", wrap_insert1 )
        .def( "insert", wrap_insert2 )
    ;

    class_<SymbolValue>("SymbolValue", init<SymbolID,double>())
        .def( "ID", &SymbolValue::ID )
        .def( "value", &SymbolValue::value )
    ;
    
    implicitly_convertible<SymbolValue,Values>();
    
    class_<SymbolComplex>("SymbolComplex", init<SymbolID,const Complex&>())
        .def( init<SymbolID,double>() )
        .def( init<const SymbolValue&>() )
        .def( "ID", &SymbolComplex::ID )
        .def( "value", &SymbolComplex::value, return_value_policy<copy_const_reference>() )
    ;
    
    implicitly_convertible<SymbolComplex,ComplexValues>();
    
    class_<SymbolExpression>("SymbolExpression", init<const Symbol&, const Expression&>())
        .def( "symbol", &SymbolExpression::symbol, return_value_policy<copy_const_reference>() )
        .def( "isFunction", &SymbolExpression::isFunction )
        .def( "function", &SymbolExpression::function, 
                                return_value_policy<copy_const_reference>())
        .def( "expression", &SymbolExpression::expression, 
                                return_value_policy<copy_const_reference>())
    ;
    
    implicitly_convertible<SymbolExpression,Identities>();
}

}
