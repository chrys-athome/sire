
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
export_EnergyPack()
{
    //we can't wrap up all combinations of the EnergyPack as python doesn't have templates...
    //instead we will wrap up an EnergyPack that contains all Energy components...
    
    class_<EnergyPackAll, bases<EnergyBase> >("EnergyPack", init<>())
                        .def( self += CoulombEnergy() )
                        .def( self -= CoulombEnergy() )
                        .def( self += LJEnergy() )
                        .def( self -= LJEnergy() )
                        .def( self += BondEnergy() )
                        .def( self -= BondEnergy() )
                        .def( self += AngleEnergy() )
                        .def( self -= AngleEnergy() )
                        .def( self += DihedralEnergy() )
                        .def( self -= DihedralEnergy() )
                        .def( self += HBondEnergy() )
                        .def( self -= HBondEnergy() )
                        .def( self += IntraCoulombEnergy() )
                        .def( self -= IntraCoulombEnergy() )
                        .def( self += IntraLJEnergy() )
                        .def( self -= IntraLJEnergy() )
                        .def( self += QMEnergy() )
                        .def( self -= QMEnergy() )
                        .def("toString", &EnergyPackAll::toString)
                        .def("coulomb", &EnergyPackAll::get<Coulomb>,
                                       return_value_policy<copy_const_reference>())
                        .def("lj", &EnergyPackAll::get<LJ>, 
                                       return_value_policy<copy_const_reference>())
                        .def("bond", &EnergyPackAll::get<Bond>, 
                                       return_value_policy<copy_const_reference>())
                        .def("angle", &EnergyPackAll::get<Angle>, 
                                       return_value_policy<copy_const_reference>())
                        .def("dihedral", &EnergyPackAll::get<Dihedral>, 
                                       return_value_policy<copy_const_reference>())
                        .def("hbond", &EnergyPackAll::get<HBond>, 
                                       return_value_policy<copy_const_reference>())
                        .def("intraCoulomb", &EnergyPackAll::get<IntraCoulomb>, 
                                       return_value_policy<copy_const_reference>())
                        .def("intraLJ", &EnergyPackAll::get<IntraLJ>, 
                                       return_value_policy<copy_const_reference>())
                        .def("QM", &EnergyPackAll::get<QM>, 
                                       return_value_policy<copy_const_reference>())
                                       
    ;
    
}

}
