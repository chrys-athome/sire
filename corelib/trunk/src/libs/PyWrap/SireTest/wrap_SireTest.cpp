
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireTest.h"
#endif

namespace SireTest
{
void export_RunTests();
void export_TestCast();

void SIRETEST_EXPORT export_SireTest()
{
    export_RunTests();
    export_TestCast();
}

}
