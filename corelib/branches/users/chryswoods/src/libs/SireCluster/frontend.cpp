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

#include "frontend.h"
#include "workpacket.h"

#include <QDebug>

using namespace SireCluster;
using namespace Siren;

/////////
///////// Implementation of Frontend
/////////

/** Constructor */
Frontend::Frontend() : boost::noncopyable()
{}

/** Destructor */
Frontend::~Frontend()
{}

/** Call this function is signal that the front end is to 
    be activated */
void Frontend::activate()
{
    active_lock.lock();
}

/** Call this function to try to activate this front end */
bool Frontend::tryActivate(int ms)
{
    return active_lock.tryLock(ms);
}

/** Call this function when the front end has been deactivated */
void Frontend::deactivate()
{
    active_lock.unlock();
}

/** Internal function called by derived classes to set the UID
    of the backend */
void Frontend::setUID(const QUuid &uid)
{
    backend_uid = uid;
}

/** Return the (locally cached) UID of the backend to which this
    frontend is connected */
const QUuid& Frontend::UID() const
{
    return backend_uid;
}

/** Return the (locally cached) description of the backend to 
    which this frontend is connected */
const QString& Frontend::description() const
{
    return desc;
}

/////////
///////// Implementation of LocalFrontend
/////////

/** Construct a Frontend for the local Backend 'backend' */
LocalFrontend::LocalFrontend(const ActiveBackend &b)
              : Frontend(), backend(b)
{
    this->setUID(backend.UID());
    this->setDescription(backend.description());
}

/** Destructor */
LocalFrontend::~LocalFrontend()
{}

bool LocalFrontend::isLocal() const
{
    return true;
}

void LocalFrontend::startJob(const WorkPacket &workpacket)
{
    backend.startJob(workpacket);
}

void LocalFrontend::stopJob()
{
    backend.stopJob();
}

void LocalFrontend::abortJob()
{
    backend.abortJob();
}

void LocalFrontend::wait()
{
    backend.wait();
}

bool LocalFrontend::wait(int timeout)
{
    return backend.wait(timeout);
}

float LocalFrontend::progress()
{
    return backend.progress();
}

WorkPacketPtr LocalFrontend::interimResult()
{
    return backend.interimResult();
}

WorkPacketPtr LocalFrontend::result()
{
    return backend.result();
}

/////////
///////// Implementation of DormantFrontend
/////////

/** Null constructor */
DormantFrontend::DormantFrontend()
                : ImplementsHandle< DormantFrontend,Handles<Frontend> >()
{}

/** Construct to hold the frontend 'frontend' (which must be connected to 
    an active backend) */
DormantFrontend::DormantFrontend(Frontend *frontend)
                : ImplementsHandle< DormantFrontend,Handles<Frontend> >(frontend)
{}

/** Copy constructor */
DormantFrontend::DormantFrontend(const DormantFrontend &other)
                : ImplementsHandle< DormantFrontend,Handles<Frontend> >(other)
{}

/** Destructor */
DormantFrontend::~DormantFrontend()
{}

/** Copy assignment operator */
DormantFrontend& DormantFrontend::operator=(const DormantFrontend &other)
{
    Handles<Frontend>::operator=(other);
    return *this;
}

/** Comparison operator */
bool DormantFrontend::operator==(const DormantFrontend &other) const
{
    return Handles<Frontend>::operator==(other);
}

/** Comparison operator */
bool DormantFrontend::operator!=(const DormantFrontend &other) const
{
    return Handles<Frontend>::operator!=(other);
}

QString DormantFrontend::toString() const
{
    if (this->isNull())
        return QObject::tr( "DormantFrontend::null" );
    else
        return QObject::tr( "DormantFrontend( UID = %1, description = %2 )")
                    .arg(resource().UID().toString())
                    .arg(resource().description());
}

uint DormantFrontend::hashCode() const
{
    if (this->isNull())
        return qHash(DormantFrontend::typeName());
    else
        return qHash(DormantFrontend::typeName()) + qHash(resource().UID());
}

static QUuid null_uid;
static QString null_description;

/** Return the UID of the backend */
const QUuid& DormantFrontend::UID() const
{
    if (not isNull())
        return resource().UID();

    else
        return null_uid;
}

/** Return a description of the backend */
const QString& DormantFrontend::description() const
{
    if (not isNull())
        return resource().description();
    else
        return null_description;
}

/** Activate this frontend - this will block until the frontend
    is free to be activated */
ActiveFrontend DormantFrontend::activate()
{
    if (isNull())
        return ActiveFrontend();
        
    resource().activate();
    
    return ActiveFrontend(*this);
}

/** Try to activate this frontend - this will block until the
    frontend has been activated or until ms milliseconds has
    passed. If the frontend wasn't activated then a null
    ActiveFrontend is returned */
ActiveFrontend DormantFrontend::tryActivate(int ms)
{
    if (isNull())
        return ActiveFrontend();
        
    if (resource().tryActivate(ms))
        return ActiveFrontend(*this);
    else
        return ActiveFrontend();
}

/////////
///////// Implementation of ActiveFrontend
/////////

/** Null constructor */
ActiveFrontend::ActiveFrontend() 
               : ImplementsHandle< ActiveFrontend,Handles<Frontend> >() 
{}

ActiveFrontend::ActiveToken::ActiveToken(Frontend *f) : frontend(f)
{}

ActiveFrontend::ActiveToken::~ActiveToken()
{
    if (frontend)
        frontend->deactivate();
}

/** Internal constructor used by DormantFrontend in activate() and tryActivate() */
ActiveFrontend::ActiveFrontend(const DormantFrontend &frontend)
               : ImplementsHandle< ActiveFrontend,Handles<Frontend> >(frontend)
{
    if (not this->isNull())
        active_token.reset( new ActiveToken( &(resource()) ) );
}

/** Copy constructor */
ActiveFrontend::ActiveFrontend(const ActiveFrontend &other)
               : ImplementsHandle< ActiveFrontend,Handles<Frontend> >(other),
                 active_token(other.active_token)
{}

/** Destructor */
ActiveFrontend::~ActiveFrontend()
{
    //ensure that the token is lost first
    active_token.reset();
}

/** Copy assignment operator */
ActiveFrontend& ActiveFrontend::operator=(const ActiveFrontend &other)
{
    if (this != &other)
    {
        active_token = other.active_token;
        Handles<Frontend>::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool ActiveFrontend::operator==(const ActiveFrontend &other) const
{
    return Handles<Frontend>::operator==(other);
}

/** Comparison operator */
bool ActiveFrontend::operator!=(const ActiveFrontend &other) const
{
    return Handles<Frontend>::operator!=(other);
}

QString ActiveFrontend::toString() const
{
    if (this->isNull())
        return QObject::tr( "ActiveFrontend::null" );
    else
        return QObject::tr( "ActiveFrontend( UID = %1, description = %2 )")
                    .arg(resource().UID().toString())
                    .arg(resource().description());
}

uint ActiveFrontend::hashCode() const
{
    if (this->isNull())
        return qHash(ActiveFrontend::typeName());
    else
        return qHash(ActiveFrontend::typeName()) + qHash(resource().UID());
}

/** Return whether or not the Backend is local (running
    in the same address space as the Frontend) */
bool ActiveFrontend::isLocal() const
{
    if (isNull())
        return false;
        
    else
        return resource().isLocal();
}

/** Return the UID of the backend */
const QUuid& ActiveFrontend::UID() const
{
    if (not isNull())
        return resource().UID();

    else
        return null_uid;
}

/** Return a description of the backend */
const QString& ActiveFrontend::description() const
{
    if (not isNull())
        return resource().description();
    else
        return null_description;
}

/** Perform the work in 'workpacket' on the backend - this 
    blocks until the work has started */
void ActiveFrontend::startJob(const WorkPacket &workpacket)
{
    if (not isNull())
    {
        HandleLocker lkr(*this);
        resource().startJob(workpacket);
    }
}

/** Stop the job running on the backend, and return the
    workpacket in the state it is now at now that the job
    has stopped */
void ActiveFrontend::stopJob()
{
    if (not isNull())
    {
        HandleLocker lkr(*this);
        resource().stopJob();
    }
}

/** Abort the job running on the backend and return the
    state of the work once it has been aborted */
void ActiveFrontend::abortJob()
{
    if (not isNull())
    {
        HandleLocker lkr(*this);
        resource().abortJob();
    }
}

/** Wait until the backend has finished processing the work */
void ActiveFrontend::wait()
{
    if (not isNull())
        resource().wait();
}

/** Wait until the backend has finished processing the work, or
    until 'timeout' milliseconds have passed - this returns
    whether or not the job has finished */
bool ActiveFrontend::wait(int timeout)
{
    if (not isNull())
        return resource().wait(timeout);

    else
        return true;
}

/** Return the current progress of the job */
float ActiveFrontend::progress()
{
    if (not isNull())
    {
        HandleLocker lkr(*this);
        return resource().progress(); 
    }
    else
        return 0;
}

/** Return the work as it is at the moment */
WorkPacketPtr ActiveFrontend::interimResult()
{
    if (not isNull())
    {
        HandleLocker lkr(*this);
        return resource().interimResult();
    }
    else
        return WorkPacketPtr();
}

/** Return the final result of the calculation */
WorkPacketPtr ActiveFrontend::result()
{
    if (not isNull())
    {
        HandleLocker lkr(*this);
        return resource().result();
    }
    else
        return WorkPacketPtr();
}
