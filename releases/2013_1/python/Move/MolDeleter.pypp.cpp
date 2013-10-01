// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "MolDeleter.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/molecule.h"

#include "SireMol/moleculegroup.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireSystem/system.h"

#include "moldeleter.h"

#include "uniformsampler.h"

#include "moldeleter.h"

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_MolDeleter_class(){

    { //::SireMove::MolDeleter
        typedef bp::class_< SireMove::MolDeleter, bp::bases< SireBase::Property >, boost::noncopyable > MolDeleter_exposer_t;
        MolDeleter_exposer_t MolDeleter_exposer = MolDeleter_exposer_t( "MolDeleter", bp::no_init );
        bp::scope MolDeleter_scope( MolDeleter_exposer );
        { //::SireMove::MolDeleter::deleteFrom
        
            typedef ::boost::tuples::tuple< SireMol::Molecule, double, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMove::MolDeleter::*deleteFrom_function_type )( ::SireSystem::System & ) ;
            deleteFrom_function_type deleteFrom_function_value( &::SireMove::MolDeleter::deleteFrom );
            
            MolDeleter_exposer.def( 
                "deleteFrom"
                , deleteFrom_function_value
                , ( bp::arg("system") ) );
        
        }
        { //::SireMove::MolDeleter::generator
        
            typedef ::SireMaths::RanGenerator const & ( ::SireMove::MolDeleter::*generator_function_type )(  ) const;
            generator_function_type generator_function_value( &::SireMove::MolDeleter::generator );
            
            MolDeleter_exposer.def( 
                "generator"
                , generator_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMove::MolDeleter::null
        
            typedef ::SireMove::NullDeleter const & ( *null_function_type )(  );
            null_function_type null_function_value( &::SireMove::MolDeleter::null );
            
            MolDeleter_exposer.def( 
                "null"
                , null_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMove::MolDeleter::setGenerator
        
            typedef void ( ::SireMove::MolDeleter::*setGenerator_function_type )( ::SireMaths::RanGenerator const & ) ;
            setGenerator_function_type setGenerator_function_value( &::SireMove::MolDeleter::setGenerator );
            
            MolDeleter_exposer.def( 
                "setGenerator"
                , setGenerator_function_value
                , ( bp::arg("generator") ) );
        
        }
        { //::SireMove::MolDeleter::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMove::MolDeleter::typeName );
            
            MolDeleter_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        MolDeleter_exposer.staticmethod( "null" );
        MolDeleter_exposer.staticmethod( "typeName" );
        MolDeleter_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMove::MolDeleter >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        MolDeleter_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMove::MolDeleter >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        MolDeleter_exposer.def( "__str__", &__str__< ::SireMove::MolDeleter > );
        MolDeleter_exposer.def( "__repr__", &__str__< ::SireMove::MolDeleter > );
    }

}