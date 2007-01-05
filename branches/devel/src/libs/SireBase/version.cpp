
#include "version.h"

#include "SireStream/datastream.h"

using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<Version> r_version;

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const Version &version)
{
    writeHeader(ds, r_version, 1) << version.major() << version.minor();

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, Version &version)
{
    VersionID v = readHeader(ds, r_version);

    if (v == 1)
    {
        ds >> version._major >> version._minor;
    }
    else
        throw version_error(v, "1", r_version, CODELOC);

    return ds;
}

/** Constructor */
Version::Version(quint32 major, quint32 minor)
        : _major(major), _minor(minor)
{}

/** Copy constructor */
Version::Version(const Version &other)
        : _major(other._major), _minor(other._minor)
{}

/** Destructor */
Version::~Version()
{}

/** Assignment operator */
Version& Version::operator=(const Version &other)
{
    _major = other._major;
    _minor = other._minor;

    return *this;
}

/** Return a string version of this version */
QString Version::toString() const
{
    return QString("%1.%2").arg(_major).arg(_minor);
}
