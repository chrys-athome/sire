#ifndef SIREN_DETAIL_WORKQUEUEDATA_H
#define SIREN_DETAIL_WORKQUEUEDATA_H

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

#include "Siren/workqueue.h"
#include "Siren/mutex.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    namespace detail
    {
        friend class WorkQueueData;
    
        class WorkQueueItem
        {
        public:
            WorkQueueItem();
            WorkQueueItem(const Promise &promise,
                          const WorkQueue &queue);
                          
            WorkQueueItem(const WorkQueueItem &other);
            
            ~WorkQueueItem();
            
            WorkQueueItem& operator=(const WorkQueueItem &other);
            
            bool operator==(const WorkQueueItem &other) const;
            bool operator!=(const WorkQueueItem &other) const;
            
            Obj workPacket() const;
            DateTime submissionTime() const;
            DateTime startTime() const;
            DateTime finishTime() const;
            
            Obj operator[](const String &string) const;
        
            Promise promise() const;
        
        private:
            exp_shared_ptr<detail::WorkQueueItemData>::Type d;
        };
    
        /** This class holds the metadata for a single job in the queue */
        class WorkQueueItemData : public noncopyable
        {
        public:
            WorkQueueItem(const Promise &promise,
                          const WorkQueue &queue);
            
            WorkQueueItem(const WorkQueueItem &other);
            
            ~WorkQueueItem();
            
            Obj workPacket() const;
            DateTime submissionTime() const;
            DateTime startTime() const;
            DateTime finishTime() const;
            
            Obj operator[](const String &string) const;
        
            Promise promise() const;
        
        protected:
            friend class WorkQueueData;
            void jobStarted();
            void jobFinished();
            void jobCancelled();
        
            friend class PromiseData;
            void cancelJob();
            void pauseJob();
        
        private:
            /** Mutex protecting access to the data of this object */
            Mutex mutex;
        
            /** Store the WorkPacket in the Promise, so if lose promise,
                then lose WorkPacket - the Promise is the handle to the job */
            exp_weak_ptr<PromiseData>::Type promise_data;

            /** This is the queue that is processing this item. If we lose
                the queue, then we cannot run this job */
            exp_weak_ptr<WorkQueueData>::Type parent_queue;
            
            /** The time the job was submitted (this WorkQueueItem was created) */
            DateTime submission_time;
            
            /** The time the queue told us the job was running */
            DateTime start_time;
            
            /** The time the queue told us that the job had finished.
                The result is added to the Promise attached to this item */
            DateTime finish_time;
        
            /** A set of other metadata that may be added and manipulated
                by the WorkScheduler responsible for scheduling this job */
            Hash<String,Obj>::Type mdata;
        
        }; // end of class WorkQueueItem
    
        /** This class provides the private implementation of WorkQueue */
        class WorkQueueData : public noncopyable
        {
        public:
            WorkQueueData();
            ~WorkQueueData();

            
        
        private:
            Mutex mutex;
            
            List<WorkQueueItem>::Type running_jobs;
            List<WorkQueueItem>::Type pending_jobs;
            List<WorkQueueItem>::Type cancelled_jobs;
            List<WorkQueueItem>::Type finished_jobs;
            
        };
    
    } // end of namespace detail

} // end of namespace Siren


SIREN_END_HEADER

#endif // ifndef SIREN_DETAIL_WORKQUEUEDATA_H
