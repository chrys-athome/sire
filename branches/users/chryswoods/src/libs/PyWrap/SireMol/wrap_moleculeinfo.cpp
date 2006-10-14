
/**
  * This file contains the boost::python wrapping of a MoleculeInfo
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/moleculeinfo.h"
#include "SireMol/residueinfo.h"
#include "SireMol/atominfogroup.h"

#include "SireMol/cgatomid.h"
#include "SireMol/cgnumatomid.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/residatomid.h"

using namespace boost::python;

namespace SireMol
{

template<class IDX, class VALUE>
const VALUE& __getitem__MoleculeInfo(const MoleculeInfo &molinfo, IDX idx)
{
    return molinfo[idx];
}

void export_MoleculeInfo()
{

    class_<MoleculeInfo>( "MoleculeInfo", init<>() )
    
        .def( init<const MoleculeInfo&>() )

        .def( self == self )
        .def( self != self )

        .def( "__getitem__", &__getitem__MoleculeInfo<CutGroupID,AtomInfoGroup>,
                             return_value_policy<copy_const_reference>() )
        .def( "__getitem__", &__getitem__MoleculeInfo<CutGroupNum,AtomInfoGroup>,
                             return_value_policy<copy_const_reference>() )
      
        .def( "__getitem__", &__getitem__MoleculeInfo<ResID,ResidueInfo>,
                             return_value_policy<copy_const_reference>() )
        .def( "__getitem__", &__getitem__MoleculeInfo<ResNum,ResidueInfo>,
                             return_value_policy<copy_const_reference>() )

        .def( "__getitem__", &__getitem__MoleculeInfo<AtomID,CGAtomID>,
                             return_value_policy<copy_const_reference>() )
        .def( "__getitem__", &__getitem__MoleculeInfo<const AtomIndex&,CGAtomID>,
                             return_value_policy<copy_const_reference>() )
        .def( "__getitem__", &__getitem__MoleculeInfo<const ResNumAtomID&,CGAtomID>,
                             return_value_policy<copy_const_reference>() )
        .def( "__getitem__", &__getitem__MoleculeInfo<const ResIDAtomID&,CGAtomID>,
                             return_value_policy<copy_const_reference>() )
        .def( "__getitem__", &__getitem__MoleculeInfo<const CGAtomID&,CGAtomID>,
                             return_value_policy<copy_const_reference>() )
        .def( "__getitem__", &__getitem__MoleculeInfo<const CGNumAtomID&,CGAtomID>,
                             return_value_policy<copy_const_reference>() )

        .def( "at", (const AtomInfoGroup& (MoleculeInfo::*)(CutGroupID) const)
                             &MoleculeInfo::at,
                             return_value_policy<copy_const_reference>() )
        .def( "at", (const AtomInfoGroup& (MoleculeInfo::*)(CutGroupNum) const)
                             &MoleculeInfo::at,
                             return_value_policy<copy_const_reference>() )
                             
        .def( "at", (const ResidueInfo& (MoleculeInfo::*)(ResID) const)
                             &MoleculeInfo::at,
                             return_value_policy<copy_const_reference>() )
        .def( "at", (const ResidueInfo& (MoleculeInfo::*)(ResNum) const)
                             &MoleculeInfo::at,
                             return_value_policy<copy_const_reference>() )

        .def( "at", (const CGAtomID& (MoleculeInfo::*)(AtomID) const)
                             &MoleculeInfo::at,
                             return_value_policy<copy_const_reference>() )
        .def( "at", (const CGAtomID& (MoleculeInfo::*)(const AtomIndex&) const)
                             &MoleculeInfo::at,
                             return_value_policy<copy_const_reference>() )
        .def( "at", (const CGAtomID& (MoleculeInfo::*)(const ResIDAtomID&) const)
                             &MoleculeInfo::at,
                             return_value_policy<copy_const_reference>() )
        .def( "at", (const CGAtomID& (MoleculeInfo::*)(const ResNumAtomID&) const)
                             &MoleculeInfo::at,
                             return_value_policy<copy_const_reference>() )
        .def( "at", (const CGAtomID& (MoleculeInfo::*)(const CGAtomID&) const)
                             &MoleculeInfo::at,
                             return_value_policy<copy_const_reference>() )
        .def( "at", (const CGAtomID& (MoleculeInfo::*)(const CGNumAtomID&) const)
                             &MoleculeInfo::at,
                             return_value_policy<copy_const_reference>() )

        .def( "atom", (const AtomInfo& (MoleculeInfo::*)(AtomID) const)
                             &MoleculeInfo::atom,
                             return_value_policy<copy_const_reference>() )
        .def( "atom", (const AtomInfo& (MoleculeInfo::*)(const AtomIndex&) const)
                             &MoleculeInfo::atom,
                             return_value_policy<copy_const_reference>() )
        .def( "atom", (const AtomInfo& (MoleculeInfo::*)(const ResIDAtomID&) const)
                             &MoleculeInfo::atom,
                             return_value_policy<copy_const_reference>() )
        .def( "atom", (const AtomInfo& (MoleculeInfo::*)(const ResNumAtomID&) const)
                             &MoleculeInfo::atom,
                             return_value_policy<copy_const_reference>() )
        .def( "atom", (const AtomInfo& (MoleculeInfo::*)(const CGAtomID&) const)
                             &MoleculeInfo::atom,
                             return_value_policy<copy_const_reference>() )
        .def( "atom", (const AtomInfo& (MoleculeInfo::*)(const CGNumAtomID&) const)
                             &MoleculeInfo::atom,
                             return_value_policy<copy_const_reference>() )

        .def( "atomGroup", (const AtomInfoGroup& (MoleculeInfo::*)(CutGroupID) const)
                             &MoleculeInfo::atomGroup,
                             return_value_policy<copy_const_reference>() )
        .def( "atomGroup", (const AtomInfoGroup& (MoleculeInfo::*)(CutGroupNum) const)
                             &MoleculeInfo::atomGroup,
                             return_value_policy<copy_const_reference>() )

        .def( "atomGroups", (QHash<CutGroupID,AtomInfoGroup> (MoleculeInfo::*)() const)
                             &MoleculeInfo::atomGroups )
        .def( "atomGroups", (QHash<CutGroupID,AtomInfoGroup> (MoleculeInfo::*)(ResNum) const)
                             &MoleculeInfo::atomGroups )
        .def( "atomGroups", (QHash<CutGroupID,AtomInfoGroup> (MoleculeInfo::*)(ResID) const)
                             &MoleculeInfo::atomGroups )

        .def( "atomGroups", (QHash<CutGroupID,AtomInfoGroup> (MoleculeInfo::*)(
                                                              const QSet<CutGroupID>&) const)
                             &MoleculeInfo::atomGroups )
        .def( "atomGroups", (QHash<CutGroupID,AtomInfoGroup> (MoleculeInfo::*)(
                                                              const QSet<ResNum>&) const)
                             &MoleculeInfo::atomGroups )
        .def( "atomGroups", (QHash<CutGroupID,AtomInfoGroup> (MoleculeInfo::*)(
                                                              const QSet<ResID>&) const)
                             &MoleculeInfo::atomGroups )
        
        .def( "atoms", (QVector<AtomInfo> (MoleculeInfo::*)() const)
                             &MoleculeInfo::atoms )

        .def( "atoms", (QHash<AtomID,AtomInfo> (MoleculeInfo::*)(
                                                    const QSet<AtomID>&) const)
                             &MoleculeInfo::atoms )
        .def( "atoms", (QHash<CGAtomID,AtomInfo> (MoleculeInfo::*)(
                                                    const QSet<CGAtomID>&) const)
                             &MoleculeInfo::atoms )
        .def( "atoms", (QHash<CGNumAtomID,AtomInfo> (MoleculeInfo::*)(
                                                    const QSet<CGNumAtomID>&) const)
                             &MoleculeInfo::atoms )
        .def( "atoms", (QHash<ResNumAtomID,AtomInfo> (MoleculeInfo::*)(
                                                    const QSet<ResNumAtomID>&) const)
                             &MoleculeInfo::atoms )
        .def( "atoms", (QHash<ResIDAtomID,AtomInfo> (MoleculeInfo::*)(
                                                    const QSet<ResIDAtomID>&) const)
                             &MoleculeInfo::atoms )
        .def( "atoms", (QHash<AtomIndex,AtomInfo> (MoleculeInfo::*)(
                                                    const QSet<AtomIndex>&) const)
                             &MoleculeInfo::atoms )
        
        .def( "atoms", (QVector<AtomInfo> (MoleculeInfo::*)(CutGroupID) const)
                             &MoleculeInfo::atoms )
        .def( "atoms", (QVector<AtomInfo> (MoleculeInfo::*)(CutGroupNum) const)
                             &MoleculeInfo::atoms )

        .def( "atoms", (QHash< CutGroupID, QVector<AtomInfo> > (MoleculeInfo::*)(
                                                    const QSet<CutGroupID>&) const)
                             &MoleculeInfo::atoms )
        .def( "atoms", (QHash< CutGroupNum, QVector<AtomInfo> > (MoleculeInfo::*)(
                                                    const QSet<CutGroupNum>&) const)
                             &MoleculeInfo::atoms )

        .def( "atoms", (QVector<AtomInfo> (MoleculeInfo::*)(ResNum) const)
                             &MoleculeInfo::atoms )
        .def( "atoms", (QHash< ResNum, QVector<AtomInfo> > (MoleculeInfo::*)(
                                                    const QSet<ResNum>&) const)
                             &MoleculeInfo::atoms )

        .def( "atoms", (QVector<AtomInfo> (MoleculeInfo::*)(ResID) const)
                             &MoleculeInfo::atoms )
        .def( "atoms", (QHash< ResID, QVector<AtomInfo> > (MoleculeInfo::*)(
                                                    const QSet<ResID>&) const)
                             &MoleculeInfo::atoms )

        .def( "residue", (const ResidueInfo& (MoleculeInfo::*)(AtomID) const)
                             &MoleculeInfo::residue,
                             return_value_policy<copy_const_reference>() )
        .def( "residue", (const ResidueInfo& (MoleculeInfo::*)(ResNum) const)
                             &MoleculeInfo::residue,
                             return_value_policy<copy_const_reference>() )
        .def( "residue", (const ResidueInfo& (MoleculeInfo::*)(ResID) const)
                             &MoleculeInfo::residue,
                             return_value_policy<copy_const_reference>() )

        .def( "toString", (QString (MoleculeInfo::*)() const)
                             &MoleculeInfo::toString )

        .def( "name", (QString (MoleculeInfo::*)() const)
                             &MoleculeInfo::name )

        .def( "residueName", (QString (MoleculeInfo::*)(ResNum) const)
                             &MoleculeInfo::residueName )
        .def( "residueName", (QString (MoleculeInfo::*)(ResID) const)
                             &MoleculeInfo::residueName )
    
        .def( "residueNumber", (ResNum (MoleculeInfo::*)(ResID) const)
                             &MoleculeInfo::residueNumber )
        .def( "residueNumber", (ResNum (MoleculeInfo::*)(const QString&) const)
                             &MoleculeInfo::residueNumber )

        .def( "nResidues", (int (MoleculeInfo::*)() const)
                             &MoleculeInfo::nResidues )
        .def( "nCutGroups", (int (MoleculeInfo::*)() const)
                             &MoleculeInfo::nCutGroups )

        .def( "cutGroupID", (CutGroupID (MoleculeInfo::*)(CutGroupNum) const)
                             &MoleculeInfo::cutGroupID )
        .def( "cutGroupNum", (CutGroupNum (MoleculeInfo::*)(CutGroupID) const)
                             &MoleculeInfo::cutGroupNum )

        .def( "nAtoms", (int (MoleculeInfo::*)() const)
                             &MoleculeInfo::nAtoms )
        .def( "nAtoms", (int (MoleculeInfo::*)(ResNum) const)
                             &MoleculeInfo::nAtoms )
        .def( "nAtoms", (int (MoleculeInfo::*)(ResID) const)
                             &MoleculeInfo::nAtoms )
        .def( "nAtoms", (int (MoleculeInfo::*)(CutGroupID) const)
                             &MoleculeInfo::nAtoms )
        .def( "nAtoms", (int (MoleculeInfo::*)(CutGroupNum) const)
                             &MoleculeInfo::nAtoms )

        .def( "cutGroupNums", (QVector<CutGroupNum> (MoleculeInfo::*)() const)
                             &MoleculeInfo::cutGroupNums )

        .def( "residueNumbers", (QVector<ResNum> (MoleculeInfo::*)() const)
                             &MoleculeInfo::residueNumbers )
        .def( "residueNumbers", (QVector<ResNum> (MoleculeInfo::*)(const QString&) const)
                             &MoleculeInfo::residueNumbers )
        .def( "residueNumbers", (QVector<ResNum> (MoleculeInfo::*)(CutGroupID) const)
                             &MoleculeInfo::residueNumbers )
        .def( "residueNumbers", (QVector<ResNum> (MoleculeInfo::*)(CutGroupNum) const)
                             &MoleculeInfo::residueNumbers )
    
        .def( "residueNames", (QStringList (MoleculeInfo::*)() const)
                             &MoleculeInfo::residueNames )
                             
        .def( "atomNames", (QStringList (MoleculeInfo::*)(ResNum) const)
                             &MoleculeInfo::atomNames )
        .def( "atomNames", (QStringList (MoleculeInfo::*)(ResID) const)
                             &MoleculeInfo::atomNames )

        .def( "contains", (bool (MoleculeInfo::*)(CutGroupID) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(CutGroupNum) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(ResNum) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(ResID) const)
                             &MoleculeInfo::contains )

        .def( "contains", (bool (MoleculeInfo::*)(ResNum,const QString&) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(const AtomIndex&) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(CutGroupID,AtomID) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(const CGAtomID&) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(CutGroupNum,AtomID) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(const CGNumAtomID&) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(ResNum,AtomID) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(const ResNumAtomID&) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(ResID,AtomID) const)
                             &MoleculeInfo::contains )
        .def( "contains", (bool (MoleculeInfo::*)(const ResIDAtomID&) const)
                             &MoleculeInfo::contains )

        .def( "isEmpty", (bool (MoleculeInfo::*)() const)
                             &MoleculeInfo::isEmpty )
        .def( "isEmpty", (bool (MoleculeInfo::*)(ResNum) const)
                             &MoleculeInfo::isEmpty )
        .def( "isEmpty", (bool (MoleculeInfo::*)(ResID) const)
                             &MoleculeInfo::isEmpty )
        .def( "isEmpty", (bool (MoleculeInfo::*)(CutGroupNum) const)
                             &MoleculeInfo::isEmpty )
        .def( "isEmpty", (bool (MoleculeInfo::*)(CutGroupID) const)
                             &MoleculeInfo::isEmpty )

        .def( "assertResidueExists", (void (MoleculeInfo::*)(ResNum) const)
                             &MoleculeInfo::assertResidueExists )
        .def( "assertResidueExists", (void (MoleculeInfo::*)(ResID) const)
                             &MoleculeInfo::assertResidueExists )

        .def( "assertCutGroupExists", (void (MoleculeInfo::*)(CutGroupID) const)
                             &MoleculeInfo::assertCutGroupExists )
        .def( "assertCutGroupExists", (void (MoleculeInfo::*)(CutGroupNum) const)
                             &MoleculeInfo::assertCutGroupExists )

        .def( "assertAtomExists", (void (MoleculeInfo::*)(AtomID) const)
                             &MoleculeInfo::assertAtomExists )
        .def( "assertAtomExists", (void (MoleculeInfo::*)(const AtomIndex&) const)
                             &MoleculeInfo::assertAtomExists )
        .def( "assertAtomExists", (void (MoleculeInfo::*)(const CGAtomID&) const)
                             &MoleculeInfo::assertAtomExists )
        .def( "assertAtomExists", (void (MoleculeInfo::*)(const CGNumAtomID&) const)
                             &MoleculeInfo::assertAtomExists )
        .def( "assertAtomExists", (void (MoleculeInfo::*)(const ResNumAtomID&) const)
                             &MoleculeInfo::assertAtomExists )
        .def( "assertAtomExists", (void (MoleculeInfo::*)(const ResIDAtomID&) const)
                             &MoleculeInfo::assertAtomExists )

        .def( "assertNAtoms", (void (MoleculeInfo::*)(int) const)
                             &MoleculeInfo::assertNAtoms )
    ;


}


}
