
#include "Acceleration.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const Acceleration &val){ return str(double(val));}

void register_Acceleration_class()
{
    class_< Acceleration, bases<Unit> >("Acceleration", init<>())
        .def( init<double>() )
        .def( init<const Acceleration&>() )
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
        .def( "to", (double (Acceleration::*)(const Acceleration&) const) &Acceleration::in )
    ;

}

