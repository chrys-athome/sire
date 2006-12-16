
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireVol.h"
#endif

namespace SireVol
{
void export_Space();
void export_CoordGroup();

void SIREVOL_EXPORT export_SireVol()
{
    export_Space();
    export_CoordGroup();
}

}
