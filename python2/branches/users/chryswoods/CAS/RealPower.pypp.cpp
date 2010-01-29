// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "RealPower.pypp.hpp"

namespace bp = boost::python;

#include "SireMaths/errors.h"

#include "Siren/stream.h"

#include "complexvalues.h"

#include "expression.h"

#include "identities.h"

#include "integrationconstant.h"

#include "powerconstant.h"

#include "symbol.h"

#include "values.h"

#include <QDebug>

#include "powerconstant.h"

#include "Siren/logger.h"

SireCAS::RealPower __copy__(const SireCAS::RealPower &other){ return SireCAS::RealPower(other); }

#include "Siren/str.hpp"

void register_RealPower_class(){

    { //::SireCAS::RealPower
        typedef bp::class_< SireCAS::RealPower, bp::bases< SireCAS::PowerFunction, SireCAS::CASNode, Siren::Object > > RealPower_exposer_t;
        RealPower_exposer_t RealPower_exposer = RealPower_exposer_t( "RealPower" );
        bp::scope RealPower_scope( RealPower_exposer );
        RealPower_exposer.def( bp::init< >() );
        RealPower_exposer.def( bp::init< SireCAS::Expression const &, double >(( bp::arg("expression"), bp::arg("power") )) );
        RealPower_exposer.def( bp::init< SireCAS::RealPower const & >(( bp::arg("other") )) );
        { //::SireCAS::RealPower::evaluate
        
            typedef double ( ::SireCAS::RealPower::*evaluate_function_type )( ::SireCAS::Values const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::RealPower::evaluate );
            
            RealPower_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireCAS::RealPower::evaluate
        
            typedef ::SireMaths::Complex ( ::SireCAS::RealPower::*evaluate_function_type )( ::SireCAS::ComplexValues const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::RealPower::evaluate );
            
            RealPower_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        RealPower_exposer.def( bp::self != bp::self );
        { //::SireCAS::RealPower::operator=
        
            typedef ::SireCAS::RealPower & ( ::SireCAS::RealPower::*assign_function_type )( ::SireCAS::RealPower const & ) ;
            assign_function_type assign_function_value( &::SireCAS::RealPower::operator= );
            
            RealPower_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        RealPower_exposer.def( bp::self == bp::self );
        { //::SireCAS::RealPower::power
        
            typedef ::SireCAS::Expression ( ::SireCAS::RealPower::*power_function_type )(  ) const;
            power_function_type power_function_value( &::SireCAS::RealPower::power );
            
            RealPower_exposer.def( 
                "power"
                , power_function_value );
        
        }
        { //::SireCAS::RealPower::stream
        
            typedef void ( ::SireCAS::RealPower::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireCAS::RealPower::stream );
            
            RealPower_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        RealPower_exposer.def( "__copy__", &__copy__);
        RealPower_exposer.def( "__deepcopy__", &__copy__);
        RealPower_exposer.def( "clone", &__copy__);
        RealPower_exposer.def( "__str__", &__str__< ::SireCAS::RealPower > );
        RealPower_exposer.def( "__repr__", &__str__< ::SireCAS::RealPower > );
    }

}