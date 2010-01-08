#include "Siren/convertobjptr.hpp"

#include "Siren_properties.h"

#include "Siren/errors.h"
#include "class.h"
#include "datastream.h"
#include "objptr.hpp"
#include "xmlstream.h"
#include "objptr.hpp"
void register_Siren_properties()
{
    register_objptr< Siren::ObjectPtr >();
}
