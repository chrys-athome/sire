// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "match_single_atom_num.pypp.hpp"
#include "boost/python.hpp"
#include "siredb_headers.h"
#include "SireCAS/function.h"
#include "SireCAS/expression.h"

namespace bp = boost::python;

const char* pvt_get_name(const SireDB::match_single_atom_num&){ return "SireDB::match_single_atom_num";}

void register_match_single_atom_num_class(){

    bp::class_< SireDB::match_single_atom_num >( "match_single_atom_num" )    
        .def( bp::init< >() )    
        .def( bp::self == bp::other< SireMol::AtomNum >() )    
        .def( "__str__", &pvt_get_name);

}
