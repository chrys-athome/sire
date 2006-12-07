
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireCluster.h"
#endif

namespace SireCluster
{
void export_Processor();
void export_ThreadWorker();

void SIREFF_EXPORT export_SireCluster()
{
    export_Processor();
    export_ThreadWorker();
}

}
