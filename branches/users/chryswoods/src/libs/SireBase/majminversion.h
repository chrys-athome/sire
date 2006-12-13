#ifndef SIREBASE_MAJMINVERSION_H
#define SIREBASE_MAJMINVERSION_H

#include <boost/shared_ptr.hpp>
#include <QString>

#include "incremint.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

/** This class provides a major:minor version number that can be 
    incremented in a thread-safe manner (e.g. incrementing will
    result in a new version that is known to be unique - however
    each MajMinVersion is not itself thread-safe). 
    
    There are two version numbers;
    
    (1) Major number - this is global, and is meant to ID the 
        object.
        
    (2) Minor number - this is local to all objects with the 
        same major number, and is guaranteed to be unique
        for all versions of that ID. The minor version number
        is reset when the major number is incremented. 
        
    @author Christopher Woods
*/
class SIREBASE_EXPORT MajMinVersion
{

private:
    static Incremint shared_increment;

public:
    MajMinVersion(Incremint *majint = &MajMinVersion::shared_increment);
    
    MajMinVersion(const MajMinVersion &other);
    
    ~MajMinVersion();
    
    MajMinVersion& operator=(const MajMinVersion &other);
    
    bool operator==(const MajMinVersion &other) const;
    bool operator!=(const MajMinVersion &other) const;
    
    QString toString() const;
    
    quint32 major() const;
    quint32 minor() const;
    
    void incrementMajor();
    void incrementMinor();
    
    bool sameMajorVersion(const MajMinVersion &other) const;
    bool sameMinorVersion(const MajMinVersion &other) const;
    bool sameVersion(const MajMinVersion &other) const;
    
    void assertSameMajorVersion(const MajMinVersion &other) const;
    void assertSameMinorVersion(const MajMinVersion &other) const;
    void assertSameVersion(const MajMinVersion &other) const;
    
private:
    void _pvt_throwMajorError(const MajMinVersion &other) const;
    void _pvt_throwMinorError(const MajMinVersion &other) const;
    void _pvt_throwVersionError(const MajMinVersion &other) const;

    /** Shared pointer to the Incremint used to increment 
        the minor version number */
    boost::shared_ptr<Incremint> minor_incremint;
    
    /** Pointer to the Incremint that is used to get the 
        major version number */
    Incremint *major_incremint;
    
    /** The major number */
    quint32 majnum;
    
    /** The minor number */
    quint32 minnum;
};

/** Increment the minor version number */
inline void MajMinVersion::incrementMinor()
{
    minnum = minor_incremint->increment();
}

/** Increment the major version number */
inline void MajMinVersion::incrementMajor()
{
    majnum = major_incremint->increment();
    minor_incremint.reset( new Incremint(0) );
    minnum = 0;
}

/** Return the major number */
inline quint32 MajMinVersion::major() const
{
    return majnum;
}

/** Return the minor number */
inline quint32 MajMinVersion::minor() const
{
    return minnum;
}

/** Return whether or not this has the same major version as 'other' */
inline bool MajMinVersion::sameMajorVersion(const MajMinVersion &other) const
{
    return majnum == other.majnum;
}

/** Return whether or not this has the same minor version as 'other' */
inline bool MajMinVersion::sameMinorVersion(const MajMinVersion &other) const
{
    return minnum == other.minnum;
}
    
/** Return whether or not this has the same version as 'other' */
inline bool MajMinVersion::sameVersion(const MajMinVersion &other) const
{
    return majnum == other.majnum and minnum == other.minnum;
}
    
/** Comparison operator */
inline bool MajMinVersion::operator==(const MajMinVersion &other) const
{
    return majnum == other.majnum and minnum == other.minnum 
                and major_incremint == other.major_incremint 
                and minor_incremint == other.minor_incremint;
}

/** Comparison operator */
inline bool MajMinVersion::operator!=(const MajMinVersion &other) const
{
    return majnum != other.majnum or minnum != other.minnum
                or major_incremint != other.major_incremint
                or minor_incremint != other.minor_incremint;
}

/** Assert that this has the same major version as 'other' 
  
    \throw SireStream::version_error
*/
inline void MajMinVersion::assertSameMajorVersion(const MajMinVersion &other) const
{
    if (majnum != other.majnum)
        this->_pvt_throwMajorError(other);
}

/** Assert that this has the same minor version as 'other' 

    \throw SireStream::version_error
*/
inline void MajMinVersion::assertSameMinorVersion(const MajMinVersion &other) const
{
    if (minnum != other.minnum)
        this->_pvt_throwMinorError(other);
}

/** Assert that this is the same version as 'other' 

    \throw SireStream::version_error
*/
inline void MajMinVersion::assertSameVersion(const MajMinVersion &other) const
{
    if (majnum != other.majnum or minnum != other.minnum)
        this->_pvt_throwVersionError(other);
}

}

SIRE_END_HEADER

#endif
