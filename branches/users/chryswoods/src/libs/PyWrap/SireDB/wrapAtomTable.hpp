#ifndef PYWRAP_SIREDB_WRAPATOMTABLE_HPP
#define PYWRAP_SIREDB_WRAPATOMTABLE_HPP

#include <Python.h>
#include <boost/python.hpp>

using namespace boost::python;

namespace SireDB
{

// template<class Table, class Param>
// uint wrap_atomtable_params_a_2( const Table &table, QVector<Param> &param_vector, uint strt )
// {
//     return table.parameters(param_vector, strt);
// }
//
// template<class Table, class Param>
// uint wrap_atomtable_params_a_1( const Table &table, QVector<Param> &param_vector )
// {
//     return table.parameters(param_vector);
// }
//
// template<class Table, class IDX, class Param>
// uint wrap_atomtable_params_b_3( const Table &table, IDX idx,
//                                 QVector<Param> &param_vector, uint strt )
// {
//     return table.parameters( idx, param_vector, strt );
// }
//
// template<class Table, class IDX, class Param>
// uint wrap_atomtable_params_b_2( const Table &table, IDX idx,
//                                 QVector<Param> &param_vector )
// {
//     return table.parameters( idx, param_vector );
// }
//
// template<class T>
// void export_AtomTable(const char *wrapname)
// {
//     typedef AtomTableT<T> Table;
//     typedef T Param;
//
//     class_< Table, bases<AtomTable>, boost::noncopyable >( wrapname, no_init )
//
//         .def( "__getitem__", &__getitem__dict<Table,CGAtomID,Param>,
//                       return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
//         .def( "__getitem__", &__getitem__dict<Table,ResNumAtomID,Param>,
//                       return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
//         .def( "__getitem__", &__getitem__dict<Table,AtomIndex,Param>,
//                       return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
//
//         .def( "__setitem__", &__setitem__dict<Table,CGAtomID,Param> )
//         .def( "__setitem__", &__setitem__dict<Table,ResNumAtomID,Param> )
//         .def( "__setitem__", &__setitem__dict<Table,AtomIndex,Param> )
//
//         .def( "__rrshift__", &__rrshift__QDataStream<Table>,
//                       return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
//         .def( "__rlshift__", &__rlshift__QDataStream<Table>,
//                       return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
//
//         .def( "at", (const Param& (Table::*)(const CGAtomID&) const)
//                     &Table::at, return_value_policy<copy_const_reference>() )
//         .def( "at", (const Param& (Table::*)(const ResNumAtomID&) const)
//                     &Table::at, return_value_policy<copy_const_reference>() )
//         .def( "at", (const Param& (Table::*)(const AtomIndex&) const)
//                     &Table::at, return_value_policy<copy_const_reference>() )
//
//         .def( "getParameter", (const Param& (Table::*)(const CGAtomID&) const)
//                     &Table::getParameter, return_value_policy<copy_const_reference>() )
//         .def( "getParameter", (const Param& (Table::*)(const ResNumAtomID&) const)
//                     &Table::getParameter, return_value_policy<copy_const_reference>() )
//         .def( "getParameter", (const Param& (Table::*)(const AtomIndex&) const)
//                     &Table::getParameter, return_value_policy<copy_const_reference>() )
//
//         .def( "missingParameters", (QSet<AtomIndex> (Table::*)() const)
//                     &Table::missingParameters )
//         .def( "missingParameters", (QSet<AtomIndex> (Table::*)(ResNum) const)
//                     &Table::missingParameters )
//         .def( "missingParameters", (QSet<AtomIndex> (Table::*)(CutGroupID) const)
//                     &Table::missingParameters )
//
//         .def( "parameters", (QVector<Param> (Table::*)() const)
//                     &Table::parameters )
//         .def( "parameters", (QVector<Param> (Table::*)(ResNum) const)
//                     &Table::parameters )
//         .def( "parameters", (QVector<Param> (Table::*)(CutGroupID) const)
//                     &Table::parameters )
//         .def( "parameters", &wrap_atomtable_params_a_2<Table,Param> )
//         .def( "parameters", &wrap_atomtable_params_a_1<Table,Param> )
//         .def( "parameters", &wrap_atomtable_params_b_3<Table,CutGroupID,Param> )
//         .def( "parameters", &wrap_atomtable_params_b_2<Table,CutGroupID,Param> )
//         .def( "parameters", &wrap_atomtable_params_b_3<Table,ResNum,Param> )
//         .def( "parameters", &wrap_atomtable_params_b_2<Table,ResNum,Param> )
//
//         //template<class C>
//         //uint parameters( QVector<C> &param_vector, uint strt=0 ) const;
//         //template<class C>
//         //uint parameters( ResNum resnum, QVector<C> &param_vector, uint strt=0 ) const;
//         //template<class C>
//         //uint parameters( CutGroupID cgid, QVector<C> &param_vector, uint strt=0 ) const;
//
//         .def( "parametersByCutGroup", (GroupedVector<CGAtomID,Param> (Table::*)() const)
//                     &Table::parametersByCutGroup )
//         .def( "parametersByCutGroup", (GroupedVector<CGAtomID,Param>
//                     (Table::*)(const QSet<CutGroupID>&) const) &Table::parametersByCutGroup )
//
//         //template<class C>
//         //void parametersByCutGroup( GroupedVector<C,CGAtomID> &param_vector ) const;
//         //template<class C>
//         //void parametersByCutGroup(const QSet<CutGroupID> &cgids,
//         //                          GroupedVector<C,CGAtomID> &param_vector) const;
//
//         .def( "parametersByResidue", (GroupedVector<ResNumAtomID,Param>
//                     (Table::*)(const QSet<ResNum>&) const) &Table::parametersByResidue )
//         .def( "parametersByResidue", (GroupedVector<ResNumAtomID,Param> (Table::*)() const)
//                     &Table::parametersByResidue )
//
//         //template<class C>
//         //void parametersByResidue( GroupedVector<C,ResNumAtomID> &param_vector ) const;
//         //template<class C>
//         //void parametersByResidue(const QSet<ResNum> &resnums,
//         //                         GroupedVector<C,ResNumAtomID> &param_vector) const;
//
//         .def( "setParameter", (void (Table::*)(const CGAtomID&, const Param&))
//                     &Table::setParameter )
//         .def( "setParameter", (void (Table::*)(const ResNumAtomID&, const Param&))
//                     &Table::setParameter )
//         .def( "setParameter", (void (Table::*)(const AtomIndex&, const Param&))
//                     &Table::setParameter )
//
//         .def( "value", (Param (Table::*)(const CGAtomID&) const)
//                     &Table::value )
//         .def( "value", (Param (Table::*)(const ResNumAtomID&) const)
//                     &Table::value )
//         .def( "value", (Param (Table::*)(const AtomIndex&) const)
//                     &Table::value )
//         .def( "value", (Param (Table::*)(const CGAtomID&, const Param&) const)
//                     &Table::value )
//         .def( "value", (Param (Table::*)(const ResNumAtomID&, const Param&) const)
//                     &Table::value )
//         .def( "value", (Param (Table::*)(const AtomIndex&, const Param&) const)
//                     &Table::value )
//     ;
// }

}

#endif
