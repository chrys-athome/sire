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

#ifndef SIRECLUSTER_FRONTEND_H
#define SIRECLUSTER_FRONTEND_H

#include "Siren/handle.h"
#include "Siren/mutex.h"

#include "backend.h"

#include <QUuid>

#include <boost/noncopyable.hpp>

SIRE_BEGIN_HEADER

namespace SireCluster
{

class Frontend;
class Backend;

class WorkPacket;

/** This is the base class of all Frontends - a Frontend is an object
    that you can use locally that can control a Backend that is either
    local or remote. A Frontend is a resource that is held by either
    ActiveFrontend or DormantFrontend. It may not be used on its own.
    
    Backends provide the compute resources that are available
    in a cluster. Frontends provide the means by which to 
    communicate with those resources.
    
    A Frontend can either be active or dormant. An dormant 
    frontend is connected to an active backend, but this frontend
    is currently not in use, and so is available (dormant frontends
    are held in the 'Nodes' object). An active frontend is in active
    use, and can be given jobs (an active frontend is held by a 
    'Node' object)
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Frontend : public boost::noncopyable
{
public:
    Frontend();
    
    virtual ~Frontend();
    
    virtual bool isLocal() const=0;
    
    const QUuid& UID() const;
    const QString& description() const;
    
    virtual void startJob(const WorkPacket &workpacket)=0;
    
    virtual void stopJob()=0;
    virtual void abortJob()=0;
    
    virtual void wait()=0;
    virtual bool wait(int timeout)=0;
    
    virtual float progress()=0;
    virtual WorkPacketPtr interimResult()=0;
    
    virtual WorkPacketPtr result()=0;

    void activate();
    bool tryActivate(int ms);
    
    void deactivate();

protected:
    void setUID(const QUuid &uid);
    void setDescription(const QString &description);

private:
    /** This mutex is locked when the Frontend is activated.
        It is unlocked when the mutex is deactivated */
    Siren::Mutex active_lock;
    
    /** A cache of the UID of the backend */
    QUuid backend_uid;
    
    /** Description of the backend */
    QString desc;
};

/** This is a local front end - this is a front end that communicates
    with a backend that is in this address space (i.e. a backend 
    that is running in the same process as this frontend) */
class SIRECLUSTER_EXPORT LocalFrontend : public Frontend
{
public:
    LocalFrontend();
    LocalFrontend(const ActiveBackend &backend);
    
    ~LocalFrontend();
    
    bool isLocal() const;
    
    void startJob(const WorkPacket &workpacket);
    
    void stopJob();
    void abortJob();
    
    void wait();
    bool wait(int timeout);
    
    float progress();
    WorkPacketPtr interimResult();
    
    WorkPacketPtr result();
    
private:
    /** The backend to which this frontend is connected */
    ActiveBackend backend;
};

class ActiveFrontend;

/** This is a holder for a Frontend that is not active - a non-active
    frontend cannot do anything
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT DormantFrontend 
    : public Siren::ImplementsHandle< DormantFrontend, Siren::Handles<Frontend> >
{
public:
    DormantFrontend();
    DormantFrontend(Frontend *frontend);
    
    DormantFrontend(const DormantFrontend &other);
    
    ~DormantFrontend();
    
    DormantFrontend& operator=(const DormantFrontend &other);
    
    bool operator==(const DormantFrontend &other) const;
    bool operator!=(const DormantFrontend &other) const;
    
    const QUuid& UID() const;
    const QString& description() const;
    
    QString toString() const;
    uint hashCode() const;
    
    ActiveFrontend activate();
    ActiveFrontend tryActivate(int ms);
};

/** This is the holder for an active Frontend - an active frontend
    is one that can send work to the backend for processing
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT ActiveFrontend
        : public Siren::ImplementsHandle< ActiveFrontend,Siren::Handles<Frontend> >
{
public:
    ActiveFrontend();
    
    ActiveFrontend(const ActiveFrontend &other);
    
    ~ActiveFrontend();
    
    ActiveFrontend& operator=(const ActiveFrontend &other);
    
    bool operator==(const ActiveFrontend &other) const;
    bool operator!=(const ActiveFrontend &other) const;
    
    QString toString() const;
    uint hashCode() const;
    
    bool isLocal() const;
    
    const QUuid& UID() const;
    const QString& description() const;
    
    void startJob(const WorkPacket &workpacket);
    
    void stopJob();
    void abortJob();
    
    void wait();
    bool wait(int timeout);
    
    float progress();
    WorkPacketPtr interimResult();
    
    WorkPacketPtr result();

protected:
    friend class DormantFrontend;
    ActiveFrontend(const DormantFrontend &frontend);
    
private:
    class ActiveToken
    {
    public:
        ActiveToken(Frontend *frontend);
        ~ActiveToken();
    
    private:
        Frontend *frontend;
    };

    boost::shared_ptr<ActiveToken> active_token;
};

}

Q_DECLARE_METATYPE( SireCluster::DormantFrontend )
Q_DECLARE_METATYPE( SireCluster::ActiveFrontend )

SIRE_END_HEADER

#endif
