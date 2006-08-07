#ifndef PYWRAP_SIREQT_QHASH_HPP
#define PYWRAP_SIREQT_QHASH_HPP

#include <Python.h>
#include <boost/python.hpp>
#include <boost/tuple/tuple.hpp>

#include <QHash>

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
void wrap_qhash_insert(QHash<Key,T> &hash, const Key & key, const T & value)
{
    hash.insert(key,value);
}

template<class Key, class T>
void wrap_qhash_insertMulti(QHash<Key,T> &hash, const Key & key, const T & value)
{
    hash.insertMulti(key,value);
}

template<class Key, class T>
QHash<Key,T>& operator<<(QHash<Key,T> &hash, const QHash<Key,T> &other)
{
    return hash.unite(other);
}

template<class Key, class T>
QHash<Key,T>& operator<<(QHash<Key,T> &hash, const boost::tuple<Key,T> &t)
{
    hash.insert( boost::get<0>(t), boost::get<1>(t) );
    return hash;
}

/** This provides the to-Python wrapping of QHash<key_type,value_type> */
template<class key_type, class value_type>
void wrap_QHash(const char *wrapname)
{
    //typedef the Container type
    typedef QHash<key_type,value_type> C;

    typedef key_type Key;
    typedef value_type T;

    class_<C>(wrapname, init<>())
        .def( init<const C&>() )
        
        .def( "capacity", &C::capacity )
        .def( "clear", &C::clear )
        .def( "contains", &C::contains )
        .def( "count", (int (C::*)(const Key&) const)
                          &C::count )
        .def( "count", (int (C::*)() const)
                          &C::count )
        .def( "empty", &C::empty )
        .def( "insert", &wrap_qhash_insert<Key,T> )
        .def( "insertMulti", &wrap_qhash_insertMulti<Key,T> )
        
        .def( "isEmpty", &C::isEmpty )
        .def( "key", &C::key )
        .def( "keys", (QList<Key> (C::*)() const)
                          &C::keys )
        .def( "keys", (QList<Key> (C::*)(const T&) const)
                          &C::keys )
        
        .def( "remove", &C::remove )
        .def( "reserve", &C::reserve )
        .def( "size", &C::size )
        .def( "squeeze", &C::squeeze )
        .def( "take", &C::take )
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
