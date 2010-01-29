// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "TempDir.pypp.hpp"

namespace bp = boost::python;

#include "Siren/errors.h"

#include "Siren/tostring.h"

#include "tempdir.h"

#include <QDebug>

#include <QMutex>

#include <QUuid>

#include "tempdir.h"

#include "Siren/logger.h"

SireBase::TempDir __copy__(const SireBase::TempDir &other){ return SireBase::TempDir(other); }

#include "Siren/str.hpp"

void register_TempDir_class(){

    { //::SireBase::TempDir
        typedef bp::class_< SireBase::TempDir, bp::bases< Siren::Handle > > TempDir_exposer_t;
        TempDir_exposer_t TempDir_exposer = TempDir_exposer_t( "TempDir" );
        bp::scope TempDir_scope( TempDir_exposer );
        TempDir_exposer.def( bp::init< >() );
        TempDir_exposer.def( bp::init< QString const & >(( bp::arg("temp_root") )) );
        TempDir_exposer.def( bp::init< SireBase::TempDir const & >(( bp::arg("other") )) );
        { //::SireBase::TempDir::doNotDelete
        
            typedef void ( ::SireBase::TempDir::*doNotDelete_function_type )(  ) ;
            doNotDelete_function_type doNotDelete_function_value( &::SireBase::TempDir::doNotDelete );
            
            TempDir_exposer.def( 
                "doNotDelete"
                , doNotDelete_function_value );
        
        }
        { //::SireBase::TempDir::hashCode
        
            typedef ::uint ( ::SireBase::TempDir::*hashCode_function_type )(  ) const;
            hashCode_function_type hashCode_function_value( &::SireBase::TempDir::hashCode );
            
            TempDir_exposer.def( 
                "hashCode"
                , hashCode_function_value );
        
        }
        TempDir_exposer.def( bp::self != bp::self );
        { //::SireBase::TempDir::operator=
        
            typedef ::SireBase::TempDir & ( ::SireBase::TempDir::*assign_function_type )( ::SireBase::TempDir const & ) ;
            assign_function_type assign_function_value( &::SireBase::TempDir::operator= );
            
            TempDir_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        TempDir_exposer.def( bp::self == bp::self );
        { //::SireBase::TempDir::path
        
            typedef ::QString ( ::SireBase::TempDir::*path_function_type )(  ) const;
            path_function_type path_function_value( &::SireBase::TempDir::path );
            
            TempDir_exposer.def( 
                "path"
                , path_function_value );
        
        }
        { //::SireBase::TempDir::toString
        
            typedef ::QString ( ::SireBase::TempDir::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireBase::TempDir::toString );
            
            TempDir_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        TempDir_exposer.def( "__copy__", &__copy__);
        TempDir_exposer.def( "__deepcopy__", &__copy__);
        TempDir_exposer.def( "clone", &__copy__);
        TempDir_exposer.def( "__str__", &__str__< ::SireBase::TempDir > );
        TempDir_exposer.def( "__repr__", &__str__< ::SireBase::TempDir > );
    }

}