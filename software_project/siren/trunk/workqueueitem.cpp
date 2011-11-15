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

#include "Siren/detail/workqueueitemdata.h"
#include "Siren/detail/promisedata.h"
#include "Siren/detail/workqueuedata.h"

using namespace Siren;
using namespace Siren::detail;

//////////////
////////////// Implementation of WorkQueueItemData
//////////////

/** Construct the item for the job handled by the passed promise,
    which is queued on the passed WorkQueue */
WorkQueueItemData::WorkQueueItemData(const Promise &promise,
                                     const WorkQueue &queue)
{
    promise_data = promise.d;
    parent_queue = queue.d;
    submission_time = DateTime::current();
}

/** Destructor */
WorkQueueItemData::~WorkQueueItemData()
{
    //tell the promise that the job has aborted as the
    //WorkQueueItem has been lost
    Promise(promise_data.lock()).abort();
}

/** Return the WorkPacket associated with this WorkQueueItem, or 
    None if the WorkPacket no longer exists */
Obj WorkQueueItemData::workPacket()
{
    MutexLocker lkr(&m);
    return Promise(promise_data.lock()).workPacket();
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

/** Tell the job that it should be cancelled */
void WorkQueueItemData::cancelJob()
{
    Promise p;
    
    {
        MutexLocker lkr(&m);
        p = Promise(promise_data.lock());
    }
    
    p.cancel();
}

/** Tell the job that it has been paused */
void WorkQueueItemData::pauseJob()
{
    Promise p;
    
    {
        MutexLocker lkr(&m);
        p = Promise(promise_data.lock());
    }
    
    p.pause();
}

//////////////
////////////// Implementation of WorkQueueItem
//////////////

/** Null constructor */
WorkQueueItem::WorkQueueItem()
{}

/** Construct the WorkItem that corresponds to the job behind the 
    passed Promise from the passed WorkQueue */
WorkQueueItem::WorkQueueItem(const Promise &promise,
                             const WorkQueue &queue)
{
    d.reset( new WorkQueueItemData(promise,queue) );
    d->self = d;
}

/** Copy constructor */              
WorkQueueItem::WorkQueueItem(const WorkQueueItem &other)
              : d(other.d)
{}

/** Destructor */
WorkQueueItem::~WorkQueueItem()
{}

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

