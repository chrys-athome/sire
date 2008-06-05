
#include <Python.h>

#include <boost/python.hpp>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <wchar.h>

#include "SirePy/str.hpp"

using namespace boost::python;

QString wrap_leftJustified3(const QString& string, int width, QChar fill, bool truncate)
{
    return string.leftJustified(width,fill,truncate);
}

QString wrap_leftJustified2(const QString& string, int width, QChar fill)
{
    return string.leftJustified(width,fill);
}

QString wrap_leftJustified1(const QString& string, int width)
{
    return string.leftJustified(width);
}

QString wrap_mid2(const QString &string, int i, int len)
{
    return string.mid(i,len);
}

QString wrap_mid1(const QString &string, int i)
{
    return string.mid(i);
}

template<class T>
QString& wrap_remove2(QString &string, T str, Qt::CaseSensitivity cs)
{
    return string.remove(str,cs);
}

template<class T>
QString& wrap_remove1(QString &string, T str)
{
    return string.remove(str);
}

template<class T, class S>
QString& wrap_replace2(QString &string, T before, S after, Qt::CaseSensitivity cs)
{
    return string.replace(before,after,cs);
}

template<class T, class S>
QString& wrap_replace1(QString &string, T before, S after)
{
    return string.replace(before,after);
}

QString wrap_rightJustified3(const QString &string, int width, QChar fill, bool truncate)
{
    return string.rightJustified(width, fill, truncate);
}

QString wrap_rightJustified2(const QString &string, int width, QChar fill)
{
    return string.rightJustified(width, fill);
}

QString wrap_rightJustified1(const QString &string, int width)
{
    return string.rightJustified(width);
}

template<class T>
QString wrap_section4(const QString &string, QChar sep, int start, 
                      int end, QString::SectionFlags flags)
{
    return string.section(sep,start,end,flags);
}

template<class T>
QString wrap_section3(const QString &string, QChar sep, int start, int end)
{
    return string.section(sep,start,end);
}

template<class T>
QString wrap_section2(const QString &string, QChar sep, int start)
{
    return string.section(sep,start);
}

template<typename T>
QString& wrap_setNum_int2(QString &string, T n, int base)
{
    return string.setNum(n,base);
}

template<typename T>
QString& wrap_setNum_int1(QString &string, T n)
{
    return string.setNum(n);
}

template<typename T>
QString& wrap_setNum_float3(QString &string, T n, char f, int prec)
{
    return string.setNum(n,f,prec);
}

template<typename T>
QString& wrap_setNum_float2(QString &string, T n, char f)
{
    return string.setNum(n,f);
}

template<typename T>
QString& wrap_setNum_float1(QString &string, T n)
{
    return string.setNum(n);
}

template<class T>
QStringList wrap_split3(const QString &string, const T& sep, QString::SplitBehavior behavior, 
                        Qt::CaseSensitivity cs)
{
    return string.split(sep, behavior, cs);
}

template<class T>
QStringList wrap_split2(const QString &string, const T& sep, QString::SplitBehavior behavior)
{
    return string.split(sep, behavior);
}

template<class T>
QStringList wrap_split1(const QString &string, const T& sep)
{
    return string.split(sep);
}

template<class T>
bool wrap_startsWith2(const QString &string, const T& s, Qt::CaseSensitivity cs)
{
    return string.startsWith(s,cs);
}

template<class T>
bool wrap_startsWith1(const QString &string, const T& s)
{
    return string.startsWith(s);
}

double wrap_toDouble1(const QString &string, bool * ok)
{
    return string.toDouble(ok);
}

double wrap_toDouble0(const QString &string)
{
    return string.toDouble();
}

float wrap_toFloat1(const QString &string, bool *ok )
{
    return string.toFloat(ok);
}

float wrap_toFloat0(const QString &string)
{
    return string.toFloat();
}

short wrap_toShort2(const QString &string, bool *ok, int base)
{
    return string.toShort(ok,base);
}

short wrap_toShort1(const QString &string, bool *ok)
{
    return string.toShort(ok);
}

short wrap_toShort0(const QString &string)
{
    return string.toShort();
}

qlonglong wrap_toLongLong2(const QString &string, bool *ok, int base)
{
    return string.toLongLong(ok,base);
}

qlonglong wrap_toLongLong1(const QString &string, bool *ok)
{
    return string.toLongLong(ok);
}

qlonglong wrap_toLongLong0(const QString &string)
{
    return string.toLongLong();
}

qulonglong wrap_toULongLong2(const QString &string, bool *ok, int base)
{
    return string.toULongLong(ok,base);
}

qulonglong wrap_toULongLong1(const QString &string, bool *ok)
{
    return string.toULongLong(ok);
}

qulonglong wrap_toULongLong0(const QString &string)
{
    return string.toULongLong();
}

int wrap_toInt2(const QString &string, bool *ok, int base)
{
    return string.toInt(ok,base);
}

int wrap_toInt1(const QString &string, bool *ok)
{
    return string.toInt(ok);
}

int wrap_toInt0(const QString &string)
{
    return string.toInt();
}

long wrap_toLong2(const QString &string, bool *ok, int base)
{
    return string.toLong(ok,base);
}

long wrap_toLong1(const QString &string, bool *ok)
{
    return string.toLong(ok);
}

long wrap_toLong0(const QString &string)
{
    return string.toLong();
}

ushort wrap_toUShort2(const QString &string, bool *ok, int base)
{
    return string.toUShort(ok,base);
}

ushort wrap_toUShort1(const QString &string, bool *ok)
{
    return string.toUShort(ok);
}

ushort wrap_toUShort0(const QString &string)
{
    return string.toUShort();
}

ulong wrap_toULong2(const QString &string, bool *ok, int base)
{
    return string.toULong(ok,base);
}

ulong wrap_toULong1(const QString &string, bool *ok)
{
    return string.toULong(ok);
}

ulong wrap_toULong0(const QString &string)
{
    return string.toULong();
}

uint wrap_toUInt2(const QString &string, bool *ok, int base)
{
    return string.toUInt(ok,base);
}

uint wrap_toUInt1(const QString &string, bool *ok)
{
    return string.toUInt(ok);
}

uint wrap_toUInt0(const QString &string)
{
    return string.toUInt();
}

template<typename T>
QString wrap_number_int2(T n, int base)
{
    return QString::number(n, base);
}

template<typename T>
QString wrap_number_int1(T n)
{
    return QString::number(n);
}

QString wrap_number_float3(double n, char f, int prec)
{
    return QString::number(n,f,prec);
} 

QString wrap_number_float2(double n, char f)
{
    return QString::number(n,f);
} 

QString wrap_number_float1(double n)
{
    return QString::number(n);
} 

QString wrap_fromAscii2( const char * str, int size )
{
    return QString::fromAscii(str,size);
}

QString wrap_fromAscii1( const char * str )
{
    return QString::fromAscii(str);
}

QString wrap_fromLatin1_2( const char * str, int size )
{
    return QString::fromLatin1(str,size);
}

QString wrap_fromLatin1_1( const char * str )
{
    return QString::fromLatin1(str);
}

QString wrap_fromUtf8_2( const char * str, int size )
{
    return QString::fromUtf8(str,size);
}

QString wrap_fromUtf8_1( const char * str )
{
    return QString::fromUtf8(str);
}

QString wrap_fromUtf16_2( const ushort * unicode, int size )
{
    return QString::fromUtf16(unicode, size);
}

QString wrap_fromUtf16_1( const ushort * unicode )
{
    return QString::fromUtf16(unicode);
}

QString wrap_fromLocal8Bit2( const char *str, int size )
{
    return QString::fromLocal8Bit(str, size);
}

QString wrap_fromLocal8Bit1( const char *str )
{
    return QString::fromLocal8Bit(str);
}

void export_QString2(class_<QString> &wrapper)
{
    wrapper
        .def( "leftJustified", &wrap_leftJustified3 )
        .def( "leftJustified", &wrap_leftJustified2 )
        .def( "leftJustified", &wrap_leftJustified1 )
        
        .def( "length", &QString::length )
        
        .def( "localeAwareCompare", (int (QString::*)(const QString&) const)
                                        &QString::localeAwareCompare )
        .def( "localeAwareCompare", (int (*)(const QString&, const QString&))
                                        &QString::localeAwareCompare )
                                            .staticmethod("localeAwareCompare")
        
        .def( "mid", &wrap_mid2 )
        .def( "mid", &wrap_mid1 )
        
        .def( "normalized", (QString (QString::*)(QString::NormalizationForm) const)
                                &QString::normalized )
        .def( "normalized", 
                (QString (QString::*)(QString::NormalizationForm, QChar::UnicodeVersion) const)
                                &QString::normalized )

        .def( "prepend", (QString& (QString::*)(const QString&))
                                &QString::prepend,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "prepend", (QString& (QString::*)(const QLatin1String&))
                                &QString::prepend,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "prepend", (QString& (QString::*)(const QByteArray&))
                                &QString::prepend,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "prepend", (QString& (QString::*)(const char*))
                                &QString::prepend,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "prepend", (QString& (QString::*)(QChar))
                                &QString::prepend,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "push_back", (void (QString::*)(const QString&))
                                &QString::push_back )
        .def( "push_back", (void (QString::*)(QChar))
                                &QString::push_back )
        .def( "push_front", (void (QString::*)(const QString&))
                                &QString::push_front )
        .def( "push_front", (void (QString::*)(QChar))
                                &QString::push_front )
        
        .def( "remove", (QString& (QString::*)(int, int))
                                &QString::remove,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "remove", (QString& (QString::*)(const QRegExp&))
                                &QString::remove,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "remove", &wrap_remove2<const QString&>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "remove", &wrap_remove1<const QString&>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "remove", &wrap_remove2<QChar>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "remove", &wrap_remove1<QChar>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "replace", (QString& (QString::*)(int, int, const QString&))
                                &QString::replace,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "replace", (QString& (QString::*)(int, int, const QChar*, int))
                                &QString::replace,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "replace", (QString& (QString::*)(int, int, QChar))
                                &QString::replace,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "replace", wrap_replace2<const QString&,const QString&>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "replace", wrap_replace1<const QString&,const QString&>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "replace", wrap_replace2<QChar,const QString&>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "replace", wrap_replace1<QChar,const QString&>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "replace", wrap_replace2<QChar,QChar>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "replace", wrap_replace1<QChar,QChar>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "replace", (QString& (QString::*)(const QRegExp&, const QString&))
                                &QString::replace,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    
        .def( "reserve", &QString::reserve )
        .def( "resize", &QString::resize )
        .def( "right", &QString::right )
        
        .def( "rightJustified", &wrap_rightJustified3 )
        .def( "rightJustified", &wrap_rightJustified2 )
        .def( "rightJustified", &wrap_rightJustified1 )
        
        .def( "section", &wrap_section4<QChar> )
        .def( "section", &wrap_section3<QChar> )
        .def( "section", &wrap_section2<QChar> )
        .def( "section", &wrap_section4<const QString&> )
        .def( "section", &wrap_section3<const QString&> )
        .def( "section", &wrap_section2<const QString&> )
        .def( "section", &wrap_section4<const QRegExp&> )
        .def( "section", &wrap_section3<const QRegExp&> )
        .def( "section", &wrap_section2<const QRegExp&> )
        
        .def( "setNum", &wrap_setNum_int2<int>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int1<int>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int2<uint>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int1<uint>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int2<long>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int1<long>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int2<ulong>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int1<ulong>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int2<qlonglong>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int1<qlonglong>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int2<qulonglong>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int1<qulonglong>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int2<short>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int1<short>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int2<ushort>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_int1<ushort>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "setNum", &wrap_setNum_float3<double>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_float2<double>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_float1<double>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_float3<float>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_float2<float>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setNum", &wrap_setNum_float1<float>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "setUnicode", &QString::setUnicode,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setUtf16", &QString::setUtf16,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "simplified", &QString::simplified )
        .def( "size", &QString::size )
        
        .def( "split", &wrap_split3<QString> )
        .def( "split", &wrap_split2<QString> )
        .def( "split", &wrap_split1<QString> )
        .def( "split", &wrap_split3<QChar> )
        .def( "split", &wrap_split2<QChar> )
        .def( "split", &wrap_split1<QChar> )
        .def( "split", &wrap_split2<QRegExp> )
        .def( "split", &wrap_split1<QRegExp> )
        
        //.def( "sprintf" // how on earth could I wrap this???? )
    
        .def( "squeeze", &QString::squeeze )
        
        .def( "startsWith", &wrap_startsWith2<QString> )
        .def( "startsWith", &wrap_startsWith1<QString> )
        .def( "startsWith", &wrap_startsWith2<QLatin1String> )
        .def( "startsWith", &wrap_startsWith1<QLatin1String> )
        .def( "startsWith", &wrap_startsWith2<QChar> )
        .def( "startsWith", &wrap_startsWith1<QChar> )
        
        .def( "toAscii", &QString::toAscii )
        .def( "toDouble", &wrap_toDouble1 )
        .def( "toDouble", &wrap_toDouble0 )
        .def( "toFloat", &wrap_toFloat1 )
        .def( "toFloat", &wrap_toFloat0 )
        .def( "toInt", &wrap_toInt2 )
        .def( "toInt", &wrap_toInt1 )
        .def( "toInt", &wrap_toInt0 )
        .def( "toLatin1", &QString::toLatin1 )
        .def( "toLocal8Bit", &QString::toLocal8Bit )
        .def( "toLong", &wrap_toLong2 )
        .def( "toLong", &wrap_toLong1 )
        .def( "toLong", &wrap_toLong0 )
        .def( "toLongLong", &wrap_toLongLong2 )
        .def( "toLongLong", &wrap_toLongLong1 )
        .def( "toLongLong", &wrap_toLongLong0 )
        .def( "toLower", &QString::toLower )
        .def( "toShort", &wrap_toShort2 )
        .def( "toShort", &wrap_toShort1 )
        .def( "toShort", &wrap_toShort0 )
        .def( "toStdString", &QString::toStdString )
        .def( "toStdWString", &QString::toStdWString )
        .def( "toUInt", &wrap_toUInt2 )
        .def( "toUInt", &wrap_toUInt1 )
        .def( "toUInt", &wrap_toUInt0 )
        .def( "toULong", &wrap_toULong2 )
        .def( "toULong", &wrap_toULong1 )
        .def( "toULong", &wrap_toULong0 )
        .def( "toULongLong", &wrap_toULongLong2 )
        .def( "toULongLong", &wrap_toULongLong1 )
        .def( "toULongLong", &wrap_toULongLong0 )
        .def( "toUShort", &wrap_toUShort2 )
        .def( "toUShort", &wrap_toUShort1 )
        .def( "toUShort", &wrap_toUShort0 )
        .def( "toUtf8", &QString::toUtf8 )
        
        .def( "toUpper", &QString::toUpper )
        .def( "trimmed", &QString::trimmed )
        .def( "truncate", &QString::truncate )
    
        .def( "fromAscii", &wrap_fromAscii2 )
        .def( "fromAscii", &wrap_fromAscii1 ).staticmethod("fromAscii")
        .def( "fromLatin1", &wrap_fromLatin1_2 )
        .def( "fromLatin1", &wrap_fromLatin1_1 ).staticmethod("fromLatin1")
        .def( "fromLocal8Bit", &wrap_fromLocal8Bit2 )
        .def( "fromLocal8Bit", &wrap_fromLocal8Bit1 ).staticmethod("fromLocal8Bit")
        .def( "fromStdString", &QString::fromStdString ).staticmethod("fromStdString")
        .def( "fromStdWString", &QString::fromStdWString ).staticmethod("fromStdWString")
        .def( "fromUtf8", &wrap_fromUtf8_2 )
        .def( "fromUtf8", &wrap_fromUtf8_1 ).staticmethod("fromUtf8")
        .def( "fromUtf16", &wrap_fromUtf16_2 )
        .def( "fromUtf16", &wrap_fromUtf16_1 ).staticmethod("fromUtf16")
        
        .def( "number", &wrap_number_int2<long> )
        .def( "number", &wrap_number_int1<long> )
        .def( "number", &wrap_number_int2<ulong> )
        .def( "number", &wrap_number_int1<ulong> )
        .def( "number", &wrap_number_int2<int> )
        .def( "number", &wrap_number_int1<int> )
        .def( "number", &wrap_number_int2<uint> )
        .def( "number", &wrap_number_int1<uint> )
        .def( "number", &wrap_number_int2<qlonglong> )
        .def( "number", &wrap_number_int1<qlonglong> )
        .def( "number", &wrap_number_int2<qulonglong> )
        .def( "number", &wrap_number_int1<qulonglong> )
        .def( "number", &wrap_number_float3 )
        .def( "number", &wrap_number_float2 )
        .def( "number", &wrap_number_float1 ).staticmethod("number")
    ;



}
