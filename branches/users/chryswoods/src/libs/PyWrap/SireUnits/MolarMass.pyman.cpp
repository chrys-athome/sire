
#include "MolarMass.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const MolarMass &val){ return str(double(val));}

void register_MolarMass_class()
{
    class_< MolarMass, bases<Unit> >("MolarMass", init<>())
        .def( init<double>() )
        .def( init<const MolarMass&>() )
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
        .def( "to", (double (MolarMass::*)(const MolarMass&) const) &MolarMass::in )
    ;

}

