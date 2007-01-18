
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#include "wrap_SireSystem.h"

namespace SireSystem
{

void export_FFExpression();
void export_MoleculeGroup();
void export_Move();
void export_Moves();
void export_System();
void export_SystemData();

void SIRESYSTEM_EXPORT export_SireSystem()
{
    export_FFExpression();
    export_MoleculeGroup();
    export_Move();
    export_Moves();
    export_SystemData();
    export_System();
}

}
