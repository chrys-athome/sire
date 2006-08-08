
#include <Python.h>
#include <boost/python.hpp>

#include <QString>
#include "SireQt/qset.hpp"

using namespace SireQt;
    
void export_QSet_QString()
{                            
    wrap_QSet<QString>("QSet_QString_");
}
