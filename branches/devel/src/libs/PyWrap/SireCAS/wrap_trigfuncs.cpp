
/**
  * This file contains the boost::python wrapping of the trigonometry functions
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireCAS/trigfuncs.h"
#include "SireCAS/invtrigfuncs.h"

using namespace boost::python;

namespace SireCAS
{

void export_TrigFuncs()
{

    class_<SingleFunc, bases<ExBase>, boost::noncopyable>("SingleFunc", no_init)
        .def( "argument", &SingleFunc::argument, 
                        return_value_policy<copy_const_reference>() )
    ;
    
    class_<Cos, bases<SingleFunc> >("Cos", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<Sin, bases<SingleFunc> >("Sin", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Tan, bases<SingleFunc> >("Tan", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Csc, bases<SingleFunc> >("Csc", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Sec, bases<SingleFunc> >("Sec", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Cot, bases<SingleFunc> >("Cot", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<ArcCos, bases<SingleFunc> >("ArcCos", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<ArcSin, bases<SingleFunc> >("ArcSin", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcTan, bases<SingleFunc> >("ArcTan", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcCsc, bases<SingleFunc> >("ArcCsc", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcSec, bases<SingleFunc> >("ArcSec", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcCot, bases<SingleFunc> >("ArcCot", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

}

}
