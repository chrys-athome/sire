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
#include "Siren/static.h"

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
            
            int sessionID();
            int threadID();

            void setFunction( function<void ()> func );

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
            function<void ()> func_ptr;
            
            /** Whether or not we should exit the thread */
            bool should_quit;
        };
    
        /** This class provides a simple pool of threads */
        class ThreadPool : public noncopyable
        {
        public:
            ThreadPool();
            ~ThreadPool();

            exp_shared_ptr<ThreadData>::Type getThread();
            void returnThread(const exp_shared_ptr<ThreadData>::Type &ptr);
        
        private:
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
    MutexLocker lkr(&m);
    QThread::start();
    waiter.wait(&m);
}

/** Destructor */
ThreadData::~ThreadData()
{
    this->quit();
    
    //give the thread 3 seconds to finish...
    int n = 0;
    while (not QThread::wait(1000))
    {
        n += 1;
        
        if (n > 3)
        {
            sirenDebug() << "WARNING: Had to terminate thread" << thread_id << session_id;
            QThread::terminate();
            break;
        }

        for_ages::end(thread_id);
    }
}

/** Tell this thread to quit as soon as it can */
void ThreadData::quit()
{
    MutexLocker lkr(&m);
    should_quit = true;
    
    if (thread_id != 0)
        for_ages::end( thread_id );
        
    waiter.wakeAll();
}

/** Return the session ID of this run */
int ThreadData::sessionID()
{
    MutexLocker lkr(&m);
    return session_id;
}

/** Return the thread ID */
int ThreadData::threadID()
{
    MutexLocker lkr(&m);
    return thread_id;
}

/** Set the function that should be run by this thread */
void ThreadData::setFunction( function<void ()> func )
{
    MutexLocker lkr(&m);
    
    if (func_ptr)
        throw Siren::program_bug( String::tr(
                "It should not be possible to set the function of an active thread!"),
                    CODELOC );

    session_id += 1;
    err = None();
    func_ptr = func;

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
    waiter.wakeAll();

    if (should_quit)
    {
        //quitted the thread before it even started!
        sirenDebug() << "Thread quit before started!";
        return;
    }

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
                    func_ptr();
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
            
            if (not should_quit)
            {
                waiter.wait(&m);
            }
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
    func_ptr = 0;
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
    List<exp_shared_ptr<ThreadData>::Type>::Type threads = free_threads + busy_threads;
    lkr.unlock();

    for (List<exp_shared_ptr<ThreadData>::Type>::iterator it = threads.begin();
         it != threads.end();
         ++it)
    {
        (*it)->quit();
    }
    
    for (List<exp_shared_ptr<ThreadData>::Type>::iterator it = threads.begin();
         it != threads.end();
         ++it)
    {
        (*it)->wait(1000);
    }
}

void ThreadPool::returnThread(const exp_shared_ptr<ThreadData>::Type &ptr)
{
    MutexLocker lkr(&m);
    busy_threads.append(ptr);

    //now clean up the pool, if needed
    if (busy_threads.count() > 5)
    {
        List<exp_shared_ptr<ThreadData>::Type>::MutableIterator it( busy_threads );
        
        while (it.hasNext())
        {
            it.next();
            
            if (it.value()->hasFinished())
            {
                free_threads.append(it.value());
                it.remove();
            }
        }
    }

    while (free_threads.count() > 5)
    {
        free_threads.takeFirst();
    }
}

exp_shared_ptr<ThreadData>::Type ThreadPool::getThread()
{
    MutexLocker lkr(&m);
    
    if (free_threads.isEmpty())
    {
        //have any of the active threads finished?
        List<exp_shared_ptr<ThreadData>::Type>::MutableIterator it(busy_threads);
        
        while (it.hasNext())
        {
            it.next();
            
            if ( it.value()->hasFinished() )
            {
                exp_shared_ptr<ThreadData>::Type t = it.value();
                it.remove();
                return t;
            }
        }
        
        //there were no free threads, so create a new thread
        return exp_shared_ptr<ThreadData>::Type( new ThreadData() );
    }
    else
    {
        return free_threads.takeFirst();
    }
}

//////////
////////// Implementation of Thread
//////////

SIREN_STATIC( ThreadPool, pool );

/** Construct a null Thread */
Thread::Thread()
{}

/** Copy constructor */
Thread::Thread(const Thread &other) : d(other.d)
{}

/** Destructor */
Thread::~Thread()
{
    //return the underlying threads to the pool
    exp_shared_ptr<ThreadPool>::Type p = pool();

    if (p)
    {
        for (List<exp_shared_ptr<ThreadData>::Type>::const_iterator
                                it = d.constBegin();
             it != d.constEnd();
             ++it)
        {
            if (it->unique())
            {
                p->returnThread( *it );
            }
        }
    }
}

/** Copy assignment operator */
Thread& Thread::operator=(const Thread &other)
{
    if (this == &other)
        return *this;

    if (not d.isEmpty())
    {
        exp_shared_ptr<ThreadPool>::Type p = pool();
    
        if (p)
        {
            for (List<exp_shared_ptr<ThreadData>::Type>::const_iterator 
                                                        it = d.constBegin();
                 it != d.constEnd();
                 ++it)
            {
                if (it->unique())
                    p->returnThread(*it);
            }
        }
        
        d.clear();
    }

    d = other.d;

    return *this;
}

/** Comparison operator */
bool Thread::operator==(const Thread &other) const
{
    return d == other.d;
}

/** Comparison operator */
bool Thread::operator!=(const Thread &other) const
{
    return not operator==(other);
}

/** Return whether or not this thread is null */
bool Thread::isNull()
{
    return d.isEmpty();
}

/** Start a thread to run the passed function. The thread will be started
    and run as soon as possible */
Thread Thread::run( function<void ()> func )
{
    Thread t;
    
    exp_shared_ptr<ThreadPool>::Type p = pool();
    
    if (not p)
        throw Siren::invalid_state( String::tr(
                "Cannot create a thread as the global ThreadPool is not available. "
                "Either Siren::init(...) has not been called, or you are trying to "
                "create a thread while the application is shutting down!"), CODELOC );
                
    exp_shared_ptr<ThreadData>::Type thread = p->getThread();
    thread->setFunction(func);

    t.d.append(thread);
    
    return t;
}

/** Start 'nthreads' threads, all running the passed function. The passed function
    must accept two integer arguments. These are the ID number of the thread
    running the function (0 to nthreads-1), and the total number of threads */
Thread Thread::run( function<void (int,int)> func, int nthreads )
{
    if (nthreads <= 0)
        nthreads = 1;

    Thread t;
    
    exp_shared_ptr<ThreadPool>::Type p = pool();
    
    if (not p)
        throw Siren::invalid_state( String::tr(
                "Cannot create a thread as the global ThreadPool is not available. "
                "Either Siren::init(...) has not been called, or you are trying to "
                "create a thread while the application is shutting down!"), CODELOC );

    for (int i=0; i<nthreads; ++i)
    {
        exp_shared_ptr<ThreadData>::Type thread = p->getThread();
        thread->setFunction( bind(func, i, nthreads) );
        
        t.d.append(thread);
    }
    
    return t;
}

/** Start a thread to run the passed function. The thread will be started
    and run as soon as possible */
Thread Thread::run( void (*func)() )
{
    return Thread::run( function<void ()>(func) );
}

/** Start 'nthreads' threads to run the passed function. The passed function
    must accept two integer arguments. These are the ID number of the thread
    running the function (0 to nthreads-1), and the total number of threads */
Thread Thread::run( void (*func)(int, int), int nthreads )
{
    return Thread::run( function<void (int, int)>(func), nthreads );
}

/** Return the number of sub threads running */
int Thread::nSubThreads()
{
    return d.count();
}

/** Abort the thread */
void Thread::abort()
{
    for (List<exp_shared_ptr<ThreadData>::Type>::const_iterator it = d.constBegin();
         it != d.constEnd();
         ++it)
    {
        (*it)->abort();
    }
}

/** Pause the thread */
void Thread::pause()
{
    for (List<exp_shared_ptr<ThreadData>::Type>::const_iterator it = d.constBegin();
         it != d.constEnd();
         ++it)
    {
        (*it)->pause();
    }
}

/** Play the thread */
void Thread::play()
{
    for (List<exp_shared_ptr<ThreadData>::Type>::const_iterator it = d.constBegin();
         it != d.constEnd();
         ++it)
    {
        (*it)->play();
    }
}

/** Return whether there are any errors */
bool Thread::isError()
{
    for (List<exp_shared_ptr<ThreadData>::Type>::const_iterator it = d.constBegin();
         it != d.constEnd();
         ++it)
    {
        if ((*it)->isError())
            return true;
    }

    return false;
}

/** Throw any error */
void Thread::throwError()
{
    for (List<exp_shared_ptr<ThreadData>::Type>::const_iterator it = d.constBegin();
         it != d.constEnd();
         ++it)
    {
        (*it)->throwError();
    }
}

/** Check for any errors */
void Thread::checkError()
{
    for (List<exp_shared_ptr<ThreadData>::Type>::const_iterator it = d.constBegin();
         it != d.constEnd();
         ++it)
    {
        (*it)->checkError();
    }
}
