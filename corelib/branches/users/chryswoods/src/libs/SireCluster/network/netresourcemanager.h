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

#ifndef SIRECLUSTER_NETWORK_NETRESOURCEMANAGER_H
#define SIRECLUSTER_NETWORK_NETRESOURCEMANAGER_H

#include <QList>
#include <QUuid>
#include <QMultiHash>
#include <QPair>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{

namespace resources{ class ActiveBackend; }

namespace network
{

/** This static class, not part of the public API, is used
    to reserve resources over the network */
class NetResourceManager
{
    static const int DEFAULT_TIMEOUT = 25000;

public:
    static void init();
    static void end();
    
    static QPair<QUuid,QUuid> reserveResource(int expires=DEFAULT_TIMEOUT);
    static QPair<QUuid,QUuid> reserveResource(const QString &description,
                                              int expires=DEFAULT_TIMEOUT);

    static QMultiHash<QUuid,QUuid> reserveResources(int n, int expires=DEFAULT_TIMEOUT);
    static QMultiHash<QUuid,QUuid> reserveResources(const QString &description,
                                                    int n, int expires=DEFAULT_TIMEOUT);

    static void receivedReservation(const QUuid &sender,
                                    QList<QUuid> reservation_uids,
                                    const QUuid &request_uid);

    static resources::ActiveBackend 
                collectReservation(const QPair<QUuid,QUuid> &uid);
                
    static QHash<QUuid,resources::ActiveBackend> 
                        collectReservation(const QMultiHash<QUuid,QUuid> &uids);
    
    static void releaseReservation(const QPair<QUuid,QUuid> &uid);
    static void releaseReservation(const QMultiHash<QUuid,QUuid> &uids);

};


} // end of namespace network
} // end of namespace SireCluster

SIRE_END_HEADER

#endif
