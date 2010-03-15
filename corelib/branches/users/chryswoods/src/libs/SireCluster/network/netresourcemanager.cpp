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

#include "netresourcemanager.h"
#include "communicator.h"
#include "nodemessages.h"
#include "netbackend.h"

#include "Siren/mutex.h"
#include "Siren/waitcondition.h"

#include "SireCluster/resources/backend.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::network;
using namespace SireCluster::resources;
using namespace Siren;

/** This is a small internal class that is used to manage
    the reservations that are requested by this node */
class ResRequests : public boost::noncopyable
{
public:
    ResRequests(int n) : nresources(n), nresponses(0)
    {}
    
    ~ResRequests()
    {}
    
    /** All of the reservations */
    QMultiHash<QUuid,QUuid> reservations;
    
    /** Mutex used to protect access to this object */
    Siren::Mutex datamutex;
    
    /** Wait condition used to sleep until
        reservations are available */
    Siren::WaitCondition waiter;
    
    /** The number of resources to reserve */
    int nresources;
    
    /** The number of received responses */
    int nresponses;
};

Q_GLOBAL_STATIC( Mutex, requestMutex )

/** Global map of ReserveRequest UIDs to ResRequests objects */
static QHash< QUuid,boost::weak_ptr<ResRequests> > *global_requests;

/** Internal function used to get the ResRequests object associated with
    a particular request_uid */
boost::shared_ptr<ResRequests> getResRequests(const QUuid &request_uid)
{
    MutexLocker lkr( requestMutex() );
    
    if (not global_requests)
        return boost::shared_ptr<ResRequests>();
        
    return global_requests->value(request_uid).lock();
}

/** Internal function used to create the ResRequests object associated
    with a particular request_uid - the request is for 'n' resources */
boost::shared_ptr<ResRequests> createResRequests(const QUuid &uid, int n)
{
    MutexLocker lkr( requestMutex() );
    
    if (global_requests)
    {
        if (global_requests->contains(uid))
        {
            boost::shared_ptr<ResRequests> r = global_requests->value(uid).lock();
            
            if (r.get() != 0)
                return r;
        }
        
        boost::shared_ptr<ResRequests> r( new ResRequests(n) );
        
        global_requests->insert(uid, r);
        
        return r;
    }
    else
        return boost::shared_ptr<ResRequests>();
}

/** Return the UID of the node that has a resource available, and
    the UID of the reservation - this blocks until all
    nodes have responded with their requests, or until the 
    reservation is met. This returns a null UID pair if no resource
    could be reserved */
QPair<QUuid,QUuid> reservationUID(const QUuid &request_uid,
                                  boost::shared_ptr<ResRequests> reservations,
                                  int nrequests)
{
    if (reservations.get() == 0)
        return QPair<QUuid,QUuid>();
        
    MutexLocker lkr( &(reservations->datamutex) );
    
    while (reservations->reservations.isEmpty())
    {
        if (getResRequests(request_uid).get() == 0)
        {
            //this request is no longer being serviced
            // - nothing is available
            qDebug() << "NOTHING IS AVAILABLE!!!";
            return QPair<QUuid,QUuid>();
        }
            
        if (reservations->nresponses >= nrequests)
        {
            qDebug() << "EVERYONE RESPONDED NEGATIVELY!";
        
            //everyone who was contacted has already responded
            //in the negative
            MutexLocker lkr2( requestMutex() );
            
            if (global_requests)
                global_requests->remove(request_uid);
                
            return QPair<QUuid,QUuid>();
        }

        qDebug() << "WAITING FOR A RESERVATION TO BECOME AVAILABLE...";
            
        reservations->waiter.wait( &(reservations->datamutex) );
    }

    if (reservations->nresponses >= nrequests)
    {
        //everyone who should respond has!
        MutexLocker lkr2( requestMutex() );
        
        if (global_requests)
            global_requests->remove(request_uid);
    }

    //return the first reservation
    QMultiHash<QUuid,QUuid>::const_iterator 
                        it = reservations->reservations.constBegin();
                            
    return QPair<QUuid,QUuid>(it.key(), it.value());
}

/** Return the UIDs of this reservations, indexed by the UIDs of the
    nodes that provide the corresponding resources - this blocks until all
    nodes have responded with their requests, or until the 
    reservations have been met. This returns an empty list if no resources
    could be reserved */
QMultiHash<QUuid,QUuid> reservationUIDs(boost::shared_ptr<ResRequests> reservations)
{
    return QMultiHash<QUuid,QUuid>();
}

/** Initialise the NetResourceManager */
void NetResourceManager::init()
{
    MutexLocker lkr( requestMutex() );
    
    if (not global_requests)
    {
        global_requests = new QHash< QUuid,boost::weak_ptr<ResRequests> >();
    }
    
    NetBackend::init();
}

/** End the NetResourceManager */
void NetResourceManager::end()
{
    MutexLocker lkr( requestMutex() );
    
    delete global_requests;
    global_requests = 0;
    
    NetBackend::end();
}

/** This function is called to receive a set of reservations (that
    are from the specified sender, associated with the passed
    reservation request) */
void NetResourceManager::receivedReservation(const QUuid &sender,
                                             QList<QUuid> reservation_uids,
                                             const QUuid &request_uid)
{
    boost::shared_ptr<ResRequests> requests = ::getResRequests(request_uid);
                
    if (requests.get() == 0)
    {
        //we no longer want these reservations - send back cancel
        //reservation response to the sender
        qDebug() << "Cancelling" << reservation_uids;
        Communicator::send( CancelReservation(reservation_uids), sender );
        return;
    }
    
    //signal whether or not we have a reservation (and what that is)
    {
        MutexLocker lkr( &(requests->datamutex) );
        requests->nresponses += 1;

        while (requests->reservations.count() < requests->nresources)
        {
            if (reservation_uids.isEmpty())
                break;
                
            requests->reservations.insert(sender, reservation_uids.takeFirst());
        }

        requests->waiter.wakeAll();
    }
    
    if (not reservation_uids.isEmpty())
    {
        //we no longer want these reservations - send back cancel
        //reservation response to the sender
        qDebug() << "Cancelling" << reservation_uids;
        Communicator::send( CancelReservation(reservation_uids), sender );
    }
}

/** Reserve a network resource - this sends a ReserveRequest message
    to all nodes asking to reserve one backend on that node. */
QPair<QUuid,QUuid> NetResourceManager::reserveResource(int expires)
{
    //create a request to reserve nodes
    QUuid request_uid = QUuid::createUuid();
    
    boost::shared_ptr<ResRequests> r = ::createResRequests(request_uid, 1);
    
    ReserveRequest request(request_uid, expires);

    qDebug() << "SENDING RESERVE MESSAGES" << request.toString();
    QHash<QUuid,quint64> msgs = Communicator::broadcast(request);
    Communicator::awaitSent(msgs);
    qDebug() << "MESSAGES HAVE BEEN SENT";

    //see get the UUID of the (hopefully) successful reservation
    return ::reservationUID(request_uid, r, msgs.count());
}

QPair<QUuid,QUuid> NetResourceManager::reserveResource(const QString &description,
                                                       int expires)
{
    return QPair<QUuid,QUuid>();
}

QMultiHash<QUuid,QUuid> NetResourceManager::reserveResources(int n, int expires)
{
    return QMultiHash<QUuid,QUuid>();
}

QMultiHash<QUuid,QUuid> NetResourceManager::reserveResources(const QString &description,
                                                             int n, int expires)
{
    return QMultiHash<QUuid,QUuid>();
}

ActiveBackend NetResourceManager::collectReservation(const QPair<QUuid,QUuid> &uid)
{
    return ActiveBackend();
}

QHash<QUuid,ActiveBackend>
NetResourceManager::collectReservation(const QMultiHash<QUuid,QUuid> &uids)
{
    return QHash<QUuid,ActiveBackend>();
}

void NetResourceManager::releaseReservation(const QPair<QUuid,QUuid> &uid)
{}

void NetResourceManager::releaseReservation(const QMultiHash<QUuid,QUuid> &uids)
{}
