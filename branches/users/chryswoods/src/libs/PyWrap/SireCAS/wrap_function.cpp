
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
        .def( init<const QString&>() )
        .def( "name", &Function::name )
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
