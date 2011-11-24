#ifndef SIREN_WORKQUEUEITEM_H
#define SIREN_WORKQUEUEITEM_H

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

#include "Siren/siren.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    class Promise;
    class WorkQueue;
    class WorkPacket;
    class WorkSpace;
    class Obj;
    class DateTime;
    class WorkQueueItemRef;

    namespace detail
    { 
        class PromiseData;
        class WorkQueueData;
        class WorkQueueItemData; 
    }

    /** This class represents a single work item that will
        be processed in a WorkQueue. It holds everything to do
        with the WorkPacket as it is being processed, e.g.
        the original packet, the link to the Promise,
        the WorkSpace etc. This is to allow the queue to manage
        the WorkPacket during computation, and the Promise
        to be completely separated, and just be there to hold 
        the result. */
    class SIREN_EXPORT WorkQueueItem
    {
    public:
        WorkQueueItem();
        WorkQueueItem(const WorkPacket &workpacket,
                      const WorkQueue &queue, int n=1,
                      bool is_background = false);
                      
        WorkQueueItem(const WorkPacket &workpacket,
                      const WorkSpace &workspace, 
                      const WorkQueue &queue, int n=1,
                      bool is_background = false);
                      
        WorkQueueItem(const WorkQueueItemRef &ref);
                      
        WorkQueueItem(const WorkQueueItem &other);
        
        ~WorkQueueItem();
        
        WorkQueueItem& operator=(const WorkQueueItem &other);
        
        bool operator==(const WorkQueueItem &other) const;
        bool operator!=(const WorkQueueItem &other) const;

        bool operator==(const WorkQueueItemRef &other) const;
        bool operator!=(const WorkQueueItemRef &other) const;

        static const char* typeName(){ return "Siren::WorkQueueItem"; }
        const char* what() const{ return typeName(); }
        
        String toString() const;
        
        Obj workPacket() const;
        WorkSpace workSpace() const;
        
        bool hasWorkSpace() const;
        
        int nWorkers() const;
        
        DateTime submissionTime() const;
        DateTime startTime() const;
        DateTime finishTime() const;
        
        Obj operator[](const String &key) const;
    
        Promise promise() const;
        WorkQueue queue() const;
    
        bool isBG() const;
        bool isFG() const;
    
    protected:
        friend class WorkQueueItemRef;
        friend class detail::WorkQueueItemData;
        WorkQueueItem(const exp_shared_ptr<detail::WorkQueueItemData>::Type &ptr);

        friend class detail::WorkQueueData;
        friend class detail::PromiseData;
        void setPromise(const Promise &promise);
        
        void toBG();
        void toFG();
    
        void jobStarted();
        void jobCancelled();
        void jobFinished(const Obj &result);
    
        void abort(); // called by Promise to indicate that the job should stop
    
    private:
        exp_shared_ptr<detail::WorkQueueItemData>::Type d;

    }; // end of class WorkQueueItem

    /** This is a weak reference to a WorkQueueItem */
    class SIREN_EXPORT WorkQueueItemRef
    {
    public:
        WorkQueueItemRef();
        WorkQueueItemRef(const WorkQueueItem &item);
        WorkQueueItemRef(const WorkQueueItemRef &other);
        
        ~WorkQueueItemRef();
        
        WorkQueueItemRef& operator=(const WorkQueueItemRef &other);
        
        bool operator==(const WorkQueueItemRef &other) const;
        bool operator!=(const WorkQueueItemRef &other) const;
        
        bool operator==(const WorkQueueItem &other) const;
        bool operator!=(const WorkQueueItem &other) const;
        
        String toString() const;

    protected:
        friend class detail::PromiseData;
        void abort(); // called by Promise to indicate that the job should stop

    private:
        friend class WorkQueueItem;
        exp_weak_ptr<detail::WorkQueueItemData>::Type d;
    };

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::WorkQueueItem )
SIREN_EXPOSE_CLASS( Siren::WorkQueueItemRef )

SIREN_END_HEADER

#endif // ifndef SIREN_WORKQUEUEITEM_H
