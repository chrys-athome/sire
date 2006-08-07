
#include <Python.h>
#include <boost/python.hpp>

#include "SireQt/qset.hpp"

using namespace SireQt;
    
void export_QSet_int64()
{                            
    wrap_QSet<qint64>("QSet_qint64_");
    wrap_QSet<quint64>("QSet_quint64_");
}
