
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

/*
#include "SirePy/pycontainer.hpp"
#include "SirePy/str.hpp"
#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SirePy;
using namespace SireQt;

namespace SireMol
{

/////////////
///////////// Function used to wrap the EditRes class
/////////////

void export_EditRes2(class_<EditRes> &wrapper);
void export_EditRes3(class_<EditRes> &wrapper);

void export_EditRes()
{
    //exposing overloaded functions
    void (EditRes::*wrap_add1)(const QString&) = &EditRes::add;
    void (EditRes::*wrap_add2)(const QStringList&) = &EditRes::add;
    void (EditRes::*wrap_add3)(const Atom&) = &EditRes::add;
    void (EditRes::*wrap_add4)(const AtomList&) = &EditRes::add;
    void (EditRes::*wrap_add5)(const AtomVector&) = &EditRes::add;
    void (EditRes::*wrap_add6)(const EditRes&) = &EditRes::add;
    void (EditRes::*wrap_add7)(const Bond&) = &EditRes::add;
    void (EditRes::*wrap_add8)(const BondList&) = &EditRes::add;

    void (EditRes::*wrap_update1)(const Atom&) = &EditRes::update;
    void (EditRes::*wrap_update2)(const QString&, const Vector&) = &EditRes::update;
    void (EditRes::*wrap_update3)(const QString&, const Element&) = &EditRes::update;
    void (EditRes::*wrap_update4)(const QString&, const Atom&) = &EditRes::update;

    void (EditRes::*wrap_remove1)(const QString&) = &EditRes::remove;
    void (EditRes::*wrap_remove2)(const QStringList&) = &EditRes::remove;
    void (EditRes::*wrap_remove3)(const AtomIndex&) = &EditRes::remove;
    void (EditRes::*wrap_remove4)(const AtomIndexSet&) = &EditRes::remove;
    void (EditRes::*wrap_remove5)(const AtomIndexList&) = &EditRes::remove;
    void (EditRes::*wrap_remove6)(const Bond&) = &EditRes::remove;
    void (EditRes::*wrap_remove7)(const BondList&) = &EditRes::remove;

    void (EditRes::*wrap_removeAllBonds1)(const QString&) = &EditRes::removeAllBonds;
    void (EditRes::*wrap_removeAllBonds2)() = &EditRes::removeAllBonds;

    void (EditRes::*wrap_translate1)(const Vector&) = &EditRes::translate;
    void (EditRes::*wrap_translate2)(const QString&, const Vector&) = &EditRes::translate;
    void (EditRes::*wrap_translate3)(const QStringList&, const Vector&) = &EditRes::translate;

    void (EditRes::*wrap_rotate1)(const Quaternion&, const Vector&) = &EditRes::rotate;
    void (EditRes::*wrap_rotate2)(const QString&, const Quaternion&, const Vector&)
                                     = &EditRes::rotate;
    void (EditRes::*wrap_rotate3)(const QStringList&, const Quaternion&, const Vector&)
                                     = &EditRes::rotate;

    //actual class wrapper
    class_<EditRes> wrapper( "EditRes", init<>() );

    //constructors
    wrapper.def( init<ResNum>() )
           .def( init<ResNum, const QString&>() )
           .def( init<const QString&>() )
           .def( init<const EditRes&>() )

    //operators
           .def( self == self )
           .def( self != self )

           .def( "__contains__", &__contains__<EditRes,QString> )
           .def( "__contains__", &__contains__<EditRes,AtomIndex> )

           .def( "__getitem__", &__getitem__list_const<EditRes,Atom> )
           .def( "__getitem__", &__getitem__dict_const<EditRes,QString,Atom> )

           .def( "__len__", &__len__<EditRes>)

           .def( "__str__", &__str__<EditRes>)

           .def( "__rlshift__", &__rlshift__QDataStream<EditRes>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
           .def( "__rrshift__", &__rrshift__QDataStream<EditRes>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

    //editing functions
           .def("setName", &EditRes::setName)
           .def("setNumber", &EditRes::setNumber)
           .def("clear", &EditRes::clear)
           .def("finalise", &EditRes::finalise)
           .def("add", wrap_add1)
           .def("add", wrap_add2)
           .def("add", wrap_add3)
           .def("add", wrap_add4)
           .def("add", wrap_add5)
           .def("add", wrap_add6)
           .def("add", wrap_add7)
           .def("add", wrap_add8)
           .def("merge", &EditRes::merge)
           .def("update", wrap_update1)
           .def("update", wrap_update2)
           .def("update", wrap_update3)
           .def("update", wrap_update4)
           .def("remove", wrap_remove1)
           .def("remove", wrap_remove2)
           .def("remove", wrap_remove3)
           .def("remove", wrap_remove4)
           .def("remove", wrap_remove5)
           .def("remove", wrap_remove6)
           .def("remove", wrap_remove7)
           .def("addBond", &EditRes::addBond)
           .def("removeBond", &EditRes::removeBond)
           .def("addAutoBonds", &EditRes::addAutoBonds)
           .def("removeAllBonds", wrap_removeAllBonds1)
           .def("removeAllBonds", wrap_removeAllBonds2)
           .def("applyTemplate", &EditRes::applyTemplate)

    //moving the residue
           .def("translate", wrap_translate1)
           .def("translate", wrap_translate2)
           .def("translate", wrap_translate3)
           .def("rotate", wrap_rotate1)
           .def("rotate", wrap_rotate2)
           .def("rotate", wrap_rotate3)
    ;

    //wrap up the rest of the class (split into
    //multiple files to prevent memory thrashing!)
    export_EditRes2(wrapper);
    export_EditRes3(wrapper);

}

}*/
