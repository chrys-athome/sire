
/**
  * This file contains the boost::python wrapping of Complex and complex functions
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireMaths/complex.h"

using namespace boost::python;

namespace SireMaths
{

void SIREMATHS_EXPORT export_Complex()
{
    /** Wrap the complex class itself */
    class_<Complex>( "Complex", init<>() )
        .def( init<double>() )
        .def( init<double,double>() )
        .def( init<const Complex&>() )
        .def( "real", &Complex::real )
        .def( "imag", &Complex::imag )
        .def( "isReal", &Complex::isReal )
        .def( "rect", &Complex::rect ).staticmethod("rect")
        .def( "polar", &Complex::polar ).staticmethod("polar")
        .def( "setRectangular", &Complex::setRectangular )
        .def( "setPolar", &Complex::setPolar )
        .def( "setReal", &Complex::setReal )
        .def( "setImag", &Complex::setImag )
        .def( self == self )
        .def( self != self )
        .def( self += self )
        .def( self -= self )
        .def( self *= self )
        .def( self /= self )
        .def( -self )
        .def( self += double() )
        .def( self -= double() )
        .def( self *= double() )
        .def( self /= double() )
        .def( "arg", &Complex::arg )
        .def( "abs", &Complex::abs )
        .def( "abs2", &Complex::abs2 )
        .def( "logAbs", &Complex::logAbs )
        .def( "conjugate", &Complex::conjugate )
        .def( "inverse", &Complex::inverse )
        .def( "negative", &Complex::negative )
        .def( self + self )
        .def( self - self )
        .def( self * self )
        .def( self / self )
        .def( self + double() )
        .def( self - double() )
        .def( self * double() )
        .def( self / double() )
        .def( double() + self )
        .def( double() - self )
        .def( double() * self )
        .def( double() / self )
    ;

    /** Now wrap the complex maths functions */
    Complex (*wrap_sqrt1)(const Complex&) = &sqrt;
    double (*wrap_sqrt2)(double) = &sqrt;
    Complex (*wrap_sqrt_real)(double) = &sqrt_real;
    
    Complex (*wrap_pow1)(const Complex&, const Complex&) = &pow;
    Complex (*wrap_pow2)(const Complex&, double) = &pow;
    Complex (*wrap_pow3)(const Complex&, int) = &pow;
    Complex (*wrap_pow4)(const Complex&, const Rational&) = &pow;
    Complex (*wrap_pow5)(double, const Complex&) = &pow;
    
    Complex (*wrap_exp)(const Complex&) = &exp;
    Complex (*wrap_log)(const Complex&) = &log;
    Complex (*wrap_log10)(const Complex&) = &log10;
    Complex (*wrap_log_b)(const Complex&, const Complex&) = &log_b;
    Complex (*wrap_sin)(const Complex&) = &sin;
    Complex (*wrap_cos)(const Complex&) = &cos;
    Complex (*wrap_tan)(const Complex&) = &tan;
    Complex (*wrap_sec)(const Complex&) = &sec;
    Complex (*wrap_csc)(const Complex&) = &csc;
    Complex (*wrap_cot)(const Complex&) = &cot;
    Complex (*wrap_arcsin)(const Complex&) = &arcsin;
    Complex (*wrap_arcsin_real)(double) = &arcsin_real;
    Complex (*wrap_arccos)(const Complex&) = &arccos;
    Complex (*wrap_arccos_real)(double) = &arccos_real;
    Complex (*wrap_arctan)(const Complex&) = &arctan;
    Complex (*wrap_arcsec)(const Complex&) = &arcsec;
    Complex (*wrap_arcsec_real)(double) = &arcsec_real;
    Complex (*wrap_arccsc)(const Complex&) = &arccsc;
    Complex (*wrap_arccsc_real)(double) = &arccsc_real;
    Complex (*wrap_arccot)(const Complex&) = &arccot;
    Complex (*wrap_sinh)(const Complex&) = &sinh;
    Complex (*wrap_cosh)(const Complex&) = &cosh;
    Complex (*wrap_tanh)(const Complex&) = &tanh;
    Complex (*wrap_sech)(const Complex&) = &sech;
    Complex (*wrap_csch)(const Complex&) = &csch;
    Complex (*wrap_coth)(const Complex&) = &coth;
    Complex (*wrap_arcsinh)(const Complex&) = &arcsinh;
    Complex (*wrap_arccosh)(const Complex&) = &arccosh;
    Complex (*wrap_arccosh_real)(double) = &arccosh_real;
    Complex (*wrap_arctanh)(const Complex&) = &arctanh;
    Complex (*wrap_arctanh_real)(double) = &arctanh_real;
    Complex (*wrap_arcsech)(const Complex&) = &arcsech;
    Complex (*wrap_arccsch)(const Complex&) = &arccsch;
    Complex (*wrap_arccoth)(const Complex&) = &arccoth;
    
    def( "sqrt", wrap_sqrt1 );
    def( "sqrt", wrap_sqrt2 );
    def( "sqrt_real", wrap_sqrt_real );
    def( "pow", wrap_pow1 );
    def( "pow", wrap_pow2 );
    def( "pow", wrap_pow3 );
    def( "pow", wrap_pow4 );
    def( "pow", wrap_pow5 );
    def( "exp", wrap_exp );
    def( "log", wrap_log );
    def( "log10", wrap_log10 );
    def( "log_b", wrap_log_b );
    def( "sin", wrap_sin );
    def( "cos", wrap_cos );
    def( "tan", wrap_tan );
    def( "sec", wrap_sec );
    def( "csc", wrap_csc );
    def( "cot", wrap_cot );
    def( "arcsin", wrap_arcsin );
    def( "arcsin_real", wrap_arcsin_real );
    def( "arccos", wrap_arccos );
    def( "arccos_real", wrap_arccos_real );
    def( "arctan", wrap_arctan );
    def( "arcsec", wrap_arcsec );
    def( "arcsec_real", wrap_arcsec_real );
    def( "arccsc", wrap_arccsc );
    def( "arccsc_real", wrap_arccsc_real );
    def( "arccot", wrap_arccot );
    def( "sinh", wrap_sinh );
    def( "cosh", wrap_cosh );
    def( "tanh", wrap_tanh );
    def( "sech", wrap_sech );
    def( "csch", wrap_csch );
    def( "coth", wrap_coth );
    def( "arcsinh", wrap_arcsinh );
    def( "arccosh", wrap_arccosh );
    def( "arccosh_real", wrap_arccosh_real );
    def( "arctanh", wrap_arctanh );
    def( "arctanh_real", wrap_arctanh_real );
    def( "arcsech", wrap_arcsech );
    def( "arccsch", wrap_arccsch );
    def( "arccoth", wrap_arccoth );

}

}
