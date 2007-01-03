
#include <boost/python.hpp>

#include "SireFF/ffcomponent.h"

#include "SireFF/ffbase.h"

using namespace boost::python;

using namespace SireCAS;

namespace SireFF
{

void export_FFComponent()
{
    class_<FFComponent, bases<Function> >("FFComponent", init<>())

        .def( init<const FFBase&,
                   optional<const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&>
                  >() )
        .def( init<const FFBase&, const Symbols&>() )

        .def( init<const FFBase&, const QString&,
                   optional<const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&, const Symbol&, const Symbol&,
                            const Symbol&>
                  >() )
        .def( init<const FFBase&, const QString&, const Symbols&>() )

        .def( init<const Function&>() )
        .def( init<const FFComponent&>() )

        .def( "forcefieldName", &FFComponent::forcefieldName )
        .def( "forcefieldComponent", &FFComponent::forcefieldComponent )

        .def( "forcefieldID", &FFComponent::forcefieldID )
    ;

    implicitly_convertible<Function, FFComponent>();
}

}
