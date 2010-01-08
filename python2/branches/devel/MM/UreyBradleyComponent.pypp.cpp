// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "UreyBradleyComponent.pypp.hpp"

namespace bp = boost::python;

#include "SireFF/ff.h"

#include "SireStream/datastream.h"

#include "internalcomponent.h"

#include "internalcomponent.h"

SireMM::UreyBradleyComponent __copy__(const SireMM::UreyBradleyComponent &other){ return SireMM::UreyBradleyComponent(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_UreyBradleyComponent_class(){

    { //::SireMM::UreyBradleyComponent
        typedef bp::class_< SireMM::UreyBradleyComponent, bp::bases< SireCAS::Symbol, SireCAS::ExBase > > UreyBradleyComponent_exposer_t;
        UreyBradleyComponent_exposer_t UreyBradleyComponent_exposer = UreyBradleyComponent_exposer_t( "UreyBradleyComponent", bp::init< bp::optional< SireFF::FFName const & > >(( bp::arg("ffname")=SireFF::FFName() )) );
        bp::scope UreyBradleyComponent_scope( UreyBradleyComponent_exposer );
        UreyBradleyComponent_exposer.def( bp::init< SireCAS::Symbol const & >(( bp::arg("symbol") )) );
        UreyBradleyComponent_exposer.def( bp::init< SireMM::UreyBradleyComponent const & >(( bp::arg("other") )) );
        { //::SireMM::UreyBradleyComponent::changeEnergy
        
            typedef void ( ::SireMM::UreyBradleyComponent::*changeEnergy_function_type )( ::SireFF::FF &,::SireMM::UreyBradleyEnergy const & ) const;
            changeEnergy_function_type changeEnergy_function_value( &::SireMM::UreyBradleyComponent::changeEnergy );
            
            UreyBradleyComponent_exposer.def( 
                "changeEnergy"
                , changeEnergy_function_value
                , ( bp::arg("ff"), bp::arg("ubnrg") ) );
        
        }
        { //::SireMM::UreyBradleyComponent::setEnergy
        
            typedef void ( ::SireMM::UreyBradleyComponent::*setEnergy_function_type )( ::SireFF::FF &,::SireMM::UreyBradleyEnergy const & ) const;
            setEnergy_function_type setEnergy_function_value( &::SireMM::UreyBradleyComponent::setEnergy );
            
            UreyBradleyComponent_exposer.def( 
                "setEnergy"
                , setEnergy_function_value
                , ( bp::arg("ff"), bp::arg("ubnrg") ) );
        
        }
        { //::SireMM::UreyBradleyComponent::symbols
        
            typedef ::SireCAS::Symbols ( ::SireMM::UreyBradleyComponent::*symbols_function_type )(  ) const;
            symbols_function_type symbols_function_value( &::SireMM::UreyBradleyComponent::symbols );
            
            UreyBradleyComponent_exposer.def( 
                "symbols"
                , symbols_function_value );
        
        }
        { //::SireMM::UreyBradleyComponent::total
        
            typedef ::SireMM::UreyBradleyComponent const & ( ::SireMM::UreyBradleyComponent::*total_function_type )(  ) const;
            total_function_type total_function_value( &::SireMM::UreyBradleyComponent::total );
            
            UreyBradleyComponent_exposer.def( 
                "total"
                , total_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::UreyBradleyComponent::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::UreyBradleyComponent::typeName );
            
            UreyBradleyComponent_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::UreyBradleyComponent::what
        
            typedef char const * ( ::SireMM::UreyBradleyComponent::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMM::UreyBradleyComponent::what );
            
            UreyBradleyComponent_exposer.def( 
                "what"
                , what_function_value );
        
        }
        UreyBradleyComponent_exposer.staticmethod( "typeName" );
        UreyBradleyComponent_exposer.def( "__copy__", &__copy__);
        UreyBradleyComponent_exposer.def( "__deepcopy__", &__copy__);
        UreyBradleyComponent_exposer.def( "clone", &__copy__);
        UreyBradleyComponent_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::UreyBradleyComponent >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        UreyBradleyComponent_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::UreyBradleyComponent >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        UreyBradleyComponent_exposer.def( "__str__", &__str__< ::SireMM::UreyBradleyComponent > );
        UreyBradleyComponent_exposer.def( "__repr__", &__str__< ::SireMM::UreyBradleyComponent > );
    }

}
