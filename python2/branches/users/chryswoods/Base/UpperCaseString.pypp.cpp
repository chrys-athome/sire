// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "UpperCaseString.pypp.hpp"

namespace bp = boost::python;

#include "Siren/stream.h"

#include "stringmangler.h"

#include "stringmangler.h"

#include "Siren/logger.h"

SireBase::UpperCaseString __copy__(const SireBase::UpperCaseString &other){ return SireBase::UpperCaseString(other); }

#include "Siren/str.hpp"

void register_UpperCaseString_class(){

    { //::SireBase::UpperCaseString
        typedef bp::class_< SireBase::UpperCaseString, bp::bases< SireBase::StringMangler, Siren::Object > > UpperCaseString_exposer_t;
        UpperCaseString_exposer_t UpperCaseString_exposer = UpperCaseString_exposer_t( "UpperCaseString" );
        bp::scope UpperCaseString_scope( UpperCaseString_exposer );
        UpperCaseString_exposer.def( bp::init< >() );
        UpperCaseString_exposer.def( bp::init< SireBase::UpperCaseString const & >(( bp::arg("other") )) );
        { //::SireBase::UpperCaseString::hashCode
        
            typedef ::uint ( ::SireBase::UpperCaseString::*hashCode_function_type )(  ) const;
            hashCode_function_type hashCode_function_value( &::SireBase::UpperCaseString::hashCode );
            
            UpperCaseString_exposer.def( 
                "hashCode"
                , hashCode_function_value );
        
        }
        { //::SireBase::UpperCaseString::mangle
        
            typedef ::QString ( ::SireBase::UpperCaseString::*mangle_function_type )( ::QString const & ) const;
            mangle_function_type mangle_function_value( &::SireBase::UpperCaseString::mangle );
            
            UpperCaseString_exposer.def( 
                "mangle"
                , mangle_function_value
                , ( bp::arg("input") ) );
        
        }
        UpperCaseString_exposer.def( bp::self != bp::self );
        { //::SireBase::UpperCaseString::operator=
        
            typedef ::SireBase::UpperCaseString & ( ::SireBase::UpperCaseString::*assign_function_type )( ::SireBase::UpperCaseString const & ) ;
            assign_function_type assign_function_value( &::SireBase::UpperCaseString::operator= );
            
            UpperCaseString_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        UpperCaseString_exposer.def( bp::self == bp::self );
        { //::SireBase::UpperCaseString::stream
        
            typedef void ( ::SireBase::UpperCaseString::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireBase::UpperCaseString::stream );
            
            UpperCaseString_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::SireBase::UpperCaseString::toString
        
            typedef ::QString ( ::SireBase::UpperCaseString::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireBase::UpperCaseString::toString );
            
            UpperCaseString_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        UpperCaseString_exposer.def( "__copy__", &__copy__);
        UpperCaseString_exposer.def( "__deepcopy__", &__copy__);
        UpperCaseString_exposer.def( "clone", &__copy__);
        UpperCaseString_exposer.def( "__str__", &__str__< ::SireBase::UpperCaseString > );
        UpperCaseString_exposer.def( "__repr__", &__str__< ::SireBase::UpperCaseString > );
    }

}
