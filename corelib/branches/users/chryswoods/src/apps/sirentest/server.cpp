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
#include <QTextStream>

#include "server.h"
#include "messagequeue.h"

#include "Siren/class.h"
#include "Siren/objref.h"
#include "Siren/hanref.h"
#include "Siren/logger.h"
#include "Siren/exception.h"

#include <QDebug>

using namespace SirenTest;
using namespace Siren;

int Server::run(int argc, char **argv)
{
    QCoreApplication *a = new QCoreApplication(argc, argv);
    
    //if this has been started properly, then the
    //environmental variable 'SIRENTEST_IS_SERVER' will equal '1'
    if ( ::getenv("SIRENTEST_IS_SERVER") == 0 )
    {
        QTextStream ts(stderr);
        
        ts << QObject::tr("This program cannot be run on its own. It must "
                          "be run only by \"sirentest\".") << endl;
        
        return -1;
    }

    //now get the client version and key to the shared memory segment
    int client_version = QString(::getenv("SIRENTEST_VERSION")).toInt();
    QString shmem_key = QString( ::getenv("SIRENTEST_SHMEMKEY") );
    bool print_to_screen = QString( ::getenv("SIRENTEST_PRINT_TO_SCREEN") ).toInt();
    
    Server *server = new Server();
    
    server->setPrintToScreen(print_to_screen);

    if (not server->connectClient(shmem_key, client_version))
    {
        QTextStream ts(stderr);
        
        ts << QObject::tr("Failed to connect using key %1 (client version %2).")
                .arg(shmem_key).arg(client_version) << endl;
                
        delete server;
        delete a;
        return -1;
    }

    connect(server, SIGNAL(fatalError()), a, SLOT(quit()));
    
    int exit_code = a->exec();
    
    delete server;
    delete a;
    
    return exit_code;
}

void Server::setPrintToScreen(bool on)
{
    print_to_screen = on;
}

/** Constructor */
Server::Server(QObject *parent) : QObject(parent), print_to_screen(false)
{
    message_q = new MessageQueue(this);
    connect(message_q, SIGNAL(receivedMessage()), this, SLOT(receivedMessage()));
}

/** Destructor */
Server::~Server()
{}

/** Connect the client using the passed shared memory key and
    client version - return whether or not the connection was
    successful */
bool Server::connectClient(const QString &shmem_key, int version)
{
    if (version != 1)
    {
        QTextStream ts(stderr);
        ts << QObject::tr("This server can only work with version 1 clients. "
                          "The connecting client is version %1.").arg(version)
                << endl;
    }

    if (not message_q->attachAsTails(shmem_key))
    {
        QTextStream ts(stderr);
        ts << QObject::tr("Cannot connect to the client!") << endl;
                    
        return false;
    }
    
    return true;
}

/** We've received a message - this will be the name of a class to test */
void Server::receivedMessage()
{
    QByteArray message = message_q->receive();
    
    QString class_to_test;
    {
        QDataStream ds(message);
        ds >> class_to_test;
    }
    
    bool passed;
    QString test_output;
    
    try
    {
        Class c(class_to_test);

        if (not c.isConcrete())
            passed = true;
        else
        {
            Logger logger;
            
            if (not print_to_screen)
                logger = Logger( new QTextStream(&test_output, QIODevice::WriteOnly) );
        
            if (c.isHandle())
            {
                HanRef handle = c.createHandle();
                passed = handle.test(logger);
            }
            else
            {
                ObjRef object = c.createObject();
                passed = object.test(logger);
            }
        }
    }
    catch(const Siren::exception &e)
    {
        passed = false;
        test_output += QObject::tr("\n\nCaught unexpected exception %1, %2, "
                                   "thrown from %3.")
                            .arg(e.what(), e.error(), e.where());
    }
    catch(const std::exception &e)
    {
        passed = false;
        test_output += QObject::tr("\n\nCaught unexpected exception %1.")
                             .arg(e.what());
    }
    catch(...)
    {
        passed = false;
        test_output += QObject::tr("\n\nCaught unknown and unexpected exception.");
    }
    
    QByteArray result;
    {
        QDataStream ds(&result, QIODevice::WriteOnly);
        ds << passed << test_output;
    }
    
    message_q->send(result);
}
