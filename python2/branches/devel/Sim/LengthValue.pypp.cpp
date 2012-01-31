// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "LengthValue.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireSim/values.h"

#include "values.h"

#include <QDebug>

#include <QDomDocument>

#include <QDomElement>

#include <QSet>

#include "values.h"

SireSim::LengthValue __copy__(const SireSim::LengthValue &other){ return SireSim::LengthValue(other); }

#include "Helpers/str.hpp"

void register_LengthValue_class(){

    { //::SireSim::LengthValue
        typedef bp::class_< SireSim::LengthValue, bp::bases< SireSim::Value > > LengthValue_exposer_t;
        LengthValue_exposer_t LengthValue_exposer = LengthValue_exposer_t( "LengthValue", bp::init< >() );
        bp::scope LengthValue_scope( LengthValue_exposer );
        LengthValue_exposer.def( bp::init< double, bp::optional< QString > >(( bp::arg("val"), bp::arg("units")=::QString( ) )) );
        LengthValue_exposer.def( bp::init< QString >(( bp::arg("value") )) );
        LengthValue_exposer.def( bp::init< QDomElement >(( bp::arg("elem") )) );
        LengthValue_exposer.def( bp::init< SireSim::LengthValue const & >(( bp::arg("other") )) );
        LengthValue_exposer.def( bp::self != bp::self );
        { //::SireSim::LengthValue::operator=
        
            typedef ::SireSim::LengthValue & ( ::SireSim::LengthValue::*assign_function_type )( ::SireSim::LengthValue const & ) ;
            assign_function_type assign_function_value( &::SireSim::LengthValue::operator= );
            
            LengthValue_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        LengthValue_exposer.def( bp::self == bp::self );
        { //::SireSim::LengthValue::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireSim::LengthValue::typeName );
            
            LengthValue_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireSim::LengthValue::value
        
            typedef double ( ::SireSim::LengthValue::*value_function_type )(  ) const;
            value_function_type value_function_value( &::SireSim::LengthValue::value );
            
            LengthValue_exposer.def( 
                "value"
                , value_function_value );
        
        }
        { //::SireSim::LengthValue::value
        
            typedef double ( ::SireSim::LengthValue::*value_function_type )( ::QString ) const;
            value_function_type value_function_value( &::SireSim::LengthValue::value );
            
            LengthValue_exposer.def( 
                "value"
                , value_function_value
                , ( bp::arg("units") ) );
        
        }
        { //::SireSim::LengthValue::valueWithMaximum
        
            typedef ::SireSim::LengthValue ( *valueWithMaximum_function_type )( double,double,::QString );
            valueWithMaximum_function_type valueWithMaximum_function_value( &::SireSim::LengthValue::valueWithMaximum );
            
            LengthValue_exposer.def( 
                "valueWithMaximum"
                , valueWithMaximum_function_value
                , ( bp::arg("val"), bp::arg("maximum"), bp::arg("units")=::QString( ) ) );
        
        }
        { //::SireSim::LengthValue::valueWithMinimum
        
            typedef ::SireSim::LengthValue ( *valueWithMinimum_function_type )( double,double,::QString );
            valueWithMinimum_function_type valueWithMinimum_function_value( &::SireSim::LengthValue::valueWithMinimum );
            
            LengthValue_exposer.def( 
                "valueWithMinimum"
                , valueWithMinimum_function_value
                , ( bp::arg("val"), bp::arg("minimum"), bp::arg("units")=::QString( ) ) );
        
        }
        { //::SireSim::LengthValue::valueWithRange
        
            typedef ::SireSim::LengthValue ( *valueWithRange_function_type )( double,double,double,::QString );
            valueWithRange_function_type valueWithRange_function_value( &::SireSim::LengthValue::valueWithRange );
            
            LengthValue_exposer.def( 
                "valueWithRange"
                , valueWithRange_function_value
                , ( bp::arg("val"), bp::arg("minimum"), bp::arg("maximum"), bp::arg("units")=::QString( ) ) );
        
        }
        { //::SireSim::LengthValue::what
        
            typedef char const * ( ::SireSim::LengthValue::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireSim::LengthValue::what );
            
            LengthValue_exposer.def( 
                "what"
                , what_function_value );
        
        }
        LengthValue_exposer.staticmethod( "typeName" );
        LengthValue_exposer.staticmethod( "valueWithMaximum" );
        LengthValue_exposer.staticmethod( "valueWithMinimum" );
        LengthValue_exposer.staticmethod( "valueWithRange" );
        LengthValue_exposer.def( "__copy__", &__copy__);
        LengthValue_exposer.def( "__deepcopy__", &__copy__);
        LengthValue_exposer.def( "clone", &__copy__);
        LengthValue_exposer.def( "__str__", &__str__< ::SireSim::LengthValue > );
        LengthValue_exposer.def( "__repr__", &__str__< ::SireSim::LengthValue > );
    }

}