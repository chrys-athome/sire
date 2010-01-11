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

#ifndef SIRECLUSTER_BACKEND_H
#define SIRECLUSTER_BACKEND_H

#include <boost/shared_ptr.hpp>

#include <QThread>
#include <QUuid>

#include "workpacket.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{

/** This is the base class of all backends. A Backend is an object
    that can receive a WorkPacket from a FrontEnd, can process that
    WorkPacket, and then returns the processed WorkPacket back to
    the FrontEnd.
    
    A Backend is the resource that is held by DormantBackend and
    ActiveBackend, and should not be used on its own
    
    Backends provide the compute resources that are available
    in a cluster. Frontends provide the means by which to 
    communicate with those resources.
    
    Backends can either be active or dormant. A dormant backend
    represents a compute resource that is available, but that 
    has not yet been assigned to any cluster. Active backends
    are those that have been assigned to a cluster, and that
    are connected to by the Frontends held in the Node and Nodes
    objects.
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Backend : public boost::noncopyable
{
public:
    Backend(const QString &description);
    virtual ~Backend();
    
    const QUuid& UID() const;
    const QString& description() const;
    
    virtual QString what() const=0;
    
    virtual void startJob(const WorkPacket &workpacket)=0;
    
    virtual void stopJob()=0;
    virtual void abortJob()=0;
    
    virtual void wait()=0;
    virtual bool wait(int timeout)=0;
    
    virtual float progress()=0;
    virtual WorkPacketPtr interimResult()=0;
    
    virtual WorkPacketPtr result()=0;

private:
    /** The unique ID for this backend */
    QUuid uid;

    /** A description of this backend - this can be used
        as by the user to limit their choice of node */
    QString desc;
};

/** This is a backend that runs in the current thread */
class SIRECLUSTER_EXPORT LocalBackend : public Backend
{
public:
    LocalBackend();
    ~LocalBackend();
    
    QString what() const;
    
    void startJob(const WorkPacket &workpacket);
    
    void stopJob();
    void abortJob();
    
    void wait();
    bool wait(int timeout);
    
    float progress();
    WorkPacketPtr interimResult();
    
    WorkPacketPtr result();

private:
    WorkPacketPtr job_in_progress;
};

/** This is a backend that runs the job in its own thread */
class SIRECLUSTER_EXPORT ThreadBackend : public Backend, private QThread
{
public:
    ThreadBackend();
    ~ThreadBackend();
    
    QString what() const;
    
    void startJob(const WorkPacket &workpacket);
    
    void stopJob();
    void abortJob();
    
    void wait();
    bool wait(int timeout);
    
    float progress();
    WorkPacketPtr interimResult();
    
    WorkPacketPtr result();

protected:
    void run();

private:
    /** Mutex used to protect access to the running thread */
    QMutex datamutex;

    /** This mutex is used to ensure that only one 
        thread can try to start a job at a time */
    QMutex startmutex;
    
    /** WaitCondition used to signal that the backend thread 
        has started */
    QWaitCondition startwaiter;

    /** The current state of the job */
    WorkPacketPtr job_in_progress;
    
    /** The final result of the job */
    WorkPacketPtr result;
    
    /** Whether or not to keep running */
    bool keep_running;
    
    /** A flag used to indicate whether or not a job is starting */
    bool job_is_starting;
};

class ActiveBackend;

/** This class holds a backend while it is not in use */
class SIRECLUSTER_EXPORT DormantBackend
      : public Siren::ImplementsHandle< DormantBackend, Siren::Handles<Backend> >
{
public:
    DormantBackend();
    DormantBackend(const DormantBackend &other);
    
    ~DormantBackend();
    
    DormantBackend& operator=(const DormantBackend &other);
    
    bool operator==(const DormantBackend &other) const;
    bool operator!=(const DormantBackend &other) const;
    
    QString toString() const;
    uint hashCode() const;
    
    QUuid UID() const;
    QString description() const;
    
    ActiveBackend activate();
    ActiveBackend tryActivate(int ms);
};

/** This class holds a backend that has been activate for use */
class SIRECLUSTER_EXPORT ActiveBackend
      : public Siren::ImplementsHandle< ActiveBackend, Siren::Handles<Backend> >
{
public:
    ActiveBackend();
    ActiveBackend(const ActiveBackend &other);
    
    ~ActiveBackend();
    
    ActiveBackend& operator=(const ActiveBackend &other);
    
    bool operator==(const ActiveBackend &other) const;
    bool operator!=(const ActiveBackend &other) const;
    
    DormantBackend deactivate();
    DormantBackend tryDeactivate(int ms);
    
    QString toString() const;
    uint hashCode() const;
    
    QUuid UID() const;
    QString description() const;
    
    void startJob(const WorkPacket &workpacket);
    
    void stopJob();
    void abortJob();
    
    void wait();
    bool wait(int timeout);
    
    float progress();
    WorkPacketPtr interimResult();
    
    WorkPacketPtr result();
};

}

Q_DECLARE_METATYPE( SireCluster::DormantBackend )
Q_DECLARE_METATYPE( SireCluster::ActiveBackend )

SIRE_END_HEADER

#endif
