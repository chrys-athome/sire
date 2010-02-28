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
#include <QQueue>
#include <QPair>

#include "communicator.h"
#include "hostinfo.h"
#include "envelope.h"
#include "message.h"

#include "SireCluster/cluster.h"
#include "SireCluster/errors.h"

#include "Siren/mutex.h"
#include "Siren/errors.h"
#include "Siren/thread.h"
#include "Siren/forages.h"
#include "Siren/datastream.h"
#include "Siren/tostring.h"

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

Q_GLOBAL_STATIC_WITH_ARGS( Mutex, commMutex, (QMutex::Recursive) );

static CommunicatorData* data()
{
    MutexLocker lkr( commMutex() );
    
    if (not global_d)
    {
        global_d = new CommunicatorData();

        //create the keys for signing and encryption
        boost::tuple<PublicKey,PrivateKey> encrypt_keys
                    = PrivateKey::generate("encrypt");

        boost::tuple<PublicKey,PrivateKey> sign_keys
                    = PrivateKey::generate("sign");
                                           
        //create the hostinfo object
        HostInfo hostinfo(QUuid::createUuid(),
                          encrypt_keys.get<0>(),
                          sign_keys.get<0>());

        qDebug() << Siren::getProcessString();

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

static QPair<QByteArray,quint64> pack(const Message &message, 
                                      const QUuid &sender, const QUuid &recipient,
                                      const PublicKey &encrypt_key,
                                      const PrivateKey &sign_key,
                                      bool acknowledge_receipt)
{
    QByteArray message_data;
    {
        DataStream ds(&message_data, QIODevice::WriteOnly);
        ds << message;
    }
    
    SignedPubPriLock lock(encrypt_key, sign_key);
    message_data = lock.encrypt(message_data);
    
    QByteArray enveloped_message;
    quint64 msg_id;
    {
        Envelope envelope(sender, recipient, message_data, acknowledge_receipt);
        
        msg_id = envelope.messageID();
    
        DataStream ds(&enveloped_message, QIODevice::WriteOnly);
        ds << envelope;
    }
    
    return QPair<QByteArray,quint64>(enveloped_message, msg_id);
}

static ObjRef unpack(const QByteArray &data,
                     const PrivateKey &decrypt_key,
                     const PublicKey &sign_key)
{
    SignedPubPriLock lock(decrypt_key, sign_key);
    
    QByteArray message_data = lock.decrypt(data);
    
    DataStream ds(message_data);
    return ds.loadNextObject();
}

void Communicator::received(const Message &message)
{
    qDebug() << "Received a message!";
    message.read(QUuid(), 0);
}

void Communicator::reroute(const Envelope &envelope)
{
    CommunicatorData *d = data();

    HostInfo hostinfo = resolveHost(envelope.destination());

    if (hostinfo.isNull())
        throw SireCluster::network_error( QObject::tr(
                "Do not know how to re-route a message from %1 to %2.")
                    .arg(envelope.sender().toString())
                    .arg(envelope.destination().toString()), CODELOC );

    Route route = d->routes.value(envelope.destination());
    
    if (route.send_function.empty())
        throw SireCluster::network_error( QObject::tr(
                "Cannot route the message %1 to the process with UID %2 "
                "despite this recipient being in the resolver list...")
                    .arg(global_d->localhost.toString())
                    .arg(envelope.destination().toString()), CODELOC );

    qDebug() << "reroute" << envelope.toString() << "on" 
             << d->localhost.UID() << "from" << envelope.sender().toString()
             << "to" << envelope.destination().toString();

    QByteArray message_data;
    {
        DataStream ds(&message_data, QIODevice::WriteOnly);
        ds << envelope.addRouter(d->localhost.UID());
    }

    route.send_function(message_data);
}

quint64 Communicator::send(const Message &message, const QUuid &recipient,
                           bool acknowledge_receipt)
{
    CommunicatorData *d = data();

    if ( d->localhost.UID() == recipient )
    {
        Communicator::received(message);
        return 0;
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

        qDebug() << "send" << message.toString() << "from" 
                 << d->localhost.UID() << "to" << recipient;

        QPair<QByteArray,quint64> p = ::pack(message, d->localhost.UID(),
                                             recipient, hostinfo.encryptKey(),
                                             d->sign_privkey, acknowledge_receipt);

        route.send_function(p.first);

        return p.second;
    }
}

QHash<QUuid,quint64> Communicator::send(const Message &message, 
                                        const QList<QUuid> &recipients,
                                        bool acknowledge_receipt)
{
    CommunicatorData *d = data();
    
    {
        QReadLocker lkr( &(d->resolver_lock) );
        QList<QUuid> unknown_recipients;
        
        foreach (QUuid recipient, recipients)
        {
            if (not d->resolver.contains(recipient))
                unknown_recipients.append(recipient);
        }
        
        throw SireCluster::network_error( QObject::tr(
                "Cannot find a route to some of the recipients of the message. "
                "The message should be sent to %1, but routes for the following "
                "destinations cannot be found: %2.")
                    .arg(Siren::toString(recipients), 
                         Siren::toString(unknown_recipients)), CODELOC );
    }
    
    QHash<QUuid,quint64> ids;
    
    ids.reserve(recipients.count());
    
    foreach (QUuid recipient, recipients)
    {
        quint64 id = Communicator::send(message, recipient, acknowledge_receipt);
        ids.insert(recipient, id);
    }
    
    return ids;
}

QHash<QUuid,quint64> Communicator::broadcast(const Message &message,
                                             bool acknowledge_receipt)
{
    //send the message to all known processes
    CommunicatorData *d = data();
    
    QList<QUuid> uids;
    {
        QReadLocker lkr( &(d->resolver_lock) );
        uids = d->resolver.keys();
    }
    
    QHash<QUuid,quint64> ids;
    ids.reserve(uids.count());
    
    foreach (QUuid uid, uids)
    {
        quint64 id = Communicator::send(message, uid, acknowledge_receipt);
        ids.insert(uid, id);
    }
    
    return ids;
}

static void received(const Envelope &envelope)
{
    if (envelope.destination() != Communicator::getLocalInfo().UID())
    {
        qDebug() << Communicator::getLocalInfo().UID().toString()
                 << "must reroute message from" << envelope.sender().toString()
                 << "to" << envelope.destination().toString();
               
        Communicator::reroute(envelope);
                     
        return;
    }
    
    //get the public key for the sender
    CommunicatorData *d = data();

    PublicKey sign_key;
    {
        QReadLocker lkr( &(d->resolver_lock) );
        sign_key = d->resolver.value(envelope.sender()).signatureKey();
    }
    
    if (not sign_key.isValid())
        throw SireCluster::network_error( QObject::tr(
                "Process %1 does not have a signature verification key for the "
                "process with UID %2.")
                    .arg( d->localhost.UID().toString() )
                    .arg(envelope.sender().toString()), CODELOC );
                    
    ObjRef message = ::unpack(envelope.message(), d->encrypt_privkey, sign_key);
    
    //read and act on the message
    message.asA<Message>().read(envelope.sender(), envelope.messageID());
}

/** Thread-pool used to process received messages */
class ReceivePool
{
    class ReceiveThread : public Siren::Thread
    {
        Mutex *datamutex;
        WaitCondition *job_waiter;
        QQueue< QPair<Envelope,QByteArray> > *jobs;
    
    public:
        ReceiveThread(int n, Mutex *dm, WaitCondition *jw,
                      QQueue< QPair<Envelope,QByteArray> > *j)
                : Thread( QString("ReceiveThread-%1").arg(n) ),
                  datamutex(dm), job_waiter(jw), jobs(j)
        {
            this->start();
        }
        
        ~ReceiveThread()
        {}
        
    protected:
        void threadMain()
        {
            Thread::signalStarted();
            
            try
            {
                MutexLocker lkr(datamutex);

                while (for_ages())
                {
                    while (jobs->isEmpty())
                    {
                        job_waiter->wait(datamutex);
                    }
                    
                    QPair<Envelope,QByteArray> job = jobs->dequeue();
                    
                    lkr.unlock();
                    
                    Envelope envelope;
                    
                    if (job.second.isEmpty())
                    {
                        envelope = job.first;
                    }
                    else
                    {
                        DataStream ds(job.second);
                        ds >> envelope;
                    }

                    ::received(envelope);
                    
                    lkr.relock();
                }
            }
            catch(const Siren::interupted&)
            {}
        }
    };

public:
    ReceivePool()
    {
        for (int i=0; i<4; ++i)
        {
            threads.append( new ReceiveThread(i+1, &datamutex, &job_waiter, &jobs) );
        }
    }
    
    ~ReceivePool()
    {
        foreach (ReceiveThread *thread, threads)
        {
            if (thread->isRunning())
            {
                end_for_ages(thread);
                thread->wait();
            }
            
            delete thread;
        }
    }
    
    void receive(const Envelope &envelope)
    {
        MutexLocker lkr( &datamutex );
        jobs.enqueue( QPair<Envelope,QByteArray>(envelope,QByteArray()) );
        job_waiter.wakeOne();
    }
    
    void receive(const QByteArray &envelope)
    {
        MutexLocker lkr( &datamutex );
        jobs.enqueue( QPair<Envelope,QByteArray>(Envelope(),envelope) );
        job_waiter.wakeOne();
    }
    
private:
    Mutex datamutex;
    WaitCondition job_waiter;

    QQueue< QPair<Envelope,QByteArray> > jobs;
    
    QList<ReceiveThread*> threads;
};

Q_GLOBAL_STATIC( ReceivePool, receivePool );

void Communicator::received(const Envelope &envelope)
{
    receivePool()->receive(envelope);
}

void Communicator::received(const QByteArray &data)
{
    receivePool()->receive(data);
}

void Communicator::awaitAcknowledgement(const QHash<QUuid,quint64> &messages)
{
    //just sleep for a second for the moment...
    Siren::msleep(1000);
}
