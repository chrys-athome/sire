#ifndef SIREN_WORKQUEUE_H
#define SIREN_WORKQUEUE_H

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
    namespace detail{ class WorkQueueData; }

    /** A WorkQueue provides a scheduler that manages the processing
        of WorkPackets 
        
        @author Christopher Woods
    */
    class SIREN_EXPORT WorkQueue
    {
    public:
        WorkQueue();
        
        WorkQueue(const WorkQueue &other);
    
        ~WorkQueue();
        
        WorkQueue& operator=(const WorkQueue &other);
        
        bool operator==(const WorkQueue &other) const;
        bool operator!=(const WorkQueue &other) const;
        
        Promise submit(const WorkPacket &workpacket, int n=1);
        Promise submit(const WorkPacket &workpacket, WorkSpace &workspace, int n=1);
        
        int nWaiting() const;
        int nRunning() const;
        
        int nCompleted() const;
        
    private:
        exp_shared_ptr<detail::WorkQueueData>::Type d;
        
    }; // end of class WorkQueue

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::WorkQueue )

SIREN_END_HEADER

#endif // ifndef SIREN_WORKQUEUE_H
