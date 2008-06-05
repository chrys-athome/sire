#ifndef PYWRAP_SIREQT_QDATASTREAM_HPP
#define PYWRAP_SIREQT_QDATASTREAM_HPP

#include <Python.h>

#include <QDataStream>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireQt
{

/** Expose the QDataStream serialisation function */
template<class T>
QDataStream& __rlshift__QDataStream(const T &value, QDataStream &ds)
{
    ds << value;
    return ds;
}

/** Expose the QDataStream deserialisation function */
template<class T>
QDataStream& __rrshift__QDataStream(T &value, QDataStream &ds)
{
    ds >> value;
    return ds;
}

}

SIRE_END_HEADER

#endif
