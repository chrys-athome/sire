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

#ifndef SIRECLUSTER_NETWORK_NETFRONTEND_H
#define SIRECLUSTER_NETWORK_NETFRONTEND_H

#include "SireCluster/resources/frontend.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{
namespace network
{

using SireCluster::resources::Frontend;

/** This is a Frontend that can communicate with the 
    remote NetBackend backend 
    
    @author Christopher Woods
*/
class NetFrontend : public Frontend
{
public:
    NetFrontend(const QUuid &host, const QUuid &netkey);
    ~NetFrontend();
    
    bool isLocal() const;
    
    void startJob(const WorkPacket &workpacket);
    
    void stopJob();
    void abortJob();

    void wait();
    bool wait(int timeout);
    
    float progress();
    WorkPacketPtr interimResult();
    
    WorkPacketPtr result();

private:
    /** The UID of the node that holds the resource */
    QUuid host;

    /** The key used to identify the backend */
    QUuid netkey;
};

} // end of namespace network
} // end of namespace SireCluster


SIRE_END_HEADER

#endif
