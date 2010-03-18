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

#include <boost/function.hpp>
#include <boost/bind.hpp>

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
#include "Siren/xmlstream.h"
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
    
    Route(const boost::function<void (const QByteArray&, boost::function<void ()>)> &func)
            : send_function(func), nhops(0)
    {}
    
    Route(const boost::function<void (const QByteArray&, boost::function<void ()>)> &func,
          const QUuid &r, int n)
            : send_function(func), router(r), nhops(n)
    {}
    
    ~Route()
    {}
    
    boost::function<void (const QByteArray&, boost::function<void ()>)> send_function;
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

    /** The IDs of enveloped messages that have not been 
        acknowledged */
    QSet<quint64> unacknowledged_envelopes;
    
    /** The IDs of envelopes that have not yet been sent */
    QSet<quint64> unsent_envelopes;
    
    /** The set of replies that have been received by this communicator */
    QHash<quint64, Reply> received_replies;
    
    /** Mutex used to lock access to the list of unacknowledged messages */
    Mutex acknowledge_mutex;
    
    /** Wait condition used to wait until messages have been acknowledged */
    WaitCondition acknowledge_waiter;
    
    /** Mutex used to lock access to the hash of replies */
    Mutex reply_mutex;
    
    /** Wait condition used to wait until a reply has been received */
    WaitCondition reply_waiter;
};

namespace SireCluster
{
    namespace network
    {

        /** Private message used by CommunicatorData to acknowledge message receipt */
        class AcknowledgeReceipt : public Implements<AcknowledgeReceipt,Message>
        {
        public:
            AcknowledgeReceipt();
            AcknowledgeReceipt(quint64 message_id);
    
            AcknowledgeReceipt(const AcknowledgeReceipt &other);
    
            ~AcknowledgeReceipt();
    
            AcknowledgeReceipt& operator=(const AcknowledgeReceipt &other);
    
            bool operator==(const AcknowledgeReceipt &other) const;
            bool operator!=(const AcknowledgeReceipt &other) const;
    
            QString toString() const;
            uint hashCode() const;
            void stream(Stream &s);
    
            void read(const QUuid &sender, quint64) const;

        private:
            /** The ID of the message to acknowledge */
            quint64 acknowledge_message_id;
        };
    }
}

Q_DECLARE_METATYPE( SireCluster::network::AcknowledgeReceipt )

/////////
///////// Implementation of CommunicatorData
/////////

Q_GLOBAL_STATIC_WITH_ARGS( Mutex, commMutex, (QMutex::Recursive) );

static boost::shared_ptr<CommunicatorData> global_d;

static boost::shared_ptr<CommunicatorData> data()
{    
    MutexLocker lkr( commMutex() );
    boost::shared_ptr<CommunicatorData> d( global_d );

    lkr.unlock();
    
    if (d.get() == 0)
        throw Siren::program_bug( QObject::tr(
                "You cannot use Communicator before init() has been called, "
                "or after end() has been called!"), CODELOC );
    
    return d;
}

/////////
///////// Implementation of AcknowledgeReceipt
/////////

static const RegisterObject<AcknowledgeReceipt> r_ackrec;

AcknowledgeReceipt::AcknowledgeReceipt()
                   : Implements<AcknowledgeReceipt,Message>(),
                     acknowledge_message_id(0)
{}

AcknowledgeReceipt::AcknowledgeReceipt(quint64 message_id)
                   : Implements<AcknowledgeReceipt,Message>(),
                     acknowledge_message_id(message_id)
{}

AcknowledgeReceipt::AcknowledgeReceipt(const AcknowledgeReceipt &other)
                   : Implements<AcknowledgeReceipt,Message>(other),
                     acknowledge_message_id(other.acknowledge_message_id)
{}

AcknowledgeReceipt::~AcknowledgeReceipt()
{}

AcknowledgeReceipt& AcknowledgeReceipt::operator=(const AcknowledgeReceipt &other)
{
    acknowledge_message_id = other.acknowledge_message_id;
    super::operator=(other);
    return *this;
}

bool AcknowledgeReceipt::operator==(const AcknowledgeReceipt &other) const
{
    return acknowledge_message_id == other.acknowledge_message_id and
           super::operator==(other);
}

bool AcknowledgeReceipt::operator!=(const AcknowledgeReceipt &other) const
{
    return not AcknowledgeReceipt::operator==(other);
}

QString AcknowledgeReceipt::toString() const
{
    return QString("AcknowledgeReceipt( %1 )").arg(acknowledge_message_id);
}

uint AcknowledgeReceipt::hashCode() const
{
    return qHash( AcknowledgeReceipt::typeName() ) + qHash(acknowledge_message_id);
}

void AcknowledgeReceipt::stream(Stream &s)
{
    s.assertVersion<AcknowledgeReceipt>(1);
    
    Schema schema = s.item<AcknowledgeReceipt>();
    
    schema.data("acknowledge_id") & acknowledge_message_id;
    
    super::stream( schema.base() );
}

void AcknowledgeReceipt::read(const QUuid&, quint64) const
{
    if (acknowledge_message_id != 0)
    {
        boost::shared_ptr<CommunicatorData> d = data();
        
        MutexLocker lkr( &(d->acknowledge_mutex) );
        d->unacknowledged_envelopes.remove(acknowledge_message_id);
        
        //wait everyone who is waiting for message acknowledgement
        d->acknowledge_waiter.wakeAll();
    }
}

/////////
///////// Implementation of Communicator
/////////

void Communicator::init()
{
    MutexLocker lkr( commMutex() );

    if (global_d.get() == 0)
    {
        global_d.reset( new CommunicatorData() );

        //create the keys for signing and encryption
        boost::tuple<PublicKey,PrivateKey> encrypt_keys
                    = PrivateKey::generate("encrypt");

        boost::tuple<PublicKey,PrivateKey> sign_keys
                    = PrivateKey::generate("sign");
                                           
        //create the hostinfo object
        HostInfo hostinfo(QUuid::createUuid(),
                          encrypt_keys.get<0>(),
                          sign_keys.get<0>());

        global_d->localhost = hostinfo;
        global_d->encrypt_privkey = encrypt_keys.get<1>();
        global_d->sign_privkey = sign_keys.get<1>();
    }
}

void Communicator::end()
{
    MutexLocker lkr( commMutex() );
    global_d.reset();
}

/** Get the HostInfo object for this process */
const HostInfo& Communicator::getLocalInfo()
{
    return data()->localhost;
}

static HostInfo resolveHost(const QUuid &uid)
{
    boost::shared_ptr<CommunicatorData> d = data();
    
    QReadLocker lkr( &(d->resolver_lock) );
    return d->resolver.value(uid);
}

/** Return the HostInfo object for the process with UID 'uid' */
HostInfo Communicator::getHostInfo(const QUuid &uid)
{
    boost::shared_ptr<CommunicatorData> d = data();
    
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
        boost::function<void(const QByteArray&, boost::function<void ()>)> send_function)
{
    if (hostinfo.isNull() or send_function.empty())
        return;

    boost::shared_ptr<CommunicatorData> d = data();
    
    QWriteLocker lkr( &(d->resolver_lock) );
    
    d->resolver.insert(hostinfo.UID(), hostinfo);

    d->routes.insert(hostinfo.UID(), Route(send_function));
}

/** Add a neighbour or a neighbour process, which can be communicated
    with by calling 'send_function' (which will route via 'router',
    requiring 'njumps' jumps to reach the process) */
void Communicator::addNeighbour(const HostInfo &hostinfo,
        const QUuid &router,
        boost::function<void(const QByteArray&, boost::function<void ()>)> send_function,
        int njumps)
{
    if (hostinfo.isNull() or send_function.empty())
        return;

    boost::shared_ptr<CommunicatorData> d = data();
    
    QWriteLocker lkr( &(d->resolver_lock) );
    
    d->resolver.insert(hostinfo.UID(), hostinfo);

    d->routes.insert(hostinfo.UID(), Route(send_function, router, njumps));
}

static QPair<QByteArray,quint64> pack(const QByteArray &message_data, 
                                      const QUuid &sender, const QUuid &recipient,
                                      const PublicKey &encrypt_key,
                                      const PrivateKey &sign_key,
                                      bool acknowledge_receipt)
{
    SignedPubPriLock lock(encrypt_key, sign_key);
    QByteArray encrypted_data = lock.encrypt(message_data);
    
    QByteArray enveloped_message;
    quint64 msg_id;
    {
        Envelope envelope(sender, recipient, encrypted_data, acknowledge_receipt);
        
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
    message.read(QUuid(), 0);
}

static void no_need_to_acknowledge_send()
{}

void Communicator::reroute(const Envelope &envelope)
{
    boost::shared_ptr<CommunicatorData> d = data();

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

    route.send_function(message_data, no_need_to_acknowledge_send);
}

static void message_sent(quint64 message_id)
{
    if (message_id != 0)
    {
        boost::shared_ptr<CommunicatorData> d = data();
        
        MutexLocker lkr( &(d->acknowledge_mutex) );
        d->unsent_envelopes.remove(message_id);
        d->acknowledge_waiter.wakeAll();
    }
}

static quint64 send(const QByteArray &message_data, const QUuid &recipient,
                    bool acknowledge_receipt)
{
    boost::shared_ptr<CommunicatorData> d = data();

    HostInfo hostinfo = resolveHost(recipient);

    if (hostinfo.isNull())
        throw SireCluster::network_error( QObject::tr(
                "Do not know how to route a message from %1 to %2.")
                    .arg(d->localhost.toString())
                    .arg(recipient.toString()), CODELOC );

    Route route = d->routes.value(recipient);
    
    if (route.send_function.empty())
        throw SireCluster::network_error( QObject::tr(
                "Cannot route the message from %1 to the process with UID %2 "
                "despite this recipient being in the resolver list...")
                    .arg(global_d->localhost.toString())
                    .arg(recipient.toString()), CODELOC );

    QPair<QByteArray,quint64> p = ::pack(message_data, d->localhost.UID(),
                                         recipient, hostinfo.encryptKey(),
                                         d->sign_privkey, acknowledge_receipt);

    try
    {
        //record this message as unsent (and unacknowledged if necessary)
        {
            MutexLocker lkr( &(d->acknowledge_mutex) );
        
            if (acknowledge_receipt)
                d->unacknowledged_envelopes.insert(p.second);
                
            d->unsent_envelopes.insert(p.second);
        }
        
        route.send_function(p.first, boost::bind(message_sent, p.second));
    }
    catch(...)
    {
        MutexLocker lkr( &(d->acknowledge_mutex) );
        d->unacknowledged_envelopes.remove(p.second);
        throw;
    }

    return p.second;
}

quint64 Communicator::send(const Message &message, const QUuid &recipient,
                           bool acknowledge_receipt)
{
    boost::shared_ptr<CommunicatorData> d = data();

    if ( d->localhost.UID() == recipient )
    {
        Communicator::received(message);
        return 0;
    }
    else
    {
        QByteArray message_data;
        {
            DataStream ds( &message_data, QIODevice::WriteOnly );
            ds << message;
        }
        
        return ::send(message_data, recipient, acknowledge_receipt);
    }
}

QHash<QUuid,quint64> Communicator::send(const Message &message, 
                                        const QList<QUuid> &recipients,
                                        bool acknowledge_receipt)
{
    boost::shared_ptr<CommunicatorData> d = data();

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
    
    QByteArray message_data;
    {
        DataStream ds( &message_data, QIODevice::WriteOnly );
        ds << message;
    }

    bool message_this_process = false;
    
    foreach (QUuid recipient, recipients)
    {
        if ( recipient != d->localhost.UID() )
        {
            quint64 id = ::send(message_data, recipient, acknowledge_receipt);
            ids.insert(recipient, id);
        }
        else
        {
            message_this_process = true;
        }
    }
    
    if (message_this_process)
    {
        Communicator::received(message);
    }
    
    return ids;
}

QHash<QUuid,quint64> Communicator::broadcast(const Message &message,
                                             bool acknowledge_receipt)
{
    //send the message to all known processes
    boost::shared_ptr<CommunicatorData> d = data();
    
    QList<QUuid> uids;
    {
        QReadLocker lkr( &(d->resolver_lock) );
        uids = d->resolver.keys();
    }

    QHash<QUuid,quint64> ids;
    ids.reserve(uids.count());
    
    QByteArray message_data;
    {
        DataStream ds( &message_data, QIODevice::WriteOnly );
        ds << message;
    }
    
    foreach (QUuid uid, uids)
    {
        quint64 id = ::send(message_data, uid, acknowledge_receipt);
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
    boost::shared_ptr<CommunicatorData> d = data();

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

    //acknowledge receipt of the message (if that has been requested)
    if (envelope.mustAcknowledgeReceipt())
        Communicator::send( AcknowledgeReceipt(envelope.messageID()),
                            envelope.sender(), false );
    
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
        bool *exit_now;
    
    public:
        ReceiveThread(int n, Mutex *dm, WaitCondition *jw,
                      QQueue< QPair<Envelope,QByteArray> > *j,
                      bool *exit)
                : Thread( QString("ReceiveThread-%1").arg(n) ),
                  datamutex(dm), job_waiter(jw), jobs(j), exit_now(exit)
        {
            this->start();
        }
        
        ~ReceiveThread()
        {}
        
    protected:
        void threadMain()
        {
            try
            {
                Thread::signalStarted();
                
                MutexLocker lkr(datamutex);

                while (for_ages())
                {
                    try
                    {
                        while (jobs->isEmpty())
                        {
                            if (*exit_now)
                                return;
                        
                            job_waiter->wait(datamutex);
                        }
                        
                        if (*exit_now)
                            return;
                        
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
                    catch(const Siren::interupted&)
                    {
                        break;
                    }
                    catch(const Siren::exception &e)
                    {
                        qDebug() << CODELOC;
                        Siren::printError(e);
                    }
                    catch(const std::exception &e)
                    {
                        qDebug() << CODELOC;
                        Siren::printError(Siren::std_exception(e,CODELOC));
                    }
                    catch(...)
                    {
                        qDebug() << CODELOC;
                        Siren::printError(Siren::unknown_error(QObject::tr(
                                "Unknown error in ReceiveThread"), CODELOC ));
                    }
                }
            }
            catch(const Siren::interupted&)
            {
                return;
            }
        }
    };

public:
    ReceivePool() : exit_now(false)
    {
        for (int i=0; i<4; ++i)
        {
            threads.append( new ReceiveThread(i+1, &datamutex, &job_waiter, 
                                              &jobs, &exit_now) );
        }
    }
    
    ~ReceivePool()
    {
        exit_now = true;

        job_waiter.wakeAll();
        
        foreach (ReceiveThread *thread, threads)
        {
            if (thread->isRunning())
            {
                job_waiter.wakeAll();
                thread->wait();
            }

            delete thread;
        }
        
        threads.clear();
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
    
    bool exit_now;
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

bool Communicator::messageAcknowledged(quint64 message)
{
    boost::shared_ptr<CommunicatorData> d = data();
    
    MutexLocker lkr( &(d->acknowledge_mutex) );
    return not d->unacknowledged_envelopes.contains(message);
}

bool Communicator::allMessagesAcknowledged(const QHash<QUuid,quint64> &messages)
{
    boost::shared_ptr<CommunicatorData> d = data();
    
    MutexLocker lkr( &(d->acknowledge_mutex) );
    
    for (QHash<QUuid,quint64>::const_iterator it = messages.constBegin();
         it != messages.constEnd();
         ++it)
    {
        if (d->unacknowledged_envelopes.contains(it.value()))
            return false;
    }
    
    return true;
}

void Communicator::awaitAcknowledgement(quint64 message)
{
    boost::shared_ptr<CommunicatorData> d = data();

    MutexLocker lkr( &(d->acknowledge_mutex) );
    
    while (d->unacknowledged_envelopes.contains(message))
    {
        d->acknowledge_waiter.wait( &(d->acknowledge_mutex) );
    }
}

void Communicator::awaitAcknowledgement(const QHash<QUuid,quint64> &messages)
{
    boost::shared_ptr<CommunicatorData> d = data();

    MutexLocker lkr( &(d->acknowledge_mutex) );

    while (for_ages())
    {
        bool any_unacknowledged = false;
    
        for (QHash<QUuid,quint64>::const_iterator it = messages.constBegin();
             it != messages.constEnd();
             ++it)
        {
            if (d->unacknowledged_envelopes.contains(it.value()))
            {
                any_unacknowledged = true;
                break;
            }
        }

        if (any_unacknowledged)
            d->acknowledge_waiter.wait( &(d->acknowledge_mutex) );
        else
            return;
    }
}

bool Communicator::awaitAcknowledgement(quint64 message, int ms)
{
    if (ms < 0)
    {
        Communicator::awaitAcknowledgement(message);
        return true;
    }

    QTime t;
    t.start();

    boost::shared_ptr<CommunicatorData> d = data();

    MutexLocker lkr( &(d->acknowledge_mutex) );
    
    while (d->unacknowledged_envelopes.contains(message))
    {
        if (t.elapsed() > ms)
            return false;
    
        d->acknowledge_waiter.wait( &(d->acknowledge_mutex) );
    }
    
    return true;
}

bool Communicator::awaitAcknowledgement(const QHash<QUuid,quint64> &messages, int ms)
{
    if (ms < 0)
    {
        Communicator::awaitAcknowledgement(messages);
        return true;
    }

    QTime t;
    t.start();

    boost::shared_ptr<CommunicatorData> d = data();

    MutexLocker lkr( &(d->acknowledge_mutex) );

    while (for_ages())
    {
        bool any_unacknowledged = false;
    
        for (QHash<QUuid,quint64>::const_iterator it = messages.constBegin();
             it != messages.constEnd();
             ++it)
        {
            if (d->unacknowledged_envelopes.contains(it.value()))
            {
                any_unacknowledged = true;
                break;
            }
        }

        if (any_unacknowledged)
        {
            if (t.elapsed() > ms)
                return false;
            
            d->acknowledge_waiter.wait( &(d->acknowledge_mutex) );
        }
        else
            return true;
    }
    
    return true;
}

bool Communicator::messageSent(quint64 message)
{
    boost::shared_ptr<CommunicatorData> d = data();
    
    MutexLocker lkr( &(d->acknowledge_mutex) );
    return not d->unsent_envelopes.contains(message);
}

bool Communicator::allMessagesSent(const QHash<QUuid,quint64> &messages)
{
    boost::shared_ptr<CommunicatorData> d = data();
    
    MutexLocker lkr( &(d->acknowledge_mutex) );
    
    for (QHash<QUuid,quint64>::const_iterator it = messages.constBegin();
         it != messages.constEnd();
         ++it)
    {
        if (d->unsent_envelopes.contains(it.value()))
            return false;
    }
    
    return true;
}

void Communicator::awaitSent(quint64 message)
{
    boost::shared_ptr<CommunicatorData> d = data();

    MutexLocker lkr( &(d->acknowledge_mutex) );
    
    while (d->unsent_envelopes.contains(message))
    {
        d->acknowledge_waiter.wait( &(d->acknowledge_mutex) );
    }
}

void Communicator::awaitSent(const QHash<QUuid,quint64> &messages)
{
    boost::shared_ptr<CommunicatorData> d = data();

    MutexLocker lkr( &(d->acknowledge_mutex) );

    while (for_ages())
    {
        bool any_unsent = false;
    
        for (QHash<QUuid,quint64>::const_iterator it = messages.constBegin();
             it != messages.constEnd();
             ++it)
        {
            if (d->unsent_envelopes.contains(it.value()))
            {
                any_unsent = true;
                break;
            }
        }

        if (any_unsent)
            d->acknowledge_waiter.wait( &(d->acknowledge_mutex) );
        else
            return;
    }
}

bool Communicator::awaitSent(quint64 message, int ms)
{
    if (ms < 0)
    {
        Communicator::awaitSent(message);
        return true;
    }

    QTime t;
    t.start();

    boost::shared_ptr<CommunicatorData> d = data();

    MutexLocker lkr( &(d->acknowledge_mutex) );
    
    while (d->unsent_envelopes.contains(message))
    {
        if (t.elapsed() > ms)
            return false;
    
        d->acknowledge_waiter.wait( &(d->acknowledge_mutex) );
    }
    
    return true;
}

bool Communicator::awaitSent(const QHash<QUuid,quint64> &messages, int ms)
{
    if (ms < 0)
    {
        Communicator::awaitSent(messages);
        return true;
    }

    QTime t;
    t.start();

    boost::shared_ptr<CommunicatorData> d = data();

    MutexLocker lkr( &(d->acknowledge_mutex) );

    while (for_ages())
    {
        bool any_unsent = false;
    
        for (QHash<QUuid,quint64>::const_iterator it = messages.constBegin();
             it != messages.constEnd();
             ++it)
        {
            if (d->unsent_envelopes.contains(it.value()))
            {
                any_unsent = true;
                break;
            }
        }

        if (any_unsent)
        {
            if (t.elapsed() > ms)
                return false;
            
            d->acknowledge_waiter.wait( &(d->acknowledge_mutex) );
        }
        else
            return true;
    }
    
    return true;
}

/** This function is called to deliver a reply that has been received
    by in response to a message sent by this node */
void Communicator::deliverReply(quint64 message_id, const Reply &reply)
{
    boost::shared_ptr<CommunicatorData> d = data();

    MutexLocker lkr( &(d->reply_mutex) );
    
    d->received_replies.insert(message_id, reply);
    
    d->reply_waiter.wakeAll();
}

/** Wait until a reply to the message with ID 'message_id' has been 
    received, and then take that reply. Note that this will take the 
    reply, so will cause any other thread that is waiting for the 
    reply to wait forages - so don't wait for the same reply in two
    different threads! */
Reply Communicator::awaitReply(const QUuid &host, quint64 message_id)
{
    boost::shared_ptr<CommunicatorData> d = data();

    MutexLocker lkr( &(d->reply_mutex) );

    QHash<quint64,Reply>::const_iterator it = d->received_replies.constFind(message_id);
    
    while (it == d->received_replies.constEnd())
    {
        d->reply_waiter.wait( &(d->reply_mutex) );
        
        it = d->received_replies.constFind(message_id);
    }
    
    if (it.value().sender() != host)
        throw SireCluster::network_error( QObject::tr(
                "Strange - we are waiting for the reply to message %1 from "
                "host %2, but the received reply to this message is from "
                "host %3.")
                    .arg(message_id).arg(host.toString(), it.value().sender().toString()),
                        CODELOC );

    Reply reply = it.value();
    
    d->received_replies.remove(message_id);
    
    return reply;
}

/** Wait until a reply to the message with ID 'message_id' has been 
    received, or 'timeout' milliseconds have passed, and then take that reply. 
    A null Reply will be returned if a reply is not available. Note that this 
    will take the reply, so will cause any other thread that is waiting for the 
    reply to wait forages - so don't wait for the same reply in two
    different threads! */
Reply Communicator::tryAwaitReply(const QUuid &host, quint64 message_id, int timeout)
{
    if (timeout < 0)
        return Communicator::awaitReply(host, message_id);

    QTime t;
    t.start();

    boost::shared_ptr<CommunicatorData> d = data();

    MutexLocker lkr( &(d->reply_mutex) );

    QHash<quint64,Reply>::const_iterator it = d->received_replies.constFind(message_id);
    
    timeout -= t.restart();
    
    while (it == d->received_replies.constEnd())
    {
        d->reply_waiter.wait( &(d->reply_mutex), timeout );
        
        it = d->received_replies.constFind(message_id);
        
        timeout -= t.restart();
        
        if (timeout <= 0 and it == d->received_replies.constEnd())
            return Reply();
    }
    
    if (it.value().sender() != host)
        throw SireCluster::network_error( QObject::tr(
                "Strange - we are waiting for the reply to message %1 from "
                "host %2, but the received reply to this message is from "
                "host %3.")
                    .arg(message_id).arg(host.toString(), it.value().sender().toString()),
                        CODELOC );

    Reply reply = it.value();
    
    d->received_replies.remove(message_id);
    
    return reply;
}
