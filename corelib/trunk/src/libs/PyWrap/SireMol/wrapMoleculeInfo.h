#ifndef SIREMOL_WRAPMOLECULEINFO_H
#define SIREMOL_WRAPMOLECULEINFO_H

#include "SireMol/moleculeinfo.hpp"

#include "SirePy/indexer.hpp"

namespace SireMol
{

template<typename IDX>
SIRE_OUTOFLINE_TEMPLATE
void wrapMoleculeInfo(const char *molinfonme, const char *resinfonme)
{
    typedef MoleculeInfo<IDX> M;
    typedef ResidueInfo<IDX> R;
    
    const IDX& (M::*wrap_m_index1)(const AtomIndex&) const = &M::index;
    const IDX& (M::*wrap_m_index2)(AtomID) const = &M::index;
    const IDX& (M::*wrap_m_index3)(ResNum, AtomID) const = &M::index;
    const IDX& (M::*wrap_m_index4)(const ResNumAtomID&) const = &M::index;

    AtomIndex (M::*wrap_m_atom1)(AtomID) const = &M::atom;
    AtomIndex (M::*wrap_m_atom2)(ResNum, AtomID) const = &M::atom;
    AtomIndex (M::*wrap_m_atom3)(const ResNumAtomID&) const = &M::atom;

    const R& (M::*wrap_m_at1)(ResID) const = &M::at;
    const IDX& (M::*wrap_m_at2)(const AtomIndex&) const = &M::at;
    const IDX& (M::*wrap_m_at3)(const ResNumAtomID&) const = &M::at;

    QSet<AtomIndex> (M::*wrap_m_getAtoms1)(const QSet<ResNumAtomID>&) const = &M::getAtoms;
    QSet<AtomIndex> (M::*wrap_m_getAtoms2)(const QSet<AtomID>&) const = &M::getAtoms;

    const ResNumList& (M::*wrap_m_residueNumbers1)() const = &M::residueNumbers;
    ResNumList (M::*wrap_m_residueNumbers2)(const QString&) const = &M::residueNumbers;

    int (M::*wrap_nAtoms1)() const = &M::nAtoms;
    int (M::*wrap_nAtoms2)(ResNum) const = &M::nAtoms;

    class_<M>(molinfonme, init<>())
      .def(init<const QList<R>&>())
      .def(init<const QString&, const QList<R>&>())
      .def(init<const M&>())
      .def(self == self)
      .def(self != self)
      .def("__getitem__",&readonly_indexer<M,R>::__getitem__)
      .def("__len__", &M::size)
      .def("__str__", &M::toString)
      .def("name", &M::name, return_value_policy<copy_const_reference>())
      .def("toString", &M::toString)
      .def("index", wrap_m_index1, return_value_policy<copy_const_reference>())
      .def("index", wrap_m_index2, return_value_policy<copy_const_reference>())
      .def("index", wrap_m_index3, return_value_policy<copy_const_reference>())
      .def("index", wrap_m_index4, return_value_policy<copy_const_reference>())
      .def("at", wrap_m_at1, return_value_policy<copy_const_reference>())
      .def("at", wrap_m_at2, return_value_policy<copy_const_reference>())
      .def("at", wrap_m_at3, return_value_policy<copy_const_reference>())
      .def("atom", wrap_m_atom1)
      .def("atom", wrap_m_atom2)
      .def("atom", wrap_m_atom3)
      .def("getAtoms", wrap_m_getAtoms1)
      .def("getAtoms", wrap_m_getAtoms2)
      .def("size", &M::size)
      .def("nResidues", &M::nResidues)
      .def("count", &M::count)
      .def("nAtoms", wrap_nAtoms1)
      .def("nAtoms", wrap_nAtoms2)
      .def("add", &M::add)
      .def("remove", &M::remove)
      .def("residueNumbers", wrap_m_residueNumbers1, return_value_policy<copy_const_reference>())
      .def("residueNumbers", wrap_m_residueNumbers2)
      .def("residueNames", &M::residueNames)
      .def("residue", &M::residue, return_value_policy<copy_const_reference>())
    ;

    
    const IDX& (R::*wrap_r_index1)(AtomID) const = &R::index;
    const IDX& (R::*wrap_r_index2)(const QString&) const = &R::index;

    const IDX& (R::*wrap_r_at)(AtomID) const = &R::at;

    bool (R::*wrap_r_contains1)(const QString&) const = &R::contains;
    bool (R::*wrap_r_contains2)(const AtomIndex&) const = &R::contains;

    class_<R>(resinfonme, init<>())
      .def(init<const QString&, ResNum>())
      .def(init<const QString&, ResNum, const QHash<QString,IDX> >())
      .def(init<const R&>())
      .def(self == self)
      .def(self != self)
      .def("__getitem__",&readonly_indexer<R,IDX>::__getitem__)
      .def("__len__",&R::size)
      .def("__str__",&R::toString)
      .def("add", &R::add)
      .def("indicies", &R::indicies, return_value_policy<copy_const_reference>())
      .def("index", wrap_r_index1, return_value_policy<copy_const_reference>())
      .def("index", wrap_r_index2, return_value_policy<copy_const_reference>())
      .def("at", wrap_r_at, return_value_policy<copy_const_reference>())
      .def("clear", &R::clear)
      .def("squeeze", &R::squeeze)
      .def("reserve", &R::reserve)
      .def("name", &R::name)
      .def("resName", &R::resName)
      .def("number", &R::number)
      .def("resNum", &R::resNum)
      .def("ID", &R::ID, return_value_policy<copy_const_reference>())
      .def("atom", &R::atom)
      .def("atomNames",  &R::atomNames, return_value_policy<copy_const_reference>())
      .def("setName", &R::setName)
      .def("setNumber", &R::setNumber)
      .def("clear", &R::clear)
      .def("squeeze", &R::squeeze)
      .def("reserve", &R::reserve)
      .def("contains", wrap_r_contains1)
      .def("contains", wrap_r_contains2)
    ;
}

}

#endif
