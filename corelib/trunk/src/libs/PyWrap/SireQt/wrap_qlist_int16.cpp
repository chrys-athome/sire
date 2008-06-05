
#include <Python.h>
#include <boost/python.hpp>

#include "SireQt/qlist.hpp"

using namespace SireQt;
    
void export_QList_int16()
{                            
    wrap_QList<qint16>("QList_qint16_");
    wrap_QList<quint16>("QList_quint16_");
}
