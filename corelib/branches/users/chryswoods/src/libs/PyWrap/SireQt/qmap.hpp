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

#ifndef PYWRAP_SIREQT_QMAP_HPP
#define PYWRAP_SIREQT_QMAP_HPP

#include <Python.h>
#include <boost/python.hpp>

#include <QMap>

#include "sireglobal.h"

#include "SirePy/pycontainer.hpp"
#include "SirePy/convertdict.hpp"
#include "SirePy/converttuple.hpp"
#include "SirePy/str.hpp"

#include "qdatastream.hpp"

SIRE_BEGIN_HEADER

using namespace boost::python;
using namespace SirePy;

namespace SireQt
{

template<class Key, class T>
void wrap_qmap_insert(QMap<Key,T> &map, const Key & key, const T & value)
{
    map.insert(key,value);
}

template<class Key, class T>
void wrap_qmap_insertMulti(QMap<Key,T> &map, const Key & key, const T & value)
{
    map.insertMulti(key,value);
}

template<class Key, class T>
QMap<Key,T>& operator<<(QMap<Key,T> &map, const QMap<Key,T> &other)
{
    for (typename QMap<Key,T>::const_iterator it = other.begin();
         it != other.end();
         ++it)
    {
        map.insert( it.key(), it.value() );
    }
    
    return map;
}

template<class Key, class T>
QMap<Key,T>& operator<<(QMap<Key,T> &map, const boost::tuple<Key,T> &t)
{
    map.insert( boost::get<0>(t), boost::get<1>(t) );
    return map;
}

/** This provides the to-Python wrapping of QMap<key_type,value_type> */
template<class key_type, class value_type>
void wrap_QMap(const char *wrapname)
{
    //typedef the Container type
    typedef QMap<key_type,value_type> C;

    typedef key_type Key;
    typedef value_type T;

    class_<C>(wrapname, init<>())
        .def( init<const C&>() )
        .def( init<const std::map<Key,T>&>() )
        
        .def( "clear", &C::clear )
        .def( "contains", &C::contains )
        .def( "count", (int (C::*)(const Key&) const)
                          &C::count )
        .def( "count", (int (C::*)() const)
                          &C::count )
        .def( "empty", &C::empty )
        .def( "insert", &wrap_qmap_insert<Key,T> )
        .def( "insertMulti", &wrap_qmap_insertMulti<Key,T> )
        
        .def( "isEmpty", &C::isEmpty )
        .def( "key", &C::key )
        .def( "keys", (QList<Key> (C::*)() const)
                          &C::keys )
        .def( "keys", (QList<Key> (C::*)(const T&) const)
                          &C::keys )
        
        .def( "remove", &C::remove )
        .def( "size", &C::size )
        .def( "take", &C::take )
        
        .def( "toStdMap", &C::toStdMap )
        
        .def( "unite", &C::unite,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
                  
        .def( "value", (const T (C::*)(const Key&) const)
                        &C::value )
        .def( "value", (const T (C::*)(const Key&, const T&) const)
                        &C::value )
        .def( "values", (QList<T> (C::*)() const)
                        &C::values )
        .def( "values", (QList<T> (C::*)(const Key&) const)
                        &C::values )
        
        .def( self == self )
        .def( self != self )
        
        .def( "__len__", &C::size )
        .def( "__iter__", boost::python::iterator<C>() )
        .def( "__iter__", boost::python::iterator<const C>() )
        
        .def( "__getitem__", &__getitem__dict<C,key_type,value_type>,
                                typename get_return_reference_policy<value_type>::type() )
        .def( "__setitem__", &__setitem__dict<C,key_type,value_type> )
        .def( "__delitem__", &__delitem__dict<C,key_type> )
        .def( "__contains__", &__contains__<C,key_type> )
        
        .def( "__lshift__", &__lshift__< C, boost::tuple<Key,T> > )
        .def( "__lshift__", &__lshift__< C, C > )
        
        .def( "__rlshift__", &__rlshift__QDataStream<C>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<C>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "__str__", &__str__dict<C> )
    ;

    //expose the from-python converters
    convert_python_to_dict<C,key_type,value_type>();
    
    //allow conversion from tuple of type 'key_type,value_type' to 
    //a boost::tuple
    convert_python_tuple_to_boost_tuple2<key_type,value_type>();
}

}

SIRE_END_HEADER

#endif
