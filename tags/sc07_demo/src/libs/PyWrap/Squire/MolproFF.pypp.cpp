// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "MolproFF.pypp.hpp"
#include "boost/python.hpp"
#include "squire_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/molecules.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"

namespace bp = boost::python;

Squire::MolproFF __copy__(const Squire::MolproFF &other){ return Squire::MolproFF(other); }

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_MolproFF_class(){

    { //::Squire::MolproFF
        typedef bp::class_< Squire::MolproFF, bp::bases< SireFF::FFBase > > MolproFF_exposer_t;
        MolproFF_exposer_t MolproFF_exposer = MolproFF_exposer_t( "MolproFF" );
        bp::scope MolproFF_scope( MolproFF_exposer );
        bp::class_< Squire::MolproFF::Components, bp::bases< SireFF::FFBase::Components > >( "Components" )    
            .def( bp::init< >() )    
            .def( bp::init< SireFF::FFBase const &, bp::optional< SireCAS::Symbols const & > >(( bp::arg("ffbase"), bp::arg("symbols")=::SireCAS::Symbols( ) )) )    
            .def( 
                "describe_qm"
                , (::QString (*)(  ))( &::Squire::MolproFF::Components::describe_qm ) )    
            .def( 
                "qm"
                , (::SireFF::FFComponent const & ( ::Squire::MolproFF::Components::* )(  ) const)( &::Squire::MolproFF::Components::qm )
                , bp::return_value_policy< bp::copy_const_reference >() )    
            .staticmethod( "describe_qm" );
        bp::class_< Squire::MolproFF::Groups, bp::bases< SireFF::FFBase::Groups > >( "Groups" )    
            .def( bp::init< >() )    
            .def( 
                "mm"
                , (::SireFF::FFBase::Group ( ::Squire::MolproFF::Groups::* )(  ) const)( &::Squire::MolproFF::Groups::mm ) )    
            .def( 
                "qm"
                , (::SireFF::FFBase::Group ( ::Squire::MolproFF::Groups::* )(  ) const)( &::Squire::MolproFF::Groups::qm ) );
        bp::class_< Squire::MolproFF::Parameters, bp::bases< SireFF::FFBase::Parameters > >( "Parameters" )    
            .def( bp::init< >() )    
            .def( 
                "coulomb"
                , (::SireFF::ParameterName const & ( ::Squire::MolproFF::Parameters::* )(  ) const)( &::Squire::MolproFF::Parameters::coulomb )
                , bp::return_value_policy< bp::copy_const_reference >() )    
            .def_readonly( "default_sources", Squire::MolproFF::Parameters::default_sources );
        MolproFF_exposer.def( bp::init< >() );
        MolproFF_exposer.def( bp::init< SireVol::Space const &, SireMM::SwitchingFunction const & >(( bp::arg("space"), bp::arg("switchfunc") )) );
        { //::Squire::MolproFF::add
        
            typedef bool ( ::Squire::MolproFF::*add_function_type )( ::SireMol::PartialMolecule const &,::SireFF::ParameterMap const & ) ;
            
            MolproFF_exposer.def( 
                "add"
                , add_function_type( &::Squire::MolproFF::add )
                , ( bp::arg("molecule"), bp::arg("map")=::SireFF::ParameterMap( ) ) );
        
        }
        { //::Squire::MolproFF::add
        
            typedef bool ( ::Squire::MolproFF::*add_function_type )( ::SireMol::Molecules const &,::SireFF::ParameterMap const & ) ;
            
            MolproFF_exposer.def( 
                "add"
                , add_function_type( &::Squire::MolproFF::add )
                , ( bp::arg("molecules"), bp::arg("map")=::SireFF::ParameterMap( ) ) );
        
        }
        { //::Squire::MolproFF::addTo
        
            typedef bool ( ::Squire::MolproFF::*addTo_function_type )( ::SireFF::FFBase::Group const &,::SireMol::PartialMolecule const &,::SireFF::ParameterMap const & ) ;
            
            MolproFF_exposer.def( 
                "addTo"
                , addTo_function_type( &::Squire::MolproFF::addTo )
                , ( bp::arg("group"), bp::arg("molecule"), bp::arg("map")=::SireFF::ParameterMap( ) ) );
        
        }
        { //::Squire::MolproFF::addTo
        
            typedef bool ( ::Squire::MolproFF::*addTo_function_type )( ::SireFF::FFBase::Group const &,::SireMol::Molecules const &,::SireFF::ParameterMap const & ) ;
            
            MolproFF_exposer.def( 
                "addTo"
                , addTo_function_type( &::Squire::MolproFF::addTo )
                , ( bp::arg("group"), bp::arg("molecules"), bp::arg("map")=::SireFF::ParameterMap( ) ) );
        
        }
        { //::Squire::MolproFF::addToMM
        
            typedef bool ( ::Squire::MolproFF::*addToMM_function_type )( ::SireMol::PartialMolecule const &,::SireFF::ParameterMap const & ) ;
            
            MolproFF_exposer.def( 
                "addToMM"
                , addToMM_function_type( &::Squire::MolproFF::addToMM )
                , ( bp::arg("molecule"), bp::arg("map")=::SireFF::ParameterMap( ) ) );
        
        }
        { //::Squire::MolproFF::addToMM
        
            typedef bool ( ::Squire::MolproFF::*addToMM_function_type )( ::SireMol::Molecules const &,::SireFF::ParameterMap const & ) ;
            
            MolproFF_exposer.def( 
                "addToMM"
                , addToMM_function_type( &::Squire::MolproFF::addToMM )
                , ( bp::arg("molecules"), bp::arg("map")=::SireFF::ParameterMap( ) ) );
        
        }
        { //::Squire::MolproFF::addToQM
        
            typedef bool ( ::Squire::MolproFF::*addToQM_function_type )( ::SireMol::PartialMolecule const &,::SireFF::ParameterMap const & ) ;
            
            MolproFF_exposer.def( 
                "addToQM"
                , addToQM_function_type( &::Squire::MolproFF::addToQM )
                , ( bp::arg("molecule"), bp::arg("map")=::SireFF::ParameterMap( ) ) );
        
        }
        { //::Squire::MolproFF::addToQM
        
            typedef bool ( ::Squire::MolproFF::*addToQM_function_type )( ::SireMol::Molecules const &,::SireFF::ParameterMap const & ) ;
            
            MolproFF_exposer.def( 
                "addToQM"
                , addToQM_function_type( &::Squire::MolproFF::addToQM )
                , ( bp::arg("molecules"), bp::arg("map")=::SireFF::ParameterMap( ) ) );
        
        }
        { //::Squire::MolproFF::basisSet
        
            typedef ::QString const & ( ::Squire::MolproFF::*basisSet_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "basisSet"
                , basisSet_function_type( &::Squire::MolproFF::basisSet )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::extraCommands
        
            typedef ::QString const & ( ::Squire::MolproFF::*extraCommands_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "extraCommands"
                , extraCommands_function_type(
                &::Squire::MolproFF::extraCommands )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::header
        
            typedef ::QString const & ( ::Squire::MolproFF::*header_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "header"
                , header_function_type(
                &::Squire::MolproFF::header )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::change
        
            typedef bool ( ::Squire::MolproFF::*change_function_type )( ::SireMol::PartialMolecule const & ) ;
            
            MolproFF_exposer.def( 
                "change"
                , change_function_type( &::Squire::MolproFF::change )
                , ( bp::arg("molecule") ) );
        
        }
        { //::Squire::MolproFF::change
        
            typedef bool ( ::Squire::MolproFF::*change_function_type )( ::SireMol::Molecules const & ) ;
            
            MolproFF_exposer.def( 
                "change"
                , change_function_type( &::Squire::MolproFF::change )
                , ( bp::arg("molecules") ) );
        
        }
        { //::Squire::MolproFF::components
        
            typedef ::Squire::MolproFF::Components const & ( ::Squire::MolproFF::*components_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "components"
                , components_function_type( &::Squire::MolproFF::components )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::contains
        
            typedef bool ( ::Squire::MolproFF::*contains_function_type )( ::SireMol::PartialMolecule const & ) const;
            
            MolproFF_exposer.def( 
                "contains"
                , contains_function_type( &::Squire::MolproFF::contains )
                , ( bp::arg("molecule") ) );
        
        }
        { //::Squire::MolproFF::contains
        
            typedef bool ( ::Squire::MolproFF::*contains_function_type )( ::SireMol::PartialMolecule const &,::SireFF::FFBase::Group const & ) const;
            
            MolproFF_exposer.def( 
                "contains"
                , contains_function_type( &::Squire::MolproFF::contains )
                , ( bp::arg("molecule"), bp::arg("group") ) );
        
        }
        { //::Squire::MolproFF::containsProperty
        
            typedef bool ( ::Squire::MolproFF::*containsProperty_function_type )( ::QString const & ) const;
            
            MolproFF_exposer.def( 
                "containsProperty"
                , containsProperty_function_type( &::Squire::MolproFF::containsProperty )
                , ( bp::arg("name") ) );
        
        }
        { //::Squire::MolproFF::contents
        
            typedef ::SireMol::Molecules ( ::Squire::MolproFF::*contents_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "contents"
                , contents_function_type( &::Squire::MolproFF::contents ) );
        
        }
        { //::Squire::MolproFF::contents
        
            typedef ::SireMol::Molecules ( ::Squire::MolproFF::*contents_function_type )( ::SireFF::FFBase::Group const & ) const;
            
            MolproFF_exposer.def( 
                "contents"
                , contents_function_type( &::Squire::MolproFF::contents )
                , ( bp::arg("group") ) );
        
        }
        { //::Squire::MolproFF::energyOrigin
        
            typedef double ( ::Squire::MolproFF::*energyOrigin_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "energyOrigin"
                , energyOrigin_function_type( &::Squire::MolproFF::energyOrigin ) );
        
        }
        { //::Squire::MolproFF::getProperty
        
            typedef ::SireBase::Property ( ::Squire::MolproFF::*getProperty_function_type )( ::QString const & ) const;
            
            MolproFF_exposer.def( 
                "getProperty"
                , getProperty_function_type( &::Squire::MolproFF::getProperty )
                , ( bp::arg("name") ) );
        
        }
        { //::Squire::MolproFF::groups
        
            typedef ::Squire::MolproFF::Groups const & ( ::Squire::MolproFF::*groups_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "groups"
                , groups_function_type( &::Squire::MolproFF::groups )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::groupsReferringTo
        
            typedef ::QSet<SireFF::FFBase::Group> ( ::Squire::MolproFF::*groupsReferringTo_function_type )( ::SireMol::MoleculeID ) const;
            
            MolproFF_exposer.def( 
                "groupsReferringTo"
                , groupsReferringTo_function_type( &::Squire::MolproFF::groupsReferringTo )
                , ( bp::arg("molid") ) );
        
        }
        { //::Squire::MolproFF::mmCoordsAndChargesArray
        
            typedef ::QVector<double> const & ( ::Squire::MolproFF::*mmCoordsAndChargesArray_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "mmCoordsAndChargesArray"
                , mmCoordsAndChargesArray_function_type( &::Squire::MolproFF::mmCoordsAndChargesArray )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::mmMolecules
        
            typedef ::SireMol::Molecules ( ::Squire::MolproFF::*mmMolecules_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "mmMolecules"
                , mmMolecules_function_type( &::Squire::MolproFF::mmMolecules ) );
        
        }
        { //::Squire::MolproFF::molecule
        
            typedef ::SireMol::PartialMolecule ( ::Squire::MolproFF::*molecule_function_type )( ::SireMol::MoleculeID ) const;
            
            MolproFF_exposer.def( 
                "molecule"
                , molecule_function_type( &::Squire::MolproFF::molecule )
                , ( bp::arg("molid") ) );
        
        }
        { //::Squire::MolproFF::molecule
        
            typedef ::SireMol::PartialMolecule ( ::Squire::MolproFF::*molecule_function_type )( ::SireMol::MoleculeID,::SireFF::FFBase::Group const & ) const;
            
            MolproFF_exposer.def( 
                "molecule"
                , molecule_function_type( &::Squire::MolproFF::molecule )
                , ( bp::arg("molid"), bp::arg("group") ) );
        
        }
        { //::Squire::MolproFF::moleculeIDs
        
            typedef ::QSet<SireMol::MoleculeID> ( ::Squire::MolproFF::*moleculeIDs_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "moleculeIDs"
                , moleculeIDs_function_type( &::Squire::MolproFF::moleculeIDs ) );
        
        }
        { //::Squire::MolproFF::moleculeIDs
        
            typedef ::QSet<SireMol::MoleculeID> ( ::Squire::MolproFF::*moleculeIDs_function_type )( ::SireFF::FFBase::Group const & ) const;
            
            MolproFF_exposer.def( 
                "moleculeIDs"
                , moleculeIDs_function_type( &::Squire::MolproFF::moleculeIDs )
                , ( bp::arg("group") ) );
        
        }
        { //::Squire::MolproFF::molproCommandInput
        
            typedef ::QString ( ::Squire::MolproFF::*molproCommandInput_function_type )(  ) ;
            
            MolproFF_exposer.def( 
                "molproCommandInput"
                , molproCommandInput_function_type( &::Squire::MolproFF::molproCommandInput ) );
        
        }
        { //::Squire::MolproFF::molproExe
        
            typedef ::QFileInfo const & ( ::Squire::MolproFF::*molproExe_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "molproExe"
                , molproExe_function_type( &::Squire::MolproFF::molproExe )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::molproTempDir
        
            typedef ::QDir const & ( ::Squire::MolproFF::*molproTempDir_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "molproTempDir"
                , molproTempDir_function_type( &::Squire::MolproFF::molproTempDir )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::mustNowRecalculateFromScratch
        
            typedef void ( ::Squire::MolproFF::*mustNowRecalculateFromScratch_function_type )(  ) ;
            
            MolproFF_exposer.def( 
                "mustNowRecalculateFromScratch"
                , mustNowRecalculateFromScratch_function_type( &::Squire::MolproFF::mustNowRecalculateFromScratch ) );
        
        }
        { //::Squire::MolproFF::parameters
        
            typedef ::Squire::MolproFF::Parameters const & ( ::Squire::MolproFF::*parameters_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "parameters"
                , parameters_function_type( &::Squire::MolproFF::parameters )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::program
        
            typedef ::QString const & ( ::Squire::MolproFF::*program_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "program"
                , program_function_type( &::Squire::MolproFF::program )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::properties
        
            typedef ::QHash<QString,SireBase::Property> ( ::Squire::MolproFF::*properties_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "properties"
                , properties_function_type( &::Squire::MolproFF::properties ) );
        
        }
        { //::Squire::MolproFF::qmCoordsArray
        
            typedef ::QVector<double> const & ( ::Squire::MolproFF::*qmCoordsArray_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "qmCoordsArray"
                , qmCoordsArray_function_type( &::Squire::MolproFF::qmCoordsArray )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::qmMolecules
        
            typedef ::SireMol::Molecules ( ::Squire::MolproFF::*qmMolecules_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "qmMolecules"
                , qmMolecules_function_type( &::Squire::MolproFF::qmMolecules ) );
        
        }
        { //::Squire::MolproFF::qmVersion
        
            typedef ::quint32 ( ::Squire::MolproFF::*qmVersion_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "qmVersion"
                , qmVersion_function_type( &::Squire::MolproFF::qmVersion ) );
        
        }
        { //::Squire::MolproFF::refersTo
        
            typedef bool ( ::Squire::MolproFF::*refersTo_function_type )( ::SireMol::MoleculeID ) const;
            
            MolproFF_exposer.def( 
                "refersTo"
                , refersTo_function_type( &::Squire::MolproFF::refersTo )
                , ( bp::arg("molid") ) );
        
        }
        { //::Squire::MolproFF::refersTo
        
            typedef bool ( ::Squire::MolproFF::*refersTo_function_type )( ::SireMol::MoleculeID,::SireFF::FFBase::Group const & ) const;
            
            MolproFF_exposer.def( 
                "refersTo"
                , refersTo_function_type( &::Squire::MolproFF::refersTo )
                , ( bp::arg("molid"), bp::arg("group") ) );
        
        }
        { //::Squire::MolproFF::remove
        
            typedef bool ( ::Squire::MolproFF::*remove_function_type )( ::SireMol::PartialMolecule const & ) ;
            
            MolproFF_exposer.def( 
                "remove"
                , remove_function_type( &::Squire::MolproFF::remove )
                , ( bp::arg("molecule") ) );
        
        }
        { //::Squire::MolproFF::remove
        
            typedef bool ( ::Squire::MolproFF::*remove_function_type )( ::SireMol::Molecules const & ) ;
            
            MolproFF_exposer.def( 
                "remove"
                , remove_function_type( &::Squire::MolproFF::remove )
                , ( bp::arg("molecules") ) );
        
        }
        { //::Squire::MolproFF::removeFrom
        
            typedef bool ( ::Squire::MolproFF::*removeFrom_function_type )( ::SireFF::FFBase::Group const &,::SireMol::PartialMolecule const & ) ;
            
            MolproFF_exposer.def( 
                "removeFrom"
                , removeFrom_function_type( &::Squire::MolproFF::removeFrom )
                , ( bp::arg("group"), bp::arg("molecule") ) );
        
        }
        { //::Squire::MolproFF::removeFrom
        
            typedef bool ( ::Squire::MolproFF::*removeFrom_function_type )( ::SireFF::FFBase::Group const &,::SireMol::Molecules const & ) ;
            
            MolproFF_exposer.def( 
                "removeFrom"
                , removeFrom_function_type( &::Squire::MolproFF::removeFrom )
                , ( bp::arg("group"), bp::arg("molecules") ) );
        
        }
        { //::Squire::MolproFF::removeFromMM
        
            typedef bool ( ::Squire::MolproFF::*removeFromMM_function_type )( ::SireMol::PartialMolecule const & ) ;
            
            MolproFF_exposer.def( 
                "removeFromMM"
                , removeFromMM_function_type( &::Squire::MolproFF::removeFromMM )
                , ( bp::arg("molecule") ) );
        
        }
        { //::Squire::MolproFF::removeFromMM
        
            typedef bool ( ::Squire::MolproFF::*removeFromMM_function_type )( ::SireMol::Molecules const & ) ;
            
            MolproFF_exposer.def( 
                "removeFromMM"
                , removeFromMM_function_type( &::Squire::MolproFF::removeFromMM )
                , ( bp::arg("molecules") ) );
        
        }
        { //::Squire::MolproFF::removeFromQM
        
            typedef bool ( ::Squire::MolproFF::*removeFromQM_function_type )( ::SireMol::PartialMolecule const & ) ;
            
            MolproFF_exposer.def( 
                "removeFromQM"
                , removeFromQM_function_type( &::Squire::MolproFF::removeFromQM )
                , ( bp::arg("molecule") ) );
        
        }
        { //::Squire::MolproFF::removeFromQM
        
            typedef bool ( ::Squire::MolproFF::*removeFromQM_function_type )( ::SireMol::Molecules const & ) ;
            
            MolproFF_exposer.def( 
                "removeFromQM"
                , removeFromQM_function_type( &::Squire::MolproFF::removeFromQM )
                , ( bp::arg("molecules") ) );
        
        }
        { //::Squire::MolproFF::setBasisSet
        
            typedef bool ( ::Squire::MolproFF::*setBasisSet_function_type )( ::QString const & ) ;
            
            MolproFF_exposer.def( 
                "setBasisSet"
                , setBasisSet_function_type( &::Squire::MolproFF::setBasisSet )
                , ( bp::arg("basisset") ) );
        
        }
        { //::Squire::MolproFF::setExtraCommands
        
            typedef bool ( ::Squire::MolproFF::*setExtraCommands_function_type )( ::QString const & ) ;
            
            MolproFF_exposer.def( 
                "setExtraCommands"
                , setExtraCommands_function_type(
                &::Squire::MolproFF::setExtraCommands )
                , ( bp::arg("extracmds") ) );
        
        }
        { //::Squire::MolproFF::setHeader
        
            typedef bool ( ::Squire::MolproFF::*setHeader_function_type )( ::QString const & ) ;
            
            MolproFF_exposer.def( 
                "setHeader"
                , setHeader_function_type(
                &::Squire::MolproFF::setHeader )
                , ( bp::arg("header") ) );
        
        }
        { //::Squire::MolproFF::setEnergyOrigin
        
            typedef bool ( ::Squire::MolproFF::*setEnergyOrigin_function_type )( double ) ;
            
            MolproFF_exposer.def( 
                "setEnergyOrigin"
                , setEnergyOrigin_function_type( &::Squire::MolproFF::setEnergyOrigin )
                , ( bp::arg("nrg_zero") ) );
        
        }
        { //::Squire::MolproFF::setMolproExe
        
            typedef bool ( ::Squire::MolproFF::*setMolproExe_function_type )( ::QFileInfo const & ) ;
            
            MolproFF_exposer.def( 
                "setMolproExe"
                , setMolproExe_function_type( &::Squire::MolproFF::setMolproExe )
                , ( bp::arg("molpro") ) );
        
        }
        { //::Squire::MolproFF::setMolproTempDir
        
            typedef bool ( ::Squire::MolproFF::*setMolproTempDir_function_type )( ::QDir const & ) ;
            
            MolproFF_exposer.def( 
                "setMolproTempDir"
                , setMolproTempDir_function_type( &::Squire::MolproFF::setMolproTempDir )
                , ( bp::arg("tempdir") ) );
        
        }
        { //::Squire::MolproFF::setProgram
        
            typedef bool ( ::Squire::MolproFF::*setProgram_function_type )( ::QString const & ) ;
            
            MolproFF_exposer.def( 
                "setProgram"
                , setProgram_function_type( &::Squire::MolproFF::setProgram )
                , ( bp::arg("cmd") ) );
        
        }
        { //::Squire::MolproFF::setProperty
        
            typedef bool ( ::Squire::MolproFF::*setProperty_function_type )( ::QString const &,::SireBase::Property const & ) ;
            
            MolproFF_exposer.def( 
                "setProperty"
                , setProperty_function_type( &::Squire::MolproFF::setProperty )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::Squire::MolproFF::setSpace
        
            typedef bool ( ::Squire::MolproFF::*setSpace_function_type )( ::SireVol::Space const & ) ;
            
            MolproFF_exposer.def( 
                "setSpace"
                , setSpace_function_type( &::Squire::MolproFF::setSpace )
                , ( bp::arg("space") ) );
        
        }
        { //::Squire::MolproFF::setSwitchingFunction
        
            typedef bool ( ::Squire::MolproFF::*setSwitchingFunction_function_type )( ::SireMM::SwitchingFunction const & ) ;
            
            MolproFF_exposer.def( 
                "setSwitchingFunction"
                , setSwitchingFunction_function_type( &::Squire::MolproFF::setSwitchingFunction )
                , ( bp::arg("switchfunc") ) );
        
        }
        { //::Squire::MolproFF::space
        
            typedef ::SireVol::Space const & ( ::Squire::MolproFF::*space_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "space"
                , space_function_type( &::Squire::MolproFF::space )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::switchingFunction
        
            typedef ::SireMM::SwitchingFunction const & ( ::Squire::MolproFF::*switchingFunction_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "switchingFunction"
                , switchingFunction_function_type( &::Squire::MolproFF::switchingFunction )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::MolproFF::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            
            MolproFF_exposer.def( 
                "typeName"
                , typeName_function_type( &::Squire::MolproFF::typeName ) );
        
        }
        { //::Squire::MolproFF::what
        
            typedef char const * ( ::Squire::MolproFF::*what_function_type )(  ) const;
            
            MolproFF_exposer.def( 
                "what"
                , what_function_type( &::Squire::MolproFF::what ) );
        
        }
        MolproFF_exposer.staticmethod( "typeName" );
        MolproFF_exposer.def( "__copy__", &__copy__);
        MolproFF_exposer.def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::Squire::MolproFF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        MolproFF_exposer.def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::Squire::MolproFF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        MolproFF_exposer.def( "__str__", &SirePy::__str__< ::Squire::MolproFF > );
    }

}
