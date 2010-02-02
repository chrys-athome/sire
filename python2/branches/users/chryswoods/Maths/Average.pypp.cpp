// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Average.pypp.hpp"

namespace bp = boost::python;

#include "SireMaths/maths.h"

#include "Siren/errors.h"

#include "Siren/objref.h"

#include "Siren/stream.h"

#include "Siren/streamqt.h"

#include "accumulator.h"

#include <QDebug>

#include <cmath>

#include "accumulator.h"

#include "Siren/logger.h"

SireMaths::Average __copy__(const SireMaths::Average &other){ return SireMaths::Average(other); }

#include "Siren/str.hpp"

void register_Average_class(){

    { //::SireMaths::Average
        typedef bp::class_< SireMaths::Average, bp::bases< SireMaths::Accumulator, Siren::Mutable, Siren::Object > > Average_exposer_t;
        Average_exposer_t Average_exposer = Average_exposer_t( "Average" );
        bp::scope Average_scope( Average_exposer );
        Average_exposer.def( bp::init< >() );
        Average_exposer.def( bp::init< SireMaths::Average const & >(( bp::arg("other") )) );
        { //::SireMaths::Average::accumulate
        
            typedef void ( ::SireMaths::Average::*accumulate_function_type )( double ) ;
            accumulate_function_type accumulate_function_value( &::SireMaths::Average::accumulate );
            
            Average_exposer.def( 
                "accumulate"
                , accumulate_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMaths::Average::average
        
            typedef double ( ::SireMaths::Average::*average_function_type )(  ) const;
            average_function_type average_function_value( &::SireMaths::Average::average );
            
            Average_exposer.def( 
                "average"
                , average_function_value );
        
        }
        { //::SireMaths::Average::clear
        
            typedef void ( ::SireMaths::Average::*clear_function_type )(  ) ;
            clear_function_type clear_function_value( &::SireMaths::Average::clear );
            
            Average_exposer.def( 
                "clear"
                , clear_function_value );
        
        }
        Average_exposer.def( bp::self != bp::self );
        { //::SireMaths::Average::operator=
        
            typedef ::SireMaths::Average & ( ::SireMaths::Average::*assign_function_type )( ::SireMaths::Average const & ) ;
            assign_function_type assign_function_value( &::SireMaths::Average::operator= );
            
            Average_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Average_exposer.def( bp::self == bp::self );
        { //::SireMaths::Average::stream
        
            typedef void ( ::SireMaths::Average::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireMaths::Average::stream );
            
            Average_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::SireMaths::Average::toString
        
            typedef ::QString ( ::SireMaths::Average::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMaths::Average::toString );
            
            Average_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        Average_exposer.def( "__copy__", &__copy__);
        Average_exposer.def( "__deepcopy__", &__copy__);
        Average_exposer.def( "clone", &__copy__);
        Average_exposer.def( "__str__", &__str__< ::SireMaths::Average > );
        Average_exposer.def( "__repr__", &__str__< ::SireMaths::Average > );
    }

}
