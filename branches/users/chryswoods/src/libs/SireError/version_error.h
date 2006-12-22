#ifndef SIREERROR_VERSION_ERROR_H
#define SIREERROR_VERSION_ERROR_H

#include "SireError/exception.h"

SIRE_BEGIN_HEADER

namespace SireError
{

/** This exception is thrown whenever there is an error with a
    version number

    @author Christopher Woods
*/
class SIREERROR_EXPORT version_error : public exception
{
public:
    version_error() : exception()
    {}

    version_error(QString err, QString place = QString::null)
                  : exception(err,place)
    {}

    version_error(const version_error &other) : exception(other)
    {}

    ~version_error() throw()
    {}

    const char* what() const throw()
    {
        return "SireError::version_error";
    }
};

}

Q_DECLARE_METATYPE(SireError::version_error)

SIRE_END_HEADER

#endif
