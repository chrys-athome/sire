// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "NullGLMesh.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "glmesh.h"

#include "glrendercontext.h"

#include "glmesh.h"

Spier::NullGLMesh __copy__(const Spier::NullGLMesh &other){ return Spier::NullGLMesh(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_NullGLMesh_class(){

    { //::Spier::NullGLMesh
        typedef bp::class_< Spier::NullGLMesh, bp::bases< Spier::GLMesh, SireBase::Property > > NullGLMesh_exposer_t;
        NullGLMesh_exposer_t NullGLMesh_exposer = NullGLMesh_exposer_t( "NullGLMesh", bp::init< >() );
        bp::scope NullGLMesh_scope( NullGLMesh_exposer );
        NullGLMesh_exposer.def( bp::init< Spier::NullGLMesh const & >(( bp::arg("other") )) );
        NullGLMesh_exposer.def( bp::self != bp::self );
        { //::Spier::NullGLMesh::operator=
        
            typedef ::Spier::NullGLMesh & ( ::Spier::NullGLMesh::*assign_function_type )( ::Spier::NullGLMesh const & ) ;
            assign_function_type assign_function_value( &::Spier::NullGLMesh::operator= );
            
            NullGLMesh_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        NullGLMesh_exposer.def( bp::self == bp::self );
        { //::Spier::NullGLMesh::render
        
            typedef void ( ::Spier::NullGLMesh::*render_function_type )( ::Spier::GLRenderContext &,float ) const;
            render_function_type render_function_value( &::Spier::NullGLMesh::render );
            
            NullGLMesh_exposer.def( 
                "render"
                , render_function_value
                , ( bp::arg("render_context"), bp::arg("distance_from_camera") ) );
        
        }
        { //::Spier::NullGLMesh::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::Spier::NullGLMesh::typeName );
            
            NullGLMesh_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        NullGLMesh_exposer.staticmethod( "typeName" );
        NullGLMesh_exposer.def( "__copy__", &__copy__);
        NullGLMesh_exposer.def( "__deepcopy__", &__copy__);
        NullGLMesh_exposer.def( "clone", &__copy__);
        NullGLMesh_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::Spier::NullGLMesh >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullGLMesh_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::Spier::NullGLMesh >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullGLMesh_exposer.def( "__str__", &__str__< ::Spier::NullGLMesh > );
        NullGLMesh_exposer.def( "__repr__", &__str__< ::Spier::NullGLMesh > );
    }

}
