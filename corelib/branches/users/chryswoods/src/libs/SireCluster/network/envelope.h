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

#ifndef SIRECLUSTER_NETWORK_ENVELOPE_H
#define SIRECLUSTER_NETWORK_ENVELOPE_H

#include <QUuid>
#include <QByteArray>
#include <QList>

#include "Siren/object.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{
namespace network
{

/** Messages are sent by putting them into envelopes,
    which hold information about where the message
    is sent from, how the message has been routed,
    and where the message should be sent to
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Envelope : public Siren::Implements<Envelope,Siren::Object>
{
public:
    Envelope();
    Envelope(const QUuid &sender, const QUuid &recipient,
             const QByteArray &message_data);

    Envelope(const Envelope &other);
    
    ~Envelope();
    
    Envelope& operator=(const Envelope &other);
    
    bool operator==(const Envelope &other) const;
    bool operator!=(const Envelope &other) const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    bool isNull() const;
    
    const QByteArray& message() const;
    
    const QUuid& sender() const;
    const QUuid& destination() const;
    
    bool isRecipient(const QUuid &uid) const;
    bool wasRoutedBy(const QUuid &uid) const;
    
    QList<QUuid> route() const;
    
    Envelope addRouter(const QUuid &uid) const;
    
private:
    /** Encrypted, signed, compressed, binary representation
        of the message */
    QByteArray message_data;

    /** The source of the message */
    QUuid sender_uid;
    
    /** The UIDs of processes through which this message
        has been routed (in the order they were routed) */
    QList<QUuid> routed_uids;
    
    /** The UIDs of the process that should receive this message */
    QUuid recipient_uid;
};

} // end of namespace network
} // end of namespace SireCluster

Q_DECLARE_METATYPE( SireCluster::network::Envelope )

SIRE_END_HEADER

#endif
