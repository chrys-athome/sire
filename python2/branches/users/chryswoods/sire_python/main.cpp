
#ifdef SIRE_USE_MPI
    //mpich requires that mpi.h is included first
    #include <mpi.h>
#endif

#include <Python.h>

#include <cstdio>

#include "Siren/errors.h"

#include "SireCluster/cluster.h"
#include "SireCluster/nodes.h"
#include "SireCluster/node.h"
#include "SireCluster/promise.h"
#include "SireCluster/promises.h"

#include "SireBase/process.h"

#include "Siren/forages.h"

#include "pythonpacket.h"

using std::printf;

using namespace SireCluster;
using namespace Siren;

#include <QDebug>

#ifdef Q_OS_UNIX

#include <signal.h>

//handle CTRL-C signal - this should kill the calculation
// - with thanks to 
//  <http://www.gnu.org/software/libtool/manual/libc/Termination-in-Handler.html#Termination-in-Handler>

volatile sig_atomic_t fatal_error_in_progress = 0;
     
void fatal_error_signal (int sig)
{
    // Since this handler is established for more than one kind of signal, 
    // it might still get invoked recursively by delivery of some other kind
    // of signal.  Use a static variable to keep track of that.
    if (fatal_error_in_progress)
        raise (sig);
 
    fatal_error_in_progress = 1;

    printf("You're killing me!!!\n");
     
    // Kill any child processes
    SireBase::Process::killAll();

    // This is the end of for-ages
    Siren::end_for_ages();

    // Now do the clean up actions:
    Cluster::shutdown();

    printf("\nI, and all of my children are now dead. Adieu...\n");

    // Now reraise the signal.  We reactivate the signal's
    // default handling, which is to terminate the process.
    // We could just call exit or abort,
    // but reraising the signal sets the return status
    // from the process correctly.
    signal (sig, SIG_DFL);
    raise (sig);
}

#endif // Q_OS_UNIX

int main(int argc, char **argv)
{
    #ifdef Q_OS_UNIX
        signal(SIGINT, fatal_error_signal);
        signal(SIGTERM, fatal_error_signal);
    #endif // Q_OS_UNIX

    int status = 0;

    //start Python - ABSOLUTELY must use multi-threaded python
    Py_Initialize();
    
    //this function starts threaded python and grabs
    //the global python lock
    PyEval_InitThreads();

    //no command line arguments are passed to python
    PySys_SetArgv(1, argv);

    //release the global python lock
    PyEval_ReleaseLock();
 
    try
    {
        int mpi_rank = 0;
        int mpi_count = 1;

        #ifdef SIRE_USE_MPI
            //start MPI - ABSOLUTELY must use multi-threaded MPI
            ::MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);
            mpi_rank = ::MPI::Get_rank();
            mpi_count = ::MPI_Get_count();
        #endif

        //are we the first node in the cluster?
        if (mpi_rank == 0)
        {
            printf("Starting master node (%d of %d)\n", 
                       mpi_rank, mpi_rank);

            //name this process and thread
            Siren::setProcessString("master");
            Siren::setThreadString("main");

            //start the cluster - on the master we need one extra
            //thread for the Python interpreter
            #ifdef SIRE_USE_MPI
                //::MPI::COMM_WORLD.Barrier();
            #endif

            Cluster::start();

            #ifdef SIRE_USE_MPI
                //::MPI::COMM_WORLD.Barrier();
            #endif

            //run python - each argument is a python script
            if (argc >= 2)
            {
                int nscripts = argc - 1;

                printf("Running %d python script(s)...\n", nscripts);

                Nodes nodes = Cluster::getNodes(nscripts, 500);

                QVector<WorkPacketPtr> workpackets;
                workpackets.reserve(nscripts);

                for (int i=0; i<nscripts; ++i)
                {
                    workpackets.append( PythonPacket(argv[i+1]) );
                }

                //submit all of the scripts
                Promises promises = nodes.submit(workpackets);
                
                //wait for them all to finish
                promises.wait();

                //did any script finish in error?
                for (int i=0; i<nscripts; ++i)
                {
                    if (promises[i].isError())
                    {
                        printf("\nThere was a problem when running %s.\n", argv[i+1]);
                        
                        try
                        {
                            promises[i].throwError();
                        }
                        catch(const Siren::exception &e)
                        {
                            Siren::printError(e);
                        }
                    }
                }
            }
        }
        else
        {
            //this is one of the compute nodes...
            printf("Starting one of the compute nodes (%d of %d)\n", 
                        mpi_rank, mpi_count);

            //name this process
            Siren::setProcessString( QString("compute%1").arg(mpi_rank) );
            Siren::setThreadString( "main" );

            //exec the Cluster - this starts the cluster and then
            //blocks while it is running
            #ifdef SIRE_USE_MPI
                //::MPI::COMM_WORLD.Barrier();
            #endif
 
            Cluster::start();

            #ifdef SIRE_USE_MPI
                //::MPI::COMM_WORLD.Barrier();
            #endif

            Cluster::wait();
            status = 0;
        }
    }
    catch(const Siren::exception &e)
    {
        Siren::printError(e);
        status = -1;
    }
    catch(const std::exception &e)
    {
        Siren::printError( Siren::std_exception(e) );
        status = -1;
    }
    catch(...)
    {
        Siren::printError(Siren::unknown_error(
                            QObject::tr("An unknown error occurred!"), CODELOC ) );

        status = -1;
    }

    //shutdown the cluster
    #ifdef SIRE_USE_MPI
        if (::MPI::COMM_WORLD.Get_rank() == 0)
        {
            printf("Shutting down the cluster...\n");
            Cluster::shutdown();
        }

        //wait for all of the MPI jobs to finish
        ::MPI::COMM_WORLD.Barrier();

        if (::MPI::COMM_WORLD.Get_rank() == 0)
        {
            printf("The entire cluster has now shutdown.\n");
        }

        ::MPI::Finalize();
    #else
        printf("Shutting down the cluster...\n");
        Cluster::shutdown();
        printf("The entire cluster has now shutdown.\n");
    #endif

    //now shutdown Python - currently commented out
    //as calling these functions causes a bus error...
    //PyEval_AcquireLock();
    //Py_Finalize();

    return status;
}
