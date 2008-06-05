
#include <Python.h>
#include <boost/python.hpp>

#include <QStringList>
#include <QString>

#include "SireQt/qlist.hpp"

using namespace boost::python;

using namespace SireQt;
using namespace SirePy;
    
bool wrap_contains2(const QStringList &slist, const QString &str, Qt::CaseSensitivity cs)
{
    return slist.contains(str,cs);
}
    
bool wrap_contains1(const QStringList &slist, const QString &str)
{
    return slist.contains(str);
}

QStringList wrap_filter2(const QStringList &slist, const QString &str, 
                         Qt::CaseSensitivity cs)
{
    return slist.filter(str,cs);
}

QStringList wrap_filter1(const QStringList &slist, const QString &str)
{
    return slist.filter(str);
}

int wrap_indexOf2(const QStringList &slist, const QRegExp &rx, int from)
{
    return slist.indexOf(rx,from);
}

int wrap_indexOf1(const QStringList &slist, const QRegExp &rx)
{
    return slist.indexOf(rx);
}

int wrap_lastIndexOf2(const QStringList &slist, const QRegExp &rx, int from)
{
    return slist.lastIndexOf(rx,from);
}

int wrap_lastIndexOf1(const QStringList &slist, const QRegExp &rx)
{
    return slist.lastIndexOf(rx);
}

QStringList& wrap_replaceInStrings3(QStringList &slist, const QString &before, 
                                    const QString &after, Qt::CaseSensitivity cs)
{
    return slist.replaceInStrings(before,after,cs);
}

QStringList& wrap_replaceInStrings2(QStringList &slist, const QString &before, 
                                    const QString &after)
{
    return slist.replaceInStrings(before,after);
}

void export_QStringList()
{                            
    class_<QStringList, bases< QList<QString> > >("QStringList", init<>())
        .def( init<const QString&>() )
        .def( init<const QStringList&>() )
        .def( init<const QList<QString>&>() )
        .def( "contains", &wrap_contains2 )
        .def( "contains", &wrap_contains1 )
        .def( "filter", &wrap_filter2 )
        .def( "filter", &wrap_filter1 )
        .def( "filter", (QStringList (QStringList::*)(const QRegExp&) const)
                          &QStringList::filter )
        .def( "indexOf", &wrap_indexOf2 )
        .def( "indexOf", &wrap_indexOf1 )
        .def( "join", &QStringList::join )
        .def( "lastIndexOf", &wrap_lastIndexOf2 )
        .def( "lastIndexOf", &wrap_lastIndexOf1 )
        
        .def( "replaceInStrings", &wrap_replaceInStrings3,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "replaceInStrings", &wrap_replaceInStrings2,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "replaceInStrings", (QStringList& (QStringList::*)(const QRegExp&, 
                                                                 const QString&))
                                          &QStringList::replaceInStrings,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "sort", &QStringList::sort )
        
        .def( self + self )
    
        .def( "__lshift__", &__lshift__<QStringList,QString> )
        .def( "__lshift__", &__lshift__<QStringList,QStringList> )
        
        .def( "__rlshift__", &__rlshift__QDataStream<QStringList>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<QStringList>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;

}
