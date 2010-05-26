// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "GLCanvas.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "glbackground.h"

#include "glcanvas.h"

#include "glinitstate.h"

#include "glsphere.h"

#include <QDebug>

#include <QGLWidget>

#include "glcanvas.h"

Spier::GLCanvas __copy__(const Spier::GLCanvas &other){ return Spier::GLCanvas(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_GLCanvas_class(){

    { //::Spier::GLCanvas
        typedef bp::class_< Spier::GLCanvas, bp::bases< SireBase::Property > > GLCanvas_exposer_t;
        GLCanvas_exposer_t GLCanvas_exposer = GLCanvas_exposer_t( "GLCanvas" );
        bp::scope GLCanvas_scope( GLCanvas_exposer );
        GLCanvas_exposer.def( bp::init< >() );
        GLCanvas_exposer.def( bp::init< Spier::GLCanvas const & >(( bp::arg("other") )) );
        { //::Spier::GLCanvas::UID
        
            typedef ::QUuid const & ( ::Spier::GLCanvas::*UID_function_type )(  ) const;
            UID_function_type UID_function_value( &::Spier::GLCanvas::UID );
            
            GLCanvas_exposer.def( 
                "UID"
                , UID_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Spier::GLCanvas::null
        
            typedef ::Spier::GLCanvas const & ( *null_function_type )(  );
            null_function_type null_function_value( &::Spier::GLCanvas::null );
            
            GLCanvas_exposer.def( 
                "null"
                , null_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        GLCanvas_exposer.def( bp::self != bp::self );
        { //::Spier::GLCanvas::operator=
        
            typedef ::Spier::GLCanvas & ( ::Spier::GLCanvas::*assign_function_type )( ::Spier::GLCanvas const & ) ;
            assign_function_type assign_function_value( &::Spier::GLCanvas::operator= );
            
            GLCanvas_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        GLCanvas_exposer.def( bp::self == bp::self );
        { //::Spier::GLCanvas::render
        
            typedef void ( ::Spier::GLCanvas::*render_function_type )( ::Spier::GLRenderContext & ) const;
            render_function_type render_function_value( &::Spier::GLCanvas::render );
            
            GLCanvas_exposer.def( 
                "render"
                , render_function_value
                , ( bp::arg("render_context") ) );
        
        }
        { //::Spier::GLCanvas::renderSelector
        
            typedef void ( ::Spier::GLCanvas::*renderSelector_function_type )( ::Spier::GLRenderContext & ) const;
            renderSelector_function_type renderSelector_function_value( &::Spier::GLCanvas::renderSelector );
            
            GLCanvas_exposer.def( 
                "renderSelector"
                , renderSelector_function_value
                , ( bp::arg("render_context") ) );
        
        }
        { //::Spier::GLCanvas::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::Spier::GLCanvas::typeName );
            
            GLCanvas_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::Spier::GLCanvas::version
        
            typedef ::quint64 ( ::Spier::GLCanvas::*version_function_type )(  ) const;
            version_function_type version_function_value( &::Spier::GLCanvas::version );
            
            GLCanvas_exposer.def( 
                "version"
                , version_function_value );
        
        }
        GLCanvas_exposer.staticmethod( "null" );
        GLCanvas_exposer.staticmethod( "typeName" );
        GLCanvas_exposer.def( "__copy__", &__copy__);
        GLCanvas_exposer.def( "__deepcopy__", &__copy__);
        GLCanvas_exposer.def( "clone", &__copy__);
        GLCanvas_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::Spier::GLCanvas >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        GLCanvas_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::Spier::GLCanvas >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        GLCanvas_exposer.def( "__str__", &__str__< ::Spier::GLCanvas > );
        GLCanvas_exposer.def( "__repr__", &__str__< ::Spier::GLCanvas > );
    }

}