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

#include "message.h"
#include "communicator.h"
#include "hostinfo.h"

#include "SireCluster/cluster.h"

#include "Siren/datastream.h"
#include "Siren/streamqt.h"

#include "Siren/exception.h"

#include "SireCluster/errors.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::network;
using namespace Siren;

//////
////// Implementation of Message
//////

static const RegisterObject<Message> r_message( VIRTUAL_CLASS );

/** Constructor */
Message::Message() : Extends<Message,Object>()
{}

/** Copy constructor */
Message::Message(const Message &other) : Extends<Message,Object>(other)
{}

/** Destructor */
Message::~Message()
{}

/** Copy assignment */
Message& Message::operator=(const Message &other)
{
    super::operator=(other);
    return *this;
}

/** Comparison operator */
bool Message::operator==(const Message &other) const
{
    return super::operator==(other);
}

/** Comparison operator */
bool Message::operator!=(const Message &other) const
{
    return super::operator!=(other);
}

QString Message::typeName()
{
    return "SireCluster::network::Message";
}

//////
////// Implementation of Reply
//////

static const RegisterObject<Reply> r_reply;

/** Null constructor */
Reply::Reply() : Implements<Reply,Message>(), msgid(0), is_error(false)
{}

/** Construct to send an empty reply back in response to the 
    message with ID 'msid' - this can be used to send back
    a void (e.g. OK, finished) message */
Reply::Reply(quint64 mid) : Implements<Reply,Message>(),
         sender_uid( Communicator::getLocalInfo().UID() ),
         msgid(mid), is_error(false)
{}

/** Construct to reply to the message with ID 'msgid', with contents
    'contents */
Reply::Reply(quint64 mid, const QByteArray &conts)
      : Implements<Reply,Message>(), reply_contents(conts), 
        sender_uid( Communicator::getLocalInfo().UID() ), 
        msgid(mid), is_error(false)
{}

/** Construct to send an error in response to the message with ID 'msgid' */
Reply::Reply(quint64 mid, const Siren::exception &error)
      : Implements<Reply,Message>(), 
        sender_uid( Communicator::getLocalInfo().UID() ), 
        msgid(mid), is_error(true)
{
    DataStream ds( &reply_contents, QIODevice::WriteOnly );
    ds << error;
}
        
/** Copy constructor */
Reply::Reply(const Reply &other)
      : Implements<Reply,Message>(other), 
        reply_contents(other.reply_contents),
        sender_uid(other.sender_uid),
        msgid(other.msgid), is_error(other.is_error)
{}

/** Destructor */
Reply::~Reply()
{}

/** Copy assignment operator */
Reply& Reply::operator=(const Reply &other)
{
    if (this != &other)
    {
        reply_contents = other.reply_contents;
        sender_uid = other.sender_uid;
        msgid = other.msgid;
        is_error = other.is_error;
        super::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Reply::operator==(const Reply &other) const
{
    return reply_contents == other.reply_contents and 
           sender_uid == other.sender_uid and msgid == other.msgid and
           is_error == other.is_error and super::operator==(other);
}

/** Comparison operator */
bool Reply::operator!=(const Reply &other) const
{
    return not Reply::operator==(other);
}

uint Reply::hashCode() const
{
    return qHash( Reply::typeName() ) + msgid + qHash(is_error);
}

QString Reply::toString() const
{
    if (is_error)
        return QObject::tr("Reply( %1, ERROR )").arg(msgid);
    
    else if (msgid == 0)
        return QObject::tr("Reply::null");
    
    else if (reply_contents.isEmpty())
        return QObject::tr("Reply( %1, empty )").arg(msgid);
        
    else
        return QObject::tr("Reply( %1, contents size %2 bytes )")
                        .arg(msgid).arg(reply_contents.count());
}

void Reply::stream(Siren::Stream &s)
{
    s.assertVersion<Reply>(1);
    
    Schema schema = s.item<Reply>();
    
    schema.data("sender") & sender_uid;
    schema.data("message_id") & msgid;
    schema.data("contents") & reply_contents;
    schema.data("is_error") & is_error;
    
    super::stream(schema.base());
}

/** Read this message - this posts a copy of this reply in the 
    hash of replies in the Communicator */
void Reply::read(const QUuid &sender, quint64) const
{
    if (sender != sender_uid)
        throw SireCluster::network_error( QObject::tr(
                "Something is weird with this reply (%1). The reply should have "
                "been sent by node %2, but has actually been sent by the "
                "node %3. This is either a program bug or an attempt to "
                "spoof the reply.")
                    .arg(this->toString())
                    .arg(sender_uid.toString(), sender.toString()), CODELOC );

    Communicator::deliverReply(msgid, *this);
}

/** Return the UID of the node that sent this reply */
const QUuid& Reply::sender() const
{
    return sender_uid;
}

/** Return whehter or not this is the null reply */
bool Reply::isNull() const
{
    return msgid == 0;
}

/** Return whether or not this message is an error */
bool Reply::isError() const
{
    return is_error;
}

/** Throw the error if this reply is an error */
void Reply::throwError() const
{
    if (is_error)
    {
        DataStream ds(reply_contents);
        
        ds.loadNextObject().asA<Siren::exception>().throwSelf();
    }
}

/** Return whether or not this reply is empty */
bool Reply::isEmpty() const
{
    return reply_contents.isEmpty();
}

/** Return the contents of this reply */
const QByteArray& Reply::contents() const
{
    return reply_contents;
}

//////
////// Implementation of Shutdown
//////

static const RegisterObject<Shutdown> r_shutdown;

/** Constructor */
Shutdown::Shutdown() : Implements<Shutdown,Message>()
{}

/** Copy constructor */
Shutdown::Shutdown(const Shutdown &other) : Implements<Shutdown,Message>(other)
{}

/** Destructor */
Shutdown::~Shutdown()
{}

/** Copy assignment operator */
Shutdown& Shutdown::operator=(const Shutdown &other)
{
    super::operator=(other);
    return *this;
}

/** Comparison operator */
bool Shutdown::operator==(const Shutdown &other) const
{
    return super::operator==(other);
}

/** Comparison operator */
bool Shutdown::operator!=(const Shutdown &other) const
{
    return super::operator!=(other);
}

uint Shutdown::hashCode() const
{
    return qHash(Shutdown::typeName());
}

QString Shutdown::toString() const
{
    return "Shutdown()";
}

void Shutdown::stream(Siren::Stream &s)
{
    s.assertVersion<Shutdown>(1);
    
    Schema schema = s.item<Shutdown>();
    
    super::stream(schema.base());
}

/** This just calls Cluster::shutdown() */
void Shutdown::read(const QUuid &sender, quint64 msg_id) const
{
    Cluster::shutdown();
}
