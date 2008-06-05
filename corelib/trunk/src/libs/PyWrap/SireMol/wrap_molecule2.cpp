
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

#include "SireMaths/quaternion.h"
#include "SireMaths/angle.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

#include "SirePy/indexer.hpp"

using namespace boost::python;

namespace SireMol
{

/////////////
///////////// Wrapping the 'change' functions...
/////////////

void wrap_change1(Molecule &mol, const Bond &bnd, double delta)
{
    mol.change(bnd,delta);
}

void wrap_change2(Molecule &mol, const Bond &bnd, double delta, const AtomIndexSet &anchors)
{
    mol.change(bnd,delta,anchors);
}

void wrap_change3(Molecule &mol, const Bond &bnd, double delta, const WeightFunction &weightfunc)
{
    mol.change(bnd,delta,weightfunc);
}

void wrap_change4(Molecule &mol, const Bond &bnd, double delta,
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    mol.change(bnd,delta,weightfunc,anchors);
}

void wrap_change5(Molecule &mol, const SireMol::Angle &ang, const SireMaths::Angle &delta)
{
    mol.change(ang,delta);
}

void wrap_change6(Molecule &mol, const SireMol::Angle &ang, const SireMaths::Angle &delta,
                  const AtomIndexSet &anchors)
{
    mol.change(ang,delta,anchors);
}

void wrap_change7(Molecule &mol, const SireMol::Angle &ang, const SireMaths::Angle &delta,
                  const WeightFunction &weightfunc)
{
    mol.change(ang,delta,weightfunc);
}

void wrap_change8(Molecule &mol, const SireMol::Angle &ang, const SireMaths::Angle &delta,
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    mol.change(ang,delta,weightfunc,anchors);
}

void wrap_change9(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &delta)
{
    mol.change(dih,delta);
}

void wrap_change10(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &delta,
                  const AtomIndexSet &anchors)
{
    mol.change(dih,delta,anchors);
}

void wrap_change11(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &delta,
                  const WeightFunction &weightfunc)
{
    mol.change(dih,delta,weightfunc);
}

void wrap_change12(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &delta,
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    mol.change(dih,delta,weightfunc,anchors);
}

void wrap_change13(Molecule &mol, const Bond &bnd, const SireMaths::Angle &delta)
{
    mol.change(bnd,delta);
}

void wrap_change14(Molecule &mol, const Bond &bnd, const SireMaths::Angle &delta,
                  const AtomIndexSet &anchors)
{
    mol.change(bnd,delta,anchors);
}

void wrap_change15(Molecule &mol, const Bond &bnd, const SireMaths::Angle &delta,
                  const WeightFunction &weightfunc)
{
    mol.change(bnd,delta,weightfunc);
}

void wrap_change16(Molecule &mol, const Bond &bnd, const SireMaths::Angle &delta,
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    mol.change(bnd,delta,weightfunc,anchors);
}

void wrap_change17(Molecule &mol, const Improper &imp, const SireMaths::Angle &delta)
{
    mol.change(imp,delta);
}

void wrap_change18(Molecule &mol, const Improper &imp, const SireMaths::Angle &delta,
                  const AtomIndexSet &anchors)
{
    mol.change(imp,delta,anchors);
}

void wrap_change19(Molecule &mol, const Improper &imp, const SireMaths::Angle &delta,
                  const WeightFunction &weightfunc)
{
    mol.change(imp,delta,weightfunc);
}

void wrap_change20(Molecule &mol, const Improper &imp, const SireMaths::Angle &delta,
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    mol.change(imp,delta,weightfunc,anchors);
}

/////////////
///////////// Wrapping the 'set' functions...
/////////////

void wrap_set1(Molecule &mol, const Bond &bnd, double size)
{
    mol.set(bnd,size);
}

void wrap_set2(Molecule &mol, const Bond &bnd, double size, const AtomIndexSet &anchors)
{
    mol.set(bnd,size,anchors);
}

void wrap_set3(Molecule &mol, const Bond &bnd, double size, const WeightFunction &weightfunc)
{
    mol.set(bnd,size,weightfunc);
}

void wrap_set4(Molecule &mol, const Bond &bnd, double size,
               const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    mol.set(bnd,size,weightfunc,anchors);
}

void wrap_set5(Molecule &mol, const SireMol::Angle &ang, const SireMaths::Angle &size)
{
    mol.set(ang,size);
}

void wrap_set6(Molecule &mol, const SireMol::Angle &ang, const SireMaths::Angle &size,
               const AtomIndexSet &anchors)
{
    mol.set(ang,size,anchors);
}

void wrap_set7(Molecule &mol, const SireMol::Angle &ang, const SireMaths::Angle &size, 
               const WeightFunction &weightfunc)
{
    mol.set(ang,size,weightfunc);
}

void wrap_set8(Molecule &mol, const SireMol::Angle &ang, const SireMaths::Angle &size,
               const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    mol.set(ang,size,weightfunc,anchors);
}

void wrap_set9(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &size)
{
    mol.set(dih,size);
}

void wrap_set10(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &size,
                const AtomIndexSet &anchors)
{
    mol.set(dih,size,anchors);
}

void wrap_set11(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &size, 
                const WeightFunction &weightfunc)
{
    mol.set(dih,size,weightfunc);
}

void wrap_set12(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &size,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    mol.set(dih,size,weightfunc,anchors);
}

void wrap_set13(Molecule &mol, const Improper &imp, const SireMaths::Angle &size)
{
    mol.set(imp,size);
}

void wrap_set14(Molecule &mol, const Improper &imp, const SireMaths::Angle &size,
                const AtomIndexSet &anchors)
{
    mol.set(imp,size,anchors);
}

void wrap_set15(Molecule &mol, const Improper &imp, const SireMaths::Angle &size, 
                const WeightFunction &weightfunc)
{
    mol.set(imp,size,weightfunc);
}

void wrap_set16(Molecule &mol, const Improper &imp, const SireMaths::Angle &size,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    mol.set(imp,size,weightfunc,anchors);
}

/////////////
///////////// Wrapping the 'setAll' functions...
/////////////

void wrap_setAll1(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &size)
{
    mol.setAll(dih,size);
}

void wrap_setAll2(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &size,
                  const AtomIndexSet &anchors)
{
    mol.setAll(dih,size,anchors);
}

void wrap_setAll3(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &size, 
                  const WeightFunction &weightfunc)
{
    mol.setAll(dih,size,weightfunc);
}

void wrap_setAll4(Molecule &mol, const Dihedral &dih, const SireMaths::Angle &size,
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    mol.setAll(dih,size,weightfunc,anchors);
}

void export_Molecule2(class_<Molecule> &wrapper)
{
    void (Molecule::*wrap_translate1)(const Vector&) = &Molecule::translate;
    void (Molecule::*wrap_translate2)(const AtomIDGroup&, 
                    const Vector&) = &Molecule::translate;
    void (Molecule::*wrap_translate3)(const AtomIndexSet&, 
                    const Vector&) = &Molecule::translate;
    void (Molecule::*wrap_translate4)(ResNum, const Vector&) = &Molecule::translate;
    
    void (Molecule::*wrap_rotate1)(const Quaternion&, const Vector&) = &Molecule::rotate;
    void (Molecule::*wrap_rotate2)(const AtomIDGroup&, const Quaternion&, 
                    const Vector&) = &Molecule::rotate;
    void (Molecule::*wrap_rotate3)(const AtomIndexSet&, const Quaternion&, 
                    const Vector&) = &Molecule::rotate;
    void (Molecule::*wrap_rotate4)(ResNum, const Quaternion&, const Vector&)
                     = &Molecule::rotate;
    
    wrapper               
        .def("translate", wrap_translate1)
        .def("translate", wrap_translate2)
        .def("translate", wrap_translate3)
        .def("translate", wrap_translate4)
        .def("rotate", wrap_rotate1)
        .def("rotate", wrap_rotate2)
        .def("rotate", wrap_rotate3)
        .def("rotate", wrap_rotate4)
        .def("change", &wrap_change1)
        .def("change", &wrap_change2)
        .def("change", &wrap_change3)
        .def("change", &wrap_change4)
        .def("change", &wrap_change5)
        .def("change", &wrap_change6)
        .def("change", &wrap_change7)
        .def("change", &wrap_change8)
        .def("change", &wrap_change9)
        .def("change", &wrap_change10)
        .def("change", &wrap_change11)
        .def("change", &wrap_change12)
        .def("change", &wrap_change13)
        .def("change", &wrap_change14)
        .def("change", &wrap_change15)
        .def("change", &wrap_change16)
        .def("change", &wrap_change17)
        .def("change", &wrap_change18)
        .def("change", &wrap_change19)
        .def("change", &wrap_change20)
        .def("set", &wrap_set1)
        .def("set", &wrap_set2)
        .def("set", &wrap_set3)
        .def("set", &wrap_set4)
        .def("set", &wrap_set5)
        .def("set", &wrap_set6)
        .def("set", &wrap_set7)
        .def("set", &wrap_set8)
        .def("set", &wrap_set9)
        .def("set", &wrap_set10)
        .def("set", &wrap_set11)
        .def("set", &wrap_set12)
        .def("set", &wrap_set13)
        .def("set", &wrap_set14)
        .def("set", &wrap_set15)
        .def("set", &wrap_set16)
        .def("setAll", &wrap_setAll1)
        .def("setAll", &wrap_setAll2)
        .def("setAll", &wrap_setAll3)
        .def("setAll", &wrap_setAll4)
    ;
}

}
