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

#include <boost/weak_ptr.hpp>

#include "backend.h"
#include "cluster.h"
#include "workpacket.h"

#include "SireMaths/rangenerator.h"

#include "Siren/forages.h"
#include "Siren/errors.h"

#include <QDebug>

using namespace SireCluster;
using namespace Siren;

//////
////// Implementation of Backend
//////

/** Create the Backend whose resource description is in 'description' */
Backend::Backend(const QString &description) 
        : is_active(1), desc(description)
{}

/** Destructor */
Backend::~Backend()
{}

/** Internal function called by the resource manager to set 
    the UID of this backend */
void Backend::setUID(const QUuid &id)
{
    uid = id;
}

/** Return the UID of the backend */
const QUuid& Backend::UID() const
{
    return uid;
}

/** Return whether or not this backend is registered - you can't
    start any jobs on an unregistered backend */
bool Backend::isRegistered() const
{
    return not uid.isNull();
}

/** Assert that the backend is registered

    \throw Siren::unavailable_resource
*/
void Backend::assertIsRegistered() const
{
    if (uid.isNull())
    {
        throw Siren::unavailable_resource( QObject::tr(
            "The resource \"%1\" is no longer available. It either has "
            "not been registered, or it has been unregistered (possibly "
            "because this resource is about to be released or has been "
            "disconnected?").arg(desc), CODELOC );
    }
}

/** Return the resource description of the backend */
const QString& Backend::description() const
{
    return desc;
}

/** Activate this backend */
void Backend::activate()
{
    is_active.acquire();
}

/** Try to activate this backend - this returns
    immediately if this backend is already active */
bool Backend::tryActivate()
{
    return is_active.tryAcquire();
}

/** Try to activate this backend, returning whether
    or not we were successful */
bool Backend::tryActivate(int ms)
{
    return is_active.tryAcquire(1, ms);
}

/** Return whether or not this backend is activated */
bool Backend::isActivated() const
{
    return is_active.available() == 0;
}

/** Deactivate this backend */
void Backend::deactivate()
{
    is_active.release();
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
    
    resultpacket = job_in_progress;
}

/** Return whether or not this backend is busy */
bool LocalBackend::isBusy()
{
    return resultpacket.isNull();
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
    WorkPacketPtr final_result = resultpacket;
    resultpacket = WorkPacketPtr();
    return final_result;
}

//////
////// Implementation of ThreadBackend
//////

/** Construct a new thread backend */
ThreadBackend::ThreadBackend() : Backend("thread"), 
                                 Thread( QObject::tr("ThreadBackend") ),
                                 job_is_starting(false)
{}

/** Destructor */
ThreadBackend::~ThreadBackend()
{}

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
    MutexLocker lkr( &startmutex );

    while (job_is_starting)
    {
        //another thread is trying to start a job - we have to wait
        Siren::msleep(100);
    }
    
    //set the flag to say that *we* are starting a job
    job_is_starting = true;
    
    //wait until the last job has finished
    Thread::wait();
    
    //ok, we now know that we are the only thread trying to start
    //a job, and we know that no job is currently running
    {
        MutexLocker lkr2( &datamutex );

        while (not resultpacket.isNull())
        {
            //the user has not retrieved the results of the last job!
            Siren::msleep( 250 );
        }

        job_in_progress = workpacket;
        resultpacket = WorkPacketPtr();
        keep_running = true;
    }
    
    //start the job
    this->start();
}

/** Return if this backend is busy */
bool ThreadBackend::isBusy()
{
    return Thread::isRunning();
}

/** Stop the job */
void ThreadBackend::stopJob()
{
    MutexLocker lkr(&datamutex);
    keep_running = false;
}

/** Abort the job */
void ThreadBackend::abortJob()
{
    MutexLocker lkr(&datamutex);
    keep_running = false;
    resultpacket = AbortPacket();
    job_in_progress = resultpacket;
}

/** Wait for the workpacket to complete */
void ThreadBackend::wait()
{
    Thread::wait();
}

/** Wait for the workpacket to complete */
bool ThreadBackend::wait(int timeout)
{
    return Thread::wait(timeout);
}

/** Return the progress of the calculation */
float ThreadBackend::progress()
{
    MutexLocker lkr(&datamutex);
    return job_in_progress.read().progress();
}

/** Return an interim result */
WorkPacketPtr ThreadBackend::interimResult()
{
    MutexLocker lkr(&datamutex);
    return job_in_progress;
}

/** Return the final result - this will block until the 
    result is ready - you must call this to collect 
    the result of some work as this compute thread
    will be blocked until the result is collected */
WorkPacketPtr ThreadBackend::result()
{
    MutexLocker lkr(&datamutex);
    
    if (job_in_progress.isNull())
        //there is nothing being run
        return WorkPacketPtr();
    
    lkr.unlock();
    
    //wait for the job to finish
    Thread::wait();
    
    lkr.relock();
    
    if (resultpacket.isNull())
        //someone beat us to the result
        return WorkPacketPtr();
    else
    {
        WorkPacketPtr final_result = resultpacket;
        job_in_progress = WorkPacketPtr();
        resultpacket = WorkPacketPtr();
        
        return final_result;
    }
}

/** Function run by the backend thread */
void ThreadBackend::threadMain()
{
    if (not job_is_starting)
    {
        qDebug() << Siren::getPIDString() << "How have we been started???"
                 << "job_is_started is false in BackendPvt::run()";

        Thread::signalStarted();
                 
        return;
    }
    
    job_is_starting = false;
    Thread::signalStarted();
    
    while (true)
    {
        try
        {
            WorkPacketPtr local_packet;
        
            //// copy the work packet into a local space
            {
                MutexLocker lkr(&datamutex);
            
                if (not keep_running)
                    break;
                
                if (job_in_progress.read().hasFinished())
                    break;
                
                local_packet = job_in_progress;
            }
        
            //now perform the work on the local packet
            if (not local_packet.read().hasFinished())
            {
                local_packet = local_packet.read().runChunk();
            }
        
            //// copy the local work back to the global work
            {
                MutexLocker lkr(&datamutex);
            
                job_in_progress = local_packet;
            
                if (job_in_progress.read().hasFinished() or not keep_running)
                    break;
            }
        }
        catch(const Siren::exception &e)
        {
            MutexLocker lkr(&datamutex);
            job_in_progress = ErrorPacket(e);
            break;
        }
        catch(const std::exception &e)
        {
            MutexLocker lkr(&datamutex);
            job_in_progress = ErrorPacket(Siren::std_exception(e));
            break;
        }
        catch(...)
        {
            MutexLocker lkr(&datamutex);
            job_in_progress = ErrorPacket(Siren::unknown_error( QObject::tr(
                    "An unknown error occured while running a workpacket."),
                        CODELOC ) );
            break;
        }
    }
    
    //the work has finished - copy the results
    MutexLocker lkr(&datamutex);
    resultpacket = job_in_progress;
}

//////
////// Implementation of DormantBackend
//////

/** Null constructor */
DormantBackend::DormantBackend()
               : ImplementsHandle< DormantBackend,Handles<Backend> >()
{}

/** Construct a holder for the passed backend - this takes over
    ownership of the backend */
DormantBackend::DormantBackend(Backend *backend)
               : ImplementsHandle< DormantBackend,Handles<Backend> >(backend)
{}

/** Copy constructor */
DormantBackend::DormantBackend(const DormantBackend &other)
               : ImplementsHandle< DormantBackend,Handles<Backend> >(other)
{}
  
/** Destructor */
DormantBackend::~DormantBackend()
{}

/** Copy assignment operator */
DormantBackend& DormantBackend::operator=(const DormantBackend &other)
{
    Handles<Backend>::operator=(other);
    return *this;
}

/** Comparison operator */
bool DormantBackend::operator==(const DormantBackend &other) const
{
    return Handles<Backend>::operator==(other);
}

/** Comparison operator */
bool DormantBackend::operator!=(const DormantBackend &other) const
{
    return Handles<Backend>::operator!=(other);
}

QString DormantBackend::toString() const
{
    if (isNull())
        return QObject::tr("DormantBackend::null");
    else
        return QObject::tr( "DormantBackend( UID = %1, description = %2 )")
                    .arg(resource().UID().toString())
                    .arg(resource().description());
}

uint DormantBackend::hashCode() const
{
    if (this->isNull())
        return qHash(DormantBackend::typeName());
    else
        return qHash(DormantBackend::typeName()) + qHash(resource().UID());
}

QUuid DormantBackend::UID() const
{
    if (isNull())
        return QUuid();
    else
        return resource().UID();
}

void DormantBackend::setUID(const QUuid &uid) const
{
    if (not isNull())
        const_cast<Backend*>(&(resource()))->setUID(uid);
}

QString DormantBackend::description() const
{
    if (isNull())
        return QString::null;
    else
        return resource().description();
}

/** Activate this backend - this blocks until the backend
    is ready to be activated (and is not in use elsewhere) */
ActiveBackend DormantBackend::activate() const
{
    if (isNull())
        return ActiveBackend();
        
    const_cast<Backend*>(&(resource()))->activate();
    
    return ActiveBackend(*this);
}

/** Return whether or not this backend is currently active */
bool DormantBackend::isActivated() const
{
    if (isNull())
        return false;
        
    return resource().isActivated();
}

/** Try to activate this backend - this returns a null
    ActiveBackend immediately if the backend cannot 
    be activated */
ActiveBackend DormantBackend::tryActivate() const
{
    if (isNull())
        return ActiveBackend();
        
    if (const_cast<Backend*>(&(resource()))->tryActivate())
        return ActiveBackend(*this);
    else
        return ActiveBackend();
}

/** Try to activate this backend - this blocks until the backend
    is ready to be activated (and is not in use elsewhere)
    or 'ms' milliseconds have passed. If the backend cannot
    be activated then a null active backend is returned */
ActiveBackend DormantBackend::tryActivate(int ms) const
{
    if (isNull())
        return ActiveBackend();
        
    if (const_cast<Backend*>(&(resource()))->tryActivate(ms))
    {
        return ActiveBackend(*this);
    }
    else
    {
        return ActiveBackend();
    }
}

//////
////// Implementation of ActiveBackend
//////

ActiveBackend::ActiveToken::ActiveToken(Backend *b) : backend(b)
{}

ActiveBackend::ActiveToken::~ActiveToken()
{
    if (backend)
        backend->deactivate();
}

/** Null constructor */
ActiveBackend::ActiveBackend()
              : ImplementsHandle< ActiveBackend,Handles<Backend> >()
{}

/** Internal function called by DormantBackend::activate or
    DormantBackend::tryActivate used to activate a dormant backend */
ActiveBackend::ActiveBackend(const DormantBackend &other)
              : ImplementsHandle< ActiveBackend,Handles<Backend> >(other)
{
    if (not isNull())
        active_token.reset( new ActiveToken( &(resource()) ) );
}

/** Copy constructor */
ActiveBackend::ActiveBackend(const ActiveBackend &other)
              : ImplementsHandle< ActiveBackend,Handles<Backend> >(other),
                active_token(other.active_token)
{}

/** Destructor */
ActiveBackend::~ActiveBackend()
{}

/** Copy assignment operator */
ActiveBackend& ActiveBackend::operator=(const ActiveBackend &other)
{
    active_token = other.active_token;
    Handles<Backend>::operator=(other);
    return *this;
}

/** Comparison operator */
bool ActiveBackend::operator==(const ActiveBackend &other) const
{
    return Handles<Backend>::operator==(other);
}

/** Comparison operator */
bool ActiveBackend::operator!=(const ActiveBackend &other) const
{
    return Handles<Backend>::operator!=(other);
}

QString ActiveBackend::toString() const
{
    if (isNull())
        return QObject::tr("ActiveBackend::null");
    else
        return QObject::tr( "ActiveBackend( UID = %1, description = %2 )")
                    .arg(resource().UID().toString())
                    .arg(resource().description());
}

uint ActiveBackend::hashCode() const
{
    if (this->isNull())
        return qHash(ActiveBackend::typeName());
    else
        return qHash(ActiveBackend::typeName()) + qHash(resource().UID());
}

QUuid ActiveBackend::UID() const
{
    if (isNull())
        return QUuid();
    else
        return resource().UID();
}

QString ActiveBackend::description() const
{
    if (isNull())
        return QString::null;
    else
        return resource().description();
}

/** Wait until the backend has finished processing */
void ActiveBackend::wait()
{
    if (not isNull())
        resource().wait();
}

/** Wait until the backend has finished processing, or
    until 'ms' milliseconds have passed. Return whether or
    not the backend finished */
bool ActiveBackend::wait(int ms)
{
    if (isNull())
        return true;
    else
        return resource().wait(ms);
}

/** Start running the job in 'workpacket' on this backend */
void ActiveBackend::startJob(const WorkPacket &workpacket)
{
    if (isNull())
        return;

    HandleLocker lkr(*this);
    
    while (resource().isBusy())
    {
        lkr.unlock();
        resource().wait();
        lkr.relock();
    }

    resource().startJob(workpacket);
}

/** Stop any running job */
void ActiveBackend::stopJob()
{
    if (isNull())
        return;
        
    HandleLocker lkr(*this);
    resource().stopJob();
}

/** Abort any running job */
void ActiveBackend::abortJob()
{
    if (isNull())
        return;
        
    HandleLocker lkr(*this);
    resource().abortJob();
}

/** Return the progress of the job */
float ActiveBackend::progress()
{
    if (isNull())
        return 0;
    else
    {
        HandleLocker lkr(*this);
        return resource().progress();
    }
}

/** Return the current state of the result */
WorkPacketPtr ActiveBackend::interimResult()
{
    if (isNull())
        return WorkPacketPtr();
    else
    {
        HandleLocker lkr(*this);
        return resource().interimResult();
    }
}

/** Return the final result of the calculation */
WorkPacketPtr ActiveBackend::result()
{
    if (isNull())
        return WorkPacketPtr();
        
    HandleLocker lkr(*this);
    
    while (resource().isBusy())
    {
        lkr.unlock();
        resource().wait();
        lkr.relock();
    }
    
    return resource().result();
}
