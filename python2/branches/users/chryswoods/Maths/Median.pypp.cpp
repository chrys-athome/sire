// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Median.pypp.hpp"

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

SireMaths::Median __copy__(const SireMaths::Median &other){ return SireMaths::Median(other); }

#include "Siren/str.hpp"

void register_Median_class(){

    { //::SireMaths::Median
        typedef bp::class_< SireMaths::Median, bp::bases< SireMaths::Accumulator, Siren::Mutable, Siren::Object > > Median_exposer_t;
        Median_exposer_t Median_exposer = Median_exposer_t( "Median" );
        bp::scope Median_scope( Median_exposer );
        Median_exposer.def( bp::init< >() );
        Median_exposer.def( bp::init< SireMaths::Median const & >(( bp::arg("other") )) );
        { //::SireMaths::Median::accumulate
        
            typedef void ( ::SireMaths::Median::*accumulate_function_type )( double ) ;
            accumulate_function_type accumulate_function_value( &::SireMaths::Median::accumulate );
            
            Median_exposer.def( 
                "accumulate"
                , accumulate_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMaths::Median::clear
        
            typedef void ( ::SireMaths::Median::*clear_function_type )(  ) ;
            clear_function_type clear_function_value( &::SireMaths::Median::clear );
            
            Median_exposer.def( 
                "clear"
                , clear_function_value );
        
        }
        { //::SireMaths::Median::max
        
            typedef double ( ::SireMaths::Median::*max_function_type )(  ) const;
            max_function_type max_function_value( &::SireMaths::Median::max );
            
            Median_exposer.def( 
                "max"
                , max_function_value );
        
        }
        { //::SireMaths::Median::maximum
        
            typedef double ( ::SireMaths::Median::*maximum_function_type )(  ) const;
            maximum_function_type maximum_function_value( &::SireMaths::Median::maximum );
            
            Median_exposer.def( 
                "maximum"
                , maximum_function_value );
        
        }
        { //::SireMaths::Median::median
        
            typedef double ( ::SireMaths::Median::*median_function_type )(  ) const;
            median_function_type median_function_value( &::SireMaths::Median::median );
            
            Median_exposer.def( 
                "median"
                , median_function_value );
        
        }
        { //::SireMaths::Median::min
        
            typedef double ( ::SireMaths::Median::*min_function_type )(  ) const;
            min_function_type min_function_value( &::SireMaths::Median::min );
            
            Median_exposer.def( 
                "min"
                , min_function_value );
        
        }
        { //::SireMaths::Median::minimum
        
            typedef double ( ::SireMaths::Median::*minimum_function_type )(  ) const;
            minimum_function_type minimum_function_value( &::SireMaths::Median::minimum );
            
            Median_exposer.def( 
                "minimum"
                , minimum_function_value );
        
        }
        Median_exposer.def( bp::self != bp::self );
        { //::SireMaths::Median::operator=
        
            typedef ::SireMaths::Median & ( ::SireMaths::Median::*assign_function_type )( ::SireMaths::Median const & ) ;
            assign_function_type assign_function_value( &::SireMaths::Median::operator= );
            
            Median_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Median_exposer.def( bp::self == bp::self );
        { //::SireMaths::Median::stream
        
            typedef void ( ::SireMaths::Median::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireMaths::Median::stream );
            
            Median_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::SireMaths::Median::toString
        
            typedef ::QString ( ::SireMaths::Median::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMaths::Median::toString );
            
            Median_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        Median_exposer.def( "__copy__", &__copy__);
        Median_exposer.def( "__deepcopy__", &__copy__);
        Median_exposer.def( "clone", &__copy__);
        Median_exposer.def( "__str__", &__str__< ::SireMaths::Median > );
        Median_exposer.def( "__repr__", &__str__< ::SireMaths::Median > );
    }

}
