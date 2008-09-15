
#include <boost/python.hpp>

#include "Helpers/objectregistry.hpp"

using namespace boost::python;

void register_SireStream_functions()
{
    def( "load", (object (*)(const QByteArray&)) &ObjectRegistry::load );
    def( "load", (object (*)(const QString&)) &ObjectRegistry::load );

    def( "save", (QByteArray (*)(const object&)) &ObjectRegistry::save );
    def( "save", (void (*)(const object&, const QString&)) &ObjectRegistry::save );
}
