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

#ifndef SIRECLUSTER_NETWORK_RECEIVEQUEUE_H
#define SIRECLUSTER_NETWORK_RECEIVEQUEUE_H

#include "Siren/handle.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{
namespace network
{

class Envelope;

namespace detail
{
class ReceiveQueueData;
}

/** This is a queue onto which received messages can
    be pushed, and which provides a thread for the 
    reading and processing of these messages
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT ReceiveQueue
: public Siren::ImplementsHandle< ReceiveQueue,Siren::Handles<detail::ReceiveQueueData> >
{
public:
    ReceiveQueue();
    ReceiveQueue(const ReceiveQueue &other);
    
    ~ReceiveQueue();
    
    ReceiveQueue& operator=(const ReceiveQueue &other);
    
    bool operator==(const ReceiveQueue &other) const;
    bool operator!=(const ReceiveQueue &other) const;
    
    QString toString() const;
    uint hashCode() const;
    
    void push(const Envelope &message);
    
    void start();
    void stop();
};

} // end of namespace network
} // end of namespace SireCluster

Q_DECLARE_METATYPE( SireCluster::network::ReceiveQueue )

SIRE_END_HEADER

#endif
