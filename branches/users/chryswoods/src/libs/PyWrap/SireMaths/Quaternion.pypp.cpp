// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "Quaternion.pypp.hpp"
#include "boost/python.hpp"
#include "siremaths_headers.h"

namespace bp = boost::python;

SireMaths::Quaternion __copy__(const SireMaths::Quaternion &other){ return SireMaths::Quaternion(other); }

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_Quaternion_class(){

    bp::class_< SireMaths::Quaternion >( "Quaternion" )    
        .def( bp::init< >() )    
        .def( bp::init< SireMaths::Angle const &, SireMaths::Vector const & >(( bp::arg("angle"), bp::arg("axis") )) )    
        .def( bp::init< SireMaths::Matrix const & >(( bp::arg("m") )) )    
        .def( bp::init< double, double, double, double >(( bp::arg("x"), bp::arg("y"), bp::arg("z"), bp::arg("w") )) )    
        .def( 
            "conjugate"
            , (::SireMaths::Quaternion ( ::SireMaths::Quaternion::* )(  ) const)( &::SireMaths::Quaternion::conjugate ) )    
        .def( 
            "dot"
            , (double ( ::SireMaths::Quaternion::* )( ::SireMaths::Quaternion const & ) const)( &::SireMaths::Quaternion::dot )
            , ( bp::arg("q") ) )    
        .def( 
            "fromMatrix"
            , (void ( ::SireMaths::Quaternion::* )( ::SireMaths::Matrix const & ) )( &::SireMaths::Quaternion::fromMatrix )
            , ( bp::arg("m") ) )    
        .def( 
            "fromString"
            , (::SireMaths::Quaternion (*)( ::QString const & ))( &::SireMaths::Quaternion::fromString )
            , ( bp::arg("str") ) )    
        .def( 
            "identity"
            , (::SireMaths::Quaternion (*)(  ))( &::SireMaths::Quaternion::identity ) )    
        .def( 
            "inverse"
            , (::SireMaths::Quaternion ( ::SireMaths::Quaternion::* )(  ) const)( &::SireMaths::Quaternion::inverse ) )    
        .def( 
            "isIdentity"
            , (bool ( ::SireMaths::Quaternion::* )(  ) const)( &::SireMaths::Quaternion::isIdentity ) )    
        .def( bp::self != bp::self )    
        .def( bp::self == bp::self )    
        .def( 
            "pow"
            , (::SireMaths::Quaternion ( ::SireMaths::Quaternion::* )( double ) const)( &::SireMaths::Quaternion::pow )
            , ( bp::arg("n") ) )    
        .def( 
            "renormalise"
            , (void ( ::SireMaths::Quaternion::* )(  ) )( &::SireMaths::Quaternion::renormalise ) )    
        .def( 
            "rotate"
            , (::SireMaths::Vector ( ::SireMaths::Quaternion::* )( ::SireMaths::Vector const & ) const)( &::SireMaths::Quaternion::rotate )
            , ( bp::arg("p") ) )    
        .def( 
            "slerp"
            , (::SireMaths::Quaternion ( ::SireMaths::Quaternion::* )( ::SireMaths::Quaternion const &,double ) const)( &::SireMaths::Quaternion::slerp )
            , ( bp::arg("q"), bp::arg("lambda") ) )    
        .def( 
            "toMatrix"
            , (::SireMaths::Matrix ( ::SireMaths::Quaternion::* )(  ) const)( &::SireMaths::Quaternion::toMatrix ) )    
        .def( 
            "toString"
            , (::QString ( ::SireMaths::Quaternion::* )(  ) const)( &::SireMaths::Quaternion::toString ) )    
        .def( 
            "w"
            , (double ( ::SireMaths::Quaternion::* )(  ) const)( &::SireMaths::Quaternion::w ) )    
        .def( 
            "x"
            , (double ( ::SireMaths::Quaternion::* )(  ) const)( &::SireMaths::Quaternion::x ) )    
        .def( 
            "y"
            , (double ( ::SireMaths::Quaternion::* )(  ) const)( &::SireMaths::Quaternion::y ) )    
        .def( 
            "z"
            , (double ( ::SireMaths::Quaternion::* )(  ) const)( &::SireMaths::Quaternion::z ) )    
        .staticmethod( "fromString" )    
        .staticmethod( "identity" )    
        .def( bp::self != bp::self )    
        .def( bp::self * bp::self )    
        .def( bp::self * bp::other< SireMaths::Vector >() )    
        .def( bp::self + bp::self )    
        .def( bp::self - bp::self )    
        .def( bp::self == bp::self )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMaths::Quaternion >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMaths::Quaternion >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireMaths::Quaternion > );

}
