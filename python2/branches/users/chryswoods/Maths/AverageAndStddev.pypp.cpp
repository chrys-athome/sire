// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "AverageAndStddev.pypp.hpp"

namespace bp = boost::python;

#include "SireMaths/maths.h"

#include "Siren/errors.h"

#include "Siren/objref.h"

#include "Siren/stream.h"

#include "Siren/streamqt.h"

#include "accumulator.h"

#include <QDebug>

#include <QMutex>

#include <cmath>

#include "accumulator.h"

#include "Siren/logger.h"

SireMaths::AverageAndStddev __copy__(const SireMaths::AverageAndStddev &other){ return SireMaths::AverageAndStddev(other); }

#include "Siren/str.hpp"

void register_AverageAndStddev_class(){

    { //::SireMaths::AverageAndStddev
        typedef bp::class_< SireMaths::AverageAndStddev, bp::bases< SireMaths::Average, SireMaths::Accumulator, Siren::Mutable, Siren::Object > > AverageAndStddev_exposer_t;
        AverageAndStddev_exposer_t AverageAndStddev_exposer = AverageAndStddev_exposer_t( "AverageAndStddev" );
        bp::scope AverageAndStddev_scope( AverageAndStddev_exposer );
        AverageAndStddev_exposer.def( bp::init< >() );
        AverageAndStddev_exposer.def( bp::init< SireMaths::AverageAndStddev const & >(( bp::arg("other") )) );
        { //::SireMaths::AverageAndStddev::accumulate
        
            typedef void ( ::SireMaths::AverageAndStddev::*accumulate_function_type )( double ) ;
            accumulate_function_type accumulate_function_value( &::SireMaths::AverageAndStddev::accumulate );
            
            AverageAndStddev_exposer.def( 
                "accumulate"
                , accumulate_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMaths::AverageAndStddev::clear
        
            typedef void ( ::SireMaths::AverageAndStddev::*clear_function_type )(  ) ;
            clear_function_type clear_function_value( &::SireMaths::AverageAndStddev::clear );
            
            AverageAndStddev_exposer.def( 
                "clear"
                , clear_function_value );
        
        }
        { //::SireMaths::AverageAndStddev::meanOfSquares
        
            typedef double ( ::SireMaths::AverageAndStddev::*meanOfSquares_function_type )(  ) const;
            meanOfSquares_function_type meanOfSquares_function_value( &::SireMaths::AverageAndStddev::meanOfSquares );
            
            AverageAndStddev_exposer.def( 
                "meanOfSquares"
                , meanOfSquares_function_value );
        
        }
        AverageAndStddev_exposer.def( bp::self != bp::self );
        { //::SireMaths::AverageAndStddev::operator=
        
            typedef ::SireMaths::AverageAndStddev & ( ::SireMaths::AverageAndStddev::*assign_function_type )( ::SireMaths::AverageAndStddev const & ) ;
            assign_function_type assign_function_value( &::SireMaths::AverageAndStddev::operator= );
            
            AverageAndStddev_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        AverageAndStddev_exposer.def( bp::self == bp::self );
        { //::SireMaths::AverageAndStddev::standardDeviation
        
            typedef double ( ::SireMaths::AverageAndStddev::*standardDeviation_function_type )(  ) const;
            standardDeviation_function_type standardDeviation_function_value( &::SireMaths::AverageAndStddev::standardDeviation );
            
            AverageAndStddev_exposer.def( 
                "standardDeviation"
                , standardDeviation_function_value );
        
        }
        { //::SireMaths::AverageAndStddev::stddev
        
            typedef double ( ::SireMaths::AverageAndStddev::*stddev_function_type )(  ) const;
            stddev_function_type stddev_function_value( &::SireMaths::AverageAndStddev::stddev );
            
            AverageAndStddev_exposer.def( 
                "stddev"
                , stddev_function_value );
        
        }
        { //::SireMaths::AverageAndStddev::stream
        
            typedef void ( ::SireMaths::AverageAndStddev::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireMaths::AverageAndStddev::stream );
            
            AverageAndStddev_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::SireMaths::AverageAndStddev::toString
        
            typedef ::QString ( ::SireMaths::AverageAndStddev::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMaths::AverageAndStddev::toString );
            
            AverageAndStddev_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        AverageAndStddev_exposer.def( "__copy__", &__copy__);
        AverageAndStddev_exposer.def( "__deepcopy__", &__copy__);
        AverageAndStddev_exposer.def( "clone", &__copy__);
        AverageAndStddev_exposer.def( "__str__", &__str__< ::SireMaths::AverageAndStddev > );
        AverageAndStddev_exposer.def( "__repr__", &__str__< ::SireMaths::AverageAndStddev > );
    }

}