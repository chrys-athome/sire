/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#ifndef SIREBASE_INCREMINT_H
#define SIREBASE_INCREMINT_H

#include "sireglobal.h"

#if QT_VERSION >= 0x040400
  #include <QAtomicInt>
#elif QT_VERSION >= 0x040100
  #include <QAtomic>
#else
  #error You need at least Qt Version 4.1
#endif

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
    #if QT_VERSION >= 0x040400
    QAtomicInt atomic_int;
    #elif QT_VERSION >= 0x040100
    volatile int atomic_int;
    #endif
};

/** Constructor */
inline Incremint::Incremint(int value) : atomic_int(value)
{}

/** Copy constructor */
inline Incremint::Incremint(const Incremint &other) 
                 : atomic_int(other.atomic_int)
{}

/** Destructor */
inline Incremint::~Incremint()
{}

/** Increment the Incremint and return its new value */
inline int Incremint::increment()
{
    #if QT_VERSION >= 0x040400
    
        register int oldValue;
        
        do
        {
            oldValue = atomic_int;
        
        } while (not atomic_int.testAndSetAcquire(oldValue, oldValue+1));
        
        return oldValue + 1;
    
    #elif QT_VERSION >= 0x040100

        register int oldValue;
    
        do
        {
            oldValue = atomic_int;
 
        } while ( not q_atomic_test_and_set_int(&atomic_int, oldValue, oldValue+1) );
    
        return oldValue + 1;
    
    #endif
}

}

SIRE_END_HEADER

#endif
