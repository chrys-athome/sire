
#include <Python.h>
#include <boost/python.hpp>

#include "SireQt/qset.hpp"

using namespace SireQt;
    
void export_QSet_int8()
{                            
    wrap_QSet<qint8>("QSet_qint8_");
    wrap_QSet<quint8>("QSet_quint8_");
}
