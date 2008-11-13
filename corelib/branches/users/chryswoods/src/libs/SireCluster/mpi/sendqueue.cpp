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

#ifdef __SIRE_USE_MPI__

#include <mpi.h>  // needs to be first for mpich

#include "sendqueue.h"
#include "mpicluster.h"

using namespace SireCluster::MPI;
using namespace SireCluster::MPI::Messages;

/** Construct, using the passed MPI communicator to send the 
    messages */
SendQueue::SendQueue(::MPI::Intracomm &comm)
          : QThread(), boost::noncopyable(),
            send_comm(comm)
{}

/** Destructor */
SendQueue::~SendQueue()
{
    datamutex.lock();
    message_queue.clear();
    datamutex.unlock();
    
    waiter.wakeAll();
}

/** Start the event loop in a background thread */
void SendQueue::start()
{
    QThread::start();
}

/** Send the message 'message' */
void SendQueue::send(const Message &message)
{
    if (not message.isNull())
    {
        QMutexLocker lkr(&datamutex);
        message_queue.enqueue(message);
        waiter.wakeAll();
    }
}

/** Stop the queue - the clears all pending messages */
void SendQueue::stop()
{
    QMutexLocker lkr(&datamutex);
    message_queue.clear();
    waiter.wakeAll();
    lkr.unlock();
    
    while (not this->wait(200))
    {
        waiter.wakeAll();
    }
}

/** This is the event loop */
void SendQueue::run()
{
    QMutexLocker lkr(&datamutex);
    
    if (message_queue.isEmpty())
    {
        waiter.wait(&datamutex);
    }
    
    while (not message_queue.isEmpty())
    {
        Message message = message_queue.dequeue();

        lkr.unlock();

        if (message.destination() == MPICluster::getRank())
        {
            //the message is for us! - no need to send it
            MPICluster::received(message);
        }
        else if ( MPICluster::isMaster() )
        {
            //we can directly send the messages ourselves!
            if (message.destination() == -1 and 
                not message.isA<Broadcast>())
            {
                //this message is to be broadcast to everyone
                message = Broadcast(message);
            }
            
            //the master sends messages by broadcasting them to all
            //processes, and they are then only read by the intended
            //recipients
            QByteArray message_data = message.pack();
            
            int size = message_data.count();
            
            BOOST_ASSERT( size != 0 );
            
            //tell the nodes how large the message is
            send_comm.Bcast( &size, 1, ::MPI::INT, MPICluster::master());
            
            //now send them the actual message
            send_comm.Bcast( message_data.data(), message_data.count(),
                             ::MPI::BYTE, MPICluster::master() );
            
            //is the message intended for the master as well?
            if (message.isA<Broadcast>())
            {
                if (message.asA<Broadcast>().isRecipient(MPICluster::master()))
                {
                    MPICluster::received(message);
                }
            }
        }
        else
        {
            //we need to send the message to the master, for 
            //retransmission to the destination process
            Message broadcast( Broadcast(message, message.destination()) );
            
            QByteArray message_data = broadcast.pack();
            
            send_comm.Send( message_data.constData(), message_data.count(), 
                            ::MPI::BYTE, MPICluster::master(), 1 );
        }
        
        lkr.relock();
        
        if (message_queue.isEmpty())
        {
            //wait until there are some more messages
            waiter.wait( &datamutex );
        }
    }
    
    //there are no more messages being broadcast - we need to tell
    //the backend node of this fact
    if ( MPICluster::isMaster() )
    {
        int quit = 0;
        send_comm.Bcast( &quit, 1, ::MPI::INT, MPICluster::master());
    }
}

#endif // __SIRE_USE_MPI__
