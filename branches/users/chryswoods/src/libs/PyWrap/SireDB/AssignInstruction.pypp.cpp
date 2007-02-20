// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "AssignInstruction.pypp.hpp"
#include "boost/python.hpp"
#include "siredb_headers.h"
#include "SireCAS/function.h"
#include "SireCAS/expression.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireDB::AssignInstruction&){ return "SireDB::AssignInstruction";}

void register_AssignInstruction_class(){

    bp::class_< SireDB::AssignInstruction, boost::noncopyable >( "AssignInstruction", bp::no_init )    
        .def( 
            "what"
            , (char const * ( ::SireDB::AssignInstruction::* )(  ) const)( &::SireDB::AssignInstruction::what ) )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireDB::AssignInstruction >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireDB::AssignInstruction >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
