
/**
  * This file contains the boost::python wrapping of Expression operators 
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
#include "SireCAS/values.h"
#include "SireCAS/expressions.h"
#include "SireCAS/exbase.h"

using namespace boost::python;

namespace SireCAS
{

void  
SIRECAS_EXPORT
export_ExBase_Operators(class_<ExBase, boost::noncopyable> &wrapper)
{
    wrapper
        .def( -self )
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
        .def( self + Rational() )
        .def( self - Rational() )
        .def( self * Rational() )
        .def( self / Rational() )
        .def( Rational() + self )
        .def( Rational() - self )
        .def( Rational() * self )
        .def( Rational() / self )
        .def( self + Complex() )
        .def( self - Complex() )
        .def( self * Complex() )
        .def( self / Complex() )
        .def( Complex() + self )
        .def( Complex() - self )
        .def( Complex() * self )
        .def( Complex() / self )
        .def( self + Expression() )
        .def( self - Expression() )
        .def( self * Expression() )
        .def( self / Expression() )
        .def( Expression() + self )
        .def( Expression() - self )
        .def( Expression() * self )
        .def( Expression() / self )
    ;
}

void  
SIRECAS_EXPORT
export_Operators(class_<Expression> &wrapper)
{
    wrapper
        .def( -self )
        .def( self + self )
        .def( self - self )
        .def( self * self )
        .def( self / self )
        .def( self += self )
        .def( self -= self )
        .def( self *= self )
        .def( self /= self )
        .def( self + double() )
        .def( self - double() )
        .def( self * double() )
        .def( self / double() )
        .def( double() + self )
        .def( double() - self )
        .def( double() * self )
        .def( double() / self )
        .def( self + Rational() )
        .def( self - Rational() )
        .def( self * Rational() )
        .def( self / Rational() )
        .def( Rational() + self )
        .def( Rational() - self )
        .def( Rational() * self )
        .def( Rational() / self )
        .def( self + Complex() )
        .def( self - Complex() )
        .def( self * Complex() )
        .def( self / Complex() )
        .def( Complex() + self )
        .def( Complex() - self )
        .def( Complex() * self )
        .def( Complex() / self )
    ;
}

}
