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
  * This file contains the boost::python wrapping of the trigonometry functions
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireCAS/trigfuncs.h"
#include "SireCAS/invtrigfuncs.h"

using namespace boost::python;

namespace SireCAS
{

void  
SIRECAS_EXPORT
export_TrigFuncs()
{

    class_<SingleFunc, bases<ExBase>, boost::noncopyable>("SingleFunc", no_init)
        .def( "argument", &SingleFunc::argument, 
                        return_value_policy<copy_const_reference>() )
    ;
    
    class_<Cos, bases<SingleFunc> >("Cos", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<Sin, bases<SingleFunc> >("Sin", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Tan, bases<SingleFunc> >("Tan", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Csc, bases<SingleFunc> >("Csc", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Sec, bases<SingleFunc> >("Sec", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<Cot, bases<SingleFunc> >("Cot", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<ArcCos, bases<SingleFunc> >("ArcCos", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

    class_<ArcSin, bases<SingleFunc> >("ArcSin", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcTan, bases<SingleFunc> >("ArcTan", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcCsc, bases<SingleFunc> >("ArcCsc", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcSec, bases<SingleFunc> >("ArcSec", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;
    
    class_<ArcCot, bases<SingleFunc> >("ArcCot", init<>())
        .def( init<const ExBase&>() )
        .def( init<const Expression&>() )
    ;

}

}
