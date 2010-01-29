// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Tanh.pypp.hpp"

namespace bp = boost::python;

#include "SireMaths/complex.h"

#include "Siren/stream.h"

#include "complexvalues.h"

#include "exp.h"

#include "expression.h"

#include "hyperbolicfuncs.h"

#include "identities.h"

#include "trigfuncs.h"

#include "hyperbolicfuncs.h"

#include "Siren/logger.h"

SireCAS::Tanh __copy__(const SireCAS::Tanh &other){ return SireCAS::Tanh(other); }

#include "Siren/str.hpp"

void register_Tanh_class(){

    { //::SireCAS::Tanh
        typedef bp::class_< SireCAS::Tanh, bp::bases< SireCAS::SingleFunc, SireCAS::CASNode, Siren::Object > > Tanh_exposer_t;
        Tanh_exposer_t Tanh_exposer = Tanh_exposer_t( "Tanh" );
        bp::scope Tanh_scope( Tanh_exposer );
        Tanh_exposer.def( bp::init< >() );
        Tanh_exposer.def( bp::init< SireCAS::Expression const & >(( bp::arg("ex") )) );
        Tanh_exposer.def( bp::init< SireCAS::Tanh const & >(( bp::arg("other") )) );
        { //::SireCAS::Tanh::evaluate
        
            typedef double ( ::SireCAS::Tanh::*evaluate_function_type )( ::SireCAS::Values const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::Tanh::evaluate );
            
            Tanh_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireCAS::Tanh::evaluate
        
            typedef ::SireMaths::Complex ( ::SireCAS::Tanh::*evaluate_function_type )( ::SireCAS::ComplexValues const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::Tanh::evaluate );
            
            Tanh_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        Tanh_exposer.def( bp::self != bp::self );
        { //::SireCAS::Tanh::operator=
        
            typedef ::SireCAS::Tanh & ( ::SireCAS::Tanh::*assign_function_type )( ::SireCAS::Tanh const & ) ;
            assign_function_type assign_function_value( &::SireCAS::Tanh::operator= );
            
            Tanh_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Tanh_exposer.def( bp::self == bp::self );
        { //::SireCAS::Tanh::stream
        
            typedef void ( ::SireCAS::Tanh::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireCAS::Tanh::stream );
            
            Tanh_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        Tanh_exposer.def( "__copy__", &__copy__);
        Tanh_exposer.def( "__deepcopy__", &__copy__);
        Tanh_exposer.def( "clone", &__copy__);
        Tanh_exposer.def( "__str__", &__str__< ::SireCAS::Tanh > );
        Tanh_exposer.def( "__repr__", &__str__< ::SireCAS::Tanh > );
    }

}