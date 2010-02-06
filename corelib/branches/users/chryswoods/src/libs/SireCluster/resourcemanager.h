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

#ifndef SIRECLUSTER_RESOURCEMANAGER_H
#define SIRECLUSTER_RESOURCEMANAGER_H

#include <QList>
#include <QHash>
#include <QUuid>
#include <QPair>
#include <QDateTime>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{

class ActiveBackend;
class DormantBackend;

/** This is a static class, and it is responsible
    for managing all of the Backend compute resources that are directly
    connected to this process. Each Backend registers with the 
    ResourceManager, and the ResourceManager is responsible for
    brokering access to each Backend resource in response to 
    requests from Cluster objects.
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT ResourceManager
{
    static const int DEFAULT_TIMEOUT = 25000;

public:
    static void registerResource(const DormantBackend &backend);
    static void unregisterResource(ActiveBackend &backend);

    static QUuid reserveResource(int expires=DEFAULT_TIMEOUT);
    static QUuid reserveResource(const QString &description,
                                 int expires=DEFAULT_TIMEOUT);

    static QList<QUuid> reserveResources(int n, int expires=DEFAULT_TIMEOUT);
    static QList<QUuid> reserveResources(const QString &description,
                                         int n, int expires=DEFAULT_TIMEOUT);
    
    static QUuid tryReserveResource(int ms, int expires=DEFAULT_TIMEOUT);
    static QUuid tryReserveResource(int ms,
                                    const QString &description,
                                    int expires=DEFAULT_TIMEOUT);

    static QList<QUuid> tryReserveResources(int n, int ms, int expires=DEFAULT_TIMEOUT);
    static QList<QUuid> tryReserveResources(const QString &description,
                                            int n, int ms, int expires=DEFAULT_TIMEOUT);
    
    static ActiveBackend collectReservation(const QUuid &uid);
    static QHash<QUuid,ActiveBackend> collectReservation(const QList<QUuid> &uids);
    
    static void releaseReservation(const QUuid &uid);
    static void releaseReservation(const QList<QUuid> &uids);
};

}

SIRE_END_HEADER

#endif
