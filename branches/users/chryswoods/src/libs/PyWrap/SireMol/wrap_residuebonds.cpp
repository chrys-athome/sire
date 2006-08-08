
/**
  * This file contains the boost::python wrapping of ResidueBonds
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/residuebonds.h"
#include "SireMol/bond.h"

using namespace boost::python;

namespace SireMol
{

void export_ResidueBonds()
{
    
    ResNumList (ResidueBonds::*wrap_residuesBondedTo1)() const
                      = &ResidueBonds::residuesBondedTo;
    ResNumList (ResidueBonds::*wrap_residuesBondedTo2)(const QString&) const
                      = &ResidueBonds::residuesBondedTo;
    
    BondList (ResidueBonds::*wrap_bonds1)() const = &ResidueBonds::bonds;
    BondList (ResidueBonds::*wrap_bonds2)(ResNum) const = &ResidueBonds::bonds;
    
    BondList (ResidueBonds::*wrap_interBonds1)(const QString&) const
                      = &ResidueBonds::interBonds;
    BondList (ResidueBonds::*wrap_interBonds2)() const
                      = &ResidueBonds::interBonds;
    
    BondList (ResidueBonds::*wrap_intraBonds1)(const QString&) const = &ResidueBonds::intraBonds;
    BondList (ResidueBonds::*wrap_intraBonds2)() const = &ResidueBonds::intraBonds;
    
    bool (ResidueBonds::*wrap_contains1)(const QString&) const = &ResidueBonds::contains;
    bool (ResidueBonds::*wrap_contains2)(const Bond&) const = &ResidueBonds::contains;
    
    
    class_<ResidueBonds>("ResidueBonds", init<>())
        .def(init<ResNum>())
        .def(init<const ResidueBonds&>())
        .def(self == self)
        .def(self != self)
        .def("__str__", &ResidueBonds::toString)
        .def("isEmpty", &ResidueBonds::isEmpty)
        .def("resNum", &ResidueBonds::resNum)
        .def("residuesBondedTo", wrap_residuesBondedTo1)
        .def("residuesBondedTo", wrap_residuesBondedTo2)
        .def("bondedTo", &ResidueBonds::bondedTo)
        .def("bonded", &ResidueBonds::bonded)
        .def("interBonds", wrap_interBonds1)
        .def("interBonds", wrap_interBonds2)        
        .def("intraBonds", wrap_intraBonds1)
        .def("intraBonds", wrap_intraBonds2)
        .def("asymmetricBonds", &ResidueBonds::asymmetricBonds)
        .def("bonds", wrap_bonds1)
        .def("bonds", wrap_bonds2)
        .def("nBonds", &ResidueBonds::nBonds)
        .def("nAsymmetricBonds", &ResidueBonds::nAsymmetricBonds)
        .def("anchors", &ResidueBonds::anchors)
        .def("atoms", &ResidueBonds::atoms)
        .def("contains", wrap_contains1)
        .def("contains", wrap_contains2)
    ;
    
}

}
