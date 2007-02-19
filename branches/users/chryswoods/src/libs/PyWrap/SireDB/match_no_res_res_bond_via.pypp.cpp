// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "match_no_res_res_bond_via.pypp.hpp"
#include "boost/python.hpp"
#include "siredb_headers.h"
#include "SireCAS/function.h"
#include "SireCAS/expression.h"

namespace bp = boost::python;

const char* pvt_get_name(const SireDB::match_no_res_res_bond_via&){ return "SireDB::match_no_res_res_bond_via";}

void register_match_no_res_res_bond_via_class(){

    bp::class_< SireDB::match_no_res_res_bond_via >( "match_no_res_res_bond_via" )    
        .def( bp::init< >() )    
        .def( bp::init< uint >(( bp::arg("atmid") )) )    
        .def( bp::self == bp::other< QString >() )    
        .def( "__str__", &pvt_get_name);

}
