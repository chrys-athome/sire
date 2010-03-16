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
#include <QPair>

#include "Siren/objptr.hpp"

#include "message.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{

class WorkPacket;

typedef Siren::ObjPtr<WorkPacket> WorkPacketPtr;

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

/** This is a message sent from NetFrontend to NetBackend to
    get information about the resource connected to by the backend  

    @author Christopher Woods
*/
class GetResourceInfo : public Siren::Implements<GetResourceInfo,Message>
{
public:
    GetResourceInfo();
    GetResourceInfo(const QUuid &netkey);

    GetResourceInfo(const GetResourceInfo &other);
    
    ~GetResourceInfo();
    
    GetResourceInfo& operator=(const GetResourceInfo &other);
    
    bool operator==(const GetResourceInfo &other) const;
    bool operator!=(const GetResourceInfo &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;

private:
    /** The key for the resource */
    QUuid netkey;
};

/** This is a message sent by NetFrontend to NetBackend to tell the
    backend that the frontend is disconnecting from the backend  

    @author Christopher Woods
*/
class DisconnectResource : public Siren::Implements<DisconnectResource,Message>
{
public:
    DisconnectResource();
    DisconnectResource(const QUuid &netkey);
    DisconnectResource(const QList<QUuid> &netkeys);
    
    DisconnectResource(const DisconnectResource &other);
    
    ~DisconnectResource();
    
    DisconnectResource& operator=(const DisconnectResource &other);
    
    bool operator==(const DisconnectResource &other) const;
    bool operator!=(const DisconnectResource &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;

private:
    /** The keys of the resources to be disconnected */
    QList<QUuid> netkeys;
};

/** This is a message sent from NetFrontend to NetBackend to  
    start a new job on the backend  

    @author Christopher Woods
*/
class StartJob : public Siren::Implements<StartJob,Message>
{
public:
    StartJob();
    StartJob(const QUuid &netkey, const WorkPacket &workpacket);
    
    StartJob(const StartJob &other);
    
    ~StartJob();
    
    StartJob& operator=(const StartJob &other);
    
    bool operator==(const StartJob &other) const;
    bool operator!=(const StartJob &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;

private:
    /** The key for the resource */
    QUuid netkey;
    
    /** The workpacket to process */
    WorkPacketPtr packet;
};

/** This is a message sent from NetFrontend to NetBackend to stop
    any job currently running on the backend  

    @author Christopher Woods
*/
class StopJob : public Siren::Implements<StopJob,Message>
{
public:
    StopJob();
    StopJob(const QUuid &netkey);
    
    StopJob(const StopJob &other);
    
    ~StopJob();
    
    StopJob& operator=(const StopJob &other);
    
    bool operator==(const StopJob &other) const;
    bool operator!=(const StopJob &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;

private:
    /** The key for the resource */
    QUuid netkey;
};

/** This is a message sent from NetFrontend to NetBackend to abort
    any job currently running on the backend  

    @author Christopher Woods
*/
class AbortJob : public Siren::Implements<AbortJob,Message>
{
public:
    AbortJob();
    AbortJob(const QUuid &netkey);
    
    AbortJob(const AbortJob &other);
    
    ~AbortJob();
    
    AbortJob& operator=(const AbortJob &other);
    
    bool operator==(const AbortJob &other) const;
    bool operator!=(const AbortJob &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;

private:
    /** The key for the resource */
    QUuid netkey;
};

/** This is a message sent from NetFrontend to NetBackend to wait
    for any job currently running on the backend to finish  

    @author Christopher Woods
*/
class WaitForJob : public Siren::Implements<WaitForJob,Message>
{
public:
    WaitForJob();
    WaitForJob(const QUuid &netkey);
    WaitForJob(const QUuid &netkey, int ms);
    
    WaitForJob(const WaitForJob &other);
    
    ~WaitForJob();
    
    WaitForJob& operator=(const WaitForJob &other);
    
    bool operator==(const WaitForJob &other) const;
    bool operator!=(const WaitForJob &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;

private:
    /** The key for the resource */
    QUuid netkey;
    
    /** The maximum amount of time to wait for the job
        (forever if this is less than 0) */
    qint32 ms;
};

/** This is a message sent from NetFrontend to NetBackend to get
    the progress of the job currently running on the backend  

    @author Christopher Woods
*/
class GetProgress : public Siren::Implements<GetProgress,Message>
{
public:
    GetProgress();
    GetProgress(const QUuid &netkey);
    
    GetProgress(const GetProgress &other);
    
    ~GetProgress();
    
    GetProgress& operator=(const GetProgress &other);
    
    bool operator==(const GetProgress &other) const;
    bool operator!=(const GetProgress &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;

private:
    /** The key for the resource */
    QUuid netkey;
};

/** This is a message sent from NetFrontend to NetBackend to get
    the result of the job currently running on the backend  

    @author Christopher Woods
*/
class GetResult : public Siren::Implements<GetResult,Message>
{
public:
    GetResult();
    GetResult(const QUuid &netkey, bool wait_for_final=true);
    
    GetResult(const GetResult &other);
    
    ~GetResult();
    
    GetResult& operator=(const GetResult &other);
    
    bool operator==(const GetResult &other) const;
    bool operator!=(const GetResult &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;

private:
    /** The key for the resource */
    QUuid netkey;
    
    /** Whether or not to wait for the final result */
    bool wait_for_final;
};

//
///** This is a template for the messages sent from the 
//    NetFrontend to the NetBackend (resource identified
//    by UID 'netkey'
//
//    @author Christopher Woods
//*/
//class MessageTemplate : public Siren::Implements<MessageTemplate,Message>
//{
//public:
//    MessageTemplate();
//    MessageTemplate(const QUuid &netkey);
//
//    MessageTemplate(const MessageTemplate &other);
//    
//    ~MessageTemplate();
//    
//    MessageTemplate& operator=(const MessageTemplate &other);
//    
//    bool operator==(const MessageTemplate &other) const;
//    bool operator!=(const MessageTemplate &other) const;
//    
//    uint hashCode() const;
//    QString toString() const;
//    void stream(Siren::Stream &s);
//    
//    void read(const QUuid &sender, quint64 message_id) const;
//
//private:
//    /** The key for the resource */
//    QUuid netkey;
//};
//
//////////
////////// Implementation of MessageTemplate
//////////
//
//static const RegisterObject<MessageTemplate> r_message_template;
//
///** Null constructor */
//MessageTemplate::MessageTemplate() : Implements<MessageTemplate,Message>()
//{}
//
///** Construct to target the resource with netkey 'netkey' */
//MessageTemplate::MessageTemplate(const QUuid &n)
//                : Implements<MessageTemplate,Message>(),
//                  netkey(n)
//{}
//
///** Copy constructor */
//MessageTemplate::MessageTemplate(const MessageTemplate &other)
//                : Implements<MessageTemplate,Message>(other),
//                  netkey(other.netkey)
//{}
//
///** Destructor */
//MessageTemplate::~MessageTemplate()
//{}
//
///** Copy assignment operator */
//MessageTemplate& MessageTemplate::operator=(const MessageTemplate &other)
//{
//    if (this != &other)
//    {
//        super::operator=(other);
//        netkey = other.netkey;
//    }
//    
//    return *this;
//}
//
///** Comparison operator */
//bool MessageTemplate::operator==(const MessageTemplate &other) const
//{
//    return netkey == other.netkey and super::operator==(other);
//}
//
///** Comparison operator */
//bool MessageTemplate::operator!=(const MessageTemplate &other) const
//{
//    return not MessageTemplate::operator==(other);
//}
//
//uint MessageTemplate::hashCode() const
//{
//    return qHash( MessageTemplate::typeName() ) + qHash(netkey);
//}
//
//QString MessageTemplate::toString() const
//{
//    if (netkey.isNull())
//        return QObject::tr("MessageTemplate::null");
//    else
//        return QObject::tr("MessageTemplate( %1 )").arg(netkey.toString());
//}
//
//void MessageTemplate::stream(Siren::Stream &s)
//{
//    s.assertVersion<MessageTemplate>(1);
//    
//    Schema schema = s.item<MessageTemplate>();
//    
//    schema.data("netkey") & netkey;
//    
//    super::stream(schema.base());
//}
//
///** Read the message - this calls '' on the NetBackend identified by 
//    the netkey of this message */
//void MessageTemplate::read(const QUuid &sender, quint64 message_id) const
//{
//    try
//    {
//        if (netkey.isNull())
//            throw Siren::program_bug( QObject::tr(
//                    "Somehow we are reading a %1 with a null netkey...!")
//                        .arg(this->what()), CODELOC );
//    
//        QByteArray data;
//        {
//            DataStream ds( &data, QIODevice::WriteOnly );
//        }
//        
//        Communicator::send( Reply(message_id, data), sender );
//    }
//    catch(const Siren::exception &e)
//    {
//        Communicator::send( Reply(message_id, e), sender );
//    }
//    catch(const std::exception &e)
//    {
//        Communicator::send( Reply(message_id, Siren::std_exception(e,CODELOC)),
//                            sender );
//    }
//    catch(...)
//    {
//        Communicator::send( Reply(message_id, Siren::unknown_error( QObject::tr(
//                "Unknown error occurred while processing %1 from %2 on %3.")
//                    .arg(this->toString())
//                    .arg(sender.toString())
//                    .arg(Communicator::getLocalInfo().UID().toString()), CODELOC )),
//                            sender );
//    }
//}

} // end of namespace network
} // end of namespace SireCluster

Q_DECLARE_METATYPE( SireCluster::network::ReserveRequest )
Q_DECLARE_METATYPE( SireCluster::network::ReserveResponse )
Q_DECLARE_METATYPE( SireCluster::network::CancelReservation )
Q_DECLARE_METATYPE( SireCluster::network::CollectReservation )
Q_DECLARE_METATYPE( SireCluster::network::GetResourceInfo )
Q_DECLARE_METATYPE( SireCluster::network::DisconnectResource )
Q_DECLARE_METATYPE( SireCluster::network::StartJob )
Q_DECLARE_METATYPE( SireCluster::network::StopJob )
Q_DECLARE_METATYPE( SireCluster::network::AbortJob )
Q_DECLARE_METATYPE( SireCluster::network::WaitForJob )
Q_DECLARE_METATYPE( SireCluster::network::GetProgress )
Q_DECLARE_METATYPE( SireCluster::network::GetResult )

SIRE_END_HEADER

#endif
