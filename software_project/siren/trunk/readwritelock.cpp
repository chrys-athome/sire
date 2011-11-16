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

#include "Siren/readwritelock.h"
#include "Siren/string.h"
#include "Siren/forages.h"
#include "Siren/timer.h"

#include <algorithm>

using namespace Siren;
using namespace Siren::detail;

namespace Siren
{
    namespace detail
    {
        class LockBreaker : public noncopyable
        {
        public:
            LockBreaker() : noncopyable()
            {}
            
            ~LockBreaker()
            {}
            
            QMutex mutex;
            QWaitCondition waiter;
        };
    
    } // end of namespace detail

} // end of namespace Siren


/** Constructor */
ReadWriteLock::ReadWriteLock() : Block()
{}

/** Destructor */
ReadWriteLock::~ReadWriteLock()
{
    LockBreaker *b = breaker.fetchAndStoreRelaxed(0);
    delete b;
}

void ReadWriteLock::createBreaker()
{
    while (breaker == 0)
    {
        LockBreaker *b = new LockBreaker();
        
        if (not breaker.testAndSetAcquire(0,b))
            delete b;
    }
}

/** Lock for reading */
void ReadWriteLock::lockForRead()
{
    if (l.tryLockForRead(200))
        return;

    try
    {
        createBreaker();
        Block::aboutToSleep();
    
        while (not l.tryLockForRead(200))
        {
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

/** Try to lock for reading - return whether we succeed */
bool ReadWriteLock::tryLockForRead()
{
    return l.tryLockForRead();
}

/** Try to lock for reading for up to 'ms' milliseconds.
    This returns whether or not we succeeded */
bool ReadWriteLock::tryLockForRead(int ms)
{
    if (ms < 200)
        return l.tryLockForRead(ms);
        
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
                return l.tryLockForRead();
            else
                return l.tryLockForRead(remaining_time);
        }

        while (remaining_time > 0)
        {
            if (l.tryLockForRead( std::min(remaining_time,200) ))
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

/** Lock fro writing */
void ReadWriteLock::lockForWrite()
{
    if (l.tryLockForWrite(200))
        return;

    try
    {
        createBreaker();
        Block::aboutToSleep();
    
        while (not l.tryLockForWrite(200))
        {
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

/** Try to lock for writing - return whether or not we succeed */
bool ReadWriteLock::tryLockForWrite()
{
    return l.tryLockForWrite();
}

/** Try to lock for writing for up to "ms" milliseconds */
bool ReadWriteLock::tryLockForWrite(int ms)
{
    if (ms < 200)
        return l.tryLockForWrite(ms);
        
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
                return l.tryLockForWrite();
            else
                return l.tryLockForWrite(remaining_time);
        }

        while (remaining_time > 0)
        {
            if (l.tryLockForWrite( std::min(remaining_time,200) ))
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

/** Unlock the lock */
void ReadWriteLock::unlock()
{
    l.unlock();
    
    if (breaker)
        breaker->waiter.wakeAll();
}

/** Return a string representation of this lock */
String ReadWriteLock::toString() const
{
    return String::tr("ReadWriteLock(%1)").arg(this);
}

/** Called by for_ages to asked this lock to check for the end of for_ages */
void ReadWriteLock::checkEndForAges() const
{
    if (breaker)
        breaker->waiter.wakeAll();
}
