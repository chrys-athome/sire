
#include <Python.h>
#include <boost/python.hpp>

#include "qhash_real.h"

#include "SireQt/qvector.hpp"

using namespace SireQt;
    
void export_QVector_real()
{                            
    wrap_QVector<float>("QVector_float_");
    wrap_QVector<double>("QVector_double_");
}
