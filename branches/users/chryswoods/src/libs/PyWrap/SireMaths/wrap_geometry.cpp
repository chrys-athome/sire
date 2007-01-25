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
  * This file contains the boost::python wrapping of the geometry types (Line, Triange and Torsion)
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SirePy/indexer.hpp"

#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

using namespace boost::python;

namespace SireMaths
{

void SIREMATHS_EXPORT export_Geometry()
{
    /** Wrap up the Line class */
    class_<Line>("Line", init<>())
            .def(init<const Vector&, const Vector&>())
            .def("toString", &Line::toString)
            .def("length", &Line::length)
            .def("vector", &Line::vector)
            .def("count", &Line::count)
            .def("point", &Line::point, return_value_policy<copy_const_reference>())
            .def("at", &Line::at, return_value_policy<copy_const_reference>())
            .def("__getitem__",&readonly_indexer<Line, Vector>::__getitem__)
            .def("__len__", &Line::count)
    ;
    
    /** Wrap up the triangle class */
    class_<Triangle>("Triangle", init<>())
                .def(init<const Vector&, const Vector&, const Vector&>())
                .def("toString", &Triangle::toString)
                .def("angle", &Triangle::angle)
                .def("angle0", &Triangle::angle0)
                .def("angle1", &Triangle::angle1)
                .def("angle2", &Triangle::angle2)
                .def("vector", &Triangle::vector)
                .def("vector0", &Triangle::vector0)
                .def("vector1", &Triangle::vector1)
                .def("vector2", &Triangle::vector2)
                .def("count", &Triangle::count)
                .def("point", &Triangle::point, return_value_policy<copy_const_reference>())
                .def("at", &Triangle::at, return_value_policy<copy_const_reference>())
                .def("__getitem__",&readonly_indexer<Triangle, Vector>::__getitem__)
                .def("__len__", &Triangle::count)
    ;
    
    /** Wrap up the torsion class */
    class_<Torsion>("Torsion", init<>())
               .def(init<const Vector&, const Vector&, const Vector&, const Vector&>())
               .def("toString", &Torsion::toString)
               .def("angle", &Torsion::angle)
               .def("line03", &Torsion::line03)
               .def("line12", &Torsion::line12)
               .def("vector03", &Torsion::vector03)
               .def("vector12", &Torsion::vector12)
               .def("triangle1", &Torsion::triangle1)
               .def("triangle2", &Torsion::triangle2)
               .def("count", &Torsion::count)
               .def("point", &Torsion::point, return_value_policy<copy_const_reference>())
               .def("at", &Torsion::at, return_value_policy<copy_const_reference>())
               .def("__getitem__",&readonly_indexer<Torsion, Vector>::__getitem__)
               .def("__len__", &Torsion::count)
    ;               
                
}

}
