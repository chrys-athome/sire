
#ifdef SIRE_USE_MPI
    //mpich requires that mpi.h is included first
    #include <mpi.h>
#endif

#include <Python.h>

#include <cstdio>

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include "SireCluster/cluster.h"
#include "SireCluster/nodes.h"
#include "SireCluster/node.h"
#include "SireCluster/promise.h"

#include "SireBase/process.h"

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
        //read the command line options to get the number of
        //thread per node
        int ppn = 1;

        //now read the number of OpenMP threads per process
        int nomp = 1;

        #ifdef SIRE_USE_MPI
            //start MPI - ABSOLUTELY must use multi-threaded MPI
            int level;
            MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &level);
        #endif

        //are we the first node in the cluster?
        if (Cluster::getRank() == 0)
        {
            QStringList scripts;

            //read command line options
            {
                int i = 1;
                while (i < argc)
                {
                    QString arg(argv[i]);

                    if (arg.startsWith("--"))
                    {
                        if (arg.startsWith("--ppn"))
                        {
                            QStringList vals = arg.split("=");
                            if (vals.count() == 2)
                            {
                                bool ok;
                                ppn = vals[1].toInt(&ok);

                                if (ppn <= 0 or (not ok)){ ppn = 1; }
                            }
                        }
                    }
                    else
                        scripts.append(QString(argv[i]));

                    i += 1;
                }
            }

            printf("Starting master node (%d of %d): nThreads()=%d\n", 
                       Cluster::getRank(), Cluster::getCount(), ppn);

            //name this process and thread
            SireError::setProcessString("master");
            SireError::setThreadString("main");

            //start the cluster - on the master we need one extra
            //thread for the Python interpreter
            #ifdef SIRE_USE_MPI
                //MPI_Barrier( MPI_COMM_WORLD );
            #endif

            Cluster::start(ppn);

            #ifdef SIRE_USE_MPI
                //MPI_Barrier( MPI_COMM_WORLD );
            #endif

            //run python - each argument is a python script
            if (not scripts.isEmpty())
            {
                int nscripts = scripts.count();

                printf("Running %d python script(s)...\n", nscripts);

                Nodes nodes = Cluster::getNodes(nscripts);

                printf("...over %d process(es)\n", nodes.count());

                QList<Promise> promises;
 
                //submit all of the scripts
                for (int i=0; i<nscripts; ++i)
                {
                    Node node = nodes.getNode();
                    printf("\nRunning script %d of %d: %s\n", i+1, 
                           nscripts, scripts[i].toAscii().constData());
                    promises.append( node.startJob( PythonPacket(scripts[i]) ) );
                }
                
                //wait for them all to finish
                for (int i=0; i<nscripts; ++i)
                {
                    promises[i].wait();
                }

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
                //MPI_Barrier( MPI_COMM_WORLD );
            #endif
 
            Cluster::start(ppn);

            #ifdef SIRE_USE_MPI
                //MPI_Barrier( MPI_COMM_WORLD );
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
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank == 0)
        {
            printf("Shutting down the cluster...\n");
            Cluster::shutdown();
        }

        //wait for all of the MPI jobs to finish
        MPI_Barrier( MPI_COMM_WORLD );

        if (rank == 0)
        {
            printf("The entire cluster has now shutdown.\n");
        }

        MPI_Finalize();
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
