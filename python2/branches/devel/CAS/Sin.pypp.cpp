// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Sin.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "complexvalues.h"

#include "exp.h"

#include "expression.h"

#include "identities.h"

#include "trigfuncs.h"

#include "trigfuncs.h"

SireCAS::Sin __copy__(const SireCAS::Sin &other){ return SireCAS::Sin(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_Sin_class(){

    { //::SireCAS::Sin
        typedef bp::class_< SireCAS::Sin, bp::bases< SireCAS::SingleFunc, SireCAS::ExBase > > Sin_exposer_t;
        Sin_exposer_t Sin_exposer = Sin_exposer_t( "Sin" );
        bp::scope Sin_scope( Sin_exposer );
        Sin_exposer.def( bp::init< >() );
        Sin_exposer.def( bp::init< SireCAS::Expression const & >(( bp::arg("ex") )) );
        Sin_exposer.def( bp::init< SireCAS::Sin const & >(( bp::arg("other") )) );
        { //::SireCAS::Sin::evaluate
        
            typedef double ( ::SireCAS::Sin::*evaluate_function_type )( ::SireCAS::Values const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::Sin::evaluate );
            
            Sin_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireCAS::Sin::evaluate
        
            typedef ::SireMaths::Complex ( ::SireCAS::Sin::*evaluate_function_type )( ::SireCAS::ComplexValues const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::Sin::evaluate );
            
            Sin_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        Sin_exposer.def( bp::self == bp::other< SireCAS::ExBase >() );
        { //::SireCAS::Sin::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireCAS::Sin::typeName );
            
            Sin_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireCAS::Sin::what
        
            typedef char const * ( ::SireCAS::Sin::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireCAS::Sin::what );
            
            Sin_exposer.def( 
                "what"
                , what_function_value );
        
        }
        Sin_exposer.staticmethod( "typeName" );
        Sin_exposer.def( "__copy__", &__copy__);
        Sin_exposer.def( "__deepcopy__", &__copy__);
        Sin_exposer.def( "clone", &__copy__);
        Sin_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireCAS::Sin >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Sin_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireCAS::Sin >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Sin_exposer.def( "__str__", &__str__< ::SireCAS::Sin > );
        Sin_exposer.def( "__repr__", &__str__< ::SireCAS::Sin > );
    }

}
