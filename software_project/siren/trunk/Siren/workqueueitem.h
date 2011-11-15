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
    class Obj;
    class DateTime;

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
        WorkQueueItem(const Promise &promise,
                      const WorkQueue &queue);
                      
        WorkQueueItem(const WorkQueueItem &other);
        
        ~WorkQueueItem();
        
        WorkQueueItem& operator=(const WorkQueueItem &other);
        
        bool operator==(const WorkQueueItem &other) const;
        bool operator!=(const WorkQueueItem &other) const;

        static const char* typeName(){ return "Siren::WorkQueueItem"; }
        const char* what() const{ return typeName(); }
        
        Obj workPacket() const;
        DateTime submissionTime() const;
        DateTime startTime() const;
        DateTime finishTime() const;
        
        Obj operator[](const String &key) const;
    
        Promise promise() const;
    
    protected:
        friend class detail::PromiseData;
        friend class detail::WorkQueueData;
        friend class detail::WorkQueueItemData;
        WorkQueueItem(const exp_shared_ptr<detail::WorkQueueItemData> &ptr);
    
        void jobStarted();
        void jobCancelled();
        void jobFinished(const Obj &result);
    
    private:
        exp_shared_ptr<detail::WorkQueueItemData>::Type d;

    }; // end of class WorkQueueItem

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::WorkQueueItem )

SIREN_END_HEADER

#endif // ifndef SIREN_WORKQUEUEITEM_H
