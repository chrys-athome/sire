
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireBase.h"
#endif

namespace SireBase
{
void export_MD5Sum();
void export_IDVersion();
void export_SharedPolyPointer();
 
void SIREBASE_EXPORT export_SireBase()
{
    export_MD5Sum();
    export_IDVersion();
    export_SharedPolyPointer();
}

}
