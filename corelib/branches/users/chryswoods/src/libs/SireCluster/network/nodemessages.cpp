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

#include "nodemessages.h"
#include "communicator.h"
#include "netresourcemanager.h"
#include "netbackend.h"
#include "netfrontend.h"
#include "hostinfo.h"

#include "SireCluster/resources/resourcemanager.h"
#include "SireCluster/resources/backend.h"

#include "Siren/stream.h"
#include "Siren/streamqt.h"
#include "Siren/tostring.h"
#include "Siren/forages.h"
#include "Siren/errors.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::network;
using namespace SireCluster::resources;
using namespace Siren;

////////
//////// Implementation of ReserveRequest
////////

static const RegisterObject<ReserveRequest> r_reserve_request;

/** Null constructor */
ReserveRequest::ReserveRequest() 
               : Implements<ReserveRequest,Message>(),
                 nresources(0), expire_time(0)
{}

/** Construct a request to reserve one resource, requesting
    that the reservation will expire after 'expires' ms */
ReserveRequest::ReserveRequest(const QUuid &uid, int expires)
               : Implements<ReserveRequest,Message>(),
                 request_uid(uid), nresources(1), expire_time(expires)
{}

/** Construct a request to reserve one resource that matches
    the description 'description', requesting
    that the reservation will expire after 'expires' ms */
ReserveRequest::ReserveRequest(const QUuid &uid, QString description, int expires)
               : Implements<ReserveRequest,Message>(),
                 request_uid(uid), desc(description), nresources(1), expire_time(expires)
{}

/** Construct a request to reserve up to 'nresources' resources, requesting
    that the reservation will expire after 'expires' ms */
ReserveRequest::ReserveRequest(const QUuid &uid, int n, int expires)
               : Implements<ReserveRequest,Message>(),
                 request_uid(uid), nresources(n), expire_time(expires)
{}

/** Construct a request to reserve up to 'nresources' resources, 
    that all match the description 'description', requesting
    that the reservation will expire after 'expires' ms */
ReserveRequest::ReserveRequest(const QUuid &uid, int n, QString description, int expires)
               : Implements<ReserveRequest,Message>(),
                 request_uid(uid), desc(description), nresources(n), expire_time(expires)
{}

/** Copy constructor */
ReserveRequest::ReserveRequest(const ReserveRequest &other)
               : Implements<ReserveRequest,Message>(other),
                 request_uid(other.request_uid),
                 desc(other.desc), nresources(other.nresources),
                 expire_time(other.expire_time)
{}

/** Destructor */
ReserveRequest::~ReserveRequest()
{}

/** Copy assignment operator */
ReserveRequest& ReserveRequest::operator=(const ReserveRequest &other)
{
    if (this != &other)
    {
        super::operator=(other);
        request_uid = other.request_uid;
        desc = other.desc;
        nresources = other.nresources;
        expire_time = other.expire_time;
    }
    
    return *this;
}

/** Comparison operator */
bool ReserveRequest::operator==(const ReserveRequest &other) const
{
    return request_uid == other.request_uid and
           desc == other.desc and nresources == other.nresources and
           expire_time == other.expire_time and super::operator==(other);
}

/** Comparison operator */
bool ReserveRequest::operator!=(const ReserveRequest &other) const
{
    return not ReserveRequest::operator==(other);
}

uint ReserveRequest::hashCode() const
{
    return qHash( ReserveRequest::typeName() ) + qHash(desc)
             + qHash(nresources);
}

QString ReserveRequest::toString() const
{
    if (desc.isEmpty())
        return QObject::tr("ReserveRequest( nResources() == %1 )").arg(nresources);
    else
        return QObject::tr("ReserveRequest( description() == %1, nResources() == %2 )")
                    .arg(desc).arg(nresources);
}

void ReserveRequest::stream(Siren::Stream &s)
{
    s.assertVersion<ReserveRequest>(1);

    Schema schema = s.item<ReserveRequest>();

    schema.data("request_uid") & request_uid;
    schema.data("description") & desc;
    schema.data("nresources") & nresources;
    schema.data("expire_time") & expire_time;

    super::stream( schema.base() );
}

/** Read and act on this message */
void ReserveRequest::read(const QUuid &sender, quint64 message_id) const
{
    //reserve a node - this tries to get a node as quickly as
    //possible, but may block while other nodes are reserved
    QUuid reservation = ResourceManager::reserveResource(expire_time);
    
    //send a response back with the reservation UID
    ReserveResponse response(request_uid, reservation);
    
    Communicator::send( response, sender );
}

////////
//////// Implementation of ReserveResponse
////////

static const RegisterObject<ReserveResponse> r_reserve_response;

/** Null constructor */
ReserveResponse::ReserveResponse() : Implements<ReserveResponse,Message>()
{}

/** Construct to sent the reservation 'reservation_uid' back to the 
    the node, which made a request using a ReserveRequest message 
    with UID 'request_uid' */
ReserveResponse::ReserveResponse(const QUuid &request,
                                 const QUuid &reservation)
                : Implements<ReserveResponse,Message>(),
                  request_uid(request)
{
    if (not reservation.isNull())
        reservation_uids.append(reservation);
}
    
/** Construct to sent the reservations in 'reservation_uids' back to the 
    the node, which made a request using a ReserveRequest message 
    with UID 'request_uid' */
ReserveResponse::ReserveResponse(const QUuid &request, 
                                 const QList<QUuid> &reservations)
                : Implements<ReserveResponse,Message>(),
                  request_uid(request), reservation_uids(reservations)
{}
  
/** Copy constructor */              
ReserveResponse::ReserveResponse(const ReserveResponse &other)
                : Implements<ReserveResponse,Message>(),
                  request_uid(other.request_uid),
                  reservation_uids(other.reservation_uids)
{}

/** Destructor */
ReserveResponse::~ReserveResponse()
{}

/** Copy assignment operator */
ReserveResponse& ReserveResponse::operator=(const ReserveResponse &other)
{
    if (this != &other)
    {
        super::operator=(other);
        request_uid = other.request_uid;
        reservation_uids = other.reservation_uids;
    }
    
    return *this;
}

/** Comparison operator */
bool ReserveResponse::operator==(const ReserveResponse &other) const
{
    return request_uid == other.request_uid and 
           reservation_uids == other.reservation_uids and
           super::operator==(other);
}

/** Comparison operator */
bool ReserveResponse::operator!=(const ReserveResponse &other) const
{
    return not ReserveResponse::operator==(other);
}

uint ReserveResponse::hashCode() const
{
    return qHash(ReserveResponse::typeName()) 
             + qHash(request_uid) + reservation_uids.count();
}

QString ReserveResponse::toString() const
{
    if (reservation_uids.isEmpty())
        return QObject::tr("ReserveResponse( request == %1, nothing available )")
                        .arg(request_uid);
    else
        return QObject::tr("ReserveResponse( request == %1, reservation == %2 )")
                        .arg(request_uid)
                        .arg(Siren::toString(reservation_uids));
}

void ReserveResponse::stream(Siren::Stream &s)
{
    s.assertVersion<ReserveResponse>(1);
    
    Schema schema = s.item<ReserveResponse>();
    
    schema.data("request_uid") & request_uid;
    schema.data("reservation_uids") & reservation_uids;
    
    super::stream( schema.base() );
}

/** Read this message - this posts the reservation UIDs back into 
    the original ReserveRequest message */
void ReserveResponse::read(const QUuid &sender, quint64 message_id) const
{
    NetResourceManager::receivedReservation(sender, reservation_uids, request_uid);
}

////////
//////// Implementation of CancelReservation
////////

static const RegisterObject<CancelReservation> r_cancel_reservation;

/** Null constructor */
CancelReservation::CancelReservation() : Implements<CancelReservation,Message>()
{}

/** Construct to cancel the reservation with passed UID */
CancelReservation::CancelReservation(const QUuid &reservation_uid)
                  : Implements<CancelReservation,Message>()
{
    if (not reservation_uid.isNull())
        cancel_uids.append(reservation_uid);
}

/** Construct to cancel the reservations with passed UIDs */
CancelReservation::CancelReservation(const QList<QUuid> &reservation_uids)
                  : Implements<CancelReservation,Message>(),
                    cancel_uids(reservation_uids)
{}

/** Copy constructor */
CancelReservation::CancelReservation(const CancelReservation &other)
                  : Implements<CancelReservation,Message>(other),
                    cancel_uids(other.cancel_uids)
{}

/** Destructor */
CancelReservation::~CancelReservation()
{}

/** Copy assignmen operator */
CancelReservation& CancelReservation::operator=(const CancelReservation &other)
{
    if (this != &other)
    {
        super::operator=(other);
        cancel_uids = other.cancel_uids;
    }
    
    return *this;
}

/** Comparison operator */
bool CancelReservation::operator==(const CancelReservation &other) const
{
    return cancel_uids == other.cancel_uids and super::operator==(other);
}

/** Comparison operator */
bool CancelReservation::operator!=(const CancelReservation &other) const
{
    return not CancelReservation::operator==(other);
}

uint CancelReservation::hashCode() const
{
    return qHash(CancelReservation::typeName()) + cancel_uids.count();
}

QString CancelReservation::toString() const
{
    if (cancel_uids.isEmpty())
        return QObject::tr("CancelReservation::null");
    else
        return QObject::tr("CancelReservation( %1 )")
                        .arg( Siren::toString(cancel_uids) );
}

void CancelReservation::stream(Siren::Stream &s)
{
    s.assertVersion<CancelReservation>(1);
    
    Schema schema = s.item<CancelReservation>();
    
    schema.data("cancel_uids") & cancel_uids;
    
    super::stream( schema.base() );
}

/** Read the message - this cancels the contained reservations */
void CancelReservation::read(const QUuid&, quint64) const
{
    if (not cancel_uids.isEmpty())
        ResourceManager::releaseReservation(cancel_uids);
}

////////
//////// Implementation of CollectReservation
////////

static const RegisterObject<CollectReservation> r_collect_reservation;

/** Null constructor */
CollectReservation::CollectReservation() : Implements<CollectReservation,Message>()
{}

/** Construct to collect the reservation with passed UID */
CollectReservation::CollectReservation(const QUuid &reservation_uid)
                  : Implements<CollectReservation,Message>()
{
    if (not reservation_uid.isNull())
        collect_uids.append(reservation_uid);
}

/** Construct to collect the reservations with passed UIDs */
CollectReservation::CollectReservation(const QList<QUuid> &reservation_uids)
                  : Implements<CollectReservation,Message>(),
                    collect_uids(reservation_uids)
{}

/** Copy constructor */
CollectReservation::CollectReservation(const CollectReservation &other)
                   : Implements<CollectReservation,Message>(other),
                     collect_uids(other.collect_uids)
{}

/** Destructor */
CollectReservation::~CollectReservation()
{}

/** Copy assignmen operator */
CollectReservation& CollectReservation::operator=(const CollectReservation &other)
{
    if (this != &other)
    {
        super::operator=(other);
        collect_uids = other.collect_uids;
    }
    
    return *this;
}

/** Comparison operator */
bool CollectReservation::operator==(const CollectReservation &other) const
{
    return collect_uids == other.collect_uids and super::operator==(other);
}

/** Comparison operator */
bool CollectReservation::operator!=(const CollectReservation &other) const
{
    return not CollectReservation::operator==(other);
}

uint CollectReservation::hashCode() const
{
    return qHash(CollectReservation::typeName()) + collect_uids.count();
}

QString CollectReservation::toString() const
{
    if (collect_uids.isEmpty())
        return QObject::tr("CollectReservation::null");
    else
        return QObject::tr("CollectReservation( %1 )")
                        .arg( Siren::toString(collect_uids) );
}

void CollectReservation::stream(Siren::Stream &s)
{
    s.assertVersion<CollectReservation>(1);
    
    Schema schema = s.item<CollectReservation>();
    
    schema.data("collect_uids") & collect_uids;
    
    super::stream( schema.base() );
}

/** Read the message - this cancels the contained reservations */
void CollectReservation::read(const QUuid &sender, quint64 message_id) const
{
    if (not collect_uids.isEmpty())
    {
        QHash<QUuid,ActiveBackend> resources 
                    = ResourceManager::collectReservation(collect_uids);
                    
        //put these resources into the NetBackend holder
        QList<QUuid> netkeys = NetBackend::registerBackends(sender, resources.values());
        
        //send the netkeys back to the sender (the sender will use
        //the netkey together with the NetFrontend to control the resource)
        QByteArray data;
        {
            DataStream ds(&data, QIODevice::WriteOnly);
            ds << netkeys;
        }
        
        Communicator::send( Reply(message_id, data), sender );
    }
}

////////
//////// Implementation of GetResourceInfo
////////

static const RegisterObject<GetResourceInfo> r_get_resource_info;

/** Null constructor */
GetResourceInfo::GetResourceInfo() : Implements<GetResourceInfo,Message>()
{}

/** Construct to target the resource with netkey 'netkey' */
GetResourceInfo::GetResourceInfo(const QUuid &n)
                : Implements<GetResourceInfo,Message>(),
                  netkey(n)
{}

/** Copy constructor */
GetResourceInfo::GetResourceInfo(const GetResourceInfo &other)
                : Implements<GetResourceInfo,Message>(other),
                  netkey(other.netkey)
{}

/** Destructor */
GetResourceInfo::~GetResourceInfo()
{}

/** Copy assignment operator */
GetResourceInfo& GetResourceInfo::operator=(const GetResourceInfo &other)
{
    if (this != &other)
    {
        super::operator=(other);
        netkey = other.netkey;
    }
    
    return *this;
}

/** Comparison operator */
bool GetResourceInfo::operator==(const GetResourceInfo &other) const
{
    return netkey == other.netkey and super::operator==(other);
}

/** Comparison operator */
bool GetResourceInfo::operator!=(const GetResourceInfo &other) const
{
    return not GetResourceInfo::operator==(other);
}

uint GetResourceInfo::hashCode() const
{
    return qHash( GetResourceInfo::typeName() ) + qHash(netkey);
}

QString GetResourceInfo::toString() const
{
    if (netkey.isNull())
        return QObject::tr("GetResourceInfo::null");
    else
        return QObject::tr("GetResourceInfo( %1 )").arg(netkey.toString());
}

void GetResourceInfo::stream(Siren::Stream &s)
{
    s.assertVersion<GetResourceInfo>(1);
    
    Schema schema = s.item<GetResourceInfo>();
    
    schema.data("netkey") & netkey;
    
    super::stream(schema.base());
}

/** Read the message - this calls '' on the NetBackend identified by 
    the netkey of this message */
void GetResourceInfo::read(const QUuid &sender, quint64 message_id) const
{
    try
    {
        if (netkey.isNull())
            throw Siren::program_bug( QObject::tr(
                    "Somehow we are reading a %1 with a null netkey...!")
                        .arg(this->what()), CODELOC );
    
        QUuid resource_uid = NetBackend::UID(sender, netkey);
        QString resource_desc = NetBackend::description(sender, netkey);
    
        QByteArray data;
        {
            DataStream ds( &data, QIODevice::WriteOnly );
            
            ds << resource_uid << resource_desc;
        }
        
        Communicator::send( Reply(message_id, data), sender );
    }
    catch(const Siren::exception &e)
    {
        Communicator::send( Reply(message_id, e), sender );
    }
    catch(const std::exception &e)
    {
        Communicator::send( Reply(message_id, Siren::std_exception(e,CODELOC)),
                            sender );
    }
    catch(...)
    {
        Communicator::send( Reply(message_id, Siren::unknown_error( QObject::tr(
                "Unknown error occurred while processing %1 from %2 on %3.")
                    .arg(this->toString())
                    .arg(sender.toString())
                    .arg(Communicator::getLocalInfo().UID().toString()), CODELOC )),
                            sender );
    }
}

////////
//////// Implementation of DisconnectResource
////////

static const RegisterObject<DisconnectResource> r_disconnect_resource;

/** Null constructor */
DisconnectResource::DisconnectResource() : Implements<DisconnectResource,Message>()
{}

/** Construct to target the resource with netkey 'netkey' */
DisconnectResource::DisconnectResource(const QUuid &n)
                : Implements<DisconnectResource,Message>()
{
    if (not n.isNull())
        netkeys.append(n);
}

/** Construct to target the resource with netkey 'netkey' */
DisconnectResource::DisconnectResource(const QList<QUuid> &n)
                : Implements<DisconnectResource,Message>(), netkeys(n)
{}

/** Copy constructor */
DisconnectResource::DisconnectResource(const DisconnectResource &other)
                : Implements<DisconnectResource,Message>(other),
                  netkeys(other.netkeys)
{}

/** Destructor */
DisconnectResource::~DisconnectResource()
{}

/** Copy assignment operator */
DisconnectResource& DisconnectResource::operator=(const DisconnectResource &other)
{
    if (this != &other)
    {
        super::operator=(other);
        netkeys = other.netkeys;
    }
    
    return *this;
}

/** Comparison operator */
bool DisconnectResource::operator==(const DisconnectResource &other) const
{
    return netkeys == other.netkeys and super::operator==(other);
}

/** Comparison operator */
bool DisconnectResource::operator!=(const DisconnectResource &other) const
{
    return not DisconnectResource::operator==(other);
}

uint DisconnectResource::hashCode() const
{
    return qHash( DisconnectResource::typeName() ) + qHash(netkeys.count());
}

QString DisconnectResource::toString() const
{
    if (netkeys.isEmpty())
        return QObject::tr("DisconnectResource::null");
    else
        return QObject::tr("DisconnectResource( %1 )")
                .arg( Siren::toString(netkeys) );
}

void DisconnectResource::stream(Siren::Stream &s)
{
    s.assertVersion<DisconnectResource>(1);
    
    Schema schema = s.item<DisconnectResource>();
    
    schema.data("netkeys") & netkeys;
    
    super::stream(schema.base());
}

/** Read the message - this calls '' on the NetBackend identified by 
    the netkey of this message */
void DisconnectResource::read(const QUuid &sender, quint64) const
{
    try
    {
        foreach (const QUuid &netkey, netkeys)
        {
            NetBackend::disconnect(sender, netkey);
        }
    }
    catch(...)
    {}
}

////////
//////// Implementation of StartJob
////////

static const RegisterObject<StartJob> r_start_job;

/** Null constructor */
StartJob::StartJob() : Implements<StartJob,Message>()
{}

/** Construct to target the resource with netkey 'netkey' */
StartJob::StartJob(const QUuid &n, const WorkPacket &workpacket)
                : Implements<StartJob,Message>(),
                  netkey(n), packet(workpacket)
{}

/** Copy constructor */
StartJob::StartJob(const StartJob &other)
                : Implements<StartJob,Message>(other),
                  netkey(other.netkey), packet(other.packet)
{}

/** Destructor */
StartJob::~StartJob()
{}

/** Copy assignment operator */
StartJob& StartJob::operator=(const StartJob &other)
{
    if (this != &other)
    {
        super::operator=(other);
        netkey = other.netkey;
        packet = other.packet;
    }
    
    return *this;
}

/** Comparison operator */
bool StartJob::operator==(const StartJob &other) const
{
    return netkey == other.netkey and packet == other.packet and
           super::operator==(other);
}

/** Comparison operator */
bool StartJob::operator!=(const StartJob &other) const
{
    return not StartJob::operator==(other);
}

uint StartJob::hashCode() const
{
    return qHash( StartJob::typeName() ) + qHash(netkey);
}

QString StartJob::toString() const
{
    if (netkey.isNull() or packet.isNull())
        return QObject::tr("StartJob::null");
    else
        return QObject::tr("StartJob( %1, %2 )")
                .arg(netkey.toString(), packet.read().toString());
}

void StartJob::stream(Siren::Stream &s)
{
    s.assertVersion<StartJob>(1);
    
    Schema schema = s.item<StartJob>();
    
    schema.data("netkey") & netkey;
    schema.data("workpacket") & packet;
    
    super::stream(schema.base());
}

/** Read the message - this calls '' on the NetBackend identified by 
    the netkey of this message */
void StartJob::read(const QUuid &sender, quint64 message_id) const
{
    try
    {
        if (netkey.isNull())
            throw Siren::program_bug( QObject::tr(
                    "Somehow we are reading a %1 with a null netkey...!")
                        .arg(this->what()), CODELOC );
    
        NetBackend::startJob(sender, netkey, packet);
        
        Communicator::send( Reply(message_id), sender );
    }
    catch(const Siren::exception &e)
    {
        Communicator::send( Reply(message_id, e), sender );
    }
    catch(const std::exception &e)
    {
        Communicator::send( Reply(message_id, Siren::std_exception(e,CODELOC)),
                            sender );
    }
    catch(...)
    {
        Communicator::send( Reply(message_id, Siren::unknown_error( QObject::tr(
                "Unknown error occurred while processing %1 from %2 on %3.")
                    .arg(this->toString())
                    .arg(sender.toString())
                    .arg(Communicator::getLocalInfo().UID().toString()), CODELOC )),
                            sender );
    }
}

////////
//////// Implementation of StopJob
////////

static const RegisterObject<StopJob> r_stop_job;

/** Null constructor */
StopJob::StopJob() : Implements<StopJob,Message>()
{}

/** Construct to target the resource with netkey 'netkey' */
StopJob::StopJob(const QUuid &n)
                : Implements<StopJob,Message>(),
                  netkey(n)
{}

/** Copy constructor */
StopJob::StopJob(const StopJob &other)
                : Implements<StopJob,Message>(other),
                  netkey(other.netkey)
{}

/** Destructor */
StopJob::~StopJob()
{}

/** Copy assignment operator */
StopJob& StopJob::operator=(const StopJob &other)
{
    if (this != &other)
    {
        super::operator=(other);
        netkey = other.netkey;
    }
    
    return *this;
}

/** Comparison operator */
bool StopJob::operator==(const StopJob &other) const
{
    return netkey == other.netkey and super::operator==(other);
}

/** Comparison operator */
bool StopJob::operator!=(const StopJob &other) const
{
    return not StopJob::operator==(other);
}

uint StopJob::hashCode() const
{
    return qHash( StopJob::typeName() ) + qHash(netkey);
}

QString StopJob::toString() const
{
    if (netkey.isNull())
        return QObject::tr("StopJob::null");
    else
        return QObject::tr("StopJob( %1 )").arg(netkey.toString());
}

void StopJob::stream(Siren::Stream &s)
{
    s.assertVersion<StopJob>(1);
    
    Schema schema = s.item<StopJob>();
    
    schema.data("netkey") & netkey;
    
    super::stream(schema.base());
}

/** Read the message - this calls '' on the NetBackend identified by 
    the netkey of this message */
void StopJob::read(const QUuid &sender, quint64 message_id) const
{
    try
    {
        if (not netkey.isNull())
            NetBackend::stopJob(sender, netkey);
    }
    catch(...)
    {}
}

////////
//////// Implementation of AbortJob
////////

static const RegisterObject<AbortJob> r_abort_job;

/** Null constructor */
AbortJob::AbortJob() : Implements<AbortJob,Message>()
{}

/** Construct to target the resource with netkey 'netkey' */
AbortJob::AbortJob(const QUuid &n)
                : Implements<AbortJob,Message>(),
                  netkey(n)
{}

/** Copy constructor */
AbortJob::AbortJob(const AbortJob &other)
                : Implements<AbortJob,Message>(other),
                  netkey(other.netkey)
{}

/** Destructor */
AbortJob::~AbortJob()
{}

/** Copy assignment operator */
AbortJob& AbortJob::operator=(const AbortJob &other)
{
    if (this != &other)
    {
        super::operator=(other);
        netkey = other.netkey;
    }
    
    return *this;
}

/** Comparison operator */
bool AbortJob::operator==(const AbortJob &other) const
{
    return netkey == other.netkey and super::operator==(other);
}

/** Comparison operator */
bool AbortJob::operator!=(const AbortJob &other) const
{
    return not AbortJob::operator==(other);
}

uint AbortJob::hashCode() const
{
    return qHash( AbortJob::typeName() ) + qHash(netkey);
}

QString AbortJob::toString() const
{
    if (netkey.isNull())
        return QObject::tr("AbortJob::null");
    else
        return QObject::tr("AbortJob( %1 )").arg(netkey.toString());
}

void AbortJob::stream(Siren::Stream &s)
{
    s.assertVersion<AbortJob>(1);
    
    Schema schema = s.item<AbortJob>();
    
    schema.data("netkey") & netkey;
    
    super::stream(schema.base());
}

/** Read the message - this calls '' on the NetBackend identified by 
    the netkey of this message */
void AbortJob::read(const QUuid &sender, quint64 message_id) const
{
    try
    {
        if (not netkey.isNull())
            NetBackend::abortJob(sender, netkey);
    }
    catch(...)
    {}
}

////////
//////// Implementation of WaitForJob
////////

static const RegisterObject<WaitForJob> r_wait_for_job;

/** Null constructor */
WaitForJob::WaitForJob() : Implements<WaitForJob,Message>()
{}

/** Construct to wait until the job has finished */
WaitForJob::WaitForJob(const QUuid &n)
                : Implements<WaitForJob,Message>(),
                  netkey(n), ms(-1)
{}

/** Construct to wait until the job has finished, or ms milliseconds
    have elapsed */
WaitForJob::WaitForJob(const QUuid &n, int m)
                : Implements<WaitForJob,Message>(),
                  netkey(n), ms(m)
{}

/** Copy constructor */
WaitForJob::WaitForJob(const WaitForJob &other)
                : Implements<WaitForJob,Message>(other),
                  netkey(other.netkey), ms(other.ms)
{}

/** Destructor */
WaitForJob::~WaitForJob()
{}

/** Copy assignment operator */
WaitForJob& WaitForJob::operator=(const WaitForJob &other)
{
    if (this != &other)
    {
        super::operator=(other);
        netkey = other.netkey;
        ms = other.ms;
    }
    
    return *this;
}

/** Comparison operator */
bool WaitForJob::operator==(const WaitForJob &other) const
{
    return netkey == other.netkey and ms == other.ms and super::operator==(other);
}

/** Comparison operator */
bool WaitForJob::operator!=(const WaitForJob &other) const
{
    return not WaitForJob::operator==(other);
}

uint WaitForJob::hashCode() const
{
    return qHash( WaitForJob::typeName() ) + qHash(netkey) + qHash(ms);
}

QString WaitForJob::toString() const
{
    if (netkey.isNull())
        return QObject::tr("WaitForJob::null");
    else if (ms < 0)
        return QObject::tr("WaitForJob( %1 )").arg(netkey.toString());
    else
        return QObject::tr("WaitForJob( %1, wait <= %2 ms )")
                    .arg(netkey.toString()).arg(ms);
}

void WaitForJob::stream(Siren::Stream &s)
{
    s.assertVersion<WaitForJob>(1);
    
    Schema schema = s.item<WaitForJob>();
    
    schema.data("netkey") & netkey;
    schema.data("timeout") & ms;
    
    super::stream(schema.base());
}

/** Read the message - this calls '' on the NetBackend identified by 
    the netkey of this message */
void WaitForJob::read(const QUuid &sender, quint64 message_id) const
{
    try
    {
        if (netkey.isNull())
            throw Siren::program_bug( QObject::tr(
                    "Somehow we are reading a %1 with a null netkey...!")
                        .arg(this->what()), CODELOC );

        QByteArray data;

        if (ms < 0)
        {
            NetBackend::wait(sender, netkey);
        }
        else
        {
            bool finished = NetBackend::wait(sender, netkey, ms);
            
            DataStream ds( &data, QIODevice::WriteOnly );
            ds << finished;
        }
        
        Communicator::send( Reply(message_id, data), sender );
    }
    catch(const Siren::exception &e)
    {
        Communicator::send( Reply(message_id, e), sender );
    }
    catch(const std::exception &e)
    {
        Communicator::send( Reply(message_id, Siren::std_exception(e,CODELOC)),
                            sender );
    }
    catch(...)
    {
        Communicator::send( Reply(message_id, Siren::unknown_error( QObject::tr(
                "Unknown error occurred while processing %1 from %2 on %3.")
                    .arg(this->toString())
                    .arg(sender.toString())
                    .arg(Communicator::getLocalInfo().UID().toString()), CODELOC )),
                            sender );
    }
}

////////
//////// Implementation of GetProgress
////////

static const RegisterObject<GetProgress> r_get_progress;

/** Null constructor */
GetProgress::GetProgress() : Implements<GetProgress,Message>()
{}

/** Construct to target the resource with netkey 'netkey' */
GetProgress::GetProgress(const QUuid &n)
                : Implements<GetProgress,Message>(),
                  netkey(n)
{}

/** Copy constructor */
GetProgress::GetProgress(const GetProgress &other)
                : Implements<GetProgress,Message>(other),
                  netkey(other.netkey)
{}

/** Destructor */
GetProgress::~GetProgress()
{}

/** Copy assignment operator */
GetProgress& GetProgress::operator=(const GetProgress &other)
{
    if (this != &other)
    {
        super::operator=(other);
        netkey = other.netkey;
    }
    
    return *this;
}

/** Comparison operator */
bool GetProgress::operator==(const GetProgress &other) const
{
    return netkey == other.netkey and super::operator==(other);
}

/** Comparison operator */
bool GetProgress::operator!=(const GetProgress &other) const
{
    return not GetProgress::operator==(other);
}

uint GetProgress::hashCode() const
{
    return qHash( GetProgress::typeName() ) + qHash(netkey);
}

QString GetProgress::toString() const
{
    if (netkey.isNull())
        return QObject::tr("GetProgress::null");
    else
        return QObject::tr("GetProgress( %1 )").arg(netkey.toString());
}

void GetProgress::stream(Siren::Stream &s)
{
    s.assertVersion<GetProgress>(1);
    
    Schema schema = s.item<GetProgress>();
    
    schema.data("netkey") & netkey;
    
    super::stream(schema.base());
}

/** Read the message - this calls '' on the NetBackend identified by 
    the netkey of this message */
void GetProgress::read(const QUuid &sender, quint64 message_id) const
{
    try
    {
        if (netkey.isNull())
            throw Siren::program_bug( QObject::tr(
                    "Somehow we are reading a %1 with a null netkey...!")
                        .arg(this->what()), CODELOC );
    
        QByteArray data;
        {
            float progress = NetBackend::progress(sender, netkey);
        
            DataStream ds( &data, QIODevice::WriteOnly );
            
            ds << progress;
        }
        
        Communicator::send( Reply(message_id, data), sender );
    }
    catch(const Siren::exception &e)
    {
        Communicator::send( Reply(message_id, e), sender );
    }
    catch(const std::exception &e)
    {
        Communicator::send( Reply(message_id, Siren::std_exception(e,CODELOC)),
                            sender );
    }
    catch(...)
    {
        Communicator::send( Reply(message_id, Siren::unknown_error( QObject::tr(
                "Unknown error occurred while processing %1 from %2 on %3.")
                    .arg(this->toString())
                    .arg(sender.toString())
                    .arg(Communicator::getLocalInfo().UID().toString()), CODELOC )),
                            sender );
    }
}

////////
//////// Implementation of GetResult
////////

static const RegisterObject<GetResult> r_get_result;

/** Null constructor */
GetResult::GetResult() : Implements<GetResult,Message>()
{}

/** Construct to target the resource with netkey 'netkey' */
GetResult::GetResult(const QUuid &n, bool wait)
                : Implements<GetResult,Message>(),
                  netkey(n), wait_for_final(wait)
{}

/** Copy constructor */
GetResult::GetResult(const GetResult &other)
                : Implements<GetResult,Message>(other),
                  netkey(other.netkey), wait_for_final(other.wait_for_final)
{}

/** Destructor */
GetResult::~GetResult()
{}

/** Copy assignment operator */
GetResult& GetResult::operator=(const GetResult &other)
{
    if (this != &other)
    {
        super::operator=(other);
        netkey = other.netkey;
        wait_for_final = other.wait_for_final;
    }
    
    return *this;
}

/** Comparison operator */
bool GetResult::operator==(const GetResult &other) const
{
    return netkey == other.netkey and 
           wait_for_final == other.wait_for_final and
           super::operator==(other);
}

/** Comparison operator */
bool GetResult::operator!=(const GetResult &other) const
{
    return not GetResult::operator==(other);
}

uint GetResult::hashCode() const
{
    return qHash( GetResult::typeName() ) + qHash(netkey)
                + qHash(wait_for_final);
}

QString GetResult::toString() const
{
    if (netkey.isNull())
        return QObject::tr("GetResult::null");
    else if (wait_for_final)
        return QObject::tr("GetResult( %1 : final result )").arg(netkey.toString());
    else
        return QObject::tr("GetResult( %1 : interim result )").arg(netkey.toString());
}

void GetResult::stream(Siren::Stream &s)
{
    s.assertVersion<GetResult>(1);
    
    Schema schema = s.item<GetResult>();
    
    schema.data("netkey") & netkey;
    schema.data("wait") & wait_for_final;
    
    super::stream(schema.base());
}

/** Read the message - this calls '' on the NetBackend identified by 
    the netkey of this message */
void GetResult::read(const QUuid &sender, quint64 message_id) const
{
    try
    {
        if (netkey.isNull())
            throw Siren::program_bug( QObject::tr(
                    "Somehow we are reading a %1 with a null netkey...!")
                        .arg(this->what()), CODELOC );
    
        QByteArray data;
        {
            DataStream ds( &data, QIODevice::WriteOnly );
        
            if (wait_for_final)
            {
                WorkPacketPtr result = NetBackend::result(sender, netkey);
                ds << result;
            }
            else
            {
                WorkPacketPtr result = NetBackend::interimResult(sender, netkey);
                ds << result;
            }
        }
        
        Communicator::send( Reply(message_id, data), sender );
    }
    catch(const Siren::exception &e)
    {
        Communicator::send( Reply(message_id, e), sender );
    }
    catch(const std::exception &e)
    {
        Communicator::send( Reply(message_id, Siren::std_exception(e,CODELOC)),
                            sender );
    }
    catch(...)
    {
        Communicator::send( Reply(message_id, Siren::unknown_error( QObject::tr(
                "Unknown error occurred while processing %1 from %2 on %3.")
                    .arg(this->toString())
                    .arg(sender.toString())
                    .arg(Communicator::getLocalInfo().UID().toString()), CODELOC )),
                            sender );
    }
}
