#ifndef PYWRAP_SIREMM_WRAP_BONDTABLE_HPP
#define PYWRAP_SIREMM_WRAP_BONDTABLE_HPP

#include <Python.h>
#include <boost/python.hpp>

#include "SireMM/bondtable.hpp"

using namespace boost::python;

SIRE_BEGIN_HEADER

namespace SireMM
{

template<class Param>
void wrap_BondTable(const char *wrapname)
{
    typedef BondTableT<Param> BT;
    
    class_<BT, bases<InternalTable<MolBondInfo,Param> >, boost::noncopyable>
                    (wrapname, no_init)
    
        .def( "addBond", (void (BT::*)(const Bond&))
                    &BT::addBond )
        .def( "addBond", (void (BT::*)(const Bond&, const Param&))
                    &BT::addBond )
    
        .def( "addBonds", (void (BT::*)(const QSet<Bond>&))
                    &BT::addBonds )
        .def( "addBonds", (void (BT::*)(const QList<Bond>&))
                    &BT::addBonds )

        .def( "assignedBondParameter", (bool (BT::*)(const Bond&) const)
                    &BT::assignedBondParameter )
        .def( "assignedBondParameter", (bool (BT::*)(const GroupIndexID&) const)
                    &BT::assignedBondParameter )
    
        .def( "bonds", (QVector<Bond> (BT::*)() const)
                    &BT::bonds )
        .def( "bonds", (QVector<Bond> (BT::*)(ResNum) const)
                    &BT::bonds )
        .def( "bonds", (QVector<Bond> (BT::*)(GroupID) const)
                    &BT::bonds )
    
        .def( "bondsByGroup", (GroupedVector<GroupIndexID,Bond> (BT::*)() const)
                    &BT::bondsByGroup )
        .def( "bondsByGroup", (GroupedVector<GroupIndexID,Bond> (BT::*)(
                                                        const QSet<GroupID>&) const)
                    &BT::bondsByGroup )
    
        .def( "bondsByResidue", (GroupedVector<ResNumIndexID,Bond> (BT::*)() const)
                    &BT::bondsByResidue )
        .def( "bondsByResidue", (GroupedVector<ResNumIndexID,Bond> (BT::*)(
                                                        const QSet<ResNum>&) const)
                    &BT::bondsByResidue )
    
        .def( "bondParameters", (QVector<Param> (BT::*)() const)
                    &BT::bondParameters )
        .def( "bondParameters", (QVector<Param> (BT::*)(ResNum) const)
                    &BT::bondParameters )
        .def( "bondParameters", (QVector<Param> (BT::*)(GroupID) const)
                    &BT::bondParameters )
    
        .def( "bondParametersByGroup", (GroupedVector<GroupIndexID,Param> (BT::*)() const)
                    &BT::bondParametersByGroup )
        .def( "bondParametersByGroup", (GroupedVector<GroupIndexID,Param> (BT::*)(
                                                            const QSet<GroupID>&) const)
                    &BT::bondParametersByGroup )
        
        .def( "bondParametersByResidue", (GroupedVector<ResNumIndexID,Param> (BT::*)() const)
                    &BT::bondParametersByResidue )
        .def( "bondParametersByResidue", (GroupedVector<ResNumIndexID,Param> (BT::*)(
                                                            const QSet<ResNum>&) const)
                    &BT::bondParametersByResidue )
    
        .def( "clearBond", (void (BT::*)(const Bond&))
                    &BT::clearBond )
        
        .def( "clearBonds", (void (BT::*)())
                    &BT::clearBonds )
        .def( "clearBonds", (void (BT::*)(ResNum))
                    &BT::clearBonds )
        .def( "clearBonds", (void (BT::*)(GroupID))
                    &BT::clearBonds )
        .def( "clearBonds", (void (BT::*)(const QSet<Bond>&))
                    &BT::clearBonds )
        .def( "clearBonds", (void (BT::*)(const QList<Bond>&))
                    &BT::clearBonds )
        .def( "clearBonds", (void (BT::*)(const QSet<ResNum>&))
                    &BT::clearBonds )
        .def( "clearBonds", (void (BT::*)(const QList<ResNum>&))
                    &BT::clearBonds )
        .def( "clearBonds", (void (BT::*)(const QSet<GroupID>&))
                    &BT::clearBonds )
        .def( "clearBonds", (void (BT::*)(const QList<GroupID>&))
                    &BT::clearBonds )
    
        .def( "clearIntraBonds", (void (BT::*)())
                    &BT::clearIntraBonds )
        .def( "clearIntraBonds", (void (BT::*)(ResNum))
                    &BT::clearIntraBonds )
        .def( "clearIntraBonds", (void (BT::*)(const QSet<ResNum>&))
                    &BT::clearIntraBonds )
        .def( "clearIntraBonds", (void (BT::*)(const QList<ResNum>&))
                    &BT::clearIntraBonds )
    
        .def( "clearInterBonds", (void (BT::*)())
                    &BT::clearInterBonds )
        .def( "clearInterBonds", (void (BT::*)(ResNum))
                    &BT::clearInterBonds )
        .def( "clearInterBonds", (void (BT::*)(const QSet<ResNum>&))
                    &BT::clearInterBonds )
        .def( "clearInterBonds", (void (BT::*)(const QList<ResNum>&))
                    &BT::clearInterBonds )
    
        .def( "getBondParameter", (const Param& (BT::*)(const Bond&) const)
                    &BT::getBondParameter, return_value_policy<copy_const_reference>() )
        .def( "getBondParameter", (const Param& (BT::*)(const GroupIndexID&) const)
                    &BT::getBondParameter, return_value_policy<copy_const_reference>() )
    
        .def( "hasMissingBondParameters", (bool (BT::*)() const)
                    &BT::hasMissingBondParameters )
        .def( "hasMissingBondParameters", (bool (BT::*)(ResNum) const)
                    &BT::hasMissingBondParameters )
        .def( "hasMissingBondParameters", (bool (BT::*)(GroupID) const)
                    &BT::hasMissingBondParameters )
    
        .def( "hasMissingIntraBondParameters", (bool (BT::*)() const)
                    &BT::hasMissingIntraBondParameters )
        .def( "hasMissingIntraBondParameters", (bool (BT::*)(ResNum) const)
                    &BT::hasMissingIntraBondParameters )
    
        .def( "hasMissingInterBondParameters", (bool (BT::*)() const)
                    &BT::hasMissingInterBondParameters )
        .def( "hasMissingInterBondParameters", (bool (BT::*)(ResNum) const)
                    &BT::hasMissingInterBondParameters )
    
        .def( "intraBonds", (QVector<Bond> (BT::*)() const)
                    &BT::intraBonds )
        .def( "intraBonds", (QVector<Bond> (BT::*)(ResNum) const)
                    &BT::intraBonds )
    
        .def( "interBonds", (QVector<Bond> (BT::*)() const)
                    &BT::interBonds )
        .def( "interBonds", (QVector<Bond> (BT::*)(ResNum) const)
                    &BT::interBonds )
    
        .def( "intraBondsByResidue", (GroupedVector<ResNumIndexID,Bond> (BT::*)() const)
                    &BT::intraBondsByResidue )
        .def( "intraBondsByResidue", (GroupedVector<ResNumIndexID,Bond> (BT::*)(
                                                        const QSet<ResNum>&) const)
                    &BT::intraBondsByResidue )
    
        .def( "interBondsByResidue", (GroupedVector<ResNumIndexID,Bond> (BT::*)() const)
                    &BT::interBondsByResidue )
        .def( "interBondsByResidue", (GroupedVector<ResNumIndexID,Bond> (BT::*)(
                                                        const QSet<ResNum>&) const)
                    &BT::interBondsByResidue )
    
        .def( "intraBondParameters", (QVector<Param> (BT::*)() const)
                    &BT::intraBondParameters )
        .def( "intraBondParameters", (QVector<Param> (BT::*)(ResNum) const)
                    &BT::intraBondParameters )
    
        .def( "interBondParameters", (QVector<Param> (BT::*)() const)
                    &BT::interBondParameters )
        .def( "interBondParameters", (QVector<Param> (BT::*)(ResNum) const)
                    &BT::interBondParameters )
                                                    
        .def( "intraBondParametersByResidue", (GroupedVector<ResNumIndexID,Param> (BT::*)() const)
                    &BT::intraBondParametersByResidue )
        .def( "intraBondParametersByResidue", (GroupedVector<ResNumIndexID,Param> (BT::*)(
                                                            const QSet<ResNum>&) const)
                    &BT::intraBondParametersByResidue )
                                                    
        .def( "interBondParametersByResidue", (GroupedVector<ResNumIndexID,Param> (BT::*)() const)
                    &BT::interBondParametersByResidue )
        .def( "interBondParametersByResidue", (GroupedVector<ResNumIndexID,Param> (BT::*)(
                                                            const QSet<ResNum>&) const)
                    &BT::interBondParametersByResidue )
    
        .def( "missingBondParameters", (QSet<Bond> (BT::*)() const)
                    &BT::missingBondParameters )
        .def( "missingBondParameters", (QSet<Bond> (BT::*)(ResNum) const)
                    &BT::missingBondParameters )
        .def( "missingBondParameters", (QSet<Bond> (BT::*)(const QSet<GroupID>&) const)
                    &BT::missingBondParameters )
    
        .def( "missingIntraBondParameters", (QSet<Bond> (BT::*)() const)
                    &BT::missingIntraBondParameters )
        .def( "missingIntraBondParameters", (QSet<Bond> (BT::*)(ResNum) const)
                    &BT::missingIntraBondParameters )
    
        .def( "missingInterBondParameters", (QSet<Bond> (BT::*)() const)
                    &BT::missingInterBondParameters )
        .def( "missingInterBondParameters", (QSet<Bond> (BT::*)(ResNum) const)
                    &BT::missingInterBondParameters )
                    
        .def( "nBonds", (int (BT::*)() const)
                    &BT::nBonds )
        .def( "nBonds", (int (BT::*)(ResNum) const)
                    &BT::nBonds )
        .def( "nBonds", (int (BT::*)(GroupID) const)
                    &BT::nBonds )
    
        .def( "nIntraBonds", (int (BT::*)() const)
                    &BT::nIntraBonds )
        .def( "nIntraBonds", (int (BT::*)(ResNum) const)
                    &BT::nIntraBonds )
    
        .def( "nInterBonds", (int (BT::*)() const)
                    &BT::nInterBonds )
        .def( "nInterBonds", (int (BT::*)(ResNum) const)
                    &BT::nInterBonds )
                    
        .def( "nBondParameters", (int (BT::*)() const)
                    &BT::nBondParameters )
        .def( "nBondParameters", (int (BT::*)(ResNum) const)
                    &BT::nBondParameters )
        .def( "nBondParameters", (int (BT::*)(GroupID) const)
                    &BT::nBondParameters )
    
        .def( "nIntraBondParameters", (int (BT::*)() const)
                    &BT::nIntraBondParameters )
        .def( "nIntraBondParameters", (int (BT::*)(ResNum) const)
                    &BT::nIntraBondParameters )
    
        .def( "nInterBondParameters", (int (BT::*)() const)
                    &BT::nInterBondParameters )
        .def( "nInterBondParameters", (int (BT::*)(ResNum) const)
                    &BT::nInterBondParameters )
    
        .def( "removeBond", (void (BT::*)(const Bond&))
                    &BT::removeBond )
    
        .def( "removeBonds", (void (BT::*)())
                    &BT::removeBonds )
        .def( "removeBonds", (void (BT::*)(ResNum))
                    &BT::removeBonds )
        .def( "removeBonds", (void (BT::*)(GroupID))
                    &BT::removeBonds )
        .def( "removeBonds", (void (BT::*)(const QSet<GroupID>&))
                    &BT::removeBonds )
        .def( "removeBonds", (void (BT::*)(const QSet<Bond>&))
                    &BT::removeBonds )
        .def( "removeBonds", (void (BT::*)(const QList<Bond>&))
                    &BT::removeBonds )
        
        .def( "removeIntraBonds", (void (BT::*)())
                    &BT::removeIntraBonds )
        .def( "removeIntraBonds", (void (BT::*)(ResNum))
                    &BT::removeIntraBonds )
    
        .def( "removeInterBonds", (void (BT::*)())
                    &BT::removeInterBonds )
        .def( "removeInterBonds", (void (BT::*)(ResNum))
                    &BT::removeInterBonds )
    
        .def( "removeMissingBonds", (void (BT::*)())
                    &BT::removeMissingBonds )
        .def( "removeMissingBonds", (void (BT::*)(ResNum))
                    &BT::removeMissingBonds )
        .def( "removeMissingBonds", (void (BT::*)(GroupID))
                    &BT::removeMissingBonds )
        .def( "removeMissingBonds", (void (BT::*)(const QSet<GroupID>&))
                    &BT::removeMissingBonds )
    
        .def( "removeMissingIntraBonds", (void (BT::*)())
                    &BT::removeMissingIntraBonds )
        .def( "removeMissingIntraBonds", (void (BT::*)(ResNum))
                    &BT::removeMissingIntraBonds )
    
        .def( "removeMissingInterBonds", (void (BT::*)())
                    &BT::removeMissingInterBonds )
        .def( "removeMissingInterBonds", (void (BT::*)(ResNum))
                    &BT::removeMissingInterBonds )
    
        .def( "setBondParameter", (void (BT::*)(const Bond&, const Param&))
                    &BT::setBondParameter )
        .def( "setBondParameter", (void (BT::*)(const GroupIndexID&, const Param&))
                    &BT::setBondParameter )
    ;

}

}

SIRE_END_HEADER

#endif
