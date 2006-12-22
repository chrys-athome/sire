#ifndef SIREBASE_VERSION_H
#define SIREBASE_VERSION_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
class Version;
}

QDataStream& operator<<(QDataStream&, const SireBase::Version&);
QDataStream& operator>>(QDataStream&, SireBase::Version&);

namespace SireBase
{

/** This class provides a simple version number that consists
    of a major and minor number.

    @author Christopher Woods
*/
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

}

Q_DECLARE_METATYPE(SireBase::Version);

SIRE_END_HEADER

#endif
