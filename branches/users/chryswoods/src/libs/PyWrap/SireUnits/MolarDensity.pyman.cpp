
#include "MolarDensity.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const MolarDensity &val){ return str(double(val));}

void register_MolarDensity_class()
{
    class_< MolarDensity, bases<Unit> >("MolarDensity", init<>())
        .def( init<double>() )
        .def( init<const MolarDensity&>() )
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
        .def( "to", (double (MolarDensity::*)(const MolarDensity&) const) &MolarDensity::in )
    ;

}

