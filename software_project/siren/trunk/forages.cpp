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

#include "Siren/thread.h"
#include "Siren/threadstorage.h"
#include "Siren/forages.h"
#include "Siren/mutex.h"
#include "Siren/waitcondition.h"

#include "Siren/exceptions.h"

using namespace Siren;
using namespace Siren::detail;

namespace Siren
{
    namespace detail
    {
        /** This internal struct is used to store the for_ages state
            of an individual Siren thread */
        struct ForAgesState
        {
            ForAgesState() : thread_id(-1), counter(0), 
                             current_waiter(0), current_mutex(0),
                             is_interupted(false), is_paused(false)
            {}
            
            ~ForAgesState()
            {}

            String name;
            int thread_id;
            int counter;

            const WaitCondition *current_waiter;
            const Mutex *current_mutex;

            bool wake_from_current_waiter;
            bool wake_from_current_mutex;

            bool is_interupted;
            bool is_paused;
        };

        /** This internal struct is used to store the for_ages state
            of each active Siren thread */
        struct GlobalForAgesState
        {
            GlobalForAgesState() : last_id(0)
            {}
            
            ~GlobalForAgesState()
            {}
            
            ForAgesState global_state;
            ThreadStorage<ForAgesState*> thread_state;

            Hash<int,ForAgesState*>::Type thread_states;

            int last_id;

            Mutex global_mutex;
            WaitCondition global_waiter;
            
            Hash<WaitCondition*,int>::Type wait_conditions;
            Hash<Mutex*,int>::Type mutexes;
        };

        SIREN_GLOBAL_STATIC( GlobalForAgesState, globalForAgesState )

    } // end of namespace detail
} // end of namespace Siren

/////////
///////// Implementation of for_ages
/////////

/** Call this function to register the current thread with for-ages. */
int for_ages::registerThisThread()
{
    GlobalForAgesState *s = globalForAgesState();

    if (not s)
        throw Siren::program_bug( String::tr(
                "For some reason, we cannot get hold of the global for_ages state!"),
                    CODELOC );

    MutexLocker lkr( &(s->global_mutex) );

    if (s->thread_state.hasLocalData())
        throw Siren::program_bug( String::tr(
                "For some reason, this thread appears to have been already registered "
                "with for_ages."), CODELOC );
                
    s->last_id += 1;
    s->thread_state.setLocalData( new ForAgesState(s->last_id) );
    
    s->thread_states.insert(last_id, s->thread_state.localData());
      
    return s->last_id;
}

/** Unregister the current thread from for_ages */
void for_ages::unregisterThisThread()
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (not s)
        return;

    MutexLocker lkr( &(s->global_mutex) );

    if (not s->thread_state.hasLocalData())
        //thread is not registered
        return;

    ForAgesState *state = s->thread_state.localData();

    bool is_paused = state->is_paused;
        
    s->thread_state.setLocalData(0);
    s->thread_states.remove(state->thread_id);
        
    if (is_paused and not s->global_state.is_paused)
        //wake the thread up
        s->global_waiter.wakeAll();
}

/** Internal function used by WaitCondition to tell for_ages that a thread
    is sleeping on that WaitCondition. This is to allow for_ages to wake that
    thread when for_ages::end() is called */
void for_ages::threadSleepingOn(WaitCondition *w)
{
    GlobalForAgesState *s = globalForAgesState();

    if (not s)
        return;
    
    //don't need to record sleeping on the global for_ages waiter
    if (w == &(s->global_waiter))
        return;
                
    ForAgesState *local = s->thread_state.localData();
    
    if (not local)
        return;
        
    MutexLocker lkr( &(s->global_mutex) );
    
    if (s->wait_conditions.contains(w))
    {
        s->wait_conditions[w] += 1;
    }
    else
    {
        s->wait_conditions.insert(w, 1);
    }
    
    local->current_waiter = w;
    local->wake_from_current_waiter = false;
}

void for_ages::wakeAll(WaitCondition *w)
{
    GlobalForAgesState *s = globalForAgesState();

    if (not s)
        return;

    if (w == &(s->global_waiter))
        return;

    MutexLocker lkr( &(s->global_mutex) );
    
    for (Hash<int,ForAgesState*>::iterator it = s->thread_states.begin();
         it != s->thread_states.end();
         ++it)
    {
        if ( (*it)->current_waiter == w )
        {
            (*it)->wake_from_current_waiter = true;
        }
    }
}

void for_ages::wakeOne(WaitCondition *w)
{
    GlobalForAgesState *s = globalForAgesState();

    if (not s)
        return;

    if (w == &(s->global_waiter))
        return;

    MutexLocker lkr( &(s->global_mutex) );
    
    for (Hash<int,ForAgesState*>::iterator it = s->thread_states.begin();
         it != s->thread_states.end();
         ++it)
    {
        //wake the first thread
        if ( (*it)->current_waiter == w )
        {
            (*it)->wake_from_current_waiter = true;
            return;
        }
    }
}

/** Internal function used by WaitCondition to tell for_ages that a thread
    is no longer sleeping on a WaitCondition */
bool for_ages::threadWoken(WaitCondition *w)
{
    GlobalForAgesState *s = globalForAgesState();

    if (not s)
        return true;
    
    //don't need to record waking on the global for_ages waiter
    if (w == &(s->global_waiter))
        return true;
                
    ForAgesState *local = s->thread_state.localData();
    
    if (not local)
        return true;
        
    MutexLocker lkr( &(s->global_mutex) );

    //check for the end of for_ages in this thread
    if (s->global_state.is_interupted or local->is_interupted)
        //need to clear this thread's wait-condition pointer
        end_for_ages_state();
    
    if (local->wake_from_current_waiter)
    {
        //this thread should be woken up
        local->wake_from_current_waiter = false;
        local->current_waiter = 0;
    
        s->wait_conditions[w] -= 1;
    
        if (s->wait_conditions[w] <= 0)
            s->wait_conditions.remove(w);
            
        return true;
    }
    else
        return false;
}

/** This function is called by a WaitCondition to say that the 
    current thread has woken up because of time */
void for_ages::threadHasWoken(WaitCondition *w)
{
    GlobalForAgesState *s = globalForAgesState();

    if (not s)
        return;
    
    //don't need to record waking on the global for_ages waiter
    if (w == &(s->global_waiter))
        return;
                
    ForAgesState *local = s->thread_state.localData();
    
    if (not local)
        return;
        
    MutexLocker lkr( &(s->global_mutex) );

    //check for the end of for_ages in this thread
    if (s->global_state.is_interupted or local->is_interupted)
        //need to clear this thread's wait-condition pointer
        end_for_ages_state();

    if (local->current_waiter != w)
        throw Siren::program_bug( String::tr("WHAT???"), CODELOC );

    local->wake_from_current_waiter = false;
    local->current_waiter = 0;

    //this thread should be woken up
    s->wait_conditions[w] -= 1;
    
    if (s->wait_conditions[w] <= 0)
        s->wait_conditions.remove(w);
}

/** Pause all threads that use for_ages or for_ages mutexes. This returns whether
    or not this call actually paused the threads */
bool for_ages::pause()
{
    GlobalForAgesState *s = globalForAgesState();

    if (s)
    {
        MutexLocker lkr( &(s->global_mutex) );
        
        if (s->global_state.is_paused)
            return false;
        
        else
        {
            s->global_state.is_paused = true;
            return true;
        }
    }
    else
        return false;
}

/** Update the name used to refer to the current thread */
void for_ages::setThisThreadName(const String &name)
{
    GlobalForAgesState *s = globalForAgesState();

    if (s)
    {
        ForAgesState *local = s->thread_state.localData();
        
        if (not local)
            return;
    
        MutexLocker lkr( &(local->mutex) );

        local->name = name;
    }
}

/** Return the name used to refer to this thread */
String for_ages::getThisThreadName()
{
    GlobalForAgesState *s = globalForAgesState();

    if (s)
    {
        ForAgesState *local = s->thread_state.localData();
        
        if (local)
        {
            MutexLocker lkr( &(local->mutex) );
            return local->name;
        }
    }
    
    return String::tr("Unregistered thread");
}

/** Pause the thread with ID 'thread_id'. This returns whether or 
    not this call actually paused the threads */
bool for_ages::pause(int thread_id)
{
    GlobalForAgesState *s = globalForAgesState();

    if (s)
    {
        MutexLocker lkr( &(s->global_mutex) );
        
        if (s->thread_states.contains(thread_id))
        {
            if (s->thread_states[thread_id]->is_paused)
                return false;
            
            else
            {
                s->thread_states[thread_id]->is_paused = true;
                return true;
            }
        }
    }
    else
        return false;
}

/** Pause all threads individually - this is different to 'for_ages::pause()' as
    that applies a global pause, while this sets the pause flag for each
    registered thread */
bool for_ages::pauseAll()
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (s)
    {
        bool some_paused = false;
    
        MutexLocker lkr( &(s->global_mutex) );
        
        for (Hash<int,ForAgesState*>::iterator it = s->thread_states.begin();
             it != s->thread_states.end();
             ++it)
        {
            if (not (*it)->is_paused)
            {
                some_paused = true;
                (*it)->is_paused = true;
            }
        }
        
        if (not s->global_state.is_paused)
        {
            some_paused = true;
            s->global_state.is_paused = true;
        }
        
        return some_paused;
    }
    else
        return false;
}

/** Play (wake) all threads that are paused. This returns whether or not this
    call actually awoke the threads */
bool for_ages::play()
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (s)
    {
        MutexLocker lkr( &(s->global_mutex) );
        
        if (s->global_state.is_paused)
        {
            s->global_state.is_paused = false;
            s->global_waiter.wakeAll();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

/** Play (wake) the thread with ID 'thread_id' if it is paused. This returns
    whether or not this call actually awoke the thread */
bool for_ages::play(int thread_id)
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (s)
    {
        MutexLocker lkr( &(s->global_mutex) );
        
        if (s->thread_states.contains(thread_id))
        {
            if (s->thread_states[thread_id]->is_paused)
            {
                s->thread_states[thread_id]->is_paused = false;
                
                if (not s->global_state.is_paused)
                {
                    //have to wake everyone, but everyone but the 
                    //desired thread will see that they are still 
                    //paused so will go back to sleep
                    s->global_waiter.wakeAll();
                    return true;
                }
            }
        }
    }
    
    return false;
}

/** Tell all threads to wake up - note that this is different to for_ages::play()
    because this wakes each individual thread, rather than setting the global
    "play" flag. This will set the "play" flag for each individual thread */
bool for_ages::playAll()
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (s)
    {
        bool some_played = false;
    
        MutexLocker lkr( &(s->global_mutex) );
        
        for (Hash<int,ForAgesState*>::iterator it = s->thread_states.begin();
             it != s->thread_states.end();
             ++it)
        {
            if ((*it)->is_paused)
            {
                some_played = true;
                (*it)->is_paused = false;
            }
        }
        
        if (s->global_state.is_paused)
        {
            some_played = true;
            s->global_state.is_paused = false;
        }
        
        if (some_played)
            s->global_waiter.wakeAll();
        
        return some_played;
    }
    else
        return false;
}

/** Tell all threads that for_ages is over. This will interupt
    any thread that is blocking on a for_ages, causing it to
    throw a Siren::interupted_thread exception. This returns whether
    or not the end of for_ages was already set */
bool for_ages::end()
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (s)
    {
        MutexLocker lkr( &(s->global_mutex) );

        if (not s->global_state.is_interupted)
        {
            s->global_state.is_interupted = true;
        
            //wake up every thread waiting on any WaitCondition
            for (Hash<WaitCondition*,int>::iterator it = s->wait_conditions.begin();
                 it != s->wait_conditions.end();
                 ++it)
            {
                it.key()->checkEndForAges();
            }
        
            //unlock every mutex that is locked
            for (Hash<Mutex*,int>::iterator it = s->mutexes.begin();
                 it != s->mutexes.end();
                 ++it)
            {
                it.key()->checkEndForAges();
            }
        
            //wake everyone now in case anyone is paused
            s->global_waiter.wakeAll();
            
            return true;
        }
    }
    
    return false;
}

/** Tell the thread with ID 'thread_id' that for_ages is over. This 
    iwll interupt this thread if it is blocking on a for_ages,
    causing it to throw a Siren::interupted_thread exception. This
    returns whether or not the end of for_ages for this thread
    was already set */
bool for_ages::end(int thread_id)
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (s)
    {
        MutexLocker lkr( &(s->global_mutex) );
        
        ForAgesState *local = s->thread_states.value(thread_id, 0);
        
        if (local)
        {
            if (not local->is_interupted)
            {
                local->is_interupted = true;
                
                if (local->current_waiter)
                    local->current_waiter->checkEndForAges();
                    
                if (local->current_mutex)
                    local->current_mutex->checkEndForAges();
                
                s->global_waiter.wakeAll();
                return true;
            }
        }
    }
    
    return false;
}

/** Tell each individual thread to end for-ages */
bool for_ages::endAll()
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (s)
    {
        bool some_ended = false;
    
        MutexLocker lkr( &(s->global_mutex) );
        
        for (Hash<int,ForAgesState*>::iterator it = s->thread_states.begin();
             it != s->thread_states.end();
             ++it)
        {
            if (not (*it)->is_interupted)
            {
                some_ended = true;
                (*it)->is_interupted = true;
            }
        }
        
        if (not s->global_state.is_interupted)
        {
            some_ended = true;
            s->global_state.is_interupted = true;
        }
        
        if (some_ended)
        {
            s->global_waiter.wakeAll();

            //wake up every thread waiting on any WaitCondition
            for (Hash<WaitCondition*,int>::iterator it = s->wait_conditions.begin();
                 it != s->wait_conditions.end();
                 ++it)
            {
                it.key()->checkEndForAges();
            }
        
            //unlock every mutex that is locked
            for (Hash<Mutex*,int>::iterator it = s->mutexes.begin();
                 it != s->mutexes.end();
                 ++it)
            {
                it.key()->checkEndForAges();
            }
        }
        
        return some_ended;
    }
    else
        return false;
}

void interupt_thread(bool local_is_interupted)
{
    if (local_is_interupted)
    {
        static String err = String::tr(
                "The local end of for-ages has been signalled for this thread.");
                
        throw Siren::interupted_thread(err, CODELOC);
    }
    else
    {
        GlobalForAgesState *s = globalForAgesState();
        
        if (not s)
            return;
            
        //really check that for-ages has finished - the check 
        //before didn't acquire the mutex
        MutexLocker lkr( &(s->global_mutex) );
        
        if (s->global_state->is_interupted)
        {
            static String err = String::tr(
                    "The global end of for-ages has been signalled!");
                    
            throw Siren::interupted_thread(err, CODELOC);
        }
    }
}

void pause_thread(bool local_is_paused)
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (not s)
        return;

    //acquire the mutex, to ensure that we really have been paused
    MutexLocker lkr( &(s->global_mutex) );

    ForAgesState *global = &(s->global_state);
    ForAgesState *local = s->global_state.localData();
    
    if (not local)
        //this thread as not been registered, so is not subject to for_ages
        return;
    
    if (global->is_interupted or local->is_interupted)
    {
        //we have been interupted before we could pause!
        lkr.unlock();
        interupt_thread(local->is_interupted);
    }
    else if (global->is_paused or local->is_paused)
    {
        s->global_waiter.sleep( &(s->global_mutex) );
    }
    
    //test for pausing of interupting again, in case 
    //this has been signalled while we were sleeping
    for_ages::test();
}

/** This function does nothing, unless the end of for-ages
    has been signalled, in which case it will throw a
    Siren::interupted_thread exception, or in the case that
    the thread has been paused, in which case this function
    will block. This allows you to put
    checks in your code before long or intensive calculations
    that will allow the code to be exited by signalling
    the end of for-ages by called for_ages::end()
*/
void for_ages::test()
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (not s)
        return;

    ForAgesState *global = &(s->global_state);
    ForAgesState *local = s->global_state.localData();
    
    if (not local)
        //this thread as not been registered, so is not subject to for_ages
        return;
        
    if (global->is_interupted or local->is_interupted)
    {
        interupt_thread(local->is_interupted);
    }
    else if (global->is_paused or local->is_paused)
    {
        pause_thread(local->is_paused);
    }
}

void for_ages::test(int n)
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (not s)
        return;
        
    ForAgesState *global = &(s->global_state);
    ForAgesState *local = s->local_state.localData();
    
    if (not local)
        return;
        
    local->counter += 1;
    
    if (local->counter == std::numeric_limits<int>::max())
        local->counter = 0;
    
    if (local->counter % n == 0)
    {
        if (global->is_interupted or local->is_interupted)
        {
            interupt_thread(local->is_interupted);
        }
        else if (global->is_paused or local->is_paused)
        {
            pause_thread(local->is_paused);
        }
    }
}

/** This is a simple function that always returns 'true'.
    However, once the end for-ages has been signalled, this
    will throw a Siren::interupted_thread exception. You can use
    this to create infinite loops that will loop for-ages, e.g.
    
    while (for_ages::loop())
    {
        //do stuff
    }
    
    This loop will continue until for_ages::end() is called
*/
bool for_ages::loop()
{
    for_ages::test();
    return true;
}

bool for_ages::loop(int n)
{
    for_ages::test(n);
    return true;
}

/** Sleep for 'secs' seconds. This will pause for 'secs' seconds,
    but can be interupted by calling 'for_ages::end()', in which case
    a Siren::interupted_thread exception will be raised */
void for_ages::sleep(int secs)
{
    if (secs <= 0)
        return;

    WaitCondition w;
    w.wait(1000*secs);
}

/** Sleep for 'ms' milliseconds. This will pause for 'ms' milliseconds,
    but can be interupted by calling 'for_ages::end()', in which case
    a Siren::interupted_thread exception will be raised */
void for_ages::msleep(int ms)
{
    if (ms <= 0)
        return;
    
    WaitCondition w;
    w.wait(ms);
}
