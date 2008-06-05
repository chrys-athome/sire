
/**
  * This file contains the boost::python wrapping of a Molecule
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/moleculebonds.h"
#include "SireMol/residuebonds.h"
#include "SireMol/atomidgroup.h"
#include "SireMol/bond.h"
#include "SireMol/angle.h"
#include "SireMol/dihedral.h"
#include "SireMol/improper.h"
#include "SireMol/splitmolecule.h"
#include "SireMol/weightfunction.h"
#include "SireMol/moleculesignature.h"

#include "SireMaths/quaternion.h"
#include "SireMaths/angle.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

#include "SirePy/indexer.hpp"
#include "SirePy/str.hpp"

using namespace boost::python;
using namespace SirePy;

namespace SireMol
{

void export_Molecule2(class_<Molecule> &wrapper);

void export_Molecule()
{
    double (Molecule::*wrap_measure1)(const Bond&) const = &Molecule::measure;
    SireMaths::Angle (Molecule::*wrap_measure2)(const Angle&) const = &Molecule::measure;
    SireMaths::Angle (Molecule::*wrap_measure3)(const Dihedral&) const = &Molecule::measure;
    SireMaths::Angle (Molecule::*wrap_measure4)(const Improper&) const = &Molecule::measure;
    
    AtomSet (Molecule::*wrap_atoms1)() const = &Molecule::atoms;
    AtomSet (Molecule::*wrap_atoms2)(ResNum) const = &Molecule::atoms;
    
    AtomVector (Molecule::*wrap_atomVector1)() const = &Molecule::atomVector;
    AtomVector (Molecule::*wrap_atomVector2)(ResNum) const = &Molecule::atomVector;
    
    int (Molecule::*wrap_nAtoms1)() const = &Molecule::nAtoms;
    int (Molecule::*wrap_nAtoms2)(CutGroupID) const = &Molecule::nAtoms;
    int (Molecule::*wrap_nAtoms3)(ResNum) const = &Molecule::nAtoms;
    
    /** Wrap up a Molecule */
    class_<Molecule> wrapper( "Molecule", init<>() );
    
    wrapper               
          .def(init<const QString&, const ResidueIDSet&, const CutGroupVector&,
                    const MoleculeBonds&>())
          .def(init<const Molecule&>())
          .def("ID", &Molecule::ID)
          .def("setNewID", &Molecule::setNewID)
          .def("clone", &Molecule::clone)
          .def(self == self)
          .def(self != self)
          .def("__getitem__",&readwrite_indexer<Molecule,Residue>::__getitem__)
          .def("__len__", &Molecule::count)
          .def("__str__", &__str__<Molecule>)
          .def("deepCopy", &Molecule::deepCopy)
          .def("shallowCopy", &Molecule::shallowCopy)
          .def("detach", &Molecule::detach)
          .def("residue", (Residue (Molecule::*)(ResNum))&Molecule::residue)
          .def("residue", (Residue (Molecule::*)(ResNum) const)&Molecule::residue)
          .def("at", (Residue (Molecule::*)(int))&Molecule::at)
          .def("at", (Residue (Molecule::*)(int) const)&Molecule::at)
          .def("residues", (ResidueSet (Molecule::*)())&Molecule::residues)
          .def("residues", (ResidueSet (Molecule::*)() const)&Molecule::residues)
          .def("toString", &Molecule::toString)
          .def("isNull", &Molecule::isNull)
          .def("isEmpty", &Molecule::isEmpty)
          .def("name", &Molecule::name)
          .def("cutGroups", &Molecule::cutGroups)
          .def("connectivity", &Molecule::connectivity)
          .def("signature", &Molecule::signature)
          .def("info", &Molecule::info, return_value_policy<copy_const_reference>())
          .def("atom", &Molecule::atom, return_value_policy<copy_const_reference>())
          .def("atoms", wrap_atoms1)
          .def("atoms", wrap_atoms2)
          .def("bonds", (QList<Bond> (Molecule::*)() const) &Molecule::bonds )
          .def("bonds", (QList<Bond> (Molecule::*)(ResNum) const) &Molecule::bonds )
          .def("atomVector", wrap_atomVector1)
          .def("atomVector", wrap_atomVector2)
          .def("nAtoms", wrap_nAtoms1)
          .def("nAtoms", wrap_nAtoms2)
          .def("nAtoms", wrap_nAtoms3)
          .def("nBonds", &Molecule::nBonds)
          .def("nResidues", &Molecule::nResidues)
          .def("nCutGroups", &Molecule::nCutGroups)
          .def("count", &Molecule::count)
          .def("bond", &Molecule::bond)
          .def("angle", &Molecule::angle)
          .def("dihedral", &Molecule::dihedral)
          .def("improper", &Molecule::improper)
          .def("measure", wrap_measure1)
          .def("measure", wrap_measure2)
          .def("measure", wrap_measure3)
          .def("measure", wrap_measure4)
    ;

    export_Molecule2(wrapper);
}

}
