
/**
  * This file contains the boost::python wrapping of MoleculeBonds
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/moleculebonds.h"
#include "SireMol/bond.h"

using namespace boost::python;

namespace SireMol
{

void export_MoleculeBonds()
{
    void (MoleculeBonds::*wrap_add1)(const AtomIndex&, const AtomIndex&) 
                                      = &MoleculeBonds::add;
    void (MoleculeBonds::*wrap_add2)(ResNum, const QString&, const QString&)
                                      = &MoleculeBonds::add;
    void (MoleculeBonds::*wrap_add3)(const Bond&) = &MoleculeBonds::add;
    
    void (MoleculeBonds::*wrap_remove1)(const AtomIndex&, const AtomIndex&)
                                      = &MoleculeBonds::remove;
    void (MoleculeBonds::*wrap_remove2)(ResNum, const QString&, const QString&)
                                      = &MoleculeBonds::remove;
    void (MoleculeBonds::*wrap_remove3)(const Bond&)
                                      = &MoleculeBonds::remove;
    
    void (MoleculeBonds::*wrap_removeAll1)(const AtomIndex&) = &MoleculeBonds::removeAll;
    void (MoleculeBonds::*wrap_removeAll2)(ResNum) = &MoleculeBonds::removeAll;
    
    BondList (MoleculeBonds::*wrap_bonds1)() const = &MoleculeBonds::bonds;
    BondList (MoleculeBonds::*wrap_bonds2)(const AtomIndex&) const = &MoleculeBonds::bonds;
    BondList (MoleculeBonds::*wrap_bonds3)(ResNum) const = &MoleculeBonds::bonds;
    
    bool (MoleculeBonds::*wrap_contains1)(ResNum) const = &MoleculeBonds::contains;
    bool (MoleculeBonds::*wrap_contains2)(const AtomIndex&) const = &MoleculeBonds::contains;
    bool (MoleculeBonds::*wrap_contains3)(const Bond&) const = &MoleculeBonds::contains;
    
    bool (MoleculeBonds::*wrap_bonded1)(ResNum, ResNum) const = &MoleculeBonds::bonded;
    bool (MoleculeBonds::*wrap_bonded2)(const AtomIndex&, const AtomIndex&) const
                                     = &MoleculeBonds::bonded;
    
    
    class_<MoleculeBonds>("MoleculeBonds", init<>())
      .def(init<const MoleculeBonds&>())
      .def(self == self)
      .def(self != self)
      .def("__str__", &MoleculeBonds::toString)
      .def("add", wrap_add1)
      .def("add", wrap_add2)
      .def("add", wrap_add3)
      .def("isEmpty", &MoleculeBonds::isEmpty)
      .def("remove", wrap_remove1)
      .def("remove", wrap_remove2)
      .def("remove", wrap_remove3)
      .def("removeAll", wrap_removeAll1)
      .def("removeAll", wrap_removeAll2)
      .def("clear", &MoleculeBonds::clear)
      .def("finalise", &MoleculeBonds::finalise)
      .def("residue", &MoleculeBonds::residue)
      .def("bondedResidues", &MoleculeBonds::bondedResidues)
      .def("bonds", wrap_bonds1)
      .def("bonds", wrap_bonds2)
      .def("bonds", wrap_bonds3)
      .def("nBonds", &MoleculeBonds::nBonds)
      .def("contains", wrap_contains1)
      .def("contains", wrap_contains2)
      .def("contains", wrap_contains3)
      .def("bonded", wrap_bonded1)
      .def("bonded", wrap_bonded2)
      .def("resNumsBondedTo", &MoleculeBonds::resNumsBondedTo)
      .def("resNums", &MoleculeBonds::resNums)
      .def("atoms", &MoleculeBonds::atoms)
    ;
    
}

}
