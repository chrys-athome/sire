// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "AbortPacket.pypp.hpp"

namespace bp = boost::python;

#include "Siren/datastream.h"

#include "Siren/errors.h"

#include "Siren/stream.h"

#include "workpacket.h"

#include <QDebug>

#include <QTextStream>

#include "workpacket.h"

#include "Siren/logger.h"

SireCluster::AbortPacket __copy__(const SireCluster::AbortPacket &other){ return SireCluster::AbortPacket(other); }

#include "Siren/str.hpp"

void register_AbortPacket_class(){

    { //::SireCluster::AbortPacket
        typedef bp::class_< SireCluster::AbortPacket, bp::bases< SireCluster::WorkPacket, Siren::Object > > AbortPacket_exposer_t;
        AbortPacket_exposer_t AbortPacket_exposer = AbortPacket_exposer_t( "AbortPacket" );
        bp::scope AbortPacket_scope( AbortPacket_exposer );
        AbortPacket_exposer.def( bp::init< >() );
        AbortPacket_exposer.def( bp::init< SireCluster::AbortPacket const & >(( bp::arg("other") )) );
        { //::SireCluster::AbortPacket::hasFinished
        
            typedef bool ( ::SireCluster::AbortPacket::*hasFinished_function_type )(  ) const;
            hasFinished_function_type hasFinished_function_value( &::SireCluster::AbortPacket::hasFinished );
            
            AbortPacket_exposer.def( 
                "hasFinished"
                , hasFinished_function_value );
        
        }
        { //::SireCluster::AbortPacket::hashCode
        
            typedef ::uint ( ::SireCluster::AbortPacket::*hashCode_function_type )(  ) const;
            hashCode_function_type hashCode_function_value( &::SireCluster::AbortPacket::hashCode );
            
            AbortPacket_exposer.def( 
                "hashCode"
                , hashCode_function_value );
        
        }
        AbortPacket_exposer.def( bp::self != bp::self );
        { //::SireCluster::AbortPacket::operator=
        
            typedef ::SireCluster::AbortPacket & ( ::SireCluster::AbortPacket::*assign_function_type )( ::SireCluster::AbortPacket const & ) ;
            assign_function_type assign_function_value( &::SireCluster::AbortPacket::operator= );
            
            AbortPacket_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        AbortPacket_exposer.def( bp::self == bp::self );
        { //::SireCluster::AbortPacket::stream
        
            typedef void ( ::SireCluster::AbortPacket::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireCluster::AbortPacket::stream );
            
            AbortPacket_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("stream") ) );
        
        }
        { //::SireCluster::AbortPacket::toString
        
            typedef ::QString ( ::SireCluster::AbortPacket::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireCluster::AbortPacket::toString );
            
            AbortPacket_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireCluster::AbortPacket::wasAborted
        
            typedef bool ( ::SireCluster::AbortPacket::*wasAborted_function_type )(  ) const;
            wasAborted_function_type wasAborted_function_value( &::SireCluster::AbortPacket::wasAborted );
            
            AbortPacket_exposer.def( 
                "wasAborted"
                , wasAborted_function_value );
        
        }
        AbortPacket_exposer.def( "__copy__", &__copy__);
        AbortPacket_exposer.def( "__deepcopy__", &__copy__);
        AbortPacket_exposer.def( "clone", &__copy__);
        AbortPacket_exposer.def( "__str__", &__str__< ::SireCluster::AbortPacket > );
        AbortPacket_exposer.def( "__repr__", &__str__< ::SireCluster::AbortPacket > );
    }

}