#ifndef SIREDB_WRAP_SIREDB_H
#define SIREDB_WRAP_SIREDB_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireDB
{
    void export_SireDB();
}

BOOST_PYTHON_MODULE(_DB)
{
    SireDB::export_SireDB();
}

#endif
