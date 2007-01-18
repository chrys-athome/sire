#ifndef SIRESYSTEM_WRAP_SIRESYSTEM_H
#define SIRESYSTEM_WRAP_SIRESYSTEM_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireSystem
{
    void export_SireSystem();
}

BOOST_PYTHON_MODULE(_System)
{
    SireSystem::export_SireSystem();
}

#endif
