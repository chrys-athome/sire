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
        class PromisePvt : public QThread
        {
        public:
            PromisePvt() : QThread()
            {}
    
            ~PromisePvt()
            {
                this->wait();
            }
    
            /** The front end on which the calculation is running */
            ActiveFrontend frontend;
    
            /** The resource lock */
            Mutex *datamutex;
    
            /** Wait condition used to wait for a result of the calculation */
            WaitCondition waiter;
    
            /** The initial state of the WorkPacket before the calculation.
                It will either be in this packet, or compressed into binary */
            WorkPacketPtr initial_packet;
            QByteArray initial_data;
    
            /** The final state of the WorkPacket */
            WorkPacketPtr result_packet;

        protected:
            void run()
            {
                Siren::setThreadString("Promise");
                Siren::register_this_thread();
                SireMaths::seed_qrand();
        
                MutexLocker lkr(datamutex);
        
                //get a local copy of the node
                ActiveFrontend my_frontend = frontend;
                lkr.unlock();
        
                //submit the job
                my_frontend.startJob(initial_packet);
    
                //save a packed copy if that is desired
                if (initial_packet.read().shouldPack())
                {
                    initial_data = initial_packet.read().pack();
                    initial_packet = WorkPacketPtr();
                }
        
                //wait until the job has finished
                my_frontend.wait();
        
                //the node has finished - grab the result and
                //drop our copy of the node
                WorkPacketPtr my_result = my_frontend.result();
        
                if (my_result.isNull())
                {
                    //where did the result go???
                    my_result = ErrorPacket( Siren::program_bug( QObject::tr(
                                "There was no result from the running calculation!!!"),
                                    CODELOC ) );
                }

                my_frontend = ActiveFrontend();
    
                //copy the result to the promise
                lkr.relock();
                frontend = ActiveFrontend();
                result_packet = my_result;
        
                //wake anyone waiting for a result
                waiter.wakeAll();
            }
        };
    } // end of namespace detail
} // end of namespace SireCluster;

using namespace SireCluster::detail;

/** Construct a null promise */
Promise::Promise() : ImplementsHandle< Promise,Handles<PromisePvt> >()
{}

/** Internal constructor called by WorkQueue that constructs a promise
    that is following the progress of the work in 'initial_packet' 
    as it is being processed on the frontend 'frontend'. */
Promise::Promise(const ActiveFrontend &frontend, const WorkPacket &initial_workpacket)
        : ImplementsHandle< Promise,Handles<PromisePvt> >(new PromisePvt())
{
    BOOST_ASSERT( not frontend.isNull() );
    
    resource().frontend = frontend;
    resource().initial_packet = initial_workpacket;
    resource().datamutex = resourceLock();
    
    //now start a background thread that submits the work and grabs the result
    //as soon as it is available
    resource().start();
}

/** Copy constructor */
Promise::Promise(const Promise &other) 
        : ImplementsHandle< Promise,Handles<PromisePvt> >(other)
{}

/** Destructor */
Promise::~Promise()
{}

/** Copy assignment operator */
Promise& Promise::operator=(const Promise &other)
{
    Handles<PromisePvt>::operator=(other);
    return *this;
}

/** Comparison operator */
bool Promise::operator==(const Promise &other) const
{
    return Handles<PromisePvt>::operator==(other);
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
    resource().frontend.abortJob();
}

/** Stop this job */
void Promise::stop()
{
    if (isNull())
        return;
        
    HandleLocker lkr(*this);
    resource().frontend.stopJob();
}

/** Wait for the job to have completed */
void Promise::wait()
{
    if (isNull())
        return;

    HandleLocker lkr(*this);
    
    if (resource().result_packet.isNull())
    {
        //we still don't have the result
        while (not this->sleep(resource().waiter, 2500))
        {
            if (not resource().result_packet.isNull())
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
    
    if (resource().result_packet.isNull())
    {
        //we still don't have the result
        this->sleep( resource().waiter, timeout );
        
        return not resource().result_packet.isNull();
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
    return not resource().frontend.isNull();
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
    
    if (resource().result_packet.isNull())
        return false;
    else
        return not resource().result_packet.read().hasFinished();
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
    
    if (resource().result_packet.isNull())
        return false;
    else
        return resource().result_packet.read().wasAborted();
}

/** Return the progress of the calculation */
float Promise::progress()
{
    if (isNull())
        return 100;

    HandleLocker lkr(*this);
    
    if (resource().result_packet.isNull())
    {
        ActiveFrontend frontend = resource().frontend;
        lkr.unlock();
        
        float current_progress = frontend.progress();
        
        lkr.relock();
        
        if (not resource().result_packet.isNull())
            //the result came in while we were getting the progress
            return resource().result_packet.read().progress();
        else
            return current_progress;
    }
    else
        return resource().result_packet.read().progress();
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
    
    if (not resource().result_packet.isNull())
        //we already have the final result!
        return resource().result_packet;
        
    else
    {
        ActiveFrontend frontend = resource().frontend;
        
        lkr.unlock();
        
        WorkPacketPtr interim_result = frontend.interimResult();
        
        lkr.relock();
        
        if (not resource().result_packet.isNull())
            //we got the final result while waiting for the interim result
            return resource().result_packet;
        else
            return interim_result;
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
    WorkPacketPtr initial_packet = workpacket;
    QByteArray initial_data;
    
    WorkPacketPtr current_packet = initial_packet;
    
    if (initial_packet.read().shouldPack())
    {
        initial_data = initial_packet.read().pack();
        initial_packet = WorkPacketPtr();
    }
        
    while (for_ages())
    {
        if (current_packet.read().hasFinished())
            break;
            
        current_packet = current_packet.read().runChunk();
    }
    
    return Promise(initial_packet, initial_data, current_packet);
}
