
#include "Power.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const Power &val){ return str(double(val));}

void register_Power_class()
{
    class_< Power, bases<Unit> >("Power", init<>())
        .def( init<double>() )
        .def( init<const Power&>() )
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
        .def( "to", (double (Power::*)(const Power&) const) &Power::in )
    ;

}

