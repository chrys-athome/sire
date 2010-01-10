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

#include "sireglobal.h"

#include <boost/shared_ptr.hpp>

#include <QUuid>

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
    Backend();
    virtual ~Backend();
    
    QUuid UID() const;
    
    virtual void startJob(const WorkPacket &workpacket)=0;
    
    virtual void stopJob()=0;
    virtual void abortJob()=0;
    
    virtual void wait()=0;
    virtual bool wait(int timeout)=0;
    
    virtual float progress()=0;
    virtual WorkPacket interimResult()=0;
    
    virtual WorkPacket result()=0;

private:
    /** The unique ID for this backend */
    QUuid uid;
};

/** This is a backend that runs in the current thread */
class SIRECLUSTER_EXPORT LocalBackend : public Backend
{
public:
    LocalBackend();
    ~LocalBackend();
    
    void startJob(const WorkPacket &workpacket);
    
    void stopJob();
    void abortJob();
    
    void wait();
    bool wait(int timeout);
    
    float progress();
    WorkPacket interimResult();
    
    WorkPacket result();
};

/** This is a backend that runs the job in its own thread */
class SIRECLUSTER_EXPORT ThreadBackend : public Backend, private QThread
{
public:
    ThreadBackend();
    ~ThreadBackend();
    
    QUuid UID() const;
    
    void startJob(const WorkPacket &workpacket);
    
    void stopJob();
    void abortJob();
    
    void wait();
    bool wait(int timeout);
    
    float progress();
    WorkPacket interimResult();
    
    WorkPacket result();

protected:
    void run();
};

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
    
    QUuid UID() const;
    
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
    
    QUuid UID() const;
    
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
