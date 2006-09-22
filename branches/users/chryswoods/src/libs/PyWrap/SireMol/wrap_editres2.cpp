
/**
  * This file contains the boost::python wrapping of EditRes
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
// //////////
// ////////// Wrapping all of the change functions
// //////////
//
// void wrap_change1(EditRes &mol, const Bond &bnd, double delta)
// {
//     mol.change(bnd,delta);
// }
//
// void wrap_change2(EditRes &mol, const Bond &bnd, double delta, const AtomIndexSet &anchors)
// {
//     mol.change(bnd,delta,anchors);
// }
//
// void wrap_change3(EditRes &mol, const Bond &bnd, double delta, const WeightFunction &weightfunc)
// {
//     mol.change(bnd,delta,weightfunc);
// }
//
// void wrap_change4(EditRes &mol, const Bond &bnd, double delta,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     mol.change(bnd,delta,weightfunc,anchors);
// }
//
// void wrap_change5(EditRes &mol, const SireMol::Angle &ang, const SireMaths::Angle &delta)
// {
//     mol.change(ang,delta);
// }
//
// void wrap_change6(EditRes &mol, const SireMol::Angle &ang, const SireMaths::Angle &delta,
//                   const AtomIndexSet &anchors)
// {
//     mol.change(ang,delta,anchors);
// }
//
// void wrap_change7(EditRes &mol, const SireMol::Angle &ang, const SireMaths::Angle &delta,
//                   const WeightFunction &weightfunc)
// {
//     mol.change(ang,delta,weightfunc);
// }
//
// void wrap_change8(EditRes &mol, const SireMol::Angle &ang, const SireMaths::Angle &delta,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     mol.change(ang,delta,weightfunc,anchors);
// }
//
// void wrap_change9(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &delta)
// {
//     mol.change(dih,delta);
// }
//
// void wrap_change10(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &delta,
//                   const AtomIndexSet &anchors)
// {
//     mol.change(dih,delta,anchors);
// }
//
// void wrap_change11(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &delta,
//                   const WeightFunction &weightfunc)
// {
//     mol.change(dih,delta,weightfunc);
// }
//
// void wrap_change12(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &delta,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     mol.change(dih,delta,weightfunc,anchors);
// }
//
// void wrap_change13(EditRes &mol, const Bond &bnd, const SireMaths::Angle &delta)
// {
//     mol.change(bnd,delta);
// }
//
// void wrap_change14(EditRes &mol, const Bond &bnd, const SireMaths::Angle &delta,
//                   const AtomIndexSet &anchors)
// {
//     mol.change(bnd,delta,anchors);
// }
//
// void wrap_change15(EditRes &mol, const Bond &bnd, const SireMaths::Angle &delta,
//                   const WeightFunction &weightfunc)
// {
//     mol.change(bnd,delta,weightfunc);
// }
//
// void wrap_change16(EditRes &mol, const Bond &bnd, const SireMaths::Angle &delta,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     mol.change(bnd,delta,weightfunc,anchors);
// }
//
// void wrap_change17(EditRes &mol, const Improper &imp, const SireMaths::Angle &delta)
// {
//     mol.change(imp,delta);
// }
//
// void wrap_change18(EditRes &mol, const Improper &imp, const SireMaths::Angle &delta,
//                   const AtomIndexSet &anchors)
// {
//     mol.change(imp,delta,anchors);
// }
//
// void wrap_change19(EditRes &mol, const Improper &imp, const SireMaths::Angle &delta,
//                   const WeightFunction &weightfunc)
// {
//     mol.change(imp,delta,weightfunc);
// }
//
// void wrap_change20(EditRes &mol, const Improper &imp, const SireMaths::Angle &delta,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     mol.change(imp,delta,weightfunc,anchors);
// }
//
// /////////////
// ///////////// Wrapping the 'set' functions...
// /////////////
//
// void wrap_set1(EditRes &mol, const Bond &bnd, double size)
// {
//     mol.set(bnd,size);
// }
//
// void wrap_set2(EditRes &mol, const Bond &bnd, double size, const AtomIndexSet &anchors)
// {
//     mol.set(bnd,size,anchors);
// }
//
// void wrap_set3(EditRes &mol, const Bond &bnd, double size, const WeightFunction &weightfunc)
// {
//     mol.set(bnd,size,weightfunc);
// }
//
// void wrap_set4(EditRes &mol, const Bond &bnd, double size,
//                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     mol.set(bnd,size,weightfunc,anchors);
// }
//
// void wrap_set5(EditRes &mol, const SireMol::Angle &ang, const SireMaths::Angle &size)
// {
//     mol.set(ang,size);
// }
//
// void wrap_set6(EditRes &mol, const SireMol::Angle &ang, const SireMaths::Angle &size,
//                const AtomIndexSet &anchors)
// {
//     mol.set(ang,size,anchors);
// }
//
// void wrap_set7(EditRes &mol, const SireMol::Angle &ang, const SireMaths::Angle &size,
//                const WeightFunction &weightfunc)
// {
//     mol.set(ang,size,weightfunc);
// }
//
// void wrap_set8(EditRes &mol, const SireMol::Angle &ang, const SireMaths::Angle &size,
//                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     mol.set(ang,size,weightfunc,anchors);
// }
//
// void wrap_set9(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &size)
// {
//     mol.set(dih,size);
// }
//
// void wrap_set10(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &size,
//                 const AtomIndexSet &anchors)
// {
//     mol.set(dih,size,anchors);
// }
//
// void wrap_set11(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &size,
//                 const WeightFunction &weightfunc)
// {
//     mol.set(dih,size,weightfunc);
// }
//
// void wrap_set12(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &size,
//                 const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     mol.set(dih,size,weightfunc,anchors);
// }
//
// void wrap_set13(EditRes &mol, const Improper &imp, const SireMaths::Angle &size)
// {
//     mol.set(imp,size);
// }
//
// void wrap_set14(EditRes &mol, const Improper &imp, const SireMaths::Angle &size,
//                 const AtomIndexSet &anchors)
// {
//     mol.set(imp,size,anchors);
// }
//
// void wrap_set15(EditRes &mol, const Improper &imp, const SireMaths::Angle &size,
//                 const WeightFunction &weightfunc)
// {
//     mol.set(imp,size,weightfunc);
// }
//
// void wrap_set16(EditRes &mol, const Improper &imp, const SireMaths::Angle &size,
//                 const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     mol.set(imp,size,weightfunc,anchors);
// }
//
// /////////////
// ///////////// Wrapping the 'setAll' functions...
// /////////////
//
// void wrap_setAll1(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &size)
// {
//     mol.setAll(dih,size);
// }
//
// void wrap_setAll2(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &size,
//                   const AtomIndexSet &anchors)
// {
//     mol.setAll(dih,size,anchors);
// }
//
// void wrap_setAll3(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &size,
//                   const WeightFunction &weightfunc)
// {
//     mol.setAll(dih,size,weightfunc);
// }
//
// void wrap_setAll4(EditRes &mol, const Dihedral &dih, const SireMaths::Angle &size,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     mol.setAll(dih,size,weightfunc,anchors);
// }
//
// /////////////
// ///////////// Function used to wrap the EditRes class
// /////////////
//
// void export_EditRes2(class_<EditRes> &wrapper)
// {
//     //internal geometry moves
//     wrapper.def("change", &wrap_change1);
//     wrapper.def("change", &wrap_change2);
//     wrapper.def("change", &wrap_change3);
//     wrapper.def("change", &wrap_change4);
//     wrapper.def("change", &wrap_change5);
//     wrapper.def("change", &wrap_change6);
//     wrapper.def("change", &wrap_change7);
//     wrapper.def("change", &wrap_change8);
//     wrapper.def("change", &wrap_change9);
//     wrapper.def("change", &wrap_change10);
//     wrapper.def("change", &wrap_change11);
//     wrapper.def("change", &wrap_change12);
//     wrapper.def("change", &wrap_change13);
//     wrapper.def("change", &wrap_change14);
//     wrapper.def("change", &wrap_change15);
//     wrapper.def("change", &wrap_change16);
//     wrapper.def("change", &wrap_change17);
//     wrapper.def("change", &wrap_change18);
//     wrapper.def("change", &wrap_change19);
//     wrapper.def("change", &wrap_change20);
//
//     wrapper.def("set", &wrap_set1);
//     wrapper.def("set", &wrap_set2);
//     wrapper.def("set", &wrap_set3);
//     wrapper.def("set", &wrap_set4);
//     wrapper.def("set", &wrap_set5);
//     wrapper.def("set", &wrap_set6);
//     wrapper.def("set", &wrap_set7);
//     wrapper.def("set", &wrap_set8);
//     wrapper.def("set", &wrap_set9);
//     wrapper.def("set", &wrap_set10);
//     wrapper.def("set", &wrap_set11);
//     wrapper.def("set", &wrap_set12);
//     wrapper.def("set", &wrap_set13);
//     wrapper.def("set", &wrap_set14);
//     wrapper.def("set", &wrap_set15);
//     wrapper.def("set", &wrap_set16);
//
//     wrapper.def("setAll", &wrap_setAll1);
//     wrapper.def("setAll", &wrap_setAll2);
//     wrapper.def("setAll", &wrap_setAll3);
//     wrapper.def("setAll", &wrap_setAll4);
//
// }
//
// }
