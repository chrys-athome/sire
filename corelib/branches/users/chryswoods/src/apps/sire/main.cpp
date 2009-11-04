
#ifdef SIRE_USE_MPI
    //mpich requires that mpi.h is included first
    #include <mpi.h>
#endif

#include <cstdio>

#include <QFile>
#include <QByteArray>

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include "SireCluster/cluster.h"
#include "SireCluster/nodes.h"
#include "SireCluster/node.h"
#include "SireCluster/promise.h"

#include "SireBase/process.h"

#include "SireSystem/system.h"
#include "SireMove/suprasystem.h"

#include "SireMove/moves.h"
#include "SireMove/supramoves.h"

#include "SireMove/simpacket.h"
#include "SireMove/suprasimpacket.h"

#include "SireStream/streamdata.hpp"

using namespace SireCluster;
using namespace SireMove;
using namespace SireSystem;
using namespace SireStream;

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

void throwIncompatibleError(const Property *p0, const Property *p1)
{
    throw SireError::incompatible_error( QObject::tr(
        "We can only perform a restart simulation "
        "with a System and Move(s) object, or with a SupraSystem and "
        "SupraMove(s) object. It cannot work with a %1 and %2.")
            .arg(p0->what()).arg(p1->what()), CODELOC );
}

/** This reads a simulation restart file and creates a workpacket
    to run the next step in the simulation */
WorkPacket createWorkPacket(const char *filename,
                            int nmoves, bool record_stats)
{
    //read the contents of the restart file into memory
    QFile f(filename);

    if (not f.open( QIODevice::ReadOnly) )
        throw SireError::file_error(f, CODELOC);
    
    QByteArray restart_data = f.readAll();

    if (restart_data.isEmpty())
        throw SireError::file_error( QObject::tr(
            "There was an error reading data from the file %1. Either "
            "the file is empty, or some read error has occured.")
                .arg(filename), CODELOC );

    //sanity check the header
    {
        FileHeader header = SireStream::getDataHeader(restart_data);
    
        if (header.dataTypes().count() != 2)
        {
            throw SireError::incompatible_error( QObject::tr(
                "A RestartPacket can only process a restart file that contains "
                "two objects; the first should be the System or SupraSystem to "
                "be simulated, while the second should be the moves to be "
                "applied. The types available in the passed restart file "
                "are [ %1 ].").arg(header.dataTypes().join(", ")), CODELOC );
        }
    }
    
    //unpack the binary data
    QList< boost::tuple<boost::shared_ptr<void>,QString> > objects 
                         = SireStream::load(restart_data);
    
    if (not objects.count() == 2)
        throw SireError::file_error( QObject::tr(
            "The restart file may be corrupted as despite the header claiming "
            "there were two objects, the number of objects is actually equal "
            "to %1.").arg(objects.count()), CODELOC );
    
    //the objects must both be derived from Property - if they are not
    //then this will cause a segfault
    Property *p0 = static_cast<Property*>(objects[0].get<0>().get());
    Property *p1 = static_cast<Property*>(objects[1].get<0>().get());
    
    //the first object should be derived from System or SupraSystem
    if (p0->isA<System>())
    {
        //the second object must be a 'Move' or 'Moves'
        if (p1->isA<Moves>())
        {
            return SimPacket(p0->asA<System>(), p1->asA<Moves>(),
                             nmoves, record_stats);
        }
        else if (p1->isA<Move>())
        {
            return SimPacket(p0->asA<System>(), SameMoves(p1->asA<Move>()),
                             nmoves, record_stats);
        }
        else
            ::throwIncompatibleError(p0, p1);
    }
    else if (p0->isA<SupraSystem>())
    {
        //the second object must be a 'SupraMove' or 'SupraMoves'
        if (p1->isA<SupraMoves>())
        {
            return SupraSimPacket(p0->asA<SupraSystem>(), p1->asA<SupraMoves>(),
                                  nmoves, record_stats);
        }
        else if (p1->isA<SupraMove>())
        {
            return SupraSimPacket(p0->asA<SupraSystem>(),
                                  SameSupraMoves(p1->asA<SupraMove>()),
                                  nmoves, record_stats);
        }
        else
            ::throwIncompatibleError(p0, p1);
    }
    else
        ::throwIncompatibleError(p0, p1);
        
    return WorkPacket();
}

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

                ThisThread this_thread = nodes.borrowThisThread();

                QList<Promise> promises;
                QList<const char*> running_files;
 
                //submit all of the simulations
                for (int i=0; i<nrestarts; ++i)
                {
                    try 
                    {
                        //create a workpacket for this simulation
                        WorkPacket workpacket = createWorkPacket(argv[i+1], 1, true);

                        printf("\nRunning simulation %d of %d: %s\n", 
                               i+1, nrestarts, argv[i+1]);
                        
                        Node node = nodes.getNode();
                        
                        promises.append( node.startJob(workpacket) ); 
                        running_files.append( argv[i+1] );
                    }
                    catch (const SireError::exception &e) 
                    {
                        printf("\nThere was a problem when reading %s.\n", argv[i+1]);
                        SireError::printError(e);
                    }
                }
                
                //wait for them all to finish
                for (int i=0; i < promises.count(); ++i)
                {
                    promises[i].wait();
                }

                //did any simulations finish in error?
                for (int i=0; i < promises.count(); ++i)
                {
                    if (promises[i].isError())
                    {
                        printf("\nThere was a problem when running %s.\n",
                               running_files[i]);
                        
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
