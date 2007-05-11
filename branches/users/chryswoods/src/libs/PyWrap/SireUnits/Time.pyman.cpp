
#include "Time.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const Time &val){ return str(double(val));}

void register_Time_class()
{
    class_< Time, bases<Unit> >("Time", init<>())
        .def( init<double>() )
        .def( init<const Time&>() )
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
        .def( "to", (double (Time::*)(const Time&) const) &Time::in )
    ;

}

