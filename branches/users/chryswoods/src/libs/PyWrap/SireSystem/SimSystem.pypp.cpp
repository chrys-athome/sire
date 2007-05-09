// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "SimSystem.pypp.hpp"
#include "boost/python.hpp"
#include "siresystem_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/molecules.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireFF/ffgroupid.h"

namespace bp = boost::python;

const char* pvt_get_name(const SireSystem::SimSystem&){ return "SireSystem::SimSystem";}

void register_SimSystem_class(){

    bp::class_< SireSystem::SimSystem, bp::bases< SireSystem::QuerySystem >, boost::noncopyable >( "SimSystem", bp::no_init )    
        .def( 
            "add"
            , (::SireMol::PartialMolecule ( ::SireSystem::SimSystem::* )( ::SireMol::PartialMolecule const &,::QSet<SireMol::MoleculeGroupID> const & ) )( &::SireSystem::SimSystem::add )
            , ( bp::arg("molecule"), bp::arg("molgroupids") ) )    
        .def( 
            "add"
            , (::SireMol::PartialMolecule ( ::SireSystem::SimSystem::* )( ::SireMol::PartialMolecule const &,::QSet<SireFF::FFGroupID> const &,::SireFF::ParameterMap const & ) )( &::SireSystem::SimSystem::add )
            , ( bp::arg("molecule"), bp::arg("ffgroupids"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "add"
            , (::SireMol::PartialMolecule ( ::SireSystem::SimSystem::* )( ::SireMol::PartialMolecule const &,::QSet<SireFF::FFGroupID> const &,::QSet<SireMol::MoleculeGroupID> const &,::SireFF::ParameterMap const & ) )( &::SireSystem::SimSystem::add )
            , ( bp::arg("molecule"), bp::arg("ffgroupids"), bp::arg("molgroupids"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "add"
            , (::SireMol::Molecules ( ::SireSystem::SimSystem::* )( ::SireMol::Molecules const &,::QSet<SireMol::MoleculeGroupID> const & ) )( &::SireSystem::SimSystem::add )
            , ( bp::arg("molecules"), bp::arg("molgroupids") ) )    
        .def( 
            "add"
            , (::SireMol::Molecules ( ::SireSystem::SimSystem::* )( ::SireMol::Molecules const &,::QSet<SireFF::FFGroupID> const &,::SireFF::ParameterMap const & ) )( &::SireSystem::SimSystem::add )
            , ( bp::arg("molecules"), bp::arg("ffgroupids"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "add"
            , (::SireMol::Molecules ( ::SireSystem::SimSystem::* )( ::SireMol::Molecules const &,::QSet<SireFF::FFGroupID> const &,::QSet<SireMol::MoleculeGroupID> const &,::SireFF::ParameterMap const & ) )( &::SireSystem::SimSystem::add )
            , ( bp::arg("molecules"), bp::arg("ffgroupids"), bp::arg("molgroupids"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "change"
            , (::SireMol::PartialMolecule ( ::SireSystem::SimSystem::* )( ::SireMol::PartialMolecule const & ) )( &::SireSystem::SimSystem::change )
            , ( bp::arg("molecule") ) )    
        .def( 
            "change"
            , (::SireMol::Molecules ( ::SireSystem::SimSystem::* )( ::SireMol::Molecules const & ) )( &::SireSystem::SimSystem::change )
            , ( bp::arg("molecules") ) )    
        .def( 
            "commit"
            , (void ( ::SireSystem::SimSystem::* )(  ) )( &::SireSystem::SimSystem::commit ) )    
        .def( 
            "remove"
            , (void ( ::SireSystem::SimSystem::* )( ::SireMol::PartialMolecule const & ) )( &::SireSystem::SimSystem::remove )
            , ( bp::arg("molecule") ) )    
        .def( 
            "remove"
            , (void ( ::SireSystem::SimSystem::* )( ::SireMol::Molecules const & ) )( &::SireSystem::SimSystem::remove )
            , ( bp::arg("molecules") ) )    
        .def( 
            "remove"
            , (void ( ::SireSystem::SimSystem::* )( ::SireMol::PartialMolecule const &,::QSet<SireMol::MoleculeGroupID> const & ) )( &::SireSystem::SimSystem::remove )
            , ( bp::arg("molecule"), bp::arg("molgroupids") ) )    
        .def( 
            "remove"
            , (void ( ::SireSystem::SimSystem::* )( ::SireMol::PartialMolecule const &,::QSet<SireFF::FFGroupID> const &,::QSet<SireMol::MoleculeGroupID> const & ) )( &::SireSystem::SimSystem::remove )
            , ( bp::arg("molecule"), bp::arg("ffgroupids"), bp::arg("molgroupids")=::QSet<SireMol::MoleculeGroupID>( ) ) )    
        .def( 
            "remove"
            , (void ( ::SireSystem::SimSystem::* )( ::SireMol::Molecules const &,::QSet<SireMol::MoleculeGroupID> const & ) )( &::SireSystem::SimSystem::remove )
            , ( bp::arg("molecules"), bp::arg("molgroupids") ) )    
        .def( 
            "remove"
            , (void ( ::SireSystem::SimSystem::* )( ::SireMol::Molecules const &,::QSet<SireFF::FFGroupID> const &,::QSet<SireMol::MoleculeGroupID> const & ) )( &::SireSystem::SimSystem::remove )
            , ( bp::arg("molecules"), bp::arg("ffgroupids"), bp::arg("molgroupids")=::QSet<SireMol::MoleculeGroupID>( ) ) )    
        .def( 
            "rollBack"
            , (void ( ::SireSystem::SimSystem::* )( ::SireSystem::CheckPoint const & ) )( &::SireSystem::SimSystem::rollBack )
            , ( bp::arg("checkpoint") ) )    
        .def( 
            "setProperty"
            , (void ( ::SireSystem::SimSystem::* )( ::QString const &,::SireBase::Property const & ) )( &::SireSystem::SimSystem::setProperty )
            , ( bp::arg("name"), bp::arg("property") ) )    
        .def( 
            "setProperty"
            , (void ( ::SireSystem::SimSystem::* )( ::SireFF::ForceFieldID,::QString const &,::SireBase::Property const & ) )( &::SireSystem::SimSystem::setProperty )
            , ( bp::arg("ffid"), bp::arg("name"), bp::arg("property") ) )    
        .def( 
            "setProperty"
            , (void ( ::SireSystem::SimSystem::* )( ::QSet<SireFF::ForceFieldID> const &,::QString const &,::SireBase::Property const & ) )( &::SireSystem::SimSystem::setProperty )
            , ( bp::arg("ffids"), bp::arg("name"), bp::arg("property") ) )    
        .def( "__str__", &pvt_get_name);

}
