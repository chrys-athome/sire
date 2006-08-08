#ifndef SIREMATHS_WRAP_SIREMATHS_H
#define SIREMATHS_WRAP_SIREMATHS_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireMaths
{
    void export_SireMaths();
}
    
BOOST_PYTHON_MODULE(_Maths)
{
    SireMaths::export_SireMaths();
}


#endif
