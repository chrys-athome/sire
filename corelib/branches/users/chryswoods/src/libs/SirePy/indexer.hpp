/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#ifndef SIREPY_INDEXER_HPP
#define SIREPY_INDEXER_HPP

#include <Python.h>
#include <QObject>
#include <exception>

#include <QSet>
#include <QList>
#include <QVector>

#include "sireglobal.h"

using namespace boost::python;

SIRE_BEGIN_HEADER

/**
This class provides functions that provide read-only Python indexing functions for Sire containers. The container (T), that holds objects of type V, must implement the following functions;

typedef V value_type;
const V& at(int i) const;
int count() const;

This class is heavily influenced by the boost.python/StlContainers wiki (Thanks!)

@author Christopher Woods
*/

template<class T, class V>
struct readonly_indexer
{
    static V __getitem__(T const& x, int i)
    {
        int sz = x.count();
        if( i < 0 ) i+=sz;
        if( i >= 0 and i < sz ) return x.at(i);
        
        throw std::out_of_range(QObject::tr("Index out of range (%1 from %2)")
                        .arg(i).arg(sz).toAscii().constData());
    }
    
    static int __len__(T const &x)
    {
        return x.count();
    }
};

template<class T, class V>
struct readwrite_indexer
{
    static V __getitem__(T &x, int i)
    {
        int sz = x.count();
        if ( i < 0) i+=sz;
        if (i >= 0 and i < sz ) return x.at(i);
        
        throw std::out_of_range(QObject::tr("Index out of range (%1 from %2)")
                        .arg(i).arg(sz).toAscii().constData());
    }
    
    static int __len__(T &x)
    {
        return x.count();
    }
};

inline void indexer_error()
{
    PyErr_SetString(PyExc_IndexError, QObject::tr("Index out of range").toAscii()); 
}

/** Use this function to provide consistent and easy wrapping of any QList type object.
    
    e.g.
    
    \code
    wrapList<EditMol>("EditMolList");
    wrapList<Molecule>("MoleculeList");
    \endcode
    
    \param T The type of class held in the list.
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void wrapList(const char *wrapname)
{
    typedef QList<T> V;
    
    int (V::*wrap_count1)(const T&) const = &V::count;
    int (V::*wrap_count2)() const = &V::count;
    void (V::*wrap_insert)(int, const T&) = &V::insert;

    class_<V>(wrapname, init<>())
           .def("__getitem__", &readonly_indexer<V,T>::__getitem__)
           .def("__len__", &readonly_indexer<V,T>::__len__)
           .def("append", &V::append)
           .def("size", &V::size)
           .def("count", &V::size)
           .def("clear", &V::clear)
           .def("at", &V::at, return_value_policy<copy_const_reference>())
           .def("contains", &V::contains)
           .def("count", wrap_count1)
           .def("count", wrap_count2)
           .def("takeAt", &V::takeAt)
           .def("takeFirst", &V::takeFirst)
           .def("takeLast", &V::takeLast)
           .def("fromSet", &V::fromSet).staticmethod("fromSet")
           .def("empty", &V::empty)
           .def("pop_back", &V::pop_back)
           .def("pop_front", &V::pop_front)
           .def("prepend", &V::prepend)
           .def("insert", wrap_insert)
           .def("isEmpty", &V::isEmpty)
           .def("move", &V::move)
           .def("push_back", &V::push_back)
           .def("push_front", &V::push_front)
           .def("removeAll", &V::removeAll)
           .def("removeAt", &V::removeAt)
           .def("removeFirst", &V::removeFirst)
           .def("removeLast", &V::removeLast)
           .def("replace", &V::replace)
           .def("swap", &V::swap)
           .def("takeAt", &V::takeAt)
           .def("takeFirst", &V::takeFirst)
           .def("takeLast", &V::takeLast)
           .def("toSet", &V::toSet)
    ;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
void wrap_QSet_insert(QSet<T> &set, const T &value)
{
    set.insert(value);
}

/** Use this function to provide consistent and easy wrapping of any QSet type object.

    e.g.
    
    \code
    wrapSet<Bond>("BondSet");
    wrapSet<Int>("IntSet");
    \endcode
    
    \param T The type of object held in the set.
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void wrapSet(const char *wrapname)
{
    typedef QSet<T> V;
    
    class_<V>(wrapname, init<>())
           .def("insert", &wrap_QSet_insert<T>)
           .def("contains", &V::contains)
           .def("clear", &V::clear)
           .def("toList", &V::toList)
           .def("capacity", &V::capacity)
           .def("contains", &V::contains)
           .def("count", &V::count)
           .def("empty", &V::empty)
           .def("isEmpty", &V::isEmpty)
           .def("remove", &V::remove)
           .def("reserve", &V::reserve)
           .def("size", &V::size)
           .def("squeeze", &V::squeeze)
           .def("subtract", &V::subtract, return_value_policy<reference_existing_object>())
           .def("intersect", &V::intersect, return_value_policy<reference_existing_object>())
           .def("unite", &V::unite, return_value_policy<reference_existing_object>())
           .def("values", &V::values)
           .def("fromList", &V::fromList).staticmethod("fromList")
           .def(self += other<T>())
           .def(self += self)
    ;
}

/** Use this function to provide consistent adn easy wrapping of any QVector type object.

    e.g. 
    
    \code
    wrapVector<Atom>("AtomVector");
    wrapVector<Bond>("BondVector");
    \endcode
    
    \param T The type of class contained within the vector 
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void wrapVector(const char *wrapname)
{
    typedef QVector<T> V;

    int (V::*wrap_count1)(const T&) const = &V::count;
    int (V::*wrap_count2)() const = &V::count;
    
    void (V::*wrap_insert1)(int, const T& ) = &V::insert;
    void (V::*wrap_insert2)(int, int, const T& ) = &V::insert;
   
    void (V::*wrap_remove1)(int) = &V::remove;
    void (V::*wrap_remove2)(int, int) = &V::remove;
    
    T (V::*wrap_value1)(int) const = &V::value;
    T (V::*wrap_value2)(int, const T&) const = &V::value;

    class_<V>(wrapname, init<>())
           .def(init<int>())
           .def(init<int, const T&>())
           .def(init<const V&>())
           .def("__getitem__", &readonly_indexer<V,T>::__getitem__)
           .def("__len__", &readonly_indexer<V,T>::__len__)           
           .def("append", &V::append)
           .def("at", &V::at, return_value_policy<copy_const_reference>())
           .def("capacity", &V::capacity)
           .def("clear", &V::clear)
           .def("contains", &V::contains)
           .def("count", wrap_count1)
           .def("count", wrap_count2)
           .def("empty", &V::empty)
           .def("indexOf", &V::indexOf)
           .def("lastIndexOf", &V::lastIndexOf)
           .def("isEmpty", &V::isEmpty)
           .def("pop_front", &V::pop_front)
           .def("pop_back", &V::pop_back)
           .def("push_front", &V::push_front)
           .def("push_back", &V::push_back)
           .def("prepend", &V::prepend)
           .def("replace", &V::replace)
           .def("reserve", &V::reserve)
           .def("resize", &V::resize)
           .def("size", &V::size)
           .def("squeeze", &V::squeeze)
           .def("toList", &V::toList)
           .def(self != self)
           .def(self == self)
           .def(self += self)
           .def(self + self)
           .def(self += T())
    ;
}

SIRE_END_HEADER

#endif
