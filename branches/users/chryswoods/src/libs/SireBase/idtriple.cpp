
#include "idtriple.h"

#include "SireError/errors.h"

using namespace SireBase;

/** Private static incremint that is used when no 
    other one is supplied by the user */
Incremint IDTriple::shared_triple_incremint;

/** Constructor - optionally provide the Incremint that is used
    to get the ID version number */
IDTriple::IDTriple(Incremint *idint)
         : id_incremint(idint)
{
    if (not idint)
        throw SireError::program_bug( QObject::tr(
                  "You *must* supply a valid Incremint to the "
                  "IDTriple constructor!"), CODELOC );

    this->incrementID();
}

/** Copy constructor */
IDTriple::IDTriple(const IDTriple &other)
              : major_incremint(other.major_incremint),
                id_incremint(other.id_incremint),
                idnum(other.idnum), versn(other.versn)
{}

/** Destructor */
IDTriple::~IDTriple()
{}

/** Assignment operator */
IDTriple& IDTriple::operator=(const IDTriple &other)
{
    major_incremint = other.major_incremint;
    id_incremint = other.id_incremint;
    idnum = other.idnum;
    versn = other.versn;
    
    return *this;
}
