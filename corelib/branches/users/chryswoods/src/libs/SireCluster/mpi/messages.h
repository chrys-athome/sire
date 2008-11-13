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

#ifndef SIRECLUSTER_MPI_MESSAGES_H
#define SIRECLUSTER_MPI_MESSAGES_H

#ifdef __SIRE_USE_MPI__

#include "SireBase/sharedpolypointer.hpp"

#include <QSharedData>
#include <QUuid>
#include <QSet>

SIRE_BEGIN_HEADER

namespace SireCluster
{
namespace MPI
{

class MessageBase;
class Message;

namespace Messages
{
class RegisterBackend;
class Shutdown;
class Broadcast;
}

}
}

QDataStream& operator<<(QDataStream&, const SireCluster::MPI::Message&);
QDataStream& operator>>(QDataStream&, SireCluster::MPI::Message&);

QDataStream& operator<<(QDataStream&, const SireCluster::MPI::MessageBase&);
QDataStream& operator>>(QDataStream&, SireCluster::MPI::MessageBase&);

QDataStream& operator<<(QDataStream&, 
                        const SireCluster::MPI::Messages::RegisterBackend&);
QDataStream& operator>>(QDataStream&, 
                        SireCluster::MPI::Messages::RegisterBackend&);

QDataStream& operator<<(QDataStream&, 
                        const SireCluster::MPI::Messages::Shutdown&);
QDataStream& operator>>(QDataStream&, 
                        SireCluster::MPI::Messages::Shutdown&);

QDataStream& operator<<(QDataStream&, 
                       const SireCluster::MPI::Messages::Broadcast&);
QDataStream& operator>>(QDataStream&, 
                        SireCluster::MPI::Messages::Broadcast&);

namespace SireCluster
{
namespace MPI
{

/** This is the virtual base class of all MPI messages sent between processes
    in the MPI cluster 
    
    @author Christopher Woods
*/
class MessageBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MessageBase&);
friend QDataStream& ::operator>>(QDataStream&, MessageBase&);

public:
    MessageBase(const MessageBase &other);
    
    ~MessageBase();
    
    static const char* typeName()
    {
        return "SireCluster::MPI::MessageBase";
    }
    
    virtual const char* what() const=0;
    
    virtual MessageBase* clone() const=0;
    
    virtual void read()=0;
    
    virtual bool hasReply() const;
    
    virtual Message reply() const;
    
    const QUuid& UID() const;
    const QUuid& subjectUID() const;
    
    int sender() const;
    int destination() const;
    
    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }
    
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }
    
protected:
    MessageBase(int destination);
    MessageBase();

    MessageBase& operator=(const MessageBase &other);

private:
    /** The unique ID of this message (this allows messages and
        responses to be tallied */
    QUuid uid;

    /** The ID of the subject that this message relates to
         - this allows several related messages to be exchanged
           and tracked */
    QUuid subject_uid;

    /** The MPI rank of the process that sent this message */
    qint32 sent_by;
    
    /** The MPI rank of the process that should receive this message */
    qint32 dest;
};

/** The polymorphic holder of all of the message classes

    @author Christopher Woods
*/
class Message
{

friend QDataStream& ::operator<<(QDataStream&, const Message&);
friend QDataStream& ::operator>>(QDataStream&, Message&);

public:
    Message();
    Message(const MessageBase &message);
    
    Message(const Message &other);
    
    ~Message();
    
    Message& operator=(const Message &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Message>() );
    }
    
    const char* what() const
    {
        return Message::typeName();
    }
    
    bool isNull() const;

    QByteArray pack() const;
    static Message unpack(const QByteArray &data);

    void read();
    
    bool hasReply() const;
    
    Message reply() const;
    
    const QUuid& UID() const;
    const QUuid& subjectUID() const;
    
    int sender() const;
    int destination() const;
    
    bool isRecipient(int rank) const;
    
    template<class T>
    bool isA() const
    {
        if (this->isNull())
            return false;
        else
            return base().isA<T>();
    }
    
    template<class T>
    const T& asA() const
    {
        return base().asA<T>();
    }

private:
    const MessageBase& base() const;

    /** Polymorphic, implicitly shared pointer to the message */
    SireBase::SharedPolyPointer<MessageBase> d;
};

namespace Messages
{

/** This is the message sent by the master node when it re-broadcasts
    messages sent by the slave nodes (as the slave nodes can't talk
    to each other directly - not until they open up a point-to-point
    communicator)
    
    @author Christopher Woods
*/
class Broadcast : public MessageBase
{

friend QDataStream& ::operator<<(QDataStream&, const Broadcast&);
friend QDataStream& ::operator>>(QDataStream&, Broadcast&);

public:
    Broadcast();
    Broadcast(const Message &message);
    Broadcast(const Message &message, int rank);
    Broadcast(const Message &message, const QSet<qint32> &ranks);
    
    Broadcast(const Broadcast &other);
    
    ~Broadcast();
    
    Broadcast& operator=(const Broadcast &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Broadcast>() );
    }
    
    const char* what() const
    {
        return Broadcast::typeName();
    }
    
    Broadcast* clone() const
    {
        return new Broadcast(*this);
    }
    
    void read();
    
    bool hasReply() const;
    
    Message reply() const;
    
    bool isRecipient(int rank) const;
    
private:
    /** The actual destination of this message - this is empty
        if it is meant to go to everybody */
    QSet<qint32> destinations;
    
    /** The data containing the message being broadcast */
    QByteArray message_data;
    
    /** Any reply to this message */
    Message replymsg;
};

/** This is the message sent by a slave node to tell the master
    node that a new Backend has been created on the slave which
    the master should register in the global registry 
    
    @author Christopher Woods
*/
class RegisterBackend : public MessageBase
{

friend QDataStream& ::operator<<(QDataStream&, const RegisterBackend&);
friend QDataStream& ::operator>>(QDataStream&, RegisterBackend&);

public:
    RegisterBackend();
    RegisterBackend(const QUuid &node_uid);

    RegisterBackend(const RegisterBackend &other);
    
    ~RegisterBackend();
    
    RegisterBackend& operator=(const RegisterBackend &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RegisterBackend>() );
    }
    
    const char* what() const
    {
        return RegisterBackend::typeName();
    }
    
    RegisterBackend* clone() const
    {
        return new RegisterBackend(*this);
    }
    
    void read();

private:
    /** The UID of the node to be registered */
    QUuid node_uid;
    
    /** The rank of the MPI process that contains this node */
    qint32 node_rank;
};

/** This is the message sent to a node to tell it to shutdown. This
    is used by the master node to tell all of the slaves
    to shutdown when the program is exiting 
    
    @author Christopher Woods
*/
class Shutdown : public MessageBase
{

friend QDataStream& ::operator<<(QDataStream&, const Shutdown&);
friend QDataStream& ::operator>>(QDataStream&, Shutdown&);

public:
    Shutdown();
    Shutdown(const Shutdown &other);
    
    ~Shutdown();
    
    Shutdown& operator=(const Shutdown &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Shutdown>() );
    }
    
    const char* what() const
    {
        return Shutdown::typeName();
    }
    
    Shutdown* clone() const
    {
        return new Shutdown(*this);
    }

    void read();
};

}  // end of namespace Messages
}  // end of namespace MPI
}  // end of namespace SireCluster

Q_DECLARE_METATYPE( SireCluster::MPI::Messages::Broadcast )
Q_DECLARE_METATYPE( SireCluster::MPI::Messages::RegisterBackend )
Q_DECLARE_METATYPE( SireCluster::MPI::Messages::Shutdown )

Q_DECLARE_METATYPE( SireCluster::MPI::Message )

SIRE_END_HEADER

#endif // __SIRE_USE_MPI__

#endif
