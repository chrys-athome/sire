
#include "Velocity.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const Velocity &val){ return str(double(val));}

void register_Velocity_class()
{
    class_< Velocity, bases<Unit> >("Velocity", init<>())
        .def( init<double>() )
        .def( init<const Velocity&>() )
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
        .def( "to", (double (Velocity::*)(const Velocity&) const) &Velocity::in )
    ;

}

