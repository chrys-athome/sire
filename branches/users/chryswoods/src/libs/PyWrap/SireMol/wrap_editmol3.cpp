
/**
  * This file contains the boost::python wrapping of EditMol
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/editmol.h"
#include "SireMol/editres.h"
#include "SireMol/bond.h"
#include "SireMol/angle.h"
#include "SireMol/dihedral.h"
#include "SireMol/improper.h"
#include "SireMol/moleculebonds.h"
#include "SireMol/residuebonds.h"
#include "SireMol/atom.h"
#include "SireMol/weightfunction.h"
#include "SireMol/atomidgroup.h"

#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"
#include "SireMaths/angle.h"
#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"


#include "SirePy/indexer.hpp"

// using namespace boost::python;
//
// namespace SireMol
// {
//
// void export_EditMol3(class_<EditMol> &wrapper)
// {
//     ResNumList (EditMol::*wrap_residueNumbers1)() const = &EditMol::residueNumbers;
//     ResNumList (EditMol::*wrap_residueNumbers2)(const QString&) const = &EditMol::residueNumbers;
//
//     EditRes (EditMol::*wrap_residue1)(ResNum) = &EditMol::residue;
//     EditRes (EditMol::*wrap_residue2)(const QString&) = &EditMol::residue;
//
//     AtomSet (EditMol::*wrap_atoms1)() const = &EditMol::atoms;
//     AtomSet (EditMol::*wrap_atoms2)(ResNum) const = &EditMol::atoms;
//
//     AtomVector (EditMol::*wrap_atomVector1)() const = &EditMol::atomVector;
//     AtomVector (EditMol::*wrap_atomVector2)(ResNum) const = &EditMol::atomVector;
//
//     bool (EditMol::*wrap_contains1)(const AtomIndex&) const = &EditMol::contains;
//     bool (EditMol::*wrap_contains2)(ResNum) const = &EditMol::contains;
//     bool (EditMol::*wrap_contains3)(const QString&) const = &EditMol::contains;
//
//     double (EditMol::*wrap_measure1)(const Bond&) const = &EditMol::measure;
//     SireMaths::Angle (EditMol::*wrap_measure2)(const Angle&) const = &EditMol::measure;
//     SireMaths::Angle (EditMol::*wrap_measure3)(const Dihedral&) const = &EditMol::measure;
//     SireMaths::Angle (EditMol::*wrap_measure4)(const Improper&) const = &EditMol::measure;
//
//     BondList (EditMol::*wrap_bonds1)() const = &EditMol::bonds;
//     BondList (EditMol::*wrap_bonds2)(const AtomIndex&) const = &EditMol::bonds;
//     BondList (EditMol::*wrap_bonds3)(ResNum) const = &EditMol::bonds;
//
//     MoleculeBonds (EditMol::*wrap_connectivity1)() const = &EditMol::connectivity;
//     ResidueBonds (EditMol::*wrap_connectivity2)(ResNum) const = &EditMol::connectivity;
//
//     double (EditMol::*wrap_getWeight1)(const AtomIDGroup&, const AtomIDGroup&) const
//                         = &EditMol::getWeight;
//     double (EditMol::*wrap_getWeight2)(const AtomIDGroup&, const AtomIDGroup&,
//                      const WeightFunction&) const = &EditMol::getWeight;
//
//     wrapper
//                //Query functions
//                .def("name", &EditMol::name)
//                .def("toString", &EditMol::toString)
//                .def("count", &EditMol::count)
//                .def("size", &EditMol::size)
//                .def("nResidues", &EditMol::nResidues)
//                .def("nBonds", &EditMol::nBonds)
//                .def("at", &EditMol::at)
//                .def("residueName", &EditMol::residueName)
//                .def("residueNumbers", wrap_residueNumbers1)
//                .def("residueNumbers", wrap_residueNumbers2)
//                .def("residueNames", &EditMol::residueNames)
//                .def("isEmpty", &EditMol::isEmpty)
//                .def("nAtoms", &EditMol::nAtoms)
//                .def("residue", wrap_residue1)
//                .def("residue", wrap_residue2)
//                .def("atoms", wrap_atoms1)
//                .def("atoms", wrap_atoms2)
//                .def("atomVector", wrap_atomVector1)
//                .def("atomVector", wrap_atomVector2)
//                .def("contains", wrap_contains1)
//                .def("contains", wrap_contains2)
//                .def("contains", wrap_contains3)
//                .def("atom", &EditMol::atom, return_value_policy<copy_const_reference>())
//                .def("bond", &EditMol::bond)
//                .def("angle", &EditMol::angle)
//                .def("dihedral", &EditMol::dihedral)
//                .def("improper", &EditMol::improper)
//                .def("measure", wrap_measure1)
//                .def("measure", wrap_measure2)
//                .def("measure", wrap_measure3)
//                .def("measure", wrap_measure4)
//                .def("bonds", wrap_bonds1)
//                .def("bonds", wrap_bonds2)
//                .def("bonds", wrap_bonds3)
//                .def("connectivity", wrap_connectivity1)
//                .def("connectivity", wrap_connectivity2)
//                .def("signature", &EditMol::signature)
//                .def("getWeight", wrap_getWeight1)
//                .def("getWeight", wrap_getWeight2)
//                .def("center", &EditMol::center)
//       ;
// }
//
// }
