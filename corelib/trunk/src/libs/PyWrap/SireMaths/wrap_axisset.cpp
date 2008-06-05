
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
