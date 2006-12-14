
#include "majminversion.h"

#include "SireError/errors.h"
#include "SireStream/version_error.h"

using namespace SireBase;

/** Constructor */
MajMinVersion::MajMinVersion() : IDPair()
{}

/** Constructor - provide the Incremint that is used
    to get the major version number */
MajMinVersion::MajMinVersion(Incremint *majint) : IDPair(majint)
{}

/** Copy constructor */
MajMinVersion::MajMinVersion(const MajMinVersion &other)
              : IDPair(other)
{}

/** Destructor */
MajMinVersion::~MajMinVersion()
{}

/** Return a string representation of this version number */
QString MajMinVersion::toString() const
{
    return QString("%1:%2").arg(major()).arg(minor());
}

/** Internal function */
void MajMinVersion::_pvt_throwMajorError(const MajMinVersion &other) const
{
    throw SireStream::version_error( QObject::tr(
                "Different major version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}

/** Internal function */
void MajMinVersion::_pvt_throwMinorError(const MajMinVersion &other) const
{
    throw SireStream::version_error( QObject::tr(
                "Different minor version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}


/** Internal function */
void MajMinVersion::_pvt_throwVersionError(const MajMinVersion &other) const
{
    throw SireStream::version_error( QObject::tr(
                "Different version numbers (%1 vs. %2)")
                    .arg(toString(), other.toString()), CODELOC );
}
