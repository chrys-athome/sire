
#include <Python.h>
#include <boost/python.hpp>

#include "SireQt/qlist.hpp"

using namespace SireQt;
    
void export_QList_int8()
{                            
    wrap_QList<qint8>("QList_qint8_");
    wrap_QList<quint8>("QList_quint8_");
}
