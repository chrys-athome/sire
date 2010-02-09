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

#ifndef SIRECLUSTER_NETWORK_COMMUNICATOR_H
#define SIRECLUSTER_NETWORK_COMMUNICATOR_H

#include <QUuid>

#include <boost/noncopyable.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{
namespace network
{

class Message;

/** This is the virtual base class of all Communicator
    objects. A Communicator provides the ability to 
    send "Message" objects to another process. Each
    communicator is either a parent or a child;
    
    (1) If it is a child, then it can only send messages
        to the parent. If it needs to send messages to 
        anyone else, then it sends those messages up to
        the parent who will re-route them to their correct
        destination
        
    (2) If it is a parent, then it can only send messages
        to any of its children. If it needs to send messages
        to anyone else, then it passes the message back
        to the Cluster, which will then see if this process
        is a Child in another communicator. If it is, then
        it will pass the message to the parent in the other
        communicator for it to forward on
        
    Note that a single process may contain many communicators,
    so may be both a parent and a child. To identify processes,
    each process has a unique UID. To route a message, a process
    will select the communicator that has the lowest number of
    jumps to the process that matches that UID.
    
    Broadcasts are possible from parent down to child, e.g.
    a parent can broadcast a message to all children, or a child
    can forward a message to the parent that is then broadcast
    to all children
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Communicator : public boost::noncopyable
{
public:
    Communicator();
    virtual ~Communicator();
    
    virtual void send(const Message &message, const QUuid &recipient)=0;
    virtual void send(const Message &message, const QList<QUuid> &recipients)=0;
    
    virtual void broadcast(const Message &message)=0;
};

} // end of namespace network
} // end of namespace SireCluster

SIRE_END_HEADER

#endif
