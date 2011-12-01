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
#include "Siren/promise.h"
#include "Siren/workqueue.h"
#include "Siren/worklog.h"

SIREN_BEGIN_HEADER

namespace Siren
{
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
                              int n, bool is_background);
            
            WorkQueueItemData(const WorkPacket &packet,
                              const WorkSpace &workspace,
                              const WorkQueue &queue,
                              int n, bool is_background);
            
            ~WorkQueueItemData();
            
            Obj workPacket();
            WorkSpace workSpace();
            
            bool hasWorkSpace();
            
            int nWorkers();
            
            DateTime submissionTime();
            DateTime startTime();
            DateTime finishTime();
            
            Obj operator[](const String &string);
        
            Promise promise();
            WorkQueue queue();
            WorkLog log();
            WorkQueueItem workQueueItem();
        
            bool isBG();
            bool isFG();
        
            String toString();
        
        protected:
            friend class Siren::WorkQueueItem;
            friend class Siren::WorkQueueItemRef;
            void setPromise(const Promise &promise);
        
            void chunkStarted();
            void chunkStarted(int worker_id, int nworkers);
            
            void chunkFinished(const Obj &result);
            void chunkFinished(const Obj &result, int worker_id, int nworkers);
        
            void abort();
        
            void toFG();
            void toBG();
        
        private:
            /** Mutex protecting access to the data of this object */
            Mutex m;
        
            /** Self-pointer */
            exp_weak_ptr<WorkQueueItemData>::Type self;
            
            /** The WorkPacket to be processed */
            Obj workpacket;
            
            /** Pointer to a WorkSpace, if one is needed */
            WorkSpace *workspace;

            /** The log detailing the progress made while running the job */
            WorkLog worklog;

            /** This is the queue that is processing this item. If we lose
                the queue, then we cannot run this job */
            WorkQueueRef parent_queue;
        
            /** Store the WorkPacket in the Promise, so if lose promise,
                then lose WorkPacket - the Promise is the handle to the job */
            PromiseRef job_promise;
        
            /** A set of other metadata that may be added and manipulated
                by the WorkScheduler responsible for scheduling this job */
            Hash<String,Obj>::Type mdata;

            /** The number of worker threads to use to process the packet */
            int nworkers;
            
            /** Whether or not the job should be aborted */
            bool should_abort;
            
            /** Whether or not the work should be processed in the background */
            bool is_bg;
        
        }; // end of class WorkQueueItem
    
    } // end of namespace detail

} // end of namespace Siren

SIREN_END_HEADER

#endif // ifndef SIREN_DETAIL_WORKQUEUEITEMDATA_H
