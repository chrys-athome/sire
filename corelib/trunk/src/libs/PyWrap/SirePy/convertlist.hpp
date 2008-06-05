#ifndef PYWRAP_SIREPY_CONVERTLIST_HPP
#define PYWRAP_SIREPY_CONVERTLIST_HPP

#include "sireglobal.h"

#include <boost/python.hpp>
#include <boost/tuple/tuple.hpp>

using namespace boost::python;

SIRE_BEGIN_HEADER

namespace SirePy
{

/** This struct provides the from-Python conversion from a list or
    tuple to a list-like container of type 'C' (e.g. QVector, QList) */
template<class C>
struct convert_python_to_list
{
    typedef typename C::value_type T;

    /** Constructor - register the conversion functions
        for this type */
    convert_python_to_list()
    {
        boost::python::converter::registry::push_back(
            &convertible,
            &construct,
            type_id< C >());
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
                ( (converter::rvalue_from_python_storage<C>*)data )->storage.bytes;

            //create the T container
            new (storage) C();

            C *container = static_cast<C*>(storage);

            //add all of the elements from the tuple - get the number of elements in the tuple
            int n = PyTuple_Size(obj_ptr);

            for (int i=0; i<n; ++i)
            {
                container->append( extract<T>(t[i])() );
            }

            data->convertible = storage;
        }
        else if (PyList_Check(obj_ptr))
        {
            //convert the PyObject to a boost::python::object
            boost::python::list l( handle<>(borrowed(obj_ptr)) );

            //locate the storage space for the result
            void* storage =
                ( (converter::rvalue_from_python_storage<C>*)data )->storage.bytes;

            //create the T container
            new (storage) C();

            C *container = static_cast<C*>(storage);

            //add all of the elements from the tuple - get the number of elements in the tuple
            int n = PyList_Size(obj_ptr);

            for (int i=0; i<n; ++i)
            {
                container->append( extract<T>(l[i])() );
            }

            data->convertible = storage;
        }
    }
};

}

SIRE_END_HEADER

#endif
