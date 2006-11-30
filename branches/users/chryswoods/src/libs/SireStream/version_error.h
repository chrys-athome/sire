#ifndef SIRESTREAM_VERSION_ERROR_H
#define SIRESTREAM_VERSION_ERROR_H

#include "SireError/exception.h"

#include "registermetatype.hpp"
#include "versionid.h"

SIRE_BEGIN_HEADER

namespace SireStream
{

/** This exception is thrown whenever there is an error with the version number of
    the binary data streaming protocol.

    @author Christopher Woods
*/
class SIRESTREAM_EXPORT version_error : public SireError::exception
{
public:
    version_error() : exception()
    {}

    version_error(QString err, QString place = QString::null)
                  : exception(err,place)
    {}

    version_error(VersionID wrongid, QString supported_ids,
                  const RegisterMetaTypeBase &info,
                  QString place=QString::null)
            : exception(QObject::tr(
                    "Incompatible version for \"%1\". Got %2, but can only support [ %3 ].")
                        .arg(info.name()).arg(wrongid).arg(supported_ids), place)
    {}

    version_error(VersionID wrongid, QString supported_ids,
                  const char *type_name, QString place=QString::null)
            : exception(QObject::tr(
                    "Incompatible version for \"%1\". Got %2, but can only support [ %3 ].")
                        .arg(type_name).arg(wrongid).arg(supported_ids), place)
    {}

    version_error(const version_error &other) : exception(other)
    {}

    ~version_error() throw()
    {}

    const char* what() const throw()
    {
        return "SireStream::version_error";
    }
};

}

SIRE_END_HEADER

Q_DECLARE_METATYPE(SireStream::version_error)

#endif
