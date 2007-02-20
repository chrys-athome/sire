// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "AtomTypeTable.pypp.hpp"
#include "boost/python.hpp"
#include "siredb_headers.h"
#include "SireCAS/function.h"
#include "SireCAS/expression.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireDB::AtomTypeTable&){ return "SireDB::AtomTypeTable";}

void register_AtomTypeTable_class(){

    bp::class_< SireDB::AtomTypeTable, bp::bases< SireDB::AtomTableT<SireDB::AtomType> > >( "AtomTypeTable" )    
        .def( bp::init< >() )    
        .def( bp::init< SireMol::MoleculeInfo const & >(( bp::arg("molinfo") )) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireDB::AtomTypeTable::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireDB::AtomTypeTable::* )(  ) const)( &::SireDB::AtomTypeTable::what ) )    
        .staticmethod( "typeName" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireDB::AtomTypeTable >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireDB::AtomTypeTable >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
