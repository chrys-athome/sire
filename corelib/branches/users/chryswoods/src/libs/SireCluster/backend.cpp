/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "backend.h"
#include "cluster.h"
#include "workpacket.h"

#include <QDebug>

using namespace SireCluster;

using boost::shared_ptr;

namespace SireCluster
{
namespace detail
{

/** Private implementation of Backend */
class BackendPvt : public QThread
{
public:
    BackendPvt() : QThread(), keep_running(true)
    {
        uid = QUuid::createUuid();
    }
    
    ~BackendPvt()
    {}
    
    /** Mutex used to protect access to the data of 
        this backend */
    QMutex datamutex;
    
    /** This mutex is used to ensure that only one 
        thread can try to start a job at a time */
    QMutex startmutex;
    
    /** This mutex is used to ensure that only one Frontend
        at a time is connecting to this Backend */
    QMutex connectionmutex;
    
    /** WaitCondition used to signal that the backend thread 
        has started */
    QWaitCondition startwaiter;
    
    /** The work packet being processed by this backend */
    WorkPacket workpacket;
    
    /** The final results */
    WorkPacket resultspacket;
    
    /** The unique ID of this backend */
    QUuid uid;
    
    /** Whether or not to keep running */
    bool keep_running;
    
protected:
    void run();
};

} // end of namespace detail
} // end of namespace SireCluster

using namespace SireCluster::detail;

//////////
////////// Implementation of Backend
//////////

/** Construct a null, unusable backend */
Backend::Backend()
{}

/** Create a new, usable Backend */
Backend Backend::create()
{
    Backend backend;
    backend.d.reset( new BackendPvt() );
    
    Cluster::registerBackend(backend);
    
    return backend;
}

/** Copy constructor */
Backend::Backend(const Backend &other) : d(other.d)
{}

/** Destructor */
Backend::~Backend()
{}

/** Copy assignment operator */
Backend& Backend::operator=(const Backend &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool Backend::operator==(const Backend &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool Backend::operator!=(const Backend &other) const
{
    return d.get() != other.d.get();
}

/** Return whether or not this is a null backend */
bool Backend::isNull() const
{
    return d.get() == 0;
}

/** Return the unique ID of this backend */
QUuid Backend::UID() const
{
    if (not this->isNull())
        return d->uid;
     
    else
        return QUuid();
}

/** Function used to actually run the job */
void BackendPvt::run()
{
    //wake the thread that told us to run the job
    startwaiter.wakeAll();
    
    while (true)
    {
        WorkPacket local_packet;
        
        //// copy the work packet into a local space
        {
            QMutexLocker lkr(&datamutex);
            
            if (not keep_running)
                break;
                
            if (workpacket.hasFinished())
                break;
                
            local_packet = workpacket;
        }
        
        //now perform the work on the local packet
        local_packet.runChunk();
        
        //// copy the local work back to the global work
        {
            QMutexLocker lkr(&datamutex);
            
            if (workpacket.hasFinished())    
                break;
                
            workpacket = local_packet;
            
            if (not keep_running)
                break;
        }
    }
    
    //the work has finished - copy the results
    QMutexLocker lkr(&datamutex);
    resultspacket = workpacket;
}

/** Start a job on this backend - this blocks until any
    previous job has finished */
void Backend::startJob(const WorkPacket &workpacket)
{
    if (this->isNull() or workpacket.hasFinished())
        return;
        
    //block to ensure that only one job can be started at a time
    QMutexLocker lkr( &(d->startmutex) );

    //wait until the last job has finished
    d->wait();
    
    //ok, we now know that we are the only thread trying to start
    //a job, and we know that no job is currently running
    QMutexLocker lkr2( &(d->datamutex) );

    while (not d->resultspacket.isNull())
    {
        //the user has not retrieved the results of the last job!
        d->startwaiter.wait( &(d->datamutex), 250 );
    }

    d->workpacket = workpacket;
    d->resultspacket = WorkPacket();
    d->keep_running = true;
    lkr2.unlock();
    
    //start the job
    d->start();
    
    //wait until the job has started
    d->startwaiter.wait( &(d->startmutex) );
}

/** Stop the job - this blocks until the job has stopped, and it
    returns the current state of the WorkPacket */
void Backend::stopJob()
{
    QMutexLocker lkr( &(d->datamutex) );
    d->keep_running = false;
}

/** Abort the job - this blocks until the job has aborted,
    and then returns the aborted WorkPacket */
void Backend::abortJob()
{
    QMutexLocker lkr( &(d->datamutex) );
    
    if (not d->workpacket.isNull())
        //there is nothing running to be aborted
        d->workpacket.abort();
}

/** Wait for the backend thread to finish the work */
void Backend::wait()
{
    if (not this->isNull())
        d->wait();
}

/** Wait for the backend thread to finish the work, or for timeout
    milliseconds to pass - this returns whether or not the work has 
    finished */
bool Backend::wait(int timeout)
{
    if (not this->isNull())
        return d->wait(timeout);
    else
        return true;
}

/** Return the current progress of the work */
float Backend::progress()
{
    if (this->isNull())
        return 0;

    QMutexLocker lkr( &(d->datamutex) );
    return d->workpacket.progress();
}

/** Return the current interim result - this will be a null
    workpacket if there is no work being run */
WorkPacket Backend::interimResult()
{
    if (this->isNull())
        return WorkPacket();

    QMutexLocker lkr( &(d->datamutex) );
    return d->workpacket;
}

/** Return the result of the work. You must call this to collect
    the result of the work, so that the backend can be released
    to perform more work */
WorkPacket Backend::result()
{
    if (this->isNull())
        return WorkPacket();

    QMutexLocker lkr( &(d->datamutex) );
    
    if (d->workpacket.isNull())
        //there is nothing running to be stopped
        return d->workpacket;
    
    d->keep_running = false;
    lkr.unlock();
    
    //wait for the thread to stop
    d->wait();
    
    lkr.relock();

    if (d->workpacket.isNull())
        //someone has sneaked in and stolen the results from us!
        return d->workpacket;

    WorkPacket finished_packet = d->resultspacket;
    d->workpacket = WorkPacket();
    d->resultspacket = WorkPacket();
    d->keep_running = true;
    
    return finished_packet;
}

/** This function is called by a Frontend which it connects to 
    this backend - this blocks until there are no other frontends
    connecting to this backend */
void Backend::connect()
{
    if (not this->isNull())
    {
        d->connectionmutex.lock();
    }
}

/** This function is called by a Frontend when it disconnects
    from this Backend - this must only be called by the same thread
    used by the Frontend when it called Backend::connect() */
void Backend::disconnect()
{
    if (not this->isNull())
    {
        //ensure that this node has finished work
        this->stopJob();
        d->connectionmutex.unlock();
    }
}

/** This function is called by a Frontend to attempt a connection to this 
    Backend - this returns true if the connection was successful */
bool Backend::tryConnect()
{
    if (not this->isNull())
    {
        return d->connectionmutex.tryLock();
    }
    else
        return false;
}

/** This function shuts down the backend, stopping any running
    jobs and preventing any from being started */
void Backend::shutdown()
{
    if (this->isNull())
        return;
        
    this->abortJob();
}
