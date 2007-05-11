
#include "ElecPotential.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const ElecPotential &val){ return str(double(val));}

void register_ElecPotential_class()
{
    class_< ElecPotential, bases<Unit> >("ElecPotential", init<>())
        .def( init<double>() )
        .def( init<const ElecPotential&>() )
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
        .def( "to", (double (ElecPotential::*)(const ElecPotential&) const) &ElecPotential::in )
    ;

}

