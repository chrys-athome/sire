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
#include "Siren/mutex.h"
#include "Siren/waitcondition.h"
#include "Siren/exceptions.h"
#include "Siren/forages.h"
#include "Siren/obj.h"
#include "Siren/object.hpp"
#include "Siren/obj.hpp"
#include "Siren/none.h"
#include "Siren/siren.hpp"

using namespace Siren;
using namespace Siren::detail;

namespace Siren
{
    namespace detail
    {
        /** This class provides the actual implementation of Thread */
        class ThreadData : public QThread
        {
        public:
            ThreadData();
            ~ThreadData();
            
            int session();

            void setFunction( void (*function)() );

            bool isError();
            bool hasFinished();
            
            void throwError();
            void checkError();
            
            void abort();
            void pause();
            void play();
            
            void quit();
            
        protected:
            void run();
            
        private:
            /** Mutex protecting access to the data of this thread */
            Mutex m;
        
            /** WaitCondition used to wake the thread when it has something to run */
            WaitCondition waiter;
        
            /** Any error that was caught during running of the function */
            Obj err;
        
            /** The for_ages thread ID */
            int thread_id;
        
            /** The current session number - this is incremented
                when the thread has finished its work */
            int session_id;
            
            /** Pointer to the function that should be run */
            void (*func_ptr)();
            
            /** Whether or not we should exit the thread */
            bool should_quit;
        };
    
        /** This class provides a simple pool of threads */
        class ThreadPool : public noncopyable
        {
        public:
            ThreadPool();
            ~ThreadPool();
            
            Mutex m;
            List<exp_shared_ptr<ThreadData>::Type>::Type free_threads;
            List<exp_shared_ptr<ThreadData>::Type>::Type busy_threads;
        };
    
    } // end of namespace detail

} // end of namespace Siren

//////////
////////// Implementation of ThreadData
//////////

/** Constructor */
ThreadData::ThreadData() 
           : QThread(), thread_id(0), session_id(0), func_ptr(0), should_quit(false)
{
    QThread::start();
}

/** Destructor */
ThreadData::~ThreadData()
{
    this->quit();
    QThread::wait();
}

/** Tell this thread to quit as soon as it can */
void ThreadData::quit()
{
    MutexLocker lkr(&m);
    should_quit = true;
    waiter.wakeAll();
}

/** Return the session ID of this run */
int ThreadData::session()
{
    MutexLocker lkr(&m);
    return session_id;
}

/** Set the function that should be run by this thread */
void ThreadData::setFunction( void (*function)() )
{
    MutexLocker lkr(&m);
    
    if (func_ptr)
        throw Siren::program_bug( String::tr(
                "It should not be possible to set the function of an active thread!"),
                    CODELOC );

    session_id += 1;
    err = None();
    func_ptr = function;

    waiter.wakeAll();
}

/** Return whether processing the function caused an error */
bool ThreadData::isError()
{
    MutexLocker lkr(&m);
    return not err.isNone();
}

/** Return whether or not the last job has finished */
bool ThreadData::hasFinished()
{
    MutexLocker lkr(&m);
    return func_ptr == 0;
}

/** Throw the error (if one exists) */
void ThreadData::throwError()
{
    MutexLocker lkr(&m);
    
    if (err.isNone())
        return;
    
    else if (err.isA<Exception>())
        err.asA<Exception>().throwSelf();
}

/** Check if there was an error, and if there was, then throw it */
void ThreadData::checkError()
{
    MutexLocker lkr(&m);
    
    if (err.isNone())
        return;
    
    else if (err.isA<Exception>())
        err.asA<Exception>().throwSelf();
}

/** Abort running this thread - this sends an end_for_ages to this thread */
void ThreadData::abort()
{
    MutexLocker lkr(&m);

    if (thread_id)
        for_ages::end(thread_id);
}

/** Pause this thread */
void ThreadData::pause()
{
    MutexLocker lkr(&m);
    
    if (thread_id)
        for_ages::pause(thread_id);
}

/** Resume playing this thread */
void ThreadData::play()
{
    MutexLocker lkr(&m);
    
    if (thread_id)
        for_ages::play(thread_id);
}

/** The actual code running in the thread */
void ThreadData::run()
{
    MutexLocker lkr(&m);

    try
    {
        thread_id = for_ages::registerThisThread();
        
        while (for_ages::loop())
        {
            if (should_quit)
            {
                break;
            }
            else if (func_ptr)
            {
                lkr.unlock();
                
                try
                {
                    (*func_ptr)();
                }
                catch(const Siren::Exception &e)
                {
                    err = e;
                }
                catch(const std::exception &e)
                {
                    err = standard_exception(e, CODELOC);
                }
                catch(...)
                {
                    err = unknown_exception(CODELOC);
                }
                
                lkr.relock();
                func_ptr = 0;
            }
            
            waiter.wait(&m);
        }
    }
    catch(const Siren::Exception &e)
    {
        err = e;
    }
    catch(const std::exception &e)
    {
        err = standard_exception(e,CODELOC);
    }
    catch(...)
    {
        err = unknown_exception(CODELOC);
    }
    
    for_ages::unregisterThisThread();
    thread_id = 0;
    session_id = 0;
}

//////////
////////// Implementation of ThreadPool
//////////

ThreadPool::ThreadPool()
{}

ThreadPool::~ThreadPool()
{
    //make sure that all of the threads quit before the program ends
    MutexLocker lkr(&m);
    
    for (List<exp_shared_ptr<ThreadData>::Type>::iterator it = free_threads.begin();
         it != free_threads.end();
         ++it)
    {
        (*it)->quit();
    }
    
    for (List<exp_shared_ptr<ThreadData>::Type>::iterator it = busy_threads.begin();
         it != busy_threads.end();
         ++it)
    {
        (*it)->quit();
    }
}

//////////
////////// Implementation of Thread
//////////

static AtomicPointer<ThreadPool>::Type global_pool;

static ThreadPool& pool()
{
    while (global_pool == 0)
    {
        ThreadPool *p = new ThreadPool();
        
        if (not global_pool.testAndSetAcquire(0,p))
            delete p;
    }
    
    return *global_pool;
}

/** Construct a null Thread */
Thread::Thread() : session_id(0)
{}

/** Copy constructor */
Thread::Thread(const Thread &other) : d(other.d), session_id(other.session_id)
{}

/** Destructor */
Thread::~Thread()
{
    if (d.unique())
    {
        //return the underlying thread to the pool
        MutexLocker lkr( &(pool().m) );
        pool().busy_threads.append(d);
    }
}

/** Copy assignment operator */
Thread& Thread::operator=(const Thread &other)
{
    d = other.d;
    session_id = other.session_id;
    return *this;
}

/** Comparison operator */
bool Thread::operator==(const Thread &other) const
{
    return d.get() == other.d.get() and session_id == other.session_id;
}

/** Comparison operator */
bool Thread::operator!=(const Thread &other) const
{
    return not operator==(other);
}

/** Return whether or not this thread is null */
bool Thread::isNull()
{
    return d.get() == 0;
}

/** Start a thread to run the passed function. The thread will be started
    and run as soon as possible */
Thread Thread::run( void (*function)() )
{
    ThreadPool &p = pool();
    
    Thread t;
    int nfree = 0;
    
    {
        MutexLocker lkr( &(p.m) );
        
        if (p.free_threads.isEmpty())
        {
            //have any of the active threads finished?
            List<exp_shared_ptr<ThreadData>::Type>::MutableIterator it(p.busy_threads);
            
            while (it.hasNext())
            {
                it.next();
                
                if ( it.value()->hasFinished() )
                {
                    t.d = it.value();
                    it.remove();
                    break;
                }
            }
            
            if (t.isNull())
                //there were no free threads, so create a new thread
                t.d.reset( new ThreadData() );
        }
        else
        {
            t.d = p.free_threads.takeFirst();
            nfree = p.free_threads.count();
        }
    }

    //start the background job
    t.d->setFunction(function);
    
    //now clean up the pool, if needed
    if (nfree > 5)
    {
        MutexLocker lkr( &(p.m) );
        
        while (p.free_threads.count() > 5)
        {
            p.free_threads.takeFirst();
        }
    }
    
    return t;
}
