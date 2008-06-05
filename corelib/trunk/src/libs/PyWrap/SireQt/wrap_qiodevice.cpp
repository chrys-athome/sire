
#include <Python.h>
#include <boost/python.hpp>

#include <QIODevice>

using namespace boost::python;

/** Provide a basic wrapping of QIODevice so that it can be seen as the common
    base of the other IO device classes. */
void export_QIODevice()
{
    scope QIODevice_scope = 
    class_<QIODevice, boost::noncopyable>("QIODevice", no_init)
        .def( "atEnd", &QIODevice::atEnd )
        .def( "bytesAvailable", &QIODevice::bytesAvailable )
        .def( "bytesToWrite", &QIODevice::bytesToWrite )
        .def( "canReadLine", &QIODevice::canReadLine )
        .def( "close", &QIODevice::close )
        .def( "errorString", &QIODevice::errorString )
        .def( "getChar", &QIODevice::getChar )
        .def( "isOpen", &QIODevice::isOpen )
        .def( "isReadable", &QIODevice::isReadable )
        .def( "isSequential", &QIODevice::isSequential )
        .def( "isTextModeEnabled", &QIODevice::isTextModeEnabled )
        .def( "isWritable", &QIODevice::isWritable )
        .def( "open", &QIODevice::open )
        .def( "openMode", &QIODevice::openMode )
        .def( "peek", (qint64 (QIODevice::*)(char*, qint64))
                          &QIODevice::peek )
        .def( "peek", (QByteArray (QIODevice::*)(qint64))
                          &QIODevice::peek )
        .def( "pos", &QIODevice::pos )
        .def( "putChar", &QIODevice::putChar )
        .def( "read", (qint64 (QIODevice::*)(char*, qint64))
                          &QIODevice::read )
        .def( "read", (QByteArray (QIODevice::*)(qint64))
                          &QIODevice::read )
        .def( "readAll", &QIODevice::readAll )
        .def( "readLine", (qint64 (QIODevice::*)(char*, qint64))
                          &QIODevice::readLine )
        .def( "readLine", (QByteArray (QIODevice::*)(qint64 maxSize))
                          &QIODevice::readLine )
        .def( "reset", &QIODevice::reset )
        .def( "seek", &QIODevice::seek )
        .def( "setTextModeEnabled", &QIODevice::setTextModeEnabled )
        .def( "size", &QIODevice::size )
        .def( "ungetChar", &QIODevice::ungetChar )
        .def( "waitForBytesWritten", &QIODevice::waitForBytesWritten )
        .def( "waitForReadyRead", &QIODevice::waitForReadyRead )
        .def( "write", (qint64 (QIODevice::*)(const char*, qint64))
                        &QIODevice::write )
        .def( "write", (qint64 (QIODevice::*)(const QByteArray&))
                        &QIODevice::write )
    ;

    enum_<QIODevice::OpenModeFlag>("OpenMode")
        .value( "NotOpen", QIODevice::NotOpen )
        .value( "ReadOnly", QIODevice::ReadOnly )
        .value( "WriteOnly", QIODevice::WriteOnly )
        .value( "ReadWrite", QIODevice::ReadWrite )
        .value( "Append", QIODevice::Append )
        .value( "Truncate", QIODevice::Truncate )
        .value( "Text", QIODevice::Text )
        .value( "Unbuffered", QIODevice::Unbuffered )
    ;

    implicitly_convertible< int, QFlags<QIODevice::OpenModeFlag> >();
    implicitly_convertible< QIODevice::OpenModeFlag, QFlags<QIODevice::OpenModeFlag> >();

}
