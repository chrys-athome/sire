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

#include <algorithm>

using namespace Siren;
using namespace Siren::detail;

namespace Siren
{
    namespace detail
    {
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
    } // end of namespace detail
} // end of namespace Siren

/** Internal function called to create the MutexBreaker object that
    is used to unblock stuck mutexes */
void Mutex::createBreaker()
{
    while (not breaker)
    {
        MutexBreaker *tmp = new MutexBreaker();
        
        if (not breaker.testAndSetAcquire(tmp, 0))
        {
            delete tmp;
        }
    }
}

/** Construct a mutex with the specified recursion mode */
Mutex::Mutex(Mutex::RecursionMode mode) : m( QMutex::RecursionMode(mode) )
{}

/** Destructor - please make sure you have unlocked the mutex
    before destroying it! */
Mutex::~Mutex()
{
    MutexBreaker *b = breaker.fetchAndStoreRelaxed(0);
    delete b;
}

/** Return a string representation of this Mutex */
String Mutex::toString() const
{
    return String::tr("Mutex(%1)").arg(this);
}

/** Lock the mutex. This will block until the mutex is locked,
    or the end of for_ages is signalled on this thread
    
    \throw Siren::interupted_thread
*/
void Mutex::lock()
{
    if (not m.tryLock(200))
    {
        //could not get the lock quickly, so we will use a semaphore 
        //to try to get an interruptable lock
        createBreaker();

        try
        {
            //sleep until it is possible to break this lock
            Block::aboutToSleep();
            
            while (not m.tryLock(200))
            {
                breaker->mutex.lock();
                breaker->waiter.wait( &(breaker->mutex) );
                breaker->mutex.unlock();
                
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
    return m.tryLock();
}

/** Try to lock this mutex, for up to 'ms' milliseconds.
    This returns whether or not locking was successful */
bool Mutex::tryLock(int ms)
{
    if (ms <= 200)
        return m.tryLock(ms);

    try
    {
        //sleep until it is possible to break this lock
        createBreaker();
        Block::aboutToSleep();

        Timer t = Timer::start();
        int remaining_time = ms - t.elapsed();

        if (remaining_time < 200)
        {
            Block::hasWoken();
            
            if (remaining_time <= 0)
                return m.tryLock();
            else
                return m.tryLock(remaining_time);
        }

        while (remaining_time > 0)
        {
            if (m.tryLock( std::min(remaining_time,200) ))
            {
                Block::hasWoken();
                return true;
            }

            for_ages::test();

            remaining_time = ms - t.elapsed();

            breaker->mutex.lock();
            breaker->waiter.wait( &(breaker->mutex), remaining_time );
            breaker->mutex.unlock();
            
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
    m.unlock();

    //wake up anyone waiting for the lock
    if (breaker)
        breaker->waiter.wakeAll();
}

/** Called by for_ages to tell the mutex to check for the
    end of for_ages in each blocked thread */
void Mutex::checkEndForAges() const
{
    if (breaker)
        breaker->waiter.wakeAll();
}
