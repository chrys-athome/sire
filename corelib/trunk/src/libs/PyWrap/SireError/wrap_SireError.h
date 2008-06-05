#ifndef WRAP_SIREERROR_H
#define WRAP_SIREERROR_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireError
{
void export_SireError();
}

BOOST_PYTHON_MODULE(_Error)
{
    SireError::export_SireError();
}

#endif
