
#include <Python.h>
#include <boost/python.hpp>

#include <QString>

#include "SireQt/qvector.hpp"

using namespace SireQt;

void export_QVector_int16()
{
    wrap_QVector<qint16>("QVector_qint16_");
    wrap_QVector<quint16>("QVector_quint16_");
    
}
