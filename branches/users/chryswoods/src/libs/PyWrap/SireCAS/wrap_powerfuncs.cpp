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
  * This file contains the boost::python wrapping of the power functions (incl. Exp and Ln)
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireCAS/power.h"
#include "SireCAS/exp.h"
#include "SireCAS/powerconstant.h"

using namespace boost::python;

namespace SireCAS
{

void  
SIRECAS_EXPORT
export_PowerFuncs()
{
    class_<PowerFunction, bases<ExBase>, boost::noncopyable>("PowerFunction", no_init)
        .def("core", &PowerFunction::core)
        .def("power", &PowerFunction::power)
        .def("reduce", &PowerFunction::reduce)
    ;

    class_<Power, bases<PowerFunction> >("Power", init<>())
        .def( init<const ExBase&, const ExBase&>() )
        .def( init<const Expression&, const Expression&>() )
        .def( init<const Expression&, const ExBase&>() )
        .def( init<const ExBase&, const Expression&>() )
    ;
    
    class_<PowerConstant, bases<PowerFunction> >("PowerConstant", init<>())
        .def( init<double, const Expression&>() )
        .def( init<double, const ExBase&>() )
    ;
    
    class_<ConstantPower, bases<PowerFunction>, boost::noncopyable >("ConstantPower", no_init)
    ;
    
    class_<IntegerPower, bases<ConstantPower> >("IntegerPower", init<>())
        .def( init<const ExBase&, int>() )
        .def( init<const Expression&, int>() )
    ;
    
    class_<RationalPower, bases<ConstantPower> >("RationalPower", init<>())
        .def( init<const ExBase&, const Rational&>() )
        .def( init<const Expression&, const Rational&>() )
    ;
    
    class_<RealPower, bases<ConstantPower> >("RealPower", init<>())
        .def( init<const ExBase&, double>() )
        .def( init<const Expression&, double>() )
    ;
    
    class_<ComplexPower, bases<ConstantPower> >("ComplexPower", init<>())
        .def( init<const ExBase&, const Complex&>() )
        .def( init<const Expression&, const Complex&>() )
    ;
    
    class_<Exp, bases<PowerFunction> >("Exp", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Ln, bases<SingleFunc> >("Ln", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

}

}
