
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireError.h"
#endif

namespace SireError
{

void export_exceptions();

void SIREERROR_EXPORT export_SireError()
{
    export_exceptions();
}

}
