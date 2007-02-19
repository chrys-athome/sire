// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ParameterDB.pypp.hpp"
#include "boost/python.hpp"
#include "siredb_headers.h"
#include "SireCAS/function.h"
#include "SireCAS/expression.h"

namespace bp = boost::python;

#include "SirePy/str.hpp"

void register_ParameterDB_class(){

    bp::class_< SireDB::ParameterDB >( "ParameterDB" )    
        .def( bp::init< >() )    
        .def( 
            "addLog"
            , (void ( ::SireDB::ParameterDB::* )( ::QString const & ) )( &::SireDB::ParameterDB::addLog )
            , ( bp::arg("logtext") ) )    
        .def( 
            "assign"
            , (::SireDB::ParameterTable ( ::SireDB::ParameterDB::* )( ::SireMol::Molecule const &,::SireDB::assign_parameters const &,::SireDB::MatchMRData const & ) )( &::SireDB::ParameterDB::assign )
            , ( bp::arg("molecule"), bp::arg("assign_instruct"), bp::arg("matchmr")=::SireDB::MatchMRData( ) ) )    
        .def( 
            "assign"
            , (::SireDB::ParameterTable ( ::SireDB::ParameterDB::* )( ::SireMol::Molecule const &,::SireDB::ParameterTable const &,::SireDB::assign_parameters const &,::SireDB::MatchMRData const & ) )( &::SireDB::ParameterDB::assign )
            , ( bp::arg("molecule"), bp::arg("params"), bp::arg("assign_instruct"), bp::arg("matchmr")=::SireDB::MatchMRData( ) ) )    
        .def( 
            "clear"
            , (void ( ::SireDB::ParameterDB::* )(  ) )( &::SireDB::ParameterDB::clear ) )    
        .def( 
            "clearLog"
            , (void ( ::SireDB::ParameterDB::* )(  ) )( &::SireDB::ParameterDB::clearLog ) )    
        .def( 
            "dump"
            , (void ( ::SireDB::ParameterDB::* )( ::QString const & ) )( &::SireDB::ParameterDB::dump )
            , ( bp::arg("filename") ) )    
        .def( 
            "dump"
            , (void ( ::SireDB::ParameterDB::* )( ::QTextStream & ) )( &::SireDB::ParameterDB::dump )
            , ( bp::arg("ts") ) )    
        .def( 
            "dumpToString"
            , (::QString ( ::SireDB::ParameterDB::* )(  ) )( &::SireDB::ParameterDB::dumpToString ) )    
        .def( 
            "getLog"
            , (::QString ( ::SireDB::ParameterDB::* )(  ) const)( &::SireDB::ParameterDB::getLog ) )    
        .def( 
            "isA"
            , (bool ( ::SireDB::ParameterDB::* )( ::QString const & ) const)( &::SireDB::ParameterDB::isA )
            , ( bp::arg("type_name") ) )    
        .def( 
            "load"
            , (void ( ::SireDB::ParameterDB::* )( ::QString const & ) )( &::SireDB::ParameterDB::load )
            , ( bp::arg("filename") ) )    
        .def( 
            "load"
            , (void ( ::SireDB::ParameterDB::* )( ::QTextStream & ) )( &::SireDB::ParameterDB::load )
            , ( bp::arg("ts") ) )    
        .def( 
            "loadFromString"
            , (void ( ::SireDB::ParameterDB::* )( ::QString ) )( &::SireDB::ParameterDB::loadFromString )
            , ( bp::arg("sqldump") ) )    
        .def( 
            "nParameters"
            , (int ( ::SireDB::ParameterDB::* )(  ) const)( &::SireDB::ParameterDB::nParameters ) )    
        .def( 
            "nRelationships"
            , (int ( ::SireDB::ParameterDB::* )(  ) const)( &::SireDB::ParameterDB::nRelationships ) )    
        .def( 
            "toString"
            , (::QString ( ::SireDB::ParameterDB::* )(  ) const)( &::SireDB::ParameterDB::toString ) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireDB::ParameterDB::typeName ) )    
        .def( 
            "types"
            , (::QStringList ( ::SireDB::ParameterDB::* )(  ) const)( &::SireDB::ParameterDB::types ) )    
        .def( 
            "what"
            , (char const * ( ::SireDB::ParameterDB::* )(  ) const)( &::SireDB::ParameterDB::what ) )    
        .staticmethod( "typeName" )    
        .def( "__str__", &SirePy::__str__< ::SireDB::ParameterDB > );

}
