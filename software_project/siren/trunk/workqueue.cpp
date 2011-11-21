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

#include "Siren/promise.h"
#include "Siren/workpacket.h"
#include "Siren/workspace.h"
#include "Siren/workqueue.h"
#include "Siren/workqueueitem.h"
#include "Siren/exceptions.h"
#include "Siren/static.h"
#include "Siren/forages.h"
#include "Siren/thread.h"

#include "Siren/detail/workqueuedata.h"

#include <boost/bind.hpp>

using namespace Siren;
using namespace Siren::detail;

//////////
////////// Implementation of WorkQueueData
//////////

/** Constructor */
WorkQueueData::WorkQueueData() : noncopyable(), nthreads(0)
{}

/** Construct a WorkQueue that can manage up to 'nthreads' CPU threads */
WorkQueueData::WorkQueueData(int n) : noncopyable(), nthreads(n)
{
    if (nthreads < 0)
        nthreads = 0;
}

/** Destructor */
WorkQueueData::~WorkQueueData()
{}

/** Submit the passed WorkPacket for processing using 'n' workers. Return the 
    promise that will be used to get the result of processing, and to get a handle
    on the job */
Promise WorkQueueData::submit(const WorkPacket &packet, int n)
{
    MutexLocker lkr(&m);
    
    WorkQueue queue(self.lock());
    
    WorkQueueItem workitem(packet, queue, n);

    Promise promise(workitem);
    workitem.setPromise(promise);

    if (n <= nthreads)
    {
        waiting_jobs.append(workitem);
        waiter.wakeAll();
    }
    else
        blocked_jobs.append(workitem);
    
    return promise;
}

/** Submit the passed WorkPacket for processing using 'n' workers with the 
    passed WorkSpace used for inter-process communication. Return the 
    promise that will be used to get the result of processing, and to get a handle
    on the job */
Promise WorkQueueData::submit(const WorkPacket &packet, const WorkSpace &space, int n)
{
    MutexLocker lkr(&m);
    
    WorkQueue queue(self.lock());
    
    WorkQueueItem workitem(packet, space, queue, n);

    Promise promise(workitem);
    workitem.setPromise(promise);
    
    if (n <= nthreads)
    {
        waiting_jobs.append(workitem);
        waiter.wakeAll();
    }
    else
        blocked_jobs.append(workitem);
    
    return promise;
}

/** Return the number of running jobs */
int WorkQueueData::nRunning()
{
    MutexLocker lkr(&m);
    return running_jobs.count();
}

/** Return the number of waiting jobs */
int WorkQueueData::nWaiting()
{
    MutexLocker lkr(&m);
    return waiting_jobs.count();
}

/** Return the number of blocked jobs */
int WorkQueueData::nBlocked()
{
    MutexLocker lkr(&m);
    return blocked_jobs.count();
}

/** Return the number of completed jobs */
int WorkQueueData::nCompleted()
{
    MutexLocker lkr(&m);
    return completed_jobs.count();
}

/** Return a string representation of this queue */
String WorkQueueData::toString()
{
    MutexLocker lkr(&m);
    
    return String::tr("WorkQueue( nCPUs() == %1, nRunning() == %2, "
                      "nWaiting() == %3, nBlocked() == %4, nCompleted() == %5 )")
                        .arg(nthreads)
                        .arg(running_jobs.count())
                        .arg(waiting_jobs.count())
                        .arg(blocked_jobs.count())
                        .arg(completed_jobs.count());
}

//////////
////////// Implementation of WorkQueue
//////////

void manage_queue(WorkQueue queue)
{
    while (for_ages::loop())
    {
        sirenDebug() << "HELLO WORLD!!!\n" << queue.toString();
        for_ages::sleep(1);
    }
}

/** Construct a WorkQueue that manages no resources */
WorkQueue::WorkQueue() : d( new WorkQueueData() )
{
    d->self = d;
    Thread::run( boost::bind(manage_queue,*this) );
}

/** Construct a WorkQueue that wants to manage 'n' CPU threads */
WorkQueue::WorkQueue(int n) : d( new WorkQueueData(n) )
{
    d->self = d;
}

/** Internal constructor used to construct the WorkQueue from the shared pointer */
WorkQueue::WorkQueue(const exp_shared_ptr<detail::WorkQueueData>::Type &ptr)
          : d(ptr)
{}

/** Copy constructor */
WorkQueue::WorkQueue(const WorkQueue &other) : d(other.d)
{}

/** Destructor */
WorkQueue::~WorkQueue()
{}

/** Copy assignment operator */
WorkQueue& WorkQueue::operator=(const WorkQueue &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool WorkQueue::operator==(const WorkQueue &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool WorkQueue::operator!=(const WorkQueue &other) const
{
    return d.get() != other.d.get();
}

/** Return whether or not this WorkQueue is null */
bool WorkQueue::isNull() const
{
    return d.get() == 0;
}

/** Return a string representation of the WorkQueue - this gives
    a summary of the available resources, waiting jobs etc. */
String WorkQueue::toString() const
{
    if (not d)
        return String::tr("WorkQueue::null");
    else
        return d->toString();
}

/** Submit the passed WorkPacket to the queue for processing, requiring 'n'
    workers to process the packet */
Promise WorkQueue::submit(const WorkPacket &workpacket, int n)
{
    if (not d)
        throw Siren::invalid_state( String::tr(
                "You cannot submit the WorkPacket \"%1\" for processing on "
                "a null WorkQueue!").arg(workpacket.toString()), CODELOC );
    
    return d->submit(workpacket,n);
}

/** Submit the passed WorkPacket to the queue for processing, requiring 'n'
    workers that communicate using the passed WorkSpace */
Promise WorkQueue::submit(const WorkPacket &workpacket, WorkSpace &workspace, int n)
{
    if (not d)
        throw Siren::invalid_state( String::tr(
                "You cannot submit the WorkPacket \"%1\" for processing on "
                "a null WorkQueue!").arg(workpacket.toString()), CODELOC );

    return d->submit(workpacket,workspace,n);
}

/** Return the number of waiting jobs */
int WorkQueue::nWaiting() const
{
    if (d)
        return d->nWaiting();
    else
        return 0;
}

/** Return the number of running jobs */
int WorkQueue::nRunning() const
{
    if (d)
        return d->nRunning();
    else
        return 0;
}

/** Return the number of completed jobs */
int WorkQueue::nCompleted() const
{
    if (d)
        return d->nCompleted();
    else
        return 0;
}

/** Return the number of blocked jobs (ones that request more resources
    than this queue will ever have available!) */
int WorkQueue::nBlocked() const
{
    if (d)
        return d->nBlocked();
    else
        return 0;
}

/** Internal function called to tell the WorkQueue that the passed  
    job should be aborted (either cancelled, or, if running, stopped!) */
void WorkQueue::abort(const WorkQueueItem &item)
{
    throw Siren::incomplete_code("TODO", CODELOC);
}
