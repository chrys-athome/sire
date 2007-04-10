// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ForceFields.pypp.hpp"
#include "boost/python.hpp"
#include "sireff_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"
#include "SireMol/cgatomid.h"
#include "SireMol/cutgroupnum.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/resid.h"
#include "SireMol/moleculeid.h"

namespace bp = boost::python;

SireFF::ForceFields __copy__(const SireFF::ForceFields &other){ return SireFF::ForceFields(other); }

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireFF::ForceFields&){ return "SireFF::ForceFields";}

void register_ForceFields_class(){

    bp::class_< SireFF::ForceFields, bp::bases< SireFF::ForceFieldsBase > >( "ForceFields" )    
        .def( bp::init< >() )    
        .def( bp::init< QList<SireFF::ForceField> const & >(( bp::arg("ffields") )) )    
        .def( bp::init< QHash<SireFF::ForceFieldID, SireFF::ForceField> const & >(( bp::arg("ffields") )) )    
        .def( bp::init< SireFF::ForceFieldsBase const & >(( bp::arg("other") )) )    
        .def( 
            "add"
            , (void ( ::SireFF::ForceFields::* )( ::SireFF::ForceField const & ) )( &::SireFF::ForceFields::add )
            , ( bp::arg("ffield") ) )    
        .def( 
            "addTo"
            , (void ( ::SireFF::ForceFields::* )( ::SireFF::ForceFieldID,::SireFF::FFBase::Group const &,::SireMol::PartialMolecule const &,::SireFF::ParameterMap const & ) )( &::SireFF::ForceFields::addTo )
            , ( bp::arg("ffid"), bp::arg("group"), bp::arg("molecule"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "assertSameContents"
            , (void ( ::SireFF::ForceFields::* )( ::SireFF::ForceFieldsBase const & ) const)( &::SireFF::ForceFields::assertSameContents )
            , ( bp::arg("other") ) )    
        .def( 
            "assertValidComponents"
            , (void ( ::SireFF::ForceFields::* )( ::SireFF::FFExpression const & ) const)( &::SireFF::ForceFields::assertValidComponents )
            , ( bp::arg("expression") ) )    
        .def( 
            "change"
            , (void ( ::SireFF::ForceFields::* )( ::SireMol::PartialMolecule const & ) )( &::SireFF::ForceFields::change )
            , ( bp::arg("molecule") ) )    
        .def( 
            "change"
            , (void ( ::SireFF::ForceFields::* )( ::QHash<SireMol::MoleculeID,SireMol::PartialMolecule> const & ) const)( &::SireFF::ForceFields::change )
            , ( bp::arg("molecules") ) )    
        .def( 
            "forceFieldGroupsReferringTo"
            , (::QSet<SireFF::FFGroupID> ( ::SireFF::ForceFields::* )( ::SireMol::MoleculeID ) const)( &::SireFF::ForceFields::forceFieldGroupsReferringTo )
            , ( bp::arg("molid") ) )    
        .def( 
            "forceFieldIDs"
            , (::QSet<SireFF::ForceFieldID> ( ::SireFF::ForceFields::* )(  ) const)( &::SireFF::ForceFields::forceFieldIDs ) )    
        .def( 
            "forceFields"
            , (::QHash<SireFF::ForceFieldID, SireFF::ForceField> ( ::SireFF::ForceFields::* )(  ) const)( &::SireFF::ForceFields::forceFields ) )    
        .def( 
            "molecule"
            , (::SireMol::PartialMolecule ( ::SireFF::ForceFields::* )( ::SireMol::MoleculeID,::SireFF::ForceFieldID,::SireFF::FFBase::Group const & ) const)( &::SireFF::ForceFields::molecule )
            , ( bp::arg("molid"), bp::arg("ffid"), bp::arg("group") ) )    
        .def( 
            "moleculeIDs"
            , (::QSet<SireMol::MoleculeID> ( ::SireFF::ForceFields::* )( ::SireFF::ForceFieldID,::SireFF::FFBase::Group const & ) const)( &::SireFF::ForceFields::moleculeIDs )
            , ( bp::arg("ffid"), bp::arg("group") ) )    
        .def( 
            "properties"
            , (::QHash<QString,QHash<SireFF::ForceFieldID, SireBase::Property> > ( ::SireFF::ForceFields::* )(  ) const)( &::SireFF::ForceFields::properties ) )    
        .def( 
            "refersTo"
            , (bool ( ::SireFF::ForceFields::* )( ::SireMol::MoleculeID,::SireFF::ForceFieldID,::SireFF::FFBase::Group const & ) const)( &::SireFF::ForceFields::refersTo )
            , ( bp::arg("molid"), bp::arg("ffid"), bp::arg("group") ) )    
        .def( 
            "remove"
            , (void ( ::SireFF::ForceFields::* )( ::SireFF::ForceFieldID ) )( &::SireFF::ForceFields::remove )
            , ( bp::arg("ffid") ) )    
        .def( 
            "remove"
            , (void ( ::SireFF::ForceFields::* )( ::QString const & ) )( &::SireFF::ForceFields::remove )
            , ( bp::arg("ffname") ) )    
        .def( 
            "removeFrom"
            , (void ( ::SireFF::ForceFields::* )( ::SireFF::ForceFieldID,::SireFF::FFBase::Group const &,::SireMol::PartialMolecule const & ) )( &::SireFF::ForceFields::removeFrom )
            , ( bp::arg("ffid"), bp::arg("group"), bp::arg("molecule") ) )    
        .def( 
            "set"
            , (void ( ::SireFF::ForceFields::* )( ::SireFF::ForceField const & ) )( &::SireFF::ForceFields::set )
            , ( bp::arg("ffield") ) )    
        .def( 
            "setProperty"
            , (void ( ::SireFF::ForceFields::* )( ::SireFF::ForceFieldID,::QString const &,::SireBase::Property const & ) )( &::SireFF::ForceFields::setProperty )
            , ( bp::arg("ffid"), bp::arg("name"), bp::arg("property") ) )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireFF::ForceFields >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireFF::ForceFields >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
