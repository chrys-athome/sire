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

#include "messages.h"
#include "mpicluster.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireCluster;
using namespace SireCluster::MPI;
using namespace SireCluster::MPI::Messages;

using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of MessageBase
/////////

static const RegisterMetaType<MessageBase> r_msgbase( MAGIC_ONLY, 
                                                      MessageBase::typeName() );
                                                      
/** Serialise to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const MessageBase &msgbase)
{
    writeHeader(ds, r_msgbase, 1);
    
    ds << msgbase.uid << msgbase.subject_uid
       << msgbase.sent_by << msgbase.dest;
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream& operator>>(QDataStream &ds, MessageBase &msgbase)
{
    VersionID v = readHeader(ds, r_msgbase);
    
    if (v == 1)
    {
        ds >> msgbase.uid >> msgbase.subject_uid
           >> msgbase.sent_by >> msgbase.dest;
    }
    else
        throw version_error( v, "1", r_msgbase, CODELOC );
        
    return ds;
}

/** Null constructor */
MessageBase::MessageBase()
            : QSharedData(), sent_by(-1), dest(-1)
{}

/** Construct a message that is intended to go to the MPI process
    with rank 'destination' */
MessageBase::MessageBase(int destination) 
            : QSharedData(), 
              uid( QUuid::createUuid() ),
              subject_uid( QUuid::createUuid() ),
              sent_by( MPICluster::getRank() ),
              dest(destination)
{}

/** Copy constructor */
MessageBase::MessageBase(const MessageBase &other)
            : QSharedData(),
              uid(other.uid),
              subject_uid(other.subject_uid),
              sent_by(other.sent_by), dest(other.dest)
{}

/** Destructor */
MessageBase::~MessageBase()
{}

/** Copy assignment operator */
MessageBase& MessageBase::operator=(const MessageBase &other)
{
    if (this != &other)
    {
        uid = other.uid;
        subject_uid = other.subject_uid;
        sent_by = other.sent_by;
        dest = other.dest;
    }
    
    return *this;
}

/** Return whether or not this message has a reply */
bool MessageBase::hasReply() const
{
    return false;
}

/** Return the reply to this message (which will be null, if hasReply() is false) */
Message MessageBase::reply() const
{
    return Message();
}

/** Return the unique ID of this message */
const QUuid& MessageBase::UID() const
{
    return uid;
}

/** Return the unique ID of the subject to which this 
    message relates */
const QUuid& MessageBase::subjectUID() const
{
    return subject_uid;
}

/** Return the rank of the MPI process that sent this message */
int MessageBase::sender() const
{
    return sent_by;
}

/** Return the rank of the MPI process to which this message
    should be sent */
int MessageBase::destination() const
{
    return dest;
}

/////////
///////// Implementation of Message
/////////

static const RegisterMetaType<Message> r_message;
                                                      
/** Serialise to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const Message &message)
{
    writeHeader(ds, r_message, 1);
    
    SharedDataStream sds(ds);
    sds << message.d;
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream& operator>>(QDataStream &ds, Message &message)
{
    VersionID v = readHeader(ds, r_message);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> message.d;
    }
    else
        throw version_error( v, "1", r_message, CODELOC );
        
    return ds;
}

/** Construct a null message */
Message::Message()
{}

/** Construct from the message 'message' */
Message::Message(const MessageBase &message)
        : d(message)
{}

/** Copy constructor */
Message::Message(const Message &other)
        : d(other.d)
{}

/** Destructor */
Message::~Message()
{}

/** Copy assignment operator */
Message& Message::operator=(const Message &other)
{
    d = other.d;
    return *this;
}

/** Return whether or not this is a null message */
bool Message::isNull() const
{
    return d.constData() == 0;
}

/** Internal function used to return the base object of the message */
const MessageBase& Message::base() const
{
    return *d;
}

/** Pack this message into a QByteArray */
QByteArray Message::pack() const
{
    QByteArray data;
    QDataStream ds(data);
    ds << *this;
    
    return data;
}

/** Load and return a message from the passed data */
Message Message::unpack(const QByteArray &data)
{
    QDataStream ds(data);
    
    Message message;
    ds >> message;
    
    return message;
}

/** Read the message - this performs any actions contained therein */
void Message::read()
{
    if (not this->isNull())
    {
        d->read();
    }
}

/** Return whether or not this message has a reply */
bool Message::hasReply() const
{
    if (not this->isNull())
    {
        return d->hasReply();
    }
    else
        return false;
}

/** Return any reply associated with this message
    (a null Message will be returned if there is no reply) */
Message Message::reply() const
{
    if (not this->isNull())
    {
        return d->reply();
    }
    else
        return Message();
}

static QUuid null_uid;    

/** Return the unique ID of this message */
const QUuid& Message::UID() const
{
    if (not this->isNull())
    {
        return d->UID();
    }
    else
        return null_uid;
}

/** Return the unique ID of the subject of this message */
const QUuid& Message::subjectUID() const
{
    if (not this->isNull())
    {
        return d->subjectUID();
    }
    else
        return null_uid;
}

/** Return the rank of the MPI process that originally
    sent this message (-1 if this is not known) */
int Message::sender() const
{
    if (not this->isNull())
    {
        return d->sender();
    }
    else
        return -1;
}

/** Return the rank of the MPI process to which this
    message should be sent (-1 if it is to go to everyone) */
int Message::destination() const
{
    if (not this->isNull())
    {
        return d->destination();
    }
    else
        return -1;
}

/** Return whether this is one of the recipients of this message */
int Message::isRecipient(int rank) const
{
    int dest = this->destination();

    return dest == -1 or dest == rank;
}

/////////
///////// Implementation of Broadcast
/////////

static const RegisterMetaType<Broadcast> r_broadcast;
                                                      
/** Serialise to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const Broadcast &broadcast)
{
    writeHeader(ds, r_broadcast, 1);
    
    SharedDataStream sds(ds);
    sds << broadcast.destinations
        << broadcast.message_data
        << broadcast.replymsg
        << static_cast<const MessageBase&>(broadcast);
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream& operator>>(QDataStream &ds, Broadcast &broadcast)
{
    VersionID v = readHeader(ds, r_message);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> broadcast.destinations
            >> broadcast.message_data
            >> broadcast.replymsg
            >> static_cast<MessageBase&>(broadcast);
    }
    else
        throw version_error( v, "1", r_broadcast, CODELOC );
        
    return ds;
}

/** Constructor */
Broadcast::Broadcast() : MessageBase()
{}

/** Construct to broadcast the message 'message' to
    every single MPI process */
Broadcast::Broadcast(const Message &message)
          : MessageBase(-1)  // -1 means broadcast to all
{
    if (message.isA<Broadcast>())
    {
        this->operator=( message.asA<Broadcast>() );
    }
    else
    {
        message_data = message.pack();
    }
}       

/** Construct to broadcast this message to just the MPI 
    process with rank 'rank' */
Broadcast::Broadcast(const Message &message, int rank)
          : MessageBase(-1),
            message_data( message.pack() )
{
    if (rank != -1) // -1 means broadcast to everyone
    {
        destinations.insert(rank);
    }
    
    if (message.isA<Broadcast>())
    {
        message_data = message.asA<Broadcast>().message_data;
    }
    else
    {
        message_data = message.pack();
    }
}

/** Construct to broadcast this message to the MPI processes
    with ranks that are in 'ranks' */
Broadcast::Broadcast(const Message &message, const QSet<qint32> &ranks)
          : MessageBase(-1),
            destinations(ranks),
            message_data( message.pack() )
{
    if (destinations.contains(-1))
    {
        //this should be sent to everyone
        destinations = QSet<qint32>();
    }

    if (message.isA<Broadcast>())
    {
        message_data = message.asA<Broadcast>().message_data;
    }
    else
    {
        message_data = message.pack();
    }
}

/** Copy constructor */
Broadcast::Broadcast(const Broadcast &other)
          : MessageBase(other),
            destinations(other.destinations),
            message_data(other.message_data), 
            replymsg(other.replymsg)
{}

/** Destructor */
Broadcast::~Broadcast()
{}

/** Copy assignment operator */
Broadcast& Broadcast::operator=(const Broadcast &other)
{
    if (this != &other)
    {
        destinations = other.destinations;
        message_data = other.message_data;
        replymsg = other.replymsg;
        MessageBase::operator=(other);
    }
    
    return *this;
}

/** Return whether or not the process with MPI rank 'rank' 
    is an intended recipient of this broadcast */
bool Broadcast::isRecipient(int rank) const
{
    return destinations.isEmpty() or destinations.contains(rank);
}

/** Read this message - this will only read this message if
    it is really intended for this node */
void Broadcast::read()
{
    if ( this->isRecipient(MPICluster::getRank()) )
    {
        //yes - we are an intended recipient
        Message message = Message::unpack(message_data);
        
        //read the real message
        message.read();
        
        //save any reply
        if (message.hasReply())
        {
            replymsg = message.reply();
        }
    }
}

/** Return whether or not the message resulted in a reply */
bool Broadcast::hasReply() const
{
    return not replymsg.isNull();
}

/** Return the reply that resulted from this message */
Message Broadcast::reply() const
{
    return replymsg;
}

/////////
///////// Implementation of RegisterBackend
/////////

static const RegisterMetaType<RegisterBackend> r_regbackend;
                                                      
/** Serialise to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const RegisterBackend &regbackend)
{
    writeHeader(ds, r_regbackend, 1);
    
    ds << regbackend.node_uid << regbackend.node_rank
       << static_cast<const MessageBase&>(regbackend);
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream& operator>>(QDataStream &ds, RegisterBackend &regbackend)
{
    VersionID v = readHeader(ds, r_regbackend);
    
    if (v == 1)
    {
        ds >> regbackend.node_uid >> regbackend.node_rank
           >> static_cast<MessageBase&>(regbackend);
    }
    else
        throw version_error( v, "1", r_regbackend, CODELOC );
        
    return ds;
}

/** Constructor */
RegisterBackend::RegisterBackend() : node_rank(-1)
{}

/** Construct the message to register the node with unique
    ID 'node_uid' that resides on this MPI process */
RegisterBackend::RegisterBackend(const QUuid &uid)
                : MessageBase( MPICluster::master() ),  // must be sent to the master
                  node_uid(uid),
                  node_rank( MPICluster::getRank() )
{}

/** Copy constructor */
RegisterBackend::RegisterBackend(const RegisterBackend &other)
                : MessageBase(other),
                  node_uid(other.node_uid),
                  node_rank(other.node_rank)
{}

/** Destructor */
RegisterBackend::~RegisterBackend()
{}

/** Copy assignment operator */
RegisterBackend& RegisterBackend::operator=(const RegisterBackend &other)
{
    if (this != &other)
    {
        node_uid = other.node_uid;
        node_rank = other.node_rank;
        MessageBase::operator=(other);
    }
    
    return *this;
}

/** Read this message - this must only occur on the master process! */
void RegisterBackend::read()
{
    //register the node
    MPICluster::registerBackend(node_rank, node_uid);
}

/////////
///////// Implementation of Shutdown
/////////

static const RegisterMetaType<Shutdown> r_shutdown;
                                                      
/** Serialise to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const Shutdown &shutdown)
{
    writeHeader(ds, r_shutdown, 1);
    
    ds << static_cast<const MessageBase&>(shutdown);
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream& operator>>(QDataStream &ds, Shutdown &shutdown)
{
    VersionID v = readHeader(ds, r_shutdown);
    
    if (v == 1)
    {
        ds >> static_cast<MessageBase&>(shutdown);
    }
    else
        throw version_error( v, "1", r_shutdown, CODELOC );
        
    return ds;
}

/** Construct a message to tell everyone to shut down */
Shutdown::Shutdown() : MessageBase(-1)  // goes to everyone
{}

/** Copy constructor */
Shutdown::Shutdown(const Shutdown &other) : MessageBase(other)
{}

/** Destructor */
Shutdown::~Shutdown()
{}

/** Copy assignment operator */
Shutdown& Shutdown::operator=(const Shutdown &other)
{
    MessageBase::operator=(other);
    return *this;
}

/** Read this message */
void Shutdown::read()
{
    MPICluster::informedShutdown();
}

#endif // __SIRE_USE_MPI__
