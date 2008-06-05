
#include <Python.h>
#include <boost/python.hpp>

#include <QString>

#include "SireQt/qvector.hpp"

using namespace SireQt;

void export_QVector_int64()
{
    wrap_QVector<qint64>("QVector_qint64_");
    wrap_QVector<quint64>("QVector_quint64_");
    
}
