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

#ifndef SIREBASE_WRAP_SET_H
#define SIREBASE_WRAP_SET_H

#include "SireBase/set.hpp"
#include "SireBase/setiterator.hpp"
#include "SireBase/mutablesetiterator.hpp"

#include "SirePy/indexer.hpp"

using namespace boost::python;

namespace SireBase
{

/** Function used to wrap up a Set<K,V> */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
void wrapSet(const char *wrapname)
{
    typedef Set<K,V> S;
        
    void (S::*wrap_insert)(const V&) = &S::insert;
        
    class_<S>(wrapname, init<>())
            .def(init<const S&>())
            .def(self == self)
            .def(self != self)
            .def("at", &S::at, return_value_policy<copy_const_reference>())
            .def("at_key", &S::at_key, return_value_policy<copy_const_reference>())
            .def("__getitem__",&readonly_indexer<S,V>::__getitem__)
            .def("__len__", &S::count)
            .def("count", &S::count)
            .def("size", &S::size)
            .def("isEmpty", &S::isEmpty)
            .def("clear", &S::clear)
            .def("toString", &S::toString)
            .def("__str__", &S::toString)
            .def("indexOfKey", &S::indexOfKey)
            .def("keys", &S::keys)
            .def("values", &S::values, return_value_policy<copy_const_reference>())
            .def("contains", &S::contains)
            .def("has_key", &S::has_key)
            .def("insert", wrap_insert)
            .def("remove", &S::remove)
            .def("remove_key", &S::remove_key)
            .def("removeAt", &S::removeAt)
            .def("reserve", &S::reserve)
            .def("squeeze", &S::squeeze)
            .def("sum", &S::sum)
            .def("intersection", &S::intersection)
            .def("leftDifference", &S::leftDifference)
            .def("rightDifference", &S::rightDifference)
            .def("symmetricDifference", &S::symmetricDifference)
            .def("splitIntoVectors", &S::splitIntoVectors)
            .def("splitIntoSets", &S::splitIntoSets)
            .def("intersects", &S::intersects)
            .def("isSubSet", &S::isSubSet)
            .def("isSuperSet", &S::isSuperSet)
            .def("isStrictSubSet", &S::isStrictSubSet)
            .def("isStrictSuperSet", &S::isStrictSuperSet)
    ;          
    
    //wrap up the SetIterator class
    typedef SetIterator<K,V> SI;
    
    class_<SI>( QString("%1Iterator").arg(wrapname).toLatin1().constData(), init<>() )
            .def(init<const S&>())
            .def(init<const SI&>())
            .def("value", &SI::value, return_value_policy<copy_const_reference>())
            .def("key", &SI::key)
            .def("next", &SI::next, return_value_policy<copy_const_reference>())
            .def("peekNext", &SI::peekNext, return_value_policy<copy_const_reference>())
            .def("previous", &SI::previous, return_value_policy<copy_const_reference>())
            .def("peekPrevious", &SI::peekPrevious, return_value_policy<copy_const_reference>())
            .def("jumpTo", &SI::jumpTo, return_value_policy<copy_const_reference>())
            .def("peekAt", &SI::peekAt, return_value_policy<copy_const_reference>())
            .def("find", &SI::find)
            .def("findKey", &SI::findKey)
            .def("index", &SI::index)
            .def("hasNext", &SI::hasNext)
            .def("hasPrevious", &SI::hasPrevious)
            .def("isValid", &SI::isValid)
            .def("toBack", &SI::toBack)
            .def("toFront", &SI::toFront)
    ;

    //wrap up the MutableSetIterator class
    typedef MutableSetIterator<K,V> MSI;
    
    const V& (MSI::*wrap_value1)() const = &MSI::value;
    V& (MSI::*wrap_value2)() = &MSI::value;
    
    class_<MSI>( QString("Mutable%1Iterator").arg(wrapname).toLatin1().constData(), init<>() )
            .def(init<const MSI&>())
            .def(init<S&>())
            .def("value", wrap_value1, return_value_policy<copy_const_reference>())
            .def("value", wrap_value2, return_internal_reference<1,
                                          with_custodian_and_ward<1,2> >())
            .def("key", &MSI::key)
            .def("isValid", &MSI::isValid)
            .def("setValue", &MSI::setValue)
            .def("remove", &MSI::remove)
            .def("next", &MSI::next, return_value_policy<copy_const_reference>())
            .def("peekNext", &MSI::peekNext, return_value_policy<copy_const_reference>())
            .def("previous", &MSI::previous, return_value_policy<copy_const_reference>())
            .def("peekPrevious", &MSI::peekPrevious,
                                    return_value_policy<copy_const_reference>())
            .def("jumpTo", &MSI::jumpTo, return_value_policy<copy_const_reference>())
            .def("peekAt", &MSI::peekAt, return_value_policy<copy_const_reference>())
            .def("index", &MSI::index)
            .def("hasNext", &MSI::hasNext)
            .def("hasPrevious", &MSI::hasPrevious)
            .def("find", &MSI::find)
            .def("findKey", &MSI::findKey)
            .def("toBack", &MSI::toBack)
            .def("toFront", &MSI::toFront)
    ;
}

}

#endif
