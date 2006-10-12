
/**
  * This file contains the boost::python wrapping of AtomTable
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>


using namespace boost::python;

namespace SireDB
{

/*void export_AtomTable()
{
    class_<TableBase, boost::noncopyable>("TableBase", no_init)
          .def( "isCompatibleWith", &TableBase::isCompatibleWith )

          .def( "isEmpty", &TableBase::isEmpty )

          .def( "what", &TableBase::what )
    ;

    class_<AtomTable, bases<TableBase>, boost::noncopyable>("AtomTable", no_init)

        .def( "__contains__", &__contains__<AtomTable,CGAtomID> )
        .def( "__contains__", &__contains__<AtomTable,ResNumAtomID> )
        .def( "__contains__", &__contains__<AtomTable,AtomIndex> )
        .def( "__contains__", &__contains__<AtomTable,CutGroupID> )
        .def( "__contains__", &__contains__<AtomTable,ResNum> )

        .def( "__rrshift__", &__rrshift__QDataStream<AtomTable>,
                      return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<AtomTable>,
                      return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "assignedParameter", (bool (AtomTable::*)(const CGAtomID&) const)
                    &AtomTable::assignedParameter )
        .def( "assignedParameter", (bool (AtomTable::*)(const ResNumAtomID&) const)
                    &AtomTable::assignedParameter )
        .def( "assignedParameter", (bool (AtomTable::*)(const AtomIndex&) const)
                    &AtomTable::assignedParameter )

        .def( "atom", (AtomIndex (AtomTable::*)(const CGAtomID&) const)
                            &AtomTable::atom )
        .def( "atom", (AtomIndex (AtomTable::*)(const ResNumAtomID&) const)
                            &AtomTable::atom )

        .def( "atoms", (QSet<AtomIndex> (AtomTable::*)(const QSet<CGAtomID>&) const)
                            &AtomTable::atoms )
        .def( "atoms", (QSet<AtomIndex> (AtomTable::*)(const QSet<ResNumAtomID>&) const)
                            &AtomTable::atoms )

        .def( "atoms", (QVector<AtomIndex> (AtomTable::*)() const)
                            &AtomTable::atoms )
        .def( "atoms", (QVector<AtomIndex> (AtomTable::*)(ResNum) const)
                            &AtomTable::atoms )
        .def( "atoms", (QVector<AtomIndex> (AtomTable::*)(CutGroupID cgid) const)
                            &AtomTable::atoms )

        .def( "atomsByCutGroup", (GroupedVector<CGAtomID,AtomIndex> (AtomTable::*)() const)
                            &AtomTable::atomsByCutGroup )
        .def( "atomsByCutGroup", (GroupedVector<CGAtomID,AtomIndex>
                            (AtomTable::*)(const QSet<CutGroupID> &cgids) const)
                            &AtomTable::atomsByCutGroup )

        .def( "atomsByResidue", (GroupedVector<ResNumAtomID,AtomIndex> (AtomTable::*)() const)
                            &AtomTable::atomsByResidue )
        .def( "atomsByResidue", (GroupedVector<ResNumAtomID,AtomIndex>
                            (AtomTable::*)(const QSet<ResNum> &resnums) const)
                            &AtomTable::atomsByResidue )

        .def( "clear", (void (AtomTable::*)()) &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(const CGAtomID&)) &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(const ResNumAtomID&)) &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(const AtomIndex&)) &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(ResNum)) &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(CutGroupID)) &AtomTable::clear )

        //template<class C>
        //void clear(const C &lots);

        .def( "contains", (bool (AtomTable::*)(const AtomIndex&) const)
                            &AtomTable::contains )
        .def( "contains", (bool (AtomTable::*)(CutGroupID) const)
                            &AtomTable::contains )
        .def( "contains", (bool (AtomTable::*)(ResNum) const)
                            &AtomTable::contains )
        .def( "contains", (bool (AtomTable::*)(const CGAtomID&) const)
                            &AtomTable::contains )
        .def( "contains", (bool (AtomTable::*)(const ResNumAtomID&) const)
                            &AtomTable::contains )

        .def( "count", &AtomTable::count )

        .def( "cutGroupIDs", &AtomTable::cutGroupIDs )

        .def( "hasMissingParameters", (bool (AtomTable::*)() const)
                    &AtomTable::hasMissingParameters )
        .def( "hasMissingParameters", (bool (AtomTable::*)(ResNum) const)
                    &AtomTable::hasMissingParameters )
        .def( "hasMissingParameters", (bool (AtomTable::*)(CutGroupID) const)
                    &AtomTable::hasMissingParameters )

        .def( "info", &AtomTable::info, return_value_policy<copy_const_reference>() )

        .def( "nAtoms", (int (AtomTable::*)() const)
                            &AtomTable::nAtoms )
        .def( "nAtoms", (int (AtomTable::*)(ResNum) const)
                            &AtomTable::nAtoms )
        .def( "nAtoms", (int (AtomTable::*)(CutGroupID) const)
                            &AtomTable::nAtoms )
        .def( "nAtoms", (int (AtomTable::*)(const QSet<ResNum> &) const)
                            &AtomTable::nAtoms )
        .def( "nAtoms", (int (AtomTable::*)(const QSet<CutGroupID>&) const)
                            &AtomTable::nAtoms )

        .def( "nAtomsPerCutGroup", &AtomTable::nAtomsPerCutGroup )

        .def( "nCutGroups", &AtomTable::nCutGroups )

        .def( "nParameters", (int (AtomTable::*)() const) &AtomTable::nParameters )
        .def( "nParameters", (int (AtomTable::*)(ResNum) const) &AtomTable::nParameters )
        .def( "nParameters", (int (AtomTable::*)(CutGroupID) const) &AtomTable::nParameters )

        .def( "nResidues", &AtomTable::nResidues )

        .def( "orderedCutGroups", &AtomTable::orderedCutGroupIDs )
        .def( "residueNumbers", &AtomTable::residueNumbers )
        .def( "size", &AtomTable::size )
    ;

    //wrap the GroupedVector used by AtomTable
    wrap_GroupedVector<CGAtomID,AtomIndex>("GroupedVector_AtomIndex_CGAtomID");*/
//}

}
