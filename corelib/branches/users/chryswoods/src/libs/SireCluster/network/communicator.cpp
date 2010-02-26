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

#include <QReadWriteLock>

#include "communicator.h"
#include "hostinfo.h"
#include "envelope.h"
#include "message.h"

#include "SireCluster/cluster.h"
#include "SireCluster/errors.h"

#include "Siren/mutex.h"
#include "Siren/errors.h"

#include "SireSec/publickey.h"
#include "SireSec/privatekey.h"
#include "SireSec/signedpubprilock.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::network;
using namespace SireSec;
using namespace Siren;

class Route
{
public:
    Route() : nhops(0)
    {}
    
    Route(const boost::function<void (const QByteArray&)> &func)
            : send_function(func), nhops(0)
    {}
    
    Route(const boost::function<void (const QByteArray&)> &func,
          const QUuid &r, int n)
            : send_function(func), router(r), nhops(n)
    {}
    
    ~Route()
    {}
    
    boost::function<void (const QByteArray&)> send_function;
    QUuid router;
    int nhops;
};

class CommunicatorData
{
public:
    CommunicatorData()
    {}
    
    ~CommunicatorData()
    {}
    
    /** HostInfo for this process */
    HostInfo localhost;
    
    /** The private key used to decrypt messages
        sent to this communicator */
    PrivateKey encrypt_privkey;
    
    /** The private key used to sign messages
        sent by this communicator */
    PrivateKey sign_privkey;
    
    QReadWriteLock resolver_lock;
    
    /** Information about all of the hosts that can 
        be communicated with by this communicator */
    QHash<QUuid,HostInfo> resolver;
    
    /** All of the routes available to all of the hosts
        that can be communicated with by this communicator */
    QMultiHash<QUuid,Route> routes;
};

static CommunicatorData *global_d(0);

Q_GLOBAL_STATIC( Mutex, commMutex );

static CommunicatorData* data()
{
    MutexLocker lkr( commMutex() );
    
    if (not global_d)
    {
        global_d = new CommunicatorData();

        //get the hostname
        QString hostname = Cluster::hostName();

        //create the keys for signing and encryption
        boost::tuple<PublicKey,PrivateKey> encrypt_keys
                    = PrivateKey::generate(QString("%1:encrypt").arg(hostname), 
                                           KeyTypes::RSA, 4096);
                                           
        boost::tuple<PublicKey,PrivateKey> sign_keys
                    = PrivateKey::generate(QString("%1:sign").arg(hostname),
                                           KeyTypes::RSA, 4096);
                                           
        //create the hostinfo object
        HostInfo hostinfo(QUuid::createUuid(), hostname, 
                          encrypt_keys.get<0>(),
                          sign_keys.get<0>());
                          
        global_d->localhost = hostinfo;
        global_d->encrypt_privkey = encrypt_keys.get<1>();
        global_d->sign_privkey = sign_keys.get<1>();
    }
    
    return global_d;
}

/** Get the HostInfo object for this process */
const HostInfo& Communicator::getLocalInfo()
{
    return data()->localhost;
}

static HostInfo resolveHost(const QUuid &uid)
{
    CommunicatorData *d = data();
    
    QReadLocker lkr( &(d->resolver_lock) );
    return d->resolver.value(uid);
}

/** Return the HostInfo object for the process with UID 'uid' */
HostInfo Communicator::getHostInfo(const QUuid &uid)
{
    CommunicatorData *d = data();
    
    QReadLocker lkr( &(d->resolver_lock) );
    
    HostInfo hostinfo = d->resolver.value(uid);
    
    if (hostinfo.isNull())
        throw Siren::unavailable_resource( QObject::tr(
                "There is no known process with UID '%1' that can be "
                "seen from the process %2.")
                    .arg(uid.toString(), d->localhost.toString()), CODELOC );
                    
    return hostinfo;
}

/** Add a neighbour process to this process, which can be communicated
    with by calling 'send_function' */
void Communicator::addNeighbour(const HostInfo &hostinfo,
                             boost::function<void(const QByteArray&)> send_function)
{
    if (hostinfo.isNull() or send_function.empty())
        return;

    CommunicatorData *d = data();
    
    QWriteLocker lkr( &(d->resolver_lock) );
    
    d->resolver.insert(hostinfo.UID(), hostinfo);

    d->routes.insert(hostinfo.UID(), Route(send_function));
}

/** Add a neighbour or a neighbour process, which can be communicated
    with by calling 'send_function' (which will route via 'router',
    requiring 'njumps' jumps to reach the process) */
void Communicator::addNeighbour(const HostInfo &hostinfo,
                             const QUuid &router,
                             boost::function<void(const QByteArray&)> send_function,
                             int njumps)
{
    if (hostinfo.isNull() or send_function.empty())
        return;

    CommunicatorData *d = data();
    
    QWriteLocker lkr( &(d->resolver_lock) );
    
    d->resolver.insert(hostinfo.UID(), hostinfo);

    d->routes.insert(hostinfo.UID(), Route(send_function, router, njumps));
}

static QByteArray pack(const Message &message, 
                       const QUuid &sender, const QUuid &recipient,
                       const PublicKey &encrypt_key,
                       const PrivateKey &sign_key)
{
    QByteArray message_data;
    {
        DataStream ds(&message_data, QIODevice::WriteOnly);
        ds << message;
    }
    
    SignedPubPriLock lock(encrypt_key, sign_key);
    message_data = lock.encrypt(message_data);
    
    QByteArray enveloped_message;
    {
        Envelope envelope(sender, recipient, message_data);
    
        DataStream ds(&enveloped_message, QIODevice::WriteOnly);
        ds << envelope;
    }
    
    return enveloped_message;
}

void Communicator::send(const Message &message, const QUuid &recipient)
{
    CommunicatorData *d = data();

    if ( d->localhost.UID() == recipient )
    {
        Communicator::received(message);
    }
    else
    {
        HostInfo hostinfo = resolveHost(recipient);
    
        if (hostinfo.isNull())
            throw SireCluster::network_error( QObject::tr(
                    "Do not know how to route a message from %1 to %2.")
                        .arg(d->localhost.toString())
                        .arg(recipient.toString()), CODELOC );

        Route route = d->routes.value(recipient);
        
        if (route.send_function.empty())
            throw SireCluster::network_error( QObject::tr(
                    "Cannot route the message %1 to the process with UID %2 "
                    "despite this recipient being in the resolver list...")
                        .arg(global_d->localhost.toString())
                        .arg(recipient.toString()), CODELOC );

        route.send_function( ::pack(message, d->localhost.UID(),
                                    recipient, hostinfo.encryptKey(),
                                    d->sign_privkey) );
    }
}

void Communicator::received(const Message &message)
{
    qDebug() << "Received a message!";
}

void Communicator::received(const Envelope &envelope)
{
    qDebug() << "Received an envelope from" << envelope.sender().toString();
}

void Communicator::received(const QByteArray &data)
{
    DataStream ds(data);
    
    Envelope envelope;
    ds >> envelope;
    
    Communicator::received(envelope);
}
