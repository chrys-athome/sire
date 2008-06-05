#ifndef SIREMM_WRAP_SIREMM_H
#define SIREMM_WRAP_SIREMM_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireMM
{
    void export_SireMM();
}

BOOST_PYTHON_MODULE(_MM)
{
    SireMM::export_SireMM();
}

#endif
