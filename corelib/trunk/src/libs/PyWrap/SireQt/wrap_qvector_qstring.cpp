
#include <Python.h>
#include <boost/python.hpp>

#include <QString>

#include "SireQt/qvector.hpp"

using namespace SireQt;
    
void export_QVector_QString()
{                            
    wrap_QVector<QString>("QVector_QString_");
}
