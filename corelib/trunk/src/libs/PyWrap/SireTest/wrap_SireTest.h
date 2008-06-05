#ifndef SIRETEST_WRAP_SIRETEST_H
#define SIRETEST_WRAP_SIRETEST_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireTest
{
    void export_SireTest();
}

BOOST_PYTHON_MODULE(_Test)
{
    SireTest::export_SireTest();
}

#endif
