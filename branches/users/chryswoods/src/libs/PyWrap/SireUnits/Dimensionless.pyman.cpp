
#include "Dimensionless.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const Dimensionless &val){ return str(double(val));}

void register_Dimensionless_class()
{
    class_< Dimensionless, bases<Unit> >("Dimensionless", init<>())
        .def( init<double>() )
        .def( init<const Dimensionless&>() )
        .def( "__str__", &_pvt_get_name )
        .def( self == self )
        .def( self != self )
        .def( -self )
        .def( self + self )
        .def( self - self )
        .def( self += self )
        .def( self -= self )
        .def( self * other<double>() )
        .def( self / other<double>() )
        .def( other<double>() * self )
        .def( other<double>() / self )
        .def( "to", (double (Dimensionless::*)(const Dimensionless&) const) &Dimensionless::in )
    ;

}

