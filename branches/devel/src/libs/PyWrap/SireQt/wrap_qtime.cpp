
#include <Python.h>
#include <boost/python.hpp>

#include <QTime>
#include <QString>

#include "SirePy/str.hpp"

#include "qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;
using namespace SirePy;

bool wrap_setHMS4(QTime &qtime, int h, int m, int s, int ms)
{
    return qtime.setHMS(h,m,s,ms);
}

bool wrap_setHMS3(QTime &qtime, int h, int m, int s)
{
    return qtime.setHMS(h,m,s);
}

QString wrap_toString1(const QTime &qtime, Qt::DateFormat f)
{
    return qtime.toString(f);
}

QString wrap_toString0(const QTime &qtime)
{
    return qtime.toString();
}

QTime wrap_fromString2(const QString &string, Qt::DateFormat format)
{
    return QTime::fromString(string,format);
}

QTime wrap_fromString1(const QString &string)
{
    return QTime::fromString(string);
}

bool wrap_isValid4(int h, int m, int s, int ms)
{
    return QTime::isValid(h,m,s,ms);
}

bool wrap_isValid3(int h, int m, int s)
{
    return QTime::isValid(h,m,s);
}

void  
SIREQT_EXPORT
export_QTime()
{
    class_<QTime>("QTime", init<>() )
        .def(init<int,int,optional<int,int> >() )
        
        .def( "__rrshift__", &__rrshift__QDataStream<QTime>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<QTime>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "addMSecs", &QTime::addMSecs )
        .def( "addSecs",  &QTime::addSecs )
        .def( "elapsed", &QTime::elapsed )
        .def( "hour", &QTime::hour )
        .def( "isNull", &QTime::isNull )
        .def( "isValid", (bool (QTime::*)() const)&QTime::isValid )
        .def( "minute", &QTime::minute )
        .def( "msec", &QTime::msec )
        .def( "msecsTo", &QTime::msecsTo )
        .def( "restart", &QTime::restart )
        .def( "second", &QTime::second )
        .def( "secsTo", &QTime::secsTo )
        .def( "setHMS", &wrap_setHMS4 )
        .def( "setHMS", &wrap_setHMS3 )
        .def( "start", &QTime::start )
        .def( "toString", (QString (QTime::*)(const QString&) const)
                              &QTime::toString )
        .def( "toString", &wrap_toString1 )
        .def( "toString", &wrap_toString0 )
        
        .def( "currentTime", &QTime::currentTime ).staticmethod("currentTime")
        
        .def( "fromString", &wrap_fromString2 )
        .def( "fromString", &wrap_fromString1 )
        .def( "fromString", (QTime (*)(const QString&, const QString&))
                                &QTime::fromString ).staticmethod("fromString")
        
        .def( "isValid", &wrap_isValid4 )
        .def( "isValid", &wrap_isValid3 ).staticmethod("isValid")
        
        .def( self == self )
        .def( self != self )
        .def( self < self )
        .def( self <= self )
        .def( self > self )
        .def( self >= self )
        
        .def( "__str__", &__str__<QTime> )
    ;

}
