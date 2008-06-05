
#include <Python.h>
#include <boost/python.hpp>

#include "SireQt/qset.hpp"

using namespace SireQt;
    
void export_QSet_int32()
{                            
    wrap_QSet<qint32>("QSet_qint32_");
    wrap_QSet<quint32>("QSet_quint32_");
}
