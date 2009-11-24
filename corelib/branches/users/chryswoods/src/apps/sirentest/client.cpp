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
#include <QTimer>

#include "client.h"
#include "sharedmemory.h"
#include "messagequeue.h"

#include "Siren/class.h"

#include <QDebug>

using namespace SirenTest;
using namespace Siren;

int Client::run(int argc, char **argv)
{
    QCoreApplication *a = new QCoreApplication(argc, argv);
    
    /** Construct the Client object */
    Client *client = new Client();

    client->startServerProcess();

    //get the list of classes to test
    QStringList test_classes = Class::registeredTypes();
    
    client->runTests(test_classes);
    
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

    test_timer = new QTimer(this);
    
    connect(test_timer, SIGNAL(timeout()), this, SLOT(testTimeout()));
}

Client::~Client()
{}

/** Run the tests for all of the passed classes */
void Client::runTests(const QStringList &classes)
{
    classes_to_test = classes;
    current_test = -1;
    
    QTimer::singleShot(0, this, SLOT(runNextTest()));
}

/** Run the next test */
void Client::runNextTest()
{
    test_timer->stop();
    message_q->clear();
    
    ++current_test;

    if (current_test >= classes_to_test.count())
    {
        //we've finished
        keep_alive = false;
        
        disconnect(this, 0,0,0);
        disconnect(server_process, 0,0,0);
        disconnect(message_q, 0,0,0);
        disconnect(test_timer, 0,0,0);
        
        server_process->terminate();
        server_process->waitForFinished();

        this->printSummary();
    
        QCoreApplication::exit(0);
        return;
    }

    QByteArray data;
    {
        QDataStream ds(&data, QIODevice::WriteOnly);
        ds << classes_to_test.at(current_test);
    }
    
    QTextStream ts(stdout);
    
    ts.setFieldWidth(3);
    ts.setRealNumberPrecision(0);
    ts.setRealNumberNotation(QTextStream::FixedNotation);
    ts << ( (100.0*(current_test+1))/classes_to_test.count() );
    
    ts.setFieldWidth(0);
    ts << "%    ";
    
    ts.setFieldWidth(40);
    ts << classes_to_test.at(current_test);
    
    ts.setFieldWidth(0);
    ts << " : ";
    
    message_q->send(data);
    
    //each test has 60 seconds to complete
    test_timer->start(60000);
}

/** Print a summary of all of the failures */
void Client::printSummary()
{
    QStringList failed_classes = failed_tests.keys();
    qSort(failed_classes);
    
    QTextStream ts(stdout);
    
    if (failed_tests.isEmpty())
    {
        ts << endl << endl << "ALL TESTS PASSED - EVERYTHING IS OK!" << endl;
    }
    else
    {
        ts << endl << "DETAILS OF FAILED TESTS" << endl;
    
        foreach (QString failed_class, failed_classes)
        {
            ts << endl << " ****** " << failed_class << " ******" << endl;
            ts << failed_tests.value(failed_class) << endl;
        }
    }
    
    ts << "\n  ------- Goodbye ------ " << endl;
}

/** Called when a test passes */
void Client::testPassed()
{
    if (current_test >= 0 and current_test < classes_to_test.count())
    {
        QTextStream ts(stdout);
        ts << "  Passed" << endl;
    }
    
    this->runNextTest();
}

/** Called when a test failed */
void Client::testFailed(QString error)
{
    if (current_test >= 0 and current_test < classes_to_test.count())
    {
        failed_tests[ classes_to_test.at(current_test) ] += error;


        QTextStream ts(stdout);
        ts << "**Failed**" << endl;
    }

    this->runNextTest();
}

/** Slot called when the server process exits */
void Client::serverProcessExited(int exitCode, QProcess::ExitStatus)
{
    test_timer->stop();

    //are we running a test?
    this->testFailed( QObject::tr("CRASHED!") );

    if (keep_alive)
    {
        this->startServerProcess();
    }
}

static QString toString(QProcess::ProcessError error)
{
    switch(error)
    {
        case QProcess::FailedToStart:
            return QObject::tr("QProcess::FailedToStart");
        case QProcess::Crashed:
            return QObject::tr("QProcess::Crashed");
        case QProcess::Timedout:
            return QObject::tr("QProcess::Timedout");
        case QProcess::WriteError:
            return QObject::tr("QProcess::WriteError");
        case QProcess::ReadError:
            return QObject::tr("QProcess::ReadError");
        default:
            return QObject::tr("QProcess::FailedToStart");
    }
}

void Client::serverProcessError(QProcess::ProcessError error)
{
    test_timer->stop();

    if (current_test >= 0 and current_test < classes_to_test.count())
    {
        failed_tests[ classes_to_test.at(current_test) ] += 
                            QObject::tr("\n ** ERROR (%1) ** \n")
                                    .arg( ::toString(error) );
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

/** The test has timed out */
void Client::testTimeout()
{
    test_timer->stop();

    if (current_test >= 0 and current_test < classes_to_test.count())
    {
        failed_tests[ classes_to_test.at(current_test) ] += 
                            QObject::tr("\n ** TIMEOUT ** \n");
    }

    //lets just assume that the test has hung - just kill the process
    server_process->terminate();
}

/** A message has been received - the test has finished */
void Client::receivedMessage()
{
    test_timer->stop();
    
    bool passed;
    QString test_output;
    {
        QByteArray data = message_q->receive();
        
        if (not data.isEmpty())
        {
            QDataStream ds(data);
            ds >> passed >> test_output;
        }
    }
    
    if (passed)
        this->testPassed();
    else
        this->testFailed(test_output);
}
