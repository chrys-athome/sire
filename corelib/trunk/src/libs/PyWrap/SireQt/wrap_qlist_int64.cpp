
#include <Python.h>
#include <boost/python.hpp>

#include "SireQt/qlist.hpp"

using namespace SireQt;
    
void export_QList_int64()
{                            
    wrap_QList<qint64>("QList_qint64_");
    wrap_QList<quint64>("QList_quint64_");
}
