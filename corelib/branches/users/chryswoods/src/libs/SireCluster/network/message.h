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

#include "Siren/object.h"
#include "Siren/objptr.hpp"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

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
class SIRECLUSTER_EXPORT Message : public Siren::Object
{
public:
    Message();
    Message(const Message &other);
    
    ~Message();
    
    static QString typeName();
    
    virtual void read() const=0;
    
protected:
    Message& operator=(const Message &other);
    bool operator==(const Message &other) const;
    bool operator!=(const Message &other) const;
    
};

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
    Envelope(const Envelope &other);
    
    ~Envelope();
    
    Envelope& operator=(const Envelope &other);
    
    bool operator==(const Envelope &other) const;
    bool operator!=(const Envelope &other) const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);
    
    static Envelope send(const Message &message, const QUuid &destination);
    static Envelope send(const Message &message, const QSet<QUuid> &destinations);

    static Envelope broadcast(const Message &message);
    
    static Envelope routedVia(const Envelope &message, const QUuid &router);
    
private:
    /** The message being sent */
    Siren::ObjPtr<Message> msg;
    
    /** The source of the message */
    QUuid sender_uid;
    
    /** The UIDs of processes through which this message
        has been routed */
    QSet<QUuid> routed_uids;
    
    /** The UIDs of processes to which this message should
        be sent */
    QSet<QUuid> destination_uids;
};

} // end of namespace network
} // end of namespace SireCluster

SIRE_END_HEADER

#endif
