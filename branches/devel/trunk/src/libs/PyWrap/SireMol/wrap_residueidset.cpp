
/**
  * This file contains the boost::python wrapping of a ResidueIDSet (and ResidueID)
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/residueidset.h"

#include "../SireBase/wrap_set.h"

using namespace boost::python;

namespace SireMol
{

void export_ResidueIDSet()
{
    class_<ResidueID>("ResidueID", init<>())
          .def("name", &ResidueID::name)
          .def("number", &ResidueID::number)
          .def("setName", &ResidueID::setName)
          .def("setNumber", &ResidueID::setNumber)
          .def(self == self)
          .def(self != self)
          .def("toString", &ResidueID::toString)
          .def("__str__", &ResidueID::toString);
    ;

    SireBase::wrapSet<ResNum,ResidueID>("ResidueIDSet");
}

}
