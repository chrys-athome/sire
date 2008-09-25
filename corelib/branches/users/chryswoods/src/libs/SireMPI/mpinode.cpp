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

#include <boost/weak_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include <QTextStream>

#include "mpinode.h"
#include "mpinodes.h"
#include "mpipromise.h"
#include "mpiworker.h"

#include "detail/mpidetail.h"     // CONDITIONAL_INCLUDE

#include "SireStream/streamdata.hpp"
#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

#include "tostring.h"

#include <QDebug>

using namespace SireMPI;
using namespace SireMPI::detail;

using namespace SireStream;

using boost::shared_ptr;
using boost::tuple;

static const int SHUTDOWN_MPI = 0;
static const int START_MPI_BACKEND = 1;
static const int STOP_MPI_BACKEND = 2;

//////////
////////// Implementation of MPIBackend
//////////

#ifdef __SIRE_USE_MPI__

    namespace SireMPI
    {

    /** Global mutex to protect access to starting and stopping
        the global SireMPI event loop */
    static QMutex *exec_mutex_ptr(0);

    static QMutex exec_mutex_mutex;

    static QMutex& exec_mutex()
    {
        if (exec_mutex_ptr == 0)
        {
            QMutexLocker lkr(&exec_mutex_mutex);
            
            if (exec_mutex_ptr == 0)
                exec_mutex_ptr = new QMutex();
        }
        
        return *exec_mutex_ptr;
    }

    /** This variable is used to see if an MPI loop (from SireMPI::exec) is 
        currently running */
    static bool exec_is_running(false);

    /** Pointer to the global communicator used by SireMPI */
    static MPI::Comm *comm_world(0);
    
    MPI::Comm& COMM_WORLD()
    {
        BOOST_ASSERT( comm_world != 0 );
        return *comm_world;
    }

    class MPIBackend : private QThread
    {
    public:
        ~MPIBackend();

        using QThread::start;

        static void start(MPI::Comm *mpicom, int master, int tag);
        static void stop(MPI::Comm *mpicom, int master, int tag);

        static void stopAll();

    protected:
        MPIBackend(MPI::Comm *mpicom, int master_rank, int mpitag);
    
        void run();
    
        void sendResponse(int tag, const MPIWorker &worker);
        void runJob(MPIWorker &worker);
    
        static QHash< int, shared_ptr<MPIBackend> >& registry();
        static QList< shared_ptr<MPIBackend> >& stopped_backends();
    
        static QHash< int, shared_ptr<MPIBackend> > *registry_ptr;
        static QList< shared_ptr<MPIBackend> > *stopped_backends_ptr;
    
        static QMutex *registry_mutex_ptr;
        
        static QMutex& registry_mutex();
    
        /** The communicator used to talk to the master */
        MPI::Comm *mpicom;
    
        /** The rank of the master process in the communicator */
        int master_rank;
        
        /** The tag that will be used for all communication */
        int mpitag;
    
        /** flag used to stop the backend */
        bool keep_running;
    };

    /** Mutex to protect access to the registry */
    QMutex* MPIBackend::registry_mutex_ptr(0);

    QMutex& MPIBackend::registry_mutex()
    {
        if (registry_mutex_ptr == 0)
        {
            QMutexLocker lkr(&exec_mutex_mutex);
            
            if (registry_mutex_ptr == 0)
                registry_mutex_ptr = new QMutex();
        }
        
        return *registry_mutex_ptr;
    }

    /** The registry of all active backends on this node */
    QHash< int, shared_ptr<MPIBackend> >* MPIBackend::registry_ptr(0);
    
    QHash< int, shared_ptr<MPIBackend> >& MPIBackend::registry()
    {
        QMutexLocker lkr(&registry_mutex());
        
        if (registry_ptr == 0)
            registry_ptr = new QHash< int, shared_ptr<MPIBackend> >();
            
        return *registry_ptr;
    }
    
    /** The list of backends that have been scheduled to be stopped */
    QList< shared_ptr<MPIBackend> >* MPIBackend::stopped_backends_ptr(0);
    
    QList< shared_ptr<MPIBackend> >& MPIBackend::stopped_backends()
    {
        QMutexLocker lkr(&registry_mutex());
        
        if (stopped_backends_ptr == 0)
            stopped_backends_ptr = new QList< shared_ptr<MPIBackend> >();
            
        return *stopped_backends_ptr;
    }
    
    /** Constructor, telling the background the rank of the master
        process, the communicator used to communicate with that
        process, and the  */
    MPIBackend::MPIBackend(MPI::Comm *communicator, int rank, int tag) 
               : QThread(), mpicom(communicator), master_rank(rank), mpitag(tag)
    {
        qDebug() << "Creating backend with communicator" << communicator;
    }
    
    /** Destructor */
    MPIBackend::~MPIBackend()
    {
        keep_running = false;
        
        //give the thread 10 seconds to exit
        if (not QThread::wait(10000))
        {
            QThread::terminate();
        }
    }
    
    /** Small function used to send the MPIWorker back to the master */
    void MPIBackend::sendResponse(int tag, const MPIWorker &worker)
    {
        int envelope[3];
        envelope[0] = tag;

        QByteArray worker_data;
        double current_progress = 0;

        if (tag == MPIWORKER_RESULT or tag == MPIWORKER_ABORT )
        {
            envelope[2] = 0;
        }
        else
        {
            envelope[2] = 1;
            current_progress = worker.progress();
        }

        if (tag == MPIWORKER_PROGRESS or tag == MPIWORKER_ABORT)
        {
            envelope[1] = 0;
        }
        else
        {
            worker_data = SireStream::save(worker);
            envelope[1] = worker_data.size();
        }
        
        //send the envelope describing the response
        qDebug() << comm_world->Get_rank() << master_rank << mpitag  
                 << "backend: mpicom->Send(envelope, 3, MPI::INT, master_rank, mpitag);";
        mpicom->Send(envelope, 3, MPI::INT, master_rank, mpitag);
        qDebug() << comm_world->Get_rank()  
                 << "backend: SENT!";
        
        if (envelope[1] > 0)
        {
            qDebug() << comm_world->Get_rank() << master_rank << mpitag
                 << "backend: mpicom->Send(worker_data.constData(), worker_data.size(), MPI::BYTE, master_rank, mpitag);";
            mpicom->Send(worker_data.constData(), worker_data.size(), MPI::BYTE,
                         master_rank, mpitag);
            qDebug() << comm_world->Get_rank()  
                 << "backend: SENT!";
        }
        
        if (envelope[2] > 0)
        {
            qDebug() << comm_world->Get_rank() << master_rank << mpitag
                 << "backend: mpicom->Send(&current_progress, 1, MPI::DOUBLE, master_rank, mpitag);";
            mpicom->Send(&current_progress, 1, MPI::DOUBLE, master_rank, mpitag);
            qDebug() << comm_world->Get_rank()  
                 << "backend: SENT!";
        }
    }
    
    void MPIBackend::runJob(MPIWorker &worker)
    {
        try
        {
            while (not worker.hasFinished())
            {
                //do we have an instruction?
                if (mpicom->Iprobe(master_rank, mpitag))
                {
                    //yes there is - receive it
                    int envelope[3];
                    qDebug() << comm_world->Get_rank() << master_rank << mpitag  
                 << "backend: mpicom->Recv(envelope, 3, MPI::INT, master_rank, mpitag);";
                    mpicom->Recv(envelope, 3, MPI::INT, master_rank, mpitag);
                    qDebug() << comm_world->Get_rank()  
                 << "backend: RECEIVED!";

                    if (envelope[1] > 0)
                    {
                        //there is an accompanying bytearray package
                        QByteArray worker_data(envelope[1]+1, ' ');
                        qDebug() << comm_world->Get_rank() << master_rank << mpitag
                 << "backend: mpicom->Recv(worker_data.data(), envelope[1], MPI::BYTE, master_rank, mpitag);";
                        mpicom->Recv(worker_data.data(), envelope[1], MPI::BYTE,
                                     master_rank, mpitag);
                        qDebug() << comm_world->Get_rank()  
                 << "backend: RECEIVED!";
                    }
                    
                    if (envelope[2] > 0)
                    {
                        //there is an accompanying double...
                        double current_progress;
                        qDebug() << comm_world->Get_rank() << master_rank << mpitag 
                 << "backend: mpicom->Recv(&current_progress, 1, MPI::DOUBLE, master_rank, mpitag);";
                        mpicom->Recv(&current_progress, 1, MPI::DOUBLE, master_rank, 
                                     mpitag);
                        qDebug() << comm_world->Get_rank()  
                 << "backend: RECEIVED!";
                    }
                
                    switch (envelope[0])
                    {
                        case MPIWORKER_STOP:
                            this->sendResponse(MPIWORKER_STOP, worker);
                            return;
                        
                        case MPIWORKER_ABORT:
                            this->sendResponse(MPIWORKER_ABORT, worker);
                            return;
                            
                        case MPIWORKER_PROGRESS:
                            this->sendResponse(MPIWORKER_PROGRESS, worker);
                            break; 
                            
                        case MPIWORKER_INTERIM:
                            this->sendResponse(MPIWORKER_INTERIM, worker);
                            break;
                    }
                }

                //now run a chunk of work
                worker.runChunk();
                qDebug() << "Progress == " << worker.progress() << "%";
            }
            
            this->sendResponse(MPIWORKER_RESULT, worker);
            return;
        }
        catch(const SireError::exception &e)
        {
            this->sendResponse( MPIWORKER_STOP, MPIError(e) );
        }
        catch(const std::exception &e)
        {
            this->sendResponse( MPIWORKER_STOP, MPIError(e) );
        }
        catch(...)
        {
            this->sendResponse( MPIWORKER_STOP,
                                    MPIError(SireError::unknown_exception( QObject::tr(
                                        "An unknown error has occurred!"), CODELOC ) ) );
        }
    }
    
    /** This is the function run by the MPI backend that actually processes
        the MPIWorker object that is sent by the master node, and performs
        the work contained in that worker. Once the work is complete, it
        then repacks that worker and sends it back to the master */
    void MPIBackend::run()
    {
        qDebug() << comm_world->Get_rank()  
                 << " ** STARTING BACKGROUND WORKER THREAD ** ";
    
        try
        {
            do
            {
                //is there a message to receive?
                qDebug() << mpicom << "->Iprobe(" << master_rank << mpitag << ")";
                
                if (mpicom->Iprobe(master_rank, mpitag))
                {
                    //yes there is - receive it
                    int envelope[3];
                    qDebug() << comm_world->Get_rank() << master_rank << mpitag
                     << "backend_run: mpicom->Recv(envelope, 3, MPI::INT, master_rank, mpitag);";
                    mpicom->Recv(envelope, 3, MPI::INT, master_rank, mpitag);
                    qDebug() << comm_world->Get_rank()  
                     << "backend_run: RECEIVED!";

                    QByteArray worker_data;

                    if (envelope[1] > 0)
                    {
                        //there is an accompanying bytearray package
                        worker_data = QByteArray(envelope[1]+1, ' ');
                        qDebug() << comm_world->Get_rank() << master_rank << mpitag
                     << "backend_run: mpicom->Recv(worker_data.data(), envelope[1], MPI::BYTE, master_rank, mpitag);";
                        mpicom->Recv(worker_data.data(), envelope[1], MPI::BYTE,
                                     master_rank, mpitag);
                        qDebug() << comm_world->Get_rank()  
                     << "backend_run: RECEIVED!";
                    }

                    if (envelope[2] > 0)
                    {
                        //there is an accompanying double...
                        double current_progress;
                        qDebug() << comm_world->Get_rank() << master_rank << mpitag
                     << "backend_run: mpicom->Recv(&current_progress, 1, MPI::DOUBLE, master_rank, mpitag);";
                        mpicom->Recv(&current_progress, 1, MPI::DOUBLE, master_rank, 
                                     mpitag);
                        qDebug() << comm_world->Get_rank()  
                     << "backend_run: RECEIVED!";
                    }

                    if (envelope[0] != MPIWORKER_START)
                    {
                        //this is an old message from a previous job - ignore it
                        continue;
                    }
                           
                    //extract the worker
                    tuple< shared_ptr<void>,QString > object 
                                                = SireStream::load(worker_data);
                                                
                    //assume that this is the right type
                    MPIWorker *worker = static_cast<MPIWorker*>( object.get<0>().get() );

                    if (worker)
                        this->runJob(*worker);
                }
                else
                {
                    //wait a little before checking again
                    if (keep_running)
                        QThread::msleep(5000);
                }

            } while (keep_running);
        }
        catch(const SireError::exception &e)
        {
            SireMPI::writeErrorString(CODELOC, e);
        }
        catch(const std::exception &e)
        {
            SireMPI::writeErrorString(CODELOC, SireError::std_exception(e));
        }
        catch(...)
        {
            SireMPI::writeErrorString(CODELOC, 
                    SireError::unknown_exception("An unknown error occured!", CODELOC));
        }
    }

    /** Start a backend */
    void MPIBackend::start(MPI::Comm *mpicom, int master, int tag)
    {
        //we shouldn't have this tag already...
        if (registry().contains(tag))
            throw SireError::program_bug( QObject::tr(
                "It should not be possible to start two backends with the "
                "same tag (%1)").arg(tag), CODELOC );
    
        //create a backend
        shared_ptr<MPIBackend> backend( new MPIBackend(mpicom, master, tag) );
        
        //start the thread
        backend->start();
        
        //add it to the registry
        registry().insert( tag, backend );
    }
    
    /** Stop the backend with the specified tag */
    void MPIBackend::stop(MPI::Comm *mpicom, int master, int tag)
    {
        shared_ptr<MPIBackend> backend = registry().take(tag);
        
        if (backend.get() != 0)
        {
            if (mpicom != backend->mpicom)
                qWarning() << CODELOC << "\n\nMix up in MPI::Comm at shutdown...";
                
            if (master != backend->master_rank)
                qWarning() << CODELOC 
                           << "\n\nMix up in MPI rank at shutdown...";

            //stop the backend
            backend->keep_running = false;
            
            //add this backend to the list of ones that have been stopped
            stopped_backends().append(backend);
        }
    }

    /** Stop all of the backend loops that are running on this node */
    void MPIBackend::stopAll()
    {
        //stop all backends
        QList<int> tags = registry().keys();
        
        foreach (int tag, tags)
        {
            shared_ptr<MPIBackend> backend = registry().take(tag);
            backend->keep_running = false;
            
            stopped_backends().append(backend);
        }
        
        //now wait for them all to finish
        while (not stopped_backends().isEmpty())
        {
            shared_ptr<MPIBackend> backend = stopped_backends().takeFirst();
            backend->wait();
        }
    }

    /** Write an error string to standard output - this is used by background
        threads when they die horribly! */
    void writeErrorString(const QString &location, const SireError::exception &e)
    {
        QTextStream ts(stdout);
        
        ts << QObject::tr(" ** FATAL ERROR ON NODE %1 **\n"
                          " ** LOCATION %2 **\n%3\n")
                    .arg(comm_world->Get_rank())
                    .arg(location, e.toString());
    }

    /** Return whether SireMPI::exec is running */
    bool SIREMPI_EXPORT exec_running()
    {
        QMutexLocker lkr( &exec_mutex() );
        return exec_is_running;
    }

    /** This function starts MPI::Init_thread if MPI is not yet initialized,
        and checks that the required level of thread support is available. */
    void ensureInitializedMPI(int &argc, char **argv)
    {
        QMutexLocker lkr( &exec_mutex() );

        if (not MPI::Is_initialized())
        {
            int thread_support = MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);
            
            QString support_level;
            
            switch (thread_support)
            {
                case MPI_THREAD_MULTIPLE:
                    break;
            
                case MPI_THREAD_SINGLE:
                    support_level = QObject::tr(
                        "MPI_THREAD_SINGLE - Only one thread will execute");
                    break;
                    
                case MPI_THREAD_FUNNELED:
                    support_level = QObject::tr(
                        "MPI_THREAD_FUNNELED - Only the thread that called "
                        "MPI_Init_thread will make MPI calls");
                    break;
                    
                case MPI_THREAD_SERIALIZED:
                    support_level = QObject::tr(
                        "MPI_THREAD_SERIALIZED - Only one thread will make MPI "
                        "library calls at one time.");
                    break;
                    
                default:
                    support_level = QObject::tr(
                        "??? - Unknown MPI thread support level");
                    break;
            }
            
            if (thread_support != MPI_THREAD_MULTIPLE)
            {
                qWarning() << "Required level of MPI thread support is not available."
                           << "The available level is just" << support_level;
            }
        }
    }

    /** This is the main function that you must call to allow SireMPI
        to work in your process. You must only call this once in a process
        - you cannot call it several times!!! */
    int SIREMPI_EXPORT exec(int &argc, char **argv)
    {
        exec_mutex().lock();
    
        if (exec_is_running)
        {
            exec_mutex().unlock();
            return 0;
        }
        else
        {
            exec_is_running = true;
            exec_mutex().unlock();
        }

        SireMPI::ensureInitializedMPI(argc, argv);
                
        //now create a new communicator, used to keep
        //communication within SireMPI private
        exec_mutex().lock();
        SireMPI::comm_world = &(MPI::COMM_WORLD.Clone());
        qDebug() << "CLONE COMM_WORLD" << SireMPI::comm_world;
        exec_mutex().unlock();
                
        //now go into a loop waiting for instructions
        bool keep_looping = true;
    
        do
        {   
            try
            {
                //listen for any messages on channel 1
                MPI::Status status;
                
                if (comm_world->Iprobe(MPI_ANY_SOURCE, 1, status))
                {
                    if (status.Get_count(MPI::INT) != 2)
                        continue;
                    
                    int master_rank = status.Get_source();
                
                    qDebug() << comm_world->Get_rank() << master_rank << 1
                        << "comm_world->Recv( message, 2, MPI::INT, MPI_ANY_SOURCE, 1, status );";
                    
                    int message[2];
                    comm_world->Recv( message, 2, MPI::INT, master_rank, 1, status );
                                          
                    qDebug() << comm_world->Get_rank()  
                             << "RECEIVED!";
        
                    //what is the message?
                    switch( message[0] )
                    {
                        case START_MPI_BACKEND:
                            //start a new MPI backend loop that listens to instructions
                            //on the specified channel from the specified master
                            MPIBackend::start( &(SireMPI::COMM_WORLD()),
                                               master_rank, message[1] );
                            break;
                
                        case STOP_MPI_BACKEND:
                            //stop the specified MPI backend loop
                            MPIBackend::stop( &(SireMPI::COMM_WORLD()),
                                              master_rank, message[1] );
                            break;
                        
                        case SHUTDOWN_MPI:
                            //completely shut down MPI on this node
                            MPIBackend::stopAll();
                            keep_looping = false;
                            break;
                        
                        default:
                            qWarning() << "Could not interpret MPI instruction" << message[0];
                            break;
                    }
                }
                else
                {
                    //spend a couple of seconds sleeping
                    sleep(2);
                }
            }
            catch(const SireError::exception &e)
            {
                SireMPI::writeErrorString(CODELOC, e);
            }
            catch(const std::exception &e)
            {
                SireMPI::writeErrorString(CODELOC, SireError::std_exception(e));
            }
            catch(...)
            {
                SireMPI::writeErrorString(CODELOC, 
                        SireError::unknown_exception("An unknown error occured!", 
                                                      CODELOC));
            }
    
        } while (keep_looping);
    
        exec_mutex().lock();

        //ok, we have finished - destroy the communicator
        qDebug() << comm_world->Get_rank()  
                 << "MPI::Free()";
        comm_world->Free();
        delete comm_world;
        comm_world = 0;

        exec_is_running = false;
        exec_mutex().unlock();
    
        return 0;
    }

    /** This class private class is used to run SireMPI::exec in a background
        thread - this is useful for running it as a background thread of
        the master process */
    class SireMPI_ExecRunner : private QThread
    {
    public:
        SireMPI_ExecRunner(int &argc, char **argv) : QThread()
        {
            SireMPI::ensureInitializedMPI(argc, argv);
            this->start();
        }
    
        ~SireMPI_ExecRunner()
        {
            //now shut the MPI nodes down
            SireMPI::shutdown();
            this->wait();
        }
    
    protected:
        void run()
        {
            try
            {
                int argc = 0;
                SireMPI::exec(argc,0);
                return;
            }
            catch(const SireError::exception &e)
            {
                SireMPI::writeErrorString(CODELOC, e);
            }
            catch(const std::exception &e)
            {
                SireMPI::writeErrorString(CODELOC, SireError::std_exception(e));
            }
            catch(...)
            {
                SireMPI::writeErrorString(CODELOC, 
                        SireError::unknown_exception("An unknown error occured!", 
                                                        CODELOC));
            }
        }
    };

    static shared_ptr<SireMPI_ExecRunner> exec_runner;

    /** Call SireMPI::exec, but set it running in a background thread. You should
        really only do this on the master node */
    void SIREMPI_EXPORT bg_exec(int &argc, char **argv)
    {
        //use an object to start and stop the loop
        //(as hopefully the object will be deleted when the library
        // exits, and thus an MPI shutdown will be called)
        exec_runner.reset( new SireMPI_ExecRunner(argc, argv) );
    }

    /** Call this at the end of the program (on the master node) to shutdown
        all of the MPI nodes */
    void SIREMPI_EXPORT shutdown()
    {
        if (exec_running())
        {
            QMutexLocker lkr(&exec_mutex());
        
            //shutdown each node (except ourselves!)
            int nnodes = comm_world->Get_size();
            int my_rank = comm_world->Get_rank();
            
            int message[2];
            message[0] = SHUTDOWN_MPI;
            message[1] = 0;
            
            for (int i=0; i<nnodes; ++i)
            {
                if (i != my_rank)
                {
                    //received in SireMPI::exec()
                    // 'comm_world->Recv( message, 2, MPI_ANY_SOURCE, 1, status );'
                    qDebug() << comm_world->Get_rank() << i << 1  
                             << "(1) comm_world->Send(message, 2, MPI::INT, i, 1);";
                    comm_world->Send(message, 2, MPI::INT, i, 1);
                    qDebug() << comm_world->Get_rank()  
                             << "(1) SENT!";
                }
            }
            
            //now shutdown this node
            //received in SireMPI::exec()
            // comm_world->Recv( message, 2, MPI_ANY_SOURCE, 1, status );'
            qDebug() << comm_world->Get_rank() << my_rank << 1  
                     << "(2) comm_world->Send(message, 2, MPI::INT, my_rank, 1);";
            comm_world->Send(message, 2, MPI::INT, my_rank, 1);
            qDebug() << comm_world->Get_rank()  
                     << "(2) SENT!";
            
            exec_is_running = false;
        }
    }

    } // end of namespace SireMPI

#else

    #error Need to write non-mpi versions!!!

#endif

//////////
////////// Implementation of MPINodeData
//////////

/** Null constructor */
MPINodeData::MPINodeData() : QThread(), is_master(false)
{
    #ifdef __SIRE_USE_MPI__
        mpirank = 0;
    #endif
}

/** Function used to start the code necessary on the backend node
    so that this processor can communicate with the backend */
void MPINodeData::startBackend()
{
    #ifdef __SIRE_USE_MPI__
        //tell the node to start a new backend loop using the provided
        //tag for all communication

        boost::shared_ptr<MPINodesData> communicator = parent.lock();
    
        BOOST_ASSERT( communicator.get() != 0 );

        MPI::Comm *mpicomm = communicator->mpiCommunicator();

        int message[2];
        message[0] = START_MPI_BACKEND;
        message[1] = communicator->mpiTag();

        //send the message - this is received in SireMPI::exec()
        // 'comm->world->Recv( message, 2, MPI_ANY_SOURCE, 1, status );'
        qDebug() << comm_world->Get_rank() << mpirank << 1  
                 << "mpicomm->Send(message, 2, MPI::INT, mpirank, 1);";
        mpicomm->Send(message, 2, MPI::INT, mpirank, 1); 
        qDebug() << comm_world->Get_rank()  
                 << "SENT!!!";

    #else
        #error Need to write this
    #endif
}

/** This function tells the backend node that we won't be communicating any more,
    so to stop the backend loop */
void MPINodeData::stopBackend()
{
    #ifdef __SIRE_USE_MPI__
        //tell the node to stop the backend loop

        boost::shared_ptr<MPINodesData> communicator = parent.lock();
    
        BOOST_ASSERT( communicator.get() != 0 );

        MPI::Comm *mpicomm = communicator->mpiCommunicator();

        int message[2];
        message[0] = STOP_MPI_BACKEND;
        message[1] = communicator->mpiTag();

        //received in SireMPI::exec()
        // 'comm_world->Recv( message, 2, MPI_ANY_SOURCE, 1, status );'
        qDebug() << comm_world->Get_rank() << mpirank << 1  
                 << "mpicomm->Send(message, 2, MPI::INT, mpirank, 1);";
        mpicomm->Send(message, 2, MPI::INT, mpirank, 1); 
        qDebug() << comm_world->Get_rank()  
                 << "SENT!";
    
    #else
        #error Need to write this
    
    #endif
}

/** This constructor is called only by the MPINodesData communicator, and is
    used to create a new MPINode with specified communicator, processor rank
    in that communicator, and whether or not this is the master processor
    for the communicator */
MPINodeData::MPINodeData(const boost::shared_ptr<MPINodesData> &communicator, 
                         int processor_rank, bool ismaster)
            : QThread(),
              parent(communicator),
              uid( QUuid::createUuid() ), 
              is_master(ismaster)
{
    BOOST_ASSERT( not parent.expired() );
    BOOST_ASSERT( processor_rank >= 0 );
    
    #ifdef __SIRE_USE_MPI__
        mpirank = processor_rank;
    #endif

    this->startBackend();
}

/** This copy constructor is used when the node is returned to the 
    list of free nodes in the communicator */
MPINodeData::MPINodeData(const MPINodeData &other)
            : QThread(),
              parent(other.parent), uid(other.uid),
              is_master(other.is_master)
{
    #ifdef __SIRE_USE_MPI__
        mpirank = other.mpirank;
    #endif
}

/** Destructor - this blocks until the node is not busy, and then
    it either completely deletes the node, or posts a new copy
    back to the list of free nodes in the communicator */
MPINodeData::~MPINodeData()
{
    //wait until this node is no longer busy...
    this->wait();

    boost::shared_ptr<MPINodesData> communicator = parent.lock();

    if (communicator.get() != 0)
    {
        //create a null copy of this data and return the node
        boost::shared_ptr<MPINodeData> data_copy( new MPINodeData(*this) );
        
        communicator->returnNode( MPINode(data_copy) );
    }
}

/** Return whether or not the node is busy (processing an MPIWorker) */
bool MPINodeData::isBusy() const
{
    return QThread::isRunning();
}

/** Block until the node is no longer busy */
void MPINodeData::wait()
{
    QThread::wait();
}

/** Block until the node is no longer busy, or time ms have passed.
    Returns whether or not the node is busy */
bool MPINodeData::wait(int time)
{
    return QThread::wait(time);
}

/** Start and run the job that is contained in the passed worker */
MPIPromise MPINodeData::start(const MPIWorker &worker)
{
    if (parent.expired() or uid.isNull())
        throw SireError::invalid_state( QObject::tr(
            "You cannot run any work on the null MPI node."), CODELOC );

    QMutexLocker lkr(&data_mutex);

    //wait until the node is no longer busy
    while (this->isBusy())
    {
        lkr.unlock();
        this->wait();
        lkr.relock();
    }

    //save the worker to be processed
    mpipromise = MPIPromise(worker, MPINode(self_ptr.lock()));

    //reset the aborted status
    is_aborted = false;

    //now start the thread that is used to start the job and wait
    //for responses from the backend node
    start_mutex.lock();
    QThread::start();
    start_waiter.wait( &start_mutex );
    start_mutex.unlock();
    
    return mpipromise;
}

/** Return whether or not the last job run on this node was aborted */
bool MPINodeData::wasAborted() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&data_mutex) );
    return is_aborted;
}

/** Small private class used to clean up MPINode objects that have finished
    their jobs. This is used so that the destructors are called in a dedicated
    thread (thus preventing the possibility that the destructor for a node
    will be called in its own thread - thereby leading to deadlock) */
class NodeDeleter : private QThread
{
public:
    ~NodeDeleter()
    {
        keep_running = false;
        this->wait();
    }
    
    static void scheduleForDeletion(const MPINode &node)
    {
        QMutexLocker lkr(&data_mutex);

        if (global_deleter.get() == 0)
            global_deleter.reset( new NodeDeleter() );

        global_deleter->delete_nodes.append(node);
    }
    
protected:
    NodeDeleter() : QThread()
    {
        keep_running = true;
        this->start();
    }
    
    void run()
    {
        try
        {
            while (keep_running)
            {
                QMutexLocker lkr(&data_mutex);
                
                //delete all of the nodes - this calls all 
                //of their destructors
                delete_nodes.clear();

                data_mutex.unlock();
                
                //only do this once every 5 seconds
                QThread::sleep(5);
            }
            
            //ok, we are exiting - clear any final nodes
            QMutexLocker lkr(&data_mutex);
            delete_nodes.clear();
        }
        catch(const SireError::exception &e)
        {
            SireMPI::writeErrorString(CODELOC, e);
        }
        catch(const std::exception &e)
        {
            SireMPI::writeErrorString(CODELOC, SireError::std_exception(e));
        }
        catch(...)
        {
            SireMPI::writeErrorString(CODELOC, 
                    SireError::unknown_exception("An unknown error occured!", CODELOC));
        }
    }
    
private:
    static shared_ptr<NodeDeleter> global_deleter;

    static QMutex data_mutex;

    QList<MPINode> delete_nodes;
    bool keep_running;
};

shared_ptr<NodeDeleter> NodeDeleter::global_deleter;
QMutex NodeDeleter::data_mutex;

/** This is what is run in the background to send a job to the backend 
    node and to wait for a response */
void MPINodeData::run()
{
    //tell the parent thread that we are now definitely running
    start_mutex.lock();
    start_waiter.wakeAll();
    start_mutex.unlock();
    
    //copy the promise from the parent thread into a thread-local copy
    MPIPromise local_promise;
    
    {
        QMutexLocker lkr(&data_mutex);
        is_aborted = false;
        
        if (mpipromise.isNull())
            return;
            
        local_promise = mpipromise;
        
        if (is_aborted)
        {
            //we've been aborted before even starting!
            local_promise.setAborted();
        }
    }

    //the job is running the right node, isn't it...?
    BOOST_ASSERT( local_promise.node().UID() == this->uid );

    //take a copy of this node to ensure that it is not deleted during
    //the job
    MPINode this_node = local_promise.node();

    try
    {
        //now send the data for the worker to the backend node
        #ifdef __SIRE_USE_MPI__
        
            shared_ptr<MPINodesData> communicator = parent.lock();
            BOOST_ASSERT( communicator.get() != 0 );
            
            MPI::Comm *mpicom = communicator->mpiCommunicator();
            int mpitag = communicator->mpiTag();
            
            BOOST_ASSERT( mpicom != 0 );
            BOOST_ASSERT( mpitag > 1 );
            
            QByteArray worker_data = local_promise.initialData();
            
            //received by MPIBackend::run()
            int message[3];
            message[0] = MPIWORKER_START;
            message[1] = worker_data.size();
            message[2] = 0;

            qDebug() << comm_world->Get_rank() << mpirank << mpitag  
                     << "NodeData:: mpicom->Send( message, 3, MPI::INT, mpirank, mpitag );";
            mpicom->Send( message, 3, MPI::INT, mpirank, mpitag );
            qDebug() << comm_world->Get_rank()  
                     << "NodeData:: SENT!";

            qDebug() << comm_world->Get_rank() << mpirank << mpitag  
                     << "NodeData:: mpicom->Send( worker_data.constData(), worker_data.size(), MPI::BYTE, mpirank, mpitag );";
            mpicom->Send( worker_data.constData(), worker_data.size(), MPI::BYTE,
                          mpirank, mpitag );
            qDebug() << comm_world->Get_rank()  
                     << "NodeData:: SENT!";
        
        #else
            #error Need to write this!
            
        #endif

        //now wait for the work to be complete - wait for the result
        //to be sent back
        
        #ifdef __SIRE_USE_MPI__
        
            bool keep_running = true;
            
            do
            {
                qDebug() << comm_world->Get_rank() << mpirank << mpitag  
                     << "NodeData:: mpicom->Recv( message, 3, MPI::INT, mpirank, mpitag )";
                mpicom->Recv( message, 3, MPI::INT, mpirank, mpitag );
                qDebug() << comm_world->Get_rank()  
                     << "NodeData:: RECEIVED!";
                
                QByteArray result_data;
            
                if (message[1] > 0)
                {
                    //there is an accompanying QByteArray of the specfied size
                    result_data = QByteArray(message[1]+1, ' ');
                    qDebug() << comm_world->Get_rank() << mpirank << mpitag
                     << "NodeData:: mpicom->Recv( result_data.data(), message[1], MPI::BYTE, mpirank, mpitag );";
                    mpicom->Recv( result_data.data(), message[1], MPI::BYTE, 
                                  mpirank, mpitag );
                    qDebug() << comm_world->Get_rank()  
                     << "NodeData:: RECEIVED!";
                }
            
                double current_progress = 0;
            
                if (message[2] > 0)
                {
                    //the progress data is being sent as well
                    qDebug() << comm_world->Get_rank() << mpirank << mpitag
                     << "NodeData:: mpicom->Recv( &current_progress, 1, MPI::DOUBLE, mpirank, mpitag );";
                    mpicom->Recv( &current_progress, 1, MPI::DOUBLE, mpirank, mpitag );
                    qDebug() << comm_world->Get_rank()  
                     << "NodeData:: RECEIVED!";
                }
            
                switch( message[0] )
                {
                    case MPIWORKER_STOP:
                        //the result from a stopped job has been returned
                        local_promise.setStopped(result_data, current_progress);
                        keep_running = false;
                        break;
                
                    case MPIWORKER_ABORT:
                        //the job has been aborted
                        local_promise.setAborted();
                        keep_running = false;
                        break;
                        
                    case MPIWORKER_PROGRESS:
                        //just a progress update
                        local_promise.setProgress(current_progress);
                        break;
                        
                    case MPIWORKER_INTERIM:
                        //an interim result
                        local_promise.setInterimData(result_data, current_progress);
                        break;
                     
                    case MPIWORKER_RESULT:
                        //the final result!
                        local_promise.setFinalData(result_data);
                        keep_running = false;
                        break;
                }
            
            } while (keep_running);

        #else
            #error Need to write this...
        #endif
    }
    catch(const SireError::exception &e)
    {
        SireMPI::writeErrorString(CODELOC, e);
    }
    catch(const std::exception &e)
    {
        SireMPI::writeErrorString(CODELOC, SireError::std_exception(e));
    }
    catch(...)
    {
        SireMPI::writeErrorString(CODELOC, 
                SireError::unknown_exception("An unknown error occured!", CODELOC));
    }

    //we've now finished - we currently hold a pointer to ourselves,
    //so this will need to be scheduled to be freed (otherwise
    //we will deadlock as the destructor is called, waiting for the
    //node to delete itself!)
    NodeDeleter::scheduleForDeletion(this_node);
}

/** Send a message to the backend thread on the node running the job */
void MPINodeData::sendMessage(int message)
{
    shared_ptr<MPINodesData> communicator = parent.lock();
    BOOST_ASSERT( communicator.get() != 0 );
        
    MPI::Comm *mpicom = communicator->mpiCommunicator();
    int mpitag = communicator->mpiTag();
    
    BOOST_ASSERT( mpicom != 0 );
    BOOST_ASSERT( mpitag > 1 );

    int envelope[3];
    envelope[0] = message;
    envelope[1] = 0;
    envelope[2] = 0;
    
    qDebug() << comm_world->Get_rank() << mpirank << mpitag  
                 << "mpicom->Send( envelope, 3, MPI::INT, mpirank, mpitag );";
    mpicom->Send( envelope, 3, MPI::INT, mpirank, mpitag );
    qDebug() << comm_world->Get_rank()  
                 << "SENT!";
}

/** Stop any jobs running on this node */
void MPINodeData::stop()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not QThread::isRunning())
        //nothing to stop
        return;
        
    //send a message to the node telling it to stop
    this->sendMessage( MPIWORKER_STOP );
    
    //now wait until we know that the node has finished
    lkr.unlock();
    
    QThread::wait();
}

/** Abort any jobs running on this node */
void MPINodeData::abort()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not QThread::isRunning())
        //nothing to abort
        return;

    if (is_aborted)
        //the job has already been aborted
        return;

    //tell the node to abort
    this->sendMessage( MPIWORKER_ABORT );

    //wait until the node has finished
    lkr.unlock();
    
    QThread::wait();
}

/** Internal function called by MPIPromise to trigger a progress request */
void MPINodeData::getProgress()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not QThread::isRunning())
        //the job isn't running!
        return;
        
    //tell the node to give us a progress report
    this->sendMessage( MPIWORKER_PROGRESS );
}
    
/** Internal function called by MPIPromise to ask for an interim result */
void MPINodeData::getInterimResult()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not QThread::isRunning())
        //the job isn't running
        return;
        
    //tell the node to give us an interim result
    this->sendMessage( MPIWORKER_INTERIM );
}

//////////
////////// Implementation of MPINode
//////////

static boost::shared_ptr<MPINodeData> shared_null;

static const boost::shared_ptr<MPINodeData>& getSharedNull()
{
    if (shared_null.get() == 0)
    {
        shared_null.reset( new MPINodeData() );
        shared_null->self_ptr = shared_null;
    }
    
    return shared_null;
}

/** Null constructor */
MPINode::MPINode() : d( getSharedNull() )
{}

/** Construct an MPINode which is part of the passed communicator,
    with the specified rank, and saying whether or not this is 
    the master node of this communicator */
MPINode::MPINode(const MPINodes &communicator, int rank, bool is_master)
        : d( new MPINodeData(communicator.d, rank, is_master) )
{
    d->self_ptr = d;
}

/** Private copy constructor */
MPINode::MPINode(const boost::shared_ptr<detail::MPINodeData> &data)
        : d( data )
{}

/** Copy constructor */
MPINode::MPINode(const MPINode &other)
        : d(other.d)
{}

/** Destructor */
MPINode::~MPINode()
{}

/** Copy assignment operator */
MPINode& MPINode::operator=(const MPINode &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPINode::operator==(const MPINode &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPINode::operator!=(const MPINode &other) const
{
    return d.get() != other.d.get();
}

/** Return the unique ID for this MPI node */
const QUuid& MPINode::UID() const
{
    return d->uid;
}

/** Return whether or not this node is null */
bool MPINode::isNull() const
{
    return d->parent.expired() or d->uid.isNull();
}

/** Return this node to the pool of unused threads - this returns
    immediately, but the node will only be returned once it is no 
    longer busy, and no other copies of this MPINode exist */
void MPINode::returnNode()
{
    this->operator=( MPINode() );
}

/** Return the communicator in which this node belongs 

    \throw SireError::unavailable_resource
*/
MPINodes MPINode::communicator() const
{
    boost::shared_ptr<MPINodesData> parent = d->parent.lock();

    if (parent.get() == 0)
        throw SireError::unavailable_resource( QObject::tr(
            "A null MPINode doesn't have a communicator!"), CODELOC );
            
    return MPINodes(parent);
}

/** Return the rank of this node in its communicator */
int MPINode::rank() const
{
    #ifdef __SIRE_USE_MPI__
        return d->mpirank;
    #else
        return 0;
    #endif
}

/** Return whether this node is the master node for the communicator */
bool MPINode::isMaster() const
{
    return d->is_master;
}

/** Return whether or not this node is busy doing some work */
bool MPINode::isBusy() const
{
    return d->isBusy();
}
    
/** Wait until this node is no longer busy doing work */
void MPINode::wait()
{
    d->wait();
}

/** Wait until this node is no longer busy doing work, or until
    'time' milliseconds have passed. This returns whether or not
    the nodes is not busy */
bool MPINode::wait(int time)
{
    return d->wait(time);
}

/** Start and set the node working in the job contained in 'worker'. 
    This blocks until any other jobs running on this node have
    finished. This returns an MPIPromise that can be used to follow
    the progress of the work and get the result */
MPIPromise MPINode::start(const MPIWorker &worker)
{
    return d->start(worker);
}

/** This tells the node to stop performing any work that it may 
    be running. This stops the work at the next convienient moment
    and returns to the promise the result at the point of stopping.
    This does not block */
void MPINode::stop()
{
    d->stop();
}

/** This tells the node to abort any work it may be running. This
    stops the work at the next convienient moment and returns to
    the promise the work from before it started. This does not block. */
void MPINode::abort()
{
    d->abort();
}

/** Return whether the last (or current) job has been aborted */
bool MPINode::wasAborted() const
{
    return d->wasAborted();
}

/** Internal function called by MPIPromise to request the progress of the job */
void MPINode::getProgress()
{
    d->getProgress();
}

/** Internal function used by MPIPromise to ask for an interim result */
void MPINode::getInterimResult()
{
    d->getInterimResult();
}
