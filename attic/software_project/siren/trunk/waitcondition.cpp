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

#include "Siren/waitcondition.h"
#include "Siren/mutex.h"
#include "Siren/readwritelock.h"
#include "Siren/forages.h"
#include "Siren/string.h"
#include "Siren/timer.h"
#include "Siren/block.hpp"
#include "Siren/exceptions.h"

#include "Siren/detail/blockdata.h"

using namespace Siren;
using namespace Siren::detail;

namespace Siren
{
    namespace detail
    {
        class WaitConditionData : public BlockData
        {
        public:
            WaitConditionData();
            ~WaitConditionData();
        
            String toString() const;

            const char* blockType() const;

            void checkEndForAges() const;

            QWaitCondition w;
        
        }; // end of class WaitConditionData
    
    } // end of namespace detail

} // end of namespace Siren

////////////
//////////// Implementation of class WaitConditionData
////////////

WaitConditionData::WaitConditionData() : BlockData()
{}

WaitConditionData::~WaitConditionData()
{
    w.wakeAll();
}

const char* WaitConditionData::blockType() const
{
    return WaitCondition::typeName();
}

/** Return a string representation of this WaitCondition */
String WaitConditionData::toString() const
{
    return String::tr("WaitCondition(%1)").arg(this);
}

/** This function wakes up all sleeping threads so that they
    can check if their for_ages state has changed */
void WaitConditionData::checkEndForAges() const
{
    const_cast<QWaitCondition*>(&w)->wakeAll();
}

////////////
//////////// Implementation of class WaitCondition
////////////

/** Constructor */
WaitCondition::WaitCondition() : Block()
{
    d = new WaitConditionData();
    Block::setData(d);
}

/** Internal constructor used to construct a Semaphore from a BlockRef */
WaitCondition::WaitCondition(const exp_shared_ptr<detail::BlockData>::Type &ptr)
              : Block(ptr), d(0)
{
    if (ptr)
    {
        if ( std::strcmp(ptr->blockType(), WaitCondition::typeName()) == 0 )
        {
            d = static_cast<WaitConditionData*>(ptr.get());
        }
        else
            throw Siren::invalid_cast( String::tr(
                    "Cannot cast a Block of type %1 to a Siren::WaitCondition.")
                        .arg(ptr->blockType()), CODELOC );
    }
}

/** Copy constructor */
WaitCondition::WaitCondition(const WaitCondition &other) : Block(other), d(other.d)
{}

/** Destructor */
WaitCondition::~WaitCondition()
{}

/** Internal function used by BlockRef to check if the passed data is a WaitCondition */
bool WaitCondition::isOfType(const exp_shared_ptr<BlockData>::Type &ptr)
{
    if (ptr.get() == 0)
        return false;
    else
        return std::strcmp(ptr->blockType(), WaitCondition::typeName()) == 0;
}

/** Copy assignment operator */
WaitCondition& WaitCondition::operator=(const WaitCondition &other)
{
    if (this != &other)
    {
        Block::operator=(other);
        d = other.d;
    }
    
    return *this;
}

/** Wait until this thread has been woken up.
    Note that the passed mutex must have been locked! */
void WaitCondition::wait(Mutex *mutex)
{
    if (mutex == 0)
        return;

    //first wait for 200 ms - this will prevent us from
    //wasting a lot of resources on short sleeps
    if (d->w.wait( mutex->pointer(), 200 ))
        //the wait was interupted :-)
        return;

    try
    {
        this->aboutToSleep();

        do
        {
            d->w.wait( mutex->pointer() );
    
        } while (not this->shouldWake());
    }
    catch(...)
    {
        this->hasWoken();
        throw;
    }
}

/** Wait until this thread has been woken up.
    Note that the passed lock must have been locked! */
void WaitCondition::wait(ReadWriteLock *lock)
{
    if (lock == 0)
        return;

    //first wait for 200 ms - this will prevent us from
    //wasting a lot of resources on short sleeps
    if (d->w.wait( lock->pointer(), 200 ))
        //the wait was interupted :-)
        return;

    try
    {
        this->aboutToSleep();

        do
        {
            d->w.wait( lock->pointer() );
    
        } while (not this->shouldWake());
    }
    catch(...)
    {
        this->hasWoken();
        throw;
    }
}

/** Wait until this thread has been woken up */
void WaitCondition::wait()
{
    Mutex m;
    MutexLocker lkr(&m);
    this->wait(&m);
}

/** Wait until this thread has been woken up, or until 'time' milliseconds
    has passed, whichever comes first. Returns whether or not the thread
    was woken up before 'time' milliseconds */
bool WaitCondition::wait(Mutex *mutex, unsigned long time)
{
    if (mutex == 0)
        return true;

    else if (time == 0)
        return true;

    else if (time < 200)
        return d->w.wait( mutex->pointer(), time );

    try
    {
        //switch over to use a local mutex so that we can ensure
        //that for_ages transitions are atomic
        Timer t = Timer::start();
        this->aboutToSleep();
    
        do
        {
            d->w.wait( mutex->pointer(), time - t.elapsed() );
            
            if (t.elapsed() >= time)
            {
                //we have run out of time without being woken!
                this->hasWoken();
                return false;
            }
        
        } while (not this->shouldWake());
        
        //we were woken up before time :-)
        this->hasWoken();
        return true;
    }
    catch(...)
    {
        this->hasWoken();
        throw;
    }
    
    return true;
}

/** Wait until this thread has been woken up, or until 'time' milliseconds
    has passed, whichever comes first. Returns whether or not the thread
    was woken up before 'time' milliseconds */
bool WaitCondition::wait(ReadWriteLock *lock, unsigned long time)
{
    if (lock == 0)
        return true;

    else if (time == 0)
        return true;

    else if (time < 200)
        return d->w.wait( lock->pointer(), time );

    try
    {
        Timer t = Timer::start();
        this->aboutToSleep();
    
        bool woken = false;
    
        do
        {
            woken = d->w.wait( lock->pointer(), time - t.elapsed() );

            if (woken)
            {
                this->hasWoken();
                return true;
            }
            else if (t.elapsed() >= time)
            {
                this->hasWoken();
                return false;
            }
            
        } while (not this->shouldWake());
    }
    catch(...)
    {
        this->hasWoken();
        throw;
    }
    
    return true;
}

/** Wait until this thread has been woken up, or until 'time' milliseconds
    has passed, whichever comes first. Returns whether or not the thread
    was woken up before 'time' milliseconds */
bool WaitCondition::wait(unsigned long time)
{
    Mutex m;
    MutexLocker lkr(&m);
    return this->wait(&m, time);
}

/** Wake up one of the threads sleeping on this wait condition */
void WaitCondition::wakeOne()
{
    Block::setShouldWakeOne();
    d->w.wakeAll(); // for_ages will assure that only one thread
                    // is woken - we need to wake them all temporarily to do this
}

/** Wake up all of the threads sleeping on this wait condition */
void WaitCondition::wakeAll()
{
    Block::setShouldWakeAll();
    d->w.wakeAll();
}
