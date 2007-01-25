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
