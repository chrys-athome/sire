#ifndef SIREBASE_INCREMINT_H
#define SIREBASE_INCREMINT_H

#include "sireglobal.h"

#include <QAtomic>

SIRE_BEGIN_HEADER

namespace SireBase
{

/** This is a simple class that provides a thread-safe
    incrementable integer. This can be used, for example,
    to give a unique version of ID number to objects.
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT Incremint
{
public:
    Incremint(int value=0);
    Incremint(const Incremint &other);
    
    ~Incremint();
    
    int increment();

private:
    /** The volatile integer that is being incremented */
    volatile int atomic;
};

/** Constructor */
inline Incremint::Incremint(int value) : atomic(value)
{}

/** Copy constructor */
inline Incremint::Incremint(const Incremint &other) : atomic(other.atomic)
{}

/** Destructor */
inline Incremint::~Incremint()
{}

/** Increment the Incremint and return its new value */
inline int Incremint::increment()
{
    register int oldValue;
    
    do
    {
        oldValue = atomic;
    
    } while ( not q_atomic_test_and_set_int(&atomic, oldValue, oldValue+1) );
    
    return oldValue + 1;
}

}

SIRE_END_HEADER

#endif
