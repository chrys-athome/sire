
#include "idpair.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<IDPair> r_idpair;

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
