// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Fahrenheit.pypp.hpp"

namespace bp = boost::python;

#include "temperature.h"

#include "temperature.h"

#include "Siren/logger.h"

SireUnits::Fahrenheit __copy__(const SireUnits::Fahrenheit &other){ return SireUnits::Fahrenheit(other); }

#include "Siren/str.hpp"

void register_Fahrenheit_class(){

    { //::SireUnits::Fahrenheit
        typedef bp::class_< SireUnits::Fahrenheit, bp::bases< SireUnits::Dimension::detail::Temperature, Siren::Object > > Fahrenheit_exposer_t;
        Fahrenheit_exposer_t Fahrenheit_exposer = Fahrenheit_exposer_t( "Fahrenheit" );
        bp::scope Fahrenheit_scope( Fahrenheit_exposer );
        Fahrenheit_exposer.def( bp::init< >() );
        Fahrenheit_exposer.def( bp::init< double >(( bp::arg("value") )) );
        Fahrenheit_exposer.def( bp::init< SireUnits::Dimension::Temperature const & >(( bp::arg("temperature") )) );
        Fahrenheit_exposer.def( bp::init< SireUnits::Dimension::detail::Temperature const & >(( bp::arg("temperature") )) );
        Fahrenheit_exposer.def( bp::init< SireUnits::Fahrenheit const & >(( bp::arg("other") )) );
        { //::SireUnits::Fahrenheit::convertFromInternal
        
            typedef double ( ::SireUnits::Fahrenheit::*convertFromInternal_function_type )( double ) const;
            convertFromInternal_function_type convertFromInternal_function_value( &::SireUnits::Fahrenheit::convertFromInternal );
            
            Fahrenheit_exposer.def( 
                "convertFromInternal"
                , convertFromInternal_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireUnits::Fahrenheit::convertFromInternal
        
            typedef double ( ::SireUnits::Fahrenheit::*convertFromInternal_function_type )(  ) const;
            convertFromInternal_function_type convertFromInternal_function_value( &::SireUnits::Fahrenheit::convertFromInternal );
            
            Fahrenheit_exposer.def( 
                "convertFromInternal"
                , convertFromInternal_function_value );
        
        }
        { //::SireUnits::Fahrenheit::convertToInternal
        
            typedef double ( ::SireUnits::Fahrenheit::*convertToInternal_function_type )( double ) const;
            convertToInternal_function_type convertToInternal_function_value( &::SireUnits::Fahrenheit::convertToInternal );
            
            Fahrenheit_exposer.def( 
                "convertToInternal"
                , convertToInternal_function_value
                , ( bp::arg("value") ) );
        
        }
        Fahrenheit_exposer.def( bp::self != bp::self );
        Fahrenheit_exposer.def( bp::self * bp::other< double >() );
        Fahrenheit_exposer.def( bp::self * bp::other< int >() );
        Fahrenheit_exposer.def( bp::self + bp::self );
        Fahrenheit_exposer.def( -bp::self );
        Fahrenheit_exposer.def( bp::self - bp::self );
        Fahrenheit_exposer.def( bp::self / bp::other< double >() );
        Fahrenheit_exposer.def( bp::self / bp::other< int >() );
        { //::SireUnits::Fahrenheit::operator=
        
            typedef ::SireUnits::Fahrenheit & ( ::SireUnits::Fahrenheit::*assign_function_type )( ::SireUnits::Fahrenheit const & ) ;
            assign_function_type assign_function_value( &::SireUnits::Fahrenheit::operator= );
            
            Fahrenheit_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Fahrenheit_exposer.def( bp::self == bp::self );
        { //::SireUnits::Fahrenheit::stream
        
            typedef void ( ::SireUnits::Fahrenheit::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireUnits::Fahrenheit::stream );
            
            Fahrenheit_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::SireUnits::Fahrenheit::toString
        
            typedef ::QString ( ::SireUnits::Fahrenheit::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireUnits::Fahrenheit::toString );
            
            Fahrenheit_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        Fahrenheit_exposer.def( bp::other< int >() * bp::self );
        Fahrenheit_exposer.def( bp::other< double >() * bp::self );
        Fahrenheit_exposer.def( bp::other< int >() / bp::self );
        Fahrenheit_exposer.def( bp::other< double >() / bp::self );
        Fahrenheit_exposer.def( bp::other<double>() * bp::self );
        Fahrenheit_exposer.def( bp::other<double>() / bp::self );
        Fahrenheit_exposer.def( "__copy__", &__copy__);
        Fahrenheit_exposer.def( "__deepcopy__", &__copy__);
        Fahrenheit_exposer.def( "clone", &__copy__);
        Fahrenheit_exposer.def( "__str__", &__str__< ::SireUnits::Fahrenheit > );
        Fahrenheit_exposer.def( "__repr__", &__str__< ::SireUnits::Fahrenheit > );
    }

}