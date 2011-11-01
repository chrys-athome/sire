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
            of each individual thread */
        struct ThreadState
        {
            ThreadState() : id(-1), counter(0), 
                            current_block(0),
                            current_block_type(NOTBLOCKED),
                            wake_from_current(false),
                            is_interupted(false), is_paused(false)
            {}
            
            ~ThreadState()
            {}

            String name;
            int id;
            int counter;

            const void *current_block;
            
            enum BlockType
            {
                NOTBLOCKED = 0,
                WAITCONDITION = 1,
                MUTEX = 2,
                READWRITELOCK = 3,
                SEMAPHORE = 4
            };
            
            BlockType current_block_type;
            
            bool wake_from_current;

            bool is_interupted;
            bool is_paused;
        };

        /** This internal struct is used to store the thread state
            of each thread in the program */
        struct ProgramState
        {
            ProgramState() : last_id(0), is_interupted(false), is_paused(false)
            {}
            
            ~ProgramState()
            {}
            
            ThreadStorage<ThreadState*> thread;

            Hash<int,ThreadState*>::Type threads;

            int last_id;

            ReadWriteLock lock; // used to protect the directory of threads,
                                // whether or not we are interupted,
                                // the dictionary of wait conditions,
                                // the dictionary of locks and
                                // the dictionary of mutexes

            Mutex pause_mutex;  // used to protect the program and thread pause flags
            WaitCondition pause_waiter;
            
            Hash<WaitCondition*,int>::Type wait_conditions;
            Hash<Mutex*,int>::Type mutexes;
            Hash<ReadWriteLock*,int>::Type locks;
            Hash<Semaphore*,int>::Type semaphores;
            
            bool is_interupted;
            bool is_paused;
        };

    } // end of namespace detail
} // end of namespace Siren

/////////
///////// Implementation of for_ages
/////////

static AtomicPointer<ProgramState>::Type global_state_pvt;

/** Internal function used to get the global ForAges registry */
static ProgramState* programState()
{
    while (global_state_pvt == 0)
    {
        ProgramState *new_state = new ProgramState();
        
        if (not global_state_pvt.testAndSetAcquire(new_state,0))
        {
            delete new_state;
        }
    }
    
    return global_state_pvt;
}

/** Call this function to register the current thread with for-ages. */
int for_ages::registerThisThread()
{
    ProgramState *program = programState();

    WriteLocker lkr( &(program->lock) );

    if (program->thread.hasLocalData())
        throw Siren::program_bug( String::tr(
                "For some reason, this thread appears to have been already registered "
                "with for_ages."), CODELOC );
                
    program->last_id += 1;
    program->thread.setLocalData( new ThreadState(s->last_id) );
    
    program->threads.insert(last_id, program->thread.localData());
      
    return program->last_id;
}

/** Unregister the current thread from for_ages */
void for_ages::unregisterThisThread()
{
    ProgramState *program = programState();

    WriteLocker lkr( &(program->lock) );

    if (program->thread.hasLocalData())
    {
        ThreadState *thread = program->thread.localData();

        bool is_paused = thread->is_paused;
        
        program->threads.remove(thread->id);
        program->thread.setLocalData(0);
        
        if (is_paused and not program->is_paused)
            //wake the thread up
            program->pause_waiter.wakeAll();
    }
}

/** Internal function called by WaitCondition to set the for_ages flag
    to show that all threads waiting on this condition can now wake up */
void for_ages::wakeAll(WaitCondition *w)
{
    ProgramState *program = programState();

    //ignore the global "pause_waiter"
    if (w == &(program->pause_waiter))
        return;

    ReadLocker lkr( &(program->lock) );
    
    //tell each thread in the program that they can awaken from the passed
    //wait condition
    for (Hash<int,ThreadState*>::const_iterator it = program->threads.constBegin();
         it != program->threads.constEnd();
         ++it)
    {
        if ( (*it)->current_waiter == w )
        {
            (*it)->wake_from_current = true;
        }
    }
}

/** Internal function called by WaitCondition to say that just
    one thread that is waiting on this condition may now wake up */
void for_ages::wakeOne(WaitCondition *w)
{
    ProgramState *program = programState();

    //ignore the global "pause_waiter"
    if (w == &(program->pause_waiter))
        return;

    ReadLocker lkr( &(program->lock) );
    
    //tell the first thread waiting on this WaitCondition that it is allowed
    //to wake up
    for (Hash<int,ThreadState*>::const_iterator it = program->threads.constBegin();
         it != program->threads.constEnd();
         ++it)
    {
        //wake the first thread
        if ( (*it)->current_waiter == w )
        {
            (*it)->wake_from_current = true;
            return;
        }
    }
}

static String errorString()
{
    static String err = String::tr("Thread interupted by for_ages::end()");
    return err;
}

/** Internal function used to set the flags necessary to record that the 
    thread represented by 'local' is waiting on the passed wait condition.
    Note that you must hold a write lock on program->lock */
static void addWaiterRef(ProgramState *program, ThreadState *thread,
                         WaitCondition *w)
{
    if (w == &(program->pause_waiter))
        return;

    if (thread->current_waiter != 0)
        throw Siren::program_bug( String::tr(
                "It should not be possible for a single thread (%1, %2) to be waiting "
                "simultaneously on two WaitConditions!")
                    .arg(thread->name).arg(thread->id), CODELOC );

    if (program->wait_conditions.contains(w))
    {
        program->wait_conditions[w] += 1;
    }
    else
    {
        program->wait_conditions.insert(w, 1);
    }
    
    thread->current_waiter = w;
    thread->wake_from_current_waiter = false;
}

/** Internal function used to remove the flags used to record that
    the thread represented by 'local' is waiting on the passed wait condition.
    Note that you must hold a write lock on program->lock */
static void removeWaiterRef(ProgramState *program, ThreadState *thread,
                            WaitCondition *w)
{
    if (w == &(program->pause_waiter))
        return;

    if (thread->current_waiter == w)
    {
        thread->current_waiter = 0;
        thread->wake_from_current_waiter = false;
        
        if (program->wait_conditions.value(w, 1) <= 1)
        {
            program->wait_conditions.remove(w);
        }
        else
        {
            program->wait_conditions[w] -= 1;
        }
    }
}

/** Internal function used by WaitCondition to tell for_ages that a thread
    is sleeping on that WaitCondition. This is to allow for_ages to wake that
    thread when for_ages::end() is called */
void for_ages::threadSleepingOn(WaitCondition *w)
{
    ProgramState *program = programState();
    
    //don't need to record sleeping on the global for_ages pause_waiter
    if (w == &(program->pause_waiter))
        return;

    if (program->thread.hasLocalData())
    {
        ThreadState *thread = program->thread.localData();
    
        WriteLocker lkr( &(program->lock) );
        addWaiterRef(program, thread, w);
    }
}

/** Internal function used by WaitCondition to tell for_ages that a thread
    is no longer sleeping on a WaitCondition */
bool for_ages::threadWoken(WaitCondition *w)
{
    ProgramState *program = programState();
    
    //don't need to record waking on the global for_ages waiter
    if (w == &(program->pause_waiter))
        return true;

    if (program->thread.hasLocalData())
    {
        ThreadState *thread = program->thread.localData();
    
        if (thread->current_waiter != w)
            throw Siren::program_bug( String::tr(
                "A thread should only be able to be woken from one wait condition "
                "at a time... (%1, %2)").arg(thread->name).arg(thread->id),
                    CODELOC );

        WriteLocker lkr( &(program->lock) );
    
        if (thread->wake_from_current_waiter)
        {
            removeWaiterRef(program, thread, w);

            //check for the end of for_ages in this thread
            if (program->is_interupted or thread->is_interupted)
                throw Siren::interupted_thread( errorString(), CODELOC );
            
            return true;
        }
        else
        {
            if (program->is_interupted or thread->is_interupted)
            {
                removeWaiterRef(program, thread, w);
                throw Siren::interupted_thread( errorString(), CODELOC );
            }
            
            return false;
        }
    }
    else
        return true;
}

/** This function is called by a WaitCondition to say that the 
    current thread has woken up because of time */
void for_ages::threadHasWoken(WaitCondition *w)
{
    ProgramState *program = programState();
    
    //don't need to record waking on the global for_ages waiter
    if (w == &(program->pause_waiter))
        return;
                
    if (program->thread.hasLocalData())
    {
        ThreadState *thread = program->thread.localData();
    
        WriteLocker lkr( &(program->lock) );

        removeWaiterRef(program, thread, w);

        //check for the end of for_ages in this thread
        if (program->is_interupted or thread->is_interupted)
            throw Siren::interupted_thread( errorString(), CODELOC );
    }
}

/** Pause all threads that use for_ages or for_ages mutexes. This returns whether
    or not this call actually paused the threads */
bool for_ages::pause()
{
    ProgramState *program = programState();

    MutexLocker lkr( &(program->pause_mutex) );

    if (program->is_paused)
        return false;
    
    else
    {
        program->is_paused = true;
        return true;
    }
}

/** Update the name used to refer to the current thread */
void for_ages::setThisThreadName(const String &name)
{
    ProgramState *program = programState();

    if (program->thread.hasLocalData())
    {
        ThreadState *thread = program->thread.localData();
        thread->name = name;
    }
}

/** Return the name used to refer to this thread */
String for_ages::getThisThreadName()
{
    ProgramState *program = programState();

    if (program->thread.hasLocalData())
    {
        ThreadState *thread = program->thread.localData();
        
        return thread->name;
    }
    else
        return String::tr("Unregistered thread");
}

/** Pause the thread with ID 'thread_id'. This returns whether or 
    not this call actually paused the threads */
bool for_ages::pause(int thread_id)
{
    ProgramState *program = programState();

    ReadLocker lkr( &(program->lock) );
        
    if (program->threads.contains(thread_id))
    {
        MutexLocker lkr2( &(program->pause_mutex) );
        
        if (program->threads[thread_id]->is_paused)
            return false;
        
        else
        {
            program->threads[thread_id]->is_paused = true;
            return true;
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
    ProgramState *program = programState();
    
    bool some_paused = false;
    
    ReadLocker lkr( &(program->lock) );
    MutexLocker lkr2( &(program->pause_mutex) );
            
    for (Hash<int,ThreadState*>::const_iterator it = program->threads.constBegin();
         it != program->threads.constEnd();
         ++it)
    {
        if (not (*it)->is_paused)
        {
            some_paused = true;
            (*it)->is_paused = true;
        }
    }
        
    if (not program->is_paused)
    {
        some_paused = true;
        program->is_paused = true;
    }
        
    return some_paused;
}

/** Play (wake) all threads that are paused. This returns whether or not this
    call actually awoke the threads */
bool for_ages::play()
{
    ProgramState *s = ProgramState();
    
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
    ProgramState *s = ProgramState();
    
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
    ProgramState *s = ProgramState();
    
    if (s)
    {
        bool some_played = false;
    
        MutexLocker lkr( &(s->global_mutex) );
        
        for (Hash<int,ThreadState*>::iterator it = s->thread_states.begin();
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
    ProgramState *s = ProgramState();
    
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
    ProgramState *s = ProgramState();
    
    if (s)
    {
        MutexLocker lkr( &(s->global_mutex) );
        
        ThreadState *local = s->thread_states.value(thread_id, 0);
        
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
    ProgramState *s = ProgramState();
    
    if (s)
    {
        bool some_ended = false;
    
        MutexLocker lkr( &(s->global_mutex) );
        
        for (Hash<int,ThreadState*>::iterator it = s->thread_states.begin();
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
        ProgramState *s = ProgramState();
        
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
    ProgramState *s = ProgramState();
    
    if (not s)
        return;

    //acquire the mutex, to ensure that we really have been paused
    MutexLocker lkr( &(s->global_mutex) );

    ThreadState *global = &(s->global_state);
    ThreadState *local = s->global_state.localData();
    
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
    ProgramState *s = ProgramState();
    
    if (not s)
        return;

    ThreadState *global = &(s->global_state);
    ThreadState *local = s->global_state.localData();
    
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
    ProgramState *s = ProgramState();
    
    if (not s)
        return;
        
    ThreadState *global = &(s->global_state);
    ThreadState *local = s->local_state.localData();
    
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
