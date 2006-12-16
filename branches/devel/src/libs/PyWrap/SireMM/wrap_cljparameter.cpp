
/**
  * This file contains the boost::python wrapping of CLJParameter, LJParameter and CLJPair
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMM/cljparameter.h"
#include "SireMM/cljpair.h"

#include "SirePy/str.hpp"

using namespace boost::python;
using namespace SirePy;

namespace SireMM
{

void  
SIREMM_EXPORT
export_CLJParameter()
{
    class_<ChargeParameter>("ChargeParameter", init<>())
        .def( init<double>() )
        .def( "isDummy", &ChargeParameter::isDummy )
        .def( "zeroCharge", &ChargeParameter::zeroCharge )
        .def( "charge", &ChargeParameter::charge )
        .def( "__str__", &__str__<ChargeParameter> )
        .def( "toString", &ChargeParameter::toString )
        .def( self == self )
        .def( self != self )
        .def( "dummy", &ChargeParameter::dummy ).staticmethod("dummy")
    ;

    class_<LJParameter>("LJParameter", init<>())
        .def(init<double, double>())
        .def(init<const LJParameter&>())
        .def("isDummy", &LJParameter::isDummy)
        .def("zeroLJ", &LJParameter::zeroLJ)
        .def("sigma", &LJParameter::sigma)
        .def("epsilon", &LJParameter::epsilon)
        .def("sqrtEpsilon", &LJParameter::sqrtEpsilon)
        .def("sqrtSigma", &LJParameter::sqrtSigma)
        .def("A", &LJParameter::A)
        .def("B", &LJParameter::B)
        .def("rmin", &LJParameter::rmin)
        .def("toString", &LJParameter::toString)
        .def("__str__", &__str__<LJParameter>)
        .def(self == self)
        .def(self != self)
        .def("dummy", &LJParameter::dummy).staticmethod("dummy")
        .def("fromSigmaAndEpsilon", &LJParameter::fromSigmaAndEpsilon)
                             .staticmethod("fromSigmaAndEpsilon")
        .def("fromAAndB", &LJParameter::fromAAndB).staticmethod("fromAAndB")
        .def("fromRMinAndEpsilon", &LJParameter::fromRMinAndEpsilon)
                             .staticmethod("fromRMinAndEpsilon")
    ;
   
    class_<CLJParameter, bases<ChargeParameter,LJParameter> >("CLJParameter", init<>())
        .def( init<const ChargeParameter&, double, double>() )
        .def( init<double, double, double>() )
        .def( init<const ChargeParameter&, const LJParameter&>() )
        .def( init<double, const LJParameter&>() )
        .def("toString", &CLJParameter::toString)
        .def("__str__", &CLJParameter::toString)
        .def(self == self)
        .def(self != self)
        .def("isDummy", &CLJParameter::isDummy)
        .def("dummy", &CLJParameter::dummy).staticmethod("dummy")
    ;
    
    //allow a double to be automatically converted into a ChargeParameter
    implicitly_convertible<double, ChargeParameter>();
    
    class_<CLJPair>("CLJPair", init<>())
        .def(init<double, double, double>())
        .def("__str__", &CLJPair::toString)
        .def("toString", &CLJPair::toString)
        .def("charge2", &CLJPair::charge2 )
        .def("sigma", &CLJPair::sigma )
        .def("epsilon", &CLJPair::epsilon )
        .def("A", &CLJPair::A)
        .def("B", &CLJPair::B)
        .def("rmin", &CLJPair::rmin)
        .def("arithmetic", &CLJPair::arithmetic).staticmethod("arithmetic")
        .def("geometric", &CLJPair::geometric).staticmethod("geometric")
    ;
}

}
