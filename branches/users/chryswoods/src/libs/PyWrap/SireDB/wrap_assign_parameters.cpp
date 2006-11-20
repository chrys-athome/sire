
#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/assign_parameters.h"
#include "SireDB/parameterdb.h"
#include "SireDB/parametertable.h"

#include "SireMol/molecule.h"

#include "SireQt/qdatastream.hpp"
#include "SirePy/str.hpp"

using namespace boost::python;
using namespace SirePy;
using namespace SireQt;

namespace SireDB
{

/** Struct used to convert a python list or tuple of AssignInstructions
    into an assign_parameter */
struct assign_parameters_from_python
{
    assign_parameters_from_python()
    {
        converter::registry::push_back(  &convertible,
                                         &construct,
                                         type_id<assign_parameters>() );
    }

    /** Can the python object pointed to by 'obj_ptr' be converted
        to an assign_parameters?
    */
    static void* convertible(PyObject* obj_ptr)
    {
        if ( PyTuple_Check(obj_ptr) )
        {
            //are all of the elements of the list convertible to
            //AssignInstruction?

            //check the tuple elements... - convert to a boost::tuple object
            boost::python::tuple t( handle<>(borrowed(obj_ptr)) );

            //how many elements are there?
            int n = PyTuple_Size(obj_ptr);

            //can they all be converted to AssertInstruction?
            for (int i=0; i<n; ++i)
            {
                if (not extract<const AssignInstruction&>(t[i]).check())
                    return 0;
            }

            return obj_ptr;
        }
        else if ( PyList_Check(obj_ptr) )
        {
            //check that all of the list elements can be converted to the right type
            boost::python::list l( handle<>(borrowed(obj_ptr)) );

            //how many elements are there?
            int n = PyList_Size(obj_ptr);

            //can all of the elements be converted to AssertInstruction?
            for (int i=0; i<n; ++i)
            {
                if (not extract<const AssignInstruction&>(l[i]).check())
                    return 0;
            }

            //the list is ok!
            return obj_ptr;
        }
        else
            return 0;
    }

    /** Perform the actual conversion */
    static void construct(  PyObject* obj_ptr,
                            converter::rvalue_from_python_stage1_data* data)
    {
        //locate the storage space for the result
        void* storage = ( (converter::rvalue_from_python_storage<assign_parameters>*
                          ) data )->storage.bytes;

        //create the new assign_parameters
        new (storage) assign_parameters();

        assign_parameters *assign_params = static_cast<assign_parameters*>(storage);

        if ( PyTuple_Check(obj_ptr) )
        {
            //convert the PyObject to a boost::python::object
            boost::python::tuple t( handle<>(borrowed(obj_ptr)) );

            //add all of the elements from the tuple - get the number of elements in the tuple
            int n = PyTuple_Size(obj_ptr);

            for (int i=0; i<n; ++i)
            {
                assign_params->addInstruction(
                                extract<const AssignInstruction&>(t[i])() );
            }
        }
        else if ( PyList_Check(obj_ptr) )
        {
            //convert the PyObject to a boost::python::object
            boost::python::list l( handle<>(borrowed(obj_ptr)) );

            //add all of the elements from the list
            //  - get the number of elements in the tuple
            int n = PyList_Size(obj_ptr);

            for (int i=0; i<n; ++i)
            {
                assign_params->addInstruction(
                                extract<const AssignInstruction&>(l[i])() );
            }
        }

        data->convertible = storage;
    }
};

ParameterTable wrap_assign_a3(const assign_parameters &assigner, const Molecule &molecule,
                              ParameterDB &database, const MatchMRData &matchmr)
{
    return assigner.assign(molecule, database, matchmr);
}

ParameterTable wrap_assign_a2(const assign_parameters &assigner, const Molecule &molecule,
                              ParameterDB &database)
{
    return assigner.assign(molecule, database);
}

ParameterTable wrap_assign_b3(const assign_parameters &assigner,
                              const Molecule &molecule,
                              const ParameterTable &orig_table,
                              ParameterDB &database, const MatchMRData &matchmr)
{
    return assigner.assign(molecule, orig_table, database, matchmr);
}

ParameterTable wrap_assign_b2(const assign_parameters &assigner,
                              const Molecule &molecule,
                              const ParameterTable &orig_table,
                              ParameterDB &database)
{
    return assigner.assign(molecule, orig_table, database);
}

void  
SIREDB_EXPORT
export_assign_parameters()
{
    //register the from-python converter for assign_parameters
    //this allow assign_parameters to be constructed from a python list
    //or tuple of AssignInstruction objects
    assign_parameters_from_python();

    class_<assign_parameters>( "assign_parameters", init<const assign_parameters&>() )

        .def( init< optional<const AssignInstruction&, const AssignInstruction&,
                             const AssignInstruction&, const AssignInstruction&,
                             const AssignInstruction&, const AssignInstruction&,
                             const AssignInstruction&, const AssignInstruction&,
                             const AssignInstruction&, const AssignInstruction&> >() )

        .def( "__rlshift__", &__rlshift__QDataStream<assign_parameters>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<assign_parameters>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "addInstruction", &assign_parameters::addInstruction )

        .def( "assign", &wrap_assign_a3 )
        .def( "assign", &wrap_assign_a2 )

        .def( "assign", &wrap_assign_b3 )
        .def( "assign", &wrap_assign_b2 )

    ;

    implicitly_convertible<const AssignInstruction&, assign_parameters>();

}

}
