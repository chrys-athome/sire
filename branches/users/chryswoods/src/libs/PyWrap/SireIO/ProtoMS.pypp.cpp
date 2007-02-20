// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ProtoMS.pypp.hpp"
#include "boost/python.hpp"
#include "sireio_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/editmol.h"
#include "SireDB/parameterdb.h"

namespace bp = boost::python;

const char* pvt_get_name(const SireIO::ProtoMS&){ return "SireIO::ProtoMS";}

void register_ProtoMS_class(){

    bp::class_< SireIO::ProtoMS >( "ProtoMS" )    
        .def( bp::init< >() )    
        .def( 
            "read"
            , (void ( ::SireIO::ProtoMS::* )( ::QString const &,::SireDB::ParameterDB & ) )( &::SireIO::ProtoMS::read )
            , ( bp::arg("filename"), bp::arg("ffdb") ) )    
        .def( "__str__", &pvt_get_name);

}
