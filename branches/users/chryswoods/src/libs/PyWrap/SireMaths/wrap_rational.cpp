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
