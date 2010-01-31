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
#include <QHash>
#include <QQueue>

#include <boost/noncopyable.hpp>

#include "Siren/thread.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Siren{ template<class T> class ObjPtr; }

namespace SireCluster
{

class DormantFrontend;
class Promise;
class Promises;
class WorkPacket;
typedef Siren::ObjPtr<WorkPacket> WorkPacketPtr;

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

    virtual ~WorkQueue();
    
    virtual WorkQueue* create(const DormantFrontend &frontend) const=0;
    virtual WorkQueue* create(const QList<DormantFrontend> &frontends) const=0;
    
    virtual QString what() const=0;
    
    virtual QString toString() const=0;
    
    virtual WorkQueue* merge(WorkQueue &other)=0;
    
    virtual Promise submit(const WorkPacket &workpacket)=0;
    virtual Promises submit(const QList<WorkPacketPtr> &workpackets)=0;

    virtual QPair<int,int> nBusyFree() const=0;
    virtual int nJobsToRun() const=0;

protected:
    static Promise createPromise(const WorkPacket &workpacket, 
                                 bool forbid_local=false);
    static Promises createPromises(const QList<WorkPacketPtr> &workpackets,
                                   bool forbid_local=false);
};

/** This is a simple WorkQueue that just assigns WorkPackets to 
    backends without any care or consideration of the compatibility
    of the packets or resources
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT SimpleQueue : public WorkQueue, private Siren::Thread
{
public:
    SimpleQueue();
    ~SimpleQueue();
    
    SimpleQueue* create(const DormantFrontend &frontend) const;
    SimpleQueue* create(const QList<DormantFrontend &frontends) const;
    
    QString what() const;
    
    QString toString() const;
    
    WorkQueue* merge(WorkQueue &other);
    
    Promise submit(const WorkPacket &workpacket);
    Promises submit(const QList<WorkPacketPtr> &workpackets);
    
    QPair<int,int> nBusyFree() const;
    int nJobsToRun() const;

protected:
    void threadMain();

private:
    void kill();

    /** Mutex to protect access to the queue */
    Mutex datamutex;

    /** Waitcondition used to hold the scheduling thread until
        there is some work to schedule */
    WaitCondition waiter;

    /** The list of frontends for the available resources */
    QHash<QUuid,DormantFrontend> frontends;
    
    /** The IDs of busy frontends */
    QList<QUuid> busy_frontends;
    
    /** The IDs of idle frontends */
    QList<QUuid> idle_frontends;
    
    /** The list of promises still to process */
    QQueue<Promise> promises_to_process;
};

}

SIRE_END_HEADER

#endif
