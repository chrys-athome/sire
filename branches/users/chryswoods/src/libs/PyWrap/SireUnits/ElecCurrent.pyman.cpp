
#include "ElecCurrent.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const ElecCurrent &val){ return str(double(val));}

void register_ElecCurrent_class()
{
    class_< ElecCurrent, bases<Unit> >("ElecCurrent", init<>())
        .def( init<double>() )
        .def( init<const ElecCurrent&>() )
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
        .def( "to", (double (ElecCurrent::*)(const ElecCurrent&) const) &ElecCurrent::in )
    ;

}

