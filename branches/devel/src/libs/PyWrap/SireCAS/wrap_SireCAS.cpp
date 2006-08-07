
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireCAS.h"
#endif

namespace SireCAS
{
void export_ExBase();
void export_Expression();
void export_ExpressionBase();
void export_Function();
void export_HyperbolicFuncs();
void export_Identities();
void export_PowerFuncs();
void export_Symbol();
void export_TrigFuncs();
void export_Values();

void SIRECAS_EXPORT export_SireCAS()
{
    export_ExBase();
    export_Expression();
    export_ExpressionBase();
    export_Symbol();
    export_Function();
    export_TrigFuncs();
    export_HyperbolicFuncs();
    export_Identities();
    export_Values();
    export_PowerFuncs();
}

}
