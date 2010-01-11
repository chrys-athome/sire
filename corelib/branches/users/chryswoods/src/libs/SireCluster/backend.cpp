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

#include <boost/weak_ptr.hpp>

#include "backend.h"
#include "cluster.h"
#include "workpacket.h"

#include "SireMaths/rangenerator.h"

#include "Siren/errors.h"

#include <QDebug>

using namespace SireCluster;
using namespace Siren;

//////
////// Implementation of Backend
//////

/** Create the Backend whose resource description is in 'description' */
Backend::Backend(const QString &description) 
        : desc(description), uid( QUuid::createUuid() )
{}

/** Destructor */
Backend::~Backend()
{}

/** Return the UID of the backend */
const QUuid& Backend::UID() const
{
    return uid;
}

/** Return the resource description of the backend */
const QString& Backend::description() const
{
    return desc;
}

//////
////// Implementation of LocalBackend
//////

/** Construct a new local backend */
LocalBackend::LocalBackend() : Backend( "local" )
{}

/** Destructor */
LocalBackend::~LocalBackend()
{}

QString LocalBackend::what() const
{
    return "SireCluster::LocalBackend";
}

/** Start the job on this backend (this actually runs the complete
    job as this is the main thread) */
void LocalBackend::startJob(const WorkPacket &workpacket)
{
    job_in_progress = workpacket;
    
    while (not job_in_progress.read().hasFinished())
    {
        job_in_progress = job_in_progress.read().runChunk();
    }
}

/** Stop the job */
void LocalBackend::stopJob()
{
    //no-op
}

/** Abort the job */
void LocalBackend::abortJob()
{
    //no-op
}

/** Wait for the job to finish */
void LocalBackend::wait()
{
    //no-op
}

/** Wait for the job to finish */
bool LocalBackend::wait(int)
{
    return true;
}

/** Return the current progress of the job */
float LocalBackend::progress()
{
    if (job_in_progress.isNull())
        return 0;
    else
        return job_in_progress.read().progress();
}

/** Return the current state of the running job */
WorkPacketPtr LocalBackend::interimResult()
{
    return job_in_progress;
}

/** Return the final state of the running job */
WorkPacketPtr LocalBackend::result()
{
    return job_in_progress;
}

//////
////// Implementation of ThreadBackend
//////

/** Construct a new thread backend */
ThreadBackend::ThreadBackend() : Backend("thread"), QThread()
{}

/** Destructor */
ThreadBackend::~ThreadBackend()
{
    QThread::wait();
}

QString ThreadBackend::what() const
{
    return "SireCluster::ThreadBackend";
}

/** Start a job on this backend - this blocks until the job 
    has started */
void ThreadBackend::startJob(const WorkPacket &workpacket)
{
    //block to ensure that only one job can be started 
    //at a time
    QMutexLocker lkr( &start_mutex );
    
    while (job_is_starting)
    {
        //another thread is trying to start a job - we have to wait
        startwaiter.wait( &startmutex );
    }
    
    //set the flag to say that *we* are starting a job
    job_is_starting = true;
    
    //wait until the last job has finished
    QThread::wait();
    
    //ok, we now know that we are the only thread trying to start
    //a job, and we know that no job is currently running
    {
        QMutexLocker lkr2( &datamutex );

        job_in_progress = workpacket;
        result = WorkPacketPtr();
        keep_running = true;
    }
    
    //start the job
    this->start();
    
    //wait until the job has started
    startwaiter.wait( &startmutex );
}

/** Stop the job */
void ThreadBackend::stopJob()
{
    QMutexLocker lkr(&datamutex);
    keep_running = false;
}

/** Abort the job */
void ThreadBackend::abortJob()
{
    QMutexLocker lkr(&datamutex);
    keep_running = false;
    result = AbortPacket();
    job_in_progress = result;
}

/** Wait for the workpacket to complete */
void ThreadBackend::wait()
{
    while (not QThread::wait(2000))
    {
        if (not QThread::isRunning())
            //the job has stopped
            return;
    }
}

bool ThreadBackend::wait(int timeout);

float ThreadBackend::progress();
WorkPacketPtr ThreadBackend::interimResult();

WorkPacketPtr ThreadBackend::result();

/** Function run by the backend thread */
void ThreadBackend::run()
{
    Siren::setThreadString( QString("ThreadBackend-%1")
                              .arg( toInt(QThread::currentThread()) ));

    SireMaths::seed_qrand();

    //wake the thread that told us to run the job
    startmutex.lock();
    
    if (not job_is_starting)
    {
        qDebug() << Siren::getPIDString() << "How have we been started???"
                 << "job_is_started is false in BackendPvt::run()";
                 
        startwaiter.wakeAll();
        startmutex.unlock();
        return;
    }
    
    job_is_starting = false;
    
    startwaiter.wakeAll();
    startmutex.unlock();
    
    while (true)
    {
        try
        {
            WorkPacketPtr local_packet;
        
            //// copy the work packet into a local space
            {
                QMutexLocker lkr(&datamutex);
            
                if (not keep_running)
                    break;
                
                if (job_in_progress.read().hasFinished())
                    break;
                
                local_packet = job_in_progress;
            }
        
            //now perform the work on the local packet
            if (not local_packet.read().hasFinished())
                local_packet = local_packet.read().runChunk();
        
            //// copy the local work back to the global work
            {
                QMutexLocker lkr(&datamutex);
            
                job_in_progress = local_packet;
            
                if (job_in_progress.read().hasFinished() or not keep_running)
                    break;
            }
        }
        catch(const Siren::exception &e)
        {
            QMutexLocker lkr(&datamutex);
            job_in_progress = ErrorPacket(e);
            break;
        }
        catch(const std::exception &e)
        {
            QMutexLocker lkr(&datamutex);
            job_in_progress = ErrorPacket(Siren::std_exception(e));
            break;
        }
        catch(...)
        {
            QMutexLocker lkr(&datamutex);
            job_in_progress = ErrorPacket(Siren::unknown_error( QObject::tr(
                    "An unknown error occured while running a workpacket."),
                        CODELOC ) );
            break;
        }
    }
    
    //the work has finished - copy the results
    QMutexLocker lkr(&datamutex);
    result = job_in_progress;
}

//////
////// Implementation of DormantBackend
//////

//////
////// Implementation of ActiveBackend
//////


/** This function is called by a Frontend which it connects to 
    this backend - this blocks until there are no other frontends
    connecting to this backend. This returns an ActiveBackend - which
    provides the active interface that a Frontend uses to talk with
    a Backend */
ActiveBackend Backend::connect() const
{
    return ActiveBackend(*this);
}

/** This function is called by a Frontend to attempt a connection to this 
    Backend - this returns a non-null ActiveBackend if it was successful.
    Use this function to test if a Backend is active (as it avoids
    the possibility of race conditions) */
ActiveBackend Backend::tryConnect() const
{
    return ActiveBackend::tryConnect(*this);
}

/** This function is called by a Frontend to attempt a connection to this 
    Backend and to keep trying for up to 'timeout' milliseconds if necessary.
    This returns a non-null ActiveBackend if it was successful */
ActiveBackend Backend::tryConnect(int timeout) const
{
    return ActiveBackend::tryConnect(*this, timeout);
}

/** Try to lock this Backend - this returns a null pointer if
    this Backend is already locked */
shared_ptr<BackendLock> BackendPvt::tryLock()
{
    QMutexLocker lkr( &connectionmutex );
    
    shared_ptr<BackendLock> my_lock;
    
    if (backend_lock.expired() and (connectionwaiter.get() != 0))
    {
        //we can connect!
        my_lock.reset( new BackendLock(connectionwaiter) );
        backend_lock = my_lock;
    }

    return my_lock;
}

/** Try to lock this backend, and keep trying for up to 'timeout' 
    milliseconds - returns a null pointer if it fails */
shared_ptr<BackendLock> BackendPvt::tryLock(int timeout)
{
    if (timeout < 0)
        return this->lock();
        
    shared_ptr<BackendLock> my_lock = this->tryLock();
    
    if (my_lock.get() == 0)
    {
        //we weren't successful - we may have to sleep on it
        QMutexLocker lkr( &connectionmutex );
        
        if (connectionwaiter.get() == 0)
            //this backend is shutting down - noone may connect to it
            return my_lock;
        
        shared_ptr<QWaitCondition> w = connectionwaiter;
        w->wait( &connectionmutex, timeout );
        
        if (connectionwaiter.get() != 0)
        {
            //we are still allowed to connect
            my_lock.reset( new BackendLock(connectionwaiter) );
            backend_lock = my_lock;
        }
    }
        
    return my_lock;
}

/** Lock this Backend - this blocks until a lock is obtained
    (or this backend is being shutdown). This returns the lock */
shared_ptr<BackendLock> BackendPvt::lock()
{
    shared_ptr<BackendLock> my_lock = this->tryLock();
    
    if (my_lock.get() == 0)
    {
        //we weren't successful - we may have to sleep on it
        QMutexLocker lkr( &connectionmutex );
        
        if (connectionwaiter.get() == 0)
            //this backend is shutting down - no-one may connect to it
            return my_lock;
        
        shared_ptr<QWaitCondition> w = connectionwaiter;
        w->wait( &connectionmutex );
        
        if (connectionwaiter.get() != 0)
        {
            //we are still allowed to connect
            my_lock.reset( new BackendLock(connectionwaiter) );
            backend_lock = my_lock;
        }
    }

    return my_lock;
}

/** This function shuts down the backend, stopping any running
    jobs and preventing any from being started */
void Backend::shutdown()
{
    if (this->isNull())
        return;
        
    /////// Abort any running jobs
    {
        QMutexLocker lkr( &(d->datamutex) );
    
        if (not d->workpacket.isNull())
            //there is nothing running to be aborted
            d->workpacket.abort();
    }

    //now make sure that no other frontends can 
    //connect to this backend - we do this by wiping
    //out the connectionwaiter (after waking anyone
    //waiting on it)
    {
        QMutexLocker lkr( &(d->connectionmutex) );
    
        if (d->connectionwaiter.get() != 0)
        {
            d->connectionwaiter->wakeAll();
            d->connectionwaiter.reset();
        }
    }
}

//////////
////////// Implementation of ActiveBackend
//////////

/** Null constructor */
ActiveBackend::ActiveBackend()
{}

/** Construct to activate the backend 'backend'. This blocks
    until the backend is available */
ActiveBackend::ActiveBackend(const Backend &backend)
{
    this->operator=( ActiveBackend::connect(backend) );
}
    
/** Copy constructor */
ActiveBackend::ActiveBackend(const ActiveBackend &other)
              : d(other.d), d_lock(other.d_lock)
{}
  
/** Destructor */ 
ActiveBackend::~ActiveBackend()
{
    d_lock.reset();
    d.reset();
}

/** Copy assignment operator */
ActiveBackend& ActiveBackend::operator=(const ActiveBackend &other)
{
    d_lock = other.d_lock;
    d = other.d;

    return *this;
}

/** Comparison operator */
bool ActiveBackend::operator==(const ActiveBackend &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool ActiveBackend::operator!=(const ActiveBackend &other) const
{
    return d.get() != other.d.get();
}

/** Try to connect to the passed backend - if this fails it returns
    a null ActiveBackend */
ActiveBackend ActiveBackend::tryConnect(const Backend &backend)
{
    ActiveBackend active_backend;
    
    if (not backend.isNull())
    {
        shared_ptr<BackendLock> d_lock = backend.d->tryLock();
    
        if (d_lock.get() != 0)
        {
            //we got a lock
            active_backend.d = backend.d;
            active_backend.d_lock = d_lock;
        }
    }
    
    return active_backend;
}

/** Try to connect to the passed backend - and keep trying
    for up to 'timeout' milliseconds if necessary
    - if this fails it returns a null ActiveBackend */
ActiveBackend ActiveBackend::tryConnect(const Backend &backend, int timeout)
{
    ActiveBackend active_backend;
    
    if (not backend.isNull())
    {
        shared_ptr<BackendLock> d_lock = backend.d->tryLock(timeout);
    
        if (d_lock.get() != 0)
        {
            //we got a lock
            ActiveBackend active_backend;
            active_backend.d = backend.d;
            active_backend.d_lock = d_lock;
        }
    }
    
    return active_backend;
}

/** Connect to the passed backend - this will block until we
    get a hold of the backend. In some extreme circumstances
    this may fail, e.g. if the backend is shutdown while
    we are trying to connect. So always remember to check
    if the returned ActiveBackend is non-null */
ActiveBackend ActiveBackend::connect(const Backend &backend)
{
    ActiveBackend active_backend;
    
    if (not backend.isNull())
    {
        shared_ptr<BackendLock> d_lock = backend.d->lock();
        
        if (d_lock.get() != 0)
        {
            active_backend.d = backend.d;
            active_backend.d_lock = d_lock;
        }
    }
    
    return active_backend;
}

/** Return whether or not this is a null backend */
bool ActiveBackend::isNull() const
{
    return d.get() == 0;
}

/** Return the unique ID of this backend */
QUuid ActiveBackend::UID() const
{
    if (not this->isNull())
        return d->uid;
     
    else
        return QUuid();
}

/** Start a job on this backend - this blocks until any
    previous job has finished */
void ActiveBackend::startJob(const WorkPacket &workpacket)
{
    if (this->isNull() or workpacket.hasFinished())
        return;
        
    //block to ensure that only one job can be started at a time
    QMutexLocker lkr( &(d->startmutex) );

    while ( d->job_is_starting )
    {
        //another process is already starting a job - we have
        //to wait...
        d->startwaiter.wait( &(d->startmutex) );
    }

    //set the flag to say that *we* are now starting a job
    d->job_is_starting = true;

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
void ActiveBackend::stopJob()
{
    QMutexLocker lkr( &(d->datamutex) );
    d->keep_running = false;
}

/** Abort the job - this blocks until the job has aborted,
    and then returns the aborted WorkPacket */
void ActiveBackend::abortJob()
{
    QMutexLocker lkr( &(d->datamutex) );
    
    if (not d->workpacket.isNull())
        //there is nothing running to be aborted
        d->workpacket.abort();
}

/** Wait for the backend thread to finish the work */
void ActiveBackend::wait()
{
    if (not this->isNull())
    {
        while (not d->wait(2000))
        {
            if (not d->isRunning())
            {
                //the job really has stopped
                return;
            }
        }
    }
}

/** Wait for the backend thread to finish the work, or for timeout
    milliseconds to pass - this returns whether or not the work has 
    finished */
bool ActiveBackend::wait(int timeout)
{
    if (not this->isNull())
        return d->wait(timeout);
    else
        return true;
}

/** Return the current progress of the work */
float ActiveBackend::progress()
{
    if (this->isNull())
        return 0;

    QMutexLocker lkr( &(d->datamutex) );
    return d->workpacket.progress();
}

/** Return the current interim result - this will be a null
    workpacket if there is no work being run */
WorkPacket ActiveBackend::interimResult()
{
    if (this->isNull())
        return WorkPacket();

    QMutexLocker lkr( &(d->datamutex) );
    return d->workpacket;
}

/** Return the result of the work. You must call this to collect
    the result of the work, so that the backend can be released
    to perform more work */
WorkPacket ActiveBackend::result()
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
