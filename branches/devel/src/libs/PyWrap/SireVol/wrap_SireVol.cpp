
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireVol.h"
#endif

namespace SireVol
{
void export_SimVolume();

void SIREVOL_EXPORT export_SireVol()
{
    export_SimVolume();
}

}
