
#include "idversion.h"

#include "SireError/errors.h"

using namespace SireBase;

/** Constructor */
IDVersion::IDVersion() : IDPair()
{}

/** Constructor - provide the Incremint that is used
    to get the major version number */
IDVersion::IDVersion(Incremint *majint) : IDPair(majint)
{}

/** Copy constructor */
IDVersion::IDVersion(const IDVersion &other)
              : IDPair(other)
{}

/** Destructor */
IDVersion::~IDVersion()
{}

/** Return a string representation of this version number */
QString IDVersion::toString() const
{
    return QString("%1.%2").arg(major()).arg(minor());
}

/** Internal function */
void IDVersion::_pvt_throwIDError(const IDVersion &other) const
{
    throw SireError::id_error( QObject::tr(
                "Different ID numbers (%1 vs. %2)")
                    .arg(major()).arg(other.major()), CODELOC );
}

/** Internal function */
void IDVersion::_pvt_throwVersionError(const IDVersion &other) const
{
    throw SireError::version_error( QObject::tr(
                "Different version numbers (%1 vs. %2) for objects withs IDs %3 and %4")
                    .arg(minor()).arg(other.minor())
                    .arg(major()).arg(other.major()), CODELOC );
}
