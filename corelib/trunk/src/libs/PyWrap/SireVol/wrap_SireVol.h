#ifndef SIREVOL_WRAP_SIREVOL_H
#define SIREVOL_WRAP_SIREVOL_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireVol
{
    void export_SireVol();
}

BOOST_PYTHON_MODULE(_Vol)
{
    SireVol::export_SireVol();
}

#endif
