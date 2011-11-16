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

#include "Siren/workqueueitem.h"
#include "Siren/none.h"
#include "Siren/promise.h"
#include "Siren/workqueue.h"
#include "Siren/workpacket.h"
#include "Siren/workspace.h"
#include "Siren/exceptions.h"

#include "Siren/detail/workqueueitemdata.h"
#include "Siren/detail/promisedata.h"
#include "Siren/detail/workqueuedata.h"

using namespace Siren;
using namespace Siren::detail;

//////////////
////////////// Implementation of WorkQueueItemData
//////////////

/** Construct the item representing the processing of the passed WorkPacket
    using 'n' worker threads, submitted to the passed queue */
WorkQueueItemData::WorkQueueItemData(const WorkPacket &packet,
                                     const WorkQueue &queue, int n)
                  : workpacket(packet), workspace(0), parent_queue(queue.d), 
                    nworkers(n)
{
    submission_time = DateTime::current();

    if (nworkers <= 0)
        nworkers = 1;
}

/** Construct the item representing the processing of the passed WorkPacket
    using the passed WorkSpace, using 'n' worker threads, submitted to the 
    passed queue */
WorkQueueItemData::WorkQueueItemData(const WorkPacket &packet,
                                     const WorkSpace &space,
                                     const WorkQueue &queue, int n)
                  : workpacket(packet), workspace(new WorkSpace(space)),
                    parent_queue(queue.d), nworkers(n)
{
    submission_time = DateTime::current();
    
    if (nworkers <= 0)
        nworkers = 1;
}

/** Destructor */
WorkQueueItemData::~WorkQueueItemData()
{
    //tell the promise that the job has aborted as the
    //WorkQueueItem has been lost
    Promise(promise_data.lock()).abort();

    //delete the WorkSpace
    delete workspace;
}

/** Internal function used to set the Promise that is associated with this job */
void WorkQueueItemData::setPromise(const Promise &promise)
{
    promise_data = promise.d;
}

/** Return the WorkPacket associated with this WorkQueueItem, or 
    None if the WorkPacket no longer exists */
Obj WorkQueueItemData::workPacket()
{
    MutexLocker lkr(&m);
    return workpacket;
}

/** Return the WorkSpace associated with this WorkQueueItem, or raise
    an exception if a WorkSpace is not available */
WorkSpace WorkQueueItemData::workSpace()
{
    MutexLocker lkr(&m);
    
    if (not workspace)
        throw Siren::invalid_state( String::tr(
                "You cannot request a WorkSpace for the processing of the "
                "WorkPacket \"%1\" when no such WorkSpace was provided!")
                    .arg(workpacket.toString()), CODELOC );
                    
    return *workspace;
}

/** Return whether or not this WorkPacket comes with a WorkSpace */
bool WorkQueueItemData::hasWorkSpace()
{
    MutexLocker lkr(&m);
    return workspace != 0;
}

/** Return the number of worker threads required for this job */
int WorkQueueItemData::nWorkers()
{
    MutexLocker lkr(&m);
    return nworkers;
}

/** Return the original submission time of the job, or a null
    datetime if this is a null item */
DateTime WorkQueueItemData::submissionTime()
{
    MutexLocker lkr(&m);
    return submission_time;
}

/** Return the time the job started, or null if the job has not started */
DateTime WorkQueueItemData::startTime()
{
    MutexLocker lkr(&m);
    return start_time;
}

/** Return the time the job ended, or null if the job hasn't ended */
DateTime WorkQueueItemData::finishTime()
{
    MutexLocker lkr(&m);
    return finish_time;
}

/** Return the object associated with the key 'key', or None if 
    there is no object with this key */
Obj WorkQueueItemData::operator[](const String &string)
{
    MutexLocker lkr(&m);
    return mdata.value(string, Obj());
}

/** Return the promise associated with this item, or a null
    promise if the Promise is lost or this is a null item */
Promise WorkQueueItemData::promise()
{
    MutexLocker lkr(&m);
    return Promise(promise_data.lock());
}

/** Return the WorkQueue on which this item is queueued. This will
    return a null WorkQueue if the queue is lost or this item is invalid */
WorkQueue WorkQueueItemData::queue()
{
    MutexLocker lkr(&m);
    return WorkQueue(parent_queue.lock());
}

/** Return the WorkQueueItem associated with this object */
WorkQueueItem WorkQueueItemData::workQueueItem()
{
    MutexLocker lkr(&m);
    return WorkQueueItem(self.lock());
}

/** Internal function used to tell this item that the job has been
    started */
void WorkQueueItemData::jobStarted()
{
    Promise p;

    {
        MutexLocker lkr(&m);
        p = Promise(promise_data.lock());
        start_time = DateTime::current();
    }
    
    p.jobStarted();
}

/** Internal function used to tell this item that the job has finished */
void WorkQueueItemData::jobFinished(const Obj &result)
{
    Promise p;

    {
        MutexLocker lkr(&m);
        p = Promise(promise_data.lock());
        finish_time = DateTime::current();
    }
    
    p.jobFinished(result);
}

/** Internal function used to tell this item that the job has been cancelled */
void WorkQueueItemData::jobCancelled()
{
    Promise p;

    {
        MutexLocker lkr(&m);
        p = Promise(promise_data.lock());
        finish_time = DateTime::current();
    }
    
    p.jobCancelled();
}

/** Internal function used to relay the message to the queue that this job
    should be aborted */
void WorkQueueItemData::abort()
{
    MutexLocker lkr(&m);
    
    WorkQueue q(parent_queue.lock());
    
    if (not q.isNull())
    {
        q.abort( WorkQueueItem(self.lock()) );
    }
}

//////////////
////////////// Implementation of WorkQueueItem
//////////////

/** Null constructor */
WorkQueueItem::WorkQueueItem()
{}

/** Construct from the passed pointer */
WorkQueueItem::WorkQueueItem(const exp_shared_ptr<WorkQueueItemData>::Type &ptr)
              : d(ptr)
{}

/** Construct the item representing the processing of the passed WorkPacket
    using 'n' worker threads, submitted to the passed queue */
WorkQueueItem::WorkQueueItem(const WorkPacket &workpacket, 
                             const WorkQueue &workqueue, int nworkers)
{
    d.reset( new WorkQueueItemData(workpacket,workqueue,nworkers) );
    d->self = d;
}

/** Construct the item representing the processing of the passed WorkPacket
    using the passed WorkSpace, using 'n' worker threads, submitted to the 
    passed queue */
WorkQueueItem::WorkQueueItem(const WorkPacket &workpacket, const WorkSpace &workspace,
                             const WorkQueue &workqueue, int nworkers)
{
    d.reset( new WorkQueueItemData(workpacket,workspace,workqueue,nworkers) );
    d->self = d;
}

/** Copy constructor */              
WorkQueueItem::WorkQueueItem(const WorkQueueItem &other)
              : d(other.d)
{}

/** Destructor */
WorkQueueItem::~WorkQueueItem()
{}

/** Internal function used to relay the message that the job should be aborted */
void WorkQueueItem::abort()
{
    if (d)
        d->abort();
}

/** Internal function called by WorkQueue to tell this item which Promise
    has been assigned to the job */
void WorkQueueItem::setPromise(const Promise &promise)
{
    if (d)
        d->setPromise(promise);
}

/** Copy assignment operator */
WorkQueueItem& WorkQueueItem::operator=(const WorkQueueItem &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool WorkQueueItem::operator==(const WorkQueueItem &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool WorkQueueItem::operator!=(const WorkQueueItem &other) const
{
    return not operator==(other);
}

/** Return the WorkPacket that is processed as part of this WorkQueueItem */
Obj WorkQueueItem::workPacket() const
{
    if (d)
        return d->workPacket();
    else
        return None();
}

/** Return the submission time for this packet. This is a null
    time if the job has not been submitted */
DateTime WorkQueueItem::submissionTime() const
{
    if (d)
        return d->submissionTime();
    else
        return DateTime();
}

/** Return the start time for the job. This is a null DateTime
    if the job has not started */
DateTime WorkQueueItem::startTime() const
{
    if (d)
        return d->startTime();
    else
        return DateTime();
}

/** Return the time the job finished. This is a null DateTime if
    the job has not finished */
DateTime WorkQueueItem::finishTime() const
{
    if (d)
        return d->finishTime();
    else
        return DateTime();
}

/** Return the value of the metadata item with key 'key' - this is equal
    to 'None' if there is no item with that key */
Obj WorkQueueItem::operator[](const String &key) const
{
    if (d)
        return d->operator[](key);
    else
        return None();
}

/** Return the promise associated with this WorkQueueItem. This
    is a null Promise if this is a null WorkQueueItem */
Promise WorkQueueItem::promise() const
{
    if (d)
        return d->promise();
    else
        return Promise();
}

