
#include "MolarCharge.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const MolarCharge &val){ return str(double(val));}

void register_MolarCharge_class()
{
    class_< MolarCharge, bases<Unit> >("MolarCharge", init<>())
        .def( init<double>() )
        .def( init<const MolarCharge&>() )
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
        .def( "to", (double (MolarCharge::*)(const MolarCharge&) const) &MolarCharge::in )
    ;

}

