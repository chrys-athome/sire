#ifndef SIREMOVE_WRAP_SIREMOVE_H
#define SIREMOVE_WRAP_SIREMOVE_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireMove
{
    void export_SireMove();
}

BOOST_PYTHON_MODULE(_Move)
{
    SireMove::export_SireMove();
}

#endif
