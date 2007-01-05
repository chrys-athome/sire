
#include <boost/python.hpp>

#include "SireMol/property.h"
#include "SireMol/molecule.h"

using namespace boost::python;

namespace SireMol
{

void SIREMOL_EXPORT export_Property()
{

    class_<PropertyBase, boost::noncopyable>("PropertyBase", no_init)

        .def( "what", &PropertyBase::what )
        .def( "isCompatibleWith", &PropertyBase::isCompatibleWith )
        .def( "assertCompatibleWith", &PropertyBase::assertCompatibleWith )
    ;

    register_ptr_to_python< Property >();

    class_<VariantProperty, bases<PropertyBase, QVariant> >( "VariantProperty", init<>() )

        .def( init<const QVariant&>() )
        .def( init<const VariantProperty&>() )
    ;
}

}
