// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "InterFF.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "cljshiftfunction.h"

#include "interff.h"

#include "interff.h"

SireMM::InterFF __copy__(const SireMM::InterFF &other){ return SireMM::InterFF(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_InterFF_class(){

    { //::SireMM::InterFF
        typedef bp::class_< SireMM::InterFF, bp::bases< SireFF::G1FF, SireFF::FF, SireMol::MolGroupsBase, SireBase::Property > > InterFF_exposer_t;
        InterFF_exposer_t InterFF_exposer = InterFF_exposer_t( "InterFF", bp::init< >() );
        bp::scope InterFF_scope( InterFF_exposer );
        InterFF_exposer.def( bp::init< QString const & >(( bp::arg("name") )) );
        InterFF_exposer.def( bp::init< SireMM::InterFF const & >(( bp::arg("other") )) );
        { //::SireMM::InterFF::components
        
            typedef ::SireMM::CLJComponent const & ( ::SireMM::InterFF::*components_function_type )(  ) const;
            components_function_type components_function_value( &::SireMM::InterFF::components );
            
            InterFF_exposer.def( 
                "components"
                , components_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InterFF::containsProperty
        
            typedef bool ( ::SireMM::InterFF::*containsProperty_function_type )( ::QString const & ) const;
            containsProperty_function_type containsProperty_function_value( &::SireMM::InterFF::containsProperty );
            
            InterFF_exposer.def( 
                "containsProperty"
                , containsProperty_function_value
                , ( bp::arg("name") ) );
        
        }
        { //::SireMM::InterFF::mustNowRecalculateFromScratch
        
            typedef void ( ::SireMM::InterFF::*mustNowRecalculateFromScratch_function_type )(  ) ;
            mustNowRecalculateFromScratch_function_type mustNowRecalculateFromScratch_function_value( &::SireMM::InterFF::mustNowRecalculateFromScratch );
            
            InterFF_exposer.def( 
                "mustNowRecalculateFromScratch"
                , mustNowRecalculateFromScratch_function_value );
        
        }
        InterFF_exposer.def( bp::self != bp::self );
        { //::SireMM::InterFF::operator=
        
            typedef ::SireMM::InterFF & ( ::SireMM::InterFF::*assign_function_type )( ::SireMM::InterFF const & ) ;
            assign_function_type assign_function_value( &::SireMM::InterFF::operator= );
            
            InterFF_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        InterFF_exposer.def( bp::self == bp::self );
        { //::SireMM::InterFF::properties
        
            typedef ::SireBase::Properties const & ( ::SireMM::InterFF::*properties_function_type )(  ) const;
            properties_function_type properties_function_value( &::SireMM::InterFF::properties );
            
            InterFF_exposer.def( 
                "properties"
                , properties_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::InterFF::property
        
            typedef ::SireBase::Property const & ( ::SireMM::InterFF::*property_function_type )( ::QString const & ) const;
            property_function_type property_function_value( &::SireMM::InterFF::property );
            
            InterFF_exposer.def( 
                "property"
                , property_function_value
                , ( bp::arg("name") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InterFF::setProperty
        
            typedef bool ( ::SireMM::InterFF::*setProperty_function_type )( ::QString const &,::SireBase::Property const & ) ;
            setProperty_function_type setProperty_function_value( &::SireMM::InterFF::setProperty );
            
            InterFF_exposer.def( 
                "setProperty"
                , setProperty_function_value
                , ( bp::arg("name"), bp::arg("property") ) );
        
        }
        { //::SireMM::InterFF::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::InterFF::typeName );
            
            InterFF_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::InterFF::what
        
            typedef char const * ( ::SireMM::InterFF::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMM::InterFF::what );
            
            InterFF_exposer.def( 
                "what"
                , what_function_value );
        
        }
        InterFF_exposer.staticmethod( "typeName" );
        InterFF_exposer.def( "__copy__", &__copy__);
        InterFF_exposer.def( "__deepcopy__", &__copy__);
        InterFF_exposer.def( "clone", &__copy__);
        InterFF_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::InterFF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        InterFF_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::InterFF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        InterFF_exposer.def( "__str__", &__str__< ::SireMM::InterFF > );
        InterFF_exposer.def( "__repr__", &__str__< ::SireMM::InterFF > );
        InterFF_exposer.def( "__len__", &__len_count< ::SireMM::InterFF > );
    }

}
