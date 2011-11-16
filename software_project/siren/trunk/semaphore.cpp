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

#include "Siren/semaphore.h"
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
        class SemBreaker : public noncopyable
        {
        public:
            SemBreaker() : noncopyable()
            {}
            
            ~SemBreaker()
            {}
            
            QMutex mutex;
            QWaitCondition waiter;
        
        }; // end of class SemBreaker
    
    } // end of namespace detail

} // end of namespace Siren

/** Construct a semaphore with 'n' reservations available */
Semaphore::Semaphore(int n) : Block(), s(n)
{}

/** Destructor */
Semaphore::~Semaphore()
{
    SemBreaker *b = breaker.fetchAndStoreRelaxed(0);
    delete b;
}

void Semaphore::createBreaker()
{
    while (breaker == 0)
    {
        SemBreaker *b = new SemBreaker();
        
        if (not breaker.testAndSetAcquire(0, b))
            delete b;
    }
}

/** Acquire 'n' reservations - block until this is available */
void Semaphore::acquire(int n)
{
    if (n <= 0)
        return;
        
    //try for 200 ms, so that we don't invoke for_ages for short acquires
    if (s.tryAcquire(n, 200))
        return;
        
    try
    {
        createBreaker();
        Block::aboutToSleep();
    
        while (not s.tryAcquire(n,200))
        {
            for_ages::test();

            breaker->mutex.lock();
            breaker->waiter.wait( &(breaker->mutex) );
            breaker->mutex.unlock();
            
            for_ages::test();
        }
        
        Block::hasWoken();
    }
    catch(...)
    {
        Block::hasWoken();
        throw;
    }
}

/** Try to acquire 'n' reservations - block until this is available */
bool Semaphore::tryAcquire(int n)
{
    return s.tryAcquire(n);
}

/** Try to acquire 'n' reservations for up to 'ms' milliseconds.
    Retruns whether or not it was successful */
bool Semaphore::tryAcquire(int n, int ms)
{
    if (n <= 0 or ms < 0)
        return false;
    
    else if (ms == 0)
        return s.tryAcquire(n);
        
    else if (ms <= 200)
        return s.tryAcquire(n,ms);
        
    try
    {
        //sleep until it is possible to break this lock
        createBreaker();
        Block::aboutToSleep();

        Timer t = Timer::start();
        int remaining_time = ms - t.elapsed();

        if (remaining_time <= 200)
        {
            Block::hasWoken();
            
            if (remaining_time <= 0)
                return s.tryAcquire(n);
            else
                return s.tryAcquire(n, remaining_time);
        }

        while (remaining_time > 0)
        {
            if (s.tryAcquire(n,std::min(remaining_time,200)))
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

/** Release 'n' reservations - this will create extra reservations
    on the semaphore if this increases the limit beyond the original
    value */
void Semaphore::release(int n)
{
    s.release(n);
    
    if (breaker)
        breaker->waiter.wakeAll();
}

/** Return the number of available reservations */
int Semaphore::available() const
{
    return s.available();
}

/** Return a string representation of the semaphore */
String Semaphore::toString() const
{
    return String::tr("Semaphore(%1)").arg(this);
}

/** Check for the end of for_ages */
void Semaphore::checkEndForAges() const
{
    if (breaker)
        breaker->waiter.wakeAll();
}
