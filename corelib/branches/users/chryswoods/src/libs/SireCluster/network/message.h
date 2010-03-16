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

#ifndef SIRECLUSTER_NETWORK_MESSAGE_H
#define SIRECLUSTER_NETWORK_MESSAGE_H

#include <QUuid>
#include <QByteArray>

#include "Siren/object.h"
#include "Siren/objptr.hpp"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Siren{ class exception; }

namespace SireCluster
{
namespace network
{

/** This is the virtual base class of all Message objects.
    A Message is a piece of data that can be sent via
    the SireCluster::network subsystem - using the 
    Communicator classes. Messages are active - i.e.
    they are acted on as soon as they arrive. The 
    ".read()" function of the message opens the message,
    and performs the actions that are contained. The 
    .read() function may also then send out more
    messages, and can also send a reply() back to
    the original sender.
  
    Messages are derived from Siren::Object, so are
    implicitly shared const classes
        
    @author Christopher Woods
*/
class Message : public Siren::Extends<Message,Siren::Object>
{
public:
    Message();
    Message(const Message &other);
    
    ~Message();
    
    static QString typeName();
    
    virtual void read(const QUuid &sender, quint64 message_id) const=0;
    
protected:
    Message& operator=(const Message &other);
    bool operator==(const Message &other) const;
    bool operator!=(const Message &other) const;
};

/** This message is used to send a reply back to the sender of a message */
class Reply : public Siren::Implements<Reply,Message>
{
public:
    Reply();
    Reply(const QUuid &host, quint64 msgid, const QByteArray &contents);
    Reply(const QUuid &host, quint64 msgid, const Siren::exception &error);
    
    Reply(const Reply &other);
    
    ~Reply();
    
    Reply& operator=(const Reply &other);
    
    bool operator==(const Reply &other) const;
    bool operator!=(const Reply &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;
    
    bool isNull() const;
    
    bool isError() const;
    void throwError() const;
    
    bool isEmpty() const;
    
    const QUuid& sender() const;
    
    const QByteArray& contents() const;
    
private:
    /** The contents of this reply */
    QByteArray reply_contents;
    
    /** The UID of the host that sent this reply */
    QUuid sender_uid;
    
    /** The ID of the message that this is in reply to */
    quint64 msgid;
    
    /** Whether or not this reply is an error */
    bool is_error;
};

/** This message is used to tell a node in the cluster to shutdown */
class Shutdown : public Siren::Implements<Shutdown,Message>
{
public:
    Shutdown();
    Shutdown(const Shutdown &other);
    
    ~Shutdown();
    
    Shutdown& operator=(const Shutdown &other);
    
    bool operator==(const Shutdown &other) const;
    bool operator!=(const Shutdown &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;
};

} // end of namespace network
} // end of namespace SireCluster

Q_DECLARE_METATYPE( SireCluster::network::Reply )
Q_DECLARE_METATYPE( SireCluster::network::Shutdown )

SIRE_END_HEADER

#endif
