
#include <Python.h>
#include <boost/python.hpp>
#include <QObject>

#include "SireError/exception.h"

using namespace boost::python;

namespace SireError
{

void exception_translator( const SireError::exception &ex )
{
    PyErr_SetString(PyExc_UserWarning,ex.toString().toAscii());
}

void SIREERROR_EXPORT export_exceptions()
{
    register_exception_translator<SireError::exception>(&exception_translator);
}

}
