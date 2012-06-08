// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "FastInterCLJFF.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/errors.h"

#include "SireBase/properties.h"

#include "SireBase/propertymap.h"

#include "SireError/errors.h"

#include "SireMol/molecule.h"

#include "SireMol/partialmolecule.h"

#include "SireMol/viewsofmol.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "fastintercljff.h"

#include "tostring.h"

#include "fastintercljff.h"

SireMM::FastInterCLJFF __copy__(const SireMM::FastInterCLJFF &other){ return SireMM::FastInterCLJFF(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_FastInterCLJFF_class(){

    { //::SireMM::FastInterCLJFF
        typedef bp::class_< SireMM::FastInterCLJFF, bp::bases< SireFF::FF3D, SireFF::G1FF, SireFF::FF, SireMol::MolGroupsBase, SireBase::Property > > FastInterCLJFF_exposer_t;
        FastInterCLJFF_exposer_t FastInterCLJFF_exposer = FastInterCLJFF_exposer_t( "FastInterCLJFF" );
        bp::scope FastInterCLJFF_scope( FastInterCLJFF_exposer );
        FastInterCLJFF_exposer.def( bp::init< >() );
        FastInterCLJFF_exposer.def( bp::init< QString const & >(( bp::arg("name") )) );
        FastInterCLJFF_exposer.def( bp::init< SireMM::FastInterCLJFF const & >(( bp::arg("other") )) );
        { //::SireMM::FastInterCLJFF::combiningRules
        
            typedef ::QString const & ( ::SireMM::FastInterCLJFF::*combiningRules_function_type )(  ) const;
            combiningRules_function_type combiningRules_function_value( &::SireMM::FastInterCLJFF::combiningRules );
            
            FastInterCLJFF_exposer.def( 
                "combiningRules"
                , combiningRules_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::FastInterCLJFF::components
        
            typedef ::SireMM::CLJComponent const & ( ::SireMM::FastInterCLJFF::*components_function_type )(  ) const;
            components_function_type components_function_value( &::SireMM::FastInterCLJFF::components );
            
            FastInterCLJFF_exposer.def( 
                "components"
                , components_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::FastInterCLJFF::containsProperty
        
            typedef bool ( ::SireMM::FastInterCLJFF::*containsProperty_function_type )( ::QString const & ) const;
            containsProperty_function_type containsProperty_function_value( &::SireMM::FastInterCLJFF::containsProperty );
            
            FastInterCLJFF_exposer.def( 
                "containsProperty"
                , containsProperty_function_value
                , ( bp::arg("name") ) );
        
        }
        { //::SireMM::FastInterCLJFF::energy
        
            typedef void ( ::SireMM::FastInterCLJFF::*energy_function_type )( ::SireFF::EnergyTable &,double ) ;
            energy_function_type energy_function_value( &::SireMM::FastInterCLJFF::energy );
            
            FastInterCLJFF_exposer.def( 
                "energy"
                , energy_function_value
                , ( bp::arg("energytable"), bp::arg("scale_energy")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::energy
        
            typedef void ( ::SireMM::FastInterCLJFF::*energy_function_type )( ::SireFF::EnergyTable &,::SireCAS::Symbol const &,double ) ;
            energy_function_type energy_function_value( &::SireMM::FastInterCLJFF::energy );
            
            FastInterCLJFF_exposer.def( 
                "energy"
                , energy_function_value
                , ( bp::arg("energytable"), bp::arg("symbol"), bp::arg("scale_energy")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::field
        
            typedef void ( ::SireMM::FastInterCLJFF::*field_function_type )( ::SireFF::FieldTable &,double ) ;
            field_function_type field_function_value( &::SireMM::FastInterCLJFF::field );
            
            FastInterCLJFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::field
        
            typedef void ( ::SireMM::FastInterCLJFF::*field_function_type )( ::SireFF::FieldTable &,::SireCAS::Symbol const &,double ) ;
            field_function_type field_function_value( &::SireMM::FastInterCLJFF::field );
            
            FastInterCLJFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("component"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::field
        
            typedef void ( ::SireMM::FastInterCLJFF::*field_function_type )( ::SireFF::FieldTable &,::SireFF::Probe const &,double ) ;
            field_function_type field_function_value( &::SireMM::FastInterCLJFF::field );
            
            FastInterCLJFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("probe"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::field
        
            typedef void ( ::SireMM::FastInterCLJFF::*field_function_type )( ::SireFF::FieldTable &,::SireCAS::Symbol const &,::SireFF::Probe const &,double ) ;
            field_function_type field_function_value( &::SireMM::FastInterCLJFF::field );
            
            FastInterCLJFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("component"), bp::arg("probe"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::force
        
            typedef void ( ::SireMM::FastInterCLJFF::*force_function_type )( ::SireFF::ForceTable &,double ) ;
            force_function_type force_function_value( &::SireMM::FastInterCLJFF::force );
            
            FastInterCLJFF_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("scale_force")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::force
        
            typedef void ( ::SireMM::FastInterCLJFF::*force_function_type )( ::SireFF::ForceTable &,::SireCAS::Symbol const &,double ) ;
            force_function_type force_function_value( &::SireMM::FastInterCLJFF::force );
            
            FastInterCLJFF_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("symbol"), bp::arg("scale_force")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::mustNowRecalculateFromScratch
        
            typedef void ( ::SireMM::FastInterCLJFF::*mustNowRecalculateFromScratch_function_type )(  ) ;
            mustNowRecalculateFromScratch_function_type mustNowRecalculateFromScratch_function_value( &::SireMM::FastInterCLJFF::mustNowRecalculateFromScratch );
            
            FastInterCLJFF_exposer.def( 
                "mustNowRecalculateFromScratch"
                , mustNowRecalculateFromScratch_function_value );
        
        }
        FastInterCLJFF_exposer.def( bp::self != bp::self );
        { //::SireMM::FastInterCLJFF::operator=
        
            typedef ::SireMM::FastInterCLJFF & ( ::SireMM::FastInterCLJFF::*assign_function_type )( ::SireMM::FastInterCLJFF const & ) ;
            assign_function_type assign_function_value( &::SireMM::FastInterCLJFF::operator= );
            
            FastInterCLJFF_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        FastInterCLJFF_exposer.def( bp::self == bp::self );
        { //::SireMM::FastInterCLJFF::potential
        
            typedef void ( ::SireMM::FastInterCLJFF::*potential_function_type )( ::SireFF::PotentialTable &,double ) ;
            potential_function_type potential_function_value( &::SireMM::FastInterCLJFF::potential );
            
            FastInterCLJFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::potential
        
            typedef void ( ::SireMM::FastInterCLJFF::*potential_function_type )( ::SireFF::PotentialTable &,::SireCAS::Symbol const &,double ) ;
            potential_function_type potential_function_value( &::SireMM::FastInterCLJFF::potential );
            
            FastInterCLJFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("component"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::potential
        
            typedef void ( ::SireMM::FastInterCLJFF::*potential_function_type )( ::SireFF::PotentialTable &,::SireFF::Probe const &,double ) ;
            potential_function_type potential_function_value( &::SireMM::FastInterCLJFF::potential );
            
            FastInterCLJFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("probe"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::potential
        
            typedef void ( ::SireMM::FastInterCLJFF::*potential_function_type )( ::SireFF::PotentialTable &,::SireCAS::Symbol const &,::SireFF::Probe const &,double ) ;
            potential_function_type potential_function_value( &::SireMM::FastInterCLJFF::potential );
            
            FastInterCLJFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("component"), bp::arg("probe"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireMM::FastInterCLJFF::properties
        
            typedef ::SireBase::Properties const & ( ::SireMM::FastInterCLJFF::*properties_function_type )(  ) const;
            properties_function_type properties_function_value( &::SireMM::FastInterCLJFF::properties );
            
            FastInterCLJFF_exposer.def( 
                "properties"
                , properties_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::FastInterCLJFF::property
        
            typedef ::SireBase::Property const & ( ::SireMM::FastInterCLJFF::*property_function_type )( ::QString const & ) const;
            property_function_type property_function_value( &::SireMM::FastInterCLJFF::property );
            
            FastInterCLJFF_exposer.def( 
                "property"
                , property_function_value
                , ( bp::arg("name") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::FastInterCLJFF::setCombiningRules
        
            typedef bool ( ::SireMM::FastInterCLJFF::*setCombiningRules_function_type )( ::QString const & ) ;
            setCombiningRules_function_type setCombiningRules_function_value( &::SireMM::FastInterCLJFF::setCombiningRules );
            
            FastInterCLJFF_exposer.def( 
                "setCombiningRules"
                , setCombiningRules_function_value
                , ( bp::arg("combiningrules") ) );
        
        }
        { //::SireMM::FastInterCLJFF::setProperty
        
            typedef bool ( ::SireMM::FastInterCLJFF::*setProperty_function_type )( ::QString const &,::SireBase::Property const & ) ;
            setProperty_function_type setProperty_function_value( &::SireMM::FastInterCLJFF::setProperty );
            
            FastInterCLJFF_exposer.def( 
                "setProperty"
                , setProperty_function_value
                , ( bp::arg("name"), bp::arg("property") ) );
        
        }
        { //::SireMM::FastInterCLJFF::setShiftElectrostatics
        
            typedef bool ( ::SireMM::FastInterCLJFF::*setShiftElectrostatics_function_type )( bool ) ;
            setShiftElectrostatics_function_type setShiftElectrostatics_function_value( &::SireMM::FastInterCLJFF::setShiftElectrostatics );
            
            FastInterCLJFF_exposer.def( 
                "setShiftElectrostatics"
                , setShiftElectrostatics_function_value
                , ( bp::arg("switchelectro") ) );
        
        }
        { //::SireMM::FastInterCLJFF::setSpace
        
            typedef bool ( ::SireMM::FastInterCLJFF::*setSpace_function_type )( ::SireVol::Space const & ) ;
            setSpace_function_type setSpace_function_value( &::SireMM::FastInterCLJFF::setSpace );
            
            FastInterCLJFF_exposer.def( 
                "setSpace"
                , setSpace_function_value
                , ( bp::arg("space") ) );
        
        }
        { //::SireMM::FastInterCLJFF::setSwitchingFunction
        
            typedef bool ( ::SireMM::FastInterCLJFF::*setSwitchingFunction_function_type )( ::SireMM::SwitchingFunction const & ) ;
            setSwitchingFunction_function_type setSwitchingFunction_function_value( &::SireMM::FastInterCLJFF::setSwitchingFunction );
            
            FastInterCLJFF_exposer.def( 
                "setSwitchingFunction"
                , setSwitchingFunction_function_value
                , ( bp::arg("new_switchfunc") ) );
        
        }
        { //::SireMM::FastInterCLJFF::shiftElectrostatics
        
            typedef bool ( ::SireMM::FastInterCLJFF::*shiftElectrostatics_function_type )(  ) const;
            shiftElectrostatics_function_type shiftElectrostatics_function_value( &::SireMM::FastInterCLJFF::shiftElectrostatics );
            
            FastInterCLJFF_exposer.def( 
                "shiftElectrostatics"
                , shiftElectrostatics_function_value );
        
        }
        { //::SireMM::FastInterCLJFF::space
        
            typedef ::SireVol::Space const & ( ::SireMM::FastInterCLJFF::*space_function_type )(  ) const;
            space_function_type space_function_value( &::SireMM::FastInterCLJFF::space );
            
            FastInterCLJFF_exposer.def( 
                "space"
                , space_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::FastInterCLJFF::switchingFunction
        
            typedef ::SireMM::SwitchingFunction const & ( ::SireMM::FastInterCLJFF::*switchingFunction_function_type )(  ) const;
            switchingFunction_function_type switchingFunction_function_value( &::SireMM::FastInterCLJFF::switchingFunction );
            
            FastInterCLJFF_exposer.def( 
                "switchingFunction"
                , switchingFunction_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::FastInterCLJFF::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::FastInterCLJFF::typeName );
            
            FastInterCLJFF_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        FastInterCLJFF_exposer.staticmethod( "typeName" );
        FastInterCLJFF_exposer.def( "__copy__", &__copy__);
        FastInterCLJFF_exposer.def( "__deepcopy__", &__copy__);
        FastInterCLJFF_exposer.def( "clone", &__copy__);
        FastInterCLJFF_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::FastInterCLJFF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        FastInterCLJFF_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::FastInterCLJFF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        FastInterCLJFF_exposer.def( "__str__", &__str__< ::SireMM::FastInterCLJFF > );
        FastInterCLJFF_exposer.def( "__repr__", &__str__< ::SireMM::FastInterCLJFF > );
    }

}