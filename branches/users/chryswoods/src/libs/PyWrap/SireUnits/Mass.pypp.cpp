// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "Mass.pypp.hpp"
#include "boost/python.hpp"
#include "sireunits_headers.h"

namespace bp = boost::python;

SireUnits::Dimension::Mass __copy__(const SireUnits::Dimension::Mass &other){ return SireUnits::Dimension::Mass(other); }

const char* pvt_get_name(const SireUnits::Dimension::Mass&){ return "SireUnits::Dimension::Mass";}

void register_Mass_class(){

    bp::class_< SireUnits::Dimension::Mass, bp::bases< SireUnits::Dimension::Unit > >( "Mass", bp::init< double >(( bp::arg("scale_factor") )) )    
        .def( bp::other< double >() * bp::self )    
        .def( bp::other< double >() / bp::self )    
        .def( "__copy__", &__copy__)    
        .def( "__str__", &pvt_get_name);

}
