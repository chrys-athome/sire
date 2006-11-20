
/**
  * This file contains the boost::python wrapping of the energy classes
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireFF/energytypes.h"

using namespace boost::python;
using namespace SireFF;
using namespace SireFF::EnergyType;

namespace SireFF
{

template<class T>
void wrapEnergy(const char *name)
{
    class_<T, bases<EnergyBase> >(name, init<>())
                             .def(init<double>())
                             .def( self += self )
                             .def( self -= self )
                             .def("toString", &T::toString)
    ;
}

void  
SIREFF_EXPORT
export_Energy()
{
    wrapEnergy<CoulombEnergy>("CoulombEnergy");
    wrapEnergy<LJEnergy>("LJEnergy");
    wrapEnergy<BondEnergy>("BondEnergy");
    wrapEnergy<AngleEnergy>("AngleEnergy");
    wrapEnergy<DihedralEnergy>("DihedralEnergy");
    wrapEnergy<HBondEnergy>("HBondEnergy");
    wrapEnergy<IntraCoulombEnergy>("IntraCoulombEnergy");
    wrapEnergy<IntraLJEnergy>("IntraLJEnergy");
    wrapEnergy<QMEnergy>("QMEnergy");
}

}
