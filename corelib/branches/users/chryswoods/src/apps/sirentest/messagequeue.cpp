/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include <QSharedMemory>
#include <QTimer>

#include <cstring>

#include "messagequeue.h"
#include "sharedmemory.h"

#include <QDebug>

using namespace SirenTest;

/** Constructor */
MessageQueue::MessageQueue(QObject *parent) : QObject(parent), is_heads(true)
{
    shmem = new QSharedMemory(this);
    poller = new QTimer(this);
    
    SirenTest::registerSharedMemory(shmem);
    
    connect(poller, SIGNAL(timeout()), this, SLOT(pollForMessages()));

    poller->start(2);
}

/** Destructor */
MessageQueue::~MessageQueue()
{
    shmem->unlock();
    shmem->detach();
}

/** Create the message pipe as the 'heads' side of the pipe,
    using 'key' to specify the channel of communication */
bool MessageQueue::createAsHeads(const QString &key)
{
    if (shmem->isAttached())
        return false;
        
    is_heads = true;
    return this->create(key);
}

/** Create the message pipe as the 'tails' side of the pipe,
    using 'key' to specify the channel of communication */
bool MessageQueue::createAsTails(const QString &key)
{
    if (shmem->isAttached())
        return false;
        
    is_heads = false;
    return this->create(key);
}

/** Attach to the pipe as the 'heads' side of the pipe,
    using 'key' to specify the channel of communication */
bool MessageQueue::attachAsHeads(const QString &key)
{
    if (shmem->isAttached())
        return false;
        
    is_heads = true;
    return this->attach(key);
}

/** Attach to the pipe as the 'tails' side of the pipe,
    using 'key' to specify the channel of communication */
bool MessageQueue::attachAsTails(const QString &key)
{
    if (shmem->isAttached())
        return false;
        
    is_heads = false;
    return this->attach(key);
}

/** Create the pipe between the processes */
bool MessageQueue::create(const QString &key)
{
    //create this shared memory
    shmem = new QSharedMemory(key, this);
    SirenTest::registerSharedMemory(shmem);
    
    //create 128kB of shared memory
    if (not shmem->create(128 * 1024, QSharedMemory::ReadWrite))
    {
        return false;
    }

    /** Initialise this memory to all 0 */
    std::memset(shmem->data(), 0, shmem->size());
    
    return true;
}

/** Attach to the pipe */
bool MessageQueue::attach(const QString &key)
{
    shmem->setKey(key);
    
    return shmem->attach(QSharedMemory::ReadWrite);
}

/** Is there a message available to receive? */
bool MessageQueue::messageAvailable() const
{
    return not received_queue.isEmpty();
}

/** Return the next available received message - this
    will return a null array if there are no pending 
    messages to receive */
QByteArray MessageQueue::receive()
{
    if (not received_queue.isEmpty())
        return received_queue.dequeue();
    else
        return QByteArray();
}

/** Send a message */
void MessageQueue::send(const QByteArray &message)
{
    send_queue.enqueue(message);
    
    //don't wait for the next poll to send the message
    QTimer::singleShot(0, this, SLOT(pollForMessages()));
}

////// Buffer format
//////
//////   0 1 2 3   4 5 6 7   8 9 10 11  12 13 14 15  xxxxxxxxxxxxxxxxxxxxx
//////    status   total_size  chunk_size   n_left      data

bool MessageQueue::isHeads() const
{
    return is_heads;
}

bool MessageQueue::isTails() const
{
    return not is_heads;
}

QString MessageQueue::me() const
{
    if (isHeads())
        return "CLIENT";
    else
        return "SERVER";
}

/** Send the message 'message' - this is safe as we have
    sole access to the shared memory buffer */
void MessageQueue::sendData(const QByteArray &data)
{
    //we can only send data in chunks of shmem->count() - 16 bytes
    // (first 16 bytes are state, total size of the data, size of
    //  the data contained in this chunk, and the number of chunks left)
    quint32 max_size = shmem->size();
    
    if (max_size <= 16)
        return;
    
    max_size -= 16;
    
    qint8 *shmem_data = (qint8*)(shmem->data());

    if (partial_sent.isEmpty())
    {
        partial_sent = data;
        n_to_send = 0;
    }
    else
    {
        Q_ASSERT( partial_sent.constData() == data.constData() );
    }
        
    quint32 total_sz = partial_sent.count();
    quint32 chunk_sz = total_sz;
    
    if (total_sz > max_size)
        chunk_sz = max_size;

    quint32 nchunks = ((total_sz-1) / chunk_sz) + 1;
        
    quint32 send_sz = chunk_sz;
        
    if (n_to_send == 0)
        n_to_send = nchunks;
    else
    {
        n_to_send -= 1;
        
        if (n_to_send == 1)
        {
            send_sz = total_sz - (nchunks-1)*chunk_sz;
        }
    }
    
    if (isHeads())
        *((quint32*)(shmem_data)) = quint32( HEADS_SEND );
    else
        *((quint32*)(shmem_data)) = quint32( TAILS_SEND );
    
    *((quint32*)(shmem_data) + 1) = total_sz;
    *((quint32*)(shmem_data) + 2) = send_sz;
    *((quint32*)(shmem_data) + 3) = n_to_send;

    int start = (nchunks - n_to_send) * chunk_sz;
    
    std::memcpy(shmem_data+16+start, partial_sent.constData()+start, send_sz);
    
    if (n_to_send == 1)
        partial_sent = QByteArray();
    else
        //immediately poll to send the next part of the message
        QTimer::singleShot(0, this, SLOT(pollForMessages()));
}

/** Receive the next message */
bool MessageQueue::receiveData()
{    
    const int max_size = shmem->size() - 16;
    
    if (max_size <= 0)
        return false;
    
    qint8 *shmem_data = (qint8*)(shmem->data());
    
    quint32 total_sz = *((quint32*)(shmem_data+4));
    quint32 chunk_sz = *((quint32*)(shmem_data+8));
    quint32 n_left = *((quint32*)(shmem_data+12));
    
    if (partial_receive.isEmpty())
        partial_receive.reserve(total_sz);

    int start = partial_receive.size();
    int end = start + chunk_sz;

    partial_receive.resize(end);
    
    std::memcpy(partial_receive.data() + start, shmem_data+16, chunk_sz);
    
    //now clear the buffer
    std::memset(shmem->data(), 0, shmem->size());
    
    if (n_left == 1)
    {
        //we've finished receiving the data
        received_queue.enqueue(partial_receive);
        partial_receive = QByteArray();
        return true;
    }
    else
    {
        //immediately poll to receive the next part of the message
        QTimer::singleShot(0, this, SLOT(pollForMessages()));
        return false;
    }
}

/** Poll for messages - this looks to see if any messages need
    to be sent or received and takes appropriate action */
void MessageQueue::pollForMessages()
{
    if (not shmem->isAttached())
        return;

    try
    {
        shmem->lock();
        
        //the first 4 bytes of the shared memory is an integer
        //giving the state of the shared memory buffer
        quint32 state = *((quint32*)(shmem->data()));
    
        bool message_received = false;
    
        switch(state)
        {
            case EMPTY:
            {
                if (not partial_sent.isEmpty())
                    this->sendData(partial_sent);

                else if (not send_queue.isEmpty())
                    this->sendData(send_queue.dequeue());
                    
                break;
            }
            case HEADS_SEND:
            {
                if (isTails())
                    message_received = this->receiveData();
                    
                break;
            }
            case TAILS_SEND:
            {
                if (isHeads())
                    message_received = this->receiveData();
                    
                break;
            }
            default:
            {
                break;
            }
        }
        
        shmem->unlock();
        
        if (message_received)
            emit( receivedMessage() );
    }
    catch(...)
    {
        shmem->unlock();
        throw;
    }
}

/** Completely clear the message buffer */
void MessageQueue::clear()
{
    if (not shmem->isAttached())
        return;

    try
    {
        shmem->lock();
        
        std::memset( shmem->data(), 0, shmem->size() );
        
        partial_sent = QByteArray();
        partial_receive = QByteArray();
        send_queue.clear();
        received_queue.clear();
        
        shmem->unlock();
    }
    catch(...)
    {
        //shmem->unlock();
        throw;
    }
}
