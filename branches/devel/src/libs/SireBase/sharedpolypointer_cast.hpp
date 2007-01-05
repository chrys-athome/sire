#ifndef SIREBASE_SHAREDPOLYPOINTERCAST_HPP

#include "sharedpolypointer.hpp"

#include "SireError/errors.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

/** Cast the pointer 'ptr' from type 'T' to an object of type 'S'

    \throw SireError::nullptr_error
    \throw SireError::invalid_cast
*/
template<class S, class T>
const S& sharedpolypointer_cast(const SharedPolyPointer<T> &ptr)
{
    if (not ptr)
        throw SireError::nullptr_error( QObject::tr("Cannot cast a null pointer!"),
                                        CODELOC );

    const S *sptr = dynamic_cast<const S*>(ptr.constData());

    if (not sptr)
        throw SireError::invalid_cast( QObject::tr(
                      "Cannot cast a SharedPolyPointer of type \"%1\" to "
                      "type \"%2\".")
                          .arg( SharedPolyPointerHelper<T>::what(*ptr),
                                SharedPolyPointerHelper<S>::typeName() ), CODELOC );

    return *sptr;
}

}

SIRE_END_HEADER

#endif
