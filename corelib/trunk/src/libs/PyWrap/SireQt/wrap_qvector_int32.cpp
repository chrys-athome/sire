
#include <Python.h>
#include <boost/python.hpp>

#include <QString>

#include "SireQt/qvector.hpp"

using namespace SireQt;
    
void export_QVector_int32()
{
    //can only expose two qvectors at a time due to memory problems...
    wrap_QVector<qint32>("QVector_qint32_");
    wrap_QVector<quint32>("QVector_quint32_");
}
