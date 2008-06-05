
/**
  * This file contains the boost::python wrapping of a Residue
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

void export_Residue2(class_<Residue> &wrapper);

void export_Residue()
{
    
    const Atom& (Residue::*wrap_atom1)(const QString&) const = &Residue::atom;
    const Atom& (Residue::*wrap_atom2)(int) const = &Residue::atom;
    
    double (Residue::*wrap_measure1)(const QString&, const QString&) const
                                        = &Residue::measure;
    SireMaths::Angle (Residue::*wrap_measure2)(const QString&, const QString&,
                              const QString&) const = &Residue::measure;
    SireMaths::Angle (Residue::*wrap_measure3)(const QString&, const QString&,
                              const QString&, const QString&) const = &Residue::measure;
    
    void (Residue::*wrap_translate1)(const Vector&) = &Residue::translate;
    void (Residue::*wrap_translate2)(const QString&, const Vector&)
                               = &Residue::translate;
    void (Residue::*wrap_translate3)(const QStringList&, const Vector&)
                               = &Residue::translate;
          
    void (Residue::*wrap_rotate1)(const Quaternion&, const Vector&)
                               = &Residue::rotate;
    void (Residue::*wrap_rotate2)(const QString&, const Quaternion&, const Vector&)
                               = &Residue::rotate;
    void (Residue::*wrap_rotate3)(const QStringList&, const Quaternion&, const Vector&)
                               = &Residue::rotate;

    /** Wrap up a Residue */
    class_<Residue> wrapper( "Residue", init<>() );
    
    wrapper               
        .def(init<const Residue&>())
        .def(self == self)
        .def(self != self)
        .def("__getitem__",&readonly_indexer<Residue,Atom>::__getitem__)
        .def("__len__", &Residue::nAtoms)
        .def("__str__", &__str__<Residue>)
        .def("deepCopy", &Residue::deepCopy)
        .def("shallowCopy", &Residue::shallowCopy)
        .def("detach", &Residue::detach)
        .def("molecule", &Residue::molecule)
        .def("name", &Residue::name)
        .def("toString", &Residue::toString)
        .def("isNull", &Residue::isNull)
        .def("isEmpty", &Residue::isEmpty)
        .def("number", &Residue::number)
        .def("resNum", &Residue::resNum)
        .def("cutGroups", &Residue::cutGroups)
        .def("at", &Residue::at, return_value_policy<copy_const_reference>())
        .def("count", &Residue::count)
        .def("size", &Residue::size)
        .def("nAtoms", &Residue::nAtoms)
        .def("atomNames", &Residue::atomNames)
        .def("nBonds", &Residue::nBonds)
        .def("connectivity", &Residue::connectivity)
        .def("contains", &Residue::contains)
        .def("atom", wrap_atom1, return_value_policy<copy_const_reference>())
        .def("atom", wrap_atom2, return_value_policy<copy_const_reference>())
        .def("bond", &Residue::bond)
        .def("angle", &Residue::angle)
        .def("dihedral", &Residue::dihedral)
        .def("improper", &Residue::improper)
        .def("measure", wrap_measure1)
        .def("measure", wrap_measure2)
        .def("measure", wrap_measure3)
        .def("measureImproper", &Residue::measureImproper)
        .def("atoms", &Residue::atoms)
        .def("atomVector", &Residue::atomVector)
        .def("bonds", &Residue::bonds)
        .def("residuesBondedTo", &Residue::residuesBondedTo)  
        .def("translate", wrap_translate1)
        .def("translate", wrap_translate2)
        .def("translate", wrap_translate3)
        .def("rotate", wrap_rotate1)
        .def("rotate", wrap_rotate2)
        .def("rotate", wrap_rotate3)
    ;

    export_Residue2(wrapper);
}

}
