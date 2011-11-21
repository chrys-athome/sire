/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/mutex.h"
#include "Siren/waitcondition.h"
#include "Siren/forages.h"
#include "Siren/string.h"
#include "Siren/timer.h"
#include "Siren/exceptions.h"

#include "Siren/detail/blockdata.h"

#include <algorithm>

using namespace Siren;
using namespace Siren::detail;

namespace Siren
{
    namespace detail
    {
        /** Small class that is used to break open a mutex
            at the end of for_ages */
        class MutexBreaker : public noncopyable
        {
        public:
            MutexBreaker() : noncopyable()
            {}
            
            ~MutexBreaker()
            {}
            
            QMutex mutex;
            QWaitCondition waiter;
        
        }; // end of class MutexBreaker
        
        /** Small class that is used to hold the data for a mutex */
        class MutexData : public BlockData
        {
        public:
            MutexData(Mutex::RecursionMode mode);
            ~MutexData();
            
            const char* blockType() const;

            void checkEndForAges() const;
            
            String toString() const;

            void createBreaker();

            /** The actual QMutex */
            QMutex m;

            /** Pointer to a breaker used to unblock a mutex */
            AtomicPointer<MutexBreaker>::Type breaker;
        
        };
        
    } // end of namespace detail
} // end of namespace Siren

///////////
/////////// Implementation of MutexData
///////////

/** Constructor */
MutexData::MutexData(Mutex::RecursionMode mode)
          : BlockData(), m( QMutex::RecursionMode(mode) )
{}

/** Destructor - make sure you have unlocked the mutex before
    losing the last reference to it! */
MutexData::~MutexData()
{
    MutexBreaker *b = breaker.fetchAndStoreRelaxed(0);
    delete b;
}

/** Return a string representation of this Mutex */
String MutexData::toString() const
{
    return String::tr("Mutex(%1)").arg(this);
}

/** Return the typename for the block that holds this data */
const char* MutexData::blockType() const
{
    return Mutex::typeName();
}

/** Internal function called to create the MutexBreaker object that
    is used to unblock stuck mutexes */
void MutexData::createBreaker()
{
    while (not breaker)
    {
        MutexBreaker *tmp = new MutexBreaker();
        
        if (not breaker.testAndSetAcquire(0, tmp))
        {
            delete tmp;
        }
    }
}

/** Called by for_ages to tell the mutex to check for the
    end of for_ages in each blocked thread */
void MutexData::checkEndForAges() const
{
    if (breaker)
        breaker->waiter.wakeAll();
}

///////////
/////////// Implementation of Mutex
///////////

/** Construct a mutex with the specified recursion mode */
Mutex::Mutex(Mutex::RecursionMode mode) : Block(), d(0)
{
    d = new MutexData(mode);
    Block::setData(d);
}

/** Internal constructor used to construct a Mutex from a BlockRef */
Mutex::Mutex(const exp_shared_ptr<detail::BlockData>::Type &ptr)
      : Block(ptr), d(0)
{
    if (ptr)
    {
        if ( std::strcmp(ptr->blockType(), Mutex::typeName()) == 0 )
        {
            d = static_cast<MutexData*>(ptr.get());
        }
        else
            throw Siren::invalid_cast( String::tr(
                    "Cannot cast a Block of type %1 to a Siren::Mutex.")
                        .arg(ptr->blockType()), CODELOC );
    }
}

/** Copy constructor */
Mutex::Mutex(const Mutex &other) : Block(other), d(other.d)
{}

/** Destructor */
Mutex::~Mutex()
{}

/** Copy assignment operator */
Mutex& Mutex::operator=(const Mutex &other)
{
    if (this != &other)
    {
        Block::operator=(other);
        d = other.d;
    }
    
    return *this;
}

/** Internal function used by BlockRef to check if the passed data is a Mutex */
bool Mutex::isOfType(const exp_shared_ptr<BlockData>::Type &ptr)
{
    if (ptr.get() == 0)
        return false;
    else
        return std::strcmp(ptr->blockType(), Mutex::typeName()) == 0;
}

/** Internal function used by WaitCondition to get a pointer to the underlying QMutex */
QMutex* Mutex::pointer()
{
    return &(d->m);
}

/** Lock the mutex. This will block until the mutex is locked,
    or the end of for_ages is signalled on this thread
    
    \throw Siren::interupted_thread
*/
void Mutex::lock()
{
    if (not d->m.tryLock(200))
    {
        //could not get the lock quickly, so we will use a semaphore 
        //to try to get an interruptable lock
        d->createBreaker();

        try
        {
            //sleep until it is possible to break this lock
            Block::aboutToSleep();
            
            while (not d->m.tryLock(200))
            {
                d->breaker->mutex.lock();
                d->breaker->waiter.wait( &(d->breaker->mutex) );
                d->breaker->mutex.unlock();
                
                for_ages::test();
            }
            
            //we have acquired the lock :-)
            Block::hasWoken();
        }
        catch(...)
        {
            Block::hasWoken();
            throw;
        }
    }
}

/** Try to lock this mutex - this will return whether 
    or not locking was successful */
bool Mutex::tryLock()
{
    return d->m.tryLock();
}

/** Try to lock this mutex, for up to 'ms' milliseconds.
    This returns whether or not locking was successful */
bool Mutex::tryLock(int ms)
{
    if (ms <= 200)
        return d->m.tryLock(ms);

    try
    {
        //sleep until it is possible to break this lock
        d->createBreaker();
        Block::aboutToSleep();

        Timer t = Timer::start();
        int remaining_time = ms - t.elapsed();

        if (remaining_time < 200)
        {
            Block::hasWoken();
            
            if (remaining_time <= 0)
                return d->m.tryLock();
            else
                return d->m.tryLock(remaining_time);
        }

        while (remaining_time > 0)
        {
            if (d->m.tryLock( std::min(remaining_time,200) ))
            {
                Block::hasWoken();
                return true;
            }

            for_ages::test();

            remaining_time = ms - t.elapsed();

            d->breaker->mutex.lock();
            d->breaker->waiter.wait( &(d->breaker->mutex), remaining_time );
            d->breaker->mutex.unlock();
            
            for_ages::test();
            remaining_time = ms - t.elapsed();
        }
        
        Block::hasWoken();
        
        return false;
    }
    catch(...)
    {
        Block::hasWoken();
        throw;
    }
    
    return false;
}

/** Unlock the mutex. You can only unlock the mutex in the 
    same thread in which it was locked */
void Mutex::unlock()
{
    d->m.unlock();

    //wake up anyone waiting for the lock
    if (d->breaker)
        d->breaker->waiter.wakeAll();
}
