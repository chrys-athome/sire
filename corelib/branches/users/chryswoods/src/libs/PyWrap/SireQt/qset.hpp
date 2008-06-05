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

#ifndef PYWRAP_SIREQT_QSET_HPP
#define PYWRAP_SIREQT_QSET_HPP

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
void wrap_qset_insert(QSet<T> &qset, const T &value)
{
    qset.insert(value);
}

/** This struct provides the from-Python conversion from a list or
    tuple to a QSet<T> */
template<class T>
struct convert_python_to_qset
{
    /** Constructor - register the conversion functions
        for this type */
    convert_python_to_qset()
    {
        boost::python::converter::registry::push_back(
            &convertible,
            &construct,
            type_id< QSet<T> >());
    }

    /** Test whether or not it is possible to convert the PyObject
        to a QVector where all of the elements are of type 'T' */
    static void* convertible(PyObject* obj_ptr)
    {
        //is this a tuple type?
        if ( PyTuple_Check(obj_ptr) )
        {
            //check the tuple elements... - convert to a boost::tuple object
            boost::python::tuple t( handle<>(borrowed(obj_ptr)) );
            
            //how many elements are there?
            int n = PyTuple_Size(obj_ptr);
            
            //can they all be converted to type 'T'?
            for (int i=0; i<n; ++i)
            {
                if (not boost::python::extract<T>(t[i]).check())
                    return 0;
            }
            
            //the tuple is ok!
            return obj_ptr;
        }
        //is this a list type?
        else if ( PyList_Check(obj_ptr) )
        {
            //check that all of the list elements can be converted to the right type
            boost::python::list l( handle<>(borrowed(obj_ptr)) );
            
            //how many elements are there?
            int n = PyList_Size(obj_ptr);
            
            //can all of the elements be converted to type 'T'?
            for (int i=0; i<n; ++i)
            {
                if (not boost::python::extract<T>(l[i]).check())
                    return 0;
            }
            
            //the list is ok!
            return obj_ptr;
        }
        else
            //could not recognise the type...
            return 0;
    }

    /** Construct a container of type T from the PyObject pointed to
        by 'obj_ptr' */
    static void construct(
        PyObject* obj_ptr,
        boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        if (PyTuple_Check(obj_ptr))
        {
            //convert the PyObject to a boost::python::object
            boost::python::tuple t( handle<>(borrowed(obj_ptr)) );

            //locate the storage space for the result
            void* storage =
                ( (converter::rvalue_from_python_storage< QSet<T> >*)data )->storage.bytes;

            //create the T container
            new (storage) QSet<T>();

            QSet<T> *container = static_cast<QSet<T>*>(storage);

            //add all of the elements from the tuple - get the number of elements in the tuple
            int n = PyTuple_Size(obj_ptr);

            for (int i=0; i<n; ++i)
            {
                container->insert( extract<T>(t[i])() );
            }

            data->convertible = storage;
        }
        else if (PyList_Check(obj_ptr))
        {
            //convert the PyObject to a boost::python::object
            boost::python::list l( handle<>(borrowed(obj_ptr)) );

            //locate the storage space for the result
            void* storage =
                ( (converter::rvalue_from_python_storage< QSet<T> >*)data )->storage.bytes;

            //create the T container
            new (storage) QSet<T>();

            QSet<T> *container = static_cast<QSet<T>*>(storage);

            //add all of the elements from the tuple - get the number of elements in the tuple
            int n = PyList_Size(obj_ptr);

            for (int i=0; i<n; ++i)
            {
                container->insert( extract<T>(l[i])() );
            }

            data->convertible = storage;
        }
    }
};

/** This provides the to-Python wrapping of QSet */
template<class T>
void wrap_QSet(const char *wrapname)
{
    //typedef the Container type
    typedef QSet<T> C;

    class_<C>(wrapname, init<>())
        .def( init<const C&>() )
        
        .def( "capacity", &C::capacity )
        .def( "clear", &C::clear )
        .def( "contains", &C::contains )
        .def( "count", &C::count )
        .def( "empty", &C::empty )
        .def( "insert", &wrap_qset_insert<T> )
        .def( "intersect", &C::intersect,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "isEmpty", &C::isEmpty )
        .def( "remove", &C::remove )
        .def( "reserve", &C::reserve )
        .def( "size", &C::size )
        .def( "squeeze", &C::squeeze )
        .def( "subtract", &C::subtract,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "toList", &C::toList )
        .def( "unite", &C::unite,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "values", &C::values )
        
        .def( "fromList", &C::fromList ).staticmethod("fromList")
        
        .def( self == self )
        .def( self != self )
        .def( self & self )
        .def( self &= self )
        .def( self &= other<T>() )
        .def( self + self )
        .def( self += self )
        .def( self += other<T>() )
        .def( self - self )
        .def( self -= self )
        .def( self -= other<T>() )
        .def( self | self )
        .def( self |= self )
        .def( self |= other<T>() )
        
        .def( "__len__", &C::size )
        .def( "__iter__", boost::python::iterator<const C>() )
        
        .def( "__lshift__", &__lshift__<C,T> )
        
        .def( "__rlshift__", &__rlshift__QDataStream<C>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<C>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    
        .def( "__str__", &__str__list<C> )
        .def( "__contains__", &__contains__<C,T> )
    ;
    
    //expose the from-python converters
    convert_python_to_qset<T>();
}
    
}

SIRE_END_HEADER

#endif
