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
#include "Siren/exceptions.h"

#include "Siren/detail/blockdata.h"

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
    
        class SemaphoreData : public BlockData
        {
        public:
            SemaphoreData(int n);
            ~SemaphoreData();
        
            const char* blockType() const;
                    
            String toString() const;
        
            void checkEndForAges() const;
        
            void createBreaker();
    
            QSemaphore s;
    
            AtomicPointer<detail::SemBreaker>::Type breaker;
        };
    
    } // end of namespace detail

} // end of namespace Siren

///////////
/////////// Implementation of SemaphoreData
///////////

/** Constructor */
SemaphoreData::SemaphoreData(int n) : BlockData(), s(n)
{}

/** Destructor */
SemaphoreData::~SemaphoreData()
{
    SemBreaker *b = breaker.fetchAndStoreRelaxed(0);
    delete b;
}

void SemaphoreData::createBreaker()
{
    while (breaker == 0)
    {
        SemBreaker *b = new SemBreaker();
        
        if (not breaker.testAndSetAcquire(0, b))
            delete b;
    }
}

const char* SemaphoreData::blockType() const
{
    return Semaphore::typeName();
}

/** Return a string representation of the semaphore */
String SemaphoreData::toString() const
{
    return String::tr("Semaphore(%1)").arg(this);
}

/** Check for the end of for_ages */
void SemaphoreData::checkEndForAges() const
{
    if (breaker)
        breaker->waiter.wakeAll();
}

///////////
/////////// Implementation of Semaphore
///////////

/** Construct a sempaphore with 1 reservation available */
Semaphore::Semaphore() : Block()
{
    d = new SemaphoreData(1);
    Block::setData(d);
}

/** Construct a semaphore with 'n' reservations available */
Semaphore::Semaphore(int n) : Block()
{
    d = new SemaphoreData(n);
    Block::setData(d);
}

/** Internal constructor used to construct a Semaphore from a BlockRef */
Semaphore::Semaphore(const exp_shared_ptr<detail::BlockData>::Type &ptr)
          : Block(ptr), d(0)
{
    if (ptr)
    {
        if ( std::strcmp(ptr->blockType(), Semaphore::typeName()) == 0 )
        {
            d = static_cast<SemaphoreData*>(ptr.get());
        }
        else
            throw Siren::invalid_cast( String::tr(
                    "Cannot cast a Block of type %1 to a Siren::Semaphore.")
                        .arg(ptr->blockType()), CODELOC );
    }
}

/** Copy constructor */
Semaphore::Semaphore(const Semaphore &other) : Block(other), d(other.d)
{}

/** Destructor */
Semaphore::~Semaphore()
{}

/** Copy assignment operator */
Semaphore& Semaphore::operator=(const Semaphore &other)
{
    if (this != &other)
    {
        Block::operator=(other);
        d = other.d;
    }
    
    return *this;
}

/** Internal function used by BlockRef to check if the passed data is a Mutex */
bool Semaphore::isOfType(const exp_shared_ptr<BlockData>::Type &ptr)
{
    if (ptr.get() == 0)
        return false;
    else
        return std::strcmp(ptr->blockType(), Semaphore::typeName()) == 0;
}

/** Acquire 'n' reservations - block until this is available */
void Semaphore::acquire(int n)
{
    if (n <= 0)
        return;
        
    //try for 200 ms, so that we don't invoke for_ages for short acquires
    if (d->s.tryAcquire(n, 200))
        return;
        
    try
    {
        d->createBreaker();
        Block::aboutToSleep();
    
        while (not d->s.tryAcquire(n,200))
        {
            for_ages::test();

            d->breaker->mutex.lock();
            d->breaker->waiter.wait( &(d->breaker->mutex) );
            d->breaker->mutex.unlock();
            
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
    return d->s.tryAcquire(n);
}

/** Try to acquire 'n' reservations for up to 'ms' milliseconds.
    Retruns whether or not it was successful */
bool Semaphore::tryAcquire(int n, int ms)
{
    if (n <= 0 or ms < 0)
        return false;
    
    else if (ms == 0)
        return d->s.tryAcquire(n);
        
    else if (ms <= 200)
        return d->s.tryAcquire(n,ms);
        
    try
    {
        //sleep until it is possible to break this lock
        d->createBreaker();
        Block::aboutToSleep();

        Timer t = Timer::start();
        int remaining_time = ms - t.elapsed();

        if (remaining_time <= 200)
        {
            Block::hasWoken();
            
            if (remaining_time <= 0)
                return d->s.tryAcquire(n);
            else
                return d->s.tryAcquire(n, remaining_time);
        }

        while (remaining_time > 0)
        {
            if (d->s.tryAcquire(n,std::min(remaining_time,200)))
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

/** Release 'n' reservations - this will create extra reservations
    on the semaphore if this increases the limit beyond the original
    value */
void Semaphore::release(int n)
{
    d->s.release(n);
    
    if (d->breaker)
        d->breaker->waiter.wakeAll();
}

/** Return the number of available reservations */
int Semaphore::available() const
{
    return d->s.available();
}
