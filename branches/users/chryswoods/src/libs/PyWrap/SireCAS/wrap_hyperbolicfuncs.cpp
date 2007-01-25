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
  * This file contains the boost::python wrapping of the hyperbolic trig functions
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireCAS/hyperbolicfuncs.h"
#include "SireCAS/invhyperbolicfuncs.h"

using namespace boost::python;

namespace SireCAS
{

void  
SIRECAS_EXPORT
export_HyperbolicFuncs()
{

    class_<Cosh, bases<SingleFunc> >("Cosh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<Sinh, bases<SingleFunc> >("Sinh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Tanh, bases<SingleFunc> >("Tanh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Csch, bases<SingleFunc> >("Csch", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Sech, bases<SingleFunc> >("Sech", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Coth, bases<SingleFunc> >("ArcCoth", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<ArcCosh, bases<SingleFunc> >("ArcCosh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<ArcSinh, bases<SingleFunc> >("ArcSinh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcTanh, bases<SingleFunc> >("ArcTanh", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcCsch, bases<SingleFunc> >("ArcCsch", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcSech, bases<SingleFunc> >("ArcSech", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcCoth, bases<SingleFunc> >("ArcCoth", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

}

}
