
/**
  * This file contains the boost::python wrapping of the ProtoMS file reader class
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/parameterdb.h"

#include "SireIO/protoms.h"

using namespace boost::python;

namespace SireIO
{

void  
SIREIO_EXPORT
export_ProtoMS()
{
    class_<ProtoMS>("ProtoMS", init<>())
        .def("read", &ProtoMS::read)
    ;
}

}
