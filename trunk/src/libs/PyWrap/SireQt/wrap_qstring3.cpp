
#include <Python.h>

#include <boost/python.hpp>
#include <QString>
#include <QRegExp>
#include <wchar.h>

#include "SirePy/pycontainer.hpp"
#include "qdatastream.hpp"

using namespace boost::python;

using namespace SirePy;
using namespace SireQt;

void __delitem__QString(QString &string, int i)
{
    int string_size = string.size();
    
    //implement negative indicies
    if (i < 0) i += string_size;
    
    if ( i < 0 or i >= string_size )
    {
        PyErr_SetString( PyExc_IndexError, 
                qPrintable(QObject::tr(
                    "String index out of range (%1 from %2)")
                        .arg(i).arg(string_size)) ); 

        boost::python::throw_error_already_set();
    }

    string.remove(i,1);
}

QCharRef __getitem__QString(QString &string, int i)
{
    int string_size = string.size();
    
    //implement negative indicies
    if (i < 0) i += string_size;
    
    if ( i < 0 or i >= string_size )
    {
        PyErr_SetString( PyExc_IndexError, 
                qPrintable(QObject::tr(
                    "String index out of range (%1 from %2)")
                        .arg(i).arg(string_size)) ); 

        boost::python::throw_error_already_set();
    }

    return string[i];
}

void export_QString3(class_<QString> &wrapper)
{
    wrapper
        .def( self == self )
        .def( self != self )
        .def( self == other<QLatin1String>() )
        .def( self != other<QLatin1String>() )
        .def( self == other<QByteArray>() )
        .def( self != other<QByteArray>() )
        .def( self == other<const char*>() )
        .def( self != other<const char*>() )
        
        .def( self += self )
        .def( self += other<QLatin1String>() )
        .def( self += other<QByteArray>()  )
        .def( self += other<const char*>() )
        .def( self += char() )
        .def( self += QChar() )
        
        .def( self < self )
        .def( self <= self )
        .def( self > self )
        .def( self >= self )
        
        .def( self < other<QLatin1String>() )
        .def( self <= other<QLatin1String>() )
        .def( self > other<QLatin1String>() )
        .def( self >= other<QLatin1String>() )
        
        .def( self < other<QByteArray>() )
        .def( self <= other<QByteArray>() )
        .def( self > other<QByteArray>() )
        .def( self >= other<QByteArray>() )
        
        .def( self < other<const char*>() )
        .def( self <= other<const char*>() )
        .def( self > other<const char*>() )
        .def( self >= other<const char*>() )
        
        .def( "__getitem__", &__getitem__QString )
        .def( "__getitem__", &__getitem__list_const<QString,QChar> )
        .def( "__getitem__", &__getitem__slice<QString> )
        .def( "__setitem__", &__setitem__list<QString,QChar> )
        .def( "__delitem__", &__delitem__QString )
        .def( "__contains__", &__contains__<QString,QChar> )
        .def( "__contains__", &__contains__<QString,QString> )
        
        .def( "__rrshift__", &__rrshift__QDataStream<QString>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<QString>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;
    
    //also need to export the QCharRef class, which is used as a helper
    //class for QString for operator[] - it is exactly like QChar
    class_<QCharRef>("QCharRef", no_init)
        .def( "isNull", &QCharRef::isNull )
        .def( "isPrint", &QCharRef::isPrint )
        .def( "isPunct", &QCharRef::isPunct )
        .def( "isSpace", &QCharRef::isSpace )
        .def( "isMark", &QCharRef::isMark )
        .def( "isLetter", &QCharRef::isLetter )
        .def( "isNumber", &QCharRef::isNumber )
        .def( "isLetterOrNumber", &QCharRef::isLetterOrNumber )
        .def( "isDigit", &QCharRef::isDigit )
        .def( "digitValue", &QCharRef::digitValue )
        .def( "toLower", &QCharRef::toLower )
        .def( "toUpper", &QCharRef::toUpper )
        .def( "category", &QCharRef::category )
        .def( "direction", &QCharRef::direction )
        //.def( "joining", &QCharRef::joining ) //preliminary function - subject to change
        .def( "hasMirrored", &QCharRef::hasMirrored )
        .def( "mirroredChar", &QCharRef::mirroredChar )
        .def( "decomposition", &QCharRef::decomposition )
        .def( "decompositionTag", &QCharRef::decompositionTag )
        .def( "combiningClass", &QCharRef::combiningClass )
        .def( "unicodeVersion", &QCharRef::unicodeVersion )
        .def( "cell", &QCharRef::cell )
        .def( "row", &QCharRef::row )
        .def( "setCell", &QCharRef::setCell )
        .def( "setRow", &QCharRef::setRow )
        .def( "toAscii", &QCharRef::toAscii )
        .def( "toLatin1", &QCharRef::toLatin1 )
        .def( "unicode", &QCharRef::unicode )
    ;
    
    implicitly_convertible<QCharRef,QChar>();
    
    scope QString_scope = wrapper;
    
    enum_<QString::NormalizationForm>("NormalizationForm")
        .value( "NormalizationForm_D", QString::NormalizationForm_D )
        .value( "NormalizationForm_C", QString::NormalizationForm_C )
        .value( "NormalizationForm_KD", QString::NormalizationForm_KD )
        .value( "NormalizationForm_KC", QString::NormalizationForm_KC )
    ;
    
    enum_<QString::SectionFlag>("SectionFlag")
        .value( "SectionDefault", QString::SectionDefault )
        .value( "SectionSkipEmpty", QString::SectionSkipEmpty )
        .value( "SectionIncludeLeadingSep", QString::SectionIncludeLeadingSep )
        .value( "SectionIncludeTrailingSep", QString::SectionIncludeTrailingSep )
        .value( "SectionCaseInsensitiveSeps", QString::SectionCaseInsensitiveSeps )    
    ;
    
    implicitly_convertible<int, QString::SectionFlags>();
    implicitly_convertible<QString::SectionFlag, QString::SectionFlags>();
    
    enum_<QString::SplitBehavior>("SplitBehavior")
        .value( "KeepEmptyParts", QString::KeepEmptyParts )
        .value( "SkipEmptyParts", QString::SkipEmptyParts )  
    ;
}
