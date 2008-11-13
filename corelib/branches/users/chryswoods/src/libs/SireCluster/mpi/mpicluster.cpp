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

#ifndef __SIRE_USE_MPI__

#include <mpi.h>   //mpich requires that mpi.h is included first

#include <QThread>

#include "mpicluster.h"
#include "frontend.h"
#include "backend.h"

using namespace SireCluster;

namespace SireCluster
{
namespace detail
{

/** Private implementation of MPICluster */
class MPIClusterPvt : public QThread
{
public:
    MPIClusterPvt() : datamutex(QMutex::Recursive), keep_running(true)
    {}
    
    ~MPIClusterPvt()
    {}
    
    bool isMaster() const;
    
    int master() const;
    MPI::Any slaves() const;
    
    void registerBacked(const Backend &backend);
    
    Frontend getFrontend(const QUuid &uid);
    
    QList<QUuid> UIDs();
    
    void shutdown();
    
    /** Mutex to protect access to the data of this cluster */
    QMutex datamutex;
    
    /** A communicator for sending messages */
    MPI::Intracomm send_comm;
    
    /** A communicator for receiving messages */
    MPI::Intracomm recv_comm;
    
    /** Mutex to protect send_comm */
    QMutex send_mutex;
    
    /** Mutex to protect recv_comm */
    QMutex recv_mutex;
    
    /** Whether or not to keep the event loop running */
    bool keep_running;
    
protected:
    void run();
};

} // end of namespace detail
} // end of namespace SireCluster

using namespace SireCluster::detail;

Q_GLOBAL_STATIC_WITH_ARGS( QMutex, globalMutex, QMutex::Recursive );

static MPICluster *global_cluster;

/** Return the global MPICluster */
MPICluster& MPICluster::cluster()
{
    if (global_cluster == 0)
    {
        global_cluster = new MPICluster();
        global_cluster->start();
    }
    
    return *global_cluster;
}

/** Constructor */
MPICluster::MPICluster() : d( new MPIClusterPvt() )
{}

/** Destructor */
MPICluster::~MPICluster()
{}

/** Return the rank of the master process */
int MPIClusterPvt::master() const
{
    return 0;
}

/** Return whether or not this is the master process */
bool MPIClusterPvt::isMaster() const
{
    return MPI::COMM_WORLD.Get_rank() == master();
}

/** Return the MPI ID of the slaves */
MPI::Any MPIClusterPvt::slaves() const
{
    return MPI::Any;
}

/** The event loop for the MPICluster */
void MPIClusterPvt::run()
{
    if (MPI::COMM_WORLD.Get_rank() == 0)
    {
        //this is the master - run the master event loop
        while (keep_running)
        {
            //are there any requests from the slaves?
            QMutexLocker lkr(&recv_mutex);
        
            //if (recv_comm.Irecv(.... slaves(), 0, status)...
        
            lkr.unlock();
            QThread::sleep(1);
        }
    }
    else
    {
        //this is one of the slave nodes - run the slave event loop
        while (keep_running)
        {
            //are there any instructions from the master?
            QMutexLocker lkr(&recv_mutex);

            //if (recv_comm.Irecv(... , master(), 0, status )...

            QThread::sleep(1);
        }
    }
}

/** Start the cluster */
void MPICluster::start()
{
    if (d->isRunning())
        return;

    //use our own copy of MPI::COMM_WORLD - create one communicator
    //for sending to the master, and one for recieving
    
    if (not MPI::Is_initialized())
    {
        int argc = 0;
        char **argv = 0;
        MPI::Init(argc, argv);
    }
    
    if (d->isMaster())
    {
        //this is the first process - this is the master that 
        //will contain the global registry
        
        //create a communicator to receive messages from the nodes
        d->recv_comm = MPI::COMM_WORLD.Clone();
        
        //now create a communicator to send messages to the nodes
        d->send_comm = MPI::COMM_WORLD.Clone();
    }
    else
    {
        //this is not the first process - it will use the global registry
        //on the master
        
        //create a communicator to send messages to the master
        d->send_comm = MPI::COMM_WORLD.Clone();
        
        //now create a communicator to receive messages from the master
        d->recv_comm = MPI::COMM_WORLD.Clone();
    }
    
    //now start the event loop
    d->keep_running = true;
    d->start();
}

void MPIClusterPvt::registerBackend(const Backend &backend)
{
    if (this->isMaster())
    {

    QMutexLocker lkr(&send_mutex);
}

/** Register the local Backend 'backend' with the MPI cluster so that
    it can be connected to by any MPI-connected node */
void MPICluster::registerBackend(const Backend &backend)
{
    if (backend.isNull())
        return;

    QMutexLocker lkr( globalMutex() );
    
    cluster().d->registerBackend(backend.UID());
}

/** Return the frontend for backend with UID 'uid', or return
    a null Frontend if there is no such backend. This blocks
    until the Frontend is available */
Frontend MPICluster::getFrontend(const QUuid &uid)
{
    if (uid.isNull())
        return Frontend();
        
    QMutexLocker lkr( globalMutex() );
    MPICluster &c = cluster();
    lkr.unlock();
    
    return c.d->getFrontend(uid);
}

/** Return the list of all of the UIDs of all of the backends
    that are available via MPI */
QList<QUuid> MPICluster::UIDs()
{
    QMutexLocker lkr( globalMutex() );
    
    return cluster().d->UIDs();
}

/** Shutdown the MPI cluster */
void MPICluster::shutdown()
{
    QMutexLocker lkr( globalMutex() );
    
    cluster().d->shutdown();
    
    //now delete the cluster
    delete global_cluster;
    global_cluster = 0;
}

#endif // __SIRE_USE_MPI__
