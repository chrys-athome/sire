
#ifdef SIRE_USE_MPI
    //mpich requires that mpi.h is included first
    #include <mpi.h>
#endif

#include <cstdio>

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include "SireCluster/cluster.h"
#include "SireCluster/nodes.h"
#include "SireCluster/node.h"
#include "SireCluster/promise.h"

#include "SireBase/process.h"

#include "restartpacket.h"

using namespace SireCluster;

using std::printf;

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
    const int ppn = 1;  //the number of processes per node

    try
    {
        #ifdef SIRE_USE_MPI
            //start MPI - ABSOLUTELY must use multi-threaded MPI
            ::MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);
        #endif

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
            #ifdef SIRE_USE_MPI
                ::MPI::COMM_WORLD.Barrier();
            #endif

            Cluster::start(ppn);

            #ifdef SIRE_USE_MPI
                ::MPI::COMM_WORLD.Barrier();
            #endif

            //each argument is a binary restart file that needs
            //to be run
            if (argc >= 2)
            {
                int nrestarts = argc - 1;

                printf("Running %d simulation(s)...\n", nrestarts);

                Nodes nodes = Cluster::getNodes(nrestarts);

                QList<Promise> promises;
 
                //submit all of the simulations
                for (int i=0; i<nrestarts; ++i)
                {
                    Node node = nodes.getNode();
                    printf("\nRunning simulation %d of %d: %s\n", 
                           i+1, nrestarts, argv[i+1]);
                           
                    promises.append( node.startJob( 
                                Sire::RestartPacket( QLatin1String(argv[i+1]) ) ) );
                }
                
                //wait for them all to finish
                for (int i=0; i<nrestarts; ++i)
                {
                    promises[i].wait();
                }

                //did any simulations finish in error?
                for (int i=0; i<nrestarts; ++i)
                {
                    if (promises[i].isError())
                    {
                        printf("\nThere was a problem when running %s.\n", argv[i+1]);
                        
                        try
                        {
                            promises[i].throwError();
                        }
                        catch(const SireError::exception &e)
                        {
                            SireError::printError(e);
                        }
                    }
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
            #ifdef SIRE_USE_MPI
                ::MPI::COMM_WORLD.Barrier();
            #endif
 
            Cluster::start(ppn);

            #ifdef SIRE_USE_MPI
                ::MPI::COMM_WORLD.Barrier();
            #endif

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
    
    return status;
}
