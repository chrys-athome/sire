// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "Matrix.pypp.hpp"
#include "boost/python.hpp"
#include "siremaths_headers.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_Matrix_class(){

    bp::class_< SireMaths::Matrix >( "Matrix" )    
        .def( bp::init< >() )    
        .def( bp::init< double >(( bp::arg("diagonal_value") )) )    
        .def( bp::init< double, double, double, double, double, double, double, double, double >(( bp::arg("xx"), bp::arg("xy"), bp::arg("xz"), bp::arg("yx"), bp::arg("yy"), bp::arg("yz"), bp::arg("zx"), bp::arg("zy"), bp::arg("zz") )) )    
        .def( bp::init< SireMaths::Vector const &, SireMaths::Vector const &, SireMaths::Vector const & >(( bp::arg("r1"), bp::arg("r2"), bp::arg("r3") )) )    
        .def( bp::init< boost::tuples::tuple<SireMaths::Vector, SireMaths::Vector, SireMaths::Vector, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type> const & >(( bp::arg("rows") )) )    
        .def( 
            "column0"
            , (::SireMaths::Vector ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::column0 ) )    
        .def( 
            "column1"
            , (::SireMaths::Vector ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::column1 ) )    
        .def( 
            "column2"
            , (::SireMaths::Vector ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::column2 ) )    
        .def( 
            "determinant"
            , (double ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::determinant ) )    
        .def( 
            "enforceSymmetric"
            , (void ( ::SireMaths::Matrix::* )(  ) )( &::SireMaths::Matrix::enforceSymmetric ) )    
        .def( 
            "getPrincipalAxes"
            , (::SireMaths::Matrix ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::getPrincipalAxes ) )    
        .def( 
            "identity"
            , (::SireMaths::Matrix (*)(  ))( &::SireMaths::Matrix::identity ) )    
        .def( 
            "inverse"
            , (::SireMaths::Matrix ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::inverse ) )    
        .def( 
            "isIdentity"
            , (bool ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::isIdentity ) )    
        .def( 
            "isSymmetric"
            , (bool ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::isSymmetric ) )    
        .def( bp::self != bp::self )    
        .def( bp::self *= bp::self )    
        .def( bp::self *= bp::other< double >() )    
        .def( bp::self += bp::self )    
        .def( bp::self -= bp::self )    
        .def( bp::self /= bp::other< double >() )    
        .def( bp::self == bp::self )    
        .def( 
            "row0"
            , (::SireMaths::Vector ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::row0 ) )    
        .def( 
            "row1"
            , (::SireMaths::Vector ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::row1 ) )    
        .def( 
            "row2"
            , (::SireMaths::Vector ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::row2 ) )    
        .def( 
            "setToIdentity"
            , (void ( ::SireMaths::Matrix::* )(  ) )( &::SireMaths::Matrix::setToIdentity ) )    
        .def( 
            "toString"
            , (::QString ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::toString ) )    
        .def( 
            "trace"
            , (::SireMaths::Vector ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::trace ) )    
        .def( 
            "transpose"
            , (::SireMaths::Matrix ( ::SireMaths::Matrix::* )(  ) const)( &::SireMaths::Matrix::transpose ) )    
        .def( 
            "zero"
            , (::SireMaths::Matrix (*)(  ))( &::SireMaths::Matrix::zero ) )    
        .staticmethod( "identity" )    
        .staticmethod( "zero" )    
        .def(self + self)    
        .def(self - self)    
        .def(self * self)    
        .def(self += self)    
        .def(self -= self)    
        .def(self *= self)    
        .def(self *= other<double>())    
        .def(self * other<double>())    
        .def(other<double>() * self)    
        .def(self * other<SireMaths::Vector>())    
        .def(other<SireMaths::Vector>() * self)    
        .def(self == self)    
        .def(self != self)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMaths::Matrix >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMaths::Matrix >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireMaths::Matrix > );

}
