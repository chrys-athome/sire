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

#ifndef SIREMPI_MPIBACKENDS_H
#define SIREMPI_MPIBACKENDS_H

#include "sireglobal.h"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

class QUuid;

namespace SireMPI
{

class MPIWorker;
class MPINode;
class MPINodes;
class MPIBackendNodes;
class MPIBackends;
class MPIFrontend;
class MPIFrontends;

MPIBackends getBackends(const MPINodes &nodes);
MPIBackends getBackends(const MPIBackendNodes &nodes);

namespace detail
{
class MPIBackendsPvt;
class MPIBackendPvt;

class MPIFrontendPvt;
}

/** This class manages the collection of backend workers
    for a collection of MPI nodes
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIBackends
{

friend class MPIFrontends;
friend class detail::MPIFrontendPvt;

public:

    MPIBackends();
    MPIBackends(const MPINodes &nodes);
    MPIBackends(const MPIBackendNodes &nodes);
    
    MPIBackends(const MPIBackends &other);
    
    ~MPIBackends();
    
    MPIBackends& operator=(const MPIBackends &other);
    
    bool operator==(const MPIBackends &other) const;
    bool operator!=(const MPIBackends &other) const;
    
    static int globalRank();
    
    void exec();

protected:
    MPIFrontend start(const MPINode &node); // called by MPIFrontends
    void stop(const QUuid &uid); // called by MPIFrontendPvt
    
    void broadcastMessage(int message, int data); // called by MPIFrontends
    void broadcastMessage(int message, const QByteArray &data); // called by MPIFrontends

    void shutdown(); // called by MPIFrontends

private:

    enum {
           START_MPI_BACKEND   = 0x0100,   //
           STOP_MPI_BACKEND    = 0x0200,   // Message used to start and stop a
           SHUTDOWN_MPI        = 0x0400    // backend
         };

    void startLocalBackend(const QUuid &uid);
    void stopLocalBackend(const QUuid &uid);

    void startBackend(const QUuid &uid);
    void stopBackend(const QUuid &uid);
    
    void stopAllBackends();
    
    /** PIMPL pointer */
    boost::shared_ptr<detail::MPIBackendsPvt> d;
};

/** This class is an individual backend that performs the actual
    work of a job on the backend node 
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIBackend
{

friend class MPIBackends;

public:
    enum { EMPTY_MESSAGE       = 0x0000,
           
           START_WORK          = 0x0001,   //
           STOP_WORK           = 0x0002,   // Messages sent from the frontend
           ABORT_WORK          = 0x0004,   // to the backend
           SEND_PROGRESS       = 0x0008,   //
           SEND_INTERIM_RESULT = 0x000f,   //
           
           JOB_FINISHED        = 0x0010,   //
           JOB_STOPPED         = 0x0020,   // Messages sent from the backend
           JOB_ABORTED         = 0x0040,   // back to the frontend
           JOB_PROGRESS_UPDATE = 0x0080,   //
           JOB_RESULT_UPDATE   = 0x00f0    //
         };
        
    MPIBackend();
    ~MPIBackend();

    static int globalRank();

protected:
    MPIBackend(void *communicator, const QUuid &uid);

    void start();
    void stop();
    
    void wait();
    
private:

    /** PIMPL pointer */
    boost::shared_ptr<detail::MPIBackendPvt> d;
};

}

SIRE_END_HEADER

#endif
