// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "IntegrationConstant.pypp.hpp"

namespace bp = boost::python;

#include "SireCAS/errors.h"

#include "SireStream/datastream.h"

#include "integrationconstant.h"

#include "integrationconstant.h"

SireCAS::IntegrationConstant __copy__(const SireCAS::IntegrationConstant &other){ return SireCAS::IntegrationConstant(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_IntegrationConstant_class(){

    { //::SireCAS::IntegrationConstant
        typedef bp::class_< SireCAS::IntegrationConstant, bp::bases< SireCAS::Symbol, SireCAS::ExBase > > IntegrationConstant_exposer_t;
        IntegrationConstant_exposer_t IntegrationConstant_exposer = IntegrationConstant_exposer_t( "IntegrationConstant", bp::init< >() );
        bp::scope IntegrationConstant_scope( IntegrationConstant_exposer );
        IntegrationConstant_exposer.def( bp::init< SireCAS::IntegrationConstant const & >(( bp::arg("other") )) );
        { //::SireCAS::IntegrationConstant::hash
        
            typedef ::uint ( ::SireCAS::IntegrationConstant::*hash_function_type )(  ) const;
            hash_function_type hash_function_value( &::SireCAS::IntegrationConstant::hash );
            
            IntegrationConstant_exposer.def( 
                "hash"
                , hash_function_value );
        
        }
        { //::SireCAS::IntegrationConstant::integrate
        
            typedef ::SireCAS::Expression ( ::SireCAS::IntegrationConstant::*integrate_function_type )( ::SireCAS::Symbol const & ) const;
            integrate_function_type integrate_function_value( &::SireCAS::IntegrationConstant::integrate );
            
            IntegrationConstant_exposer.def( 
                "integrate"
                , integrate_function_value
                , ( bp::arg("symbol") ) );
        
        }
        IntegrationConstant_exposer.def( bp::self == bp::other< SireCAS::ExBase >() );
        { //::SireCAS::IntegrationConstant::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireCAS::IntegrationConstant::typeName );
            
            IntegrationConstant_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireCAS::IntegrationConstant::what
        
            typedef char const * ( ::SireCAS::IntegrationConstant::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireCAS::IntegrationConstant::what );
            
            IntegrationConstant_exposer.def( 
                "what"
                , what_function_value );
        
        }
        IntegrationConstant_exposer.staticmethod( "typeName" );
        IntegrationConstant_exposer.def( "__copy__", &__copy__);
        IntegrationConstant_exposer.def( "__deepcopy__", &__copy__);
        IntegrationConstant_exposer.def( "clone", &__copy__);
        IntegrationConstant_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireCAS::IntegrationConstant >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        IntegrationConstant_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireCAS::IntegrationConstant >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        IntegrationConstant_exposer.def( "__str__", &__str__< ::SireCAS::IntegrationConstant > );
        IntegrationConstant_exposer.def( "__repr__", &__str__< ::SireCAS::IntegrationConstant > );
    }

}
