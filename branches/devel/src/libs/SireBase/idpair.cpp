
#include "idpair.h"

#include "SireError/errors.h"

using namespace SireBase;

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
                majnum(other.majnum), minnum(other.minnum)
{}

/** Destructor */
IDPair::~IDPair()
{}

/** Assignment operator */
IDPair& IDPair::operator=(const IDPair &other)
{
    minor_incremint = other.minor_incremint;
    major_incremint = other.major_incremint;
    majnum = other.majnum;
    minnum = other.minnum;
    
    return *this;
}
