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

#include "receivequeue.h"
#include "mpicluster.h"

#include "SireMaths/rangenerator.h"

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include <QDebug>

using namespace SireCluster::MPI;

/** Construct a queue that listens for messages using 'recv_comm' */
ReceiveQueue::ReceiveQueue(::MPI::Intracomm *comm)
             : QThread(), boost::noncopyable(),
               recv_comm(comm), been_stopped(false)
{
    secondthread = new ReceiveQueue::SecondThread(this);
}

/** Destructor */
ReceiveQueue::~ReceiveQueue()
{
    datamutex.lock();
    keep_listening = false;
    been_stopped = true;
    message_queue.clear();
    datamutex.unlock();
    
    while (not QThread::wait(200))
    {
        waiter.wakeAll();
    }
    
    secondthread->wait();
    
    delete secondthread;
    
    delete recv_comm;
}

/** Start the two background event loops */
void ReceiveQueue::start()
{
    been_stopped = false;
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
        
        if (not been_stopped)
            message_queue.enqueue(message);
            
        waiter.wakeAll();
    }
}

/** Stop the queue - clears all pending received messages */
void ReceiveQueue::stop()
{
    QMutexLocker lkr(&datamutex);
    keep_listening = false;
    been_stopped = true;
    message_queue.clear();
    waiter.wakeAll();
    lkr.unlock();
}

/** Wait until the queues have stopped */
void ReceiveQueue::wait()
{
    secondthread->wait();
    
    while (not QThread::wait(200))
    {
        waiter.wakeAll();
    }
}

/** Return whether or not this queue is still running */
bool ReceiveQueue::isRunning()
{
    return secondthread->isRunning() or QThread::isRunning();
}

/** This function contains the event loop that reads and processes messages */
void ReceiveQueue::run()
{
    SireError::setThreadString("ReceiveQueue_A");
    SireMaths::seed_qrand();

    QMutexLocker lkr(&datamutex);
    
    if (message_queue.isEmpty())
    {
        waiter.wait(&datamutex);
    }

    while (not message_queue.isEmpty())
    {
        if (been_stopped)
            break;
    
        //get the next message to read
        Message message = message_queue.dequeue();
        lkr.unlock();

        //qDebug() << MPICluster::getRank() << "received" << message.toString()
        //         << "from" << message.sender();

        try
        {
            message.read();
        }
        catch (const SireError::exception &e)
        {
            MPICluster::send( Messages::Error(message, e) );
        }
        catch (const std::exception &e)
        {
            MPICluster::send( Messages::Error(message, e) );
        }
        catch (...)
        {
            MPICluster::send( Messages::Error(message, CODELOC) );
        }
        
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

/** This function unpacks the message contained in 'message_data'
    that was sent by the node with rank 'rank', and returns it.
    If something goes wrong an empty (null) message is returned */
Message ReceiveQueue::unpackMessage(const QByteArray &message_data, int sender) const
{
    try
    {
        return Message::unpack(message_data);
    }
    catch (const SireError::exception &e)
    {
        MPICluster::send( Messages::Error(sender, e) );
    }
    catch (const std::exception &e)
    {
        MPICluster::send( Messages::Error(sender, e) );
    }
    catch (...)
    {
        MPICluster::send( Messages::Error(sender, CODELOC) );
    }
    
    return Message();
}

/** This function contains the event loop that receives the 
    messages using MPI */
void ReceiveQueue::run2()
{
    SireError::setThreadString("ReceiveQueue_B");
    SireMaths::seed_qrand();

    //wait until everyone has got here
    recv_comm->Barrier();

    QByteArray message_data;

    if (MPICluster::isMaster())
    {
        ::MPI::Status status;

        while (keep_listening)
        {        
            //the master listens for messages from anyone
            if (recv_comm->Iprobe(MPI_ANY_SOURCE, 1, status))
            {
                //there is a message from one of the slaves
                int slave_rank = status.Get_source();
                int count = status.Get_count(::MPI::BYTE);
                
                //receive the message
                message_data.resize(count + 1);
                
                //perhaps change this to use Irecv so that we can kill
                //the communication if 'keep_listening' is set to false
                recv_comm->Recv(message_data.data(), count, ::MPI::BYTE,
                                slave_rank, 1);

                //unpack the message
                Message message = this->unpackMessage(message_data, slave_rank);

                if (message.isNull())
                    continue;
                
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
                QThread::msleep(200);
        }
    }
    else
    {
        //everyone else listens to broadcasts from the master
        while (true)
        {

            //block until everyone is ready to receive the message
            recv_comm->Barrier();

            int count;
            recv_comm->Bcast( &count, 1, ::MPI::INT, MPICluster::master() );
        
            if (count == 0)
            {
                //we've just been told to quit
                break;
            }
        
            //receive the message
            message_data.resize(count + 1);
        
            recv_comm->Bcast( message_data.data(), count, 
                              ::MPI::BYTE, MPICluster::master() );
        
            Message message = this->unpackMessage( message_data,
                                                   MPICluster::master() );
        
            if (not message.isNull())
            {
                if (message.isA<Messages::Shutdown>())
                {
                    if (message.isRecipient(MPICluster::getRank()))
                    {
                        //shutdown here and now - don't queue this message
                        //as we could deadlock at shutdown if we do!
                        message.read();
                        
                        //the master will send a zero size message to signal
                        //that it has also quit - wait for that message now
                        recv_comm->Barrier();
                        recv_comm->Bcast( &count, 1, ::MPI::INT, MPICluster::master() );
                        
                        if (count != 0)
                            qDebug() << "Shutdown has not been followed by a zero "
                                     << "shutdown message from the master?" << count;
                        
                        //stop listening for any further messages
                        break;
                    }
                }
                else
                {
                    if (message.isRecipient(MPICluster::getRank()))
                        this->received(message);
                }
            }
        }
    }

    recv_comm->Barrier();
    
    //release the resources held by this communicator
    recv_comm->Free();
    
    delete recv_comm;
    recv_comm = 0;
}

#endif // SIRE_USE_MPI
