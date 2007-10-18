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

#ifndef SIREMOL_WRAPINTERNALINFO_HPP
#define SIREMOL_WRAPINTERNALINFO_HPP

#include <Python.h>
#include <boost/python.hpp>

#include "SireMM/molinternalinfo.hpp"
#include "SireMM/resinternalinfo.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{

using boost::python::class_;
using boost::python::init;
using boost::python::self;


/** Wrap up the iterator type */
template<class T>
void wrap_InternalIterator(const char *iterator_name)
{
    typedef detail::InternalGroupIterator<T> const_iterator;

    class_< const_iterator >(iterator_name, init<>())
        .def( self == self )
        .def( self != self )
        .def( self += int() )
        .def( self -= int() )
        .def( self + int() )
        .def( self - int() )
        .def( "isValid", &const_iterator::isValid )
        .def( "key", &const_iterator::key )
        .def( "value", &const_iterator::value )
        .def( "toBegin", &const_iterator::toBegin )
        .def( "toEnd", &const_iterator::toEnd )
        .def( "toFirst", &const_iterator::toFirst )
        .def( "toLast", &const_iterator::toLast )
        .def( "next", &const_iterator::next )
        .def( "prev", &const_iterator::prev )
        .def( "nextGroup", &const_iterator::nextGroup )
        .def( "prevGroup", &const_iterator::prevGroup )
        .def( "currentGroup", &const_iterator::currentGroup )
        .def( "residueNumbers", &const_iterator::residueNumbers )
    ;
}

/** Wrap up the underlying InternalInfo<T> part of the class */
template<class T>
void wrap_InternalInfo( class_<T> &wrapper )
{
    typedef typename T::const_iterator const_iterator;
    
    bool (T::*wrap_contains1)(const typename T::internal_type&) const = &T::contains;
    bool (T::*wrap_contains2)(ResNum) const = &T::contains;
    bool (T::*wrap_contains3)(const AtomIndex&) const = &T::contains;
    
    wrapper
        .def( "nGroups", &T::nGroups )
        .def( "contains", wrap_contains1 )
        .def( "contains", wrap_contains2 )
        .def( "contains", wrap_contains3 )
        .def( "at", &T::at )
        .def( "index", &T::index )
        .def( "__getitem__", &T::operator[] )
        .def( "__len__", &T::count )
        .def( "count", &T::count )
        .def( "size", &T::size )
        .def( "begin", &T::begin )
        .def( "end", &T::end )
        .def( "find", &T::find )
    ;
}

/** Wrap up the underlying MolInternalInfo<T> part of the class */
template<class T>
void wrap_MolInternalInfo( class_<T> &wrapper )
{
    //wrap up the underlying InternalInfo<T> parts...
    wrap_InternalInfo<T>(wrapper);

    //export the MolInternalInfo<T> parts...
    wrapper.def("residue", &T::residue);
}

/** Wrap up the underlying ResInternalInfo<T> part of the class */
template<class T>
void wrap_ResInternalInfo( class_<T> &wrapper )
{
    //wrap up the underlying InternalInfo<T> parts...
    wrap_InternalInfo<T>(wrapper);
}


}

SIRE_END_HEADER

#endif
