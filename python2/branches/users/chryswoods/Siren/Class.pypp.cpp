// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Class.pypp.hpp"

namespace bp = boost::python;

#include "Siren/errors.h"

#include "class.h"

#include "hanref.h"

#include "logger.h"

#include "objref.h"

#include "stream.h"

#include "tester.h"

#include <QDebug>

#include "class.h"

#include "Siren/logger.h"

Siren::Class __copy__(const Siren::Class &other){ return Siren::Class(other); }

#include "Siren/str.hpp"

void register_Class_class(){

    { //::Siren::Class
        typedef bp::class_< Siren::Class, bp::bases< Siren::Object > > Class_exposer_t;
        Class_exposer_t Class_exposer = Class_exposer_t( "Class" );
        bp::scope Class_scope( Class_exposer );
        Class_exposer.def( bp::init< >() );
        Class_exposer.def( bp::init< Siren::Object const & >(( bp::arg("object") )) );
        Class_exposer.def( bp::init< QString const & >(( bp::arg("type_name") )) );
        Class_exposer.def( bp::init< Siren::Class const & >(( bp::arg("other") )) );
        { //::Siren::Class::assertCanCast
        
            typedef void ( ::Siren::Class::*assertCanCast_function_type )( ::QString const & ) const;
            assertCanCast_function_type assertCanCast_function_value( &::Siren::Class::assertCanCast );
            
            Class_exposer.def( 
                "assertCanCast"
                , assertCanCast_function_value
                , ( bp::arg("classname") ) );
        
        }
        { //::Siren::Class::canCast
        
            typedef bool ( ::Siren::Class::*canCast_function_type )( ::QString const & ) const;
            canCast_function_type canCast_function_value( &::Siren::Class::canCast );
            
            Class_exposer.def( 
                "canCast"
                , canCast_function_value
                , ( bp::arg("classname") ) );
        
        }
        { //::Siren::Class::canCreate
        
            typedef bool ( ::Siren::Class::*canCreate_function_type )(  ) const;
            canCreate_function_type canCreate_function_value( &::Siren::Class::canCreate );
            
            Class_exposer.def( 
                "canCreate"
                , canCreate_function_value );
        
        }
        { //::Siren::Class::createHandle
        
            typedef ::Siren::HanRef ( ::Siren::Class::*createHandle_function_type )(  ) const;
            createHandle_function_type createHandle_function_value( &::Siren::Class::createHandle );
            
            Class_exposer.def( 
                "createHandle"
                , createHandle_function_value );
        
        }
        { //::Siren::Class::createObject
        
            typedef ::Siren::ObjRef ( ::Siren::Class::*createObject_function_type )(  ) const;
            createObject_function_type createObject_function_value( &::Siren::Class::createObject );
            
            Class_exposer.def( 
                "createObject"
                , createObject_function_value );
        
        }
        { //::Siren::Class::hasSuper
        
            typedef bool ( ::Siren::Class::*hasSuper_function_type )(  ) const;
            hasSuper_function_type hasSuper_function_value( &::Siren::Class::hasSuper );
            
            Class_exposer.def( 
                "hasSuper"
                , hasSuper_function_value );
        
        }
        { //::Siren::Class::hashCode
        
            typedef ::uint ( ::Siren::Class::*hashCode_function_type )(  ) const;
            hashCode_function_type hashCode_function_value( &::Siren::Class::hashCode );
            
            Class_exposer.def( 
                "hashCode"
                , hashCode_function_value );
        
        }
        { //::Siren::Class::implements
        
            typedef bool ( ::Siren::Class::*implements_function_type )( ::QString const & ) const;
            implements_function_type implements_function_value( &::Siren::Class::implements );
            
            Class_exposer.def( 
                "implements"
                , implements_function_value
                , ( bp::arg("type_name") ) );
        
        }
        { //::Siren::Class::interfaces
        
            typedef ::QStringList ( ::Siren::Class::*interfaces_function_type )(  ) const;
            interfaces_function_type interfaces_function_value( &::Siren::Class::interfaces );
            
            Class_exposer.def( 
                "interfaces"
                , interfaces_function_value );
        
        }
        { //::Siren::Class::isConcrete
        
            typedef bool ( ::Siren::Class::*isConcrete_function_type )(  ) const;
            isConcrete_function_type isConcrete_function_value( &::Siren::Class::isConcrete );
            
            Class_exposer.def( 
                "isConcrete"
                , isConcrete_function_value );
        
        }
        { //::Siren::Class::isHandle
        
            typedef bool ( ::Siren::Class::*isHandle_function_type )(  ) const;
            isHandle_function_type isHandle_function_value( &::Siren::Class::isHandle );
            
            Class_exposer.def( 
                "isHandle"
                , isHandle_function_value );
        
        }
        { //::Siren::Class::isObject
        
            typedef bool ( ::Siren::Class::*isObject_function_type )(  ) const;
            isObject_function_type isObject_function_value( &::Siren::Class::isObject );
            
            Class_exposer.def( 
                "isObject"
                , isObject_function_value );
        
        }
        { //::Siren::Class::isPrimitive
        
            typedef bool ( ::Siren::Class::*isPrimitive_function_type )(  ) const;
            isPrimitive_function_type isPrimitive_function_value( &::Siren::Class::isPrimitive );
            
            Class_exposer.def( 
                "isPrimitive"
                , isPrimitive_function_value );
        
        }
        { //::Siren::Class::name
        
            typedef ::QString ( ::Siren::Class::*name_function_type )(  ) const;
            name_function_type name_function_value( &::Siren::Class::name );
            
            Class_exposer.def( 
                "name"
                , name_function_value );
        
        }
        Class_exposer.def( bp::self != bp::self );
        { //::Siren::Class::operator=
        
            typedef ::Siren::Class & ( ::Siren::Class::*assign_function_type )( ::Siren::Class const & ) ;
            assign_function_type assign_function_value( &::Siren::Class::operator= );
            
            Class_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Class_exposer.def( bp::self == bp::self );
        { //::Siren::Class::registeredTypes
        
            typedef ::QStringList ( *registeredTypes_function_type )(  );
            registeredTypes_function_type registeredTypes_function_value( &::Siren::Class::registeredTypes );
            
            Class_exposer.def( 
                "registeredTypes"
                , registeredTypes_function_value );
        
        }
        { //::Siren::Class::super
        
            typedef ::Siren::Class const & ( ::Siren::Class::*super_function_type )(  ) const;
            super_function_type super_function_value( &::Siren::Class::super );
            
            Class_exposer.def( 
                "super"
                , super_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Siren::Class::toString
        
            typedef ::QString ( ::Siren::Class::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::Siren::Class::toString );
            
            Class_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        Class_exposer.staticmethod( "registeredTypes" );
        Class_exposer.def( "__copy__", &__copy__);
        Class_exposer.def( "__deepcopy__", &__copy__);
        Class_exposer.def( "clone", &__copy__);
        Class_exposer.def( "__str__", &__str__< ::Siren::Class > );
        Class_exposer.def( "__repr__", &__str__< ::Siren::Class > );
    }

}
