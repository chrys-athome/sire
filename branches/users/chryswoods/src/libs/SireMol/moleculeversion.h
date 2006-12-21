#ifndef SIREMOL_MOLECULEVERSION_H
#define SIREMOL_MOLECULEVERSION_H

#include "sireglobal.h"

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

namespace SireMol
{
class MoleculeVersion;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeVersion&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeVersion&);

namespace SireMol
{

/**
This class holds the version number of the molecule. The version number is used to that either the coordinates of type of the molecule has changed, and can thus be used as an internal check by the code to verify that the things that it thinks have changed are indeed what the molecule itself thinks have changed.

@author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeVersion
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeVersion&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeVersion&);

public:
    MoleculeVersion();
    MoleculeVersion(quint32 major, quint32 minor);

    MoleculeVersion(const MoleculeVersion &other);

    ~MoleculeVersion();

    bool operator==(const MoleculeVersion &other) const;
    bool operator!=(const MoleculeVersion &other) const;

    MoleculeVersion& operator=(const MoleculeVersion &other);

    QString toString() const;

    quint32 major() const;
    quint32 minor() const;

    void incrementMajor();
    void incrementMinor();

    bool sameVersion(const MoleculeVersion &other) const;
    bool minorDifference(const MoleculeVersion &other) const;
    bool majorDifference(const MoleculeVersion &other) const;

private:
    /** The major version number */
    quint32 majversion;
    /** The minor version number */
    quint32 minversion;
};

/** Return the major version number */
inline quint32 MoleculeVersion::major() const
{
    return majversion;
}

/** Return the minor version number */
inline quint32 MoleculeVersion::minor() const
{
    return minversion;
}

/** Increment the major version number - this will reset the minor
    version number to 0 */
inline void MoleculeVersion::incrementMajor()
{
    ++majversion;
    minversion = 0;
}

/** Increment the minor version number */
inline void MoleculeVersion::incrementMinor()
{
    ++minversion;
}

/** Comparison operator */
inline bool MoleculeVersion::operator==(const MoleculeVersion &other) const
{
    return majversion == other.majversion and
           minversion == other.minversion;
}

/** Comparison operator */
inline bool MoleculeVersion::operator!=(const MoleculeVersion &other) const
{
    return majversion != other.majversion or
           minversion != other.minversion;
}

/** Return whether this is the same version as 'other' */
inline bool MoleculeVersion::sameVersion(const MoleculeVersion &other) const
{
    return majversion == other.majversion and
           minversion == other.minversion;
}

/** Return whether there are only minor differences between this version
    and 'other' (e.g. minor versions only are different) */
inline bool MoleculeVersion::minorDifference(const MoleculeVersion &other) const
{
    return majversion == other.majversion and
           minversion != other.minversion;
}

/** Return whether or not there is a major difference between versions */
inline bool MoleculeVersion::majorDifference(const MoleculeVersion &other) const
{
    return majversion != other.majversion;
}

}

Q_DECLARE_METATYPE(SireMol::MoleculeVersion)
Q_DECLARE_TYPEINFO(SireMol::MoleculeVersion, Q_PRIMITIVE_TYPE);

#endif
