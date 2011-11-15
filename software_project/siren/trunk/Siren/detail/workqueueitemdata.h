#ifndef SIREN_DETAIL_WORKQUEUEITEMDATA_H
#define SIREN_DETAIL_WORKQUEUEITEMDATA_H

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
#include "Siren/mutex.h"
#include "Siren/datetime.h"
#include "Siren/string.h"
#include "Siren/obj.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    class Promise;
    class WorkQueue;
    class WorkQueueItem;

    namespace detail
    {
        class WorkQueueData;
        class PromiseData;
    
        /** This class holds the metadata for a single job in the queue */
        class WorkQueueItemData : public noncopyable
        {
        public:
            WorkQueueItemData(const WorkPacket &packet,
                              const WorkQueue &queue,
                              int n);
            
            WorkQueueItemData(const WorkPacket &packet,
                              WorkSpace &workspace,
                              const WorkQueue &queue,
                              int n);
            
            ~WorkQueueItemData();
            
            Obj workPacket();
            DateTime submissionTime();
            DateTime startTime();
            DateTime finishTime();
            
            Obj operator[](const String &string);
        
            Promise promise();
        
        protected:
            friend class WorkQueueItem;
        
            friend class WorkQueueData;
            void jobStarted();
            void jobFinished(const Obj &result);
            void jobCancelled();
        
            friend class PromiseData;
            void cancelJob();
            void pauseJob();
        
        private:
            /** Mutex protecting access to the data of this object */
            Mutex m;
        
            /** Self-pointer */
            exp_weak_ptr<WorkQueueItemData>::Type self;
        
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
    
    } // end of namespace detail

} // end of namespace Siren

SIREN_END_HEADER

#endif // ifndef SIREN_DETAIL_WORKQUEUEITEMDATA_H
