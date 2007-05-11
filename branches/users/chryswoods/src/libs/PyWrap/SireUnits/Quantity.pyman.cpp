
#include "Quantity.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

str _pvt_get_name(const Quantity &val){ return str(double(val));}

void register_Quantity_class()
{
    class_< Quantity, bases<Unit> >("Quantity", init<>())
        .def( init<double>() )
        .def( init<const Quantity&>() )
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
        .def( "to", (double (Quantity::*)(const Quantity&) const) &Quantity::in )
    ;

}

