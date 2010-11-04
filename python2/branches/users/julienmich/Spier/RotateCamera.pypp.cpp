// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "RotateCamera.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "SireUnits/units.h"

#include "camera.h"

#include "cameracommand.h"

#include "renderview.h"

#include "cameracommand.h"

Spier::RotateCamera __copy__(const Spier::RotateCamera &other){ return Spier::RotateCamera(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_RotateCamera_class(){

    { //::Spier::RotateCamera
        typedef bp::class_< Spier::RotateCamera, bp::bases< Spier::CameraCommand, Spier::RenderViewCommand, Spier::Command, SireBase::Property > > RotateCamera_exposer_t;
        RotateCamera_exposer_t RotateCamera_exposer = RotateCamera_exposer_t( "RotateCamera" );
        bp::scope RotateCamera_scope( RotateCamera_exposer );
        RotateCamera_exposer.def( bp::init< >() );
        RotateCamera_exposer.def( bp::init< SireUnits::Dimension::Angle const &, SireUnits::Dimension::Angle const &, SireUnits::Dimension::Angle const & >(( bp::arg("rotate_x"), bp::arg("rotate_y"), bp::arg("rotate_z") )) );
        RotateCamera_exposer.def( bp::init< Spier::RotateCamera const & >(( bp::arg("other") )) );
        { //::Spier::RotateCamera::mergeWith
        
            typedef ::Spier::CommandPtr ( ::Spier::RotateCamera::*mergeWith_function_type )( ::Spier::Command const & ) const;
            mergeWith_function_type mergeWith_function_value( &::Spier::RotateCamera::mergeWith );
            
            RotateCamera_exposer.def( 
                "mergeWith"
                , mergeWith_function_value
                , ( bp::arg("other") ) );
        
        }
        RotateCamera_exposer.def( bp::self != bp::self );
        { //::Spier::RotateCamera::operator=
        
            typedef ::Spier::RotateCamera & ( ::Spier::RotateCamera::*assign_function_type )( ::Spier::RotateCamera const & ) ;
            assign_function_type assign_function_value( &::Spier::RotateCamera::operator= );
            
            RotateCamera_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        RotateCamera_exposer.def( bp::self == bp::self );
        { //::Spier::RotateCamera::redo
        
            typedef void ( ::Spier::RotateCamera::*redo_function_type )(  ) const;
            redo_function_type redo_function_value( &::Spier::RotateCamera::redo );
            
            RotateCamera_exposer.def( 
                "redo"
                , redo_function_value );
        
        }
        { //::Spier::RotateCamera::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::Spier::RotateCamera::typeName );
            
            RotateCamera_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::Spier::RotateCamera::undo
        
            typedef void ( ::Spier::RotateCamera::*undo_function_type )(  ) const;
            undo_function_type undo_function_value( &::Spier::RotateCamera::undo );
            
            RotateCamera_exposer.def( 
                "undo"
                , undo_function_value );
        
        }
        RotateCamera_exposer.staticmethod( "typeName" );
        RotateCamera_exposer.def( "__copy__", &__copy__);
        RotateCamera_exposer.def( "__deepcopy__", &__copy__);
        RotateCamera_exposer.def( "clone", &__copy__);
        RotateCamera_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::Spier::RotateCamera >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RotateCamera_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::Spier::RotateCamera >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RotateCamera_exposer.def( "__str__", &__str__< ::Spier::RotateCamera > );
        RotateCamera_exposer.def( "__repr__", &__str__< ::Spier::RotateCamera > );
    }

}
