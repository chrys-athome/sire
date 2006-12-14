#ifndef SIREBASE_IDPAIR_H
#define SIREBASE_IDPAIR_H

#include <boost/shared_ptr.hpp>
#include <QString>

#include "incremint.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

class IDTriple;

/** This class provides a two part ID number that can be 
    incremented in a thread-safe manner (e.g. incrementing will
    result in a new version that is known to be unique - however
    each ID number is not itself thread-safe). 
    
    There are two version numbers;
    
    (1) Major number - this is global, and is meant to ID the 
        object.
        
    (2) Minor number - this is local to all objects with the 
        same major number, and is guaranteed to be unique
        for all versions of that ID. The minor version number
        is reset when the major number is incremented. 
        
    This class is not designed to be used on its own, but
    rather to be derived to an over-ridden class (e.g.
    MajMinVersion or IDVersion, or IDMajMinVersion)
        
    @author Christopher Woods
*/
class SIREBASE_EXPORT IDPair
{

friend class IDTriple;

private:
    static Incremint shared_pair_increment;

public:
    IDPair(Incremint *majint = &IDPair::shared_pair_increment);
    
    ~IDPair();
    
    bool operator==(const IDPair &other) const;
    bool operator!=(const IDPair &other) const;
    
protected:
    IDPair(const IDPair &other);

    IDPair& operator=(const IDPair &other);
    
    quint32 major() const;
    quint32 minor() const;
    
    void incrementMajor();
    void incrementMinor();
    
    bool sameMajorVersion(const IDPair &other) const;
    bool sameMinorVersion(const IDPair &other) const;
    bool sameVersion(const IDPair &other) const;
    
private:
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
inline void IDPair::incrementMinor()
{
    minnum = minor_incremint->increment();
}

/** Increment the major version number */
inline void IDPair::incrementMajor()
{
    majnum = major_incremint->increment();
    minor_incremint.reset( new Incremint(0) );
    minnum = 0;
}

/** Return the major number */
inline quint32 IDPair::major() const
{
    return majnum;
}

/** Return the minor number */
inline quint32 IDPair::minor() const
{
    return minnum;
}

/** Return whether or not this has the same major version as 'other' */
inline bool IDPair::sameMajorVersion(const IDPair &other) const
{
    return majnum == other.majnum;
}

/** Return whether or not this has the same minor version as 'other' */
inline bool IDPair::sameMinorVersion(const IDPair &other) const
{
    return minnum == other.minnum;
}
    
/** Return whether or not this has the same version as 'other' */
inline bool IDPair::sameVersion(const IDPair &other) const
{
    return majnum == other.majnum and minnum == other.minnum;
}
    
/** Comparison operator */
inline bool IDPair::operator==(const IDPair &other) const
{
    return majnum == other.majnum and minnum == other.minnum 
                and major_incremint == other.major_incremint 
                and minor_incremint == other.minor_incremint;
}

/** Comparison operator */
inline bool IDPair::operator!=(const IDPair &other) const
{
    return majnum != other.majnum or minnum != other.minnum
                or major_incremint != other.major_incremint
                or minor_incremint != other.minor_incremint;
}

}

#endif
