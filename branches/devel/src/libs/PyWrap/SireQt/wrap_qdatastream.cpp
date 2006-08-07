
#include <Python.h>
#include <boost/python.hpp>

#include <QDataStream>
#include <QIODevice>

using namespace boost::python;

template<typename T>
QDataStream& __lshift__(QDataStream &ds, const T &value)
{
    ds << value;
    return ds;
}

template<typename T>
QDataStream& __rshift__(QDataStream &ds, T &value)
{
    ds >> value;
    return ds;
}

void export_QDataStream()
{
    scope QDataStream_scope = 
    class_<QDataStream, boost::noncopyable>("QDataStream", init<>())
        .def( init<QIODevice*>() )
        .def( init<QByteArray*, QIODevice::OpenMode>() )
        .def( init<const QByteArray&>() )
        
        .def( "__lshift__", &__lshift__<qint8>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<qint8>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<bool>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<bool>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<quint8>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<quint8>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<quint16>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<quint16>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<qint16>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<qint16>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<quint64>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<quint64>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<qint64>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<qint64>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<float>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<float>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<double>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<double>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<const char*>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<char*>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "atEnd", &QDataStream::atEnd )
        .def( "byteOrder", &QDataStream::byteOrder )
        .def( "device", &QDataStream::device,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "readBytes", &QDataStream::readBytes,
                                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "readRawData", &QDataStream::readRawData )
        .def( "resetStatus", &QDataStream::resetStatus )
        .def( "setByteOrder", &QDataStream::setByteOrder )
        .def( "setDevice", &QDataStream::setDevice )
        .def( "setStatus",  &QDataStream::setStatus )
        .def( "setVersion", &QDataStream::setVersion )
        .def( "skipRawData", &QDataStream::skipRawData )
        .def( "status", &QDataStream::status )
        .def( "unsetDevice", &QDataStream::unsetDevice )
        .def( "version", &QDataStream::version )
        .def( "writeBytes", &QDataStream::writeBytes,
                                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "writeRawData", &QDataStream::writeRawData )
    ;

    enum_<QDataStream::ByteOrder>("ByteOrder")
        .value( "BigEndian", QDataStream::BigEndian )
        .value( "LittleEndian", QDataStream::LittleEndian )
    ;
    
    enum_<QDataStream::Status>("Status")
        .value( "Ok", QDataStream::Ok )
        .value( "ReadPastEnd", QDataStream::ReadPastEnd )
        .value( "ReadCorruptData", QDataStream::ReadCorruptData )
    ;
    
    enum_<QDataStream::Version>("Version")
        .value( "Qt_1_0", QDataStream::Qt_1_0 )
        .value( "Qt_2_0", QDataStream::Qt_2_0 )
        .value( "Qt_2_1", QDataStream::Qt_2_1 )
        .value( "Qt_3_0", QDataStream::Qt_3_0 )
        .value( "Qt_3_1", QDataStream::Qt_3_1 )
        .value( "Qt_3_3", QDataStream::Qt_3_3 )
        .value( "Qt_4_0", QDataStream::Qt_4_0 )
        .value( "Qt_4_1", QDataStream::Qt_4_1 )
    ;
}
