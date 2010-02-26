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

#include "Siren/streamqt.h"

#include "envelope.h"

#include "Siren/stream.h"
#include "Siren/tostring.h"

using namespace SireCluster;
using namespace SireCluster::network;
using namespace Siren;

static const RegisterObject<Envelope> r_envelope;

/** Null constructor */
Envelope::Envelope() : Implements<Envelope,Object>()
{}

/** Construct to envelope the message held in 'message_data',
    which is sent by 'sender', and should be recieved by 
    'recipient' */
Envelope::Envelope(const QUuid &sender, const QUuid &recipient,
                   const QByteArray &data)
         : Implements<Envelope,Object>(),
           message_data(data), sender_uid(sender),
           recipient_uid(recipient)
{
    if (sender.isNull() or recipient.isNull() or message_data.isEmpty())
    {
        this->operator=( Envelope() );
    }
}
                
/** Copy constructor */
Envelope::Envelope(const Envelope &other) 
         : Implements<Envelope,Object>(other),
           message_data(other.message_data),
           sender_uid(other.sender_uid),
           routed_uids(other.routed_uids),
           recipient_uid(other.recipient_uid)
{}

/** Destructor */
Envelope::~Envelope()
{}

/** Copy assignment operator */
Envelope& Envelope::operator=(const Envelope &other)
{
    if (this != &other)
    {
        message_data = other.message_data;
        sender_uid = other.sender_uid;
        routed_uids = other.routed_uids;
        recipient_uid = other.recipient_uid;
        super::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Envelope::operator==(const Envelope &other) const
{
    return this == &other or
           (sender_uid == other.sender_uid and
            recipient_uid == other.recipient_uid and
            routed_uids == other.routed_uids and
            message_data == other.message_data and
            super::operator==(other));
}

/** Comparison operator */
bool Envelope::operator!=(const Envelope &other) const
{
    return not Envelope::operator==(other);
}

QString Envelope::toString() const
{
    if (sender_uid.isNull())
        return QObject::tr("Envelope::null");
    else
    {
        if (routed_uids.isEmpty())
            return QObject::tr("Envelope( sender = %1, recipient = %2, "
                               "message size = %3 bytes )")
                            .arg(sender_uid.toString())
                            .arg(recipient_uid.toString())
                            .arg(message_data.count());
        else
            return QObject::tr("Envelope( sender = %1, recipient = %2, "
                               "routed via %3, message size = %4 bytes )")
                            .arg(sender_uid.toString())
                            .arg(recipient_uid.toString())
                            .arg( Siren::toString(routed_uids) )
                            .arg(message_data.count());
    }
}

uint Envelope::hashCode() const
{
    return qHash(Envelope::typeName()) + qHash(sender_uid) + qHash(recipient_uid);
}

void Envelope::stream(Siren::Stream &s)
{
    s.assertVersion<Envelope>(1);
    
    Schema schema = s.item<Envelope>();
    
    schema.data("sender_uid") & sender_uid;
    schema.data("recipient_uid") & recipient_uid;
    schema.data("routed_uids") & routed_uids;
    schema.data("message_data") & message_data;
    
    super::stream( schema.base() );
}

/** Return whether or not this is a null (empty) envelope */
bool Envelope::isNull() const
{
    return message_data.isEmpty();
}

/** Return the (encrypted, compressed, signed) message */
const QByteArray& Envelope::message() const
{
    return message_data;
}

/** Return the UID of the process that sent the message */
const QUuid& Envelope::sender() const
{
    return sender_uid;
}

/** Return the UID of the process that should receive this message */
const QUuid& Envelope::destination() const
{
    return recipient_uid;
}

/** Return whether or not the process with UID 'uid' is the intended
    recipient of this message */
bool Envelope::isRecipient(const QUuid &uid) const
{
    return uid == recipient_uid;
}

/** Return whether or not this message has been routed by the
    process with UID 'uid' */
bool Envelope::wasRoutedBy(const QUuid &uid) const
{
    return sender_uid == uid or routed_uids.contains(uid);
}

/** Return the list of UIDs along which this message has been routed
    (note that this does not include the sender and recipient) */
QList<QUuid> Envelope::route() const
{
    return routed_uids;
}

/** Return the envelope that has the additional information that the
    message has been routed via the process with UID 'uid' */
Envelope Envelope::addRouter(const QUuid &uid) const
{
    Envelope ret(*this);
    ret.routed_uids.append(uid);
    
    return ret;
}
