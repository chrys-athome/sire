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
#include "Siren/forages.h"
#include "Siren/string.h"
#include "Siren/timer.h"
#include "Siren/block.hpp"

using namespace Siren;

/** Constructor */
WaitCondition::WaitCondition()
{}

/** Destructor */
WaitCondition::~WaitCondition()
{
    w.wakeAll();
}

/** Wait until this thread has been woken up.
    Note that the passed mutex must have been locked! */
void WaitCondition::wait(Mutex *mutex)
{
    if (mutex == 0)
        return;

    //first wait for 200 ms - this will prevent us from
    //wasting a lot of resources on short sleeps
    if (w.wait( &(mutex->m), 200 ))
        //the wait was interupted :-)
        return;

    try
    {
        this->aboutToSleep();

        do
        {
            w.wait( &(mutex->m) );
    
        } while (not this->shouldWake());
    }
    catch(...)
    {
        this->hasWoken();
        throw;
    }
}

/** Return a string representation of this WaitCondition */
String WaitCondition::toString() const
{
    return String::tr("WaitCondition(%1)").arg(this);
}

/** This function wakes up all sleeping threads so that they
    can check if their for_ages state has changed */
void WaitCondition::checkEndForAges() const
{
    const_cast<QWaitCondition*>(&w)->wakeAll();
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
        return w.wait( &(mutex->m), time );

    try
    {
        Timer t = Timer::start();
        this->aboutToSleep();
    
        bool woken = false;
    
        do
        {
            woken = w.wait( &(mutex->m), time - t.elapsed() );

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
    w.wakeAll(); // for_ages will assure that only one thread
                 // is woken - we need to wake them all temporarily to do this
}

/** Wake up all of the threads sleeping on this wait condition */
void WaitCondition::wakeAll()
{
    Block::setShouldWakeAll();
    w.wakeAll();
}
