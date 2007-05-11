
#include "Pressure.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const Pressure &val){ return str(double(val));}

void register_Pressure_class()
{
    class_< Pressure, bases<Unit> >("Pressure", init<>())
        .def( init<double>() )
        .def( init<const Pressure&>() )
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
        .def( "to", (double (Pressure::*)(const Pressure&) const) &Pressure::in )
    ;

}

