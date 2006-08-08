
/**
  * This file contains the boost::python wrapping of Rational and Irrational
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>

#include "SireMaths/rational.h"

using namespace boost::python;

namespace SireMaths
{

void SIREMATHS_EXPORT export_Rational()
{
    class_<Rational>("Rational", init<>())
          .def(init<int>())
          .def(init<int,int>())
          .def(init<const Rational&>())
          .def(self == self)
          .def(self != self)
          .def(self == int())
          .def(self != int())
          .def(self + self)
          .def(self - self)
          .def(self * self)
          .def(self / self)
          .def(self + int())
          .def(int() + self)
          .def(self - int())
          .def(int() - self)
          .def(self * int())
          .def(int() * self)
          .def(self / int())
          .def(int() / self)
          .def(self += self)
          .def(self -= self)
          .def(self *= self)
          .def(self /= self)
          .def(self += int())
          .def(self -= int())
          .def(self *= int())
          .def(self /= int())
          .def(self > self)
          .def(self >= self)
          .def(self < self)
          .def(self <= self)
          .def(-self)
          .def(self > self)
          .def(self >= self)
          .def(self < self)
          .def(self <= self)
          .def(int() > self)
          .def(int() >= self)
          .def(int() < self)
          .def(int() <= self)
          .def(self > int())
          .def(self >= int())
          .def(self < int())
          .def(self <= int())
          .def("__str__", &SireMaths::toString)
          .def("toString", &SireMaths::toString)
          .def("numerator", &Rational::numerator)
          .def("denominator", &Rational::denominator)
    ;
    
    bool (*wrap_isRational1)(double, int) = &isRational;
    bool (*wrap_isRational2)(double) = &isRational;
    Rational (*wrap_toRational1)(double, int) = &toRational;
    Rational (*wrap_toRational2)(double) = &toRational;
    
    def("isRational", wrap_isRational1);
    def("toRational", wrap_toRational1);
    def("isRational", wrap_isRational2);
    def("toRational", wrap_toRational2);
    
    def("isInteger", &isInteger);

    def("toDouble", &toDouble);
}

}
