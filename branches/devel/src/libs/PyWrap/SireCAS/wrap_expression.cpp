
/**
  * This file contains the boost::python wrapping of Expression
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>

#include "SireCAS/qhash_sirecas.h"

#include <QString>

#include "SireCAS/expression.h"
#include "SireCAS/identities.h"
#include "SireCAS/symbol.h"
#include "SireCAS/symbols.h"
#include "SireCAS/functions.h"
#include "SireCAS/values.h"
#include "SireCAS/expressions.h"
#include "SireCAS/complexvalues.h"

#include "SirePy/str.hpp"

using namespace boost::python;
using namespace SirePy;

namespace SireCAS
{

QString wrap_what(const ExpressionBase &ex)
{
    return ex.what();
}

void export_Operators(class_<Expression> &wrapper);

Expression wrap_diff1(const Expression &ex, const Symbol &symbol)
{
    return ex.differentiate(symbol);
}

Expression wrap_diff2(const Expression &ex, const Symbol &symbol, int level)
{
    return ex.differentiate(symbol,level);
}

Expression wrap_diff3(const Expression &ex, const Symbol &symbol)
{
    return ex.diff(symbol);
}

Expression wrap_diff4(const Expression &ex, const Symbol &symbol, int level)
{
    return ex.diff(symbol,level);
}

Expression wrap_simplify1(const Expression &ex)
{
    return ex.simplify();
}

Expression wrap_simplify2(const Expression &ex, int options)
{
    return ex.simplify(options);
}

void  
SIRECAS_EXPORT
export_Expression()
{
    Expression (Expression::*wrap_multiply1)(const Expression&) const = &Expression::multiply;
    Expression (Expression::*wrap_multiply2)(double) const = &Expression::multiply;
    Expression (Expression::*wrap_multiply3)(const Complex&) const = &Expression::multiply;
    
    Expression (Expression::*wrap_divide1)(double) const = &Expression::divide;
    Expression (Expression::*wrap_divide2)(const Expression&) const = &Expression::divide;
    Expression (Expression::*wrap_divide3)(const Complex&) const = &Expression::divide;
    
    Expression (Expression::*wrap_pow1)(const Rational&) const = &Expression::pow;    
    Expression (Expression::*wrap_pow2)(int) const = &Expression::pow;
    Expression (Expression::*wrap_pow3)(double) const = &Expression::pow;
    Expression (Expression::*wrap_pow4)(const Expression&) const = &Expression::pow;
    Expression (Expression::*wrap_pow5)(const Complex&) const = &Expression::pow;

    double (Expression::*wrap_evaluate1)(const Values&) const = &Expression::evaluate;
    Complex (Expression::*wrap_evaluate2)(const ComplexValues&) const 
                          = &Expression::evaluate;
    
    class_<Expression> wrapper("Expression", init<>());
    
    wrapper
        .def( init<int>() )
        .def( init<const Rational&>() )
        .def( init<double>() )
        .def( init<const Complex&>() )
        .def( init<const ExpressionBase&>() )
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
        .def( self == self )
        .def( self != self )
        .def( -self )
        .def( "__str__", &__str__<Expression>)
        .def( "add", &Expression::add )
        .def( "subtract", &Expression::subtract )
        .def( "multiply", wrap_multiply1 )
        .def( "multiply", wrap_multiply2 )
        .def( "multiply", wrap_multiply3 )
        .def( "divide", wrap_divide1 )
        .def( "divide", wrap_divide2 )
        .def( "divide", wrap_divide3 )
        .def( "negate", &Expression::negate )
        .def( "invert", &Expression::invert )
        .def( "conjugate", &Expression::conjugate )
        .def( "pow", wrap_pow1 )
        .def( "pow", wrap_pow2 )
        .def( "pow", wrap_pow3 )
        .def( "pow", wrap_pow4 )
        .def( "pow", wrap_pow5 )
        .def( "squared", &Expression::squared )
        .def( "cubed", &Expression::cubed )
        .def( "root", &Expression::root )
        .def( "substitute", &Expression::substitute )
        .def( "simplify", &wrap_simplify1 )
        .def( "simplify", &wrap_simplify2 )
        .def( "expand", &Expression::expand )
        .def( "collapse", &Expression::collapse )
        .def( "evaluate", wrap_evaluate1 )
        .def( "evaluate", wrap_evaluate2 )
        .def( "differentiate", &wrap_diff1 )
        .def( "differentiate", &wrap_diff2 )
        .def( "diff", &wrap_diff3 )
        .def( "diff", &wrap_diff4 )
        .def( "integrate", &Expression::integrate )
        .def( "integ", &Expression::integ )
        .def( "series", &Expression::series )
        .def( "isZero", &Expression::isZero )
        .def( "isConstant", &Expression::isConstant )
        .def( "isFunction", &Expression::isFunction )
        .def( "isCompound", &Expression::isCompound )
        .def( "isComplex", &Expression::isComplex )
        .def( "toString", &Expression::toString )
        .def( "hash", &Expression::hash )
        .def( "base", &Expression::base, return_value_policy<copy_const_reference>() )
        .def( "factor", &Expression::factor )
        .def( "symbols", &Expression::symbols )
        .def( "functions", &Expression::functions )
        .def( "children", &Expression::children )
    ;

    Expression (*wrap_SireCAS_pow1)(const Expression&, int) = &pow;
    Expression (*wrap_SireCAS_pow2)(const Expression&, const Rational&) = &pow;
    Expression (*wrap_SireCAS_pow3)(const Expression&, double) = &pow;
    Expression (*wrap_SireCAS_pow4)(const Expression&, const Expression&) = &pow;
    Expression (*wrap_SireCAS_pow5)(const Expression&, const Complex&) = &pow;
            
    def("pow", wrap_SireCAS_pow1);
    def("pow", wrap_SireCAS_pow2);
    def("pow", wrap_SireCAS_pow3);
    def("pow", wrap_SireCAS_pow4);
    def("pow", wrap_SireCAS_pow5);
    
    Expression (*wrap_SireCAS_root)(const Expression&, int) = &root;
    Expression (*wrap_SireCAS_sqrt)(const Expression&) = &sqrt;
    Expression (*wrap_SireCAS_cbrt)(const Expression&) = &cbrt;
    
    def("root", wrap_SireCAS_root);
    def("sqrt", wrap_SireCAS_sqrt);
    def("cbrt", wrap_SireCAS_cbrt);
    
    export_Operators(wrapper);
}

}
