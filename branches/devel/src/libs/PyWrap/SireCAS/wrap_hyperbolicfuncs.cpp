
/**
  * This file contains the boost::python wrapping of the hyperbolic trig functions
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireCAS/hyperbolicfuncs.h"
#include "SireCAS/invhyperbolicfuncs.h"

using namespace boost::python;

namespace SireCAS
{

void  
SIRECAS_EXPORT
export_HyperbolicFuncs()
{

    class_<Cosh, bases<SingleFunc> >("Cosh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<Sinh, bases<SingleFunc> >("Sinh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Tanh, bases<SingleFunc> >("Tanh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Csch, bases<SingleFunc> >("Csch", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Sech, bases<SingleFunc> >("Sech", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Coth, bases<SingleFunc> >("ArcCoth", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<ArcCosh, bases<SingleFunc> >("ArcCosh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<ArcSinh, bases<SingleFunc> >("ArcSinh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcTanh, bases<SingleFunc> >("ArcTanh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcCsch, bases<SingleFunc> >("ArcCsch", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcSech, bases<SingleFunc> >("ArcSech", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcCoth, bases<SingleFunc> >("ArcCoth", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

}

}
