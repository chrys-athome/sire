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

#ifdef SIRE_USE_MPI

#include <mpi.h>  // needs to be first for mpich

#include "sendqueue.h"
#include "mpicluster.h"

#include "SireMaths/rangenerator.h"

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include <QDebug>

using namespace SireCluster::MPI;
using namespace SireCluster::MPI::Messages;

/** Construct, using the passed MPI communicator to send the 
    messages */
SendQueue::SendQueue(::MPI::Intracomm *comm)
          : QThread(), boost::noncopyable(),
            send_comm(comm), been_stopped(false)
{}

/** Destructor */
SendQueue::~SendQueue()
{
    datamutex.lock();
    message_queue.clear();
    been_stopped = true;
    datamutex.unlock();
    
    while (not QThread::wait(200))
    {
        waiter.wakeAll();
    }
    
    delete send_comm;
}

/** Start the event loop in a background thread */
void SendQueue::start()
{
    QMutexLocker lkr(&datamutex);
    been_stopped = false;
    QThread::start();
}

/** Send the message 'message' */
void SendQueue::send(const Message &message)
{
    if (not message.isNull())
    {
        QMutexLocker lkr(&datamutex);
        
        if (not been_stopped)
            message_queue.enqueue(message);

        waiter.wakeAll();
    }
}

/** Wait until the queue has finished */
void SendQueue::wait()
{
    while (not QThread::wait(200))
    {
        waiter.wakeAll();
    }
}

/** Stop the queue - the clears all pending messages */
void SendQueue::stop()
{
    QMutexLocker lkr(&datamutex);
    message_queue.clear();
    been_stopped = true;
    waiter.wakeAll();
    lkr.unlock();
}

/** Return whether or not this queue is running */
bool SendQueue::isRunning()
{
    return QThread::isRunning();
}

/** This is the event loop */
void SendQueue::run()
{
    SireError::setThreadString("SendQueue");
    SireMaths::seed_qrand();
    
    QMutexLocker lkr(&datamutex);
    
    if (message_queue.isEmpty())
    {
        waiter.wait(&datamutex);
    }
    
    while (not message_queue.isEmpty())
    {
        if (been_stopped)
            //we've been stopped!
            break;
    
        Message message = message_queue.dequeue();
        lkr.unlock();

        //qDebug() << MPICluster::getRank() << "sending" << message.toString()
        //         << "to" << message.destination();

        try
        {
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
                send_comm->Bcast( &size, 1, ::MPI::INT, MPICluster::master());
                
                //now send them the actual message
                send_comm->Bcast( message_data.data(), message_data.count(),
                                  ::MPI::BYTE, MPICluster::master() );
                
                //is the message intended for the master as well?
                if (message.isA<Broadcast>())
                {
                    if (message.asA<Broadcast>().isRecipient(MPICluster::master()))
                    {
                        //if this is a shutdown, then process it here
                        //(to prevent a deadlock if the receive queue is still working)
                        if (message.isA<Shutdown>())
                        {
                            message.read();
                            been_stopped = true;
                        }
                        else
                            MPICluster::received(message);
                    }
                }
            }
            else
            {
                QByteArray message_data;
            
                if (message.destination() == MPICluster::master())
                {
                    //send this message to the master directly
                    message_data = message.pack();
                }
                else
                {
                    //we need to send the message to the master, for 
                    //retransmission to the destination process
                    Message broadcast( Broadcast(message, message.destination()) );
                    message_data = broadcast.pack();
                }
                
                //maybe change to Isend so that we can kill the send if
                //'been_stopped' is true
                send_comm->Send( message_data.constData(), message_data.count(), 
                                 ::MPI::BYTE, MPICluster::master(), 1 );
            }
        }
        catch(const SireError::exception &e)
        {
            MPICluster::send( Messages::Error(message, e) );
        }
        catch(const std::exception &e)
        {
            MPICluster::send( Messages::Error(message, e) );
        }
        catch(...)
        {
            MPICluster::send( Messages::Error(message, CODELOC) );
        }
        
        lkr.relock();
        
        if (message_queue.isEmpty())
        {
            //wait until there are some more messages
            waiter.wait( &datamutex );
        }
    }
    
    //there are no more messages being broadcast - we need to tell
    //the backend nodes of this fact
    if ( MPICluster::isMaster() )
    {
        int quit = 0;
        //send_comm->Barrier();
        send_comm->Bcast( &quit, 1, ::MPI::INT, MPICluster::master());
    }
    
    //we're not sending any more messages, so release the resources
    //held by the communicator
    send_comm->Free();

    delete send_comm;

    send_comm = 0;
}

#endif // SIRE_USE_MPI
