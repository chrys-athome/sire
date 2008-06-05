
/**
  * This file contains the boost::python wrapping of Values 
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

void export_Values()
{
    void (Values::*wrap_add0)(const SymbolValue&)
                    = &Values::add;
    void (Values::*wrap_add1)(const SymbolValue&, const SymbolValue&)
                    = &Values::add;
    void (Values::*wrap_add2)(const SymbolValue&, const SymbolValue&, const SymbolValue&)
                    = &Values::add;
    void (Values::*wrap_add3)(const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&)
                    = &Values::add;
    void (Values::*wrap_add4)(const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&, const SymbolValue&)
                    = &Values::add;
    void (Values::*wrap_add5)(const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&, const SymbolValue&, const SymbolValue&)
                    = &Values::add;
    void (Values::*wrap_add6)(const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&)
                    = &Values::add;
    void (Values::*wrap_add7)(const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&, const SymbolValue&)
                    = &Values::add;
    void (Values::*wrap_add8)(const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&, const SymbolValue&, const SymbolValue&)
                    = &Values::add;
    void (Values::*wrap_add9)(const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&, const SymbolValue&, const SymbolValue&,
                              const SymbolValue&)
                    = &Values::add;
    
    class_<Values>("Values", init<>())
        .def( init<const QList<SymbolValue>&>() )
        .def( init<const SymbolValue&>() )
        .def( init<const SymbolValue&, const SymbolValue&>() )
        .def( init<const SymbolValue&, const SymbolValue&, const SymbolValue&>() )
        .def( init<const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&>() )
        .def( init<const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&, const SymbolValue&>() )
        .def( init<const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&, const SymbolValue&, const SymbolValue&>() )
        .def( init<const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&>() )
        .def( init<const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&, const SymbolValue&>() )
        .def( init<const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&, const SymbolValue&, const SymbolValue&>() )
        .def( init<const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&, const SymbolValue&, const SymbolValue&,
                   const SymbolValue&>() )
        .def( "set", &Values::set )
        .def( "value", &Values::value )
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

    //allow implicit conversion from tuple(SymbolValue) to QList<SymbolValue>
    SirePy::convert_python_tuple_to_container< QList<SymbolValue> >();
    
    //allow implicit conversion from QList<SymbolValue> to Values
    implicitly_convertible<QList<SymbolValue>,Values>();

}

}
