
#include <Python.h>
#include <boost/python.hpp>

#include <QTextStream>
#include <QTextCodec>
#include <QByteArray>
#include <QString>
#include <QIODevice>
#include <QChar>

using namespace boost::python;

QString wrap_readLine1(QTextStream &ts, qint64 maxlen)
{
    return ts.readLine(maxlen);
}

QString wrap_readLine0(QTextStream &ts)
{
    return ts.readLine();
}

void wrap_setString2(QTextStream &ts, QString *string, QIODevice::OpenMode openMode)
{
    ts.setString(string,openMode);
}

void wrap_setString1(QTextStream &ts, QString *string)
{
    ts.setString(string);
}

template<typename T>
QTextStream& __lshift__(QTextStream &ts, T value)
{
    ts << value;
    return ts;
}

template<typename T>
QTextStream& __rshift__(QTextStream &ts, T value)
{
    ts >> value;
    return ts;
}

void export_QTextStream()
{
    scope QTextStream_scope = class_<QTextStream, boost::noncopyable>
                                                            ("QTextStream", init<>())
        .def( init<QIODevice*>() )
        .def( init<QString*, optional<QIODevice::OpenMode> >() )
        .def( init<QByteArray*, optional<QIODevice::OpenMode> >() )
        .def( init<const QByteArray&, optional<QIODevice::OpenMode> >() )
        
        .def( "atEnd", &QTextStream::atEnd )
        .def( "autoDetectUnicode", &QTextStream::autoDetectUnicode )
        .def( "codec", &QTextStream::codec,
                 return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "device", &QTextStream::device,
                 return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "fieldAlignment", &QTextStream::fieldAlignment )
        .def( "fieldWidth", &QTextStream::fieldWidth )
        .def( "flush", &QTextStream::flush )
        .def( "generateByteOrderMark", &QTextStream::generateByteOrderMark )
        .def( "integerBase", &QTextStream::integerBase )
        .def( "numberFlags", &QTextStream::numberFlags )
        .def( "padChar", &QTextStream::padChar )
        .def( "read", &QTextStream::read )
        .def( "readAll", &QTextStream::readAll )
        .def( "read", &wrap_readLine0 )
        .def( "read", &wrap_readLine1 )
        .def( "realNumberNotation", &QTextStream::realNumberNotation )
        .def( "realNumberPrecision", &QTextStream::realNumberPrecision )
        .def( "reset", &QTextStream::reset )
        .def( "resetStatus", &QTextStream::resetStatus )
        .def( "seek", &QTextStream::seek )
        .def( "setAutoDetectUnicode", &QTextStream::setAutoDetectUnicode )
        .def( "setCodec", (void (QTextStream::*)(QTextCodec*))
                      &QTextStream::setCodec )
        .def( "setCodec", (void (QTextStream::*)(const char*))
                      &QTextStream::setCodec )
        .def( "setDevice", &QTextStream::setDevice )
        .def( "setFieldAlignment", &QTextStream::setFieldAlignment )
        .def( "setFieldWidth", &QTextStream::setFieldWidth )
        .def( "setGenerateByteOrderMark", &QTextStream::setGenerateByteOrderMark )
        .def( "setIntegerBase", &QTextStream::setIntegerBase )
        .def( "setNumberFlags", &QTextStream::setNumberFlags )
        .def( "setPadChar", &QTextStream::setPadChar )
        .def( "setRealNumberNotation", &QTextStream::setRealNumberNotation )
        .def( "setRealNumberPrecision", &QTextStream::setRealNumberPrecision )
        .def( "setStatus", &QTextStream::setStatus )
        .def( "setString", &wrap_setString2 )
        .def( "setString", &wrap_setString1 )
        .def( "skipWhiteSpace", &QTextStream::skipWhiteSpace )
        .def( "status", &QTextStream::status )
        .def( "string", &QTextStream::string,
                 return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    
        .def( "__lshift__", &__lshift__<QChar>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<QChar&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<signed short>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<signed short&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<float>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<float&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<const QString&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<QString&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<char>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<char&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<unsigned short>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<unsigned short&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<signed int>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<signed int&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<unsigned int>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<unsigned int&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<signed long>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<signed long&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<unsigned long>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<unsigned long&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<qlonglong>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<qlonglong&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<qulonglong>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<qulonglong&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<double>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<double&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<const QByteArray&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<QByteArray&>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__lshift__", &__lshift__<const char*>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rshift__", &__rshift__<char*>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;
    
    enum_<QTextStream::FieldAlignment>("FieldAlignment")
        .value( "AlignLeft", QTextStream::AlignLeft )
        .value( "AlignRight", QTextStream::AlignRight )
        .value( "AlignCenter", QTextStream::AlignCenter )
        .value( "AlignAccountingStyle", QTextStream::AlignAccountingStyle )
    ;
    
    enum_<QTextStream::NumberFlag>("NumberFlag")
        .value( "ShowBase", QTextStream::ShowBase )
        .value( "ForcePoint", QTextStream::ForcePoint )
        .value( "ForceSign", QTextStream::ForceSign )
        .value( "UppercaseBase", QTextStream::UppercaseBase )
        .value( "UppercaseDigits", QTextStream::UppercaseDigits )    
    ;

    implicitly_convertible<int, QTextStream::NumberFlags>();
    implicitly_convertible<QTextStream::NumberFlag, QTextStream::NumberFlags>();

    enum_<QTextStream::RealNumberNotation>("RealNumberNotation")
        .value( "ScientificNotation", QTextStream::ScientificNotation )
        .value( "FixedNotation", QTextStream::FixedNotation )
        .value( "SmartNotation", QTextStream::SmartNotation )    
    ;
    
    enum_<QTextStream::Status>("Status")
        .value( "Ok", QTextStream::Ok )
        .value( "ReadPastEnd", QTextStream::ReadPastEnd )
        .value( "ReadCorruptData", QTextStream::ReadCorruptData )
    ;

}
