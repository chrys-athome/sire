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
#include "Siren/exceptions.h"
#include "Siren/workpacket.h"
#include "Siren/workspace.h"
#include "Siren/obj.h"
#include "Siren/none.h"
#include "Siren/forages.h"
#include "Siren/siren.hpp"

namespace Siren
{
    namespace detail
    {
        /** This internal class represents a real, physical CPU thread. */
        class CPUThread : public QThread
        {
        public:
            ~CPUThread();
            
            void start();
            
            void process(const WorkPacket &workpacket);
            
            void process(const WorkPacket &workpacket, 
                         WorkSpace &workspace,
                         int id);
            
            Obj result();
            
            bool hasResult();
            bool finished();
            bool isBusy();
            
            void pause();
            void play();
            void abort();
            
            int ID();
            String description();
            
        protected:
            friend class CPUThreads;
            CPUThread();
            void run();
        
        private:
            /** Mutex used to seriliase access to the thread's state */
            Mutex mutex;
        
            /** Waitcondition used to pause the thread */
            WaitCondition thread_waiter;
        
            /** The WorkPacket to process */
            const WorkPacket *workpacket;
            
            /** The WorkSpace that may be used during processing */
            WorkSpace *workspace;
            
            /** The result of processing - this in "None" until processing has 
                completed */
            Obj reslt;
            
            /** Any truly unexpected error that occurred with this thread */
            Obj err;
            
            /** The ID of the thread in the group processing
                the WorkPacket */
            int packet_id;

            /** The name of the thread - this comes from the WorkPacket
                being processed */
            String thread_name;
            
            /** The ID of the thread */
            int thread_id;
            
            /** The worker thread ID if this thread is part of a group
                that is processing a WorkPacket */
            int worker_id;
        };
    
    } // end of namespace detail
    
} // end of namespace Siren

using namespace Siren;
using namespace Siren::detail;

///////////////
/////////////// Implementation of CPUThread
///////////////

/** Constructor - a CPUThread sits idle on a pool until it is needed */
CPUThread::CPUThread() 
          : QThread(), workpacket(0), workspace(0),
            packet_id(0), thread_id(-1), worker_id(0)
{
    CPUThread::start();
}

/** Destructor */
CPUThread::~CPUThread()
{
    QThread::wait();
}

/** Start this thread */
void CPUThread::start()
{
    if (this->isRunning())
        return;

    MutexLocker lkr(&mutex);
    QThread::start();
    thread_waiter.wait(&mutex);
}

/** Process the passed WorkPacket - this raises an exception if this
    thread is already busy processing another WorkPacket */
void CPUThread::process(const WorkPacket &packet)
{
    MutexLocker lkr(&mutex);
    
    if (workpacket != 0)
        throw Siren::program_bug( String::tr(
                "A busy CPUThread has been asked to process the workpacket %1.")
                    .arg(packet.toString()), CODELOC );
                    
    else if (not reslt.isNone())
        throw Siren::program_bug( String::tr(
                "A completed CPUThread has not yet had its result retrieved!",
                    CODELOC );

    workpacket = &packet;
    workspace = 0;
    worker_id = -1;
    reslt = None();
    thread_name = String::tr("CPUThread(%1) processing \"%1\"")
                    .arg(thread_id).arg(packet.toString());
    
    thread_waiter.wakeAll();
}

/** Process the passed WorkPacket, as part of a worker thread group.
    This thread uses the passed WorkSpace to communicate with the other
    threads, and is given worker id "id". This raises an exception
    if this thread is already busy processing another WorkPacket */
void CPUThread::process(const WorkPacket &packet, 
                        WorkSpace &space, int id)
{
    MutexLocker lkr(&mutex);
    
    if (workpacket != 0)
        throw Siren::program_bug( String::tr(
                "A busy CPUThread has been asked to process the workpacket %1.")
                    .arg(packet.toString()), CODELOC );

    else if (not reslt.isNone())
        throw Siren::program_bug( String::tr(
                "A completed CPUThread has not yet had its result retrieved!",
                    CODELOC );

    workpacket = &packet;
    workspace = &space;
    worker_id = id;
    reslt = None();
    err = None();
    thread_name = String::tr("CPUThread(%1, worker %2) processing \"%3\"")
                    .arg(thread_id).arg(worker_id).arg(packet.toString());
    
    thread_waiter.wakeAll();
}

/** Pause this thread - this sends a for_ages::pause to this thread */
void CPUThread::pause()
{
    for_ages::pause(thread_id);
}

/** Play this thread - this sends a for_ages::play to this thread */
void CPUThread::play()
{
    for_ages::play(thread_id);
}

/** Abort running in this thread - this sends a for_ages::end to this thread,
    which will cause any blocked operations to exit, and an interupted_thread
    exception to be raised */
void CPUThread::abort()
{
    for_ages::end(thread_id);
}

/** Return the ID number of this thread. This is "-1" if this thread
    hasn't been started */
int CPUThread::ID()
{
    MutexLocker lkr(&mutex);
    return thread_id;
}

/** Return the description of this thread - this is a string describing
    what the thread is currently doing */
String CPUThread::description()
{
    MutexLocker lkr(&mutex);
    return thread_name;
}

/** The actual code running in the background thread */
void CPUThread::run()
{
    MutexLocker lkr(&mutex);
   
    thread_id = for_ages::registerThisThread();
    
    thread_waiter.wakeAll();

    try
    {
        while (for_ages::loop())
        {
            if (workpacket == 0)
                thread_waiter.sleep(&mutex);
            
            Obj result;
            lkr.unlock();
            
            try
            {
                for_ages::setThisThreadName(thread_name);

                if (workspace == 0)
                    result = workpacket->run();
            
                else if (worker_id >= 0)
                    result = workpacket->run(*workspace, worker_id);
            
                else
                    result = workpacket->run(*workspace);
            }
            catch(const Siren::Exception &e)
            {
                result = e;
            }
            catch(const std::exception &e)
            {
                result = standard_exception(e,CODELOC);
            }
            catch(...)
            {
                result = unknown_exception(CODELOC);
            }
            
            lkr.relock();
            
            for_ages::setThisThreadName( String() );

            workpacket = 0;
            workspace = 0;
            worker_id = -1;
            reslt = result;
        }
    }
    catch(const Siren::interupted_thread&)
    {
        //this is just the signal to stop the thread
        // - it is not an error
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

    for_ages::unregisterThisThread();
    thread_id = -1;
}
