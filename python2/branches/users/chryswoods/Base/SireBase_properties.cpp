#include "Siren/convertobjptr.hpp"

#include "SireBase_properties.h"

#include "Siren/stream.h"
#include "stringmangler.h"
#include <QMutex>
#include "stringmangler.h"
void register_SireBase_properties()
{
    register_objptr< SireBase::StringManglerPtr >();
}
