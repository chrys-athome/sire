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

#include <QDebug>

using namespace SireCluster::MPI;
using namespace Siren;

/** Construct, using the passed MPI communicator to send the 
    messages */
SendQueue::SendQueue(::MPI::Intracomm comm)
          : Thread(QObject::tr("SendQueue")), boost::noncopyable(),
            send_comm(comm), been_stopped(false)
{}

/** Destructor */
SendQueue::~SendQueue()
{
    datamutex.lock();
    message_queue.clear();
    been_stopped = true;
    datamutex.unlock();

    int count = 0;
    
    while (not Thread::wait(200))
    {
        waiter.wakeAll();
        
        ++count;
        
        if (count > 20)
            break;
    }

    send_comm.Free();
}

/** Start the event loop in a background thread */
void SendQueue::start()
{
    MutexLocker lkr(&datamutex);
    been_stopped = false;
    Thread::start();
}

/** Send a message contained in the envelope 'envelope' */
void SendQueue::send(const Envelope &envelope)
{
    if (envelope.isEmpty())
        return;

    MutexLocker lkr(&datamutex);
        
    if (not been_stopped)
        message_queue.enqueue(envelope);

    waiter.wakeAll();
}

/** Wait until the queue has finished */
void SendQueue::wait()
{
    while (not Thread::wait(200))
    {
        waiter.wakeAll();
    }
}

/** Stop the queue - the clears all pending messages */
void SendQueue::stop()
{
    MutexLocker lkr(&datamutex);
    message_queue.clear();
    been_stopped = true;
    waiter.wakeAll();
    lkr.unlock();
}

/** Kill the queue - this clears all pending messages */
void SendQueue::kill()
{
    this->stop();
}

/** Return whether or not this queue is running */
bool SendQueue::isRunning()
{
    return Thread::isRunning();
}

/** This is the event loop */
void SendQueue::threadMain()
{
    MutexLocker lkr(&datamutex);
    
    if (message_queue.isEmpty())
    {
        waiter.wait(&datamutex);
    }
    
    while (not message_queue.isEmpty())
    {
        if (been_stopped)
            //we've been stopped!
            break;
    
        Envelope message = message_queue.dequeue();
        lkr.unlock();

        try
        {
            if (message.isRecipient(MPICluster::processUID()) and
                message.hasSingleRecipient())
            {
                //no need to send the message as we are the only recipient
            }
            else if ( MPICluster::isMaster() )
            {
                //the master can send the message directly
                
                //get the MPI ranks of the processes to send this message to
                QSet<int> recipient_ranks;
                
                if ( message.isBroadcast() )
                {
                    //this message is to be broadcast to everyone in the 
                    //global communicator (rank 1 to count()-1)
                    for (int i=0; i < ::MPI::COMM_WORLD.Get_size()-1; ++i)
                    {
                        recipient_ranks.insert(i);
                    }
                }
                else 
                {
                    //look up the ranks of the recipients
                    
                }

                //the master sends the message to just the intended recipients
                QByteArray message_data = message.pack();
                
                int size = message_data.count();
                
                BOOST_ASSERT( size != 0 );
                
                if (message.isA<Broadcast>())
                {
                    QSet<int> recipients = message.asA<Broadcast>().recipients();
                    
                    foreach (int recipient, recipients)
                    {
                        if (recipient != MPICluster::master())
                        {
                            send_comm.Isend( &size, 1, ::MPI::INT, recipient, 1 );
                        }
                    }
                    
                    foreach (int recipient, recipients)
                    {
                        if (recipient != MPICluster::master())
                        {
                            send_comm.Send( message_data.data(), message_data.count(),
                                            ::MPI::BYTE, recipient, 1 );
                        }
                    }

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
                else
                {
                    int recipient = message.destination();
                    
                    if (recipient != MPICluster::master())
                    {
                        send_comm.Isend( &size, 1, ::MPI::INT, recipient, 1 );
                        send_comm.Send( message_data.data(), message_data.count(),
                                        ::MPI::BYTE, recipient, 1 );
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
                send_comm.Send( message_data.constData(), message_data.count(), 
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
        for (int i=0; i<MPICluster::getCount(); ++i)
        {
            if (i != MPICluster::master())
                send_comm.Send( &quit, 1, ::MPI::INT, i, 1 );
        }
    }
    
    //we're not sending any more messages, so release the resources
    //held by the communicator
    send_comm.Free();
}

#endif // SIRE_USE_MPI
