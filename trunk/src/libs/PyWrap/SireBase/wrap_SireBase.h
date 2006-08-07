#ifndef SIREBASE_WRAP_SIREBASE_H
#define SIREBASE_WRAP_SIREBASE_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireBase
{
void export_SireBase();
}

BOOST_PYTHON_MODULE(_Base)
{
    SireBase::export_SireBase();
}

#endif
