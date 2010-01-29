// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ArcCosh.pypp.hpp"

namespace bp = boost::python;

#include "SireMaths/complex.h"

#include "SireMaths/errors.h"

#include "Siren/stream.h"

#include "complexvalues.h"

#include "exp.h"

#include "expression.h"

#include "hyperbolicfuncs.h"

#include "identities.h"

#include "invhyperbolicfuncs.h"

#include "invtrigfuncs.h"

#include "trigfuncs.h"

#include <boost/math/special_functions/asinh.hpp>

#include "invhyperbolicfuncs.h"

#include "Siren/logger.h"

SireCAS::ArcCosh __copy__(const SireCAS::ArcCosh &other){ return SireCAS::ArcCosh(other); }

#include "Siren/str.hpp"

void register_ArcCosh_class(){

    { //::SireCAS::ArcCosh
        typedef bp::class_< SireCAS::ArcCosh, bp::bases< SireCAS::SingleFunc, SireCAS::CASNode, Siren::Object > > ArcCosh_exposer_t;
        ArcCosh_exposer_t ArcCosh_exposer = ArcCosh_exposer_t( "ArcCosh" );
        bp::scope ArcCosh_scope( ArcCosh_exposer );
        ArcCosh_exposer.def( bp::init< >() );
        ArcCosh_exposer.def( bp::init< SireCAS::Expression const & >(( bp::arg("ex") )) );
        ArcCosh_exposer.def( bp::init< SireCAS::ArcCosh const & >(( bp::arg("other") )) );
        { //::SireCAS::ArcCosh::evaluate
        
            typedef double ( ::SireCAS::ArcCosh::*evaluate_function_type )( ::SireCAS::Values const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::ArcCosh::evaluate );
            
            ArcCosh_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireCAS::ArcCosh::evaluate
        
            typedef ::SireMaths::Complex ( ::SireCAS::ArcCosh::*evaluate_function_type )( ::SireCAS::ComplexValues const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::ArcCosh::evaluate );
            
            ArcCosh_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        ArcCosh_exposer.def( bp::self != bp::self );
        { //::SireCAS::ArcCosh::operator=
        
            typedef ::SireCAS::ArcCosh & ( ::SireCAS::ArcCosh::*assign_function_type )( ::SireCAS::ArcCosh const & ) ;
            assign_function_type assign_function_value( &::SireCAS::ArcCosh::operator= );
            
            ArcCosh_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        ArcCosh_exposer.def( bp::self == bp::self );
        { //::SireCAS::ArcCosh::stream
        
            typedef void ( ::SireCAS::ArcCosh::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireCAS::ArcCosh::stream );
            
            ArcCosh_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        ArcCosh_exposer.def( "__copy__", &__copy__);
        ArcCosh_exposer.def( "__deepcopy__", &__copy__);
        ArcCosh_exposer.def( "clone", &__copy__);
        ArcCosh_exposer.def( "__str__", &__str__< ::SireCAS::ArcCosh > );
        ArcCosh_exposer.def( "__repr__", &__str__< ::SireCAS::ArcCosh > );
    }

}