#ifndef SIREERROR_GETMAGIC_H
#define SIREERROR_GETMAGIC_H

#include <QObject>

#include "SireError/exception.h"

#include "registermetatype.hpp"

SIRE_BEGIN_HEADER

namespace SireStream
{

using SireError::exception;

/** This exception is thrown whenever there is an error with the magic number of
    the binary data streaming protocol.

    @author Christopher Woods
*/
class SIRESTREAM_EXPORT magic_error : public SireError::exception
{
public:
    magic_error() : exception()
    {}

    magic_error(QString err, QString place = QString::null)
                  : exception(err,place)
    {}

    magic_error(MagicID wrongid,
                const RegisterMetaTypeBase &info,
                QString place=QString::null)
            : exception(QObject::tr(
                    "Magic error for \"%1\". Got %2, but expected %3.")
                        .arg(info.name()).arg(wrongid).arg(info.magicID()), place)
    {}

    magic_error(MagicID wrongid, MagicID rightid, const char *type_name,
                QString place=QString::null)
            : exception(QObject::tr(
                    "Magic error for \"%1\". Got %2, but expected %3.")
                        .arg(type_name).arg(wrongid).arg(rightid), place)
    {}

    magic_error(const magic_error &other) : exception(other)
    {}

    ~magic_error() throw()
    {}

    const char* what() const throw()
    {
        return "SireStream::magic_error";
    }
};

}

Q_DECLARE_METATYPE(SireError::exception);
Q_DECLARE_METATYPE(SireStream::magic_error);

#endif
