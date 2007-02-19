// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "Expression.pypp.hpp"
#include "boost/python.hpp"
#include "sirecas_headers.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_Expression_class(){

    bp::class_< SireCAS::Expression >( "Expression" )    
        .def( bp::init< >() )    
        .def( bp::init< int >(( bp::arg("constant") )) )    
        .def( bp::init< SireMaths::Rational const & >(( bp::arg("constant") )) )    
        .def( bp::init< double >(( bp::arg("constant") )) )    
        .def( bp::init< SireMaths::Complex const & >(( bp::arg("constant") )) )    
        .def( bp::init< SireCAS::ExpressionBase const & >(( bp::arg("base") )) )    
        .def( bp::init< SireCAS::ExBase const & >(( bp::arg("base") )) )    
        .def( 
            "add"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireCAS::Expression const & ) const)( &::SireCAS::Expression::add )
            , ( bp::arg("ex") ) )    
        .def( 
            "base"
            , (::SireCAS::ExpressionBase const & ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::base )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "children"
            , (::SireCAS::Expressions ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::children ) )    
        .def( 
            "collapse"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::collapse ) )    
        .def( 
            "conjugate"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::conjugate ) )    
        .def( 
            "cubed"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::cubed ) )    
        .def( 
            "diff"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireCAS::Symbol const &,int ) const)( &::SireCAS::Expression::diff )
            , ( bp::arg("symbol"), bp::arg("level")=(int)(1) ) )    
        .def( 
            "differentiate"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireCAS::Symbol const &,int ) const)( &::SireCAS::Expression::differentiate )
            , ( bp::arg("symbol"), bp::arg("level")=(int)(1) ) )    
        .def( 
            "divide"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireCAS::Expression const & ) const)( &::SireCAS::Expression::divide )
            , ( bp::arg("ex") ) )    
        .def( 
            "divide"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( double ) const)( &::SireCAS::Expression::divide )
            , ( bp::arg("val") ) )    
        .def( 
            "divide"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireMaths::Complex const & ) const)( &::SireCAS::Expression::divide )
            , ( bp::arg("val") ) )    
        .def( 
            "evaluate"
            , (double ( ::SireCAS::Expression::* )( ::SireCAS::Values const & ) const)( &::SireCAS::Expression::evaluate )
            , ( bp::arg("values") ) )    
        .def( 
            "evaluate"
            , (::SireMaths::Complex ( ::SireCAS::Expression::* )( ::SireCAS::ComplexValues const & ) const)( &::SireCAS::Expression::evaluate )
            , ( bp::arg("values") ) )    
        .def( 
            "expand"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::expand ) )    
        .def( 
            "factor"
            , (double ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::factor ) )    
        .def( 
            "functions"
            , (::SireCAS::Functions ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::functions ) )    
        .def( 
            "hash"
            , (::uint ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::hash ) )    
        .def( 
            "integ"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireCAS::Symbol const & ) const)( &::SireCAS::Expression::integ )
            , ( bp::arg("symbol") ) )    
        .def( 
            "integrate"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireCAS::Symbol const & ) const)( &::SireCAS::Expression::integrate )
            , ( bp::arg("symbol") ) )    
        .def( 
            "invert"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::invert ) )    
        .def( 
            "isComplex"
            , (bool ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::isComplex ) )    
        .def( 
            "isCompound"
            , (bool ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::isCompound ) )    
        .def( 
            "isConstant"
            , (bool ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::isConstant ) )    
        .def( 
            "isFunction"
            , (bool ( ::SireCAS::Expression::* )( ::SireCAS::Symbol const & ) const)( &::SireCAS::Expression::isFunction )
            , ( bp::arg("symbol") ) )    
        .def( 
            "isZero"
            , (bool ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::isZero ) )    
        .def( 
            "multiply"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireCAS::Expression const & ) const)( &::SireCAS::Expression::multiply )
            , ( bp::arg("ex") ) )    
        .def( 
            "multiply"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( double ) const)( &::SireCAS::Expression::multiply )
            , ( bp::arg("val") ) )    
        .def( 
            "multiply"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireMaths::Complex const & ) const)( &::SireCAS::Expression::multiply )
            , ( bp::arg("val") ) )    
        .def( 
            "negate"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::negate ) )    
        .def( bp::self != bp::self )    
        .def( 
            "__call__"
            , (double ( ::SireCAS::Expression::* )( ::SireCAS::Values const & ) const)( &::SireCAS::Expression::operator() )
            , ( bp::arg("values") ) )    
        .def( 
            "__call__"
            , (::SireMaths::Complex ( ::SireCAS::Expression::* )( ::SireCAS::ComplexValues const & ) const)( &::SireCAS::Expression::operator() )
            , ( bp::arg("values") ) )    
        .def( bp::self *= bp::self )    
        .def( bp::self += bp::self )    
        .def( -bp::self )    
        .def( bp::self -= bp::self )    
        .def( bp::self /= bp::self )    
        .def( bp::self == bp::self )    
        .def( 
            "pow"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( int ) const)( &::SireCAS::Expression::pow )
            , ( bp::arg("n") ) )    
        .def( 
            "pow"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireMaths::Rational const & ) const)( &::SireCAS::Expression::pow )
            , ( bp::arg("n") ) )    
        .def( 
            "pow"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( double ) const)( &::SireCAS::Expression::pow )
            , ( bp::arg("n") ) )    
        .def( 
            "pow"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireMaths::Complex const & ) const)( &::SireCAS::Expression::pow )
            , ( bp::arg("n") ) )    
        .def( 
            "pow"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireCAS::Expression const & ) const)( &::SireCAS::Expression::pow )
            , ( bp::arg("n") ) )    
        .def( 
            "root"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( int ) const)( &::SireCAS::Expression::root )
            , ( bp::arg("n") ) )    
        .def( 
            "series"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireCAS::Symbol const &,int ) const)( &::SireCAS::Expression::series )
            , ( bp::arg("symbol"), bp::arg("order") ) )    
        .def( 
            "simplify"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( int ) const)( &::SireCAS::Expression::simplify )
            , ( bp::arg("options")=(int)(0) ) )    
        .def( 
            "squared"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::squared ) )    
        .def( 
            "substitute"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireCAS::Identities const & ) const)( &::SireCAS::Expression::substitute )
            , ( bp::arg("identities") ) )    
        .def( 
            "subtract"
            , (::SireCAS::Expression ( ::SireCAS::Expression::* )( ::SireCAS::Expression const & ) const)( &::SireCAS::Expression::subtract )
            , ( bp::arg("ex") ) )    
        .def( 
            "symbols"
            , (::SireCAS::Symbols ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::symbols ) )    
        .def( 
            "toString"
            , (::QString ( ::SireCAS::Expression::* )(  ) const)( &::SireCAS::Expression::toString ) )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireCAS::Expression >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireCAS::Expression >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireCAS::Expression > );

}
