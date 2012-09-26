// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "EnergyHistogram.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireID/index.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "histogram.h"

#include "histogram.h"

SireMaths::HistogramT<SireUnits::Dimension::PhysUnit<1, 2, -2, 0, 0, 0, 0> > __copy__(const SireMaths::HistogramT<SireUnits::Dimension::PhysUnit<1, 2, -2, 0, 0, 0, 0> > &other){ return SireMaths::HistogramT<SireUnits::Dimension::PhysUnit<1, 2, -2, 0, 0, 0, 0> >(other); }

#include "Helpers/str.hpp"

void register_EnergyHistogram_class(){

    { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >
        typedef bp::class_< SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >, bp::bases< SireMaths::Histogram, SireMaths::HistogramRange > > EnergyHistogram_exposer_t;
        EnergyHistogram_exposer_t EnergyHistogram_exposer = EnergyHistogram_exposer_t( "EnergyHistogram", bp::init< >() );
        bp::scope EnergyHistogram_scope( EnergyHistogram_exposer );
        EnergyHistogram_exposer.def( bp::init< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 >, SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 >, bp::optional< int > >(( bp::arg("min"), bp::arg("max"), bp::arg("nbins")=(int)(101) )) );
        EnergyHistogram_exposer.def( bp::init< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 >, SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 >, SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >(( bp::arg("min"), bp::arg("max"), bp::arg("binwidth") )) );
        EnergyHistogram_exposer.def( bp::init< SireMaths::HistogramRangeT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > const & >(( bp::arg("range") )) );
        EnergyHistogram_exposer.def( bp::init< SireMaths::HistogramRangeT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > const &, QVector< double > const & >(( bp::arg("range"), bp::arg("values") )) );
        EnergyHistogram_exposer.def( bp::init< SireMaths::Histogram const & >(( bp::arg("other") )) );
        EnergyHistogram_exposer.def( bp::init< SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > const & >(( bp::arg("other") )) );
        { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::accumulate
        
            typedef SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef void ( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::*accumulate_function_type )( ::SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > ) ;
            accumulate_function_type accumulate_function_value( &::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::accumulate );
            
            EnergyHistogram_exposer.def( 
                "accumulate"
                , accumulate_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::accumulate
        
            typedef SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef void ( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::*accumulate_function_type )( ::SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 >,double ) ;
            accumulate_function_type accumulate_function_value( &::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::accumulate );
            
            EnergyHistogram_exposer.def( 
                "accumulate"
                , accumulate_function_value
                , ( bp::arg("value"), bp::arg("weight") ) );
        
        }
        { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::accumulate
        
            typedef SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef void ( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::*accumulate_function_type )( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > const & ) ;
            accumulate_function_type accumulate_function_value( &::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::accumulate );
            
            EnergyHistogram_exposer.def( 
                "accumulate"
                , accumulate_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::bin
        
            typedef SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef int ( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::*bin_function_type )( ::SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > ) const;
            bin_function_type bin_function_value( &::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::bin );
            
            EnergyHistogram_exposer.def( 
                "bin"
                , bin_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::binWidth
        
            typedef SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > ( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::*binWidth_function_type )(  ) const;
            binWidth_function_type binWidth_function_value( &::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::binWidth );
            
            EnergyHistogram_exposer.def( 
                "binWidth"
                , binWidth_function_value );
        
        }
        { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::maximum
        
            typedef SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > ( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::*maximum_function_type )(  ) const;
            maximum_function_type maximum_function_value( &::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::maximum );
            
            EnergyHistogram_exposer.def( 
                "maximum"
                , maximum_function_value );
        
        }
        { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::middle
        
            typedef SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > ( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::*middle_function_type )(  ) const;
            middle_function_type middle_function_value( &::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::middle );
            
            EnergyHistogram_exposer.def( 
                "middle"
                , middle_function_value );
        
        }
        { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::minimum
        
            typedef SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > ( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::*minimum_function_type )(  ) const;
            minimum_function_type minimum_function_value( &::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::minimum );
            
            EnergyHistogram_exposer.def( 
                "minimum"
                , minimum_function_value );
        
        }
        EnergyHistogram_exposer.def( bp::self != bp::self );
        { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::operator=
        
            typedef SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > & ( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::*assign_function_type )( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > const & ) ;
            assign_function_type assign_function_value( &::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::operator= );
            
            EnergyHistogram_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        EnergyHistogram_exposer.def( bp::self == bp::self );
        { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::operator[]
        
            typedef SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireMaths::HistogramValueT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > ( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::*__getitem___function_type )( int ) const;
            __getitem___function_type __getitem___function_value( &::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::operator[] );
            
            EnergyHistogram_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::toString
        
            typedef SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::QString ( ::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMaths::HistogramT< SireUnits::Dimension::PhysUnit< 1, 2, -2, 0, 0, 0, 0 > >::toString );
            
            EnergyHistogram_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        EnergyHistogram_exposer.def( "__copy__", &__copy__);
        EnergyHistogram_exposer.def( "__deepcopy__", &__copy__);
        EnergyHistogram_exposer.def( "clone", &__copy__);
        EnergyHistogram_exposer.def( "__str__", &__str__< ::SireMaths::HistogramT<SireUnits::Dimension::PhysUnit<1, 2, -2, 0, 0, 0, 0> > > );
        EnergyHistogram_exposer.def( "__repr__", &__str__< ::SireMaths::HistogramT<SireUnits::Dimension::PhysUnit<1, 2, -2, 0, 0, 0, 0> > > );
    }

}