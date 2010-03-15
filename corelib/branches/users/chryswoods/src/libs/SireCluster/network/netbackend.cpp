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

#include <QHash>
#include <QPair>

#include "netbackend.h"

#include "SireCluster/workpacket.h"

#include "Siren/mutex.h"
#include "Siren/errors.h"

using namespace SireCluster;
using namespace SireCluster::resources;
using namespace SireCluster::network;
using namespace Siren;

Q_GLOBAL_STATIC( Mutex, backendMutex );

static QHash< QUuid,QPair<QUuid,ActiveBackend> > *net_backends(0);

void NetBackend::init()
{
    MutexLocker lkr( backendMutex() );
    
    if (not net_backends)
        net_backends = new QHash< QUuid,QPair<QUuid,ActiveBackend> >();
}

void NetBackend::end()
{
    MutexLocker lkr( backendMutex() );
    
    delete net_backends;
    net_backends = 0;
}

/** Register the passed backends, returning their net keys
    (in the same order as the backends). The backends are registered
    to be connected to from the node with UID 'sender' - this is to
    prevent any other node from trying to control the backend
    (as the encryption layer should prevent nodes from spoofing
    their sender UID) */
QList<QUuid> NetBackend::registerBackends(const QUuid &sender, 
                                          const QList<ActiveBackend> &backends)
{
    MutexLocker lkr( backendMutex() );
    
    QList<QUuid> netkeys;
    
    if (net_backends)
    {
        for (QList<ActiveBackend>::const_iterator it = backends.constBegin();
             it != backends.constEnd();
             ++it)
        {
            QUuid netkey = QUuid::createUuid();
            net_backends->insert( netkey, QPair<QUuid,ActiveBackend>(sender,*it) );
        
            netkeys.append(netkey);
        }
    }
    
    return netkeys;
}

static ActiveBackend getBackend(const QUuid &sender, const QUuid &netkey)
{
    MutexLocker lkr( backendMutex() );
    
    if (not net_backends)
        throw Siren::program_bug( QObject::tr(
                "Why are we calling 'getBackend' before NetBackend::init "
                "has been called (or after NetBackend::end has been called?)"),
                    CODELOC );
    
    QPair<QUuid,ActiveBackend> backend = net_backends->value(netkey);
    
    if (backend.first.isNull())
        throw Siren::unavailable_resource( QObject::tr(
                "There is no ActiveBackend with netkey %1 on this node.")
                    .arg(netkey.toString()), CODELOC );
    
    else if (backend.first != sender)
        throw Siren::unavailable_resource( QObject::tr(
                "The ActiveBackend with netkey %1 is unavailable to the "
                "node with UID %2 (it is only available to node %3).")
                    .arg(netkey.toString(), sender.toString(),
                         backend.first.toString()), CODELOC );

    return backend.second;
}

/** Call 'toString' on the backend with key 'netkey' */
QString NetBackend::toString(const QUuid &sender, const QUuid &netkey)
{
    return ::getBackend(sender, netkey).toString();
}

/** Call 'UID' on the backend with key 'netkey' */
QUuid NetBackend::UID(const QUuid &sender, const QUuid &netkey)
{
    return ::getBackend(sender, netkey).UID();
}

/** Call 'description' on the backend with key 'netkey' */
QString NetBackend::description(const QUuid &sender, const QUuid &netkey)
{
    return ::getBackend(sender, netkey).description();
}

/** Call 'startJob' on the backend with key 'netkey' */
void NetBackend::startJob(const QUuid &sender, const QUuid &netkey, 
                          const WorkPacket &workpacket)
{
    ::getBackend(sender, netkey).startJob(workpacket);
}

/** Call 'stopJob' on the backend with key 'netkey' */
void NetBackend::stopJob(const QUuid &sender, const QUuid &netkey)
{
    ::getBackend(sender, netkey).stopJob();
}

/** Call 'abortJob' on the backend with key 'netkey' */
void NetBackend::abortJob(const QUuid &sender, const QUuid &netkey)
{
    ::getBackend(sender, netkey).abortJob();
}

/** Call 'wait' on the backend with key 'netkey' */
void NetBackend::wait(const QUuid &sender, const QUuid &netkey)
{
    ::getBackend(sender, netkey).wait();
}

/** Call 'wait' on the backend with key 'netkey' */
bool NetBackend::wait(const QUuid &sender, const QUuid &netkey, int timeout)
{
    return ::getBackend(sender, netkey).wait(timeout);
}

/** Call 'progress' on the backend with key 'netkey' */
float NetBackend::progress(const QUuid &sender, const QUuid &netkey)
{
    return ::getBackend(sender, netkey).progress();
}

/** Call 'interimResult' on the backend with key 'netkey' */
WorkPacketPtr NetBackend::interimResult(const QUuid &sender, const QUuid &netkey)
{
    return ::getBackend(sender, netkey).interimResult();
}

/** Call 'result' on the backend with key 'netkey' */
WorkPacketPtr NetBackend::result(const QUuid &sender, const QUuid &netkey)
{
    return ::getBackend(sender, netkey).result();
}

/** Disconnect the backend with key 'netkey' */
void NetBackend::disconnect(const QUuid &sender, const QUuid &netkey)
{
    MutexLocker lkr( backendMutex() );
    
    if (not net_backends)
        return;
        
    if (net_backends->contains(netkey))
    {
        if (net_backends->value(netkey).first == sender)
            net_backends->remove(netkey);
    }
}

/** Disconnect all of the backends that are connected to by the node 'sender' */
void NetBackend::disconnect(const QUuid &sender)
{
    MutexLocker lkr( backendMutex() );
    
    if (not net_backends)
        return;
        
    QMutableHashIterator< QUuid,QPair<QUuid,ActiveBackend> > it( *net_backends );
    
    while (it.hasNext())
    {
        it.next();
        
        if (it.value().first == sender)
            it.remove();
    }
}
