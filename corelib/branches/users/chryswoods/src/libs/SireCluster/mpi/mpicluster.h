/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIRECLUSTER_MPI_MPICLUSTER_H
#define SIRECLUSTER_MPI_MPICLUSTER_H

#ifdef SIRE_USE_MPI

#include "sireglobal.h"

#include <QUuid>
#include <QList>

SIRE_BEGIN_HEADER

namespace SireCluster
{

class DormantFrontend;

namespace MPI
{

class Message;
class Reply;
class P2PComm;

/** This class provides the global interface to all of the
    MPI nodes in the cluster (and, on the root node, the 
    global registry of all nodes available via MPI)
    
    This is a private class which is only available internally
    to SireCluster if MPI is available

    @author Christopher Woods
*/
class MPICluster
{
public:
    static QUuid processUID();
    
    static int getRankFor(const QUuid &uid);
    static QSet<int> getRanksFor(const QSet<QUuid> &uids);

    static void start();
    static void shutdown();

    static bool isRunning();
    
    static DormantFrontend getResource();
    static DormantFrontend getResource(int timeout);

    static DormantFrontend getResource(const QString &description);
    static DormantFrontend getResource(const QString &description, int timeout);
    
    static QList<DormantFrontend> getResources(int n);
    static QList<DormantFrontend> getResources(int n, int timeout);
    
    static QList<DormantFrontend> getResources(const QString &description, int n);
    static QList<DormantFrontend> getResources(const QString &description, 
                                               int n, int timeout);

    static bool isMaster();

    static void send(const Message &message, const QUuid &destination);
    static void send(const Message &message, const QSet<QUuid> &destinations);
    static void broadcast(const Message &message);
    
    static void received(const Message &message);

    static P2PComm createP2P(int master_rank, int slave_rank);

    static void informedShutdown();

    static Reply getReply(const Message &message);
    
    static void postResult(const QUuid &subject_uid, int sender,
                           const QByteArray &result_data);
                           
    static void postError(const QUuid &subject_uid, int sender,
                          const QByteArray &message_data,
                          const QByteArray &error_data);
};

} // end of namespace MPI

} // end of namespace SireCluster

SIRE_END_HEADER

#endif // SIRE_USE_MPI

#endif
