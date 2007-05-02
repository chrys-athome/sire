// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "FFLocalCalculator.pypp.hpp"
#include "boost/python.hpp"
#include "sireff_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"
#include "SireMol/cgatomid.h"
#include "SireMol/cutgroupnum.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/resid.h"
#include "SireMol/moleculeid.h"
#include "SireBase/property.h"

namespace bp = boost::python;

const char* pvt_get_name(const SireFF::FFLocalCalculator&){ return "SireFF::FFLocalCalculator";}

void register_FFLocalCalculator_class(){

    bp::class_< SireFF::FFLocalCalculator, bp::bases< SireFF::FFCalculatorBase >, boost::noncopyable >( "FFLocalCalculator", bp::no_init )    
        .def( 
            "ID"
            , (::SireFF::ForceFieldID ( ::SireFF::FFLocalCalculator::* )(  ) )( &::SireFF::FFLocalCalculator::ID ) )    
        .def( 
            "add"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::SireMol::PartialMolecule const &,::SireFF::ParameterMap const & ) )( &::SireFF::FFLocalCalculator::add )
            , ( bp::arg("molecule"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "add"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::QList<SireMol::PartialMolecule> const &,::SireFF::ParameterMap const & ) )( &::SireFF::FFLocalCalculator::add )
            , ( bp::arg("molecules"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "addTo"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::SireFF::FFBase::Group const &,::SireMol::PartialMolecule const &,::SireFF::ParameterMap const & ) )( &::SireFF::FFLocalCalculator::addTo )
            , ( bp::arg("group"), bp::arg("molecule"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "addTo"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::SireFF::FFBase::Group const &,::QList<SireMol::PartialMolecule> const &,::SireFF::ParameterMap const & ) )( &::SireFF::FFLocalCalculator::addTo )
            , ( bp::arg("group"), bp::arg("molecules"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "assertContains"
            , (void ( ::SireFF::FFLocalCalculator::* )( ::SireFF::FFComponent const & ) )( &::SireFF::FFLocalCalculator::assertContains )
            , ( bp::arg("component") ) )    
        .def( 
            "change"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::SireMol::PartialMolecule const & ) )( &::SireFF::FFLocalCalculator::change )
            , ( bp::arg("molecule") ) )    
        .def( 
            "change"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> const & ) )( &::SireFF::FFLocalCalculator::change )
            , ( bp::arg("molecules") ) )    
        .def( 
            "change"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::QList<SireMol::PartialMolecule> const & ) )( &::SireFF::FFLocalCalculator::change )
            , ( bp::arg("molecules") ) )    
        .def( 
            "contains"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::SireMol::PartialMolecule const & ) )( &::SireFF::FFLocalCalculator::contains )
            , ( bp::arg("molecule") ) )    
        .def( 
            "contains"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::SireMol::PartialMolecule const &,::SireFF::FFBase::Group const & ) )( &::SireFF::FFLocalCalculator::contains )
            , ( bp::arg("molecule"), bp::arg("group") ) )    
        .def( 
            "containsProperty"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::QString const & ) )( &::SireFF::FFLocalCalculator::containsProperty )
            , ( bp::arg("name") ) )    
        .def( 
            "contents"
            , (::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> ( ::SireFF::FFLocalCalculator::* )( ::SireFF::FFBase::Group const & ) )( &::SireFF::FFLocalCalculator::contents )
            , ( bp::arg("group") ) )    
        .def( 
            "contents"
            , (::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> ( ::SireFF::FFLocalCalculator::* )(  ) )( &::SireFF::FFLocalCalculator::contents ) )    
        .def( 
            "getProperty"
            , (::SireBase::Property ( ::SireFF::FFLocalCalculator::* )( ::QString const & ) )( &::SireFF::FFLocalCalculator::getProperty )
            , ( bp::arg("name") ) )    
        .def( 
            "groupsReferringTo"
            , (::QSet<SireFF::FFBase::Group> ( ::SireFF::FFLocalCalculator::* )( ::SireMol::MoleculeID ) )( &::SireFF::FFLocalCalculator::groupsReferringTo )
            , ( bp::arg("molid") ) )    
        .def( 
            "isClean"
            , (bool ( ::SireFF::FFLocalCalculator::* )(  ) )( &::SireFF::FFLocalCalculator::isClean ) )    
        .def( 
            "isDirty"
            , (bool ( ::SireFF::FFLocalCalculator::* )(  ) )( &::SireFF::FFLocalCalculator::isDirty ) )    
        .def( 
            "molecule"
            , (::SireMol::PartialMolecule ( ::SireFF::FFLocalCalculator::* )( ::SireMol::MoleculeID ) )( &::SireFF::FFLocalCalculator::molecule )
            , ( bp::arg("molid") ) )    
        .def( 
            "molecule"
            , (::SireMol::PartialMolecule ( ::SireFF::FFLocalCalculator::* )( ::SireMol::MoleculeID,::SireFF::FFBase::Group const & ) )( &::SireFF::FFLocalCalculator::molecule )
            , ( bp::arg("molid"), bp::arg("group") ) )    
        .def( 
            "moleculeIDs"
            , (::QSet<SireMol::MoleculeID> ( ::SireFF::FFLocalCalculator::* )(  ) )( &::SireFF::FFLocalCalculator::moleculeIDs ) )    
        .def( 
            "moleculeIDs"
            , (::QSet<SireMol::MoleculeID> ( ::SireFF::FFLocalCalculator::* )( ::SireFF::FFBase::Group const & ) )( &::SireFF::FFLocalCalculator::moleculeIDs )
            , ( bp::arg("group") ) )    
        .def( 
            "molecules"
            , (::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> ( ::SireFF::FFLocalCalculator::* )(  ) )( &::SireFF::FFLocalCalculator::molecules ) )    
        .def( 
            "molecules"
            , (::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> ( ::SireFF::FFLocalCalculator::* )( ::QSet<SireMol::MoleculeID> const & ) )( &::SireFF::FFLocalCalculator::molecules )
            , ( bp::arg("molids") ) )    
        .def( 
            "molecules"
            , (::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> ( ::SireFF::FFLocalCalculator::* )( ::SireFF::FFBase::Group const & ) )( &::SireFF::FFLocalCalculator::molecules )
            , ( bp::arg("group") ) )    
        .def( 
            "mustNowRecalculateFromScratch"
            , (void ( ::SireFF::FFLocalCalculator::* )(  ) )( &::SireFF::FFLocalCalculator::mustNowRecalculateFromScratch ) )    
        .def( 
            "properties"
            , (::QHash<QString,SireBase::Property> ( ::SireFF::FFLocalCalculator::* )(  ) )( &::SireFF::FFLocalCalculator::properties ) )    
        .def( 
            "refersTo"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::SireMol::MoleculeID ) )( &::SireFF::FFLocalCalculator::refersTo )
            , ( bp::arg("molid") ) )    
        .def( 
            "refersTo"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::SireMol::MoleculeID,::SireFF::FFBase::Group const & ) )( &::SireFF::FFLocalCalculator::refersTo )
            , ( bp::arg("molid"), bp::arg("group") ) )    
        .def( 
            "remove"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::SireMol::PartialMolecule const & ) )( &::SireFF::FFLocalCalculator::remove )
            , ( bp::arg("molecule") ) )    
        .def( 
            "remove"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::QList<SireMol::PartialMolecule> const & ) )( &::SireFF::FFLocalCalculator::remove )
            , ( bp::arg("molecules") ) )    
        .def( 
            "removeFrom"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::SireFF::FFBase::Group const &,::SireMol::PartialMolecule const & ) )( &::SireFF::FFLocalCalculator::removeFrom )
            , ( bp::arg("group"), bp::arg("molecule") ) )    
        .def( 
            "removeFrom"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::SireFF::FFBase::Group const &,::QList<SireMol::PartialMolecule> const & ) )( &::SireFF::FFLocalCalculator::removeFrom )
            , ( bp::arg("group"), bp::arg("molecules") ) )    
        .def( 
            "setProperty"
            , (bool ( ::SireFF::FFLocalCalculator::* )( ::QString const &,::SireBase::Property const & ) )( &::SireFF::FFLocalCalculator::setProperty )
            , ( bp::arg("name"), bp::arg("property") ) )    
        .def( 
            "version"
            , (::SireBase::Version ( ::SireFF::FFLocalCalculator::* )(  ) )( &::SireFF::FFLocalCalculator::version ) )    
        .def( "__str__", &pvt_get_name);

}
