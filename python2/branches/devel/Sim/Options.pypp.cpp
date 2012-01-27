// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Options.pypp.hpp"

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

SireSim::Options __copy__(const SireSim::Options &other){ return SireSim::Options(other); }

const char* pvt_get_name(const SireSim::Options&){ return "SireSim::Options";}

void register_Options_class(){

    { //::SireSim::Options
        typedef bp::class_< SireSim::Options, bp::bases< SireSim::Value > > Options_exposer_t;
        Options_exposer_t Options_exposer = Options_exposer_t( "Options", bp::init< >() );
        bp::scope Options_scope( Options_exposer );
        Options_exposer.def( bp::init< QString const & >(( bp::arg("xmlfile") )) );
        Options_exposer.def( bp::init< QDomElement >(( bp::arg("elem") )) );
        Options_exposer.def( bp::init< QList< SireSim::Option > const &, bp::optional< bool > >(( bp::arg("options"), bp::arg("mutually_exclusive")=(bool)(false) )) );
        Options_exposer.def( bp::init< SireSim::Options const & >(( bp::arg("other") )) );
        { //::SireSim::Options::add
        
            typedef ::SireSim::Options ( ::SireSim::Options::*add_function_type )( ::SireSim::Options const & ) const;
            add_function_type add_function_value( &::SireSim::Options::add );
            
            Options_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireSim::Options::clear
        
            typedef ::SireSim::ValuePtr ( ::SireSim::Options::*clear_function_type )(  ) const;
            clear_function_type clear_function_value( &::SireSim::Options::clear );
            
            Options_exposer.def( 
                "clear"
                , clear_function_value );
        
        }
        { //::SireSim::Options::fromConfig
        
            typedef ::SireSim::Options ( ::SireSim::Options::*fromConfig_function_type )( ::QString const & ) const;
            fromConfig_function_type fromConfig_function_value( &::SireSim::Options::fromConfig );
            
            Options_exposer.def( 
                "fromConfig"
                , fromConfig_function_value
                , ( bp::arg("text") ) );
        
        }
        { //::SireSim::Options::fromXML
        
            typedef ::SireSim::Options ( *fromXML_function_type )( ::QString const & );
            fromXML_function_type fromXML_function_value( &::SireSim::Options::fromXML );
            
            Options_exposer.def( 
                "fromXML"
                , fromXML_function_value
                , ( bp::arg("xmlfile") ) );
        
        }
        { //::SireSim::Options::fromXMLConfig
        
            typedef ::SireSim::Options ( *fromXMLConfig_function_type )( ::QString const &,::QString const & );
            fromXMLConfig_function_type fromXMLConfig_function_value( &::SireSim::Options::fromXMLConfig );
            
            Options_exposer.def( 
                "fromXMLConfig"
                , fromXMLConfig_function_value
                , ( bp::arg("xmlfile"), bp::arg("config") ) );
        
        }
        { //::SireSim::Options::getValue
        
            typedef ::SireSim::ValuePtr ( ::SireSim::Options::*getValue_function_type )( ::QString ) const;
            getValue_function_type getValue_function_value( &::SireSim::Options::getValue );
            
            Options_exposer.def( 
                "getValue"
                , getValue_function_value
                , ( bp::arg("key") ) );
        
        }
        Options_exposer.def( bp::self != bp::self );
        Options_exposer.def( bp::self + bp::self );
        { //::SireSim::Options::operator=
        
            typedef ::SireSim::Options & ( ::SireSim::Options::*assign_function_type )( ::SireSim::Options const & ) ;
            assign_function_type assign_function_value( &::SireSim::Options::operator= );
            
            Options_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Options_exposer.def( bp::self == bp::self );
        { //::SireSim::Options::options
        
            typedef ::QList< SireSim::Option > ( ::SireSim::Options::*options_function_type )(  ) const;
            options_function_type options_function_value( &::SireSim::Options::options );
            
            Options_exposer.def( 
                "options"
                , options_function_value );
        
        }
        { //::SireSim::Options::setValue
        
            typedef ::SireSim::ValuePtr ( ::SireSim::Options::*setValue_function_type )( ::QString,::SireSim::Value const & ) const;
            setValue_function_type setValue_function_value( &::SireSim::Options::setValue );
            
            Options_exposer.def( 
                "setValue"
                , setValue_function_value
                , ( bp::arg("key"), bp::arg("value") ) );
        
        }
        { //::SireSim::Options::toXML
        
            typedef ::QString ( ::SireSim::Options::*toXML_function_type )(  ) const;
            toXML_function_type toXML_function_value( &::SireSim::Options::toXML );
            
            Options_exposer.def( 
                "toXML"
                , toXML_function_value );
        
        }
        { //::SireSim::Options::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireSim::Options::typeName );
            
            Options_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireSim::Options::what
        
            typedef char const * ( ::SireSim::Options::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireSim::Options::what );
            
            Options_exposer.def( 
                "what"
                , what_function_value );
        
        }
        Options_exposer.staticmethod( "fromXML" );
        Options_exposer.staticmethod( "fromXMLConfig" );
        Options_exposer.staticmethod( "typeName" );
        Options_exposer.def( "__copy__", &__copy__);
        Options_exposer.def( "__deepcopy__", &__copy__);
        Options_exposer.def( "clone", &__copy__);
        Options_exposer.def( "__str__", &pvt_get_name);
        Options_exposer.def( "__repr__", &pvt_get_name);
    }

}
