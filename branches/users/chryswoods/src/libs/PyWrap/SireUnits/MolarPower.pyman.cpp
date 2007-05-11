
#include "MolarPower.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const MolarPower &val){ return str(double(val));}

void register_MolarPower_class()
{
    class_< MolarPower, bases<Unit> >("MolarPower", init<>())
        .def( init<double>() )
        .def( init<const MolarPower&>() )
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
        .def( "to", (double (MolarPower::*)(const MolarPower&) const) &MolarPower::in )
    ;

}

