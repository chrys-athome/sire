#ifndef SIREMOL_WRAP_SIREMOL_H
#define SIREMOL_WRAP_SIREMOL_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireMol
{
    void export_SireMol();
}

BOOST_PYTHON_MODULE(_Mol)
{
    SireMol::export_SireMol();
}

#endif
