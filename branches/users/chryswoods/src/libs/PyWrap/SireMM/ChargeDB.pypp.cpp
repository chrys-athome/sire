// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ChargeDB.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

#include "SirePy/str.hpp"

void register_ChargeDB_class(){

    bp::class_< SireMM::ChargeDB, bp::bases< SireDB::AtomDB > >( "ChargeDB" )    
        .def( bp::init< >() )    
        .def( 
            "addCharge"
            , (void ( ::SireMM::ChargeDB::* )( ::QString const &,::SireMM::ChargeParameter const & ) )( &::SireMM::ChargeDB::addCharge )
            , ( bp::arg("userid"), bp::arg("charge") ) )    
        .def( 
            "assignParameter"
            , (bool ( ::SireMM::ChargeDB::* )( ::SireMol::AtomIndex const &,::SireDB::RelateIDMap const &,::SireDB::ParameterTable & ) )( &::SireMM::ChargeDB::assignParameter )
            , ( bp::arg("atom"), bp::arg("relateids"), bp::arg("param_table") ) )    
        .def( 
            "createTable"
            , (void ( ::SireMM::ChargeDB::* )( ::SireDB::ParameterTable & ) const)( &::SireMM::ChargeDB::createTable )
            , ( bp::arg("param_table") ) )    
        .def( 
            "getCharge"
            , (::SireMM::ChargeParameter ( ::SireMM::ChargeDB::* )( ::QString const &,bool * ) )( &::SireMM::ChargeDB::getCharge )
            , ( bp::arg("userid"), bp::arg("foundcharge")=bp::object() ) )    
        .def( 
            "getCharge"
            , (::SireMM::ChargeParameter ( ::SireMM::ChargeDB::* )( ::SireDB::RelateID,bool * ) )( &::SireMM::ChargeDB::getCharge )
            , ( bp::arg("relateid"), bp::arg("foundcharge")=bp::object() ) )    
        .def( 
            "getCharge"
            , (::SireMM::ChargeParameter ( ::SireMM::ChargeDB::* )( ::SireDB::RelateIDMap const &,bool * ) )( &::SireMM::ChargeDB::getCharge )
            , ( bp::arg("relateids"), bp::arg("foundcharge")=bp::object() ) )    
        .def( 
            "relateCharge"
            , (void ( ::SireMM::ChargeDB::* )( ::SireDB::AssertMatch<1> const &,::QString const & ) )( &::SireMM::ChargeDB::relateCharge )
            , ( bp::arg("matchatom"), bp::arg("userid") ) )    
        .def( 
            "relateCharge"
            , (void ( ::SireMM::ChargeDB::* )( ::SireDB::AssertMatch<1> const &,::SireMM::ChargeParameter const & ) )( &::SireMM::ChargeDB::relateCharge )
            , ( bp::arg("matchatom"), bp::arg("charge") ) )    
        .def( 
            "relateCharge"
            , (void ( ::SireMM::ChargeDB::* )( ::SireDB::RelateID,::QString const & ) )( &::SireMM::ChargeDB::relateCharge )
            , ( bp::arg("relateid"), bp::arg("userid") ) )    
        .def( 
            "relateCharge"
            , (void ( ::SireMM::ChargeDB::* )( ::SireDB::RelateID,::SireMM::ChargeParameter const & ) )( &::SireMM::ChargeDB::relateCharge )
            , ( bp::arg("relateid"), bp::arg("charge") ) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireMM::ChargeDB::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireMM::ChargeDB::* )(  ) const)( &::SireMM::ChargeDB::what ) )    
        .staticmethod( "typeName" )    
        .def( "__str__", &SirePy::__str__< ::SireMM::ChargeDB > );

}
