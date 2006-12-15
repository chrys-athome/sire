#ifndef SIREBASE_IDVERSION_H
#define SIREBASE_IDVERSION_H

#include "idpair.h"

#include <QString>

SIRE_BEGIN_HEADER

namespace SireBase
{

/** This class provides a ID:version number that can be
    incremented in a thread-safe manner (e.g. incrementing will
    result in a new version that is known to be unique - however
    each IDVersion is not itself thread-safe).

    There are two version numbers;

    (1) Major number - this is global, and is meant to ID the
        object.

    (2) Minor number - this is local to all objects with the
        same ID, and is guaranteed to be unique
        for all versions of that ID. The version number
        is reset when the ID number is incremented.

    @author Christopher Woods
*/
class SIREBASE_EXPORT IDVersion : protected IDPair
{

public:
    IDVersion();
    IDVersion(Incremint *majint);

    IDVersion(const IDVersion &other);

    ~IDVersion();

    IDVersion& operator=(const IDVersion &other);

    bool operator==(const IDVersion &other) const;
    bool operator!=(const IDVersion &other) const;

    QString toString() const;

    quint32 ID() const;
    quint32 version() const;

    void incrementID();
    void incrementVersion();

    bool sameID(const IDVersion &other) const;
    bool sameVersion(const IDVersion &other) const;
    bool sameIDAndVersion(const IDVersion &other) const;

    void assertSameID(const IDVersion &other) const;
    void assertSameVersion(const IDVersion &other) const;
    void assertSameIDAndVersion(const IDVersion &other) const;

private:
    void _pvt_throwIDError(const IDVersion &other) const;
    void _pvt_throwVersionError(const IDVersion &other) const;
};

/** Increment the version number */
inline void IDVersion::incrementVersion()
{
    IDPair::incrementMinor();
}

/** Increment the ID number */
inline void IDVersion::incrementID()
{
    IDPair::incrementMajor();
}

/** Return the ID */
inline quint32 IDVersion::ID() const
{
    return IDPair::major();
}

/** Return the version */
inline quint32 IDVersion::version() const
{
    return IDPair::minor();
}

/** Return whether or not this has the same ID as 'other' */
inline bool IDVersion::sameID(const IDVersion &other) const
{
    return IDPair::sameMajorVersion(other);
}

/** Return whether or not this has the same version as 'other' */
inline bool IDVersion::sameVersion(const IDVersion &other) const
{
    return IDPair::sameMinorVersion(other);
}

/** Return whether or not this has the same ID and version as 'other' */
inline bool IDVersion::sameIDAndVersion(const IDVersion &other) const
{
    return IDPair::sameVersion(other);
}

/** Comparison operator */
inline bool IDVersion::operator==(const IDVersion &other) const
{
    return IDPair::operator==(other);
}

/** Comparison operator */
inline bool IDVersion::operator!=(const IDVersion &other) const
{
    return IDPair::operator!=(other);
}

/** Assert that this has the same major version as 'other'

    \throw SireError::id_error
*/
inline void IDVersion::assertSameID(const IDVersion &other) const
{
    if (not sameMajorVersion(other))
        this->_pvt_throwIDError(other);
}

/** Assert that this has the same version as 'other'

    \throw SireError::version_error
*/
inline void IDVersion::assertSameVersion(const IDVersion &other) const
{
    if (not sameMinorVersion(other))
        this->_pvt_throwVersionError(other);
}

/** Assert that this is the ID and same version as 'other'

    \throw SireError::id_error
    \throw SireError::version_error
*/
inline void IDVersion::assertSameIDAndVersion(const IDVersion &other) const
{
    assertSameID(other);
    assertSameVersion(other);
}

}

SIRE_END_HEADER

#endif
