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
#include "Siren/exceptions.h"

#include "Siren/detail/blockdata.h"

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
        
        class ReadWriteLockData : public BlockData
        {
        public:
            ReadWriteLockData();
            ~ReadWriteLockData();
            
            const char* blockType() const;
    
            String toString() const;

            void checkEndForAges() const;
            
            void createBreaker();
        
            QReadWriteLock l;
        
            AtomicPointer<detail::LockBreaker>::Type breaker;
        };
    
    } // end of namespace detail

} // end of namespace Siren

////////
//////// Implementation of ReadWriteLockData
////////

ReadWriteLockData::ReadWriteLockData() : BlockData()
{}

ReadWriteLockData::~ReadWriteLockData()
{
    LockBreaker *b = breaker.fetchAndStoreRelaxed(0);
    delete b;
}

const char* ReadWriteLockData::blockType() const
{
    return ReadWriteLock::typeName();
}

void ReadWriteLockData::createBreaker()
{
    while (breaker == 0)
    {
        LockBreaker *b = new LockBreaker();
        
        if (not breaker.testAndSetAcquire(0,b))
            delete b;
    }
}

/** Return a string representation of this lock */
String ReadWriteLockData::toString() const
{
    return String::tr("ReadWriteLock(%1)").arg(this);
}

/** Called by for_ages to asked this lock to check for the end of for_ages */
void ReadWriteLockData::checkEndForAges() const
{
    if (breaker)
        breaker->waiter.wakeAll();
}

////////
//////// Implementation of ReadWriteLock
////////

/** Constructor */
ReadWriteLock::ReadWriteLock() : Block()
{
    d = new ReadWriteLockData();
    Block::setData(d);
}

/** Internal constructor used to construct a ReadWriteLock from a BlockRef */
ReadWriteLock::ReadWriteLock(const exp_shared_ptr<detail::BlockData>::Type &ptr)
              : Block(ptr), d(0)
{
    if (ptr)
    {
        if ( std::strcmp(ptr->blockType(), ReadWriteLock::typeName()) == 0 )
        {
            d = static_cast<ReadWriteLockData*>(ptr.get());
        }
        else
            throw Siren::invalid_cast( String::tr(
                    "Cannot cast a Block of type %1 to a Siren::ReadWriteLock.")
                        .arg(ptr->blockType()), CODELOC );
    }
}

/** Copy constructor */
ReadWriteLock::ReadWriteLock(const ReadWriteLock &other) : Block(other), d(other.d)
{}

/** Destructor */
ReadWriteLock::~ReadWriteLock()
{}

/** Copy assignment operator */
ReadWriteLock& ReadWriteLock::operator=(const ReadWriteLock &other)
{
    if (this != &other)
    {
        Block::operator=(other);
        d = other.d;
    }
    
    return *this;
}

/** Internal function used by BlockRef to check if the passed data is a Mutex */
bool ReadWriteLock::isOfType(const exp_shared_ptr<BlockData>::Type &ptr)
{
    if (ptr.get() == 0)
        return false;
    else
        return std::strcmp(ptr->blockType(), ReadWriteLock::typeName()) == 0;
}

/** Internal function used by WaitCondition to get a pointer to the underlying
    QReadWriteLock */
QReadWriteLock* ReadWriteLock::pointer()
{
    return &(d->l);
}

/** Lock for reading */
void ReadWriteLock::lockForRead()
{
    if (d->l.tryLockForRead(200))
        return;

    try
    {
        d->createBreaker();
        Block::aboutToSleep();
    
        while (not d->l.tryLockForRead(200))
        {
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

/** Try to lock for reading - return whether we succeed */
bool ReadWriteLock::tryLockForRead()
{
    return d->l.tryLockForRead();
}

/** Try to lock for reading for up to 'ms' milliseconds.
    This returns whether or not we succeeded */
bool ReadWriteLock::tryLockForRead(int ms)
{
    if (ms < 200)
        return d->l.tryLockForRead(ms);
        
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
                return d->l.tryLockForRead();
            else
                return d->l.tryLockForRead(remaining_time);
        }

        while (remaining_time > 0)
        {
            if (d->l.tryLockForRead( std::min(remaining_time,200) ))
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

/** Lock fro writing */
void ReadWriteLock::lockForWrite()
{
    if (d->l.tryLockForWrite(200))
        return;

    try
    {
        d->createBreaker();
        Block::aboutToSleep();
    
        while (not d->l.tryLockForWrite(200))
        {
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

/** Try to lock for writing - return whether or not we succeed */
bool ReadWriteLock::tryLockForWrite()
{
    return d->l.tryLockForWrite();
}

/** Try to lock for writing for up to "ms" milliseconds */
bool ReadWriteLock::tryLockForWrite(int ms)
{
    if (ms < 200)
        return d->l.tryLockForWrite(ms);
        
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
                return d->l.tryLockForWrite();
            else
                return d->l.tryLockForWrite(remaining_time);
        }

        while (remaining_time > 0)
        {
            if (d->l.tryLockForWrite( std::min(remaining_time,200) ))
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

/** Unlock the lock */
void ReadWriteLock::unlock()
{
    d->l.unlock();
    
    if (d->breaker)
        d->breaker->waiter.wakeAll();
}
