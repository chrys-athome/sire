#ifndef SIREN_THREAD_H
#define SIREN_THREAD_H

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

#include "Siren/siren.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    class Promise;
    class WorkPacket;
    class WorkSpace;

    class Thread;
    class ThreadPool;

    namespace detail 
    { 
        class ThreadPoolData;

        class SystemThreadPool;
        class ProcessThreadPool;

        class ThreadRequest;
        class ThreadQueue;

        class CPUThread;
        class CPUThreads;
    }

    /** This is a pool of threads that are available to run tasks. This
        provides a user-specified number of threads, which can be asked
        to run a task. If all of the threads are busy, then the caller
        for the current task is blocked
    */
    class SIREN_EXPORT ThreadPool
    {
    public:
        ThreadPool(int n);
        ~ThreadPool();
    
        void reallocate(int n);
    
        Promise run(const WorkPacket &packet);
    
        Promise run(const WorkPacket &packet, int nthreads);
        Promise run(const WorkPacket &packet, WorkSpace &workspace, int nthreads);
    
        Promise allRun(const WorkPacket &packet);
        Promise allRun(const WorkPacket &packet, WorkSpace &workspace);
    
        void abort();
        void pause();
        void play();
    
        void stealThreads();
        void returnStolenThreads();
    
        void stealThreads(int n);
        void returnStolenThreads(int n);
    
        ThreadPool getPool(int n);
        Thread getThread();
    
        bool allAvailable();
        int nAvailable();
    
        void waitUntilAllAvailable();
        bool waitUntilAllAvailable(int ms);
        
        void waitUntilNAvailable(int n);
        bool waitUntilNAvailable(int n, int ms);
    
    private:
        exp_shared_ptr<detail::ThreadPoolData>::Type d;
        
    }; // end of class ThreadPool

    /** This is a separate thread of execution. A Siren thread provides 
        a thread that can run the passed function. While the thread runs,
        it returns a handle to the running code, which can be interupted,
        stopped, paused etc.
    */
    class SIREN_EXPORT Thread
    {
    public:
        enum ThreadState
        {
            BackGround = 1,
            ForeGround = 2
        };
        
        Thread();
        Thread(ThreadState state);
        
        ~Thread();
        
        Promise run(const WorkPacket &packet);
        Promise run(const WorkPacket &packet, WorkSpace &workspace);
        
        void abort();
        void pause();
        void play();
        
        bool available();
        
        void waitUntilAvailable();
        bool waitUntilAvailable(int ms); 
        
        void steal();
        void returnStolen();

        void toBackGround();
        void toForeGround();
        
    private:
        exp_shared_ptr<detail::ThreadPoolData>::Type d;
    
    }; // end of class Thread

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::ThreadPool )
SIREN_EXPOSE_CLASS( Sirne::Thread )

SIREN_END_HEADER

#endif // ifndef SIREN_THREAD
