#ifndef PYWRAP_SIREQT_WRAP_SIREQT_H
#define PYWRAP_SIREQT_WRAP_SIREQT_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireQt
{
    void export_SireQt();
}

BOOST_PYTHON_MODULE(_Qt)
{
    SireQt::export_SireQt();
}

#endif
