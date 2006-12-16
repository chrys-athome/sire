
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
