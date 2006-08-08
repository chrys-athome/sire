
#include <Python.h>
#include <boost/python.hpp>

#include "SireQt/qset.hpp"

using namespace SireQt;
    
void export_QSet_int16()
{                            
    wrap_QSet<qint16>("QSet_qint16_");
    wrap_QSet<quint16>("QSet_quint16_");
}
