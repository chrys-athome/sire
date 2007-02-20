// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "Angle.pypp.hpp"
#include "boost/python.hpp"
#include "siremaths_headers.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_Angle_class(){

    bp::class_< SireMaths::Angle >( "Angle" )    
        .def( bp::init< >() )    
        .def( bp::init< double >(( bp::arg("rad") )) )    
        .def( bp::init< double, double >(( bp::arg("x"), bp::arg("y") )) )    
        .def( 
            "degrees"
            , (::SireMaths::Angle (*)( double ))( &::SireMaths::Angle::degrees )
            , ( bp::arg("deg") ) )    
        .def( 
            "isWithin"
            , (bool ( ::SireMaths::Angle::* )( ::SireMaths::Angle const &,::SireMaths::Angle const & ) const)( &::SireMaths::Angle::isWithin )
            , ( bp::arg("ang"), bp::arg("delta") ) )    
        .def( 
            "isZero"
            , (bool ( ::SireMaths::Angle::* )(  ) const)( &::SireMaths::Angle::isZero ) )    
        .def( 
            "large"
            , (::SireMaths::Angle ( ::SireMaths::Angle::* )(  ) const)( &::SireMaths::Angle::large ) )    
        .def( 
            "largeDifference"
            , (::SireMaths::Angle ( ::SireMaths::Angle::* )( ::SireMaths::Angle const & ) const)( &::SireMaths::Angle::largeDifference )
            , ( bp::arg("ang") ) )    
        .def( 
            "negative"
            , (::SireMaths::Angle ( ::SireMaths::Angle::* )(  ) const)( &::SireMaths::Angle::negative ) )    
        .def( bp::self *= bp::other< double >() )    
        .def( bp::self += bp::self )    
        .def( bp::self -= bp::self )    
        .def( bp::self -= bp::other< double >() )    
        .def( bp::self /= bp::other< double >() )    
        .def( bp::self < bp::self )    
        .def( bp::self <= bp::self )    
        .def( bp::self > bp::self )    
        .def( bp::self >= bp::self )    
        .def( 
            "positive"
            , (::SireMaths::Angle ( ::SireMaths::Angle::* )(  ) const)( &::SireMaths::Angle::positive ) )    
        .def( 
            "setDegrees"
            , (void ( ::SireMaths::Angle::* )( double ) )( &::SireMaths::Angle::setDegrees )
            , ( bp::arg("theta") ) )    
        .def( 
            "setRadians"
            , (void ( ::SireMaths::Angle::* )( double ) )( &::SireMaths::Angle::setRadians )
            , ( bp::arg("theta") ) )    
        .def( 
            "small"
            , (::SireMaths::Angle ( ::SireMaths::Angle::* )(  ) const)( &::SireMaths::Angle::small ) )    
        .def( 
            "smallDifference"
            , (::SireMaths::Angle ( ::SireMaths::Angle::* )( ::SireMaths::Angle const & ) const)( &::SireMaths::Angle::smallDifference )
            , ( bp::arg("ang") ) )    
        .def( 
            "toDegrees"
            , (double ( ::SireMaths::Angle::* )(  ) const)( &::SireMaths::Angle::toDegrees ) )    
        .def( 
            "toRadians"
            , (double const & ( ::SireMaths::Angle::* )(  ) const)( &::SireMaths::Angle::toRadians )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "toString"
            , (::QString ( ::SireMaths::Angle::* )(  ) const)( &::SireMaths::Angle::toString ) )    
        .staticmethod( "degrees" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMaths::Angle >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMaths::Angle >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireMaths::Angle > );

}
