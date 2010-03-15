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

#ifndef SIRECLUSTER_NETWORK_NETBACKEND_H
#define SIRECLUSTER_NETWORK_NETBACKEND_H

#include "SireCluster/resources/backend.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{
namespace network
{

using SireCluster::resources::ActiveBackend;

/** This static class is used to pass on network requests
    for a resource to the local backend
    
    @author Christopher Woods
*/
class NetBackend
{
public:    
    static void init();
    static void end();
    
    static QList<QUuid> registerBackends(const QUuid &sender, 
                                         const QList<ActiveBackend> &backends);

    static QString toString(const QUuid &sender, const QUuid &netkey);
    
    static QUuid UID(const QUuid &sender, const QUuid &netkey);
    static QString description(const QUuid &sender, const QUuid &netkey);
    
    static void startJob(const QUuid &sender, const QUuid &netkey, 
                         const WorkPacket &workpacket);
    
    static void stopJob(const QUuid &sender, const QUuid &netkey);
    static void abortJob(const QUuid &sender, const QUuid &netkey);
    
    static void wait(const QUuid &sender, const QUuid &netkey);
    static bool wait(const QUuid &sender, const QUuid &netkey, int timeout);
    
    static float progress(const QUuid &sender, const QUuid &netkey);
    static WorkPacketPtr interimResult(const QUuid &sender, const QUuid &netkey);
    
    static WorkPacketPtr result(const QUuid &sender, const QUuid &netkey);
    
    static void disconnect(const QUuid &sender, const QUuid &netkey);
    static void disconnect(const QUuid &sender);
};

} // end of namespace network
} // end of namespace SireCluster

SIRE_END_HEADER

#endif
