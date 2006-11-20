
/**
  * This file contains the boost::python wrapping of a ResidueInfo
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
const VALUE& __getitem__ResidueInfo(const ResidueInfo &resinfo, IDX idx)
{
    return resinfo[idx];
}

void  
SIREMOL_EXPORT
export_ResidueInfo()
{

    class_<ResidueInfo>( "ResidueInfo", init<>() )
    
        .def( init<const ResidueInfo&>() )
        
        .def( self == self )
        .def( self != self )
        
        .def( "__getitem__", &__getitem__ResidueInfo<AtomID,CGAtomID>,
                             return_value_policy<copy_const_reference>() )
        .def( "__getitem__", &__getitem__ResidueInfo<const QString&,CGAtomID>,
                             return_value_policy<copy_const_reference>() )

        .def( "__getitem__", &__getitem__ResidueInfo<CutGroupID,AtomInfoGroup>,
                             return_value_policy<copy_const_reference>() )
        .def( "__getitem__", &__getitem__ResidueInfo<CutGroupNum,AtomInfoGroup>,
                             return_value_policy<copy_const_reference>() )
    
        .def( "at", (const CGAtomID& (ResidueInfo::*)(AtomID) const)
                             &ResidueInfo::at,
                             return_value_policy<copy_const_reference>() )
        .def( "at", (const CGAtomID& (ResidueInfo::*)(const QString&) const)
                             &ResidueInfo::at,
                             return_value_policy<copy_const_reference>() )

        .def( "at", (const AtomInfoGroup& (ResidueInfo::*)(CutGroupID) const)
                             &ResidueInfo::at,
                             return_value_policy<copy_const_reference>() )
        .def( "at", (const AtomInfoGroup& (ResidueInfo::*)(CutGroupNum) const)
                             &ResidueInfo::at,
                             return_value_policy<copy_const_reference>() )

        .def( "atom", (const AtomInfo& (ResidueInfo::*)(AtomID) const)
                             &ResidueInfo::atom,
                             return_value_policy<copy_const_reference>() )
        .def( "atom", (const AtomInfo& (ResidueInfo::*)(const QString&) const)
                             &ResidueInfo::atom,
                             return_value_policy<copy_const_reference>() )
        .def( "atom", (const AtomInfo& (ResidueInfo::*)(const CGAtomID&) const)
                             &ResidueInfo::atom,
                             return_value_policy<copy_const_reference>() )
        .def( "atom", (const AtomInfo& (ResidueInfo::*)(const CGAtomID&) const)
                             &ResidueInfo::atom,
                             return_value_policy<copy_const_reference>() )

        .def( "atomGroup", (const AtomInfoGroup& (ResidueInfo::*)(CutGroupID) const)
                             &ResidueInfo::atomGroup,
                             return_value_policy<copy_const_reference>() )
        .def( "atomGroup", (const AtomInfoGroup& (ResidueInfo::*)(CutGroupNum) const)
                             &ResidueInfo::atomGroup,
                             return_value_policy<copy_const_reference>() )

        .def( "atomGroups", (QHash<CutGroupID,AtomInfoGroup> (ResidueInfo::*)() const)
                             &ResidueInfo::atomGroups )
        .def( "atomGroups", (QHash<CutGroupID,AtomInfoGroup> (ResidueInfo::*)(
                                                        const QSet<CutGroupID>&) const)
                             &ResidueInfo::atomGroups )
        .def( "atomGroups", (QHash<CutGroupNum,AtomInfoGroup> (ResidueInfo::*)(
                                                        const QSet<CutGroupNum>&) const)
                             &ResidueInfo::atomGroups )

        .def( "atoms", (QVector<AtomInfo> (ResidueInfo::*)() const)
                             &ResidueInfo::atoms )

        .def( "atoms", (QHash<AtomID,AtomInfo> (ResidueInfo::*)(
                                                    const QSet<AtomID>&) const)
                             &ResidueInfo::atoms )
        .def( "atoms", (QHash<CGAtomID,AtomInfo> (ResidueInfo::*)(
                                                    const QSet<CGAtomID>&) const)
                             &ResidueInfo::atoms )
        .def( "atoms", (QHash<CGNumAtomID,AtomInfo> (ResidueInfo::*)(
                                                    const QSet<CGNumAtomID>&) const)
                             &ResidueInfo::atoms )
        .def( "atoms", (QHash<QString,AtomInfo> (ResidueInfo::*)(
                                                    const QSet<QString>&) const)
                             &ResidueInfo::atoms )

        .def( "atoms", (QVector<AtomInfo> (ResidueInfo::*)(CutGroupID) const)
                             &ResidueInfo::atoms )
        .def( "atoms", (QVector<AtomInfo> (ResidueInfo::*)(CutGroupNum) const)
                             &ResidueInfo::atoms )

        .def( "atoms", (QHash< CutGroupID, QVector<AtomInfo> > 
                                          (ResidueInfo::*)(const QSet<CutGroupID>&) const)
                             &ResidueInfo::atoms )
        .def( "atoms", (QHash< CutGroupNum, QVector<AtomInfo> > 
                                          (ResidueInfo::*)(const QSet<CutGroupNum>&) const)
                             &ResidueInfo::atoms )

        .def( "indicies", (const QVector<CGAtomID>& (ResidueInfo::*)() const)
                             &ResidueInfo::indicies,
                             return_value_policy<copy_const_reference>() )
        
        .def( "cutGroupIDs", (const QSet<CutGroupID>& (ResidueInfo::*)() const)
                             &ResidueInfo::cutGroupIDs,
                             return_value_policy<copy_const_reference>() )
    
        .def( "cutGroupNums", (QSet<CutGroupNum> (ResidueInfo::*)() const)
                             &ResidueInfo::cutGroupNums )

        .def( "toString", (QString (ResidueInfo::*)() const)
                             &ResidueInfo::toString )

        .def( "name", (QString (ResidueInfo::*)() const)
                             &ResidueInfo::name )
        .def( "resName", (QString (ResidueInfo::*)() const)
                             &ResidueInfo::resName )

        .def( "number", (ResNum (ResidueInfo::*)() const)
                             &ResidueInfo::number )
        .def( "resNum", (ResNum (ResidueInfo::*)() const)
                             &ResidueInfo::resNum )

        .def( "nCutGroups", (int (ResidueInfo::*)() const)
                             &ResidueInfo::nCutGroups )

        .def( "nAtoms", (int (ResidueInfo::*)() const)
                             &ResidueInfo::nAtoms )
        .def( "nAtoms", (int (ResidueInfo::*)(CutGroupID) const)
                             &ResidueInfo::nAtoms )
        .def( "nAtoms", (int (ResidueInfo::*)(CutGroupNum) const)
                             &ResidueInfo::nAtoms )

        .def( "atomName", (QString (ResidueInfo::*)(AtomID) const)
                             &ResidueInfo::atomName )
    
        .def( "atomNames", (QHash<AtomID,QString> (ResidueInfo::*)(
                                              const QSet<AtomID>&) const)
                             &ResidueInfo::atomNames )

        .def( "atomNames", (QStringList (ResidueInfo::*)() const)
                             &ResidueInfo::atomNames )

        .def( "contains", (bool (ResidueInfo::*)(const QString&) const)
                             &ResidueInfo::contains )
        .def( "contains", (bool (ResidueInfo::*)(const AtomIndex&) const)
                             &ResidueInfo::contains )
        .def( "contains", (bool (ResidueInfo::*)(AtomID) const)
                             &ResidueInfo::contains )
        .def( "contains", (bool (ResidueInfo::*)(CutGroupID) const)
                             &ResidueInfo::contains )
        .def( "contains", (bool (ResidueInfo::*)(CutGroupID,AtomID) const)
                             &ResidueInfo::contains )
        .def( "contains", (bool (ResidueInfo::*)(const CGAtomID&) const)
                             &ResidueInfo::contains )
        .def( "contains", (bool (ResidueInfo::*)(CutGroupNum) const)
                             &ResidueInfo::contains )
        .def( "contains", (bool (ResidueInfo::*)(CutGroupNum,AtomID) const)
                             &ResidueInfo::contains )
        .def( "contains", (bool (ResidueInfo::*)(const CGNumAtomID&) const)
                             &ResidueInfo::contains )

        .def( "isEmpty", (bool (ResidueInfo::*)() const)
                             &ResidueInfo::isEmpty )
        .def( "isEmpty", (bool (ResidueInfo::*)(CutGroupID) const)
                             &ResidueInfo::isEmpty )
        .def( "isEmpty", (bool (ResidueInfo::*)(CutGroupNum) const)
                             &ResidueInfo::isEmpty )

        .def( "assertAtomExists", (void (ResidueInfo::*)(const QString&) const)
                             &ResidueInfo::assertAtomExists )
        .def( "assertAtomExists", (void (ResidueInfo::*)(AtomID) const)
                             &ResidueInfo::assertAtomExists )

        .def( "assertSameResidue", (void (ResidueInfo::*)(const AtomIndex&) const)
                             &ResidueInfo::assertSameResidue )
        .def( "assertSameResidue", (void (ResidueInfo::*)(const QSet<AtomIndex>&) const)
                             &ResidueInfo::assertSameResidue )
        .def( "assertSameResidue", (void (ResidueInfo::*)(CutGroupID) const)
                             &ResidueInfo::assertSameResidue )
        .def( "assertSameResidue", (void (ResidueInfo::*)(CutGroupNum) const)
                             &ResidueInfo::assertSameResidue )

        .def( "assertNAtoms", (void (ResidueInfo::*)(int) const)
                             &ResidueInfo::assertNAtoms )
    ;

}

}
