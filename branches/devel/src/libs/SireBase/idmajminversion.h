#ifndef SIREBASE_IDMAJMINVERSION_H
#define SIREBASE_IDMAJMINVERSION_H

#include "idtriple.h"
#include "majminversion.h"

#include <QString>

SIRE_BEGIN_HEADER

namespace SireBase
{

/** This class provides an ID and major:minor version number that can be
    incremented in a thread-safe manner (e.g. incrementing will
    result in a new version that is known to be unique - however
    each IDMajMinVersion is not itself thread-safe).

    There are two version numbers;

    (1) Major number - this is global, and is meant to ID the
        object.

    (2) Minor number - this is local to all objects with the
        same major number, and is guaranteed to be unique
        for all versions of that ID. The minor version number
        is reset when the major number is incremented.

    @author Christopher Woods
*/
class SIREBASE_EXPORT IDMajMinVersion : protected IDTriple
{

public:
    IDMajMinVersion();
    IDMajMinVersion(Incremint *idint);

    IDMajMinVersion(const IDMajMinVersion &other);
    IDMajMinVersion(const IDTriple &other);

    ~IDMajMinVersion();

    IDMajMinVersion& operator=(const IDMajMinVersion &other);

    bool operator==(const IDMajMinVersion &other) const;
    bool operator!=(const IDMajMinVersion &other) const;

    QString toString() const;

    quint32 ID() const;

    quint32 major() const;
    quint32 minor() const;

    MajMinVersion version() const;

    void incrementID();

    void incrementMajor();
    void incrementMinor();

    bool sameID(const IDMajMinVersion &other) const;
    bool sameIDAndVersion(const IDMajMinVersion &other) const;
    bool sameIDAndMajorVersion(const IDMajMinVersion &other) const;

    bool sameMajorVersion(const IDMajMinVersion &other) const;
    bool sameMinorVersion(const IDMajMinVersion &other) const;
    bool sameVersion(const IDMajMinVersion &other) const;

    void assertSameID(const IDMajMinVersion &other) const;
    void assertSameIDAndVersion(const IDMajMinVersion &other) const;
    void assertSameIDAndMajorVersion(const IDMajMinVersion &other) const;

    void assertSameMajorVersion(const IDMajMinVersion &other) const;
    void assertSameMinorVersion(const IDMajMinVersion &other) const;
    void assertSameVersion(const IDMajMinVersion &other) const;

private:
    void _pvt_throwIDError(const IDMajMinVersion &other) const;

    void _pvt_throwMajorError(const IDMajMinVersion &other) const;
    void _pvt_throwMinorError(const IDMajMinVersion &other) const;
    void _pvt_throwVersionError(const IDMajMinVersion &other) const;
};

/** Increment the ID number */
inline void IDMajMinVersion::incrementID()
{
    IDTriple::incrementID();
}

/** Increment the minor version number */
inline void IDMajMinVersion::incrementMinor()
{
    IDTriple::incrementMinor();
}

/** Increment the major version number */
inline void IDMajMinVersion::incrementMajor()
{
    IDTriple::incrementMajor();
}

/** Return the ID number */
inline quint32 IDMajMinVersion::ID() const
{
    return IDTriple::ID();
}

/** Return the major number */
inline quint32 IDMajMinVersion::major() const
{
    return IDTriple::major();
}

/** Return the minor number */
inline quint32 IDMajMinVersion::minor() const
{
    return IDTriple::minor();
}

/** Return the version */
inline MajMinVersion IDMajMinVersion::version() const
{
    return MajMinVersion(IDTriple::version());
}

/** Return whether or not this has the same ID as 'other' */
inline bool IDMajMinVersion::sameID(const IDMajMinVersion &other) const
{
    return IDTriple::sameID(other);
}

/** Return whether or not this has the same ID and version as 'other' */
inline bool IDMajMinVersion::sameIDAndVersion(const IDMajMinVersion &other) const
{
    return IDTriple::sameIDAndVersion(other);
}

/** Return whether or not this has the same ID and major version as 'other' */
inline bool IDMajMinVersion::sameIDAndMajorVersion(const IDMajMinVersion &other) const
{
    return IDTriple::sameIDAndMajorVersion(other);
}

/** Return whether or not this has the same major version as 'other' */
inline bool IDMajMinVersion::sameMajorVersion(const IDMajMinVersion &other) const
{
    return IDTriple::sameMajorVersion(other);
}

/** Return whether or not this has the same minor version as 'other' */
inline bool IDMajMinVersion::sameMinorVersion(const IDMajMinVersion &other) const
{
    return IDTriple::sameMinorVersion(other);
}

/** Return whether or not this has the same version as 'other' */
inline bool IDMajMinVersion::sameVersion(const IDMajMinVersion &other) const
{
    return IDTriple::sameVersion(other);
}

/** Comparison operator */
inline bool IDMajMinVersion::operator==(const IDMajMinVersion &other) const
{
    return IDTriple::operator==(other);
}

/** Comparison operator */
inline bool IDMajMinVersion::operator!=(const IDMajMinVersion &other) const
{
    return IDTriple::operator!=(other);
}

/** Assert that this has the same ID number as 'other'

    \throw SireError::id_error
*/
inline void IDMajMinVersion::assertSameID(const IDMajMinVersion &other) const
{
    if (not sameID(other))
        this->_pvt_throwIDError(other);
}

/** Assert that this has the same major version as 'other'

    \throw SireError::version_error
*/
inline void IDMajMinVersion::assertSameMajorVersion(const IDMajMinVersion &other) const
{
    if (not sameMajorVersion(other))
        this->_pvt_throwMajorError(other);
}

/** Assert that this has the same minor version as 'other'

    \throw SireError::version_error
*/
inline void IDMajMinVersion::assertSameMinorVersion(const IDMajMinVersion &other) const
{
    if (not sameMinorVersion(other))
        this->_pvt_throwMinorError(other);
}

/** Assert that this is the same version as 'other'

    \throw SireError::version_error
*/
inline void IDMajMinVersion::assertSameVersion(const IDMajMinVersion &other) const
{
    if (not sameVersion(other))
        this->_pvt_throwVersionError(other);
}

/** Assert that this has the same ID and version as 'other'

    \throw SireError::id_error
    \throw SireError::version_error
*/
inline void IDMajMinVersion::assertSameIDAndVersion(const IDMajMinVersion &other) const
{
    this->assertSameID(other);
    this->assertSameVersion(other);
}

/** Assert that this has the same ID and major version number as 'other'

    \throw SireError::id_error
    \throw SireError::version_error
*/
inline void IDMajMinVersion::assertSameIDAndMajorVersion(const IDMajMinVersion &other) const
{
    this->assertSameID(other);
    this->assertSameMajorVersion(other);
}

}

SIRE_END_HEADER

#endif
