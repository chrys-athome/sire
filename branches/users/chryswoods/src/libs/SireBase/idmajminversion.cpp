
#include "idmajminversion.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<IDMajMinVersion> r_idmajmin("SireBase::IDMajMinVersion");

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const IDMajMinVersion &idmajmin)
{
    writeHeader(ds, r_idmajmin, 1)
                << static_cast<const IDTriple&>(idmajmin);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, IDMajMinVersion &idmajmin)
{
    VersionID v = readHeader(ds, r_idmajmin);

    if (v == 1)
    {
        ds >> static_cast<IDTriple&>(idmajmin);
    }
    else
        throw version_error(v, "1", r_idmajmin, CODELOC);

    return ds;
}

/** Constructor */
IDMajMinVersion::IDMajMinVersion() : IDTriple()
{}

/** Constructor - provide the Incremint that is used
    to get the major version number */
IDMajMinVersion::IDMajMinVersion(Incremint *majint) : IDTriple(majint)
{}

/** Copy constructor */
IDMajMinVersion::IDMajMinVersion(const IDTriple &other)
                : IDTriple(other)
{}

/** Copy constructor */
IDMajMinVersion::IDMajMinVersion(const IDMajMinVersion &other)
                : IDTriple(other)
{}

/** Destructor */
IDMajMinVersion::~IDMajMinVersion()
{}

/** Return a string representation of this version number */
QString IDMajMinVersion::toString() const
{
    return QString("%1:%2").arg(ID()).arg(version().toString());
}

/** Internal function */
void IDMajMinVersion::_pvt_throwIDError(const IDMajMinVersion &other) const
{
    throw SireError::id_error( QObject::tr(
                "Different ID numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}

/** Internal function */
void IDMajMinVersion::_pvt_throwMajorError(const IDMajMinVersion &other) const
{
    throw SireError::version_error( QObject::tr(
                "Different major version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}

/** Internal function */
void IDMajMinVersion::_pvt_throwMinorError(const IDMajMinVersion &other) const
{
    throw SireError::version_error( QObject::tr(
                "Different minor version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}


/** Internal function */
void IDMajMinVersion::_pvt_throwVersionError(const IDMajMinVersion &other) const
{
    throw SireError::version_error( QObject::tr(
                "Different version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}
