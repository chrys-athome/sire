
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#include "wrap_SireMaths.h"

namespace SireMaths
{
void export_Angle();
void export_AxisSet();
void export_Complex();
void export_Rational();
void export_Geometry();
void export_Maths();
void export_Vector();
void export_RanGenerator();
    
void SIREMATHS_EXPORT export_SireMaths()
{
    export_Angle();
    export_AxisSet();
    export_Complex();
    export_Rational();
    export_Geometry();
    export_Maths();
    export_Vector();
    export_RanGenerator();
}

}
