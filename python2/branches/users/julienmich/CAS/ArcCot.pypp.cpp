// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ArcCot.pypp.hpp"

namespace bp = boost::python;

#include "SireMaths/errors.h"

#include "SireStream/datastream.h"

#include "complexvalues.h"

#include "exp.h"

#include "expression.h"

#include "identities.h"

#include "invtrigfuncs.h"

#include "trigfuncs.h"

#include "invtrigfuncs.h"

SireCAS::ArcCot __copy__(const SireCAS::ArcCot &other){ return SireCAS::ArcCot(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_ArcCot_class(){

    { //::SireCAS::ArcCot
        typedef bp::class_< SireCAS::ArcCot, bp::bases< SireCAS::SingleFunc, SireCAS::ExBase > > ArcCot_exposer_t;
        ArcCot_exposer_t ArcCot_exposer = ArcCot_exposer_t( "ArcCot" );
        bp::scope ArcCot_scope( ArcCot_exposer );
        ArcCot_exposer.def( bp::init< >() );
        ArcCot_exposer.def( bp::init< SireCAS::Expression const & >(( bp::arg("ex") )) );
        ArcCot_exposer.def( bp::init< SireCAS::ArcCot const & >(( bp::arg("other") )) );
        { //::SireCAS::ArcCot::evaluate
        
            typedef double ( ::SireCAS::ArcCot::*evaluate_function_type )( ::SireCAS::Values const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::ArcCot::evaluate );
            
            ArcCot_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireCAS::ArcCot::evaluate
        
            typedef ::SireMaths::Complex ( ::SireCAS::ArcCot::*evaluate_function_type )( ::SireCAS::ComplexValues const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::ArcCot::evaluate );
            
            ArcCot_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        ArcCot_exposer.def( bp::self == bp::other< SireCAS::ExBase >() );
        { //::SireCAS::ArcCot::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireCAS::ArcCot::typeName );
            
            ArcCot_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireCAS::ArcCot::what
        
            typedef char const * ( ::SireCAS::ArcCot::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireCAS::ArcCot::what );
            
            ArcCot_exposer.def( 
                "what"
                , what_function_value );
        
        }
        ArcCot_exposer.staticmethod( "typeName" );
        ArcCot_exposer.def( "__copy__", &__copy__);
        ArcCot_exposer.def( "__deepcopy__", &__copy__);
        ArcCot_exposer.def( "clone", &__copy__);
        ArcCot_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireCAS::ArcCot >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ArcCot_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireCAS::ArcCot >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ArcCot_exposer.def( "__str__", &__str__< ::SireCAS::ArcCot > );
        ArcCot_exposer.def( "__repr__", &__str__< ::SireCAS::ArcCot > );
    }

}
