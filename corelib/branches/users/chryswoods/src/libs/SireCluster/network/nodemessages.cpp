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

#include "SireCluster/resources/resourcemanager.h"
#include "SireCluster/resources/backend.h"

#include "Siren/stream.h"
#include "Siren/streamqt.h"
#include "Siren/tostring.h"
#include "Siren/forages.h"

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
    qDebug() << "RECEIVE A RESERVEREQUEST FROM" << sender.toString() << message_id;
    qDebug() << "DESCRIPTION" << desc << "NRESOURCES" << nresources
             << "EXPIRE_TIME" << expire_time;
             
    qDebug() << "UID ==" << request_uid;
    
    //reserve a node - this tries to get a node as quickly as
    //possible, but may block while other nodes are reserved
    QUuid reservation = ResourceManager::reserveResource(expire_time);
    
    //send a response back with the reservation UID
    ReserveResponse response(request_uid, reservation);
    
    qDebug() << "SENDING RESPONSE" << response.toString();
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
    qDebug() << "RECEIVED RESPONSE FROM" << sender << this->toString();
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
void CollectReservation::read(const QUuid &sender, quint64) const
{
    if (not collect_uids.isEmpty())
    {
        QHash<QUuid,ActiveBackend> resources 
                    = ResourceManager::collectReservation(collect_uids);
                    
        qDebug() << "COLLECTED RESERVATIONS" << collect_uids << resources.count();
                    
        //put these resources into the NetBackend holder
        QList<QUuid> netkeys = NetBackend::registerBackends(sender, resources.values());
        
        qDebug() << "REGISTERED NETBACKENDS" << netkeys;
        
        //send the netkeys back to the sender (the sender will use
        //the netkey together with the NetFrontend to control the resource)
        Communicator::send( CollectResponse(netkeys), sender );
    }
}

////////
//////// Implementation of CollectResponse
////////

static const RegisterObject<CollectResponse> r_collect_response;

/** Null constructor */
CollectResponse::CollectResponse() : Implements<CollectResponse,Message>()
{}

/** Construct to return the net key to a single resource */
CollectResponse::CollectResponse(const QUuid &netkey)
                : Implements<CollectResponse,Message>()
{
    if (not netkey.isNull())
        netkeys.append(netkey);
}

/** Construct to return the net keys to several resources */
CollectResponse::CollectResponse(const QList<QUuid> &keys)
                : Implements<CollectResponse,Message>(),
                  netkeys(keys)
{}

/** Copy constructor */
CollectResponse::CollectResponse(const CollectResponse &other)
                : Implements<CollectResponse,Message>(other),
                  netkeys(other.netkeys)
{}

/** Destructor */
CollectResponse::~CollectResponse()
{}

/** Copy assignment operator */
CollectResponse& CollectResponse::operator=(const CollectResponse &other)
{
    if (this != &other)
    {
        super::operator=(other);
        netkeys = other.netkeys;
    }
    
    return *this;
}

/** Comparison operator */
bool CollectResponse::operator==(const CollectResponse &other) const
{
    return netkeys == other.netkeys and super::operator==(other);
}

/** Comparison operator */
bool CollectResponse::operator!=(const CollectResponse &other) const
{
    return not CollectResponse::operator==(other);
}

uint CollectResponse::hashCode() const
{
    return qHash(CollectResponse::typeName()) + netkeys.count();
}

QString CollectResponse::toString() const
{
    if (netkeys.isEmpty())
        return QObject::tr("CollectResponse::null");
    else
        return QObject::tr("CollectResponse( %1 )")
                        .arg( Siren::toString(netkeys) );
}

void CollectResponse::stream(Siren::Stream &s)
{
    s.assertVersion<CollectResponse>(1);
    
    Schema schema = s.item<CollectResponse>();
    
    schema.data("netkeys") & netkeys;
    
    super::stream( schema.base() );
}

/** Read this message - this creates and posts NetFrontends that communicate
    with the backends using the contained netkeys */
void CollectResponse::read(const QUuid &sender, quint64) const
{
    qDebug() << "CREATING NETFRONTENDS TO" << netkeys << "FROM" << sender;
}
