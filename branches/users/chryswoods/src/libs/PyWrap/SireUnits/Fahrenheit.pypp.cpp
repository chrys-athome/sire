// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "Fahrenheit.pypp.hpp"
#include "boost/python.hpp"
#include "sireunits_headers.h"

namespace bp = boost::python;

SireUnits::Fahrenheit __copy__(const SireUnits::Fahrenheit &other){ return SireUnits::Fahrenheit(other); }

const char* pvt_get_name(const SireUnits::Fahrenheit&){ return "SireUnits::Fahrenheit";}

void register_Fahrenheit_class(){

    bp::class_< SireUnits::Fahrenheit, bp::bases< SireUnits::Dimension::Temperature > >( "Fahrenheit" )    
        .def( bp::init< >() )    
        .def( bp::init< SireUnits::Dimension::DerivedUnit const & >(( bp::arg("unit") )) )    
        .def( 
            "convertFromInternal"
            , &::SireUnits::Fahrenheit::convertFromInternal
            , ( bp::arg("val") ) )    
        .def( 
            "convertToInternal"
            , &::SireUnits::Fahrenheit::convertToInternal
            , ( bp::arg("val") ) )    
        .def( bp::self * bp::other<SireUnits::Dimension::Unit>() )    
        .def( bp::self / bp::other<SireUnits::Dimension::Unit>() )    
        .def( "__copy__", &__copy__)    
        .def( "__str__", &pvt_get_name);

}
