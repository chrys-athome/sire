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

#include <QCoreApplication>
#include <QUuid>
#include <QTextStream>
#include <QProcess>

#include "client.h"
#include "sharedmemory.h"
#include "messagequeue.h"

#include <QDebug>

using namespace SirenTest;

int Client::run(int argc, char **argv)
{
    QCoreApplication *a = new QCoreApplication(argc, argv);
    
    /** Construct the Client object */
    Client *client = new Client();

    client->startServerProcess();
    
    client->message_q->send( "Sut dych chi?" );

    int exit_code = a->exec();
    
    delete a;
    delete client;
    
    return exit_code;
}

Client::Client(QObject *parent) : QObject(parent), keep_alive(true)
{
    //generate a key to create the QSharedMemory section
    shmem_key = QString("org.siremol.siren.shmem_key/%1")
                    .arg( QUuid::createUuid().toString() );

    //now create the server side of the message queue
    message_q = new MessageQueue(this);
    message_q->createAsHeads(shmem_key);

    connect(message_q, SIGNAL(receivedMessage()), this, SLOT(receivedMessage()));

    server_process = new QProcess(this);
    
    SirenTest::registerProcess(server_process);
    
    connect(server_process, SIGNAL(finished(int,QProcess::ExitStatus)), 
            this, SLOT(serverProcessExited(int,QProcess::ExitStatus)));

    connect(server_process, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(serverProcessError(QProcess::ProcessError)));
}

Client::~Client()
{}

/** Slot called when the server process exits */
void Client::serverProcessExited(int exitCode, QProcess::ExitStatus)
{
    if (keep_alive)
        this->startServerProcess();
}

static QString getError(QProcess::ProcessError error)
{
    switch (error)
    {
    case QProcess::FailedToStart:
        return QObject::tr("QProcess::FailedToStart");
    case QProcess::Crashed:
        return QObject::tr("QProcess::Crashed");
    case QProcess::Timedout:
        return QObject::tr("QProcess::Timedout");
    case QProcess::WriteError:
        return QObject::tr("QProcess:WriteError");
    case QProcess::ReadError:
        return QObject::tr("QProcess:ReadError");
    default:
        return QObject::tr("QProcess::UnknownError");
    }
}

void Client::serverProcessError(QProcess::ProcessError error)
{
    qDebug() << "Server process had an error!" << ::getError(error);
    
    if (error == QProcess::Crashed)
    {
        qDebug() << "Server process crashed!";
    }
    else if (error == QProcess::FailedToStart)
    {
        qDebug() << "Server process failed to start!";
    }
}

/** (Re)start the server process */
void Client::startServerProcess()
{
    if (server_process->state() == QProcess::Running)
        return;

    //get the name of the server process from this process
    QString server_name = QCoreApplication::applicationFilePath();
    
    server_name.replace("sirentest", "sirentest_backend");
    
    //pass the shmem key to the process via an environmental variable
    {
        QStringList env = QProcess::systemEnvironment();
    
        env << QString("SIRENTEST_SHMEMKEY=%1").arg(shmem_key);
        env << "SIRENTEST_IS_SERVER=1";
        env << "SIRENTEST_VERSION=1";
    
        server_process->setEnvironment(env);
    }
    
    server_process->setProcessChannelMode(QProcess::ForwardedChannels);
    server_process->start(server_name, QIODevice::ReadOnly);
}

/** Send a message to the server */
void Client::send(const QByteArray &data)
{
    message_q->send(data);
}

/** A message has been received */
void Client::receivedMessage()
{
    qDebug() << "CLIENT" << message_q->receive();
    
    message_q->send("Da iawn. A chi?");
}
