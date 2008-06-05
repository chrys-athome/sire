
#include <Python.h>
#include <boost/python.hpp>

#include <QString>

#include "SireQt/qmap.hpp"

using namespace SireQt;
    
void export_QMap_QString_QString()
{                            
    wrap_QMap<QString,QString>("QMap_QString_QString_");
}
