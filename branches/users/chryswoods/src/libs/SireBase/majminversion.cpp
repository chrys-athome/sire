
#include "majminversion.h"

#include "SireError/errors.h"
#include "SireStream/version_error.h"

using namespace SireBase;

/** Private static incremint that is used when no 
    other one is supplied by the user */
Incremint MajMinVersion::shared_increment;

/** Constructor - optionally provide the Incremint that is used
    to get the major version number */
MajMinVersion::MajMinVersion(Incremint *majint)
              : major_incremint(majint)
{
    if (not majint)
        throw SireError::program_bug( QObject::tr(
                  "You *must* supply a valid Incremint to the "
                  "MajMinVersion constructor!"), CODELOC );

    this->incrementMajor();
}

/** Copy constructor */
MajMinVersion::MajMinVersion(const MajMinVersion &other)
              : minor_incremint(other.minor_incremint),
                major_incremint(other.major_incremint),
                majnum(other.majnum), minnum(other.minnum)
{}

/** Destructor */
MajMinVersion::~MajMinVersion()
{}

/** Return a string representation of this version number */
QString MajMinVersion::toString() const
{
    return QString("%1:%2").arg(majnum).arg(minnum);
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
