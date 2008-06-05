
/**
  * This file contains the boost::python wrapping of an Angle
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>

#include "SireMaths/angle.h"

using namespace boost::python;

namespace SireMaths
{

void SIREMATHS_EXPORT export_Angle()
{
    Angle (*staticDegrees)(double) = &Angle::degrees;

    class_<Angle>("Angle", init<>())
             .def(init<double>())
             .def("toRadians", &Angle::toRadians, return_value_policy<copy_const_reference>())
             .def("toDegrees", &Angle::toDegrees)
             .def("setDegrees", &Angle::setDegrees)
             .def("setRadians", &Angle::setRadians)
             .def("isWithin", &Angle::isWithin)
             .def("toString", &Angle::toString)
             .def(self == self)
             .def(self != self)
             .def(self + self)
             .def(self - self)
             .def(self * double())
             .def(double() * self)
             .def(self / double())
             .def(self += self)
             .def(self -= self)
             .def(self *= double())
             .def(self /= double())
             .def(self > self)
             .def(self >= self)
             .def(self < self)
             .def(self <= self)
             .def("degrees", staticDegrees).staticmethod("degrees")
    ;
}

}
