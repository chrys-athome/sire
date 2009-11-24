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

#ifndef SIRENTEST_CLIENT_H
#define SIRENTEST_CLIENT_H

#include <QProcess>

#include "Siren/sirenglobal.h"

SIREN_BEGIN_HEADER

class QProcess;

namespace SirenTest
{

class MessageQueue;

/** This is the client that connects to the server running
    the tests, telling the server what to run and then 
    collecting the output 
    
    @author Christopher Woods
*/
class SIRENTEST_EXPORT Client : public QObject
{

Q_OBJECT

public:
    static int run(int argc, char **argv);

    Client(QObject *parent=0);
    ~Client();

protected slots:
    void serverProcessExited(int exitCode, QProcess::ExitStatus exit_status);
    void serverProcessError(QProcess::ProcessError error);

    void receivedMessage();

private:
    void startServerProcess();

    void send(const QByteArray &message);
    QByteArray receive();

    /** The message queue used for communication */
    MessageQueue *message_q;

    /** The key for the shared memory */
    QString shmem_key;

    /** The QProcess containing the server */
    QProcess *server_process;

    /** Whether or not to keep the child process alive */
    bool keep_alive;
};

}

SIREN_END_HEADER

#endif
