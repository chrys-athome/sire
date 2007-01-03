
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireFF.h"
#endif

namespace SireFF
{
void export_EnergyBase();
void export_Energy();
void export_EnergyPack();
void export_EnergyType();
void export_FFBase();
void export_ForceField();
void export_FFComponent();
void export_FFProcessor();
void export_FFWorker();
void export_ParameterMap();

void SIREFF_EXPORT export_SireFF()
{
    export_EnergyBase();
    export_Energy();
    export_EnergyPack();
    export_EnergyType();

    export_FFBase();
    export_FFComponent();
    export_ForceField();
    export_FFProcessor();
    export_FFWorker();
    export_ParameterMap();
}

}
