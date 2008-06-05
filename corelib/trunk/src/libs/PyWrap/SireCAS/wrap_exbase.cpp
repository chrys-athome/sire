
/**
  * This file contains the boost::python wrapping of the ExBase hierarchy of classes
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireCAS/exbase.h"
#include "SireCAS/sum.h"
#include "SireCAS/product.h"
#include "SireCAS/constant.h"
#include "SireCAS/symbol.h"
#include "SireCAS/symbols.h"
#include "SireCAS/functions.h"
#include "SireCAS/values.h"
#include "SireCAS/identities.h"
#include "SireCAS/complexvalues.h"
#include "SireCAS/i.h"

#include "SirePy/str.hpp"

using namespace boost::python;
using namespace SirePy;

namespace SireCAS
{

Expression wrap_simplify1(const ExBase &ex, int options)
{
    return ex.simplify(options);
}
    
Expression wrap_simplify2(const ExBase &ex) 
{
    return ex.simplify();
}
    
QString wrap_what(const ExBase &ex)
{
    return QString(ex.what());
}

void export_ExBase_Operators(class_<ExBase, boost::noncopyable> &wrapper);

void export_ExBase()
{
    class_<ExBase, boost::noncopyable> wrapper("ExBase", no_init);
    
    double (ExBase::*wrap_evaluate1)(const Values&) const = &ExBase::evaluate;
    Complex (ExBase::*wrap_evaluate2)(const ComplexValues&) const = &ExBase::evaluate;
    
    wrapper
        .def( self == self )
        .def( self != self )
        
        .def( "__str__", &__str__<ExBase> )
        
        .def( "toExpressionBase", &ExBase::toExpressionBase )
        .def( "toExpression", &ExBase::toExpression )
        .def( "differentiate", &ExBase::differentiate )
        .def( "integrate", &ExBase::integrate )
        .def( "series", &ExBase::series )
        .def( "simplify", wrap_simplify1 )
        .def( "simplify", wrap_simplify2 )
        .def( "expand", &ExBase::expand )
        .def( "collapse", &ExBase::collapse )
        .def( "conjugate", &ExBase::conjugate )
        .def( "isFunction", &ExBase::isFunction )
        .def( "isConstant", &ExBase::isConstant )
        .def( "isComplex", &ExBase::isComplex )
        .def( "isCompound", &ExBase::isCompound )
        .def( "hash", &ExBase::hash )
        .def( "what", &wrap_what )
        .def( "toString", &ExBase::toString )
        .def( "evaluate", wrap_evaluate1 )
        .def( "evaluate", wrap_evaluate2 )
        .def( "substitute", &ExBase::substitute )
        .def( "symbols", &ExBase::symbols )
        .def( "functions", &ExBase::functions )
        .def( "children", &ExBase::children )
    ;
    
    export_ExBase_Operators(wrapper);
    
    Expression (*wrap_SireCAS_pow1)(const ExBase&, int) = &pow;
    Expression (*wrap_SireCAS_pow2)(const ExBase&, const Rational&) = &pow;
    Expression (*wrap_SireCAS_pow3)(const ExBase&, double) = &pow;
    Expression (*wrap_SireCAS_pow4)(const ExBase&, const Expression&) = &pow;
    Expression (*wrap_SireCAS_pow5)(const ExBase&, const Complex&) = &pow;
    Expression (*wrap_SireCAS_pow6)(const ExBase&, const ExBase&) = &pow;
            
    def("pow", wrap_SireCAS_pow1);
    def("pow", wrap_SireCAS_pow2);
    def("pow", wrap_SireCAS_pow3);
    def("pow", wrap_SireCAS_pow4);
    def("pow", wrap_SireCAS_pow5);
    def("pow", wrap_SireCAS_pow6);
    
    class_<Sum, bases<ExBase> >("Sum", init<>())
        .def( init<const Expression&, const Expression&>() )
        .def( init<const Expressions&>() )
        .def( "reduce", &Sum::reduce )
    ;
    
    class_<Product, bases<ExBase> >("Product", init<>())
        .def( init<const Expression&, const Expression&>() )
        .def( init<const Expressions&>() )
        .def( "reduce", &Product::reduce )
        .def( "numerator", &Product::numerator )
        .def( "denominator", &Product::denominator )
    ;
    
    class_<Constant, bases<ExBase> >("Constant", init<>())
    ;
    
    class_<I, bases<Constant> >("I", init<>())
    ;
    
}

}
