
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

void  
SIREFF_EXPORT
export_EnergyType()
{
    enum_<EnergyType::EnergyType>("EnergyType")
                           .value("Coulomb", Coulomb)
                           .value("LJ", LJ)
                           .value("Bond", Bond)
                           .value("Angle", Angle)
                           .value("Dihedral", Dihedral)
                           .value("HBond", HBond)
                           .value("IntraCoulomb", IntraCoulomb)
                           .value("IntraLJ", IntraLJ)
                           .value("QM", QM)
    ;
}

}
