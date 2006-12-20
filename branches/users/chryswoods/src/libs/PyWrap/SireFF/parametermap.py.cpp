
#include <boost/python.hpp>

#include "SireFF/parametermap.h"

#include "SireQt/qlist.hpp"

using namespace boost::python;

using namespace SireQt;

namespace SireFF
{

void SIREFF_EXPORT export_ParameterMap()
{
    class_<ParameterName>( "ParameterName", init<>() )
        .def( init<const QString&>() )
        .def( init<const QString&,const QString&>() )
        .def( init<const ParameterName&>() )

        .def( self == other<QString>() )

        .def( "isValid", &ParameterName::isValid )
        .def( "name", &ParameterName::name,
                          return_value_policy<copy_const_reference>() )

        .def( "defaultSource", &ParameterName::defaultSource,
                          return_value_policy<copy_const_reference>() )
    ;

    class_<ParameterSource, bases<ParameterName> >( "ParameterSource", init<>() )

        .def( init<const ParameterSource&>() )

        .def( "isDefault", &ParameterSource::isDefault )
        .def( "source", &ParameterSource::source,
                          return_value_policy<copy_const_reference>() )
    ;

    wrap_QList<ParameterSource>("QList_ParameterSource_");

    class_<ParameterMap>( "ParameterMap", init<>() )

        .def( init<const ParameterSource&>() )
        .def( init<const QList<ParameterSource>&>() )

        .def( init<const ParameterMap&>() )

        .def( "__and__", &ParameterMap::operator&& )

        .def( "source", &ParameterMap::source )
    ;

    implicitly_convertible<ParameterSource, ParameterMap>();
    implicitly_convertible<QList<ParameterSource>, ParameterMap>();
}

}
