
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireIO.h"
#endif

namespace SireIO
{
void export_IOBase();
void export_IOParam();
void export_ProtoMS();

void SIREIO_EXPORT export_SireIO()
{
    export_IOBase();
    export_IOParam();
    export_ProtoMS();
}

}
