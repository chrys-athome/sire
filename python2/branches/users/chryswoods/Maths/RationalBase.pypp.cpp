// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "RationalBase.pypp.hpp"

namespace bp = boost::python;

#include "SireMaths/errors.h"

#include "Siren/tostring.h"

#include "maths.h"

#include "rational.h"

#include "rational.h"

#include "Siren/logger.h"

Siren::Primitive<SireMaths::Rational> __copy__(const Siren::Primitive<SireMaths::Rational> &other){ return Siren::Primitive<SireMaths::Rational>(other); }

const char* pvt_get_name(const Siren::Primitive<SireMaths::Rational>&){ return "Siren::Primitive<SireMaths::Rational>";}

void register_RationalBase_class(){

    { //::Siren::Primitive< SireMaths::Rational >
        typedef bp::class_< Siren::Primitive< SireMaths::Rational > > RationalBase_exposer_t;
        RationalBase_exposer_t RationalBase_exposer = RationalBase_exposer_t( "RationalBase" );
        bp::scope RationalBase_scope( RationalBase_exposer );
        RationalBase_exposer.def( bp::init< >() );
        { //::Siren::Primitive< SireMaths::Rational >::getClass
        
            typedef Siren::Primitive< SireMaths::Rational > exported_class_t;
            typedef ::Siren::Class ( ::Siren::Primitive< SireMaths::Rational >::*getClass_function_type )(  ) const;
            getClass_function_type getClass_function_value( &::Siren::Primitive< SireMaths::Rational >::getClass );
            
            RationalBase_exposer.def( 
                "getClass"
                , getClass_function_value );
        
        }
        { //::Siren::Primitive< SireMaths::Rational >::hashCode
        
            typedef Siren::Primitive< SireMaths::Rational > exported_class_t;
            typedef ::uint ( ::Siren::Primitive< SireMaths::Rational >::*hashCode_function_type )(  ) const;
            hashCode_function_type hashCode_function_value( &::Siren::Primitive< SireMaths::Rational >::hashCode );
            
            RationalBase_exposer.def( 
                "hashCode"
                , hashCode_function_value );
        
        }
        { //::Siren::Primitive< SireMaths::Rational >::load
        
            typedef Siren::Primitive< SireMaths::Rational > exported_class_t;
            typedef void ( ::Siren::Primitive< SireMaths::Rational >::*load_function_type )( ::Siren::Stream & ) ;
            load_function_type load_function_value( &::Siren::Primitive< SireMaths::Rational >::load );
            
            RationalBase_exposer.def( 
                "load"
                , load_function_value
                , ( bp::arg("s") ) );
        
        }
        RationalBase_exposer.def( bp::self != bp::other< SireMaths::Rational >() );
        { //::Siren::Primitive< SireMaths::Rational >::save
        
            typedef Siren::Primitive< SireMaths::Rational > exported_class_t;
            typedef void ( ::Siren::Primitive< SireMaths::Rational >::*save_function_type )( ::Siren::Stream & ) const;
            save_function_type save_function_value( &::Siren::Primitive< SireMaths::Rational >::save );
            
            RationalBase_exposer.def( 
                "save"
                , save_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::Siren::Primitive< SireMaths::Rational >::stream
        
            typedef Siren::Primitive< SireMaths::Rational > exported_class_t;
            typedef void ( ::Siren::Primitive< SireMaths::Rational >::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::Siren::Primitive< SireMaths::Rational >::stream );
            
            RationalBase_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::Siren::Primitive< SireMaths::Rational >::test
        
            typedef Siren::Primitive< SireMaths::Rational > exported_class_t;
            typedef bool ( ::Siren::Primitive< SireMaths::Rational >::*test_function_type )(  ) const;
            test_function_type test_function_value( &::Siren::Primitive< SireMaths::Rational >::test );
            
            RationalBase_exposer.def( 
                "test"
                , test_function_value );
        
        }
        { //::Siren::Primitive< SireMaths::Rational >::test
        
            typedef Siren::Primitive< SireMaths::Rational > exported_class_t;
            typedef bool ( ::Siren::Primitive< SireMaths::Rational >::*test_function_type )( ::Siren::Logger & ) const;
            test_function_type test_function_value( &::Siren::Primitive< SireMaths::Rational >::test );
            
            RationalBase_exposer.def( 
                "test"
                , test_function_value
                , ( bp::arg("logger") ) );
        
        }
        { //::Siren::Primitive< SireMaths::Rational >::typeName
        
            typedef Siren::Primitive< SireMaths::Rational > exported_class_t;
            typedef ::QString ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::Siren::Primitive< SireMaths::Rational >::typeName );
            
            RationalBase_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::Siren::Primitive< SireMaths::Rational >::what
        
            typedef Siren::Primitive< SireMaths::Rational > exported_class_t;
            typedef ::QString ( ::Siren::Primitive< SireMaths::Rational >::*what_function_type )(  ) const;
            what_function_type what_function_value( &::Siren::Primitive< SireMaths::Rational >::what );
            
            RationalBase_exposer.def( 
                "what"
                , what_function_value );
        
        }
        RationalBase_exposer.staticmethod( "typeName" );
        RationalBase_exposer.def( "__copy__", &__copy__);
        RationalBase_exposer.def( "__deepcopy__", &__copy__);
        RationalBase_exposer.def( "clone", &__copy__);
        RationalBase_exposer.def( "__str__", &pvt_get_name);
        RationalBase_exposer.def( "__repr__", &pvt_get_name);
    }

}