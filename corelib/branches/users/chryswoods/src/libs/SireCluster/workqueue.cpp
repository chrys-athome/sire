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

#include "workqueue.h"
#include "workpacket.h"
#include "promise.h"
#include "promises.h"
#include "frontend.h"

using namespace SireCluster;
using namespace Siren;

/////////
///////// Implementation of WorkQueue
/////////

WorkQueue::WorkQueue() : boost::noncopyable()
{}

WorkQueue::~WorkQueue()
{}

/////////
///////// Implementation of SimpleQueue
/////////

SimpleQueue::SimpleQueue() : WorkQueue(), Thread()
{
    this->start();
}

SimpleQueue::~SimpleQueue()
{
    this->kill();
    this->wait();
}

SimpleQueue* SimpleQueue::create(const DormantFrontend &frontend) const
{
    SimpleQueue *s(0);
    
    try
    {
        s = new SimpleQueue();
        
        s->frontends.insert(frontend.UID(), frontend);
        s->idle_frontends.append(frontend.UID());
    }
    catch(...)
    {
        delete s;
        throw;
    }
    
    return s;
}

SimpleQueue* SimpleQueue::create(const QList<DormantFrontend &frontends) const
{
    SimpleQueue *s(0);
    
    try
    {
        s = new SimpleQueue();
        
        foreach (DormantFrontend frontend, frontends)
        {
            s->frontends.insert(frontend.UID(), frontend);
            s->idle_frontends.append(frontend.UID());
        }
    }
    catch(...)
    {
        delete s;
        throw;
    }
    
    return s;
}

QString SimpleQueue::what() const
{
    return QString("SimpleQueue");
}

int SimpleQueue::nJobsToRun() const
{
    MutexLocker lkr( const_cast<Mutex*>(&datamutex) );
    return promises_to_process.count();
}

QPair<int,int> SimpleQueue::nBusyFree() const
{
    MutexLocker lkr( const_cast<Mutex*>(&datamutex) );
    return QPair<int,int>( busy_frontends.count(), idle_frontends.count() );
}

QString SimpleQueue::toString() const
{
    MutexLocker lkr( const_cast<Mutex*>(&datamutex) );

    return QObject::tr("WorkQueue( nFree() = %1, nBusy() = %2, nJobsToRun() = %3 )")
            .arg(idle_frontends.count())
            .arg(busy_frontends.count())
            .arg(promises_to_process.count());
}

WorkQueue* SimpleQueue::merge(WorkQueue &other)
{
    SimpleQueue *s(0);

    try
    {
    }
    catch(...)
    {
        delete s;
        throw;
    }
    
    return s;
}

Promise SimpleQueue::submit(const WorkPacket &workpacket)
{
    Promise promise = WorkQueue::createPromise(workpacket);
    
    MutexLocker lkr( &datamutex );
    promises_to_process.append(promise);
    waiter.wakeAll();
    
    return promise;
}

Promises SimpleQueue::submit(const QList<WorkPacketPtr> &workpackets)
{
    Promises promises = WorkQueue::createPromises(workpackets)
    
    if (not promises.isEmpty())
    {
        MutexLocker lkr( &datamutex );
        
        for (int i=0; i<promises.nPromises(); ++i)
        {
            promises_to_process.append(promises[i]);
        }
        
        waiter.wakeAll();
    }
    
    return promises;
}

void SimpleQueue::threadMain()
{
    MutexLocker lkr( &datamutex );
    
    while (for_ages())
    {
        while (not promises_to_process.isEmpty())
        {
            NEED TO WRITE SCHEDULER CODE
        }
    }
}
