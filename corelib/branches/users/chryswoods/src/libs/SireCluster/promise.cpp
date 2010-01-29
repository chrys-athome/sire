/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "promise.h"
#include "workpacket.h"
#include "node.h"
#include "frontend.h"

#include "SireMaths/rangenerator.h"

#include "Siren/waitcondition.h"
#include "Siren/mutex.h"
#include "Siren/forages.h"
#include "Siren/errors.h"

using namespace SireCluster;
using namespace Siren;

namespace SireCluster
{
    namespace detail
    {
        
        /** Private implementation of Promise */
        class PromiseData
        {
        public:
            PromiseData()
            {}
    
            ~PromiseData()
            {
                waiter.wakeAll();
            }

            boost::shared_ptr<PromiseWatcher> watcher;

            /** Wait condition used to wait for a result of the calculation */
            WaitCondition waiter;
    
            /** The initial state of the WorkPacket before the calculation.
                It will either be in this packet, or compressed into binary */
            WorkPacketPtr initial_packet;
            QByteArray initial_data;
    
            /** The final state of the WorkPacket */
            WorkPacketPtr result_packet;
            
            enum { IDLE=0, RUNNING=1, STOPPED=2, ABORTED=3, FINISHED=4 };
            
            /** The state of the job (waiting, running or finished) */
            int state;
            
            /** Whether or not running in the local thread is forbidden */
            bool local_running_forbidden;
        };

        class PromiseWatcher : public QThread
        {
        public:
            PromiseWatcher(const ActiveFrontend &f, const Promise &p) 
                    : QThread(), frontend(f), promise(p)
            {
                this->start();
            }
            
            ~PromiseWatcher()
            {
                this->wait();
            }
    
            /** The front end on which the calculation is running */
            ActiveFrontend frontend;
            
            /** The promise containing the work being processed */
            Promise promise;
            
        protected:
            void run()
            {
                if (promise.isNull())
                    return;
            
                Siren::setThreadString("Promise");
                Siren::register_this_thread();
                SireMaths::seed_qrand();

                WorkPacketPtr initial_packet;
                {
                    HandleLocker lkr(promise);
                    WorkPacketPtr initial_packet = promise.resource().initial_packet;
                }

                //submit the job
                frontend.startJob(initial_packet);
    
                //save a packed copy if that is desired
                if (initial_packet.read().shouldPack())
                {
                    HandleLocker lkr(promise);
                    promise.resource().initial_data = initial_packet.read().pack();
                    promise.resource().initial_packet = WorkPacketPtr();
                }
        
                //wait until the job has finished
                frontend.wait();
        
                //the node has finished - grab the result and
                //drop our copy of the node
                WorkPacketPtr my_result;
                {
                    HandleLocker lkr(promise);
                    
                    if (promise.resource().state == PromiseData::ABORTED)
                    {
                        //no need to process the result as the job was aborted
                        frontend = ActiveFrontend();
                        promise.resource().waiter.wakeAll();
                        return;
                    }
                    
                    promise.resource().result_packet = frontend.result();
        
                    frontend = ActiveFrontend();
        
                    if (promise.resource().result_packet.isNull())
                    {
                        //where did the result go???
                        promise.resource().result_packet = 
                            ErrorPacket( Siren::program_bug( QObject::tr(
                                "There was no result from the running calculation!!!"),
                                    CODELOC ) );
                    }
                    
                    if (promise.resource().state == PromiseData::RUNNING)
                        promise.resource().state = PromiseData::FINISHED;
                        
                    promise.resource().waiter.wakeAll();
                }
            }
        };
        
    } // end of namespace detail
} // end of namespace SireCluster;

using namespace SireCluster::detail;

/** This internal function is called to start running the job 
    on a remote resource connected to via 'f' */
void Promise::runRemote(ActiveFrontend f)
{
    HandleLocker lkr(*this);
    
    if (resource().state != PromiseData::IDLE)
        return;
        
    resource().state = PromiseData::RUNNING;
    
    resource().watcher.reset( new PromiseWatcher(f, *this) );
}

/** This internal function is called to run the job in the current thread */
void Promise::runLocal()
{
    HandleLocker lkr(*this);
    
    if (resource().state != PromiseData::IDLE)
        return;
        
    if (resource().local_running_forbidden)
        //we are not allowed to steal the current thread
        return;
        
    resource().state = PromiseData::RUNNING;
    
    WorkPacketPtr current_packet = resource().initial_packet;
    
    if (current_packet.read().shouldPack())
    {
        resource().initial_data = current_packet.read().pack();
        resource().initial_packet = WorkPacketPtr();
    }
    
    lkr.unlock();
    
    while (for_ages())
    {
        if (current_packet.read().hasFinished())
        {
            resource().state = PromiseData::FINISHED;
            resource().result_packet = current_packet;
            resource().waiter.wakeAll();
            return;
        }
            
        lkr.unlock();
        current_packet = current_packet.read().runChunk();
        lkr.relock();

        if (resource().state == PromiseData::ABORTED)
        {
            resource().waiter.wakeAll();
            return;
        }
        else
        {
            resource().result_packet = current_packet;
            
            if (current_packet.read().hasFinished())
            {
                resource().state = PromiseData::FINISHED;
                resource().waiter.wakeAll();
                return;
            }
            else if (resource().state == PromiseData::STOPPED)
            {
                resource().waiter.wakeAll();
                return;
            }
        }
    }
}

/** Construct a null promise */
Promise::Promise() : ImplementsHandle< Promise,Handles<PromiseData> >()
{}

/** Internal constructor */
Promise::Promise(const WorkPacket &workpacket, bool local_forbidden)
        : ImplementsHandle< Promise,Handles<PromiseData> >(new PromiseData())
{
    resource().initial_packet = workpacket;
    resource().local_running_forbidden = local_forbidden;
    
    if (workpacket.hasFinished())
        resource().state = PromiseData::FINISHED;
    else
        resource().state = PromiseData::IDLE;
}

/** Copy constructor */
Promise::Promise(const Promise &other) 
        : ImplementsHandle< Promise,Handles<PromiseData> >(other)
{}

/** Destructor */
Promise::~Promise()
{}

/** Copy assignment operator */
Promise& Promise::operator=(const Promise &other)
{
    Handles<PromiseData>::operator=(other);
    return *this;
}

/** Comparison operator */
bool Promise::operator==(const Promise &other) const
{
    return Handles<PromiseData>::operator==(other);
}

/** Comparison operator */
bool Promise::operator!=(const Promise &other) const
{
    return not Promise::operator==(other);
}

/** Abort this job */
void Promise::abort()
{
    if (isNull())
        return;
        
    HandleLocker lkr(*this);
    
    if (resource().state <= PromiseData::RUNNING)
    {
        resource().state = PromiseData::ABORTED;
        
        if (resource().watcher.get())
            resource().watcher->frontend.abortJob();
    }
}

/** Stop this job */
void Promise::stop()
{
    if (isNull())
        return;
        
    HandleLocker lkr(*this);
    
    if (resource().state <= PromiseData::RUNNING)
    {
        resource().state = PromiseData::STOPPED;
        
        if (resource().watcher.get())
            resource().watcher->frontend.stopJob();
    }
}

/** Wait for the job to have completed */
void Promise::wait()
{
    if (isNull())
        return;

    //as we would be idle, try to run the work in this thread
    this->runLocal();

    HandleLocker lkr(*this);
    
    if (resource().state <= PromiseData::RUNNING)
    {
        //we couldn't run locally - wait until the remote job has finished
        while (not this->sleep(resource().waiter, 2500))
        {
            check_for_ages();
            
            if (resource().state > PromiseData::RUNNING)
                //we've got the result!
                return;
        }
    }
}

/** Wait until the job has completed, or until 'timeout' milliseconds
    has passed. This returns whether or not the job has finished */
bool Promise::wait(int timeout)
{
    if (isNull())
        return true;
        
    HandleLocker lkr(*this);
    
    if (resource().state <= PromiseData::RUNNING)
    {
        //we still don't have the result
        this->sleep( resource().waiter, timeout );
        
        return resource().state > PromiseData::RUNNING;
    }
    else
        return true;
}

/** Return whether or not the calculation is still in progress */
bool Promise::isRunning()
{
    if (isNull())
        return false;
        
    HandleLocker lkr(*this);
    
    //if we are running, then we still have a handle on the node
    return not resource().state <= PromiseData::RUNNING;
}

/** Return whether or not the result is an error.
    This blocks until the result is available */
bool Promise::isError()
{
    if (isNull())
        return false;

    this->wait();
    
    HandleLocker lkr(*this);
    
    if (resource().result_packet.isNull())
        return false;
    else
        return resource().result_packet.read().isError();
}

/** Throw any errors associated with this promise - does
    nothing if there is no error */
void Promise::throwError()
{
    if (isNull())
    {
        this->wait();
        
        HandleLocker lkr(*this);
        
        if (not resource().result_packet.isNull())
            resource().result_packet.read().throwError();
    }
}

/** Return whether or not the job was stopped.
    This blocks until a result is available */
bool Promise::wasStopped()
{
    if (isNull())
        return false;

    this->wait();
    
    HandleLocker lkr(*this);
    return resource().state == PromiseData::STOPPED;
}

/** Return whether or not the job was aborted.
    If it was, then you can rerun the job using
    the initial state of the WorkPacket stored in
    this Promise. This blocks until a result is available */
bool Promise::wasAborted()
{
    if (isNull())
        return false;

    this->wait();
    
    HandleLocker lkr(*this);
    return resource().state == PromiseData::ABORTED;
}

/** Return the progress of the calculation */
float Promise::progress()
{
    if (isNull())
        return 100;

    HandleLocker lkr(*this);
    
    if (resource().state == PromiseData::RUNNING)
    {
        if (resource().watcher)
        {
            ActiveFrontend frontend = resource().watcher->frontend;
            lkr.unlock();
            
            float current_progress = frontend.progress();
            
            lkr.relock();
            
            if (resource().state == PromiseData::RUNNING)
                return current_progress;
        }
    }

    if (not resource().result_packet.isNull())
        return resource().result_packet.read().progress();
    else
        return 0;
}

/** Return the WorkPacket in the state it was in at the 
    start of the job. You can use this to restart jobs that
    failed in error, or were aborted, or if you just want
    to try to run the job again */
WorkPacketPtr Promise::input()
{
    if (isNull())
        return WorkPacketPtr();
        
    HandleLocker lkr(*this);
    
    if (resource().initial_data.isEmpty())
    {
        return resource().initial_packet;
    }
    else
    {
        return WorkPacket::unpack( resource().initial_data );
    }
}

/** Return an interim result of the calculation */
WorkPacketPtr Promise::interimResult()
{
    if (isNull())
        return WorkPacketPtr();
        
    HandleLocker lkr(*this);
    
    if (resource().state == PromiseData::RUNNING)
    {
        if (resource().watcher)
        {
            ActiveFrontend frontend = resource().watcher->frontend;
            lkr.unlock();
        
            WorkPacketPtr interim_result = frontend.interimResult();
        
            lkr.relock();
        
            if (resource().state == PromiseData::RUNNING)
                return interim_result;
        }
    }
        
    if (not resource().result_packet.isNull())
        return resource().result_packet;

    else if (resource().initial_data.isEmpty())
    {
        return resource().initial_packet;
    }
    else
    {
        return WorkPacket::unpack( resource().initial_data );
    }
}

/** Return the result of the work - this blocks until  
    the work has completed */
WorkPacketPtr Promise::result()
{
    if (isNull())
        return WorkPacketPtr();
        
    this->wait();
    
    HandleLocker lkr(*this);
    return resource().result_packet;
}

/** Run the passed work packet in the local thread, returning
    a Promise that contains the completed result */
Promise Promise::runLocal(const WorkPacket &workpacket)
{
    Promise promise(workpacket);
    promise.runLocal();
    return promise;
}
