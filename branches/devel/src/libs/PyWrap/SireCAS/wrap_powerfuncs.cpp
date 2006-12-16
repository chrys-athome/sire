
/**
  * This file contains the boost::python wrapping of the power functions (incl. Exp and Ln)
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireCAS/power.h"
#include "SireCAS/exp.h"
#include "SireCAS/powerconstant.h"

using namespace boost::python;

namespace SireCAS
{

void  
SIRECAS_EXPORT
export_PowerFuncs()
{
    class_<PowerFunction, bases<ExBase>, boost::noncopyable>("PowerFunction", no_init)
        .def("core", &PowerFunction::core)
        .def("power", &PowerFunction::power)
        .def("reduce", &PowerFunction::reduce)
    ;

    class_<Power, bases<PowerFunction> >("Power", init<>())
        .def( init<const ExBase&, const ExBase&>() )
        .def( init<const Expression&, const Expression&>() )
        .def( init<const Expression&, const ExBase&>() )
        .def( init<const ExBase&, const Expression&>() )
    ;
    
    class_<PowerConstant, bases<PowerFunction> >("PowerConstant", init<>())
        .def( init<double, const Expression&>() )
        .def( init<double, const ExBase&>() )
    ;
    
    class_<ConstantPower, bases<PowerFunction>, boost::noncopyable >("ConstantPower", no_init)
    ;
    
    class_<IntegerPower, bases<ConstantPower> >("IntegerPower", init<>())
        .def( init<const ExBase&, int>() )
        .def( init<const Expression&, int>() )
    ;
    
    class_<RationalPower, bases<ConstantPower> >("RationalPower", init<>())
        .def( init<const ExBase&, const Rational&>() )
        .def( init<const Expression&, const Rational&>() )
    ;
    
    class_<RealPower, bases<ConstantPower> >("RealPower", init<>())
        .def( init<const ExBase&, double>() )
        .def( init<const Expression&, double>() )
    ;
    
    class_<ComplexPower, bases<ConstantPower> >("ComplexPower", init<>())
        .def( init<const ExBase&, const Complex&>() )
        .def( init<const Expression&, const Complex&>() )
    ;
    
    class_<Exp, bases<PowerFunction> >("Exp", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Ln, bases<SingleFunc> >("Ln", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

}

}
