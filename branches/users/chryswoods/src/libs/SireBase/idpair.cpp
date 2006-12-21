
#include "idpair.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireBase;
using namespace SireStream;

//////////
////////// Implementation of Version
//////////

static const RegisterMetaType<Version> r_version("SireBase::Version");

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

//////////
////////// Implementation of IDPair
//////////

static const RegisterMetaType<IDPair> r_idpair("SireBase::IDPair");

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const IDPair &idpair)
{
    writeHeader(ds, r_idpair, 1) << idpair._version;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, IDPair &idpair)
{
    VersionID v = readHeader(ds, r_idpair);

    if (v == 1)
    {
        ds >> idpair._version;
    }
    else
        throw version_error(v, "1", r_idpair, CODELOC);

    return ds;
}

/** Private static incremint that is used when no
    other one is supplied by the user */
Incremint IDPair::shared_pair_increment;

/** Constructor - optionally provide the Incremint that is used
    to get the major version number */
IDPair::IDPair(Incremint *majint)
       : major_incremint(majint)
{
    if (not majint)
        throw SireError::program_bug( QObject::tr(
                  "You *must* supply a valid Incremint to the "
                  "IDPair constructor!"), CODELOC );

    this->incrementMajor();
}

/** Copy constructor */
IDPair::IDPair(const IDPair &other)
              : minor_incremint(other.minor_incremint),
                major_incremint(other.major_incremint),
                _version(other._version)
{}

/** Destructor */
IDPair::~IDPair()
{}

/** Assignment operator */
IDPair& IDPair::operator=(const IDPair &other)
{
    minor_incremint = other.minor_incremint;
    major_incremint = other.major_incremint;
    _version = other._version;

    return *this;
}
