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
  * This file contains the boost::python wrapping of an AxisSet
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireMaths/vector.h"
#include "SireMaths/matrix.h"
#include "SireMaths/axisset.h"

using namespace boost::python;

namespace SireMaths
{

void SIREMATHS_EXPORT export_AxisSet()
{
    class_<Matrix>("Matrix", init<>())
              .def(init<const Vector&, const Vector&, const Vector&>())
              .def(init<const Matrix&>())
              .def("transpose", &Matrix::transpose)
              .def("inverse", &Matrix::inverse)
              .def("trace", &Matrix::trace)
              .def("toString", &Matrix::toString)
              .def("column0", &Matrix::column0)
              .def("column1", &Matrix::column1)
              .def("column2", &Matrix::column2)
              .def("row0", &Matrix::row0)
              .def("row1", &Matrix::row1)
              .def("row2", &Matrix::row2)
              .def("determinant", &Matrix::determinant)
              .def("setToIdentity", &Matrix::setToIdentity)
              .def("isSymmetric", &Matrix::isSymmetric)
              .def("enforceSymmetric", &Matrix::enforceSymmetric)
              .def("getPrincipalAxes", &Matrix::getPrincipalAxes)
              .def(self == self)
              .def(self != self)
              .def(self += self)
              .def(self -= self)
              .def(self *= self)
              .def(self *= double())
              .def(self /= double())
              .def(self + self)
              .def(self - self)
              .def(self * self)
              .def(self * Vector())
              .def(self * double())
              .def(double() * self)
    ;
    
    class_<AxisSet>("AxisSet", init<>())
               .def(init<const Matrix&>())
               .def(init<const Matrix&, Vector>())
               .def(init<const AxisSet&>())
               .def("toString",&AxisSet::toString)
               .def("matrix",&AxisSet::matrix, return_value_policy<copy_const_reference>())
               .def("invMatrix",&AxisSet::invMatrix, return_value_policy<copy_const_reference>())
               .def("origin",&AxisSet::origin, return_value_policy<copy_const_reference>())
               .def("fromIdentity",&AxisSet::fromIdentity)
               .def("toIdentity",&AxisSet::toIdentity)
               .def("fromFrame",&AxisSet::fromFrame)
               .def("toFrame",&AxisSet::toFrame)
    ;
}

}
