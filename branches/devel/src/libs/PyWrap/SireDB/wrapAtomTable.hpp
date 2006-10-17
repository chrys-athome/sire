#ifndef PYWRAP_SIREDB_WRAPATOMTABLE_HPP
#define PYWRAP_SIREDB_WRAPATOMTABLE_HPP

#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/atomtable.hpp"

#include "SirePy/pycontainer.hpp"

using namespace boost::python;
using namespace SirePy;

namespace SireDB
{

template<class Table, class Idx, class Param>
AtomParameter<Param> __getitem__AtomTable(const Table &table, const Idx &idx)
{
    return table[idx];
}

template<class T>
void export_AtomTable(const char *wrapname)
{
    typedef AtomTableT<T> Table;
    typedef T Param;

    class_< Table, bases<AtomTable>, boost::noncopyable >( wrapname, no_init )

        .def( "at", (AtomParameter<Param> (Table::*)(const CGAtomID&) const)
                      &Table::at )
        .def( "at", (AtomParameter<Param> (Table::*)(const CGNumAtomID&) const)
                      &Table::at )
        .def( "at", (AtomParameter<Param> (Table::*)(const ResNumAtomID&) const)
                      &Table::at )
        .def( "at", (AtomParameter<Param> (Table::*)(const ResIDAtomID&) const)
                      &Table::at )
        .def( "at", (AtomParameter<Param> (Table::*)(const AtomIndex&) const)
                      &Table::at )
        .def( "at", (AtomParameter<Param> (Table::*)(AtomID) const)
                      &Table::at )

        .def( "__len__", (int (Table::*)() const)
                      &Table::nParameters )

        .def( "__getitem__", &__getitem__AtomTable<Table,CGAtomID,Param> )
        .def( "__getitem__", &__getitem__AtomTable<Table,CGNumAtomID,Param> )
        .def( "__getitem__", &__getitem__AtomTable<Table,ResNumAtomID,Param> )
        .def( "__getitem__", &__getitem__AtomTable<Table,ResIDAtomID,Param> )
        .def( "__getitem__", &__getitem__AtomTable<Table,AtomIndex,Param> )
        .def( "__getitem__", &__getitem__AtomTable<Table,AtomID,Param> )

        .def( "value", (AtomParameter<Param> (Table::*)(const CGAtomID&) const)
                    &Table::value )
        .def( "value", (AtomParameter<Param> (Table::*)(const CGNumAtomID&) const)
                    &Table::value )
        .def( "value", (AtomParameter<Param> (Table::*)(const ResNumAtomID&) const)
                    &Table::value )
        .def( "value", (AtomParameter<Param> (Table::*)(const ResIDAtomID&) const)
                    &Table::value )
        .def( "value", (AtomParameter<Param> (Table::*)(const AtomIndex&) const)
                    &Table::value )
        .def( "value", (AtomParameter<Param> (Table::*)(AtomID) const)
                    &Table::value )

        .def( "value", (AtomParameter<Param> (Table::*)(const CGAtomID&,
                                                        const Param&) const)
                    &Table::value )
        .def( "value", (AtomParameter<Param> (Table::*)(const CGNumAtomID&,
                                                        const Param&) const)
                    &Table::value )
        .def( "value", (AtomParameter<Param> (Table::*)(const ResNumAtomID&,
                                                        const Param&) const)
                    &Table::value )
        .def( "value", (AtomParameter<Param> (Table::*)(const ResIDAtomID&,
                                                        const Param&) const)
                    &Table::value )
        .def( "value", (AtomParameter<Param> (Table::*)(const AtomIndex&,
                                                        const Param&) const)
                    &Table::value )
        .def( "value", (AtomParameter<Param> (Table::*)(AtomID,
                                                        const Param&) const)
                    &Table::value )

        .def( "parameter", (Param (Table::*)(const CGAtomID&) const)
                    &Table::parameter )
        .def( "parameter", (Param (Table::*)(const CGNumAtomID&) const)
                    &Table::parameter )
        .def( "parameter", (Param (Table::*)(const ResNumAtomID&) const)
                    &Table::parameter )
        .def( "parameter", (Param (Table::*)(const ResIDAtomID&) const)
                    &Table::parameter )
        .def( "parameter", (Param (Table::*)(const AtomIndex&) const)
                    &Table::parameter )
        .def( "parameter", (Param (Table::*)(AtomID) const)
                    &Table::parameter )

        .def( "setParameter", (void (Table::*)(const CGAtomID&,
                                               const Param&))
                    &Table::setParameter )
        .def( "setParameter", (void (Table::*)(const CGNumAtomID&,
                                               const Param&))
                    &Table::setParameter )
        .def( "setParameter", (void (Table::*)(const ResNumAtomID&,
                                               const Param&))
                    &Table::setParameter )
        .def( "setParameter", (void (Table::*)(const ResIDAtomID&,
                                               const Param&))
                    &Table::setParameter )
        .def( "setParameter", (void (Table::*)(const AtomIndex&,
                                               const Param&))
                    &Table::setParameter )
        .def( "setParameter", (void (Table::*)(AtomID,
                                               const Param&))
                    &Table::setParameter )

        .def( "parameterGroup", (ParameterGroup<Param> (Table::*)(CutGroupID) const)
                    &Table::parameterGroup )

        .def( "parameterGroups", (QVector< ParameterGroup<Param> > (Table::*)() const)
                    &Table::parameterGroups )

        .def( "parameterGroups", (QHash< CutGroupID,ParameterGroup<Param> >
                                    (Table::*)(const QSet<CutGroupID>&) const)
                    &Table::parameterGroups )
        .def( "parameterGroups", (QHash< CutGroupNum,ParameterGroup<Param> >
                                    (Table::*)(const QSet<CutGroupNum>&) const)
                    &Table::parameterGroups )

        .def( "parameters", (QVector<Param> (Table::*)() const)
                    &Table::parameters )
        .def( "parameters", (QVector<Param> (Table::*)(ResNum) const)
                    &Table::parameters )
        .def( "parameters", (QVector<Param> (Table::*)(ResID) const)
                    &Table::parameters )
        .def( "parameters", (QVector<Param> (Table::*)(CutGroupID) const)
                    &Table::parameters )
        .def( "parameters", (QVector<Param> (Table::*)(CutGroupNum) const)
                    &Table::parameters )

        .def( "parameters", (QHash< CutGroupID,QVector<Param> >
                                  (Table::*)(const QSet<CutGroupID>&) const)
                    &Table::parameters )
        .def( "parameters", (QHash< CutGroupNum,QVector<Param> >
                                  (Table::*)(const QSet<CutGroupNum>&) const)
                    &Table::parameters )
        .def( "parameters", (QHash< ResNum,QVector<Param> >
                                  (Table::*)(const QSet<ResNum>&) const)
                    &Table::parameters )
        .def( "parameters", (QHash< ResID,QVector<Param> >
                                  (Table::*)(const QSet<ResID>&) const)
                    &Table::parameters )

        .def( "atomParameters", (QVector< AtomParameter<Param> > (Table::*)() const)
                    &Table::atomParameters )
        .def( "atomParameters", (QVector< AtomParameter<Param> > (Table::*)(ResNum) const)
                    &Table::atomParameters )
        .def( "atomParameters", (QVector< AtomParameter<Param> > (Table::*)(ResID) const)
                    &Table::atomParameters )
        .def( "atomParameters", (QVector< AtomParameter<Param> > (Table::*)(CutGroupID) const)
                    &Table::atomParameters )
        .def( "atomParameters", (QVector< AtomParameter<Param> > (Table::*)(CutGroupNum) const)
                    &Table::atomParameters )

        .def( "atomParameters", (QHash< CutGroupID,QVector< AtomParameter<Param> > >
                                  (Table::*)(const QSet<CutGroupID>&) const)
                    &Table::atomParameters )
        .def( "atomParameters", (QHash< CutGroupNum,QVector< AtomParameter<Param> > >
                                  (Table::*)(const QSet<CutGroupNum>&) const)
                    &Table::atomParameters )
        .def( "atomParameters", (QHash< ResNum,QVector< AtomParameter<Param> > >
                                  (Table::*)(const QSet<ResNum>&) const)
                    &Table::atomParameters )
        .def( "atomParameters", (QHash< ResID,QVector< AtomParameter<Param> > >
                                  (Table::*)(const QSet<ResID>&) const)
                    &Table::atomParameters )
    ;
}

}

#endif
