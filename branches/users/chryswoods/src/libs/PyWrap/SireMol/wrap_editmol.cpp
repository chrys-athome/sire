
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
#include "SireMol/molecule.h"

#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"
#include "SireMaths/angle.h"
#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"


#include "SirePy/pycontainer.hpp"
#include "SirePy/str.hpp"
#include "SireQt/qdatastream.hpp"

// using namespace boost::python;
// using namespace SirePy;
// using namespace SireQt;
//
// namespace SireMol
// {
//
// EditRes __getitem__editmol(EditMol &editmol, int index)
// {
//     int container_size = editmol.size();
//
//     //implement negative indicies
//     if (index < 0) index += container_size;
//
//     if ( index < 0 or index >= container_size )
//     {
//         PyErr_SetString( PyExc_IndexError,
//                 qPrintable(QObject::tr(
//                     "EditMol index out of range (%1 from %2)")
//                         .arg(index).arg(container_size)) );
//
//         throw_error_already_set();
//     }
//
//     return editmol[index];
// }
//
// /////////////
// ///////////// Function used to wrap the EditMol class
// /////////////
// void export_EditMol2(class_<EditMol> &wrapper);
// void export_EditMol3(class_<EditMol> &wrapper);
//
// void export_EditMol()
// {
//     //overloaded function definitions
//     EditRes (EditMol::*wrap_addResidue1)(ResNum, const QString&) = &EditMol::addResidue;
//     EditRes (EditMol::*wrap_addResidue2)(ResNum, const QString&, const EditRes&) = &EditMol::addResidue;
//     EditRes (EditMol::*wrap_addResidue3)(const QString&) = &EditMol::addResidue;
//     EditRes (EditMol::*wrap_addResidue4)(const QString&, const EditRes&) = &EditMol::addResidue;
//     EditRes (EditMol::*wrap_addResidue5)(ResNum) = &EditMol::addResidue;
//     EditRes (EditMol::*wrap_addResidue6)(ResNum, const EditRes&) = &EditMol::addResidue;
//     EditRes (EditMol::*wrap_addResidue7)(const EditRes&) = &EditMol::addResidue;
//
//     void (EditMol::*wrap_removeResidue1)(ResNum) = &EditMol::removeResidue;
//     void (EditMol::*wrap_removeResidue2)(const QString&) = &EditMol::removeResidue;
//
//     void (EditMol::*wrap_add1)(const Atom&) = &EditMol::add;
//     void (EditMol::*wrap_add2)(const AtomSet&) = &EditMol::add;
//     void (EditMol::*wrap_add3)(const AtomList&) = &EditMol::add;
//     void (EditMol::*wrap_add4)(const AtomVector&) = &EditMol::add;
//     void (EditMol::*wrap_add5)(const Bond&) = &EditMol::add;
//     void (EditMol::*wrap_add6)(const BondList&) = &EditMol::add;
//
//     void (EditMol::*wrap_remove1)(const AtomIndex&) = &EditMol::remove;
//     void (EditMol::*wrap_remove2)(const AtomIndexSet&) = &EditMol::remove;
//     void (EditMol::*wrap_remove3)(const AtomIndexList&) = &EditMol::remove;
//     void (EditMol::*wrap_remove4)(const Bond&) = &EditMol::remove;
//     void (EditMol::*wrap_remove5)(const BondList&) = &EditMol::remove;
//
//     void (EditMol::*wrap_update1)(const Atom&) = &EditMol::update;
//     void (EditMol::*wrap_update2)(const AtomIndex&, const Vector&) = &EditMol::update;
//     void (EditMol::*wrap_update3)(const AtomIndex&, const Element&) = &EditMol::update;
//     void (EditMol::*wrap_update4)(const AtomIndex&, const Atom&) = &EditMol::update;
//
//     void (EditMol::*wrap_removeAllBonds1)(ResNum) = &EditMol::removeAllBonds;
//     void (EditMol::*wrap_removeAllBonds2)(const AtomIndex&) = &EditMol::removeAllBonds;
//     void (EditMol::*wrap_removeAllBonds3)() = &EditMol::removeAllBonds;
//
//     void (EditMol::*wrap_applyTemplate1)(const EditRes&, ResNum, const TemplateFunction&)
//                         = &EditMol::applyTemplate;
//     void (EditMol::*wrap_applyTemplate2)(const EditRes&, const QString&,
//                         const TemplateFunction&) = &EditMol::applyTemplate;
//     void (EditMol::*wrap_applyTemplate3)(const EditMol&, const TemplateFunction&)
//                         = &EditMol::applyTemplate;
//
//     void (EditMol::*wrap_translate1)(const Vector&) = &EditMol::translate;
//     void (EditMol::*wrap_translate2)(const AtomIndex&, const Vector&) = &EditMol::translate;
//     void (EditMol::*wrap_translate3)(ResNum, const Vector&) = &EditMol::translate;
//     void (EditMol::*wrap_translate4)(const AtomIDGroup&, const Vector&) = &EditMol::translate;
//
//     void (EditMol::*wrap_rotate1)(const Quaternion&, const Vector&) = &EditMol::rotate;
//     void (EditMol::*wrap_rotate2)(const AtomIndex&, const Quaternion&, const Vector&)
//                                      = &EditMol::rotate;
//     void (EditMol::*wrap_rotate3)(ResNum, const Quaternion&, const Vector&) = &EditMol::rotate;
//     void (EditMol::*wrap_rotate4)(const AtomIDGroup&, const Quaternion&, const Vector&)
//                                      = &EditMol::rotate;
//
//     //actual class wrapper
//     class_<EditMol> wrapper( "EditMol", init<>() );
//
//     wrapper
//                //constructors
//                .def( init<const QString&>() )
//                .def( init<const EditMol&>() )
//                .def( init<const Molecule&>() )
//
//                //operators
//                .def( self == self )
//                .def( self != self )
//
//                .def( "__contains__", &__contains__<EditMol,AtomIndex> )
//                .def( "__contains__", &__contains__<EditMol,ResNum> )
//                .def( "__contains__", &__contains__<EditMol,QString> )
//
//                .def( "__getitem__", &__getitem__editmol )
//                .def( "__len__", &__len__<EditMol>)
//
//                .def( "__rlshift__", &__rlshift__QDataStream<EditMol>,
//                       return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
//                .def( "__rrshift__", &__rrshift__QDataStream<EditRes>,
//                       return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
//
//                .def( "__str__", &__str__<EditMol> )
//
//                //memory management
//                .def("detach", &EditMol::detach)
//                .def("shallowCopy", &EditMol::shallowCopy)
//                .def("deepCopy", &EditMol::deepCopy)
//
//                //editing functions
//                .def("setName", &EditMol::setName)
//                .def("clear", &EditMol::clear)
//                .def("addResidue", wrap_addResidue1)
//                .def("addResidue", wrap_addResidue2)
//                .def("addResidue", wrap_addResidue3)
//                .def("addResidue", wrap_addResidue4)
//                .def("addResidue", wrap_addResidue5)
//                .def("addResidue", wrap_addResidue6)
//                .def("addResidue", wrap_addResidue7)
//                .def("removeResidue", wrap_removeResidue1)
//                .def("removeResidue", wrap_removeResidue2)
//                .def("removeAllResidues", &EditMol::removeAllResidues)
//                .def("add", wrap_add1)
//                .def("add", wrap_add2)
//                .def("add", wrap_add3)
//                .def("add", wrap_add4)
//                .def("add", wrap_add5)
//                .def("add", wrap_add6)
//                .def("addAutoBonds", (void (EditMol::*)()) &EditMol::addAutoBonds)
//                .def("remove", wrap_remove1)
//                .def("remove", wrap_remove2)
//                .def("remove", wrap_remove3)
//                .def("remove", wrap_remove4)
//                .def("remove", wrap_remove5)
//                //.def("addAutoBonds", &EditMol::addAutoBonds)
//                .def("update", wrap_update1)
//                .def("update", wrap_update2)
//                .def("update", wrap_update3)
//                .def("update", wrap_update4)
//                .def("removeAllBonds", wrap_removeAllBonds1)
//                .def("removeAllBonds", wrap_removeAllBonds2)
//                .def("removeAllBonds", wrap_removeAllBonds3)
//                .def("applyTemplate", wrap_applyTemplate1)
//                .def("applyTemplate", wrap_applyTemplate2)
//                .def("applyTemplate", wrap_applyTemplate3)
//
//                //moving the molecule
//                .def("translate", wrap_translate1)
//                .def("translate", wrap_translate2)
//                .def("translate", wrap_translate3)
//                .def("translate", wrap_translate4)
//                .def("rotate", wrap_rotate1)
//                .def("rotate", wrap_rotate2)
//                .def("rotate", wrap_rotate3)
//                .def("rotate", wrap_rotate4)
//     ;
//
//     //wrap the remaining functions...
//     export_EditMol2(wrapper);
//     export_EditMol3(wrapper);
// }
//
// }
