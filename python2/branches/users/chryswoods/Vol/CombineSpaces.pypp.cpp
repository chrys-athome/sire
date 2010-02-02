// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "CombineSpaces.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/properties.h"

#include "Siren/errors.h"

#include "Siren/logger.h"

#include "Siren/stream.h"

#include "Siren/tester.h"

#include "combinedspace.h"

#include "combinespaces.h"

#include "space.h"

#include "combinespaces.h"

#include "Siren/logger.h"

SireVol::CombineSpaces __copy__(const SireVol::CombineSpaces &other){ return SireVol::CombineSpaces(other); }

#include "Siren/str.hpp"

void register_CombineSpaces_class(){

    { //::SireVol::CombineSpaces
        typedef bp::class_< SireVol::CombineSpaces, bp::bases< SireBase::CombineProperties, Siren::Object > > CombineSpaces_exposer_t;
        CombineSpaces_exposer_t CombineSpaces_exposer = CombineSpaces_exposer_t( "CombineSpaces" );
        bp::scope CombineSpaces_scope( CombineSpaces_exposer );
        CombineSpaces_exposer.def( bp::init< >() );
        CombineSpaces_exposer.def( bp::init< SireBase::PropertyName const & >(( bp::arg("source") )) );
        CombineSpaces_exposer.def( bp::init< SireBase::PropertyName const &, SireBase::PropertyName const & >(( bp::arg("source0"), bp::arg("source1") )) );
        CombineSpaces_exposer.def( bp::init< QList< SireBase::PropertyName > const & >(( bp::arg("sources") )) );
        CombineSpaces_exposer.def( bp::init< QVector< SireBase::PropertyName > const & >(( bp::arg("sources") )) );
        CombineSpaces_exposer.def( bp::init< QList< QString > const & >(( bp::arg("sources") )) );
        CombineSpaces_exposer.def( bp::init< QVector< QString > const & >(( bp::arg("sources") )) );
        CombineSpaces_exposer.def( bp::init< SireVol::CombineSpaces const & >(( bp::arg("other") )) );
        { //::SireVol::CombineSpaces::combine
        
            typedef ::Siren::ObjRef ( ::SireVol::CombineSpaces::*combine_function_type )( ::SireBase::Properties const & ) const;
            combine_function_type combine_function_value( &::SireVol::CombineSpaces::combine );
            
            CombineSpaces_exposer.def( 
                "combine"
                , combine_function_value
                , ( bp::arg("properties") ) );
        
        }
        CombineSpaces_exposer.def( bp::self != bp::self );
        { //::SireVol::CombineSpaces::operator=
        
            typedef ::SireVol::CombineSpaces & ( ::SireVol::CombineSpaces::*assign_function_type )( ::SireVol::CombineSpaces const & ) ;
            assign_function_type assign_function_value( &::SireVol::CombineSpaces::operator= );
            
            CombineSpaces_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        CombineSpaces_exposer.def( bp::self == bp::self );
        { //::SireVol::CombineSpaces::stream
        
            typedef void ( ::SireVol::CombineSpaces::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireVol::CombineSpaces::stream );
            
            CombineSpaces_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::SireVol::CombineSpaces::test
        
            typedef bool ( ::SireVol::CombineSpaces::*test_function_type )( ::Siren::Logger & ) const;
            test_function_type test_function_value( &::SireVol::CombineSpaces::test );
            
            CombineSpaces_exposer.def( 
                "test"
                , test_function_value
                , ( bp::arg("logger") ) );
        
        }
        { //::SireVol::CombineSpaces::toString
        
            typedef ::QString ( ::SireVol::CombineSpaces::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireVol::CombineSpaces::toString );
            
            CombineSpaces_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        CombineSpaces_exposer.def( "__copy__", &__copy__);
        CombineSpaces_exposer.def( "__deepcopy__", &__copy__);
        CombineSpaces_exposer.def( "clone", &__copy__);
        CombineSpaces_exposer.def( "__str__", &__str__< ::SireVol::CombineSpaces > );
        CombineSpaces_exposer.def( "__repr__", &__str__< ::SireVol::CombineSpaces > );
    }

}
