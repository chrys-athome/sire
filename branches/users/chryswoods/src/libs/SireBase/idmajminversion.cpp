
#include "idmajminversion.h"

#include "SireError/errors.h"

using namespace SireBase;

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
