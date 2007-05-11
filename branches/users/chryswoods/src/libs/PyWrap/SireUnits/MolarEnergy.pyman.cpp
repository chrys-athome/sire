
#include "MolarEnergy.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const MolarEnergy &val){ return str(double(val));}

void register_MolarEnergy_class()
{
    class_< MolarEnergy, bases<Unit> >("MolarEnergy", init<>())
        .def( init<double>() )
        .def( init<const MolarEnergy&>() )
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
        .def( "to", (double (MolarEnergy::*)(const MolarEnergy&) const) &MolarEnergy::in )
    ;

}

