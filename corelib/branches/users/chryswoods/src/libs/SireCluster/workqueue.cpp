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

#include "Siren/forages.h"

#include <QDebug>

using namespace SireCluster;
using namespace Siren;

/////////
///////// Implementation of WorkQueue
/////////

WorkQueue::WorkQueue() : boost::noncopyable()
{}

WorkQueue::~WorkQueue()
{}

Promise WorkQueue::createPromise(const WorkPacket &workpacket, bool forbid_local)
{
    return Promise(workpacket, forbid_local);
}

Promises WorkQueue::createPromises(const QVector<WorkPacketPtr> &workpackets, 
                                   bool forbid_local)
{
    if (workpackets.isEmpty())
        return Promises();

    QVector<Promise> promises;
    promises.reserve(workpackets.count());
    
    for (int i=0; i<workpackets.count(); ++i)
    {
        if (not workpackets.at(i).isNull())
            promises.append( Promise(workpackets.at(i).read(), forbid_local) );
    }

    return Promises(promises);
}

bool WorkQueue::runPromise(Promise promise, ActiveFrontend frontend)
{
    return promise.runRemote(frontend);
}

/////////
///////// Implementation of SimpleQueue
/////////

SimpleQueue::SimpleQueue() : WorkQueue(), Thread(QObject::tr("SimpleQueue"))
{
    kill_queue = false;
}

SimpleQueue::~SimpleQueue()
{
    this->kill();
}

SimpleQueue* SimpleQueue::create(const DormantFrontend &frontend) const
{
    SimpleQueue *s(0);
    
    try
    {
        s = new SimpleQueue();
        
        s->frontends.insert(frontend.UID(), frontend);
        s->idle_frontends.append(frontend.UID());

        s->start();
    }
    catch(...)
    {
        delete s;
        throw;
    }
    
    return s;
}

SimpleQueue* SimpleQueue::create(const QList<DormantFrontend> &frontends) const
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

void SimpleQueue::kill()
{
    MutexLocker lkr( &datamutex );
    kill_queue = true;
    end_for_ages(this);
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

Promises SimpleQueue::submit(const QVector<WorkPacketPtr> &workpackets)
{
    Promises promises = WorkQueue::createPromises(workpackets);
    
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

bool SimpleQueue::checkForFinishedNodes()
{
    QMutableListIterator<QUuid> it( busy_frontends );
    
    bool new_nodes = false;
    
    while (it.hasNext())
    {
        QUuid &busy_node = it.next();
        
        if (not frontends[busy_node].isActive())
        {
            idle_frontends.enqueue(busy_node);
            it.remove();
            new_nodes = true;
        }
    }
    
    return new_nodes;
}

void SimpleQueue::threadMain()
{
    MutexLocker lkr( &datamutex );
    
    while (for_ages())
    {
        if (kill_queue)
            return;

        while (not promises_to_process.isEmpty())
        {
            if (kill_queue)
                return;
        
            lkr.unlock();
            
            //see if any of the busy frontends are now idle
            this->checkForFinishedNodes();
            
            ActiveFrontend frontend;
            
            while (frontend.isNull())
            {
                if (kill_queue)
                    return;
            
                while (not idle_frontends.isEmpty())
                {
                    QUuid idle_frontend = idle_frontends.dequeue();
                    busy_frontends.append(idle_frontend);
                
                    frontend = frontends[idle_frontend].tryActivate();
                
                    if (not frontend.isNull())
                        break;
                }
                
                if (frontend.isNull())
                {
                    if (not this->checkForFinishedNodes())
                    {
                        if (not kill_queue) QThread::msleep(500);
                    }
                }
            }
            
            if (frontend.isNull())
            {
                //there is no free node available
                QThread::msleep(500);
            }
            else
            {
                //schedule the job
                lkr.relock();
                
                bool need_new_node = false;
                
                while (not promises_to_process.isEmpty())
                {
                    Promise p = promises_to_process.dequeue();
                
                    if (WorkQueue::runPromise(p, frontend))
                    {
                        need_new_node = true;
                        break;
                    }
                }
                
                lkr.unlock();
                
                if (not need_new_node)
                    //there is nothing to run
                    break;
            }
            
            lkr.relock();
        }
        
        //there is nothing to process
        try
        {
            waiter.wait( &datamutex, 5000 );
        }
        catch(...)
        {
            if (kill_queue)
                return;
                
            throw;
        }
            
        //see if any of the busy frontends are now idle
        this->checkForFinishedNodes();
    }
}
