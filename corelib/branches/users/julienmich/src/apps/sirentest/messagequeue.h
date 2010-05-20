/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIREN_MESSAGEQUEUE_H
#define SIREN_MESSAGEQUEUE_H

#include <QObject>
#include <QQueue>
#include <QByteArray>

#include "Siren/sirenglobal.h"

SIREN_BEGIN_HEADER

class QSharedMemory;
class QTimer;

namespace SirenTest
{

/** This class provides a message queue that can be used
    to allow two processes to exchange data with one another
  
    Messages take the form of QByteArrays, which contain
    serialised types, e.g. QString, qint32 or Siren objects 
        
    @author Christopher Woods
*/
class SIRENTEST_EXPORT MessageQueue : public QObject
{
    Q_OBJECT

public:
    MessageQueue(QObject *parent=0);
    ~MessageQueue();

    bool messageAvailable() const;

    bool createAsHeads(const QString &key);
    bool createAsTails(const QString &key);
    
    bool attachAsHeads(const QString &key);
    bool attachAsTails(const QString &key);

    QByteArray receive();

public slots:
    void send(const QByteArray &message);
    void clear();
    
signals:
    void receivedMessage();

private slots:
    void pollForMessages();

private:
    enum { EMPTY = 0,
           HEADS_SEND = 1,
           TAILS_SEND = 2 };
    
    bool create(const QString &key);
    bool attach(const QString &key);

    bool isHeads() const;
    bool isTails() const;

    void sendData(const QByteArray &data);
    bool receiveData();

    QString me() const;

    /** Pointer to the shared memory used to exchange messages */
    QSharedMemory *shmem;
    
    /** Timer used to poll for new messages */
    QTimer *poller;
    
    /** A partially sent message */
    QByteArray partial_sent;
    
    /** A partially received message */
    QByteArray partial_receive;
    
    /** The queue of messages to be sent */
    QQueue<QByteArray> send_queue;
    
    /** The queue of messages that have been received */
    QQueue<QByteArray> received_queue;
    
    /** The number of remaining chunks to send */
    quint32 n_to_send;
    
    /** Is this the head process? */
    bool is_heads;
};

}

SIREN_END_HEADER

#endif
