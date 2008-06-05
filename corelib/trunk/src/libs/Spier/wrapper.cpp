
#include "wrapper.hpp"

#include "SireError/errors.h"

using namespace Spier;

/** Throw a null pointer exception! Define this here to 
    prevent having to have SireError/exceptions.h included
    in wrapper.hpp */
void Spier::throwNullPtrError()
{
    throw SireError::nullptr_error(QObject::tr(
        "Attempting to dereference a null pointer!"), CODELOC);
}
