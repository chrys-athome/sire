
//mpich requires that mpi.h is included first
#include <mpi.h>

#include <Python.h>

#include <cstdio>

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include "SireCluster/cluster.h"
#include "SireCluster/nodes.h"
#include "SireCluster/node.h"
#include "SireCluster/promise.h"

#include "Helpers/pythonpacket.h"

using std::printf;

using namespace SireCluster;

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
     
    // Now do the clean up actions:
    Cluster::shutdown();

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
 
    //release the global python lock
    PyEval_ReleaseLock();
 
    try
    {
        //read the command line options to get the number of
        //thread per node
        int ppn = 1;

        //now read the number of OpenMP threads per process
        int nomp = 1;

        //start MPI - ABSOLUTELY must use multi-threaded MPI
        ::MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);

        //are we the first node in the cluster?
        if (Cluster::getRank() == 0)
        {
            printf("Starting master node (%d of %d): nThreads()=%d\n", 
                       Cluster::getRank(), Cluster::getCount(), ppn);

            //name this process and thread
            SireError::setProcessString("master");
            SireError::setThreadString("main");

            //start the cluster - on the master we need one extra
            //thread for the Python interpreter
            ::MPI::COMM_WORLD.Barrier();
            Cluster::start(ppn);
            ::MPI::COMM_WORLD.Barrier();

            //run python
            if (argc >= 2)
            {
                Nodes nodes = Cluster::getNode();
                Node node = nodes.getNode();
                Promise promise = node.startJob( PythonPacket(argv[1]) );
                promise.wait();
                
                if (promise.isError())
                {
                    promise.throwError();
                }
            }
        }
        else
        {
            //this is one of the compute nodes...
            printf("Starting one of the compute nodes (%d of %d): nThreads()=%d\n", 
                        Cluster::getRank(), Cluster::getCount(), ppn);

            //name this process
            SireError::setProcessString( QString("compute%1").arg(Cluster::getRank()) );
            SireError::setThreadString( "main" );

            //exec the Cluster - this starts the cluster and then
            //blocks while it is running
            ::MPI::COMM_WORLD.Barrier();
            Cluster::start(ppn);
            ::MPI::COMM_WORLD.Barrier();

            Cluster::wait();
            status = 0;
        }
    }
    catch(const SireError::exception &e)
    {
        SireError::printError(e);
        status = -1;
    }
    catch(const std::exception &e)
    {
        SireError::printError( SireError::std_exception(e) );
        status = -1;
    }
    catch(...)
    {
        SireError::printError(SireError::unknown_exception(
                                 QObject::tr("An unknown error occurred!"), CODELOC ) );

        status = -1;
    }

    //shutdown the cluster
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

    //now shutdown Python - currently commented out
    //as calling these functions causes a bus error...
    PyEval_AcquireLock();
    //Py_Finalize();

    return status;
}
