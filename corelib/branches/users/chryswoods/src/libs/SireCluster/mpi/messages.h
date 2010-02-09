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

#ifndef SIRECLUSTER_MPI_MESSAGES_H
#define SIRECLUSTER_MPI_MESSAGES_H

#ifdef SIRE_USE_MPI

#include <QUuid>
#include <QSet>

#include "Siren/object.h"
#include "Siren/objptr.hpp"
#include "Siren/datastream.h"
#include "Siren/exception.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{
    namespace MPI
    {
        class ProcessStarted;
        class ProcessEnded;
    
        class Message;
        class Shutdown;

        typedef Siren::ObjPtr<Message> MessagePtr;

        class Error;
        class Result;
    }
}

namespace SireCluster
{
namespace MPI
{

/** This is a holder for a Message, which provides information
    about who sent the message and where it is going */
class Envelope : public Siren::Implements<Envelope,Siren::Object>
{
public:
    Envelope();
    Envelope(const Envelope &other);
    
    ~Envelope();
    
    Envelope& operator=(const Envelope &other) const;

    bool operator==(const Envelope &other) const;
    bool operator!=(const Envelope &other) const;

    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);
    
    static Envelope send(const Message &message, const QUuid &uid);
    static Envelope send(const Message &message, const QSet<QUuid> &uid);
    static Envelope broadcast(const Message &message);
    
    bool isEmpty();
    
    const Message& message() const;
    
    bool isBroadcast() const;
    bool hasSingleRecipient() const;

    bool isRecipient(const QUuid &uid) const;
    
    const QUuid& sender() const;
    
private:
    /** The message itself */
    MessagePtr msg
    
    /** The sender */
    QUuid sender_uid;
    
    /** The destination (empty if this is a broadcast) */
    QSet<QUuid> destination_uids;
};

/** This is the virtual base class of all MPI messages sent between processes
    in the MPI cluster 
    
    @author Christopher Woods
*/
class Message : public Siren::Extends<Message,Siren::Object>
{

public:
    Message(const Message &other);
    
    virtual ~Message();
    
    static QString typeName();
    
    uint hashCode() const;
    QString toString() const;
    
    virtual void read() const=0;
    
    virtual bool hasReply() const;
    
    virtual MessagePtr reply() const;
    
    const QUuid& UID() const;
    const QUuid& subjectUID() const;
    
    int sender() const;
    int destination() const;
    
    virtual bool isRecipient(int rank) const;
    
    virtual QSet<int> recipients() const;
    
protected:
    Message();
    Message(QUuid destination);
    Message(const QSet<QUuid> &destinations);
    
    Message(QUuid destination, const QUuid &subject_uid);
    Message(const QSet<QUuid> &destinations, const QUuid &subject_uid);

    Message& operator=(const Message &other);
    
    bool operator==(const Message &other) const;
    bool operator!=(const Message &other) const;

private:
    /** The unique ID of this message (this allows messages and
        responses to be tallied */
    QUuid uid;

    /** The ID of the subject that this message relates to
         - this allows several related messages to be exchanged
           and tracked */
    QUuid subject_uid;

    /** The UID of the process that sent this message */
    QUuid sent_by;
    
    /** The UID of the process that should receive this message.
        */
    QSet<QUuid> dest;
};

/** This message is sent to announce that a process has started */
class ProcessStarted : public Siren::Implements<ProcessStarted,Message>
{
public:
    ProcessStarted();
    ProcessStarted(const QUuid &process_uid);
    ProcessStarted(const ProcessStarted &other);
    
    ~ProcessStarted();
    
    ProcessStarted& operator=(const ProcessStarted &other);
    
    bool operator==(const ProcessStarted &other) const;
    bool operator!=(const ProcessStarted &other) const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);
    
    void read() const;
    
private:
    /** The UID of the process that has just started */
    QUuid process_uid;
};

/** This message is sent to announce that a process has ended */
class ProcessEnded : public Siren::Implements<ProcessEnded,Message>
{
public:
    ProcessEnded();
    ProcessEnded(const QUuid &process_uid);
    ProcessEnded(const ProcessEnded &other);
    
    ~ProcessEnded();
    
    ProcessEnded& operator=(const ProcessEnded &other);
    
    bool operator==(const ProcessEnded &other) const;
    bool operator!=(const ProcessEnded &other) const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);
    
    void read() const;
    
private:
    /** The UID of the process that has just ended */
    QUuid process_uid;
};

/** This message is sent to return a result
    
    @author Christopher Woods
*/
class Result : public Siren::Implements<Result,Message>
{
public:
    Result();
    
    template<class T>
    Result(const Message &message, const T &value)
          : Siren::Implements<Result,Message>(message.sender(), message.subjectUID())
    {
        Siren::DataStream ds(&result_data, QIODevice::WriteOnly);
        ds << value;
    }
    
    Result(const Result &other);
    
    ~Result();
    
    Result& operator=(const Result &other);
    
    bool operator==(const Result &other) const;
    bool operator!=(const Result &other) const;
    
    QString toString() const;
    uint hashCode() const;
    
    void stream(Siren::Stream &s);
    
    void read() const;

private:
    /** A binary representation of the result */
    QByteArray result_data;
};

/** This message is sent to return an error!
    
    @author Christopher Woods
*/
class Error : public Siren::Implements<Error,Message>
{
public:
    Error();
    Error(const QString &code_loc);
    Error(const Siren::exception &e);
    Error(const std::exception &e);
    
    Error(const Message &message);
    Error(const Message &message, const QString &code_loc);
    Error(const Message &message, const Siren::exception &e);
    Error(const Message &message, const std::exception &e);
    
    Error(int sender);
    Error(int sender, const QString &code_loc);
    Error(int sender, const Siren::exception &e);
    Error(int sender, const std::exception &e);
    
    Error(const QByteArray &message_data, const QByteArray &error_data);
    
    Error(const Error &other);
    
    ~Error();
    
    Error& operator=(const Error &other);
    
    bool operator==(const Error &other) const;
    bool operator!=(const Error &other) const;
    
    QString toString() const;
    uint hashCode() const;
    
    void stream(Siren::Stream &s);
    
    void read() const;

private:
    void setError(const Siren::exception &e);
    void setError(const std::exception &e);
    void setUnknownError(const QString &code_loc);

    /** A binary representation of the message that caused the error */
    QByteArray message_data;
    
    /** A binary representation of the error */
    QByteArray error_data;
};

/** This is the message sent to a node to tell it to shutdown. 
    
    @author Christopher Woods
*/
class Shutdown : public Siren::Implements<Shutdown,Message>
{
public:
    Shutdown();
    Shutdown(const Shutdown &other);
    
    ~Shutdown();
    
    Shutdown& operator=(const Shutdown &other);

    bool operator==(const Shutdown &other) const;
    bool operator!=(const Shutdown &other) const;

    QString toString() const;
    uint hashCode() const;
    
    void stream(Siren::Stream &s);

    void read() const;
};

}  // end of namespace MPI
}  // end of namespace SireCluster

Q_DECLARE_METATYPE( SireCluster::MPI::Envelope )
Q_DECLARE_METATYPE( SireCluster::MPI::Result )
Q_DECLARE_METATYPE( SireCluster::MPI::Error )

SIRE_END_HEADER

#endif // SIRE_USE_MPI

#endif
