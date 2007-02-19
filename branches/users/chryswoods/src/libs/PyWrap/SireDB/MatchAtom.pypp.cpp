// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "MatchAtom.pypp.hpp"
#include "boost/python.hpp"
#include "siredb_headers.h"
#include "SireCAS/function.h"
#include "SireCAS/expression.h"

namespace bp = boost::python;

#include "SirePy/str.hpp"

void register_MatchAtom_class(){

    bp::class_< SireDB::MatchAtom, bp::bases< SireDB::MatchData > >( "MatchAtom" )    
        .def( bp::init< >() )    
        .def( 
            "addMirroredTo"
            , (::SireDB::RelateID ( ::SireDB::MatchAtom::* )( ::SireDB::ParameterDB &,::uint ) const)( &::SireDB::MatchAtom::addMirroredTo )
            , ( bp::arg("db"), bp::arg("n") ) )    
        .def( 
            "addTo"
            , (::SireDB::RelateID ( ::SireDB::MatchAtom::* )( ::SireDB::ParameterDB &,::uint ) const)( &::SireDB::MatchAtom::addTo )
            , ( bp::arg("db"), bp::arg("n") ) )    
        .def( 
            "hasNameMatch"
            , (bool ( ::SireDB::MatchAtom::* )(  ) const)( &::SireDB::MatchAtom::hasNameMatch ) )    
        .def( 
            "hasNumberMatch"
            , (bool ( ::SireDB::MatchAtom::* )(  ) const)( &::SireDB::MatchAtom::hasNumberMatch ) )    
        .def( 
            "isEmpty"
            , (bool ( ::SireDB::MatchAtom::* )(  ) const)( &::SireDB::MatchAtom::isEmpty ) )    
        .def( 
            "nMatches"
            , (int ( ::SireDB::MatchAtom::* )(  ) const)( &::SireDB::MatchAtom::nMatches ) )    
        .def( 
            "name"
            , (::QString ( ::SireDB::MatchAtom::* )(  ) const)( &::SireDB::MatchAtom::name ) )    
        .def( 
            "number"
            , (::SireMol::AtomNum ( ::SireDB::MatchAtom::* )(  ) const)( &::SireDB::MatchAtom::number ) )    
        .def( bp::self != bp::self )    
        .def( bp::self == bp::self )    
        .def( 
            "setName"
            , (void ( ::SireDB::MatchAtom::* )( ::QString const & ) )( &::SireDB::MatchAtom::setName )
            , ( bp::arg("name") ) )    
        .def( 
            "setNumber"
            , (void ( ::SireDB::MatchAtom::* )( ::SireMol::AtomNum ) )( &::SireDB::MatchAtom::setNumber )
            , ( bp::arg("number") ) )    
        .def( 
            "toString"
            , (::QString ( ::SireDB::MatchAtom::* )(  ) const)( &::SireDB::MatchAtom::toString ) )    
        .def( 
            "toString"
            , (::QString ( ::SireDB::MatchAtom::* )( ::uint ) const)( &::SireDB::MatchAtom::toString )
            , ( bp::arg("atm") ) )    
        .def( "__str__", &SirePy::__str__< ::SireDB::MatchAtom > );

}
