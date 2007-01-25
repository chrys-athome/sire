/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

/**
  * This file contains the boost::python wrapping of the ExpressionBase
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireCAS/expressionbase.h"
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

using namespace boost::python;

namespace SireCAS
{

Expression wrap_simplify1(const ExpressionBase &ex, int options)
{
    return ex.simplify(options);
}
    
Expression wrap_simplify2(const ExpressionBase &ex) 
{
    return ex.simplify();
}
    
void  
SIRECAS_EXPORT
export_ExpressionBase()
{
    class_<ExpressionBase> wrapper("ExpressionBase", init<>());
    
    double (ExpressionBase::*wrap_evaluate1)(const Values&) const
                              = &ExpressionBase::evaluate;
    Complex (ExpressionBase::*wrap_evaluate2)(const ComplexValues&) const 
                              = &ExpressionBase::evaluate;
    
    wrapper
        .def( self == self )
        .def( self != self )
        .def( "differentiate", &ExpressionBase::differentiate )
        .def( "integrate", &ExpressionBase::integrate )
        .def( "series", &ExpressionBase::series )
        .def( "simplify", wrap_simplify1 )
        .def( "simplify", wrap_simplify2 )
        .def( "expand", &ExpressionBase::expand )
        .def( "collapse", &ExpressionBase::collapse )
        .def( "conjugate", &ExpressionBase::conjugate )
        .def( "isFunction", &ExpressionBase::isFunction )
        .def( "isConstant", &ExpressionBase::isConstant )
        .def( "isComplex", &ExpressionBase::isComplex )
        .def( "isCompound", &ExpressionBase::isCompound )
        .def( "hash", &ExpressionBase::hash )
        .def( "what", &ExpressionBase::what )
        .def( "toString", &ExpressionBase::toString )
        .def( "__str__", &ExpressionBase::toString )
        .def( "evaluate", wrap_evaluate1 )
        .def( "evaluate", wrap_evaluate2 )
        .def( "substitute", &ExpressionBase::substitute )
        .def( "symbols", &ExpressionBase::symbols )
        .def( "functions", &ExpressionBase::functions )
        .def( "children", &ExpressionBase::children )
    ;
}

}
