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

namespace SireMPI
{

class MPIWorker;
class MPINodes;
class MPIBackendNodes;
class MPIBackends;
class MPIFrontEnd;
class MPIFrontEnds;

MPIBackends getBackends(const MPINodes &nodes);
MPIBackends getBackends(const MPIBackendNodes &nodes);

namespace detail
{
class MPIBackendsPvt;
class MPIBackendPvt;
}

/** This class manages the collection of backend workers
    for a collection of MPI nodes
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIBackends
{

friend class MPIFrontEnds;

public:
    MPIBackends();
    MPIBackends(const MPINodes &nodes);
    
    MPIBackends(const MPIBackends &other);
    
    ~MPIBackends();
    
    MPIBackends& operator=(const MPIBackends &other);
    
    bool operator==(const MPIBackends &other) const;
    bool operator!=(const MPIBackends &other) const;
    
    int exec();
    void execBG();

protected:
    MPIFrontEnd start(const MPINode &node); // called by MPIFrontEnds
    
    void broadcastMessage(int message, int data); // called by MPIFrontEnds
    void broadcastMessage(int message, const QByteArray &data); // called by MPIFrontEnds

private:
    void start(int master, const QByteArray &data);
    void stop(int master);
    
    void stopAll();
    
    /** PIMPL pointer */
    boost::shared_ptr<MPIBackendsPvt> d;
};

/** This class is an individual backend that performs the actual
    work of a job on the backend node 
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIBackend
{

friend class MPIBackends;

public:
    ~MPIBackend();

protected:
    MPIBackend(void *communicator, int master, int tag);

    void start();
    
private:
    void sendResponse(int response, const MPIWorker &worker);
    void runJob(MPIWorker &worker);

    /** PIMPL pointer */
    boost::shared_ptr<MPIBackendPvt> d;
};

}

SIRE_END_HEADER

#endif
