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
