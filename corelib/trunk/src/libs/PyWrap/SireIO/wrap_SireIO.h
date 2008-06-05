#ifndef SIREIO_WRAP_SIREIO_H
#define SIREIO_WRAP_SIREIO_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireIO
{
    void export_SireIO();
}

BOOST_PYTHON_MODULE(_IO)
{
    SireIO::export_SireIO();
}

#endif
