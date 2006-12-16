
/**
  * This file contains the boost::python wrapping of ComplexValues 
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
#include "SireCAS/symbolcomplex.h"
#include "SireCAS/complexvalues.h"

using namespace boost::python;

namespace SireCAS
{

void 
SIRECAS_EXPORT
export_ComplexValues()
{
    void (ComplexValues::*wrap_add0)(const SymbolComplex&)
                    = &ComplexValues::add;
    void (ComplexValues::*wrap_add1)(const SymbolComplex&, const SymbolComplex&)
                    = &ComplexValues::add;
    void (ComplexValues::*wrap_add2)(const SymbolComplex&, const SymbolComplex&, const SymbolComplex&)
                    = &ComplexValues::add;
    void (ComplexValues::*wrap_add3)(const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&)
                    = &ComplexValues::add;
    void (ComplexValues::*wrap_add4)(const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&, const SymbolComplex&)
                    = &ComplexValues::add;
    void (ComplexValues::*wrap_add5)(const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&, const SymbolComplex&, const SymbolComplex&)
                    = &ComplexValues::add;
    void (ComplexValues::*wrap_add6)(const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&)
                    = &ComplexValues::add;
    void (ComplexValues::*wrap_add7)(const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&, const SymbolComplex&)
                    = &ComplexValues::add;
    void (ComplexValues::*wrap_add8)(const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&, const SymbolComplex&, const SymbolComplex&)
                    = &ComplexValues::add;
    void (ComplexValues::*wrap_add9)(const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                              const SymbolComplex&)
                    = &ComplexValues::add;
    
    class_<ComplexValues>("ComplexValues", init<>())
        .def( init<const SymbolComplex&>() )
        .def( init<const SymbolComplex&, const SymbolComplex&>() )
        .def( init<const SymbolComplex&, const SymbolComplex&, const SymbolComplex&>() )
        .def( init<const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&>() )
        .def( init<const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&, const SymbolComplex&>() )
        .def( init<const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&, const SymbolComplex&, const SymbolComplex&>() )
        .def( init<const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&>() )
        .def( init<const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&, const SymbolComplex&>() )
        .def( init<const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&, const SymbolComplex&, const SymbolComplex&>() )
        .def( init<const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&, const SymbolComplex&, const SymbolComplex&,
                   const SymbolComplex&>() )
        .def( init<const Values&>() )
        .def( "set", &ComplexValues::set )
        .def( "value", &ComplexValues::value )
        .def( "add", wrap_add0 )
        .def( "add", wrap_add1 )
        .def( "add", wrap_add2 )
        .def( "add", wrap_add3 )
        .def( "add", wrap_add4 )
        .def( "add", wrap_add5 )
        .def( "add", wrap_add6 )
        .def( "add", wrap_add7 )
        .def( "add", wrap_add8 )
        .def( "add", wrap_add9 )
    ;

}

}
