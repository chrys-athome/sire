// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "RationalPower.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "complexvalues.h"

#include "expression.h"

#include "expressions.h"

#include "identities.h"

#include "integrationconstant.h"

#include "powerconstant.h"

#include "symbol.h"

#include "symbols.h"

#include "values.h"

#include <QDebug>

#include "powerconstant.h"

SireCAS::RationalPower __copy__(const SireCAS::RationalPower &other){ return SireCAS::RationalPower(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_RationalPower_class(){

    { //::SireCAS::RationalPower
        typedef bp::class_< SireCAS::RationalPower, bp::bases< SireCAS::PowerFunction, SireCAS::ExBase > > RationalPower_exposer_t;
        RationalPower_exposer_t RationalPower_exposer = RationalPower_exposer_t( "RationalPower" );
        bp::scope RationalPower_scope( RationalPower_exposer );
        RationalPower_exposer.def( bp::init< >() );
        RationalPower_exposer.def( bp::init< SireCAS::Expression const &, SireMaths::Rational const & >(( bp::arg("expression"), bp::arg("power") )) );
        RationalPower_exposer.def( bp::init< SireCAS::RationalPower const & >(( bp::arg("other") )) );
        { //::SireCAS::RationalPower::evaluate
        
            typedef double ( ::SireCAS::RationalPower::*evaluate_function_type )( ::SireCAS::Values const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::RationalPower::evaluate );
            
            RationalPower_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireCAS::RationalPower::evaluate
        
            typedef ::SireMaths::Complex ( ::SireCAS::RationalPower::*evaluate_function_type )( ::SireCAS::ComplexValues const & ) const;
            evaluate_function_type evaluate_function_value( &::SireCAS::RationalPower::evaluate );
            
            RationalPower_exposer.def( 
                "evaluate"
                , evaluate_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireCAS::RationalPower::hash
        
            typedef ::uint ( ::SireCAS::RationalPower::*hash_function_type )(  ) const;
            hash_function_type hash_function_value( &::SireCAS::RationalPower::hash );
            
            RationalPower_exposer.def( 
                "hash"
                , hash_function_value );
        
        }
        RationalPower_exposer.def( bp::self == bp::other< SireCAS::ExBase >() );
        { //::SireCAS::RationalPower::power
        
            typedef ::SireCAS::Expression ( ::SireCAS::RationalPower::*power_function_type )(  ) const;
            power_function_type power_function_value( &::SireCAS::RationalPower::power );
            
            RationalPower_exposer.def( 
                "power"
                , power_function_value );
        
        }
        { //::SireCAS::RationalPower::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireCAS::RationalPower::typeName );
            
            RationalPower_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireCAS::RationalPower::what
        
            typedef char const * ( ::SireCAS::RationalPower::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireCAS::RationalPower::what );
            
            RationalPower_exposer.def( 
                "what"
                , what_function_value );
        
        }
        RationalPower_exposer.staticmethod( "typeName" );
        RationalPower_exposer.def( "__copy__", &__copy__);
        RationalPower_exposer.def( "__deepcopy__", &__copy__);
        RationalPower_exposer.def( "clone", &__copy__);
        RationalPower_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireCAS::RationalPower >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RationalPower_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireCAS::RationalPower >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RationalPower_exposer.def( "__str__", &__str__< ::SireCAS::RationalPower > );
        RationalPower_exposer.def( "__repr__", &__str__< ::SireCAS::RationalPower > );
    }

}
