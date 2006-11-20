
#include <Python.h>
#include <boost/python.hpp>

#include <QByteArray>

#include "qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;

void  
SIREQT_EXPORT
export_QByteArray()
{
    class_<QByteArray>("QByteArray", init<>())
        .def( init<const char*>() )
        .def( init<const char*, int>() )
        .def( init<int, char>() )
        .def( init<const QByteArray&>() )
    
        .def( "at", &QByteArray::at )
        .def( "size", &QByteArray::size )
    
        .def( "__rrshift__", &__rrshift__QDataStream<QByteArray>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<QByteArray>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    
#warning need to complete wrapping of QByteArray
    ;
}
