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

#ifndef SIRECLUSTER_NETWORK_NODEMESSAGES_H
#define SIRECLUSTER_NETWORK_NODEMESSAGES_H

#include <QMultiHash>

#include "message.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{
namespace network
{

class ReserveResponse;

/** This message is sent to request that a resource 
    (or many resources) is reserved for use by the sending node
    
    @author Christopher Woods
*/
class ReserveRequest : public Siren::Implements<ReserveRequest,Message>
{
public:
    ReserveRequest();
    ReserveRequest(const QUuid &request_uid, int expires);
    ReserveRequest(const QUuid &request_uid, QString description, int expires);
    ReserveRequest(const QUuid &request_uid, int nresources, int expires);
    ReserveRequest(const QUuid &request_uid, int nresources, 
                   QString description, int expires);
    
    ReserveRequest(const ReserveRequest &other);
    
    ~ReserveRequest();
    
    ReserveRequest& operator=(const ReserveRequest &other);
    
    bool operator==(const ReserveRequest &other) const;
    bool operator!=(const ReserveRequest &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;

    QPair<QUuid,QUuid> reservationUID() const;
    QMultiHash<QUuid,QUuid> reservationUIDs() const;

private:
    /** The unique ID of this reservation request */
    QUuid request_uid;

    /** A description of the type of resource to be reserved */
    QString desc;

    /** The number of resources to try and reserve */
    qint32 nresources;

    /** How long the reservation will be valid from the
        time that it is aquired */
    qint32 expire_time;
};

/** This message is sent in response to a reservation request.
    This either says that nothing is available, or it provides
    the reservation UID

    @author Christopher Woods
*/
class ReserveResponse : public Siren::Implements<ReserveResponse,Message>
{
public:
    ReserveResponse();
    ReserveResponse(const QUuid &request_uid, 
                    const QUuid &reservation_uid);

    ReserveResponse(const QUuid &request_uid,
                    const QList<QUuid> &reservation_uids);
                    
    ReserveResponse(const ReserveResponse &other);
    
    ~ReserveResponse();
    
    ReserveResponse& operator=(const ReserveResponse &other);
    
    bool operator==(const ReserveResponse &other) const;
    bool operator!=(const ReserveResponse &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;
    
private:
    /** UID of the reservation request */
    QUuid request_uid;
    
    /** UIDs of the reservations - empty if nothing 
        could be reserved */
    QList<QUuid> reservation_uids;
};

/** This message is sent to cancel the reservations made on a node 

    @author Christopher Woods
*/
class CancelReservation : public Siren::Implements<CancelReservation,Message>
{
public:
    CancelReservation();
    CancelReservation(const QUuid &reservation_uid);
    CancelReservation(const QList<QUuid> &reservation_uids);
    
    CancelReservation(const CancelReservation &other);
    
    ~CancelReservation();
    
    CancelReservation& operator=(const CancelReservation &other);
    
    bool operator==(const CancelReservation &other) const;
    bool operator!=(const CancelReservation &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;
    
private:
    /** The UIDs of the reservations to cancel */
    QList<QUuid> cancel_uids;
};

/** This message is used to collect a reservation 

    @author Christopher Woods
*/
class CollectReservation : public Siren::Implements<CollectReservation,Message>
{
public:
    CollectReservation();
    CollectReservation(const QUuid &reservation_uid);
    CollectReservation(const QList<QUuid> &reservation_uids);
    
    CollectReservation(const CollectReservation &other);
    
    ~CollectReservation();
    
    CollectReservation& operator=(const CollectReservation &other);
    
    bool operator==(const CollectReservation &other) const;
    bool operator!=(const CollectReservation &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;
    
private:
    /** The UIDs of the reservations to collect */
    QList<QUuid> collect_uids;
};

/** The message containing the response to a collection request 

    @author Christopher Woods
*/
class CollectResponse : public Siren::Implements<CollectResponse,Message>
{
public:
    CollectResponse();
    CollectResponse(const QUuid &netkey);
    CollectResponse(const QList<QUuid> &netkeys);
    
    CollectResponse(const CollectResponse &other);
    
    ~CollectResponse();
    
    CollectResponse& operator=(const CollectResponse &other);
    
    bool operator==(const CollectResponse &other) const;
    bool operator!=(const CollectResponse &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;

private:
    /** The keys used to talk to the nodes */
    QList<QUuid> netkeys;
};

} // end of namespace network
} // end of namespace SireCluster

Q_DECLARE_METATYPE( SireCluster::network::ReserveRequest )
Q_DECLARE_METATYPE( SireCluster::network::ReserveResponse )
Q_DECLARE_METATYPE( SireCluster::network::CancelReservation )
Q_DECLARE_METATYPE( SireCluster::network::CollectReservation )
Q_DECLARE_METATYPE( SireCluster::network::CollectResponse )

SIRE_END_HEADER

#endif
