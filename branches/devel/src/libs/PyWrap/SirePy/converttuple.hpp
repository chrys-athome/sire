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

#ifndef PYWRAP_SIREPY_CONVERTTUPLE_HPP
#define PYWRAP_SIREPY_CONVERTTUPLE_HPP

#include "sireglobal.h"

#include <boost/python.hpp>
#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SirePy
{

/** Allow an implicit conversion from a python tuple of two objects 
    (of types T0 and T1) to a boost::tuple<T0,T1> */
template<class T0, class T1>
struct convert_python_tuple_to_boost_tuple2
{
    /** Constructor - register the conversion functions
        for this type */
    convert_python_tuple_to_boost_tuple2()
    {
        boost::python::converter::registry::push_back(
            &convertible,
            &construct,
            boost::python::type_id< boost::tuple<T0,T1> >());
    }
    
    /** Test whether or not it is possible to convert the PyObject
        pointed to by 'obj_ptr' to a boost::tuple<T0,T1> */
    static void* convertible(PyObject* obj_ptr)
    {
        //does 'obj_ptr' point to a python tuple containing two elements?
        if (!PyTuple_Check(obj_ptr)  or PyTuple_Size(obj_ptr) != 2)
            return 0;
        else
        {
            //we need to check that the arguments match up.
            //Do this by getting a boost::python::tuple object
            //from this tuple
            boost::python::tuple t( boost::python::handle<>(boost::python::borrowed(obj_ptr)) );
            
            //check we can extract the atoms from the tuple
            if ( boost::python::extract<T0>(t[0]).check() and
                 boost::python::extract<T1>(t[1]).check() )
            {
                //the tuple is ok!
                return obj_ptr;
            }
            else
                //the tuple contains the wrong things
                return 0;
        }
    }

    /** Construct a boost::tuple<T0,T1> from the PyObject pointed to  
        by 'obj_ptr' */
    static void construct(
        PyObject* obj_ptr,
        boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        //convert the PyObject to a boost::python::object
        boost::python::tuple t( boost::python::handle<>(boost::python::borrowed(obj_ptr)) );
        
        //locate the storage space for the result
        void* storage = 
            ( (boost::python::converter::rvalue_from_python_storage<
                                        boost::tuple<T0,T1> >*)data )->storage.bytes;
                                        
        //create the new boost::tuple by extracting the right elements
        new (storage) boost::tuple<T0,T1>( boost::python::extract<T0>(t[0])(),
                                           boost::python::extract<T1>(t[1])() );
        
        data->convertible = storage;
    }
};

/** Allow an implicit conversion from a python tuple of three objects 
    (of types T0, T1 and T2) to a boost::tuple<T0,T1,T2> */
template<class T0, class T1, class T2>
struct convert_python_tuple_to_boost_tuple3
{
    /** Constructor - register the conversion functions
        for this type */
    convert_python_tuple_to_boost_tuple3()
    {
        boost::python::converter::registry::push_back(
            &convertible,
            &construct,
            boost::python::type_id< boost::tuple<T0,T1,T2> >());
    }
    
    /** Test whether or not it is possible to convert the PyObject
        pointed to by 'obj_ptr' to a boost::tuple<T0,T1,T2> */
    static void* convertible(PyObject* obj_ptr)
    {
        //does 'obj_ptr' point to a python tuple containing two elements?
        if (!PyTuple_Check(obj_ptr)  or PyTuple_Size(obj_ptr) != 3)
            return 0;
        else
        {
            //we need to check that the arguments match up.
            //Do this by getting a boost::python::tuple object
            //from this tuple
            boost::python::tuple t( boost::python::handle<>(boost::python::borrowed(obj_ptr)) );
            
            //check we can extract the atoms from the tuple
            if ( boost::python::extract<T0>(t[0]).check() and
                 boost::python::extract<T1>(t[1]).check() and
                 boost::python::extract<T2>(t[2]).check() )
            {
                //the tuple is ok!
                return obj_ptr;
            }
            else
                //the tuple contains the wrong things
                return 0;
        }
    }

    /** Construct a boost::tuple<T0,T1,T2> from the PyObject pointed to  
        by 'obj_ptr' */
    static void construct(
        PyObject* obj_ptr,
        boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        //convert the PyObject to a boost::python::object
        boost::python::tuple t( boost::python::handle<>(boost::python::borrowed(obj_ptr)) );
        
        //locate the storage space for the result
        void* storage = 
            ( (boost::python::converter::rvalue_from_python_storage<
                                        boost::tuple<T0,T1,T2> >*)data )->storage.bytes;
                                        
        //create the new boost::tuple by extracting the right elements
        new (storage) boost::tuple<T0,T1,T2>( boost::python::extract<T0>(t[0])(),
                                              boost::python::extract<T1>(t[1])(),
                                              boost::python::extract<T2>(t[2])() );
        
        data->convertible = storage;
    }
};

/** Allow an implicit conversion from a python tuple of four objects 
    (of types T0, T1, T2 and T3) to a boost::tuple<T0,T1,T2,T3> */
template<class T0, class T1, class T2, class T3>
struct convert_python_tuple_to_boost_tuple4
{
    /** Constructor - register the conversion functions
        for this type */
    convert_python_tuple_to_boost_tuple4()
    {
        boost::python::converter::registry::push_back(
            &convertible,
            &construct,
            boost::python::type_id< boost::tuple<T0,T1,T2,T3> >());
    }
    
    /** Test whether or not it is possible to convert the PyObject
        pointed to by 'obj_ptr' to a boost::tuple<T0,T1,T2,T3> */
    static void* convertible(PyObject* obj_ptr)
    {
        //does 'obj_ptr' point to a python tuple containing two elements?
        if (!PyTuple_Check(obj_ptr)  or PyTuple_Size(obj_ptr) != 4)
            return 0;
        else
        {
            //we need to check that the arguments match up.
            //Do this by getting a boost::python::tuple object
            //from this tuple
            boost::python::tuple t( boost::python::handle<>(boost::python::borrowed(obj_ptr)) );
            
            //check we can extract the atoms from the tuple
            if ( boost::python::extract<T0>(t[0]).check() and
                 boost::python::extract<T1>(t[1]).check() and
                 boost::python::extract<T2>(t[2]).check() and
                 boost::python::extract<T3>(t[3]).check() )
            {
                //the tuple is ok!
                return obj_ptr;
            }
            else
                //the tuple contains the wrong things
                return 0;
        }
    }

    /** Construct a boost::tuple<T0,T1,T2,T3> from the PyObject pointed to  
        by 'obj_ptr' */
    static void construct(
        PyObject* obj_ptr,
        boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        //convert the PyObject to a boost::python::object
        boost::python::tuple t( boost::python::handle<>(boost::python::borrowed(obj_ptr)) );
        
        //locate the storage space for the result
        void* storage = 
            ( (boost::python::converter::rvalue_from_python_storage<
                                        boost::tuple<T0,T1,T2,T3> >*)data )->storage.bytes;
                                        
        //create the new boost::tuple by extracting the right elements
        new (storage) boost::tuple<T0,T1,T2,T3>( boost::python::extract<T0>(t[0])(),
                                                 boost::python::extract<T1>(t[1])(),
                                                 boost::python::extract<T2>(t[2])(),
                                                 boost::python::extract<T3>(t[3])() );
        
        data->convertible = storage;
    }
};

/** Allow an implicit conversion from a python tuple of n objects 
    (all of types T::value_type) to a container of type T */
template<class T>
struct convert_python_tuple_to_container
{
    /** Constructor - register the conversion functions
        for this type */
    convert_python_tuple_to_container()
    {
        boost::python::converter::registry::push_back(
            &convertible,
            &construct,
            boost::python::type_id< T >());
    }
    
    /** Test whether or not it is possible to convert the PyObject
        pointed to by 'obj_ptr' to a QList<T> */
    static void* convertible(PyObject* obj_ptr)
    {
        //does 'obj_ptr' point to a python tuple containing two elements?
        if ( not PyTuple_Check(obj_ptr) )
            return 0;
        else
        {
            //we need to check that the arguments match up.
            //Do this by getting a boost::python::tuple object
            //from this tuple
            boost::python::tuple t( boost::python::handle<>(boost::python::borrowed(obj_ptr)) );
            
            //how many elements are there?
            int n = PyTuple_Size(obj_ptr);
            
            for (int i=0; i<n; ++i)
            {
                //check we can extract the atoms from the tuple
                if ( not boost::python::extract<typename T::value_type>(t[i]).check() )
                    return 0;
            }
                
            //the tuple is ok!
            return obj_ptr;
        }
    }

    /** Construct a container of type T from the PyObject pointed to  
        by 'obj_ptr' */
    static void construct(
        PyObject* obj_ptr,
        boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        //convert the PyObject to a boost::python::object
        boost::python::tuple t( boost::python::handle<>(boost::python::borrowed(obj_ptr)) );
        
        //locate the storage space for the result
        void* storage = 
            ( (boost::python::converter::rvalue_from_python_storage<T>*)data )->storage.bytes;
                                        
        //create the T container
        new (storage) T();
        
        T *container = static_cast<T*>(storage);
        
        //add all of the elements from the tuple - get the number of elements in the tuple
        int n = PyTuple_Size(obj_ptr);
        
        for (int i=0; i<n; ++i)
        {
            container->append( boost::python::extract<typename T::value_type>(t[i])() );
        }
        
        data->convertible = storage;
    }
};

}

SIRE_END_HEADER

#endif
