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

#ifdef __SIRE_USE_MPI__

#include <mpi.h>

#include "mpifrontend.h"

#include "SireCluster/workpacket.h"

using namespace SireCluster;
using namespace SireCluster::MPI;

/** Null constructor */
MPIFrontend::MPIFrontend() : FrontendBase()
{}

/** Construct a frontend that uses the passed  
    point-to-point communicator to communicate with 
    the backend */
MPIFrontend::MPIFrontend(const P2PComm &p2pcomm)
            : FrontendBase(), p2p(p2pcomm)
{}

/** Destructor */
MPIFrontend::~MPIFrontend()
{}

/** This is only local if the p2p communicator is local */
bool MPIFrontend::isLocal() const
{
    return const_cast<P2PComm*>(&p2p)->isLocal();
}

/** Return the UID of the backend */
QUuid MPIFrontend::UID()
{
    return QUuid();
}

/** Start a job on the backend */
void MPIFrontend::startJob(const WorkPacket &workpacket)
{
}

/** Stop the job on the backend */
void MPIFrontend::stopJob()
{
}

/** Abort the job on the backend */
void MPIFrontend::abortJob()
{
}

/** Wait for the job to finish */
void MPIFrontend::wait()
{
}

/** Wait for the job to finish, or until 'timeout'
    milliseconds have passed */
bool MPIFrontend::wait(int timeout)
{
    return true;
}

/** Return the progress of the work */
float MPIFrontend::progress()
{
    return 0;
}

/** Return an interim result */
WorkPacket MPIFrontend::interimResult()
{
    return WorkPacket();
}

/** Return the final result - this blocks until
    it is available */
WorkPacket MPIFrontend::result()
{
    return WorkPacket();
}

#endif
