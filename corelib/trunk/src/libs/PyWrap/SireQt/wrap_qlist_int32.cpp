
#include <Python.h>
#include <boost/python.hpp>

#include "SireQt/qlist.hpp"

using namespace SireQt;
    
void export_QList_int32()
{                            
    wrap_QList<qint32>("QList_qint32_");
    wrap_QList<quint32>("QList_quint32_");
}
