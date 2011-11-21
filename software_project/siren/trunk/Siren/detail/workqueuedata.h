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
#include "Siren/waitcondition.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    class WorkQueue;

    namespace detail
    {
        /** This class provides the private implementation of WorkQueue */
        class WorkQueueData : public noncopyable
        {
        public:
            WorkQueueData();
            WorkQueueData(int n);
            
            ~WorkQueueData();

            Promise submit(const WorkPacket &packet, int n);
            Promise submit(const WorkPacket &packet,
                           const WorkSpace &space, int n);
        
            int nRunning();
            int nWaiting();
            int nBlocked();
            int nCompleted();
    
            String toString();
    
        private:
            /** Mutex to protect access to the data of this queue */
            Mutex m;
        
            /** WaitCondition used to wake the background thread to 
                notify it of any change in state */
            WaitCondition waiter;
            
            /** The list of jobs that have not yet run */
            List<WorkQueueItem>::Type waiting_jobs;
            
            /** The list of jobs that are currently running */
            List<WorkQueueItem>::Type running_jobs;
            
            /** The list of jobs that are blocked because they 
                request more resources than will ever be available */
            List<WorkQueueItem>::Type blocked_jobs;
            
            /** The list of jobs that have been cancelled */
            List<WorkQueueItem>::Type cancelled_jobs;
            
            /** The list of jobs that have been finished */
            List<WorkQueueItem>::Type completed_jobs;

            friend class Siren::WorkQueue;
            exp_weak_ptr<WorkQueueData>::Type self;
            
            /** The number of cpu threads that this WorkQueue 
                would like to manage */
            int nthreads;
        };
    
    } // end of namespace detail

} // end of namespace Siren


SIREN_END_HEADER

#endif // ifndef SIREN_DETAIL_WORKQUEUEDATA_H
