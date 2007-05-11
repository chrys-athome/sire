
#include "MolarVolume.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const MolarVolume &val){ return str(double(val));}

void register_MolarVolume_class()
{
    class_< MolarVolume, bases<Unit> >("MolarVolume", init<>())
        .def( init<double>() )
        .def( init<const MolarVolume&>() )
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
        .def( "to", (double (MolarVolume::*)(const MolarVolume&) const) &MolarVolume::in )
    ;

}

