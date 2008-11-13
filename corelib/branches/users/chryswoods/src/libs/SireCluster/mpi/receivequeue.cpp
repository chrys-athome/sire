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

#include "receivequeue.h"
#include "mpicluster.h"

using namespace SireCluster::MPI;

/** Construct a queue that listens for messages using 'recv_comm' */
ReceiveQueue::ReceiveQueue(::MPI::Intracomm &comm)
             : QThread(), boost::noncopyable(),
               recv_comm(comm)
{
    secondthread = new ReceiveQueue::SecondThread(this);
}

/** Destructor */
ReceiveQueue::~ReceiveQueue()
{
    datamutex.lock();
    keep_listening = false;
    message_queue.clear();
    datamutex.unlock();
    
    while (not this->wait(200))
    {
        waiter.wakeAll();
    }
    
    secondthread->wait();
    
    delete secondthread;
}

/** Start the two background event loops */
void ReceiveQueue::start()
{
    QThread::start();
    keep_listening = true;
    secondthread->start();
}

/** Add the received message to the received queue
    (only if this is an intended recipient of this message) */
void ReceiveQueue::received(const Message &message)
{
    if ((not message.isNull()) or message.isRecipient( MPICluster::getRank() ) )
    {
        QMutexLocker lkr(&datamutex);
        message_queue.enqueue(message);
        waiter.wakeAll();
    }
}

/** Stop the queue - clears all pending received messages */
void ReceiveQueue::stop()
{
    QMutexLocker lkr(&datamutex);
    keep_listening = false;
    message_queue.clear();
    waiter.wakeAll();
    lkr.unlock();
    
    while (not this->wait(200))
    {
        waiter.wakeAll();
    }
    
    secondthread->wait();
}

/** This function contains the event loop that reads and processes messages */
void ReceiveQueue::run()
{
    QMutexLocker lkr(&datamutex);
    
    if (message_queue.isEmpty())
    {
        waiter.wait(&datamutex);
    }
    
    while (not message_queue.isEmpty())
    {
        //get the next message to read
        Message message = message_queue.dequeue();
        lkr.unlock();

        message.read();
        
        if (message.hasReply())
        {
            //send the reply
            MPICluster::send( message.reply() );
        }
        
        lkr.relock();
        
        if (message_queue.isEmpty())
        {
            //wait until there are some more messages
            waiter.wait( &datamutex );
        }
    }
}

/** This function contains the event loop that receives the 
    messages using MPI */
void ReceiveQueue::run2()
{
    QByteArray message_data;

    if (MPICluster::isMaster())
    {
        ::MPI::Status status;

        while (keep_listening)
        {
        
            //the master listens for messages from anyone
            if (recv_comm.Iprobe(MPI_ANY_SOURCE, 1, status))
            {
                //there is a message from one of the slaves
                int slave_rank = status.Get_source();
                int count = status.Get_count(::MPI::BYTE);
                
                //receive the message
                message_data.resize(count + 1);
                
                recv_comm.Recv(message_data.data(), count, ::MPI::BYTE,
                               slave_rank, 1);

                //unpack the message
                Message message = Message::unpack(message_data);
                
                //are we the recipient?
                if (message.destination() == MPICluster::master())
                {
                    this->received(message);
                }
                else
                {
                    //we need to forward this on to where it has to go
                    MPICluster::send(message);
                }
            }
            else
                QThread::msleep(500);
        }
    }
    else
    {
        //everyone else listens to broadcasts from the master
        int count;
        
        recv_comm.Bcast( &count, 1, ::MPI::INT, MPICluster::master() );
        
        if (count == 0)
        {
            //we've just been told to quit
            return;
        }
        
        //receive the message
        message_data.resize(count + 1);
        
        recv_comm.Bcast( message_data.data(), count, ::MPI::BYTE, MPICluster::master() );
        
        this->received( Message::unpack(message_data) );
    }
}

#endif // __SIRE_USE_MPI__
