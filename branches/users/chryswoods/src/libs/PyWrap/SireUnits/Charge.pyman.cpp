
#include "Charge.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const Charge &val){ return str(double(val));}

void register_Charge_class()
{
    class_< Charge, bases<Unit> >("Charge", init<>())
        .def( init<double>() )
        .def( init<const Charge&>() )
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
        .def( "to", (double (Charge::*)(const Charge&) const) &Charge::in )
    ;

}

