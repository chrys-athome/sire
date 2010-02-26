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

#ifdef SIRE_USE_MPI

#include <mpi.h>   //mpich requires that mpi.h is included first

// The C MPI API is used as I have had problems caused by
// some clusters not installing the C++ API

// A single MPI thread is used as I have also had problems
// with broken or unavailable multi-threaded MPI implementations...!

// Only MPI-1 functions are used, as while MPI-2 is standard, it
// is not universally and completely implemented on most (or indeed
// any!) clusters that I have seen...

// This is the only file that contains MPI functions

#include <QQueue>
#include <QTime>

#include <boost/bind.hpp>

#include "mpicluster.h"

#include "SireCluster/network/communicator.h"
#include "SireCluster/network/hostinfo.h"

#include "SireCluster/errors.h"

#include "Siren/forages.h"
#include "Siren/thread.h"
#include "Siren/errors.h"
#include "Siren/datastream.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::MPI;
using namespace SireCluster::network;
using namespace Siren;

#ifndef HAVE_LSEEK64
    //////
    ////// add an lseek64 function stub to fill a function
    ////// that is missing - mpich needs lseek64 to be
    ////// defined, even if it is not available! Otherwise
    ////// dlopen errors as the symbol can't be found
    //////
    extern "C"
    {
        int SIREMPI_EXPORT lseek64(int fd, int offset, int whence)
        {
            throw Siren::program_bug( QObject::tr(
                "MPI implementation is calling lseek64 which is not supported "
                "on OS X (Leopard - 32bit)"), CODELOC );
            
            return 0;
        }
    }
#endif // HAVE_LSEEK64

/** Tag used by announce messages (gives the size and origin
    of a message) */
const int size_tag = 1;

/** Tag used by data messages (the actual message!) */
const int msg_tag = 2;

/** This object is used to control the queue of messages being
    sent to a recipient - one of these objects is needed
    per node in the cluster */
class MPISendQueue
{
public:
    enum SendState { IDLE = 0,
                     SENDING_SIZE = 1,
                     SENDING_BUFFER = 2 };

    MPISendQueue() : send_rank(-1), global_comm(0), send_state(IDLE)
    {}
    
    ~MPISendQueue()
    {}
    
    void set(int rank, MPI_Comm &comm)
    {
        send_rank = rank;
        global_comm = &comm;
    }
    
    void send(const QByteArray &message)
    {
        send_queue.enqueue(message);
    }
    
    bool process();
    
    /** The rank of the process to send the message to */
    int send_rank;
    
    /** The queue of messages to send to the recipient */
    QQueue<QByteArray> send_queue;
    
    /** The size of the data - this acts as a buffer for the 
        size during transmission */
    int send_buffer;
    
    /** The communicator used to communicate with the recipient */
    MPI_Comm *global_comm;
    
    /** The request object used to monitor data transmission */
    MPI_Request send_request;
           
    /** The current state of the transmission */
    SendState send_state;
    
    /** Timer to monitor how long transmission is taking,
        so that hangs or timeouts can be detected */
    QTime send_timer;
};

/** This object is used to control the queue of messages that
    are being received - there is one of these objects per
    node in the cluster */
class MPIRecvQueue
{
public:
    enum RecvState { IDLE = 0,
                     RECEIVING = 1 };

    MPIRecvQueue() : recv_rank(-1), global_comm(0), recv_state(IDLE)
    {}
    
    ~MPIRecvQueue()
    {}
    
    void set(int rank, MPI_Comm &comm)
    {
        recv_rank = rank;
        global_comm = &comm;
    }
    
    void receive(int message_size)
    {
        recv_queue.enqueue(message_size);
    }
    
    bool process();
    
    /** The rank of the process from which the message
        is being received */
    int recv_rank;
    
    /** Queue of messages to receive (their expected sizes) */
    QQueue<int> recv_queue;
    
    /** Buffer in which to receive the message */
    QByteArray recv_buffer;
    
    /** The communicator used to receive messages */
    MPI_Comm *global_comm;
    
    /** The request used to monitor data transmission */
    MPI_Request recv_request;
    
    /** The current state of the transmission */
    RecvState recv_state;
    
    /** Timer to monitor how long each receive has taken
        so that hangs or timeouts can be detected */
    QTime recv_timer;
};

class MPIClusterData : public Thread
{
public:
    MPIClusterData() 
        : Thread("MPIClusterData"),
          sleep_time(1),
          mpi_rank(-1), mpi_count(-1),
          global_recv_in_progress(false)
    {}
    
    ~MPIClusterData()
    {}
    
    void start(int ac, char **av)
    {
        argc = ac;
        argv = av;
        Thread::start();
    }
    
    void exec(int argc, char **argv);
    
    void shutdown();
    
    void send(int rank, const QByteArray &data);
    
    int rank() const
    {
        return mpi_rank;
    }
    
    int count() const
    {
        return mpi_count;
    }
    
    QString hostName() const
    {
        return mpi_hostname;
    }
    
protected:
    void threadMain()
    {
        this->exec(argc, argv);
    }

private:    
    bool checkForReceive();
    bool checkForSend();
    
    int argc;
    char **argv;
    
    MPI_Comm global_comm;
    
    MPI_Request global_request;
    int global_buffer;
    
    int sleep_time;

    /** Mutex used to protect the send queue */
    Mutex send_mutex;

    /** The queues used to receive messages
        (one per node in the MPI cluster, indexed
         by sender's MPI rank) */
    QVector<MPIRecvQueue> recv_queue;

    /** The queues used to send messages
        (one per node in the MPI cluster, indexed
         by recipient's MPI rank) */
    QVector<MPISendQueue> send_queue;
    
    /** The set of active senders */
    QSet<int> active_senders;
    
    /** The set of active recipents */
    QSet<int> active_recipients;

    /** The rank of this process in the MPI cluster */
    int mpi_rank;
    
    /** The number of nodes in the cluster */
    int mpi_count;

    /** The MPI hostname */
    QString mpi_hostname;

    /** Whether or not the global receive is in progress */
    bool global_recv_in_progress;
};

static char err_buffer[MPI_MAX_ERROR_STRING];

static void assertMPIOK(int mpierr, QUICK_CODELOC_ARGS)
{
    if (mpierr != MPI_SUCCESS)
    {
        int length;
        MPI_Error_string(mpierr, err_buffer, &length);
        
        throw SireCluster::network_error( QObject::tr(
                "MPI Error %5: \"%1\"\n"
                "The error occurred in %2, line %3, function %4.")
                    .arg(err_buffer)
                    .arg(file).arg(line).arg(current_function)
                    .arg(mpierr), CODELOC );
    }
}

Q_GLOBAL_STATIC( QMutex, mpiMutex );

////////
//////// Implementation of MPISendQueue
////////

bool MPISendQueue::process()
{
    return false;
}

////////
//////// Implementation of MPIRecvQueue
////////

bool MPIRecvQueue::process()
{
    return false;
}

////////
//////// Implementation of MPIClusterData
////////

bool MPIClusterData::checkForSend()
{
    if (active_senders.isEmpty())
        return false;

    QMutableSetIterator<int> it( active_senders );
    
    while (it.hasNext())
    {
        if (not send_queue[it.value()].process())
        {
            it.remove();
        }
    }

    return true;
}

bool MPIClusterData::checkForReceive()
{
    if (not global_recv_in_progress)
    {
        int mpierr = MPI_Irecv( &global_buffer, 1, MPI_INT, MPI_ANY_SOURCE,
                                size_tag, global_comm, &global_request );
        assertMPIOK(mpierr, QUICK_CODELOC);
        
        global_recv_in_progress = true;
    }
    
    //see if the receive has received anything
    MPI_Status status;
    int message_received;
    
    int mpierr = MPI_Test(&global_request, &message_received, &status);
    assertMPIOK(mpierr, QUICK_CODELOC);
    
    if (message_received)
    {
        //it has, it will be an integer giving the size of the buffer that
        //has been sent and should be received here and now
        global_recv_in_progress = false;
        MPI_Request_free( &global_request );

        //who sent the message?
        int sender = status.MPI_SOURCE;

        if (global_buffer == 0)
        {
            //there is no message to receive
            qDebug() << "Process" << mpi_rank << "received empty message from "
                     << "process" << sender;
        }
        else
        {
            if (sender < 0 or sender >= mpi_count)
                throw Siren::program_bug( QObject::tr(
                    "How did sender == %1 when mpi_count == %2 on node %3?")
                        .arg(sender).arg(mpi_count).arg(mpi_rank), CODELOC );
                        
            recv_queue[sender].receive(global_buffer);
            active_recipients.insert(sender);
        }
        
        mpierr = MPI_Irecv( &global_buffer, 1, MPI_INT, MPI_ANY_SOURCE,
                            size_tag, global_comm, &global_request );
        assertMPIOK(mpierr, QUICK_CODELOC);
        
        global_recv_in_progress = true;
    }
    
    //now process any receives
    QMutableSetIterator<int> it( active_recipients );
    
    while (it.hasNext())
    {
        if (not recv_queue[it.value()].process())
            it.remove();
    }
    
    return (message_received or (not active_recipients.isEmpty()));
}

void MPIClusterData::exec(int argc, char **argv)
{
    bool this_initialised_mpi = false;

    try
    {
        int mpi_initialized;
        int mpierr = MPI_Initialized(&mpi_initialized);
        assertMPIOK(mpierr, QUICK_CODELOC);
    
        if (not mpi_initialized)
        {
            mpierr = MPI_Init(&argc, &argv);
            assertMPIOK(mpierr, QUICK_CODELOC);
            this_initialised_mpi = true;
        }

        mpierr = MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
        assertMPIOK(mpierr, QUICK_CODELOC);

        mpierr = MPI_Comm_size(MPI_COMM_WORLD, &mpi_count);
        assertMPIOK(mpierr, QUICK_CODELOC);
        
        {
            QByteArray hostname_buffer;
            hostname_buffer.resize( MPI_MAX_PROCESSOR_NAME );
            
            int length;
            
            mpierr = MPI_Get_processor_name(hostname_buffer.data(), &length);
            assertMPIOK(mpierr, QUICK_CODELOC);
            
            if (mpi_count == 1)
                mpi_hostname = QLatin1String(hostname_buffer.constData());
            else
            {
                mpi_hostname = QString("%1:{MPI %2 of %3}")
                                    .arg(hostname_buffer.constData())
                                    .arg(mpi_rank+1)
                                    .arg(mpi_count);
            }
        }
        
        if (mpi_count == 1)
        {
            //there is no need for any of this!
            if (this_initialised_mpi)
                MPI_Finalize();
            
            Thread::signalStarted();
            
            return;
        }

        //copy the global communicator
        {
            MPI_Group mpigroup;

            try
            {
                mpierr = MPI_Comm_group(MPI_COMM_WORLD, &mpigroup);
                assertMPIOK(mpierr, QUICK_CODELOC);
                mpierr = MPI_Comm_create(MPI_COMM_WORLD, mpigroup, &global_comm);
                assertMPIOK(mpierr, QUICK_CODELOC);
                MPI_Group_free(&mpigroup);
            }
            catch(...)
            {
                MPI_Group_free(&mpigroup);
                throw;
            }
        }

        //create the send and receive queues
        {
            MutexLocker lkr( &send_mutex );
        
            send_queue = QVector<MPISendQueue>(mpi_count);
            recv_queue = QVector<MPIRecvQueue>(mpi_count);

            active_senders.reserve(mpi_count);
            active_recipients.reserve(mpi_count);

            //copy the global communicator to the queues
            for (int i=0; i<mpi_count; ++i)
            {
                send_queue[i].set(i, global_comm);
                recv_queue[i].set(i, global_comm);
            }
        }

        //now exchange all host information with all other nodes
        //in the cluster - this will be acheived via mpi_count
        //broadcasts - we assume that the MPI network is 
        //secure and that man-in-the-middle is not possible...
        for (int i=0; i<mpi_count; ++i)
        {
            if (i == mpi_rank)
            {
                //serialise the HostInfo for this process...
                QByteArray hostinfo;
                {
                    DataStream ds(hostinfo);
                    ds << Communicator::getLocalInfo();
                }
                
                //broadcast the size of the hostinfo object
                int size = hostinfo.count();
                mpierr = MPI_Bcast(&size, 1, MPI_INT, i, global_comm);
                assertMPIOK(mpierr, QUICK_CODELOC);
                
                //now broadcast the data
                mpierr = MPI_Bcast(hostinfo.data(), size, MPI_CHAR, i, global_comm);
                assertMPIOK(mpierr, QUICK_CODELOC);
            }
            else
            {
                //receive the hostinfo from the ith process - first the size
                int size;
                mpierr = MPI_Bcast(&size, 1, MPI_INT, i, global_comm);
                assertMPIOK(mpierr, QUICK_CODELOC);
                
                QByteArray hostinfo_data;
                hostinfo_data.resize(size);
                
                mpierr = MPI_Bcast(hostinfo_data.data(), size, MPI_CHAR, i, global_comm);
                assertMPIOK(mpierr, QUICK_CODELOC);
                
                //extract the data
                HostInfo hostinfo;
                DataStream ds(hostinfo_data);
                ds >> hostinfo;
                
                Communicator::addNeighbour(hostinfo,
                                           boost::bind(MPICluster::send, i, _1));
            }
        }

        //ok - signal that the MPI thread has now fully started
        Thread::signalStarted();
    
        while (for_ages())
        {
            bool received = checkForReceive();
            bool sent = checkForSend();
        
            if (received or sent)
            {
                sleep_time = 1;
            }
            else
            {
                Siren::msleep(sleep_time);
        
                ++sleep_time;
                sleep_time = qMin(sleep_time, 1000);
            }
        }
    }
    catch(const Siren::interupted&)
    {}
    catch(...)
    {
        if (this_initialised_mpi)
            MPI_Finalize();
            
        throw;
    }
    
    if (this_initialised_mpi)
    {
        MPI_Finalize();
    }
}

void MPIClusterData::shutdown()
{
    end_for_ages(this);
}

void MPIClusterData::send(int rank, const QByteArray &data)
{
    if (data.isEmpty())
        return;
        
    if (rank < 0 or rank >= mpi_count)
        throw Siren::unavailable_resource( QObject::tr(
                "There is no MPI node in this cluster with rank %1. "
                "Available nodes have ranks 0 to %2.")
                    .arg(rank).arg(mpi_count-1), CODELOC );
                    
    if (rank == mpi_rank)
    {
        //we are sending the message to ourselves :-)
        Communicator::received(data);
    }
    else
    {
        MutexLocker lkr( &send_mutex );
        send_queue[rank].send(data);
        active_senders.insert(rank);
    }
}

/////////////
///////////// Implementation of MPICluster
/////////////

static MPIClusterData *global_cluster(0);

void MPICluster::exec(int argc, char **argv)
{
    QMutexLocker lkr( mpiMutex() );
    
    if (not global_cluster)
    {
        global_cluster = new MPIClusterData();        
        global_cluster->exec(argc, argv);
    }
}

void MPICluster::start(int argc, char **argv)
{
    QMutexLocker lkr( mpiMutex() );
    
    if (not global_cluster)
    {
        global_cluster = new MPIClusterData();
        global_cluster->start(argc, argv);
    }
}

void MPICluster::send(int rank, const QByteArray &message)
{
    QMutexLocker lkr( mpiMutex() );
    
    if (not global_cluster)
        throw Siren::program_bug( QObject::tr(
                "Cannot send an MPI message as MPICluster has not been started!"),
                    CODELOC );
    
    lkr.unlock();
    
    global_cluster->send( rank, message );
}

int MPICluster::rank()
{
    QMutexLocker lkr( mpiMutex() );
    
    if (global_cluster)
        return global_cluster->rank();
    else
        return 0;
}

QString MPICluster::hostName()
{
    QMutexLocker lkr( mpiMutex() );
    
    if (global_cluster)
    {
        return global_cluster->hostName();
    }
    else
        return "localhost";
}

void MPICluster::shutdown()
{
    QMutexLocker lkr( mpiMutex() );
    
    if (global_cluster)
    {
        global_cluster->shutdown();
        global_cluster->wait();

        delete global_cluster;
        global_cluster = 0;
    }
}

#endif // SIRE_USE_MPI
