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
#include "Siren/timer.h"

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

    for_ages::threadSleepingOn(this);

    do
    {
        w.wait( &(mutex->m) );
    
    } while (not for_ages::threadWoken(this));
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
        return false;
        
    Timer t = Timer::start();
    for_ages::threadSleepingOn(this);
    
    bool woken = false;
    
    do
    {
        woken = w.wait( &(mutex->m), time - t.elapsed() );

        if (t.elapsed() >= time)
        {
            woken = true;
            for_ages::threadHasWoken(this);
            break;
        }

    } while (not for_ages::threadWoken(this));
    
    return woken;
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
    for_ages::wakeOne(this);
    w.wakeAll(); // for_ages will assure that only one thread
                 // is woken - we need to wake them all temporarily to do this
}

/** Wake up all of the threads sleeping on this wait condition */
void WaitCondition::wakeAll()
{
    for_ages::wakeAll(this);
    w.wakeAll();
}
