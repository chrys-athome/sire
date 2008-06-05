
#include <Python.h>
#include <boost/python.hpp>

#include "qhash_real.h"

#include "SireQt/qset.hpp"

using namespace SireQt;
    
void export_QSet_real()
{                            
    wrap_QSet<float>("QSet_float_");
    wrap_QSet<double>("QSet_double_");
}
