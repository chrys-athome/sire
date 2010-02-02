// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ErrorPacket.pypp.hpp"

namespace bp = boost::python;

#include "Siren/datastream.h"

#include "Siren/errors.h"

#include "Siren/stream.h"

#include "workpacket.h"

#include <QDebug>

#include <QTextStream>

#include "workpacket.h"

#include "Siren/logger.h"

SireCluster::ErrorPacket __copy__(const SireCluster::ErrorPacket &other){ return SireCluster::ErrorPacket(other); }

#include "Siren/str.hpp"

void register_ErrorPacket_class(){

    { //::SireCluster::ErrorPacket
        typedef bp::class_< SireCluster::ErrorPacket, bp::bases< SireCluster::WorkPacket, Siren::Object > > ErrorPacket_exposer_t;
        ErrorPacket_exposer_t ErrorPacket_exposer = ErrorPacket_exposer_t( "ErrorPacket" );
        bp::scope ErrorPacket_scope( ErrorPacket_exposer );
        ErrorPacket_exposer.def( bp::init< >() );
        ErrorPacket_exposer.def( bp::init< Siren::exception const & >(( bp::arg("e") )) );
        ErrorPacket_exposer.def( bp::init< SireCluster::ErrorPacket const & >(( bp::arg("other") )) );
        { //::SireCluster::ErrorPacket::approximatePacketSize
        
            typedef int ( ::SireCluster::ErrorPacket::*approximatePacketSize_function_type )(  ) const;
            approximatePacketSize_function_type approximatePacketSize_function_value( &::SireCluster::ErrorPacket::approximatePacketSize );
            
            ErrorPacket_exposer.def( 
                "approximatePacketSize"
                , approximatePacketSize_function_value );
        
        }
        { //::SireCluster::ErrorPacket::hasFinished
        
            typedef bool ( ::SireCluster::ErrorPacket::*hasFinished_function_type )(  ) const;
            hasFinished_function_type hasFinished_function_value( &::SireCluster::ErrorPacket::hasFinished );
            
            ErrorPacket_exposer.def( 
                "hasFinished"
                , hasFinished_function_value );
        
        }
        { //::SireCluster::ErrorPacket::hashCode
        
            typedef ::uint ( ::SireCluster::ErrorPacket::*hashCode_function_type )(  ) const;
            hashCode_function_type hashCode_function_value( &::SireCluster::ErrorPacket::hashCode );
            
            ErrorPacket_exposer.def( 
                "hashCode"
                , hashCode_function_value );
        
        }
        { //::SireCluster::ErrorPacket::isError
        
            typedef bool ( ::SireCluster::ErrorPacket::*isError_function_type )(  ) const;
            isError_function_type isError_function_value( &::SireCluster::ErrorPacket::isError );
            
            ErrorPacket_exposer.def( 
                "isError"
                , isError_function_value );
        
        }
        ErrorPacket_exposer.def( bp::self != bp::self );
        { //::SireCluster::ErrorPacket::operator=
        
            typedef ::SireCluster::ErrorPacket & ( ::SireCluster::ErrorPacket::*assign_function_type )( ::SireCluster::ErrorPacket const & ) ;
            assign_function_type assign_function_value( &::SireCluster::ErrorPacket::operator= );
            
            ErrorPacket_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        ErrorPacket_exposer.def( bp::self == bp::self );
        { //::SireCluster::ErrorPacket::stream
        
            typedef void ( ::SireCluster::ErrorPacket::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireCluster::ErrorPacket::stream );
            
            ErrorPacket_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::SireCluster::ErrorPacket::throwError
        
            typedef void ( ::SireCluster::ErrorPacket::*throwError_function_type )(  ) const;
            throwError_function_type throwError_function_value( &::SireCluster::ErrorPacket::throwError );
            
            ErrorPacket_exposer.def( 
                "throwError"
                , throwError_function_value );
        
        }
        { //::SireCluster::ErrorPacket::toString
        
            typedef ::QString ( ::SireCluster::ErrorPacket::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireCluster::ErrorPacket::toString );
            
            ErrorPacket_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        ErrorPacket_exposer.def( "__copy__", &__copy__);
        ErrorPacket_exposer.def( "__deepcopy__", &__copy__);
        ErrorPacket_exposer.def( "clone", &__copy__);
        ErrorPacket_exposer.def( "__str__", &__str__< ::SireCluster::ErrorPacket > );
        ErrorPacket_exposer.def( "__repr__", &__str__< ::SireCluster::ErrorPacket > );
    }

}
