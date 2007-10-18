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

#ifndef PYWRAP_SIREQT_QLIST_HPP
#define PYWRAP_SIREQT_QLIST_HPP

#include <Python.h>
#include <boost/python.hpp>

#include <QList>
#include <QVector>
#include <QSet>

#include "sireglobal.h"

#include "SirePy/pycontainer.hpp"
#include "SirePy/convertlist.hpp"
#include "SirePy/str.hpp"

#include "qdatastream.hpp"

SIRE_BEGIN_HEADER

using namespace boost::python;
using namespace SirePy;

namespace SireQt
{

template<class T>
int wrap_qlist_indexOf2(const QList<T> &list, const T &value, int from)
{
    return list.indexOf(value,from);
}

template<class T>
int wrap_qlist_indexOf1(const QList<T> &list, const T &value)
{
    return list.indexOf(value);
}

template<class T>
int wrap_qlist_lastIndexOf2(const QList<T> &list, const T &value, int from)
{
    return list.lastIndexOf(value,from);
}

template<class T>
int wrap_qlist_lastIndexOf1(const QList<T> &list, const T &value)
{
    return list.lastIndexOf(value);
}

template<class T>
QList<T> wrap_qlist_mid2(const QList<T> &list, int pos, int length)
{
    return list.mid(pos,length);
}

template<class T>
QList<T> wrap_qlist_mid1(const QList<T> &list, int pos)
{
    return list.mid(pos);
}

/** This provides the to-Python wrapping of QList */
template<class T>
void wrap_QList(const char *wrapname)
{
    //typedef the Container type
    typedef QList<T> C;

    //typedef the iterator types
    typedef typename C::const_iterator const_iterator;
    typedef typename C::iterator iterator;

    class_<C>(wrapname, init<>())
        .def( init<const C&>() )
        .def( "append", &C::append )
        .def( "at", &C::at, return_value_policy<copy_const_reference>() )
        .def( "clear", &C::clear )
        .def( "contains", &C::contains )
        .def( "count", (int (C::*)(const T&) const)
                          &C::count )
        .def( "count", (int (C::*)() const)
                          &C::count )
        .def( "empty", &C::empty )
        .def( "indexOf", &wrap_qlist_indexOf2<T> )
        .def( "indexOf", &wrap_qlist_indexOf1<T> )
        .def( "insert", (void (C::*)(int, const T&))
                          &C::insert )
        .def( "isEmpty", &C::isEmpty )
        .def( "lastIndexOf", &wrap_qlist_lastIndexOf2<T> )
        .def( "lastIndexOf", &wrap_qlist_lastIndexOf1<T> )
        
        .def( "mid", &wrap_qlist_mid2<T> )
        .def( "mid", &wrap_qlist_mid1<T> )
        .def( "move", &C::move )
        .def( "pop_back", &C::pop_back )
        .def( "pop_front", &C::pop_front )
        .def( "prepend", &C::prepend )
        .def( "push_back", &C::push_back )
        .def( "push_front", &C::push_front )
        .def( "removeAll", &C::removeAll )
        .def( "removeAt", &C::removeAt )
        .def( "removeFirst", &C::removeFirst )
        .def( "removeLast", &C::removeLast )
        .def( "replace", &C::replace )
        .def( "size", &C::size )
        .def( "swap", &C::swap )
        .def( "takeAt", &C::takeAt )
        .def( "takeFirst", &C::takeFirst )
        .def( "takeLast", &C::takeLast )
        .def( "toSet", &C::toSet )
        .def( "toStdList", &C::toStdList )
        .def( "toVector", &C::toVector )
        .def( "value", (T (C::*)(int) const)
                          &C::value )
        .def( "value", (T (C::*)(int, const T&) const)
                          &C::value )
                          
        .def( "fromSet", &C::fromSet ).staticmethod("fromSet")
        .def( "fromStdList", &C::fromStdList ).staticmethod("fromStdList")
        .def( "fromVector", &C::fromVector ).staticmethod("fromVector")
        
        .def( self == self )
        .def( self != self )
        .def( self + self )
        .def( self += other<T>() )
        .def( self += other<T>() )
        
        .def( "__lshift__", &__lshift__<C,T> )
        .def( "__lshift__", &__lshift__<C,C> )
        
        .def( "__rlshift__", &__rlshift__QDataStream<C>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<C>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "__len__", &C::size )
        .def( "__iter__", boost::python::iterator<C>() )
        .def( "__iter__", boost::python::iterator<const C>() )
        .def( "__getitem__", &__getitem__list<C,T>,
                              typename get_return_reference_policy<T>::type() )
        .def( "__getitem__", &__getitem__slice<C> )
        .def( "__setitem__", &__setitem__list<C,T> )
        .def( "__delitem__", &__delitem__list<C> )
        
        .def( "__str__", &__str__list<C> )
        .def( "__contains__", &__contains__<C,T> )
    ;
    
    //expose the from-python converters
    convert_python_to_list< QList<T> >();
}

}

SIRE_END_HEADER

#endif
