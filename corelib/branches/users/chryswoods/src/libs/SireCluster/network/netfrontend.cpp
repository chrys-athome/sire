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

#include "netfrontend.h"
#include "communicator.h"
#include "nodemessages.h"

#include "Siren/datastream.h"

#include "SireCluster/errors.h"
#include "Siren/errors.h"

using namespace SireCluster;
using namespace SireCluster::network;
using namespace SireCluster::resources;
using namespace Siren;

/** Construct to talk with the resource identified by 'netkey'
    on the host 'host' */
NetFrontend::NetFrontend(const QUuid &h, const QUuid &n)
            : Frontend(), host(h), netkey(n)
{
    quint64 msgid = Communicator::send( GetResourceInfo(netkey), host );
    Reply reply = Communicator::awaitReply(host, msgid);

    if (reply.isError())
        reply.throwError();

    else if (reply.isEmpty())
        throw SireCluster::network_error( QObject::tr(
                "Unable to get information about the resource running "
                "on %1.").arg(host.toString()), CODELOC );

    DataStream ds( reply.contents() );
    
    QUuid resource_uid;
    QString resource_description;
    
    ds >> resource_uid >> resource_description;
    
    Frontend::setUID(resource_uid);
    Frontend::setDescription(resource_description);
}

/** Destructor - this blocks while the resource is disconnected */
NetFrontend::~NetFrontend()
{
    Communicator::send( DisconnectResource(netkey), host );
}

/** This is not a local resource! */
bool NetFrontend::isLocal() const
{
    return false;
}

/** Start the job contained in the passed workpacket */
void NetFrontend::startJob(const WorkPacket &workpacket)
{
    quint64 msgid = Communicator::send( StartJob(netkey,workpacket), host );
    
    Reply reply = Communicator::awaitReply(host, msgid);
    
    if (reply.isError())
        reply.throwError();
}

/** Stop any running job on the resource */
void NetFrontend::stopJob()
{
    Communicator::send( StopJob(netkey), host );
}

/** Abort any running job on the resource */
void NetFrontend::abortJob()
{
    Communicator::send( AbortJob(netkey), host );
}

/** Wait until the job currently running on the resource
    has finished */
void NetFrontend::wait()
{
    quint64 msgid = Communicator::send( WaitForJob(netkey), host );
    
    Reply reply = Communicator::awaitReply(host, msgid);
    
    if (reply.isError())
        reply.throwError();
}

/** Wait until the job currently running on the resource 
    has finished, or 'timeout' milliseconds has passed.
    This returns whether or not the job has finished */
bool NetFrontend::wait(int timeout)
{
    quint64 msgid = Communicator::send( WaitForJob(netkey,timeout), host );
    
    Reply reply = Communicator::awaitReply(host, msgid);
    
    if (reply.isError())
        reply.throwError();
    
    else if (reply.isEmpty())
        return true;
        
    bool finished;
    
    DataStream ds(reply.contents());
    
    ds >> finished;
    
    return finished;
}

/** Return the progress made on the current job */
float NetFrontend::progress()
{
    quint64 msgid = Communicator::send( GetProgress(netkey), host );
    
    Reply reply = Communicator::awaitReply(host, msgid);
    
    if (reply.isError())
        reply.throwError();
    
    else if (reply.isEmpty())
        return 0;
        
    float p;
    
    DataStream ds(reply.contents());
    
    ds >> p;
    
    return p;
}

/** Return the current state of the job */
WorkPacketPtr NetFrontend::interimResult()
{
    quint64 msgid = Communicator::send( GetResult(netkey,false), host );
    
    Reply reply = Communicator::awaitReply(host, msgid);
    
    if (reply.isError())
        reply.throwError();
    
    else if (reply.isEmpty())
        return WorkPacketPtr();
        
    DataStream ds(reply.contents());
    
    return ds.loadNextObject().asA<WorkPacket>();
}

/** Return the final result of the currently running job 
     - this blocks until the job has finished running */
WorkPacketPtr NetFrontend::result()
{
    quint64 msgid = Communicator::send( GetResult(netkey,true), host );
    
    Reply reply = Communicator::awaitReply(host, msgid);
    
    if (reply.isError())
        reply.throwError();
    
    else if (reply.isEmpty())
        return WorkPacketPtr();
    
    DataStream ds(reply.contents());
    
    return ds.loadNextObject().asA<WorkPacket>();
}
