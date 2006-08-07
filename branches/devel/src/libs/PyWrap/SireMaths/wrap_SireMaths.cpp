
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireMaths.h"
#endif

namespace SireMaths
{
void export_Angle();
void export_AxisSet();
void export_Complex();
void export_Rational();
void export_Geometry();
void export_Maths();
void export_Vector();
    
void SIREMATHS_EXPORT export_SireMaths()
{
    export_Angle();
    export_AxisSet();
    export_Complex();
    export_Rational();
    export_Geometry();
    export_Maths();
    export_Vector();
}

}
