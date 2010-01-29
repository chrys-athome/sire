// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "SphereObject.pypp.hpp"

namespace bp = boost::python;

#include "Siren/stream.h"

#include "sphere.h"

#include "sphere.h"

#include "Siren/logger.h"

Siren::PrimitiveObject<SireMaths::Sphere> __copy__(const Siren::PrimitiveObject<SireMaths::Sphere> &other){ return Siren::PrimitiveObject<SireMaths::Sphere>(other); }

#include "Siren/str.hpp"

void register_SphereObject_class(){

    { //::Siren::PrimitiveObject< SireMaths::Sphere >
        typedef bp::class_< Siren::PrimitiveObject< SireMaths::Sphere >, bp::bases< Siren::Object > > SphereObject_exposer_t;
        SphereObject_exposer_t SphereObject_exposer = SphereObject_exposer_t( "SphereObject" );
        bp::scope SphereObject_scope( SphereObject_exposer );
        SphereObject_exposer.def( bp::init< >() );
        SphereObject_exposer.def( bp::init< SireMaths::Sphere const & >(( bp::arg("primitive") )) );
        SphereObject_exposer.def( bp::init< Siren::PrimitiveObject< SireMaths::Sphere > const & >(( bp::arg("other") )) );
        { //::Siren::PrimitiveObject< SireMaths::Sphere >::hashCode
        
            typedef Siren::PrimitiveObject< SireMaths::Sphere > exported_class_t;
            typedef ::uint ( ::Siren::PrimitiveObject< SireMaths::Sphere >::*hashCode_function_type )(  ) const;
            hashCode_function_type hashCode_function_value( &::Siren::PrimitiveObject< SireMaths::Sphere >::hashCode );
            
            SphereObject_exposer.def( 
                "hashCode"
                , hashCode_function_value );
        
        }
        SphereObject_exposer.def( bp::self != bp::self );
        SphereObject_exposer.def( bp::self != bp::other< SireMaths::Sphere >() );
        { //::Siren::PrimitiveObject< SireMaths::Sphere >::operator=
        
            typedef Siren::PrimitiveObject< SireMaths::Sphere > exported_class_t;
            typedef ::Siren::PrimitiveObject< SireMaths::Sphere > & ( ::Siren::PrimitiveObject< SireMaths::Sphere >::*assign_function_type )( ::Siren::PrimitiveObject< SireMaths::Sphere > const & ) ;
            assign_function_type assign_function_value( &::Siren::PrimitiveObject< SireMaths::Sphere >::operator= );
            
            SphereObject_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::Siren::PrimitiveObject< SireMaths::Sphere >::operator=
        
            typedef Siren::PrimitiveObject< SireMaths::Sphere > exported_class_t;
            typedef ::Siren::PrimitiveObject< SireMaths::Sphere > & ( ::Siren::PrimitiveObject< SireMaths::Sphere >::*assign_function_type )( ::SireMaths::Sphere const & ) ;
            assign_function_type assign_function_value( &::Siren::PrimitiveObject< SireMaths::Sphere >::operator= );
            
            SphereObject_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("primitive") )
                , bp::return_self< >() );
        
        }
        SphereObject_exposer.def( bp::self == bp::self );
        SphereObject_exposer.def( bp::self == bp::other< SireMaths::Sphere >() );
        { //::Siren::PrimitiveObject< SireMaths::Sphere >::stream
        
            typedef Siren::PrimitiveObject< SireMaths::Sphere > exported_class_t;
            typedef void ( ::Siren::PrimitiveObject< SireMaths::Sphere >::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::Siren::PrimitiveObject< SireMaths::Sphere >::stream );
            
            SphereObject_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::Siren::PrimitiveObject< SireMaths::Sphere >::test
        
            typedef Siren::PrimitiveObject< SireMaths::Sphere > exported_class_t;
            typedef bool ( ::Siren::PrimitiveObject< SireMaths::Sphere >::*test_function_type )( ::Siren::Logger & ) const;
            test_function_type test_function_value( &::Siren::PrimitiveObject< SireMaths::Sphere >::test );
            
            SphereObject_exposer.def( 
                "test"
                , test_function_value
                , ( bp::arg("logger") ) );
        
        }
        { //::Siren::PrimitiveObject< SireMaths::Sphere >::toString
        
            typedef Siren::PrimitiveObject< SireMaths::Sphere > exported_class_t;
            typedef ::QString ( ::Siren::PrimitiveObject< SireMaths::Sphere >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::Siren::PrimitiveObject< SireMaths::Sphere >::toString );
            
            SphereObject_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        SphereObject_exposer.def( "__copy__", &__copy__);
        SphereObject_exposer.def( "__deepcopy__", &__copy__);
        SphereObject_exposer.def( "clone", &__copy__);
        SphereObject_exposer.def( "__str__", &__str__< ::Siren::PrimitiveObject<SireMaths::Sphere> > );
        SphereObject_exposer.def( "__repr__", &__str__< ::Siren::PrimitiveObject<SireMaths::Sphere> > );
    }

}