// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Histogram.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireID/index.h"

#include "SireMaths/maths.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "histogram.h"

#include "histogram.h"

SireMaths::Histogram __copy__(const SireMaths::Histogram &other){ return SireMaths::Histogram(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_Histogram_class(){

    { //::SireMaths::Histogram
        typedef bp::class_< SireMaths::Histogram, bp::bases< SireBase::Property > > Histogram_exposer_t;
        Histogram_exposer_t Histogram_exposer = Histogram_exposer_t( "Histogram", bp::init< >() );
        bp::scope Histogram_scope( Histogram_exposer );
        Histogram_exposer.def( bp::init< double >(( bp::arg("binwidth") )) );
        Histogram_exposer.def( bp::init< double, QVector< double > const & >(( bp::arg("binwidth"), bp::arg("values") )) );
        Histogram_exposer.def( bp::init< SireMaths::Histogram const & >(( bp::arg("other") )) );
        { //::SireMaths::Histogram::accumulate
        
            typedef void ( ::SireMaths::Histogram::*accumulate_function_type )( double ) ;
            accumulate_function_type accumulate_function_value( &::SireMaths::Histogram::accumulate );
            
            Histogram_exposer.def( 
                "accumulate"
                , accumulate_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMaths::Histogram::accumulate
        
            typedef void ( ::SireMaths::Histogram::*accumulate_function_type )( double,double ) ;
            accumulate_function_type accumulate_function_value( &::SireMaths::Histogram::accumulate );
            
            Histogram_exposer.def( 
                "accumulate"
                , accumulate_function_value
                , ( bp::arg("value"), bp::arg("weight") ) );
        
        }
        { //::SireMaths::Histogram::accumulate
        
            typedef void ( ::SireMaths::Histogram::*accumulate_function_type )( ::QVector< double > const & ) ;
            accumulate_function_type accumulate_function_value( &::SireMaths::Histogram::accumulate );
            
            Histogram_exposer.def( 
                "accumulate"
                , accumulate_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMaths::Histogram::accumulate
        
            typedef void ( ::SireMaths::Histogram::*accumulate_function_type )( ::SireMaths::Histogram const & ) ;
            accumulate_function_type accumulate_function_value( &::SireMaths::Histogram::accumulate );
            
            Histogram_exposer.def( 
                "accumulate"
                , accumulate_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::Histogram::add
        
            typedef void ( ::SireMaths::Histogram::*add_function_type )( double ) ;
            add_function_type add_function_value( &::SireMaths::Histogram::add );
            
            Histogram_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMaths::Histogram::add
        
            typedef void ( ::SireMaths::Histogram::*add_function_type )( double,double ) ;
            add_function_type add_function_value( &::SireMaths::Histogram::add );
            
            Histogram_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("value"), bp::arg("weight") ) );
        
        }
        { //::SireMaths::Histogram::add
        
            typedef void ( ::SireMaths::Histogram::*add_function_type )( ::QVector< double > const & ) ;
            add_function_type add_function_value( &::SireMaths::Histogram::add );
            
            Histogram_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMaths::Histogram::add
        
            typedef void ( ::SireMaths::Histogram::*add_function_type )( ::SireMaths::Histogram const & ) ;
            add_function_type add_function_value( &::SireMaths::Histogram::add );
            
            Histogram_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::Histogram::at
        
            typedef ::SireMaths::HistogramValue ( ::SireMaths::Histogram::*at_function_type )( int ) const;
            at_function_type at_function_value( &::SireMaths::Histogram::at );
            
            Histogram_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMaths::Histogram::binWidth
        
            typedef double ( ::SireMaths::Histogram::*binWidth_function_type )(  ) const;
            binWidth_function_type binWidth_function_value( &::SireMaths::Histogram::binWidth );
            
            Histogram_exposer.def( 
                "binWidth"
                , binWidth_function_value );
        
        }
        { //::SireMaths::Histogram::count
        
            typedef int ( ::SireMaths::Histogram::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMaths::Histogram::count );
            
            Histogram_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMaths::Histogram::kirtosis
        
            typedef double ( ::SireMaths::Histogram::*kirtosis_function_type )(  ) const;
            kirtosis_function_type kirtosis_function_value( &::SireMaths::Histogram::kirtosis );
            
            Histogram_exposer.def( 
                "kirtosis"
                , kirtosis_function_value );
        
        }
        { //::SireMaths::Histogram::maximumValue
        
            typedef double ( ::SireMaths::Histogram::*maximumValue_function_type )(  ) const;
            maximumValue_function_type maximumValue_function_value( &::SireMaths::Histogram::maximumValue );
            
            Histogram_exposer.def( 
                "maximumValue"
                , maximumValue_function_value );
        
        }
        { //::SireMaths::Histogram::mean
        
            typedef double ( ::SireMaths::Histogram::*mean_function_type )(  ) const;
            mean_function_type mean_function_value( &::SireMaths::Histogram::mean );
            
            Histogram_exposer.def( 
                "mean"
                , mean_function_value );
        
        }
        { //::SireMaths::Histogram::meanOfSquares
        
            typedef double ( ::SireMaths::Histogram::*meanOfSquares_function_type )(  ) const;
            meanOfSquares_function_type meanOfSquares_function_value( &::SireMaths::Histogram::meanOfSquares );
            
            Histogram_exposer.def( 
                "meanOfSquares"
                , meanOfSquares_function_value );
        
        }
        { //::SireMaths::Histogram::median
        
            typedef double ( ::SireMaths::Histogram::*median_function_type )(  ) const;
            median_function_type median_function_value( &::SireMaths::Histogram::median );
            
            Histogram_exposer.def( 
                "median"
                , median_function_value );
        
        }
        { //::SireMaths::Histogram::minimumValue
        
            typedef double ( ::SireMaths::Histogram::*minimumValue_function_type )(  ) const;
            minimumValue_function_type minimumValue_function_value( &::SireMaths::Histogram::minimumValue );
            
            Histogram_exposer.def( 
                "minimumValue"
                , minimumValue_function_value );
        
        }
        { //::SireMaths::Histogram::mode
        
            typedef double ( ::SireMaths::Histogram::*mode_function_type )(  ) const;
            mode_function_type mode_function_value( &::SireMaths::Histogram::mode );
            
            Histogram_exposer.def( 
                "mode"
                , mode_function_value );
        
        }
        { //::SireMaths::Histogram::normalDistribution
        
            typedef ::QVector< SireMaths::HistogramValue > ( ::SireMaths::Histogram::*normalDistribution_function_type )(  ) const;
            normalDistribution_function_type normalDistribution_function_value( &::SireMaths::Histogram::normalDistribution );
            
            Histogram_exposer.def( 
                "normalDistribution"
                , normalDistribution_function_value );
        
        }
        { //::SireMaths::Histogram::normalise
        
            typedef ::SireMaths::Histogram ( ::SireMaths::Histogram::*normalise_function_type )(  ) const;
            normalise_function_type normalise_function_value( &::SireMaths::Histogram::normalise );
            
            Histogram_exposer.def( 
                "normalise"
                , normalise_function_value );
        
        }
        Histogram_exposer.def( bp::self != bp::self );
        Histogram_exposer.def( bp::self + bp::self );
        Histogram_exposer.def( bp::self + bp::other< double >() );
        Histogram_exposer.def( bp::self + bp::other< QVector< double > >() );
        { //::SireMaths::Histogram::operator=
        
            typedef ::SireMaths::Histogram & ( ::SireMaths::Histogram::*assign_function_type )( ::SireMaths::Histogram const & ) ;
            assign_function_type assign_function_value( &::SireMaths::Histogram::operator= );
            
            Histogram_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Histogram_exposer.def( bp::self == bp::self );
        { //::SireMaths::Histogram::operator[]
        
            typedef ::SireMaths::HistogramValue ( ::SireMaths::Histogram::*__getitem___function_type )( int ) const;
            __getitem___function_type __getitem___function_value( &::SireMaths::Histogram::operator[] );
            
            Histogram_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMaths::Histogram::range
        
            typedef double ( ::SireMaths::Histogram::*range_function_type )(  ) const;
            range_function_type range_function_value( &::SireMaths::Histogram::range );
            
            Histogram_exposer.def( 
                "range"
                , range_function_value );
        
        }
        { //::SireMaths::Histogram::resize
        
            typedef ::SireMaths::Histogram ( ::SireMaths::Histogram::*resize_function_type )( double ) const;
            resize_function_type resize_function_value( &::SireMaths::Histogram::resize );
            
            Histogram_exposer.def( 
                "resize"
                , resize_function_value
                , ( bp::arg("binwidth") ) );
        
        }
        { //::SireMaths::Histogram::size
        
            typedef int ( ::SireMaths::Histogram::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMaths::Histogram::size );
            
            Histogram_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMaths::Histogram::skew
        
            typedef double ( ::SireMaths::Histogram::*skew_function_type )(  ) const;
            skew_function_type skew_function_value( &::SireMaths::Histogram::skew );
            
            Histogram_exposer.def( 
                "skew"
                , skew_function_value );
        
        }
        { //::SireMaths::Histogram::standardDeviation
        
            typedef double ( ::SireMaths::Histogram::*standardDeviation_function_type )(  ) const;
            standardDeviation_function_type standardDeviation_function_value( &::SireMaths::Histogram::standardDeviation );
            
            Histogram_exposer.def( 
                "standardDeviation"
                , standardDeviation_function_value );
        
        }
        { //::SireMaths::Histogram::standardError
        
            typedef double ( ::SireMaths::Histogram::*standardError_function_type )(  ) const;
            standardError_function_type standardError_function_value( &::SireMaths::Histogram::standardError );
            
            Histogram_exposer.def( 
                "standardError"
                , standardError_function_value );
        
        }
        { //::SireMaths::Histogram::standardError
        
            typedef double ( ::SireMaths::Histogram::*standardError_function_type )( double ) const;
            standardError_function_type standardError_function_value( &::SireMaths::Histogram::standardError );
            
            Histogram_exposer.def( 
                "standardError"
                , standardError_function_value
                , ( bp::arg("level") ) );
        
        }
        { //::SireMaths::Histogram::sumOfBins
        
            typedef double ( ::SireMaths::Histogram::*sumOfBins_function_type )(  ) const;
            sumOfBins_function_type sumOfBins_function_value( &::SireMaths::Histogram::sumOfBins );
            
            Histogram_exposer.def( 
                "sumOfBins"
                , sumOfBins_function_value );
        
        }
        { //::SireMaths::Histogram::tValue
        
            typedef double ( *tValue_function_type )( int,double );
            tValue_function_type tValue_function_value( &::SireMaths::Histogram::tValue );
            
            Histogram_exposer.def( 
                "tValue"
                , tValue_function_value
                , ( bp::arg("nsamples"), bp::arg("level") ) );
        
        }
        { //::SireMaths::Histogram::tValue
        
            typedef double ( ::SireMaths::Histogram::*tValue_function_type )( double ) const;
            tValue_function_type tValue_function_value( &::SireMaths::Histogram::tValue );
            
            Histogram_exposer.def( 
                "tValue"
                , tValue_function_value
                , ( bp::arg("level") ) );
        
        }
        { //::SireMaths::Histogram::toString
        
            typedef ::QString ( ::SireMaths::Histogram::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMaths::Histogram::toString );
            
            Histogram_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMaths::Histogram::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMaths::Histogram::typeName );
            
            Histogram_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMaths::Histogram::values
        
            typedef ::QVector< SireMaths::HistogramValue > ( ::SireMaths::Histogram::*values_function_type )(  ) const;
            values_function_type values_function_value( &::SireMaths::Histogram::values );
            
            Histogram_exposer.def( 
                "values"
                , values_function_value );
        
        }
        { //::SireMaths::Histogram::what
        
            typedef char const * ( ::SireMaths::Histogram::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMaths::Histogram::what );
            
            Histogram_exposer.def( 
                "what"
                , what_function_value );
        
        }
        Histogram_exposer.staticmethod( "tValue" );
        Histogram_exposer.staticmethod( "typeName" );
        Histogram_exposer.def( "__copy__", &__copy__);
        Histogram_exposer.def( "__deepcopy__", &__copy__);
        Histogram_exposer.def( "clone", &__copy__);
        Histogram_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMaths::Histogram >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Histogram_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMaths::Histogram >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Histogram_exposer.def( "__str__", &__str__< ::SireMaths::Histogram > );
        Histogram_exposer.def( "__repr__", &__str__< ::SireMaths::Histogram > );
    }

}
