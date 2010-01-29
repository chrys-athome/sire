// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Max.pypp.hpp"

namespace bp = boost::python;

#include "SireMaths/complex.h"

#include "SireMaths/errors.h"

#include "Siren/stream.h"

#include "complexvalues.h"

#include "expression.h"

#include "identities.h"

#include "minmax.h"

#include "minmax.h"

#include "Siren/logger.h"

SireCAS::Max __copy__(const SireCAS::Max &other){ return SireCAS::Max(other); }

#include "Siren/str.hpp"

void register_Max_class(){

    { //::SireCAS::Max
        typedef bp::class_< SireCAS::Max, bp::bases< SireCAS::DoubleFunc, SireCAS::CASNode, Siren::Object > > Max_exposer_t;
        Max_exposer_t Max_exposer = Max_exposer_t( "Max" );
        bp::scope Max_scope( Max_exposer );
        Max_exposer.def( bp::init< >() );
        Max_exposer.def( bp::init< SireCAS::Expression const &, SireCAS::Expression const & >(( bp::arg("x"), bp::arg("y") )) );
        Max_exposer.def( bp::init< SireCAS::Max const & >(( bp::arg("other") )) );
        { //::SireCAS::Max::evaluate
        
            typedef double ( ::SireCAS::Max::*evaluate_function_type )( ::SireCAS::Values const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::Max::evaluate );
            
            Max_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireCAS::Max::evaluate
        
            typedef ::SireMaths::Complex ( ::SireCAS::Max::*evaluate_function_type )( ::SireCAS::ComplexValues const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::Max::evaluate );
            
            Max_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        Max_exposer.def( bp::self != bp::self );
        { //::SireCAS::Max::operator=
        
            typedef ::SireCAS::Max & ( ::SireCAS::Max::*assign_function_type )( ::SireCAS::Max const & ) ;
            assign_function_type assign_function_value( &::SireCAS::Max::operator= );
            
            Max_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Max_exposer.def( bp::self == bp::self );
        { //::SireCAS::Max::stream
        
            typedef void ( ::SireCAS::Max::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireCAS::Max::stream );
            
            Max_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        Max_exposer.def( "__copy__", &__copy__);
        Max_exposer.def( "__deepcopy__", &__copy__);
        Max_exposer.def( "clone", &__copy__);
        Max_exposer.def( "__str__", &__str__< ::SireCAS::Max > );
        Max_exposer.def( "__repr__", &__str__< ::SireCAS::Max > );
    }

}