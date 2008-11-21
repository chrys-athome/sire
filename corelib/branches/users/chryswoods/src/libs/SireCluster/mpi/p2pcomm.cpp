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

#include <mpi.h>  // must be first to satisy mpich

#include <QThread>
#include <QMutex>

#include "p2pcomm.h"
#include "mpicluster.h"

#include "SireCluster/frontend.h"

#include "SireMaths/rangenerator.h"

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::MPI;

namespace SireCluster
{
namespace MPI
{
namespace detail
{

/** Private implementation of P2PComm */
class P2PCommPvt : public QThread
{
public:
    P2PCommPvt() : keep_running(false)
    {
        //create a local P2P
        master_rank = MPICluster::getRank();
        slave_rank = master_rank;
        is_master = true;
        is_slave = true;
    }
    
    P2PCommPvt(::MPI::Intracomm comm, int master, int slave)
            : private_comm(comm), master_rank(master), slave_rank(slave),
              keep_running(false)
    {
        int my_rank = MPICluster::getRank();
        
        is_master = (my_rank == master_rank);
        is_slave = (my_rank == slave_rank);
    }
    
    ~P2PCommPvt()
    {
        datamutex.lock();
        keep_running = false;
        datamutex.unlock();
        
        //wait until the background thread has stopped
        this->wait();
    
        //free the communicator
        private_comm.Barrier();
        private_comm.Free();
    }
    
    /** Mutex to protect access to this communicator */
    QMutex datamutex;

    /** A local frontend to the backend that sits behind
        this communicator (this is only non-null on the slave) */
    Frontend local_backend;
    
    /** The private MPI communicator for this backend */
    ::MPI::Intracomm private_comm;
    
    /** The rank of the master process in this communicator */
    int master_rank;
    
    /** The rank of the slave process in this communicator */
    int slave_rank;
    
    /** Whether this is the master process */
    bool is_master;
    
    /** Whether this is the slave process */
    bool is_slave;
    
    /** Whether or not to keep looping */
    bool keep_running;

protected:
    void run();
};

} // end of namespace detail
} // end of namespace MPI
} // end of namespace SireCluster

using namespace SireCluster::MPI::detail;

/** Background thread containing the event loop for the slave P2PComm */
void P2PCommPvt::run()
{
    SireError::setThreadString( "P2PComm_Slave" );
    SireMaths::seed_qrand();

    QMutexLocker lkr( &datamutex );
    
    while (keep_running)
    {
        lkr.unlock();
        
        QThread::msleep(250);
        
        lkr.relock();
    }
}

/** Null constructor */
P2PComm::P2PComm()
{}

/** Construct the point-to-point communicator between the 
    processes with ranks 'master_rank' and 'slave_rank' in 
    the global MPICluster::communicator(). Note that this
    is a collective operation, so *must* be called on all
    processes in the MPI cluster at the same time. */
P2PComm::P2PComm(int master_rank, int slave_rank)
{
    this->operator=( MPICluster::createP2P(master_rank, slave_rank) );
}

/** Copy constructor */
P2PComm::P2PComm(const P2PComm &other) : d(other.d)
{}

/** Destructor */
P2PComm::~P2PComm()
{}

/** Copy assignment operator */
P2PComm& P2PComm::operator=(const P2PComm &other)
{
    d = other.d;
    return *this;
}

/** Return whether or not this is a null P2P communicator */
bool P2PComm::isNull() const
{
    return d.get() == 0;
}

/** Return whether or not this is the master side of the 
    communicator (the process holding the frontend)
    
    This can be both, if this is an intra-process communicator
*/
bool P2PComm::isMaster()
{
    if (this->isNull())
        return false;
        
    else
    {
        QMutexLocker lkr( &(d->datamutex) );
        return d->is_master;
    }
}

/** Return whether or not this is the slave side of the 
    communicator (the process holding the backend)
    
    This can be both, if this is an intra-process communicator
*/
bool P2PComm::isSlave()
{
    if (this->isNull())
        return false;
        
    else
    {
        QMutexLocker lkr( &(d->datamutex) );
        return d->is_slave;
    }
}

/** Return whether or not this communicator is local
    (is within the same MPI process) */
bool P2PComm::isLocal()
{
    if (this->isNull())
        return true;
        
    else
    {
        QMutexLocker lkr( &(d->datamutex) );
        return d->is_master and d->is_slave;
    }
}

/** Internal function called by MPICluster used to create a 
    point-to-point communicator that works for just this process */
P2PComm P2PComm::createLocal()
{
    P2PComm p2p;
    
    p2p.d.reset( new P2PCommPvt() );
    
    return p2p;
}

/** Internal function called by MPICluster used to create a 
    point-to-point communicator that works using the provided
    communicator, with this as the master process if 'is_master'
    is true */
P2PComm P2PComm::create(::MPI::Intracomm private_comm,
                        int master_rank, int slave_rank)
{
    P2PComm p2p;
    
    int my_rank = MPICluster::getRank();
    
    if (my_rank == master_rank or my_rank == slave_rank)
    {
        p2p.d.reset( new P2PCommPvt(private_comm, master_rank, slave_rank) );
    }
    else
    {
        //we are not involved
        qDebug() << SireError::getPIDString() << "is not involved in this communicator";
        
        private_comm.Barrier();
        private_comm.Free();
    }
    
    return p2p;
}

/** Return whether or not this communicator involves the process
    with MPI rank 'rank' */
bool P2PComm::involves(int rank)
{
    if (this->isNull())
        return false;
        
    else
    {
        int my_rank = MPICluster::getRank();
    
        QMutexLocker lkr( &(d->datamutex) );
        
        return (my_rank == d->master_rank) or 
               (my_rank == d->slave_rank);
    }
}

/** Set the backend that will be controlled by the MPI frontend
    (this is held using a local Frontend) */
void P2PComm::setBackend(const Frontend &backend)
{
    if (not this->isSlave())
        throw SireError::program_bug( QObject::tr(
            "Only the slave P2PComm holds the backends..."), CODELOC );

    QMutexLocker lkr( &(d->datamutex) );
    
    if (not d->local_backend.isNull())
        throw SireError::program_bug( QObject::tr(
            "You cannot give a second backend to a P2PComm!"), CODELOC );
            
    d->local_backend = backend;
    
    //start the background thread that listens for communication 
    //from the master
    d->keep_running = true;
    d->start();
}

#endif
