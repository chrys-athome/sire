// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "StringValue.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "simparams.h"

#include <QDomDocument>

#include <QDomElement>

#include <QDomNode>

#include <QFile>

#include "simparams.h"

SireSim::StringValue __copy__(const SireSim::StringValue &other){ return SireSim::StringValue(other); }

const char* pvt_get_name(const SireSim::StringValue&){ return "SireSim::StringValue";}

void register_StringValue_class(){

    { //::SireSim::StringValue
        typedef bp::class_< SireSim::StringValue, bp::bases< SireSim::Value > > StringValue_exposer_t;
        StringValue_exposer_t StringValue_exposer = StringValue_exposer_t( "StringValue", bp::init< >() );
        bp::scope StringValue_scope( StringValue_exposer );
        StringValue_exposer.def( bp::init< QString const & >(( bp::arg("value") )) );
        StringValue_exposer.def( bp::init< QDomElement >(( bp::arg("elem") )) );
        StringValue_exposer.def( bp::init< SireSim::StringValue const & >(( bp::arg("other") )) );
        { //::SireSim::StringValue::getValue
        
            typedef ::SireSim::ValuePtr ( ::SireSim::StringValue::*getValue_function_type )( ::QString ) const;
            getValue_function_type getValue_function_value( &::SireSim::StringValue::getValue );
            
            StringValue_exposer.def( 
                "getValue"
                , getValue_function_value
                , ( bp::arg("key") ) );
        
        }
        StringValue_exposer.def( bp::self != bp::self );
        { //::SireSim::StringValue::operator=
        
            typedef ::SireSim::StringValue & ( ::SireSim::StringValue::*assign_function_type )( ::SireSim::StringValue const & ) ;
            assign_function_type assign_function_value( &::SireSim::StringValue::operator= );
            
            StringValue_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        StringValue_exposer.def( bp::self == bp::self );
        { //::SireSim::StringValue::setValue
        
            typedef ::SireSim::ValuePtr ( ::SireSim::StringValue::*setValue_function_type )( ::QString,::SireSim::Value const & ) const;
            setValue_function_type setValue_function_value( &::SireSim::StringValue::setValue );
            
            StringValue_exposer.def( 
                "setValue"
                , setValue_function_value
                , ( bp::arg("key"), bp::arg("value") ) );
        
        }
        { //::SireSim::StringValue::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireSim::StringValue::typeName );
            
            StringValue_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireSim::StringValue::value
        
            typedef ::QString ( ::SireSim::StringValue::*value_function_type )(  ) const;
            value_function_type value_function_value( &::SireSim::StringValue::value );
            
            StringValue_exposer.def( 
                "value"
                , value_function_value );
        
        }
        { //::SireSim::StringValue::what
        
            typedef char const * ( ::SireSim::StringValue::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireSim::StringValue::what );
            
            StringValue_exposer.def( 
                "what"
                , what_function_value );
        
        }
        StringValue_exposer.staticmethod( "typeName" );
        StringValue_exposer.def( "__copy__", &__copy__);
        StringValue_exposer.def( "__deepcopy__", &__copy__);
        StringValue_exposer.def( "clone", &__copy__);
        StringValue_exposer.def( "__str__", &pvt_get_name);
        StringValue_exposer.def( "__repr__", &pvt_get_name);
    }

}
