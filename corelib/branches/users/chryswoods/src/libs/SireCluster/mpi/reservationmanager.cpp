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

#include "reservationmanager.h"
#include "mpicluster.h"
#include "reply.h"

#include "SireCluster/frontend.h"

using namespace SireCluster;
using namespace SireCluster::MPI;
using namespace SireCluster::MPI::Messages;

using boost::tuple;

Q_GLOBAL_STATIC( ReservationManager, reservationManager );

/** Constructor */
ReservationManager::ReservationManager() : keep_going(false)
{}

/** Destructor */
ReservationManager::~ReservationManager()
{
    this->wait();
}

/** Start the reservation manager */
void ReservationManager::start()
{
    ReservationManager *d = reservationManager();

    if (MPICluster::isMaster())
    {
        QMutexLocker lkr( &(d->datamutex) );
    
        if (d->keep_going)
            //it is already running
            return;
        
        d->keep_going = true;
        
        static_cast<QThread*>(d)->start();
    }
}

/** Release all backends that have been reserved */
void ReservationManager::freeReservedBackends()
{
    reserved_backends.clear();
}

/** Shutdown the reservation manager */
void ReservationManager::shutdown()
{
    ReservationManager *d = reservationManager();

    if (MPICluster::isMaster())
    {
        QMutexLocker lkr( &(d->datamutex) );
    
        d->keep_going = false;
    
        d->waiter.wakeAll();

        d->wait();
    }
    
    //now free all of the backends that have been reserved
    d->freeReservedBackends();
}

/** Process the request 'request' - this sends out the request
    for availability and then processes the results */
void ReservationManager::processRequest(ReserveBackend request, Reply reply)
{
    //this contains the list of available backends
    QList< tuple<int,QUuid> > backend_uids;

    //get all of the replies
    QHash<int,ReplyValue> replies = reply.replies();

    for (QHash<int,ReplyValue>::const_iterator it = replies.constBegin();
         it != replies.constEnd(); 
         ++it)
    {
        if (not it->isError())
        {
            QList<QUuid> available_backends = it->asA< QList<QUuid> >();
                                                        
            foreach (QUuid available_backend, available_backends)
            {
                backend_uids.append( tuple<int,QUuid>(it.key(),available_backend) );
            }
        }
    }
    
    //ok - we now have the list - work out if it is what we want
    Message message;
    
    if (request.requestedUID().isNull())
    {
        //we have requested 'n' nodes
        QList< tuple<int,QUuid> > reserve_uids;
        
        int navailable = qMin(backend_uids.count(), request.nBackends());
        
        for (int i=0; i<navailable; ++i)
        {
            reserve_uids.append( backend_uids[i] );
        }
        
        message = Reservation(reserve_uids, request);
    }
    else
    {
        if (backend_uids.count() == 1)
            message = Reservation(backend_uids, request);
        
        else    
            message = Reservation( QList< tuple<int,QUuid> >(), request );
    }
        
    Reply reservation_reply(message);

    //broadcast the reservation to the cluster - this allows nodes
    //that are not involved to remove themselves from the point-to-point
    //communicator, and to also tell them that they can free the nodes
    //that were temporarily reserved
    MPICluster::send(message);
        
    //wait for all of the nodes to have responded to the request
    reservation_reply.wait();

    //send a reply back to the original process that requested the
    //nodes, that contains the list of node UIDs that have connected
    replies = reservation_reply.replies();
    
    QList<QUuid> connected_backends;
    
    for (QHash<int,ReplyValue>::const_iterator it = replies.constBegin();
         it != replies.constEnd();
         ++it)
    {
        if (not it->isError())
        {
            connected_backends += it->asA< QList<QUuid> >();
        }
    }

    MPICluster::send( Result(request, connected_backends) );
}

/** The event loop that runs in a background thread - this 
    manages the reservations */
void ReservationManager::run()
{
    QMutexLocker lkr(&datamutex);
    
    while (keep_going)
    {
        if (not request_queue.isEmpty())
        {
            //we have a request to process
            tuple<ReserveBackend,Reply> request = request_queue.dequeue();
            lkr.unlock();
            
            this->processRequest(request.get<0>(), request.get<1>());
            
            lkr.relock();
        }
        else if (keep_going)
            waiter.wait( &datamutex );
    }
    
    //send a response back to any remaining requests saying that
    //no backends are available
    while (not request_queue.isEmpty())
    {
        tuple<ReserveBackend,Reply> request = request_queue.dequeue();
        MPICluster::send( Result(request.get<0>(), QList<QUuid>()) );
    }
}

/** Tell the manager to wait for the response to the request 'request'
    that will be posted using the reply 'reply' */
void ReservationManager::awaitResponse( const ReserveBackend &request, 
                                        const Reply &reply )
{
    ReservationManager *d = reservationManager();
    
    QMutexLocker lkr( &(d->datamutex) );
    d->request_queue.enqueue( tuple<ReserveBackend,Reply>(request,reply) );

    d->waiter.wakeAll();
}

/** Reserve all available backends that match the request 'request'
    and return the UIDs of all matching reserved backends */                           
QList<QUuid> ReservationManager::findAvailable( const ReserveBackend &request )
{
    return QList<QUuid>();
}

/** Collect on the request made with the message 'request', collecting
    the frontend to the backend with UID 'uid', and optionally cancelling
    the reservation of the remaining backends */
Frontend ReservationManager::collectReservation(const ReserveBackend &request,
                                                const QUuid &uid,
                                                bool dispose_of_rest)
{
    return Frontend();
}

/** Collect on the request made with the message 'request', collecting all
    the frontends to the backend whose UIDs are in 'uids', and optionally 
    cancelling the reservation of the remaining backends */
QList<Frontend> ReservationManager::collectReservation(const ReserveBackend &request,
                                                       const QList<QUuid> &uid,
                                                       bool dispose_of_rest)
{
    return QList<Frontend>();
}

/** Establish the point-to-point connections between the MPI processes
    as detailed in 'details', in response to the request in 'request' */
void ReservationManager::establishConnections(const QList< tuple<int,QUuid> > &details,
                                              const ReserveBackend &request )
{
}
 
/** Return the UIDs of all of the backends that have connections
    established in response to the request 'request' */
QList<QUuid> ReservationManager::establishedConnections(const ReserveBackend &request)
{
    return QList<QUuid>();
}
