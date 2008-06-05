
#include <Python.h>
#include <boost/python.hpp>

#include "qhash_real.h"

#include "SireQt/qlist.hpp"

using namespace SireQt;
    
void export_QList_real()
{                            
    wrap_QList<float>("QList_real_");
    wrap_QList<double>("QList_double_");
}
