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
#include "Siren/readwritelock.h"

#include "Siren/exceptions.h"
#include "Siren/assert.h"

using namespace Siren;
using namespace Siren::detail;

namespace Siren
{
    namespace detail
    {
        /** This internal struct is used to store the for_ages state
            of each individual thread */
        class ThreadState : public noncopyable
        {
        public:
        
            ThreadState(int thread_id) 
                          : noncopyable(),
                            id(thread_id), counter(0), 
                            current_block(0),
                            wake_from_current(false),
                            is_interupted(false), is_paused(false)
            {
                sirenDebug() << "CREATING" << id;
            }
            
            ~ThreadState()
            {
                sirenDebug() << "DELETING" << id;
            }

            String name;
            int id;
            int counter;

            const Block *current_block;
            bool wake_from_current;

            bool is_interupted;
            bool is_paused;
       
        }; // end of class ThreadState

        /** This internal struct is used to store the thread state
            of each thread in the program */
        class ProgramState : public noncopyable
        {
        public:
        
            ProgramState() : last_id(0), is_interupted(false), is_paused(false)
            {}
            
            ~ProgramState()
            {}
            
            ThreadStorage<ThreadState*>::Type thread;

            Hash<int,ThreadState*>::Type threads;

            int last_id;

            ReadWriteLock lock; // used to protect the directory of threads,
                                // whether or not we are interupted,
                                // the dictionary of wait conditions,
                                // the dictionary of locks and
                                // the dictionary of mutexes

            Mutex pause_mutex;  // used to protect the program and thread pause flags
            WaitCondition pause_waiter;
            
            Hash<const Block*,int>::Type blocks;
            
            bool is_interupted;
            bool is_paused;
        
        }; // end of class ProgramState

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
        
        if (not global_state_pvt.testAndSetAcquire(0,new_state))
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
    program->thread.setLocalData( new ThreadState(program->last_id) );
    
    program->threads.insert(program->last_id, program->thread.localData());
      
    return program->last_id;
}

/** Unregister the current thread from for_ages */
void for_ages::unregisterThisThread()
{
    ProgramState *program = programState();

    WriteLocker lkr( &(program->lock) );

    sirenDebug() << "UNREGISTERING THREAD";

    if (program->thread.hasLocalData())
    {
        ThreadState *thread = program->thread.localData();

        bool is_paused = thread->is_paused;
        
        sirenDebug() << "Unregister" << thread->id << is_paused;
        
        program->threads.remove(thread->id);
        
        if (is_paused and not program->is_paused)
            //wake the thread up
            program->pause_waiter.wakeAll();

        sirenDebug() << "Done :-)";

        // no need to delete the ThreadState object, as this will be 
        // deleted automatically by ThreadStorage when the thread exits
    }
}

/** Return whether or not the passed block is one of the special for_ages blocks */
static bool isForAgesBlock(const ProgramState *program, const Block *block)
{
    return (block == 0) or (block == &(program->lock)) or
           (block == &(program->pause_mutex)) or 
           (block == &(program->pause_waiter));
}

/** Internal function called by a block to set the for_ages flag
    to show that all threads waiting on this block can now wake up */
void for_ages::setShouldWakeAll(const Block *block)
{
    ProgramState *program = programState();

    if (block == 0 or isForAgesBlock(program,block))
        return;

    ReadLocker lkr( &(program->lock) );
    
    //tell each thread in the program that they can awaken from the passed
    //wait condition
    for (Hash<int,ThreadState*>::const_iterator it = program->threads.constBegin();
         it != program->threads.constEnd();
         ++it)
    {
        if ( (*it)->current_block == block )
        {
            (*it)->wake_from_current = true;
        }
    }
}

/** Internal function called by a block to say that just
    one thread that is waiting on this condition may now wake up */
void for_ages::setShouldWakeOne(const Block *block)
{
    ProgramState *program = programState();

    //ignore the global "pause_waiter"
    if (block == 0 or isForAgesBlock(program,block))
        return;

    ReadLocker lkr( &(program->lock) );
    
    bool first_thread = true;
    
    //tell the first thread waiting on this WaitCondition that it is allowed
    //to wake up
    for (Hash<int,ThreadState*>::const_iterator it = program->threads.constBegin();
         it != program->threads.constEnd();
         ++it)
    {
        //wake the first thread
        if ( (*it)->current_block == block )
        {
            (*it)->wake_from_current = first_thread;
            first_thread = false;
        }
    }
}

static String errorString()
{
    static String err = String::tr("Thread interupted by for_ages::end()");
    return err;
}

/** Internal function used to set the flags necessary to record that the 
    thread represented by 'local' is waiting on the passed Block.
    Note that you must hold a write lock on program->lock */
static void addRef(ProgramState *program, ThreadState *thread, const Block *block)
{
    if (block == 0 or isForAgesBlock(program,block))
        return;

    if (thread->current_block != 0)
        throw Siren::program_bug( String::tr(
                "It should not be possible for a single thread (%1, %2) to be waiting "
                "simultaneously on two blocks (%3 and %4)!")
                    .arg(thread->name).arg(thread->id)
                    .arg(block->toString())
                    .arg(thread->current_block->toString()), CODELOC );

    if (program->blocks.contains(block))
    {
        program->blocks[block] += 1;
    }
    else
    {
        program->blocks.insert(block, 1);
    }
    
    thread->current_block = block;
    thread->wake_from_current = false;
}

/** Internal function used to remove the flags used to record that
    the thread represented by 'local' is waiting on the passed block.
    Note that you must hold a write lock on program->lock */
static void removeRef(ProgramState *program, ThreadState *thread, const Block *block)
{
    if (isForAgesBlock(program,block))
        return;

    SIREN_ASSERT( thread->current_block == block );

    thread->current_block = 0;
    thread->wake_from_current = false;
        
    if (program->blocks.value(block, 1) <= 1)
    {
        program->blocks.remove(block);
    }
    else
    {
        program->blocks[block] -= 1;
    }
}

/** Internal function used by a block to tell for_ages that a thread
    is about sleeping on a block. This is to allow for_ages to wake that
    thread when for_ages::end() is called */
void for_ages::aboutToSleep(const Block *block)
{
    ProgramState *program = programState();
    
    if (isForAgesBlock(program,block))
        //don't record sleeping on any of the for_ages blocks
        return;

    if (program->thread.hasLocalData())
    {
        ThreadState *thread = program->thread.localData();
    
        WriteLocker lkr( &(program->lock) );
        addRef(program, thread, block);
        
        if (program->is_interupted or thread->is_interupted)
            throw Siren::interupted_thread( errorString(), CODELOC );
    }
}

/** Internal function used by Block to ask for_ages whether or not a thread
    that has woken from a block may stay awake, or whether it should go back
    to sleep. If this thread should wake, this function automatically
    removes the reference to this block from this thread */
bool for_ages::shouldWake(const Block *block)
{
    ProgramState *program = programState();
 
    sirenDebug() << "for_ages::shouldWake(" << block << ")";
          
    if (isForAgesBlock(program,block))
        //don't record waking from any of the for_ages blocks
        return true;

    if (program->thread.hasLocalData())
    {
        ThreadState *thread = program->thread.localData();
    
        if (thread->current_block == 0)
            return true;
    
        else if (thread->current_block != block)
            throw Siren::program_bug( String::tr(
                "A thread should only be able to be woken from one block "
                "at a time... (%1:%2, blocked on %3, woken from %4)")
                        .arg(thread->name).arg(thread->id)
                        .arg(thread->current_block->toString())
                        .arg(block->toString()),
                    CODELOC );

        WriteLocker lkr( &(program->lock) );
    
        if (thread->wake_from_current)
        {
            removeRef(program, thread, block);

            //check for the end of for_ages in this thread
            if (program->is_interupted or thread->is_interupted)
                throw Siren::interupted_thread( errorString(), CODELOC );
            
            return true;
        }
        else
        {
            if (program->is_interupted or thread->is_interupted)
            {
                removeRef(program, thread, block);
                throw Siren::interupted_thread( errorString(), CODELOC );
            }
            
            return false;
        }
    }
    else
        return true;
}

/** This function is called by a Block to say that the 
    current thread has woken up because of time */
void for_ages::hasWoken(const Block *block)
{
    ProgramState *program = programState();
    
    if (isForAgesBlock(program,block))
        //don't record waking from any of the for_ages blocks
        return;
                
    if (program->thread.hasLocalData())
    {
        ThreadState *thread = program->thread.localData();

        if (thread->current_block == 0)
            //the reference has been removed already
            return;

        else if (thread->current_block != block)
            throw Siren::program_bug( String::tr(
                "A thread should only be able to be woken from one block "
                "at a time... (%1:%2, blocked on %3, woken from %4)")
                        .arg(thread->name).arg(thread->id)
                        .arg(thread->current_block->toString())
                        .arg(block->toString()),
                    CODELOC );
    
        WriteLocker lkr( &(program->lock) );

        removeRef(program, thread, block);

        //check for the end of for_ages in this thread
        if (program->is_interupted or thread->is_interupted)
            throw Siren::interupted_thread( errorString(), CODELOC );
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

/** Pause the thread with ID 'thread_id'. This returns whether or 
    not this call actually paused the threads */
bool for_ages::pause(int thread_id)
{
    ProgramState *program = programState();

    ReadLocker lkr( &(program->lock) );
        
    ThreadState *thread = program->threads.value(thread_id,0);
        
    if (thread)
    {
        MutexLocker lkr2( &(program->pause_mutex) );
        
        if (thread->is_paused)
            return false;
        
        else
        {
            thread->is_paused = true;
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
    ProgramState *program = programState();

    MutexLocker lkr( &(program->pause_mutex) );
    
    if (program->is_paused)
    {
        program->is_paused = false;
        program->pause_waiter.wakeAll();
        return true;
    }
    else
        return false;
}

/** Play (wake) the thread with ID 'thread_id' if it is paused. This returns
    whether or not this call actually awoke the thread */
bool for_ages::play(int thread_id)
{
    ProgramState *program = programState();
    
    ReadLocker lkr( &(program->lock) );
        
    ThreadState *thread = program->threads.value(thread_id,0);
    
    if (thread)
    {
        MutexLocker lkr( &(program->pause_mutex) );
        
        if (thread->is_paused)
        {
            thread->is_paused = false;
            
            if (not program->is_paused)
            {
                program->pause_waiter.wakeAll();
                return true;
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
    ProgramState *program = programState();
    
    bool some_played = false;
    
    ReadLocker lkr( &(program->lock) );
    MutexLocker lkr2( &(program->pause_mutex) );
        
    for (Hash<int,ThreadState*>::const_iterator it = program->threads.constBegin();
         it != program->threads.constEnd();
         ++it)
    {
        if ((*it)->is_paused)
        {
            some_played = true;
            (*it)->is_paused = false;
        }
        
        if (program->is_paused)
        {
            some_played = true;
            program->is_paused = false;
        }
    }
    
    if (some_played)
        program->pause_waiter.wakeAll();
        
    return some_played;
}

/** Tell all threads that for_ages is over. This will interupt
    any thread that is blocking on a for_ages, causing it to
    throw a Siren::interupted_thread exception. This returns whether
    or not the end of for_ages was already set */
bool for_ages::end()
{
    ProgramState *program = programState();
    
    WriteLocker lkr( &(program->lock) ); // need write lock as will change 
                                         // interupt state

    if (program->is_interupted)
        return false;

    program->is_interupted = true;
        
    //tell every active block to check for the end of for_ages
    for (Hash<const Block*,int>::const_iterator it = program->blocks.constBegin();
         it != program->blocks.constEnd();
         ++it)
    {
        it.key()->checkEndForAges();
    }
    
    //wake everyone now in case anyone is paused
    program->pause_waiter.wakeAll();
        
    return true;
}

/** Tell the thread with ID 'thread_id' that for_ages is over. This 
    iwll interupt this thread if it is blocking on a for_ages,
    causing it to throw a Siren::interupted_thread exception. This
    returns whether or not the end of for_ages for this thread
    was already set */
bool for_ages::end(int thread_id)
{
    ProgramState *program = programState();
    
    WriteLocker lkr( &(program->lock) );
    
    sirenDebug() << "for_ages::end(" << thread_id << ")";
    
    ThreadState *thread = program->threads.value(thread_id, 0);
            
    if (thread)
    {
        if (not thread->is_interupted)
        {
            thread->is_interupted = true;
                
            if (thread->current_block)
                thread->current_block->checkEndForAges();
                    
            program->pause_waiter.wakeAll();
            return true;
        }
        else if (thread->current_block)
        {
            thread->current_block->checkEndForAges();
        }
    }
    
    return false;
}

/** Tell each individual thread to end for-ages */
bool for_ages::endAll()
{
    ProgramState *program = programState();
    
    bool some_ended = false;
    
    WriteLocker lkr( &(program->lock) );
        
    for (Hash<int,ThreadState*>::const_iterator it = program->threads.constBegin();
         it != program->threads.constEnd();
         ++it)
    {
        if (not (*it)->is_interupted)
        {
            some_ended = true;
            (*it)->is_interupted = true;
        }
    }
        
    if (not program->is_interupted)
    {
        some_ended = true;
        program->is_interupted = true;
    }
        
    if (some_ended)
    {
        program->pause_waiter.wakeAll();

        //wake up every thread waiting on a block
        for (Hash<const Block*,int>::const_iterator it = program->blocks.constBegin();
             it != program->blocks.constEnd();
             ++it)
        {
            it.key()->checkEndForAges();
        }
    }    
    
    return some_ended;
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
        ProgramState *program = programState();
        
        //really check that for-ages has finished - the check 
        //before didn't acquire the mutex
        ReadLocker lkr( &(program->lock) );
        
        if (program->is_interupted)
        {
            static String err = String::tr(
                    "The global end of for-ages has been signalled!");
                    
            throw Siren::interupted_thread(err, CODELOC);
        }
    }
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
    ProgramState *program = programState();

    if (not program->thread.hasLocalData())
    {
        //this is not a registered thread, so cannot be paused or interupted
        sirenDebug() << "RUNNING A TEST IN AN UNREGISTERED THREAD!";
        return;
    }

    ThreadState *thread = program->thread.localData();
    
    if (program->is_interupted or thread->is_interupted)
    {
        interupt_thread(thread->is_interupted);
    }
    else if (program->is_paused or thread->is_paused)
    {
        // actually pause the thread
        {
            MutexLocker lkr( &(program->pause_mutex) );
            program->pause_waiter.wait( &(program->pause_mutex) );
        }
        
        //check that we have not been interupted after we woke up
        for_ages::test();
    }
}

void for_ages::test(int n)
{
    ProgramState *program = programState();
    
    if (not program->thread.hasLocalData())
    {
        //this is not a registered thread, so cannot be paused or interupted
        sirenDebug() << "RUNNING A TEST IN AN UNREGISTERED THREAD!";
        return;
    }
    
    ThreadState *thread = program->thread.localData();
    
    thread->counter += 1;
    
    if (thread->counter == std::numeric_limits<int>::max())
        thread->counter = 0;
    
    if (thread->counter % n == 0)
    {
        if (program->is_interupted or thread->is_interupted)
        {
            interupt_thread(thread->is_interupted);
        }
        else if (program->is_paused or thread->is_paused)
        {
            // actually pause the thread
            {
                MutexLocker lkr( &(program->pause_mutex) );
                program->pause_waiter.wait( &(program->pause_mutex) );
            }
            
            //check that we have not been interupted after we woke up
            for_ages::test(n);
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
    sirenDebug() << "for_ages::sleep(" << secs << ")";
    
    if (secs <= 0)
        return;

    WaitCondition w;
    w.wait(1000*secs);
    
    sirenDebug() << "for_ages::sleep(" << secs << ") finished!";
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
