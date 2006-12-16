
/**
  * This file contains the boost::python wrapping of the energy classes
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireFF/energytypes.h"

#include <boost/shared_ptr.hpp>

using namespace boost::python;
using namespace SireFF;
using namespace SireFF::EnergyType;

namespace SireFF
{

void  
SIREFF_EXPORT
export_EnergyBase()
{
    class_<EnergyBase>("EnergyBase", no_init)
                  .def("toString", &EnergyBase::toString)
                  .def("value", &EnergyBase::value)
                  .def( self + self )
                  .def( self - self )
                  .def( self * self )
                  .def( self / self )
                  .def( Energy() == self )
                  .def( Energy() != self )
                  .def( self == Energy() )
                  .def( self != Energy() )
    ;

    class_<Energy, bases<EnergyBase> >("Energy", init<>())
                                  .def(init<double>())
    ;
}

}
