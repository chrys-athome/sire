#ifndef SIRECAS_WRAP_SIRECAS_H
#define SIRECAS_WRAP_SIRECAS_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireCAS
{
    void export_SireCAS();
}

BOOST_PYTHON_MODULE(_CAS)
{
    SireCAS::export_SireCAS();
}

#endif
