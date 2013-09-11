#ifndef SIREN_DETAIL_PROMISEDATA_H
#define SIREN_DETAIL_PROMISEDATA_H

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
#include "Siren/waitcondition.h"
#include "Siren/mutex.h"
#include "Siren/obj.h"
#include "Siren/workqueueitem.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    class WorkPacket;
    class WorkQueue;

    namespace detail
    { 
        class WorkQueueData;
        class WorkQueueItemData;
    
        /** This is an internal class used by Promise, Thread
            and ThreadPool to hold the future results of calculations
            that have yet to complete. This should not be used by
            any other code */
        class PromiseData : public noncopyable
        {
        public:
            PromiseData(const WorkQueueItem &workitem);
            PromiseData(const Obj &result);
            ~PromiseData();

            bool hasFinished();
            bool hasStarted();
            bool isCancelled();
            
            void waitForStarted();
            bool waitForStarted(int ms);
            
            void wait();
            bool wait(int ms);
            
            Obj result();

            String toString();

        protected:
            friend class Siren::Promise;
            friend class Siren::PromiseRef;
            
            void jobFinished(const Obj &result);
            void jobFinished(const Obj &result, int i, int n);
            
            void jobCancelled();
            void jobStarted();

        private:
            void setResult(const Obj &result);

            Mutex m;
            
            /** Pointer to self */
            exp_weak_ptr<PromiseData>::Type self;
            
            WaitCondition w;
            Obj reslt;
            
            Vector<Obj>::Type result_part;
            Set<int>::Type got_result;

            WorkQueueItemRef workitem;

            bool has_started;
            bool is_cancelled;
            bool has_finished;
        
        }; // end of class PromiseData
    
    } // end of namespace detail

} // end of namespace Siren

SIREN_END_HEADER

#endif // ifndef SIREN_DETAIL_PROMISEDATA_H