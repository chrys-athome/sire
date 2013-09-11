// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ArcCoth.pypp.hpp"

namespace bp = boost::python;

#include "SireMaths/errors.h"

#include "SireStream/datastream.h"

#include "complexvalues.h"

#include "exp.h"

#include "expression.h"

#include "hyperbolicfuncs.h"

#include "identities.h"

#include "invhyperbolicfuncs.h"

#include "invtrigfuncs.h"

#include "trigfuncs.h"

#include "invhyperbolicfuncs.h"

SireCAS::ArcCoth __copy__(const SireCAS::ArcCoth &other){ return SireCAS::ArcCoth(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_ArcCoth_class(){

    { //::SireCAS::ArcCoth
        typedef bp::class_< SireCAS::ArcCoth, bp::bases< SireCAS::SingleFunc, SireCAS::ExBase > > ArcCoth_exposer_t;
        ArcCoth_exposer_t ArcCoth_exposer = ArcCoth_exposer_t( "ArcCoth", bp::init< >() );
        bp::scope ArcCoth_scope( ArcCoth_exposer );
        ArcCoth_exposer.def( bp::init< SireCAS::Expression const & >(( bp::arg("ex") )) );
        ArcCoth_exposer.def( bp::init< SireCAS::ArcCoth const & >(( bp::arg("other") )) );
        { //::SireCAS::ArcCoth::evaluate
        
            typedef double ( ::SireCAS::ArcCoth::*evaluate_function_type )( ::SireCAS::Values const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::ArcCoth::evaluate );
            
            ArcCoth_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireCAS::ArcCoth::evaluate
        
            typedef ::SireMaths::Complex ( ::SireCAS::ArcCoth::*evaluate_function_type )( ::SireCAS::ComplexValues const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::ArcCoth::evaluate );
            
            ArcCoth_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        ArcCoth_exposer.def( bp::self == bp::other< SireCAS::ExBase >() );
        { //::SireCAS::ArcCoth::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireCAS::ArcCoth::typeName );
            
            ArcCoth_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireCAS::ArcCoth::what
        
            typedef char const * ( ::SireCAS::ArcCoth::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireCAS::ArcCoth::what );
            
            ArcCoth_exposer.def( 
                "what"
                , what_function_value );
        
        }
        ArcCoth_exposer.staticmethod( "typeName" );
        ArcCoth_exposer.def( "__copy__", &__copy__);
        ArcCoth_exposer.def( "__deepcopy__", &__copy__);
        ArcCoth_exposer.def( "clone", &__copy__);
        ArcCoth_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireCAS::ArcCoth >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ArcCoth_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireCAS::ArcCoth >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ArcCoth_exposer.def( "__str__", &__str__< ::SireCAS::ArcCoth > );
        ArcCoth_exposer.def( "__repr__", &__str__< ::SireCAS::ArcCoth > );
    }

}