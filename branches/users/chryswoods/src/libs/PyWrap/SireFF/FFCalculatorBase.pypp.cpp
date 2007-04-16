// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "FFCalculatorBase.pypp.hpp"
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

const char* pvt_get_name(const SireFF::FFCalculatorBase&){ return "SireFF::FFCalculatorBase";}

void register_FFCalculatorBase_class(){

    bp::class_< SireFF::FFCalculatorBase, boost::noncopyable >( "FFCalculatorBase", bp::no_init )    
        .def( 
            "ID"
            , (::SireFF::ForceFieldID ( ::SireFF::FFCalculatorBase::* )(  ) )( &::SireFF::FFCalculatorBase::ID ) )    
        .def( 
            "add"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireMol::PartialMolecule const &,::SireFF::ParameterMap const & ) )( &::SireFF::FFCalculatorBase::add )
            , ( bp::arg("molecule"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "add"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::QList<SireMol::PartialMolecule> const &,::SireFF::ParameterMap const & ) )( &::SireFF::FFCalculatorBase::add )
            , ( bp::arg("molecules"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "addTo"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireFF::FFBase::Group const &,::SireMol::PartialMolecule const &,::SireFF::ParameterMap const & ) )( &::SireFF::FFCalculatorBase::addTo )
            , ( bp::arg("group"), bp::arg("molecule"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "addTo"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireFF::FFBase::Group const &,::QList<SireMol::PartialMolecule> const &,::SireFF::ParameterMap const & ) )( &::SireFF::FFCalculatorBase::addTo )
            , ( bp::arg("group"), bp::arg("molecules"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "assertContains"
            , (void ( ::SireFF::FFCalculatorBase::* )( ::SireFF::FFComponent const & ) )( &::SireFF::FFCalculatorBase::assertContains )
            , ( bp::arg("component") ) )    
        .def( 
            "calculateEnergy"
            , (void ( ::SireFF::FFCalculatorBase::* )(  ) )( &::SireFF::FFCalculatorBase::calculateEnergy ) )    
        .def( 
            "change"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireMol::PartialMolecule const & ) )( &::SireFF::FFCalculatorBase::change )
            , ( bp::arg("molecule") ) )    
        .def( 
            "change"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> const & ) )( &::SireFF::FFCalculatorBase::change )
            , ( bp::arg("molecules") ) )    
        .def( 
            "change"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::QList<SireMol::PartialMolecule> const & ) )( &::SireFF::FFCalculatorBase::change )
            , ( bp::arg("molecules") ) )    
        .def( 
            "contains"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireMol::PartialMolecule const & ) )( &::SireFF::FFCalculatorBase::contains )
            , ( bp::arg("molecule") ) )    
        .def( 
            "contains"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireMol::PartialMolecule const &,::SireFF::FFBase::Group const & ) )( &::SireFF::FFCalculatorBase::contains )
            , ( bp::arg("molecule"), bp::arg("group") ) )    
        .def( 
            "containsProperty"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::QString const & ) )( &::SireFF::FFCalculatorBase::containsProperty )
            , ( bp::arg("name") ) )    
        .def( 
            "contents"
            , (::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> ( ::SireFF::FFCalculatorBase::* )( ::SireFF::FFBase::Group const & ) )( &::SireFF::FFCalculatorBase::contents )
            , ( bp::arg("group") ) )    
        .def( 
            "contents"
            , (::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> ( ::SireFF::FFCalculatorBase::* )(  ) )( &::SireFF::FFCalculatorBase::contents ) )    
        .def( 
            "forceField"
            , (::SireFF::ForceField ( ::SireFF::FFCalculatorBase::* )(  ) )( &::SireFF::FFCalculatorBase::forceField ) )    
        .def( 
            "getEnergies"
            , (double ( ::SireFF::FFCalculatorBase::* )( ::SireCAS::Values & ) )( &::SireFF::FFCalculatorBase::getEnergies )
            , ( bp::arg("values") ) )    
        .def( 
            "getProperty"
            , (::SireBase::Property ( ::SireFF::FFCalculatorBase::* )( ::QString const & ) )( &::SireFF::FFCalculatorBase::getProperty )
            , ( bp::arg("name") ) )    
        .def( 
            "groupsReferringTo"
            , (::QSet<SireFF::FFBase::Group> ( ::SireFF::FFCalculatorBase::* )( ::SireMol::MoleculeID ) )( &::SireFF::FFCalculatorBase::groupsReferringTo )
            , ( bp::arg("molid") ) )    
        .def( 
            "isClean"
            , (bool ( ::SireFF::FFCalculatorBase::* )(  ) )( &::SireFF::FFCalculatorBase::isClean ) )    
        .def( 
            "isDirty"
            , (bool ( ::SireFF::FFCalculatorBase::* )(  ) )( &::SireFF::FFCalculatorBase::isDirty ) )    
        .def( 
            "molecule"
            , (::SireMol::PartialMolecule ( ::SireFF::FFCalculatorBase::* )( ::SireMol::MoleculeID ) )( &::SireFF::FFCalculatorBase::molecule )
            , ( bp::arg("molid") ) )    
        .def( 
            "molecule"
            , (::SireMol::PartialMolecule ( ::SireFF::FFCalculatorBase::* )( ::SireMol::MoleculeID,::SireFF::FFBase::Group const & ) )( &::SireFF::FFCalculatorBase::molecule )
            , ( bp::arg("molid"), bp::arg("group") ) )    
        .def( 
            "moleculeIDs"
            , (::QSet<SireMol::MoleculeID> ( ::SireFF::FFCalculatorBase::* )(  ) )( &::SireFF::FFCalculatorBase::moleculeIDs ) )    
        .def( 
            "moleculeIDs"
            , (::QSet<SireMol::MoleculeID> ( ::SireFF::FFCalculatorBase::* )( ::SireFF::FFBase::Group const & ) )( &::SireFF::FFCalculatorBase::moleculeIDs )
            , ( bp::arg("group") ) )    
        .def( 
            "molecules"
            , (::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> ( ::SireFF::FFCalculatorBase::* )(  ) )( &::SireFF::FFCalculatorBase::molecules ) )    
        .def( 
            "molecules"
            , (::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> ( ::SireFF::FFCalculatorBase::* )( ::SireFF::FFBase::Group const & ) )( &::SireFF::FFCalculatorBase::molecules )
            , ( bp::arg("group") ) )    
        .def( 
            "molecules"
            , (::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> ( ::SireFF::FFCalculatorBase::* )( ::QSet<SireMol::MoleculeID> const & ) )( &::SireFF::FFCalculatorBase::molecules )
            , ( bp::arg("molids") ) )    
        .def( 
            "mustNowRecalculateFromScratch"
            , (void ( ::SireFF::FFCalculatorBase::* )(  ) )( &::SireFF::FFCalculatorBase::mustNowRecalculateFromScratch ) )    
        .def( 
            "properties"
            , (::QHash<QString,SireBase::Property> ( ::SireFF::FFCalculatorBase::* )(  ) )( &::SireFF::FFCalculatorBase::properties ) )    
        .def( 
            "refersTo"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireMol::MoleculeID ) )( &::SireFF::FFCalculatorBase::refersTo )
            , ( bp::arg("molid") ) )    
        .def( 
            "refersTo"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireMol::MoleculeID,::SireFF::FFBase::Group const & ) )( &::SireFF::FFCalculatorBase::refersTo )
            , ( bp::arg("molid"), bp::arg("group") ) )    
        .def( 
            "remove"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireMol::PartialMolecule const & ) )( &::SireFF::FFCalculatorBase::remove )
            , ( bp::arg("molecule") ) )    
        .def( 
            "remove"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::QList<SireMol::PartialMolecule> const & ) )( &::SireFF::FFCalculatorBase::remove )
            , ( bp::arg("molecules") ) )    
        .def( 
            "removeFrom"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireFF::FFBase::Group const &,::SireMol::PartialMolecule const & ) )( &::SireFF::FFCalculatorBase::removeFrom )
            , ( bp::arg("group"), bp::arg("molecule") ) )    
        .def( 
            "removeFrom"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireFF::FFBase::Group const &,::QList<SireMol::PartialMolecule> const & ) )( &::SireFF::FFCalculatorBase::removeFrom )
            , ( bp::arg("group"), bp::arg("molecules") ) )    
        .def( 
            "setForceField"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::SireFF::ForceField const & ) )( &::SireFF::FFCalculatorBase::setForceField )
            , ( bp::arg("forcefield") ) )    
        .def( 
            "setProperty"
            , (bool ( ::SireFF::FFCalculatorBase::* )( ::QString const &,::SireBase::Property const & ) )( &::SireFF::FFCalculatorBase::setProperty )
            , ( bp::arg("name"), bp::arg("value") ) )    
        .def( 
            "version"
            , (::SireBase::Version ( ::SireFF::FFCalculatorBase::* )(  ) )( &::SireFF::FFCalculatorBase::version ) )    
        .def( "__str__", &pvt_get_name);

}
