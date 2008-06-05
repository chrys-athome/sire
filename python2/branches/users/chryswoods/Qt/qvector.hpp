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

#ifndef PYWRAP_SIREQT_QVECTOR_HPP
#define PYWRAP_SIREQT_QVECTOR_HPP

#include <Python.h>
#include <boost/python.hpp>

#include <QVector>

#include "sireglobal.h"

#include "Helpers/pycontainer.hpp"
#include "Helpers/convertlist.hpp"
#include "Helpers/str.hpp"

#include "qdatastream.hpp"

SIRE_BEGIN_HEADER

using namespace boost::python;

template<class T>
void wrap_qvector_fill1(QVector<T> &vec, const T &value, int size)
{
    vec.fill(value,size);
}

template<class T>
void wrap_qvector_fill2(QVector<T> &vec, const T &value)
{
    vec.fill(value);
}

template<class T>
int wrap_qvector_indexOf1(const QVector<T> &vec, const T &value, int from)
{
    return vec.indexOf(value,from);
}

template<class T>
int wrap_qvector_indexOf2(const QVector<T> &vec, const T &value)
{
    return vec.indexOf(value);
}

template<class T>
int wrap_qvector_lastIndexOf1(const QVector<T> &vec, const T &value, int from)
{
    return vec.lastIndexOf(value,from);
}

template<class T>
int wrap_qvector_lastIndexOf2(const QVector<T> &vec, const T &value)
{
    return vec.lastIndexOf(value);
}

template<class T>
QVector<T> wrap_qvector_mid1(const QVector<T> &vec, int pos, int length)
{
    return vec.mid(pos,length);
}

template<class T>
QVector<T> wrap_qvector_mid2(const QVector<T> &vec, int pos)
{
    return vec.mid(pos);
}

/** This provides the to-Python wrapping of QVector */
template<class T>
void wrap_QVector(const char *wrapname)
{
    //typedef the Container type
    typedef QVector<T> C;

    //typedef the iterator types
    typedef typename C::const_iterator const_iterator;
    typedef typename C::iterator iterator;
    
    class_<C>(wrapname, init<>())
        .def( init<int>() )
        .def( init<int, const T&>() )
        .def( init<const C&>() )
        .def( "append", &C::append )
        .def( "at", &C::at, return_value_policy<copy_const_reference>() )
        .def( "capacity", &C::capacity )
        .def( "clear", &C::clear )
        .def( "contains", &C::contains )
        .def( "count", (int (C::*)(const T&) const)&C::count )
        .def( "count", (int (C::*)() const)&C::count )
        .def( "empty", &C::empty )
        .def( "fill", &wrap_qvector_fill1<T> )
        .def( "fill", &wrap_qvector_fill2<T> )
        .def( "indexOf", &wrap_qvector_indexOf1<T> )
        .def( "indexOf", &wrap_qvector_indexOf2<T> )
        .def( "insert", (void (C::*)(int, const T&))&C::insert )
        .def( "insert", (void (C::*)(int, int, const T&))&C::insert )
        .def( "isEmpty", &C::isEmpty )
        .def( "lastIndexOf", &wrap_qvector_lastIndexOf1<T> )
        .def( "lastIndexOf", &wrap_qvector_lastIndexOf2<T> )
        .def( "mid", &wrap_qvector_mid1<T> )
        .def( "mid", &wrap_qvector_mid2<T> )
        .def( "pop_back", &C::pop_back )
        .def( "pop_front", &C::pop_front )
        .def( "prepend", &C::prepend )
        .def( "push_back", &C::push_back )
        .def( "push_front", &C::push_front )
        .def( "remove", (void (C::*)(int))&C::remove )
        .def( "remove", (void (C::*)(int, int))&C::remove )
        .def( "replace", &C::replace )
        .def( "reserve", &C::reserve )
        .def( "resize", &C::resize )
        .def( "size", &C::size )
        .def( "squeeze", &C::squeeze )
        .def( "toList", &C::toList )
        .def( "toStdVector", &C::toStdVector )
        .def( "value", (T (C::*)(int) const)&C::value )
        .def( "value", (T (C::*)(int, const T&) const)&C::value )
        .def( "fromList", &C::fromList ).staticmethod("fromList")
        .def( "fromStdVector", &C::fromStdVector ).staticmethod("fromStdVector")
        
        .def( self == self )
        .def( self != self )
        .def( self + self )
        .def( self += self )
        .def( self += T() )
        
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
        .def( "__delitem__", &__delitem__list_remove<C> )
        
        .def( "__str__", &__str__list<C> )
        
        .def( "__contains__", &__contains__<C,T> )
    ;

    //expose the from-python converters
    convert_python_to_list< QVector<T> >();
}

SIRE_END_HEADER

#endif
