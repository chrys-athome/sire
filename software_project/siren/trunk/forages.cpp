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

namespace Siren
{
    namespace detail
    {
        /** This internal struct is used to store the for_ages state
            of an individual Siren thread */
        struct ForAgesState
        {
            ForAgesState() : thread_id(-1), is_interupted(false), is_paused(false)
            {}
            
            ~ForAgesState()
            {}

            int thread_id;
            String name;

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

            Mutex pause_mutex;
            WaitCondition pause_waiter;
        };

        SIREN_GLOBAL_STATIC( GlobalForAgesState, globalForAgesState )

    } // end of namespace detail
} // end of namespace Siren

/////////
///////// Implementation of for_ages
/////////

/** Call this function to register the current thread with for-ages. */
int for_ages::register_this_thread()
{
    GlobalForAgesState *s = globalForAgesState();

    if (not s)
        throw Siren::invalid_state( String::tr(
                "For some reason, we cannot get hold of the global for_ages state!"),
                    CODELOC );

    MutexLocker lkr( &(s->pause_mutex) );

    if (thread_state.hasLocalData())
        throw Siren::invalid_state( String::tr(
                "For some reason, this thread appears to have been already registered "
                "with for_ages."), CODELOC );
                
    s->last_id += 1;
    s->thread_state.setLocalData( new ForAgesState(s->last_id) );
    
    s->thread_states.insert(last_id, s->thread_state.localData());
      
    return last_id;
}

/** Unregister the current thread from for_ages */
void for_ages::unregister_this_thread()
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (not s)
        return;

    MutexLocker lkr( &(s->pause_mutex) );

    if (not s->thread_state.hasLocalData())
        //thread is not registered
        return;

    ForAgesState *state = s->thread_state.lockData();

    bool is_paused = state->is_paused;
        
    s->thread_state.setLocalData(0);
    s->thread_states.remove(state->thread_id);
        
    if (is_paused and not s->global_state.is_paused)
        //wake the thread up
        s->pause_waiter.wakeAll();
}

void throwInteruptedError()
{
    static String err = String::tr("for-ages is over. This thread has been interupted.");
    throw Siren::interupted_thread( err, CODELOC );
}

/** Pause all threads that use for_ages or for_ages mutexes. This returns whether
    or not this call actually paused the threads */
bool for_ages::pause_for_ages()
{
    GlobalForAgesState *s = globalForAgesState();

    if (s)
    {
        MutexLocker lkr( &(s->pause_mutex) );
        
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

/** Pause the thread with ID 'thread_id'. This returns whether or 
    not this call actually paused the threads */
bool for_ages::pause_for_ages(int thread_id)
{
    GlobalForAgesState *s = globalForAgesState();

    if (s)
    {
        MutexLocker lkr( &(s->pause_mutex) );
        
        if (s->thread_states.contains(thread_id))
        {
            if (s->thread_states[thread]->is_paused)
                return false;
            
            else
            {
                s->thread_states[thread]->is_paused = true;
                return true;
            }
        }
    }
    else
        return false;
}

/** Play (wake) all threads that are paused. This returns whether or not this
    call actually awoke the threads */
bool for_ages::play_for_ages()
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (s)
    {
        MutexLocker lkr( &(s->pause_mutex) );
        
        if (s->global_state.is_paused)
        {
            s->global_state.is_paused = false;
            s->pause_waiter.wakeAll();
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
bool for_ages::play_for_ages(int thread_id)
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (s)
    {
        MutexLocker lkr( &(s->pause_mutex) );
        
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
                    s->pause_waiter.wakeAll();
                    return true;
                }
            }
        }
    }
    
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
        MutexLocker lkr( &(s->pause_mutex) );

        if (not s->global_state.is_interupted)
        {
            s->global_state.is_interupted = true;
        
            //wake everyone now in case anyone is paused
            s->pause_waiter.wakeAll();
            
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
bool for_ages::end_for_ages(int thread_id)
{
    GlobalForAgesState *s = globalForAgesState();
    
    if (s)
    {
        MutexLocker lkr( &(s->pause_mutex) );
        
        if (s->thread_states.contains(thread))
        {
            if (not s->thread_states[thread]->is_interupted)
            {
                s->thread_states[thread]->is_interupted = true;
                s->pause_waiter.wakeAll();
                return true;
            }
        }
    }
    
    return false;
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

    ForAgesState *global = s->global_state;
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
        
    ForAgesState *global = s->global_state;
    ForAgesState *local = s->global_state.localData();
    
    if (not local)
        return;
        
    local->counter += 1;
    
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
    but can be interupted by calling 'end_for_ages()', in which case
    a Siren::interupted exception will be raised */
void SIREN_EXPORT sleep(int secs)
{
    if (secs <= 0)
        return;

    Mutex m;
    MutexLocker lkr(&m);
    
    WaitCondition w;
    w.wait(&m, 1000*secs);
}

/** Sleep for 'ms' milliseconds. This will pause for 'ms' milliseconds,
    but can be interupted by calling 'end_for_ages()', in which case
    a Siren::interupted exception will be raised */
void SIREN_EXPORT msleep(int ms)
{
    if (ms <= 0)
        return;

    Mutex m;
    MutexLocker lkr(&m);
    
    WaitCondition w;
    w.wait(&m, ms);
}
