// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "NoMangling.pypp.hpp"

namespace bp = boost::python;

#include "Siren/stream.h"

#include "stringmangler.h"

#include "stringmangler.h"

#include "Siren/logger.h"

SireBase::NoMangling __copy__(const SireBase::NoMangling &other){ return SireBase::NoMangling(other); }

#include "Siren/str.hpp"

void register_NoMangling_class(){

    { //::SireBase::NoMangling
        typedef bp::class_< SireBase::NoMangling, bp::bases< SireBase::StringMangler, Siren::Object > > NoMangling_exposer_t;
        NoMangling_exposer_t NoMangling_exposer = NoMangling_exposer_t( "NoMangling" );
        bp::scope NoMangling_scope( NoMangling_exposer );
        NoMangling_exposer.def( bp::init< >() );
        NoMangling_exposer.def( bp::init< SireBase::NoMangling const & >(( bp::arg("other") )) );
        { //::SireBase::NoMangling::hashCode
        
            typedef ::uint ( ::SireBase::NoMangling::*hashCode_function_type )(  ) const;
            hashCode_function_type hashCode_function_value( &::SireBase::NoMangling::hashCode );
            
            NoMangling_exposer.def( 
                "hashCode"
                , hashCode_function_value );
        
        }
        { //::SireBase::NoMangling::mangle
        
            typedef ::QString ( ::SireBase::NoMangling::*mangle_function_type )( ::QString const & ) const;
            mangle_function_type mangle_function_value( &::SireBase::NoMangling::mangle );
            
            NoMangling_exposer.def( 
                "mangle"
                , mangle_function_value
                , ( bp::arg("input") ) );
        
        }
        NoMangling_exposer.def( bp::self != bp::self );
        { //::SireBase::NoMangling::operator=
        
            typedef ::SireBase::NoMangling & ( ::SireBase::NoMangling::*assign_function_type )( ::SireBase::NoMangling const & ) ;
            assign_function_type assign_function_value( &::SireBase::NoMangling::operator= );
            
            NoMangling_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        NoMangling_exposer.def( bp::self == bp::self );
        { //::SireBase::NoMangling::stream
        
            typedef void ( ::SireBase::NoMangling::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireBase::NoMangling::stream );
            
            NoMangling_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::SireBase::NoMangling::toString
        
            typedef ::QString ( ::SireBase::NoMangling::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireBase::NoMangling::toString );
            
            NoMangling_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        NoMangling_exposer.def( "__copy__", &__copy__);
        NoMangling_exposer.def( "__deepcopy__", &__copy__);
        NoMangling_exposer.def( "clone", &__copy__);
        NoMangling_exposer.def( "__str__", &__str__< ::SireBase::NoMangling > );
        NoMangling_exposer.def( "__repr__", &__str__< ::SireBase::NoMangling > );
    }

}
