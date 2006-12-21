#ifndef SIREBASE_IDPAIR_H
#define SIREBASE_IDPAIR_H

#include <boost/shared_ptr.hpp>
#include <QString>

#ifdef major
//glibc #defines major == gnu_dev_major
//This ruins any use of 'major' - this is why macros are BAD!!!
//It does this in sys/sysmacros.h
#undef major
#endif

#ifdef minor
//glibc does the same thing with 'minor' as well...
// #define minor == gnu_dev_minor
#undef minor
#endif

#include "incremint.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
class IDPair;
class Version;
}

QDataStream& operator<<(QDataStream&, const SireBase::IDPair&);
QDataStream& operator>>(QDataStream&, SireBase::IDPair&);

QDataStream& operator<<(QDataStream&, const SireBase::Version&);
QDataStream& operator>>(QDataStream&, SireBase::Version&);

namespace SireBase
{

class IDTriple;

class Version
{

friend QDataStream& ::operator<<(QDataStream&, const Version&);
friend QDataStream& ::operator>>(QDataStream&, Version&);

public:
    Version(quint32 major=0, quint32 minor=0);
    Version(const Version &other);

    ~Version();

    Version& operator=(const Version &other);

    bool operator==(const Version &other) const
    {
        return _major == other._major and _minor == other._minor;
    }

    bool operator!=(const Version &other) const
    {
        return _major != other._major or _minor != other._minor;
    }

    bool sameMajorVersion(const Version &other) const
    {
        return _major == other._major;
    }

    bool sameMinorVersion(const Version &other) const
    {
        return _minor == other._minor;
    }

    quint32 major() const
    {
        return _major;
    }

    quint32 minor() const
    {
        return _minor;
    }

    void setMajor(quint32 major)
    {
        _major = major;
    }

    void setMinor(quint32 minor)
    {
        _minor = minor;
    }

    QString toString() const;

private:
    quint32 _major;
    quint32 _minor;
};

/** This class provides a two part ID number that can be
    incremented in a thread-safe manner (e.g. incrementing will
    result in a new version that is known to be unique - however
    each ID number is not itself thread-safe).

    There are two version numbers;

    (1) Major number - this is global, and is meant to ID the
        object.

    (2) Minor number - this is local to all objects with the
        same major number, and is guaranteed to be unique
        for all versions of that ID. The minor version number
        is reset when the major number is incremented.

    This class is not designed to be used on its own, but
    rather to be derived to an over-ridden class (e.g.
    MajMinVersion or IDVersion, or IDMajMinVersion)

    @author Christopher Woods
*/
class SIREBASE_EXPORT IDPair
{

friend class IDTriple;

friend QDataStream& ::operator<<(QDataStream&, const IDPair&);
friend QDataStream& ::operator>>(QDataStream&, IDPair&);

private:
    static Incremint shared_pair_increment;

public:
    IDPair(Incremint *majint = &IDPair::shared_pair_increment);

    IDPair(const IDPair &other);

    IDPair& operator=(const IDPair &other);

    ~IDPair();

    bool operator==(const IDPair &other) const;
    bool operator!=(const IDPair &other) const;

protected:
    quint32 major() const;
    quint32 minor() const;

    const Version& version() const;

    void incrementMajor();
    void incrementMinor();

    bool sameMajorVersion(const IDPair &other) const;
    bool sameMinorVersion(const IDPair &other) const;
    bool sameVersion(const IDPair &other) const;

private:
    /** Shared pointer to the Incremint used to increment
        the minor version number */
    boost::shared_ptr<Incremint> minor_incremint;

    /** Pointer to the Incremint that is used to get the
        major version number */
    Incremint *major_incremint;

    /** The version number */
    Version _version;
};

/** Increment the minor version number */
inline void IDPair::incrementMinor()
{
    _version.setMinor( minor_incremint->increment() );
}

/** Increment the major version number */
inline void IDPair::incrementMajor()
{
    _version.setMajor( major_incremint->increment() );
    _version.setMinor( 0 );

    minor_incremint.reset( new Incremint(0) );
}

/** Return the major number */
inline quint32 IDPair::major() const
{
    return _version.major();
}

/** Return the minor number */
inline quint32 IDPair::minor() const
{
    return _version.minor();
}

/** Return the version numbers */
inline const Version& IDPair::version() const
{
    return _version;
}

/** Return whether or not this has the same major version as 'other' */
inline bool IDPair::sameMajorVersion(const IDPair &other) const
{
    return _version.sameMajorVersion(other._version);
}

/** Return whether or not this has the same minor version as 'other' */
inline bool IDPair::sameMinorVersion(const IDPair &other) const
{
    return _version.sameMinorVersion(other._version);
}

/** Return whether or not this has the same version as 'other' */
inline bool IDPair::sameVersion(const IDPair &other) const
{
    return _version == other._version;
}

/** Comparison operator */
inline bool IDPair::operator==(const IDPair &other) const
{
    return _version == other._version
            and major_incremint == other.major_incremint
            and minor_incremint == other.minor_incremint;
}

/** Comparison operator */
inline bool IDPair::operator!=(const IDPair &other) const
{
    return _version != other._version
                or major_incremint != other.major_incremint
                or minor_incremint != other.minor_incremint;
}

}

Q_DECLARE_METATYPE(SireBase::IDPair);
Q_DECLARE_METATYPE(SireBase::Version);

#endif
