
/**
  * This file contains the boost::python wrapping of MolDihedralInfo
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireMM/moldihedralinfo.h"
#include "SireMM/resdihedralinfo.h"

#include "wrapInternalInfo.hpp"

using namespace boost::python;
using namespace SireMol;

namespace SireMM
{

MolDihedralInfo::const_iterator wrap_dihedrals_QSet(
                        const MolDihedralInfo &m, const QSet<ResNum> &resnums)
{
    return m.dihedrals(resnums);
}
    
MolDihedralInfo::const_iterator wrap_dihedrals_QList(
                        const MolDihedralInfo &m, const QList<ResNum> &resnums)
{
    return m.dihedrals(resnums);
}

MolDihedralInfo::const_iterator wrap_interDihedrals_QSet(
                        const MolDihedralInfo &m, const QSet<ResNum> &resnums)
{
    return m.interDihedrals(resnums);
}
    
MolDihedralInfo::const_iterator wrap_interDihedrals_QList(
                        const MolDihedralInfo &m, const QList<ResNum> &resnums)
{
    return m.interDihedrals(resnums);
}

MolDihedralInfo::const_iterator wrap_intraDihedrals_QSet(
                        const MolDihedralInfo &m, const QSet<ResNum> &resnums)
{
    return m.intraDihedrals(resnums);
}
    
MolDihedralInfo::const_iterator wrap_intraDihedrals_QList(
                        const MolDihedralInfo &m, const QList<ResNum> &resnums)
{
    return m.intraDihedrals(resnums);
}

void wrap_addDihedrals_QList(MolDihedralInfo &m, const QList<Dihedral> &bonds)
{
    m.addDihedrals(bonds);
}

void wrap_addDihedrals_QSet(MolDihedralInfo &m, const QSet<Dihedral> &bonds)
{
    m.addDihedrals(bonds);
}

void wrap_removeDihedrals_QList(MolDihedralInfo &m, const QList<Dihedral> &bonds)
{
    m.removeDihedrals(bonds);
}

void wrap_removeDihedrals_QSet(MolDihedralInfo &m, const QSet<Dihedral> &bonds)
{
    m.removeDihedrals(bonds);
}

void  
SIREMM_EXPORT
export_MolDihedralInfo()
{
    //wrap the iterator type
    wrap_InternalIterator<Dihedral>("DihedralInfoIterator");
    
    //wrap the MolDihedralInfo class
    class_<MolDihedralInfo> wrap_MolDihedralInfo("MolDihedralInfo", init<>());
    
    //wrap the common parts of this object
    wrap_MolInternalInfo<MolDihedralInfo>( wrap_MolDihedralInfo );
    
    //now wrap the specific parts
    int (MolDihedralInfo::*wrap_nDihedrals1)() const = &MolDihedralInfo::nDihedrals;
    int (MolDihedralInfo::*wrap_nDihedrals2)(GroupID) const = &MolDihedralInfo::nDihedrals;
    int (MolDihedralInfo::*wrap_nDihedrals3)(ResNum) const = &MolDihedralInfo::nDihedrals;
    int (MolDihedralInfo::*wrap_nDihedrals4)(ResNum, ResNum) const 
                                            = &MolDihedralInfo::nDihedrals;
    int (MolDihedralInfo::*wrap_nDihedrals5)(ResNum, ResNum, ResNum) const 
                                            = &MolDihedralInfo::nDihedrals;
    int (MolDihedralInfo::*wrap_nDihedrals6)(ResNum, ResNum, ResNum, ResNum) const 
                                            = &MolDihedralInfo::nDihedrals;

    int (MolDihedralInfo::*wrap_nInterDihedrals1)() const 
                                            = &MolDihedralInfo::nInterDihedrals;
    int (MolDihedralInfo::*wrap_nInterDihedrals2)(ResNum resnum) const 
                                            = &MolDihedralInfo::nInterDihedrals;
    
    int (MolDihedralInfo::*wrap_nIntraDihedrals1)() const 
                                            = &MolDihedralInfo::nIntraDihedrals;
    int (MolDihedralInfo::*wrap_nIntraDihedrals2)(ResNum) const 
                                            = &MolDihedralInfo::nIntraDihedrals;

    MolDihedralInfo::const_iterator (MolDihedralInfo::*wrap_dihedrals1)() const 
                                            = &MolDihedralInfo::dihedrals;
    MolDihedralInfo::const_iterator (MolDihedralInfo::*wrap_dihedrals2)(ResNum) const
                                            = &MolDihedralInfo::dihedrals;
    MolDihedralInfo::const_iterator (MolDihedralInfo::*wrap_dihedrals3)(ResNum, ResNum) const
                                            = &MolDihedralInfo::dihedrals;
    MolDihedralInfo::const_iterator (MolDihedralInfo::*wrap_dihedrals4)(ResNum,ResNum,
                                                                        ResNum) const
                                            = &MolDihedralInfo::dihedrals;
    MolDihedralInfo::const_iterator (MolDihedralInfo::*wrap_dihedrals5)(ResNum,ResNum,
                                                                        ResNum,ResNum) const
                                            = &MolDihedralInfo::dihedrals;
    
    MolDihedralInfo::const_iterator (MolDihedralInfo::*wrap_interDihedrals1)(ResNum) const
                                            = &MolDihedralInfo::interDihedrals;
    MolDihedralInfo::const_iterator (MolDihedralInfo::*wrap_interDihedrals2)() const
                                            = &MolDihedralInfo::interDihedrals;
    
    MolDihedralInfo::const_iterator (MolDihedralInfo::*wrap_intraDihedrals1)(ResNum) const
                                            = &MolDihedralInfo::intraDihedrals;
    MolDihedralInfo::const_iterator (MolDihedralInfo::*wrap_intraDihedrals2)() const
                                            = &MolDihedralInfo::intraDihedrals;
    
    bool (MolDihedralInfo::*wrap_residuesDihedraled1)(ResNum, ResNum) const
                                            = &MolDihedralInfo::residuesDihedraled;
    bool (MolDihedralInfo::*wrap_residuesDihedraled2)(ResNum, ResNum, ResNum) const
                                            = &MolDihedralInfo::residuesDihedraled;
    bool (MolDihedralInfo::*wrap_residuesDihedraled3)(ResNum, ResNum, ResNum, ResNum) const
                                            = &MolDihedralInfo::residuesDihedraled;
    
    void (MolDihedralInfo::*wrap_removeDihedral1)(const Dihedral&) 
                                                  = &MolDihedralInfo::removeDihedral;
    void (MolDihedralInfo::*wrap_removeDihedral2)(const GroupIndexID&) 
                                                  = &MolDihedralInfo::removeDihedral;
    
    wrap_MolDihedralInfo
        .def( "addDihedral", &MolDihedralInfo::addDihedral )
        .def( "removeDihedral", wrap_removeDihedral1 )
        .def( "removeDihedral", wrap_removeDihedral2 )
        .def( "addDihedrals", &wrap_addDihedrals_QList )
        .def( "removeDihedrals", &wrap_removeDihedrals_QList )
        .def( "addDihedrals", &wrap_addDihedrals_QSet )
        .def( "removeDihedrals", &wrap_removeDihedrals_QSet )
        .def( "nDihedrals", wrap_nDihedrals1 )
        .def( "nDihedrals", wrap_nDihedrals2 )
        .def( "nDihedrals", wrap_nDihedrals3 )
        .def( "nDihedrals", wrap_nDihedrals4 )
        .def( "nDihedrals", wrap_nDihedrals5 )
        .def( "nDihedrals", wrap_nDihedrals6 )
        .def( "nIntraDihedrals", wrap_nIntraDihedrals1 )
        .def( "nIntraDihedrals", wrap_nIntraDihedrals2 )
        .def( "nInterDihedrals", wrap_nInterDihedrals1 )
        .def( "nInterDihedrals", wrap_nInterDihedrals2 )
        .def( "residuesDihedrald", wrap_residuesDihedraled1 )
        .def( "residuesDihedrald", wrap_residuesDihedraled2 )
        .def( "residuesDihedrald", wrap_residuesDihedraled3 )
        .def( "dihedrals", wrap_dihedrals1 )
        .def( "dihedrals", wrap_dihedrals2 )
        .def( "dihedrals", wrap_dihedrals3 )
        .def( "dihedrals", wrap_dihedrals4 )
        .def( "dihedrals", wrap_dihedrals5 )
        .def( "interDihedrals", wrap_interDihedrals1 )
        .def( "interDihedrals", wrap_interDihedrals2 )
        .def( "intraDihedrals", wrap_intraDihedrals1 )
        .def( "intraDihedrals", wrap_intraDihedrals2 )
        .def( "dihedrals", &wrap_dihedrals_QSet )
        .def( "interDihedrals", &wrap_interDihedrals_QSet )
        .def( "intraDihedrals", &wrap_intraDihedrals_QSet )
        .def( "dihedrals", &wrap_dihedrals_QList )
        .def( "interDihedrals", &wrap_interDihedrals_QList )
        .def( "intraDihedrals", &wrap_intraDihedrals_QList )
    ;

}

}
