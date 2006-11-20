
/**
  * This file contains the boost::python wrapping of MolBondInfo
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireMM/molbondinfo.h"
#include "SireMM/resbondinfo.h"

#include "wrapInternalInfo.hpp"

using namespace boost::python;
using namespace SireMol;

namespace SireMM
{

MolBondInfo::const_iterator wrap_bonds_QSet(
                        const MolBondInfo &m, const QSet<ResNum> &resnums)
{
    return m.bonds(resnums);
}
    
MolBondInfo::const_iterator wrap_bonds_QList(
                        const MolBondInfo &m, const QList<ResNum> &resnums)
{
    return m.bonds(resnums);
}

MolBondInfo::const_iterator wrap_interBonds_QSet(
                        const MolBondInfo &m, const QSet<ResNum> &resnums)
{
    return m.interBonds(resnums);
}
    
MolBondInfo::const_iterator wrap_interBonds_QList(
                        const MolBondInfo &m, const QList<ResNum> &resnums)
{
    return m.interBonds(resnums);
}

MolBondInfo::const_iterator wrap_intraBonds_QSet(
                        const MolBondInfo &m, const QSet<ResNum> &resnums)
{
    return m.intraBonds(resnums);
}
    
MolBondInfo::const_iterator wrap_intraBonds_QList(
                        const MolBondInfo &m, const QList<ResNum> &resnums)
{
    return m.intraBonds(resnums);
}

void wrap_addBonds_QList(MolBondInfo &m, const QList<Bond> &bonds)
{
    m.addBonds(bonds);
}

void wrap_addBonds_QSet(MolBondInfo &m, const QSet<Bond> &bonds)
{
    m.addBonds(bonds);
}

void wrap_removeBonds_QList(MolBondInfo &m, const QList<Bond> &bonds)
{
    m.removeBonds(bonds);
}

void wrap_removeBonds_QSet(MolBondInfo &m, const QSet<Bond> &bonds)
{
    m.removeBonds(bonds);
}

void  
SIREMM_EXPORT
export_MolBondInfo()
{
    //wrap the iterator type
    wrap_InternalIterator<Bond>("BondInfoIterator");
    
    //wrap the MolBondInfo class
    class_<MolBondInfo> wrap_MolBondInfo("MolBondInfo", init<>());
    
    //wrap the common parts of this object
    wrap_MolInternalInfo<MolBondInfo>( wrap_MolBondInfo );
    
    //now wrap the specific parts
    int (MolBondInfo::*wrap_nBonds1)() const = &MolBondInfo::nBonds;
    int (MolBondInfo::*wrap_nBonds2)(GroupID) const = &MolBondInfo::nBonds;
    int (MolBondInfo::*wrap_nBonds3)(ResNum) const = &MolBondInfo::nBonds;
    int (MolBondInfo::*wrap_nBonds4)(ResNum, ResNum) const = &MolBondInfo::nBonds;

    int (MolBondInfo::*wrap_nInterBonds1)() const = &MolBondInfo::nInterBonds;
    int (MolBondInfo::*wrap_nInterBonds2)(ResNum resnum) const = &MolBondInfo::nInterBonds;
    
    int (MolBondInfo::*wrap_nIntraBonds1)() const = &MolBondInfo::nIntraBonds;
    int (MolBondInfo::*wrap_nIntraBonds2)(ResNum resnum) const = &MolBondInfo::nIntraBonds;

    MolBondInfo::const_iterator (MolBondInfo::*wrap_bonds1)() const = &MolBondInfo::bonds;
    MolBondInfo::const_iterator (MolBondInfo::*wrap_bonds2)(ResNum) const
                                                              = &MolBondInfo::bonds;
    MolBondInfo::const_iterator (MolBondInfo::*wrap_bonds3)(ResNum, ResNum) const
                                                              = &MolBondInfo::bonds;
    
    MolBondInfo::const_iterator (MolBondInfo::*wrap_interBonds1)(ResNum) const
                                                              = &MolBondInfo::interBonds;
    MolBondInfo::const_iterator (MolBondInfo::*wrap_interBonds2)() const
                                                              = &MolBondInfo::interBonds;
    
    MolBondInfo::const_iterator (MolBondInfo::*wrap_intraBonds1)(ResNum) const
                                                              = &MolBondInfo::intraBonds;
    MolBondInfo::const_iterator (MolBondInfo::*wrap_intraBonds2)() const
                                                              = &MolBondInfo::intraBonds;
    
    void (MolBondInfo::*wrap_removeBond1)(const Bond&) = &MolBondInfo::removeBond;
    void (MolBondInfo::*wrap_removeBond2)(const GroupIndexID&) = &MolBondInfo::removeBond;
    
    wrap_MolBondInfo
        .def( "addBond", &MolBondInfo::addBond )
        .def( "removeBond", wrap_removeBond1 )
        .def( "removeBond", wrap_removeBond2 )
        .def( "addBonds", &wrap_addBonds_QList )
        .def( "removeBonds", &wrap_removeBonds_QList )
        .def( "addBonds", &wrap_addBonds_QSet )
        .def( "removeBonds", &wrap_removeBonds_QSet )
        .def( "nBonds", wrap_nBonds1 )
        .def( "nBonds", wrap_nBonds2 )
        .def( "nBonds", wrap_nBonds3 )
        .def( "nBonds", wrap_nBonds4 )
        .def( "nIntraBonds", wrap_nIntraBonds1 )
        .def( "nIntraBonds", wrap_nIntraBonds2 )
        .def( "nInterBonds", wrap_nInterBonds1 )
        .def( "nInterBonds", wrap_nInterBonds2 )
        .def( "residuesBonded", &MolBondInfo::residuesBonded )
        .def( "bonds", wrap_bonds1 )
        .def( "bonds", wrap_bonds2 )
        .def( "bonds", wrap_bonds3 )
        .def( "interBonds", wrap_interBonds1 )
        .def( "interBonds", wrap_interBonds2 )
        .def( "intraBonds", wrap_intraBonds1 )
        .def( "intraBonds", wrap_intraBonds2 )
        .def( "bonds", &wrap_bonds_QSet )
        .def( "interBonds", &wrap_interBonds_QSet )
        .def( "intraBonds", &wrap_intraBonds_QSet )
        .def( "bonds", &wrap_bonds_QList )
        .def( "interBonds", &wrap_interBonds_QList )
        .def( "intraBonds", &wrap_intraBonds_QList )
    ;

}

}
