#ifndef PYWRAP_SIREMM_WRAP_INTERNALTABLE_HPP
#define PYWRAP_SIREMM_WRAP_INTERNALTABLE_HPP

#include <Python.h>
#include <boost/python.hpp>

#include <QSet>
#include <QList>

#include "SireMM/internaltable.hpp"

#include "SirePy/pycontainer.hpp"

using namespace boost::python;
using namespace SirePy;

namespace SireMM
{

template<class Internals>
void wrap_InternalTableBase(const char *wrapname)
{
    typedef InternalTableBase<Internals> ITBase;
    typedef typename ITBase::internal_type internal_type;

    //wrap the base class...
    class_<ITBase, bases<TableBase>, boost::noncopyable>(wrapname, no_init)
        
        .def( "addInternal", &ITBase::addInternal )
    
        .def( "addInternals", (void (ITBase::*)(const QSet<internal_type>&))
                      &ITBase::addInternals )
        .def( "addInternals", (void (ITBase::*)(const QList<internal_type>&))
                      &ITBase::addInternals )
    
        .def( "assignedParameter", (bool (ITBase::*)(const internal_type&) const)
                      &ITBase::assignedParameter )
        .def( "assignedParameter", (bool (ITBase::*)(const GroupIndexID&) const)
                      &ITBase::assignedParameter )
        
        .def( "clear", (void (ITBase::*)())
                      &ITBase::clear )
        .def( "clear", (void (ITBase::*)(ResNum))
                      &ITBase::clear )
        .def( "clear", (void (ITBase::*)(GroupID))
                      &ITBase::clear )
        .def( "clear", (void (ITBase::*)(const internal_type&))
                      &ITBase::clear )
        .def( "clear", (void (ITBase::*)(const QSet<internal_type>&))
                      &ITBase::clear )
        .def( "clear", (void (ITBase::*)(const QList<internal_type>&))
                      &ITBase::clear )
        .def( "clear", (void (ITBase::*)(const QSet<ResNum>&))
                      &ITBase::clear )
        .def( "clear", (void (ITBase::*)(const QSet<GroupID>&))
                      &ITBase::clear )
    
        .def( "clearIntra", (void (ITBase::*)())
                      &ITBase::clearIntra )
        .def( "clearIntra", (void (ITBase::*)(const QSet<ResNum>&))
                      &ITBase::clearIntra )
        
        .def( "clearInter", (void (ITBase::*)())
                      &ITBase::clearInter )
        .def( "clearInter", (void (ITBase::*)(const QSet<ResNum>&))
                      &ITBase::clearInter )
        
        .def( "count", &ITBase::count )
        
        .def( "groupIDs", &ITBase::groupIDs )
    
        .def( "hasMissingParameters", (bool (ITBase::*)() const)
                      &ITBase::hasMissingParameters )
        .def( "hasMissingParameters", (bool (ITBase::*)(ResNum) const)
                      &ITBase::hasMissingParameters )
        .def( "hasMissingParameters", (bool (ITBase::*)(GroupID) const)
                      &ITBase::hasMissingParameters )
    
        .def( "hasMissingIntraParameters", (bool (ITBase::*)() const)
                      &ITBase::hasMissingIntraParameters )
        .def( "hasMissingIntraParameters", (bool (ITBase::*)(ResNum) const)
                      &ITBase::hasMissingIntraParameters )
    
        .def( "hasMissingInterParameters", (bool (ITBase::*)() const)
                      &ITBase::hasMissingInterParameters )
        .def( "hasMissingInterParameters", (bool (ITBase::*)(ResNum) const)
                      &ITBase::hasMissingInterParameters )
    
        .def( "info", &ITBase::info, return_value_policy<copy_const_reference>() )
        
        .def( "interInternals", (QVector<internal_type> (ITBase::*)() const)
                      &ITBase::interInternals )
        .def( "interInternals", (QVector<internal_type> (ITBase::*)(ResNum) const)
                      &ITBase::interInternals )
        
        .def( "internals", (QVector<internal_type> (ITBase::*)() const)
                      &ITBase::internals )
        .def( "internals", (QVector<internal_type> (ITBase::*)(ResNum) const)
                      &ITBase::internals )
        .def( "internals", (QVector<internal_type> (ITBase::*)(GroupID) const)
                      &ITBase::internals )
        
        .def( "intraInternals", (QVector<internal_type> (ITBase::*)() const)
                      &ITBase::intraInternals )
        .def( "intraInternals", (QVector<internal_type> (ITBase::*)(ResNum) const)
                      &ITBase::intraInternals )
    
        .def( "internalsByGroup", 
                (GroupedVector<GroupIndexID,internal_type> (ITBase::*)() const)
                      &ITBase::internalsByGroup )
                      
        .def( "internalsByGroup", 
                (GroupedVector<GroupIndexID,internal_type> (ITBase::*)(
                                                            const QSet<GroupID> &gids) const)
                      &ITBase::internalsByGroup )
                                                            
        .def( "internalsByResidue",
                (GroupedVector<ResNumIndexID,internal_type> (ITBase::*)() const)
                      &ITBase::internalsByResidue )
        .def( "internalsByResidue",
                (GroupedVector<ResNumIndexID,internal_type> (ITBase::*)(
                                                            const QSet<ResNum> &resnums) const)
                      &ITBase::internalsByResidue )
    
        .def( "interInternalsByResidue",
                (GroupedVector<ResNumIndexID,internal_type> (ITBase::*)() const)
                      &ITBase::interInternalsByResidue )
        .def( "interInternalsByResidue",
                (GroupedVector<ResNumIndexID,internal_type> (ITBase::*)(
                                                            const QSet<ResNum> &resnums) const)
                      &ITBase::interInternalsByResidue )
                      
        .def( "intraInternalsByResidue",
                (GroupedVector<ResNumIndexID,internal_type> (ITBase::*)() const)
                      &ITBase::intraInternalsByResidue )
        .def( "intraInternalsByResidue",
                (GroupedVector<ResNumIndexID,internal_type> (ITBase::*)(
                                                            const QSet<ResNum> &resnums) const)
                      &ITBase::intraInternalsByResidue )
    
        .def( "missingParameters", (QSet<internal_type> (ITBase::*)() const)
                      &ITBase::missingParameters )
        .def( "missingParameters", (QSet<internal_type> (ITBase::*)(ResNum) const)
                      &ITBase::missingParameters )        
        .def( "missingParameters", (QSet<internal_type> (ITBase::*)(const QSet<GroupID>&) const)
                      &ITBase::missingParameters )
                      
        .def( "missingIntraParameters", (QSet<internal_type> (ITBase::*)() const)
                      &ITBase::missingIntraParameters )
        .def( "missingIntraParameters", (QSet<internal_type> (ITBase::*)(ResNum) const)
                      &ITBase::missingIntraParameters )        
    
        .def( "missingInterParameters", (QSet<internal_type> (ITBase::*)() const)
                      &ITBase::missingInterParameters )
        .def( "missingInterParameters", (QSet<internal_type> (ITBase::*)(ResNum) const)
                      &ITBase::missingInterParameters )        
    
        .def( "nInternals", (int (ITBase::*)() const)
                      &ITBase::nInternals )
        .def( "nInternals", (int (ITBase::*)(ResNum) const)
                      &ITBase::nInternals )
        .def( "nInternals", (int (ITBase::*)(GroupID) const)
                      &ITBase::nInternals )
        
        .def( "nIntraInternals", (int (ITBase::*)() const)
                      &ITBase::nIntraInternals )
        .def( "nInterInternals", (int (ITBase::*)() const)
                      &ITBase::nInterInternals )
                      
        .def( "nIntraInternals", (int (ITBase::*)(ResNum) const)
                      &ITBase::nIntraInternals )
        .def( "nInterInternals", (int (ITBase::*)(ResNum) const)
                      &ITBase::nInterInternals )
    
        .def( "nParameters", (int (ITBase::*)() const)
                      &ITBase::nParameters )
        .def( "nParameters", (int (ITBase::*)(ResNum) const)
                      &ITBase::nParameters )
        .def( "nParameters", (int (ITBase::*)(GroupID) const)
                      &ITBase::nParameters )
    
        .def( "nIntraParameters", (int (ITBase::*)() const)
                      &ITBase::nIntraParameters )
        .def( "nInterParameters", (int (ITBase::*)() const)
                      &ITBase::nInterParameters )
        .def( "nIntraParameters", (int (ITBase::*)(ResNum) const)
                      &ITBase::nIntraParameters )
        .def( "nInterParameters", (int (ITBase::*)(ResNum) const)
                      &ITBase::nInterParameters )
        
        .def( "removeInternal", &ITBase::removeInternal )

        .def( "removeInternals", (void (ITBase::*)(const QSet<internal_type>&))
                      &ITBase::removeInternals )
        .def( "removeInternals", (void (ITBase::*)(const QList<internal_type>&))
                      &ITBase::removeInternals )
        .def( "removeInternals", (void (ITBase::*)(const QSet<GroupID>&))
                      &ITBase::removeInternals )
    
        .def( "removeInternals", (void (ITBase::*)())
                      &ITBase::removeInternals )
        .def( "removeInternals", (void (ITBase::*)(ResNum))
                      &ITBase::removeInternals )
        .def( "removeInternals", (void (ITBase::*)(GroupID))
                      &ITBase::removeInternals )
    
        .def( "removeIntraInternals", (void (ITBase::*)())
                      &ITBase::removeIntraInternals )
        .def( "removeIntraInternals", (void (ITBase::*)(ResNum))
                      &ITBase::removeIntraInternals )

        .def( "removeInterInternals", (void (ITBase::*)())
                      &ITBase::removeInterInternals )
        .def( "removeInterInternals", (void (ITBase::*)(ResNum))
                      &ITBase::removeInterInternals )
    
        .def( "removeMissingInternals", (void (ITBase::*)())
                      &ITBase::removeMissingInternals )
        .def( "removeMissingInternals", (void (ITBase::*)(ResNum))
                      &ITBase::removeMissingInternals )
        .def( "removeMissingInternals", (void (ITBase::*)(GroupID))
                      &ITBase::removeMissingInternals )
        .def( "removeMissingInternals", (void (ITBase::*)(const QSet<GroupID>&))
                      &ITBase::removeMissingInternals )
    
        .def( "removeMissingIntraInternals", (void (ITBase::*)())
                      &ITBase::removeMissingIntraInternals )
        .def( "removeMissingIntraInternals", (void (ITBase::*)(ResNum))
                      &ITBase::removeMissingIntraInternals )
    
        .def( "removeMissingInterInternals", (void (ITBase::*)())
                      &ITBase::removeMissingInterInternals )
        .def( "removeMissingInterInternals", (void (ITBase::*)(ResNum))
                      &ITBase::removeMissingInterInternals )
    
        .def( "size", &ITBase::size )
    ;
}

template<class IT, class KEY>
void __delitem__InternalTable(IT &table, const KEY &key)
{
    table.removeInternal(key);
}

template<class IT, class KEY>
bool __contains__InternalTable(const IT &table, const KEY &key)
{
    return table.info().contains(key);
}

template<class Internals, class Param>
void wrap_InternalTable(const char *wrapname)
{
    typedef InternalTable<Internals,Param> IT;
    typedef InternalTableBase<Internals> ITBase;
    typedef typename IT::internal_type internal_type;
    
    class_<IT, bases<ITBase>, boost::noncopyable>(wrapname, no_init)
                  
        .def( "__getitem__", &__getitem__dict<IT,internal_type,Param>,
                      typename get_return_reference_policy<Param>::type() )
        .def( "__getitem__", &__getitem__dict<IT,GroupIndexID,Param>,
                      typename get_return_reference_policy<Param>::type() )
        
        .def( "__setitem__", &__setitem__dict<IT,internal_type,Param> )
        .def( "__setitem__", &__setitem__dict<IT,GroupIndexID,Param> )
    
        .def( "__delitem__", &__delitem__InternalTable<IT,internal_type> )
        
        .def( "__contains__", &__contains__InternalTable<IT,internal_type> )
        
        .def( "__len__", &IT::size )

        .def( "addInternal", (void (IT::*)(const internal_type&, const Param&))
                  &IT::addInternal )

        .def( "at", (const Param& (IT::*)(const internal_type&) const)
                  &IT::at, return_value_policy<copy_const_reference>() )
        .def( "at", (const Param& (IT::*)(const GroupIndexID&) const)
                  &IT::at, return_value_policy<copy_const_reference>() )

        .def( "getParameter", (const Param& (IT::*)(const internal_type&) const)
                  &IT::getParameter, return_value_policy<copy_const_reference>() )
        .def( "getParameter", (const Param& (IT::*)(const GroupIndexID&) const)
                  &IT::getParameter, return_value_policy<copy_const_reference>() )
    
        .def( "intraParameters", (QVector<Param> (IT::*)() const)
                  &IT::intraParameters )
        .def( "intraParameters", (QVector<Param> (IT::*)(ResNum) const)
                  &IT::intraParameters )
    
        .def( "interParameters", (QVector<Param> (IT::*)() const)
                  &IT::intraParameters )
        .def( "interParameters", (QVector<Param> (IT::*)(ResNum) const)
                  &IT::intraParameters )
                                                    
        .def( "intraParametersByResidue", (GroupedVector<ResNumIndexID,Param> (IT::*)() const)
                  &IT::intraParametersByResidue )
        .def( "intraParametersByResidue", (GroupedVector<ResNumIndexID,Param> (IT::*)(
                                                          const QSet<ResNum>&) const)
                  &IT::intraParametersByResidue )
                                                    
        .def( "interParametersByResidue", (GroupedVector<ResNumIndexID,Param> (IT::*)() const)
                  &IT::interParametersByResidue )
        .def( "interParametersByResidue", (GroupedVector<ResNumIndexID,Param> (IT::*)(
                                                          const QSet<ResNum>&) const)
                  &IT::interParametersByResidue )
    
        .def( "parameters", (QVector<Param> (IT::*)() const)
                  &IT::parameters )
        .def( "parameters", (QVector<Param> (IT::*)(ResNum) const)
                  &IT::parameters )
        .def( "parameters", (QVector<Param> (IT::*)(GroupID) const)
                  &IT::parameters )
    
        .def( "parametersByGroup", (GroupedVector<GroupIndexID,Param> (IT::*)() const)
                  &IT::parametersByGroup )
        .def( "parametersByGroup", (GroupedVector<GroupIndexID,Param> (IT::*)(
                                                          const QSet<GroupID>&) const)
                  &IT::parametersByGroup )
    
        .def( "parametersByResidue", (GroupedVector<ResNumIndexID,Param> (IT::*)() const)
                  &IT::parametersByResidue )
        .def( "parametersByResidue", (GroupedVector<ResNumIndexID,Param> (IT::*)(
                                                          const QSet<ResNum>&) const)
                  &IT::parametersByResidue )
    
        .def( "setParameter", (void (IT::*)(const internal_type&, const Param&))
                  &IT::setParameter )
        .def( "setParameter", (void (IT::*)(const GroupIndexID&, const Param&))
                  &IT::setParameter )
    
        .def( "value", (Param (IT::*)(const internal_type&) const)
                  &IT::value )
        .def( "value", (Param (IT::*)(const internal_type&, const Param&) const)
                  &IT::value )

        .def( "value", (Param (IT::*)(const GroupIndexID&) const)
                  &IT::value )
        .def( "value", (Param (IT::*)(const GroupIndexID&, const Param&) const)
                  &IT::value )
    ;
}

}

#endif
