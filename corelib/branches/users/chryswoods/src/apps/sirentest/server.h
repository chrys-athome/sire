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

#ifndef SIRENTEST_SERVER_H
#define SIRENTEST_SERVER_H

#include <QObject>

#include "Siren/sirenglobal.h"

SIREN_BEGIN_HEADER

namespace SirenTest
{

class MessageQueue;

/** This is the server - this is the class that runs on the 
    backend that runs the tests and reports the results
    back to the Client running on the frontend
    
    @author Christopher Woods
*/
class SIRENTEST_EXPORT Server : public QObject
{

Q_OBJECT

public:
    static int run(int argc, char **argv);

    Server(QObject *parent=0);
    ~Server();

    void setPrintToScreen(bool print_to_screen);

signals:
    void fatalError();

protected slots:
    void receivedMessage();
    
protected:
    bool connectClient(const QString &shmem_key, int version);

private:
    /** Message queue used to communicate with the server */
    MessageQueue *message_q;
    
    /** Whether or not to print test results to the screen */
    bool print_to_screen;
};

}

SIREN_END_HEADER

#endif
