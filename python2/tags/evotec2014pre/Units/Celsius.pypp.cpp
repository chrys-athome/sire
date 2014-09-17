// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Celsius.pypp.hpp"

namespace bp = boost::python;

#include "temperature.h"

#include "temperature.h"

SireUnits::Celsius __copy__(const SireUnits::Celsius &other){ return SireUnits::Celsius(other); }

const char* pvt_get_name(const SireUnits::Celsius&){ return "SireUnits::Celsius";}

void register_Celsius_class(){

    { //::SireUnits::Celsius
        typedef bp::class_< SireUnits::Celsius, bp::bases< SireUnits::Dimension::TempBase > > Celsius_exposer_t;
        Celsius_exposer_t Celsius_exposer = Celsius_exposer_t( "Celsius", bp::init< >() );
        bp::scope Celsius_scope( Celsius_exposer );
        Celsius_exposer.def( bp::init< double >(( bp::arg("value") )) );
        Celsius_exposer.def( bp::init< SireUnits::Dimension::Temperature const & >(( bp::arg("temp") )) );
        Celsius_exposer.def( bp::init< SireUnits::Dimension::TempBase const & >(( bp::arg("other") )) );
        Celsius_exposer.def( bp::init< SireUnits::Celsius const & >(( bp::arg("other") )) );
        { //::SireUnits::Celsius::convertFromInternal
        
            typedef double ( ::SireUnits::Celsius::*convertFromInternal_function_type )( double ) const;
            convertFromInternal_function_type convertFromInternal_function_value( &::SireUnits::Celsius::convertFromInternal );
            
            Celsius_exposer.def( 
                "convertFromInternal"
                , convertFromInternal_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireUnits::Celsius::convertFromInternal
        
            typedef double ( ::SireUnits::Celsius::*convertFromInternal_function_type )(  ) const;
            convertFromInternal_function_type convertFromInternal_function_value( &::SireUnits::Celsius::convertFromInternal );
            
            Celsius_exposer.def( 
                "convertFromInternal"
                , convertFromInternal_function_value );
        
        }
        { //::SireUnits::Celsius::convertToInternal
        
            typedef double ( ::SireUnits::Celsius::*convertToInternal_function_type )( double ) const;
            convertToInternal_function_type convertToInternal_function_value( &::SireUnits::Celsius::convertToInternal );
            
            Celsius_exposer.def( 
                "convertToInternal"
                , convertToInternal_function_value
                , ( bp::arg("value") ) );
        
        }
        Celsius_exposer.def( bp::self * bp::other< double >() );
        Celsius_exposer.def( bp::self * bp::other< int >() );
        Celsius_exposer.def( bp::self + bp::self );
        Celsius_exposer.def( bp::self + bp::other< SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 1, 0, 0 > >() );
        Celsius_exposer.def( -bp::self );
        Celsius_exposer.def( bp::self - bp::self );
        Celsius_exposer.def( bp::self - bp::other< SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 1, 0, 0 > >() );
        Celsius_exposer.def( bp::self / bp::other< double >() );
        Celsius_exposer.def( bp::self / bp::other< int >() );
        { //::SireUnits::Celsius::operator=
        
            typedef ::SireUnits::Celsius & ( ::SireUnits::Celsius::*assign_function_type )( ::SireUnits::Celsius const & ) ;
            assign_function_type assign_function_value( &::SireUnits::Celsius::operator= );
            
            Celsius_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireUnits::Celsius::operator=
        
            typedef ::SireUnits::Celsius & ( ::SireUnits::Celsius::*assign_function_type )( ::SireUnits::Dimension::Temperature const & ) ;
            assign_function_type assign_function_value( &::SireUnits::Celsius::operator= );
            
            Celsius_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("temp") )
                , bp::return_self< >() );
        
        }
        Celsius_exposer.def( bp::other< int >() * bp::self );
        Celsius_exposer.def( bp::other< double >() * bp::self );
        Celsius_exposer.def( bp::other< int >() / bp::self );
        Celsius_exposer.def( bp::other< double >() / bp::self );
        Celsius_exposer.def( bp::other<double>() * bp::self );
        Celsius_exposer.def( bp::other<double>() / bp::self );
        Celsius_exposer.def( "__copy__", &__copy__);
        Celsius_exposer.def( "__deepcopy__", &__copy__);
        Celsius_exposer.def( "clone", &__copy__);
        Celsius_exposer.def( "__str__", &pvt_get_name);
        Celsius_exposer.def( "__repr__", &pvt_get_name);
    }

}
