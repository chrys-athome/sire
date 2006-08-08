
#include <Python.h>
#include <boost/python.hpp>

#include <QString>

#include "SireQt/qvector.hpp"

using namespace SireQt;

void export_QVector_int8()
{
    wrap_QVector<qint8>("QVector_qint8_");
    wrap_QVector<quint8>("QVector_quint8_");
    
}
