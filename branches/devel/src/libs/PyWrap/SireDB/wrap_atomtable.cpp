
/**
  * This file contains the boost::python wrapping of AtomTable
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/atomtable.h"

#include "SireMol/cgatomid.h"
#include "SireMol/cgnumatomid.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/residatomid.h"

#include "SireMol/molecule.h"
#include "SireMol/atomindex.h"

using namespace boost::python;

using namespace SireMol;

namespace SireDB
{

void export_AtomTable()
{
    class_<TableBase, boost::noncopyable>("TableBase", no_init)

        .def( "info", (const MoleculeInfo& (TableBase::*)() const)
                    &TableBase::info,
                    return_value_policy<copy_const_reference>() )

        .def( "isCompatibleWith", (bool (TableBase::*)(const MoleculeInfo&) const)
                    &TableBase::isCompatibleWith)

        .def( "what", (const char* (TableBase::*)() const)
                    &TableBase::what )

        .def( "isEmpty", (bool (TableBase::*)() const)
                    &TableBase::isEmpty )

        .def( "add", (void (TableBase::*)(const TableBase&))
                    &TableBase::add )

        .def( "assertCompatibleWith", (void (TableBase::*)(const Molecule&) const)
                    &TableBase::assertCompatibleWith )
    ;

    class_<AtomTable, bases<TableBase>, boost::noncopyable>("AtomTable", no_init)

        .def( "nParameters", (int (AtomTable::*)() const)
                    &AtomTable::nParameters )
        .def( "nParameters", (int (AtomTable::*)(ResNum) const)
                    &AtomTable::nParameters )
        .def( "nParameters", (int (AtomTable::*)(ResID) const)
                    &AtomTable::nParameters )
        .def( "nParameters", (int (AtomTable::*)(CutGroupID) const)
                    &AtomTable::nParameters )
        .def( "nParameters", (int (AtomTable::*)(CutGroupNum) const)
                    &AtomTable::nParameters )

        .def( "nAssignedParameters", (int (AtomTable::*)() const)
                    &AtomTable::nAssignedParameters )
        .def( "nAssignedParameters", (int (AtomTable::*)(ResNum) const)
                    &AtomTable::nAssignedParameters )
        .def( "nAssignedParameters", (int (AtomTable::*)(ResID) const)
                    &AtomTable::nAssignedParameters )
        .def( "nAssignedParameters", (int (AtomTable::*)(CutGroupNum) const)
                    &AtomTable::nAssignedParameters )
        .def( "nAssignedParameters", (int (AtomTable::*)(CutGroupID) const)
                    &AtomTable::nAssignedParameters )

        .def( "nMissingParameters", (int (AtomTable::*)() const)
                    &AtomTable::nMissingParameters )
        .def( "nMissingParameters", (int (AtomTable::*)(ResNum) const)
                    &AtomTable::nMissingParameters )
        .def( "nMissingParameters", (int (AtomTable::*)(ResID) const)
                    &AtomTable::nMissingParameters )
        .def( "nMissingParameters", (int (AtomTable::*)(CutGroupNum) const)
                    &AtomTable::nMissingParameters )
        .def( "nMissingParameters", (int (AtomTable::*)(CutGroupID) const)
                    &AtomTable::nMissingParameters )

        .def( "assignedParameter", (bool (AtomTable::*)(const CGAtomID&) const)
                    &AtomTable::assignedParameter )
        .def( "assignedParameter", (bool (AtomTable::*)(const CGNumAtomID&) const)
                    &AtomTable::assignedParameter )
        .def( "assignedParameter", (bool (AtomTable::*)(const ResNumAtomID&) const)
                    &AtomTable::assignedParameter )
        .def( "assignedParameter", (bool (AtomTable::*)(const ResIDAtomID&) const)
                    &AtomTable::assignedParameter )
        .def( "assignedParameter", (bool (AtomTable::*)(const AtomIndex&) const)
                    &AtomTable::assignedParameter )
        .def( "assignedParameter", (bool (AtomTable::*)(AtomID) const)
                    &AtomTable::assignedParameter )

        .def( "hasMissingParameters", (bool (AtomTable::*)() const)
                    &AtomTable::hasMissingParameters )
        .def( "hasMissingParameters", (bool (AtomTable::*)(ResNum) const)
                    &AtomTable::hasMissingParameters )
        .def( "hasMissingParameters", (bool (AtomTable::*)(ResID) const)
                    &AtomTable::hasMissingParameters )
        .def( "hasMissingParameters", (bool (AtomTable::*)(CutGroupNum) const)
                    &AtomTable::hasMissingParameters )
        .def( "hasMissingParameters", (bool (AtomTable::*)(CutGroupID) const)
                    &AtomTable::hasMissingParameters )

        .def( "missingParameters", (QSet<AtomIndex> (AtomTable::*)() const)
                    &AtomTable::missingParameters )
        .def( "missingParameters", (QSet<AtomIndex> (AtomTable::*)(ResNum) const)
                    &AtomTable::missingParameters )
        .def( "missingParameters", (QSet<AtomIndex> (AtomTable::*)(ResID) const)
                    &AtomTable::missingParameters )
        .def( "missingParameters", (QSet<AtomIndex> (AtomTable::*)(CutGroupNum) const)
                    &AtomTable::missingParameters )
        .def( "missingParameters", (QSet<AtomIndex> (AtomTable::*)(CutGroupID) const)
                    &AtomTable::missingParameters )

        .def( "clear", (void (AtomTable::*)())
                    &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(const CGAtomID&))
                    &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(const CGNumAtomID&))
                    &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(const ResNumAtomID&))
                    &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(const ResIDAtomID&))
                    &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(const AtomIndex&))
                    &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(AtomID))
                    &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(ResNum))
                    &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(ResID))
                    &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(CutGroupNum))
                    &AtomTable::clear )
        .def( "clear", (void (AtomTable::*)(CutGroupID))
                    &AtomTable::clear )

        .def( "assertHaveParameter", (const CGAtomID& (AtomTable::*)(AtomID) const)
                    &AtomTable::assertHaveParameter,
                    return_value_policy<copy_const_reference>() )
        .def( "assertHaveParameter", (const CGAtomID&
                                        (AtomTable::*)(const CGAtomID&) const)
                    &AtomTable::assertHaveParameter,
                    return_value_policy<copy_const_reference>() )
        .def( "assertHaveParameter", (const CGAtomID&
                                        (AtomTable::*)(const CGNumAtomID&) const)
                    &AtomTable::assertHaveParameter,
                    return_value_policy<copy_const_reference>() )
        .def( "assertHaveParameter", (const CGAtomID&
                                        (AtomTable::*)(const ResNumAtomID&) const)
                    &AtomTable::assertHaveParameter,
                    return_value_policy<copy_const_reference>() )
        .def( "assertHaveParameter", (const CGAtomID&
                                        (AtomTable::*)(const ResIDAtomID&) const)
                    &AtomTable::assertHaveParameter,
                    return_value_policy<copy_const_reference>() )
        .def( "assertHaveParameter", (const CGAtomID&
                                        (AtomTable::*)(const AtomIndex&) const)
                    &AtomTable::assertHaveParameter,
                    return_value_policy<copy_const_reference>() )
    ;
}

}
