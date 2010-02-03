/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#include <QThread>
#include <QThreadStorage>
#include <QHash>

#include "forages.h"
#include "mutex.h"
#include "waitcondition.h"

#include "Siren/errors.h"

struct ForAgesState
{
    ForAgesState() : ID(0), is_interupted(false), is_paused(false)
    {}
    
    ~ForAgesState()
    {}

    int ID;
    bool is_interupted;
    bool is_paused;
};

struct GlobalForAgesState
{
    GlobalForAgesState()
    {}
    
    ~GlobalForAgesState()
    {}
    
    ForAgesState global_state;
    QThreadStorage<ForAgesState*> thread_state;

    QHash<const QThread*,ForAgesState*> thread_states;

    QMutex pause_mutex;
    QWaitCondition pause_waiter;
};

int last_id(0);

Q_GLOBAL_STATIC( GlobalForAgesState, globalForAgesState )

namespace Siren
{
    /** Call this function to register the current thread with for-ages.
        Note that only QThread-based threads can be registered */
    int SIREN_EXPORT register_this_thread()
    {
        GlobalForAgesState *s = globalForAgesState();

        QThread *t = QThread::currentThread();
        
        if (s and t)
        {
            QMutexLocker lkr( &(s->pause_mutex) );
            
            if (s->thread_states.contains(t))
                return s->thread_states[t]->ID;
            
            s->thread_state.setLocalData( new ForAgesState() );
            s->thread_states[t] = s->thread_state.localData();
            ++last_id;
            s->thread_states[t]->ID = last_id;
            
            return last_id;
        }
        else
            return 0;
    }

    /** Unregister the current thread (which must be based on QThread) */
    void SIREN_EXPORT unregister_this_thread()
    {
        GlobalForAgesState *s = globalForAgesState();

        QThread *t = QThread::currentThread();
        
        if (s and t)
        {
            QMutexLocker lkr( &(s->pause_mutex) );
            
            if (not s->thread_states.contains(t))
                return;
            
            bool is_paused = s->thread_states[t]->is_paused;
            
            s->thread_state.setLocalData(0);
            s->thread_states.remove(t);
            
            if (is_paused and not s->global_state.is_paused)
                //wake the thread up
                s->pause_waiter.wakeAll();
        }
    }

    void throwInteruptedError()
    {
        throw Siren::interupted( QObject::tr(
                "for-ages is over. This thread has been interupted."), CODELOC );
    }

    /** Pause all threads that use for_ages or for_ages mutexes */
    void SIREN_EXPORT pause_for_ages()
    {
        GlobalForAgesState *s = globalForAgesState();
    
        if (s)
        {
            QMutexLocker lkr( &(s->pause_mutex) );
            
            s->global_state.is_paused = true;
            
            foreach (const QThread *thread, s->thread_states.keys())
            {
                s->thread_states[thread]->is_paused = true;
            }
        }
    }
    
    /** Pause the thread 'thread' */
    void SIREN_EXPORT pause_for_ages(const QThread *thread)
    {
        GlobalForAgesState *s = globalForAgesState();

        if (s and thread)
        {
            QMutexLocker lkr( &(s->pause_mutex) );
            
            s->global_state.is_paused = true;
            
            if (s->thread_states.contains(thread))
            {
                s->thread_states[thread]->is_paused = true;
            }
        }
    }

    /** Play (wake) all threads that are paused */
    void SIREN_EXPORT play_for_ages()
    {
        GlobalForAgesState *s = globalForAgesState();
        
        if (s)
        {
            QMutexLocker lkr( &(s->pause_mutex) );
            
            s->global_state.is_paused = false;
            
            foreach (const QThread *thread, s->thread_states.keys())
            {
                s->thread_states[thread]->is_paused = false;
            }
            
            s->pause_waiter.wakeAll();
        }
    }
    
    /** Play (wake) the thread 'thread' if it is paused */
    void SIREN_EXPORT play_for_ages(const QThread *thread)
    {
        GlobalForAgesState *s = globalForAgesState();
        
        if (s and thread)
        {
            QMutexLocker lkr( &(s->pause_mutex) );
            
            if (s->thread_states.contains(thread))
            {
                if (s->thread_states[thread]->is_paused)
                {
                    s->thread_states[thread]->is_paused = false;
                    
                    //have to wake everyone, but everyone but the 
                    //desired thread will see that they are still 
                    //paused so will go back to sleep
                    s->pause_waiter.wakeAll();
                }
            }
        }
    }

    /** Tell all threads that for_ages is over. This will interupt
        any thread that is blocking on a for_ages, causing it to
        throw a Siren::interupted exception */
    void SIREN_EXPORT end_for_ages()
    {
        GlobalForAgesState *s = globalForAgesState();
        
        if (s)
        {
            QMutexLocker lkr( &(s->pause_mutex) );
            
            s->global_state.is_interupted = true;
            
            foreach (const QThread *thread, s->thread_states.keys())
            {
                s->thread_states[thread]->is_interupted = true;
            }
            
            //wake everyone now in case anyone is paused
            s->pause_waiter.wakeAll();
        }
    }
    
    /** Tell the thread 'thread' that for_ages is over. This 
        iwll interupt this thread if it is blocking on a for_ages,
        causing it to throw a Siren::interupted exception */
    void SIREN_EXPORT end_for_ages(const QThread *thread)
    {
        GlobalForAgesState *s = globalForAgesState();
        
        if (s and thread)
        {
            QMutexLocker lkr( &(s->pause_mutex) );
            
            if (s->thread_states.contains(thread))
            {
                s->thread_states[thread]->is_interupted = true;
                s->pause_waiter.wakeAll();
            }
        }
    }

    /** This function does nothing, unless the end of for-ages
        has been signalled, in which case it will throw a
        Siren::interupted exception. This allows you to put
        checks in your code before long or intensive calculations
        that will allow the code to be exited by signalling
        the end of for-ages by called end_for_ages()
    */
    void SIREN_EXPORT check_for_ages()
    {
        GlobalForAgesState *s = globalForAgesState();
        
        if (not s)
            return;
        
        else if (s->thread_state.hasLocalData())
        {
            if (s->thread_state.localData()->is_interupted)
                throwInteruptedError();

            else if (s->thread_state.localData()->is_paused)
            {
                //we may have been paused - we need to
                //check again, protected by a mutex, to ensure
                //that we are not interupted or unpaused before
                //we actually go to sleep
            
                QMutexLocker lkr( &(s->pause_mutex) );
                
                if (s->thread_state.localData()->is_interupted)
                    throwInteruptedError();

                else if (s->thread_state.localData()->is_paused)
                {
                    s->pause_waiter.wait( &(s->pause_mutex) );
                    
                    //we need to check again in case we've been
                    //sent back to sleep or interupted
                    lkr.unlock();
                    check_for_ages();
                }
            }
        }
        else
        {
            if (s->global_state.is_interupted)
                throwInteruptedError();
                
            else if (s->global_state.is_paused)
            {
                QMutexLocker lkr( &(s->pause_mutex) );
                
                if (s->global_state.is_interupted)
                    throwInteruptedError();
                
                else if (s->global_state.is_paused)
                {
                    s->pause_waiter.wait( &(s->pause_mutex) );

                    //we need to check again in case we've been
                    //sent back to sleep or interupted
                    lkr.unlock();
                    check_for_ages();
                }
            }
        }
    }

    /** This is a simple function that always returns 'true'.
        However, once the end for-ages has been signalled, this
        will throw a Siren::interupted exception. You can use
        this to create infinite loops that will loop for-ages, e.g.
        
        while (for_ages())
        {
            //do stuff
        }
        
        This loop will continue until end_for_ages() is called
    */
    bool SIREN_EXPORT for_ages()
    {
        check_for_ages();
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

} // end of namespace Siren
