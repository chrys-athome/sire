
/**
  * This file contains the boost::python wrapping of RunTests
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireTest/runtests.h"

using namespace boost::python;

namespace SireTest
{

void  
SIRETEST_EXPORT
export_RunTests()
{
    class_<RunTests>("RunTests", init<>())
                .def("run", &RunTests::run)
    ;
}

}
