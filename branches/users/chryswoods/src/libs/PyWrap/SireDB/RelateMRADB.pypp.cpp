// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "RelateMRADB.pypp.hpp"
#include "boost/python.hpp"
#include "siredb_headers.h"
#include "SireCAS/function.h"
#include "SireCAS/expression.h"

namespace bp = boost::python;

#include "SirePy/str.hpp"

void register_RelateMRADB_class(){

    bp::class_< SireDB::RelateMRADB, bp::bases< SireDB::RelationshipDB > >( "RelateMRADB" )    
        .def( bp::init< >() )    
        .def( 
            "add"
            , (::SireDB::RelateID ( ::SireDB::RelateMRADB::* )( ::SireDB::MatchMRAData const &,::uint ) )( &::SireDB::RelateMRADB::add )
            , ( bp::arg("match"), bp::arg("n") ) )    
        .def( 
            "add"
            , (::SireDB::RelateID ( ::SireDB::RelateMRADB::* )( ::SireDB::MatchMRA const & ) )( &::SireDB::RelateMRADB::add )
            , ( bp::arg("match") ) )    
        .def( 
            "get"
            , (::SireDB::RelateID ( ::SireDB::RelateMRADB::* )( ::SireDB::MatchMRAData const &,::uint ) )( &::SireDB::RelateMRADB::get )
            , ( bp::arg("match"), bp::arg("n") ) )    
        .def( 
            "get"
            , (::SireDB::RelateID ( ::SireDB::RelateMRADB::* )( ::SireDB::MatchMRA const & ) )( &::SireDB::RelateMRADB::get )
            , ( bp::arg("match") ) )    
        .def( 
            "get"
            , (::SireDB::MatchMRAData ( ::SireDB::RelateMRADB::* )( ::SireDB::RelateID,::uint ) )( &::SireDB::RelateMRADB::get )
            , ( bp::arg("relateid"), bp::arg("n") ) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireDB::RelateMRADB::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireDB::RelateMRADB::* )(  ) const)( &::SireDB::RelateMRADB::what ) )    
        .staticmethod( "typeName" )    
        .def( "__str__", &SirePy::__str__< ::SireDB::RelateMRADB > );

}
