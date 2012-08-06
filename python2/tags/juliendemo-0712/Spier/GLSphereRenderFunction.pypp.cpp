// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "GLSphereRenderFunction.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "glrendercontext.h"

#include "glsphere.h"

#include <QGLWidget>

#include "glsphere.h"

Spier::GLSphereRenderFunction __copy__(const Spier::GLSphereRenderFunction &other){ return Spier::GLSphereRenderFunction(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_GLSphereRenderFunction_class(){

    { //::Spier::GLSphereRenderFunction
        typedef bp::class_< Spier::GLSphereRenderFunction, bp::bases< Spier::GLRenderFunction, SireBase::Property > > GLSphereRenderFunction_exposer_t;
        GLSphereRenderFunction_exposer_t GLSphereRenderFunction_exposer = GLSphereRenderFunction_exposer_t( "GLSphereRenderFunction" );
        bp::scope GLSphereRenderFunction_scope( GLSphereRenderFunction_exposer );
        GLSphereRenderFunction_exposer.def( bp::init< >() );
        GLSphereRenderFunction_exposer.def( bp::init< int >(( bp::arg("resolution") )) );
        GLSphereRenderFunction_exposer.def( bp::init< Spier::GLSphereRenderFunction const & >(( bp::arg("other") )) );
        GLSphereRenderFunction_exposer.def( bp::self != bp::self );
        { //::Spier::GLSphereRenderFunction::operator()
        
            typedef void ( ::Spier::GLSphereRenderFunction::*__call___function_type )(  ) const;
            __call___function_type __call___function_value( &::Spier::GLSphereRenderFunction::operator() );
            
            GLSphereRenderFunction_exposer.def( 
                "__call__"
                , __call___function_value );
        
        }
        { //::Spier::GLSphereRenderFunction::operator=
        
            typedef ::Spier::GLSphereRenderFunction & ( ::Spier::GLSphereRenderFunction::*assign_function_type )( ::Spier::GLSphereRenderFunction const & ) ;
            assign_function_type assign_function_value( &::Spier::GLSphereRenderFunction::operator= );
            
            GLSphereRenderFunction_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        GLSphereRenderFunction_exposer.def( bp::self == bp::self );
        { //::Spier::GLSphereRenderFunction::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::Spier::GLSphereRenderFunction::typeName );
            
            GLSphereRenderFunction_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        GLSphereRenderFunction_exposer.staticmethod( "typeName" );
        GLSphereRenderFunction_exposer.def( "__copy__", &__copy__);
        GLSphereRenderFunction_exposer.def( "__deepcopy__", &__copy__);
        GLSphereRenderFunction_exposer.def( "clone", &__copy__);
        GLSphereRenderFunction_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::Spier::GLSphereRenderFunction >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        GLSphereRenderFunction_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::Spier::GLSphereRenderFunction >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        GLSphereRenderFunction_exposer.def( "__str__", &__str__< ::Spier::GLSphereRenderFunction > );
        GLSphereRenderFunction_exposer.def( "__repr__", &__str__< ::Spier::GLSphereRenderFunction > );
    }

}