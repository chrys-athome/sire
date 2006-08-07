
/**
  * This file contains the boost::python wrapping of the ID types
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>

#include <QObject>

#include "SireMol/atomnum.h"
#include "SireMol/cgatomid.h"
#include "SireMol/molcutgroupid.h"
#include "SireMol/molcgatomid.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/groupindexid.h"
#include "SireMol/resnumindexid.h"
#include "SireMol/wrapID.hpp"

#include "SirePy/str.hpp"
#include "SirePy/converttuple.hpp"

using namespace boost::python;
using namespace SirePy;

namespace SireMol
{


void export_IDTypes()
{
    wrap_ID<AtomNum>("AtomNum");
    wrap_ID<ResNum>("ResNum");
    wrap_ID<CutGroupID>("CutGroupID");
    wrap_ID<MoleculeID>("MoleculeID");
    wrap_ID<GroupID>("GroupID");
    wrap_ID<Index>("Index");

    class_<CGAtomID>("CGAtomID", init<>())
                .def(init<CutGroupID,AtomID>())
                .def(init<const CGAtomID&>())
                .def("cutGroupID", &CGAtomID::cutGroupID)
                .def("atomID", &CGAtomID::atomID)
                .def(self == self)
                .def(self != self)
                .def(self < self)
                .def(self <= self)
                .def(self > self)
                .def(self >= self)
                .def("__str__", &__str__<CGAtomID>)
    ;
    
    convert_python_tuple_to_boost_tuple2<CutGroupID,AtomID>();
    implicitly_convertible< boost::tuple<CutGroupID,AtomID>, CGAtomID >();
    
    class_<MolCutGroupID>("MolCutGroupID", init<>())
                .def(init<MoleculeID,CutGroupID>())
                .def(init<const MolCutGroupID&>())
                .def("moleculeID", &MolCutGroupID::moleculeID)
                .def("cutGroupID", &MolCutGroupID::cutGroupID)
                .def(self == self)
                .def(self != self)
                .def(self < self)
                .def(self <= self)
                .def(self > self)
                .def(self >= self)
                .def("__str__", &__str__<MolCutGroupID>)
    ;

    convert_python_tuple_to_boost_tuple2<MoleculeID,CutGroupID>();
    implicitly_convertible< boost::tuple<MoleculeID,CutGroupID>, MolCutGroupID >();
    
    class_<MolCGAtomID>("MolCGAtomID", init<>())
                .def(init<MoleculeID,CutGroupID,AtomID>())
                .def(init<const MolCGAtomID&>())
                .def("moleculeID", &MolCGAtomID::moleculeID)
                .def("cutGroupID", &MolCGAtomID::cutGroupID)
                .def("atomID", &MolCGAtomID::atomID)
                .def(self == self)
                .def(self != self)
                .def(self < self)
                .def(self <= self)
                .def(self > self)
                .def(self >= self)
                .def("__str__", &__str__<MolCGAtomID>)
    ;
    
    convert_python_tuple_to_boost_tuple3<MoleculeID,CutGroupID,AtomID>();
    implicitly_convertible< boost::tuple<MoleculeID,CutGroupID,AtomID>, MolCGAtomID >();
    
    class_<ResNumAtomID>("ResNumAtomID", init<>())
                .def(init<ResNum,AtomID>())
                .def(init<const ResNumAtomID&>())
                .def("resNum", &ResNumAtomID::resNum)
                .def("atomID", &CGAtomID::atomID)
                .def(self == self)
                .def(self != self)
                .def(self == self)
                .def(self != self)
                .def(self < self)
                .def(self <= self)
                .def(self > self)
                .def(self >= self)
                .def("__str__", &__str__<ResNumAtomID>)
    ;
    
    convert_python_tuple_to_boost_tuple2<ResNum,AtomID>();
    implicitly_convertible< boost::tuple<ResNum,AtomID>, ResNumAtomID >();
    
    class_<GroupIndexID>("GroupIndexID", init<>())
                .def(init<GroupID,Index>())
                .def(init<const GroupIndexID&>())
                .def("groupID", &GroupIndexID::groupID)
                .def("index", &GroupIndexID::index)
                .def(self == self)
                .def(self != self)
                .def(self < self)
                .def(self <= self)
                .def(self > self)
                .def(self >= self)
                .def("__str__", &__str__<GroupIndexID>)
    ;
    
    convert_python_tuple_to_boost_tuple2<GroupID,Index>();
    implicitly_convertible< boost::tuple<GroupID,Index>, GroupIndexID >();
    
    class_<ResNumIndexID>("ResNumIndexID", init<>())
                .def(init<ResNum,Index>())
                .def(init<const ResNumIndexID&>())
                .def("resNum", &ResNumIndexID::resNum)
                .def("index", &ResNumIndexID::index)
                .def(self == self)
                .def(self != self)
                .def(self < self)
                .def(self <= self)
                .def(self > self)
                .def(self >= self)
                .def("__str__", &__str__<ResNumIndexID>)
    ;
    
    implicitly_convertible< boost::tuple<ResNum,Index>, ResNumIndexID >();
}

}
