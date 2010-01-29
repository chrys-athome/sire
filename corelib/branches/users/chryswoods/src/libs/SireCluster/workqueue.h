/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#ifndef SIRECLUSTER_WORKQUEUE_H
#define SIRECLUSTER_WORKQUEUE_H

#include <QList>

#include <boost/noncopyable.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{

class DormantFrontend;

/** This is the base class of all WorkQueues. A WorkQueue
    is a scheduler that schedules WorkPackets to be run
    on ActiveFrontends. WorkQueue objects are non-copyable,
    and are designed to be held by Node or Nodes objects.
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT WorkQueue : public boost::noncopyable
{
public:
    WorkQueue();
    WorkQueue(const QList<DormantFrontend> &frontends);

    ~WorkQueue();
    
    virtual QString what() const=0;
    
    virtual QString toString() const=0;
    
    virtual WorkQueue* merge(const WorkQueue &other)=0;
    
    virtual Promise submit(const WorkPacket &workpacket)=0;
    virtual Promises submit(const QList<WorkPacketPtr> &workpacket)=0;

    virtual QPair<int,int> nBusyFree() const=0;
    
};

}

SIRE_END_HEADER

#endif
