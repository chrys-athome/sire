#ifndef SIREFF_WRAP_SIREFF_H
#define SIREFF_WRAP_SIREFF_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireFF
{
    void export_SireFF();
}

BOOST_PYTHON_MODULE(_FF)
{
    SireFF::export_SireFF();
}

#endif
