
#include <Python.h>

#include <QTextStream>

#include "Siren/errors.h"

#include "SireCluster/cluster.h"
#include "SireCluster/nodes.h"
#include "SireCluster/node.h"
#include "SireCluster/promise.h"
#include "SireCluster/promises.h"

#include "SireBase/process.h"

#include "Siren/forages.h"
#include "Siren/thread.h"

#include "SireSec/key.h"

#include "pythonpacket.h"

using namespace SireCluster;
using namespace SireSec;
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

    QTextStream stderr_str(stderr);
    stderr_str << QObject::tr("You're killing me!!!\n");
     
    // Kill any child processes
    SireBase::Process::killAll();

    // This is the end of for-ages
    Siren::end_for_ages();

    // Now do the clean up actions:
    Cluster::shutdownCluster();

    stderr_str << QObject::tr("\nI, and all of my children are now dead. Adieu...\n");

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
    QTextStream stdout_str(stdout);
    QTextStream stderr_str(stderr);

    //initialise Siren and SireSec - this starts the entropy collection
    Siren::init_rand();

    SireSec_init();

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

    QString hostname;
 
    try
    {
        Cluster::start();

        stdout_str.flush();
        stderr_str.flush();

        //are we the first node in the cluster?
        if (Cluster::isInitProcess())
        {
            hostname = Cluster::hostName();

            //name this process and thread
            Siren::setProcessString( QString("master_%1").arg(Cluster::hostName()) );
            Siren::setThreadString("main");

            //run python - each argument is a python script
            if (argc >= 2)
            {
                int nscripts = argc - 1;

                stdout_str << QObject::tr("Running %1 python script(s)...\n").arg(nscripts);
                stdout_str.flush();

                Nodes nodes = Cluster::getNodes(nscripts);

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
                        stdout_str << QObject::tr("\nThere was a problem when running %1.\n")
                                               .arg(argv[i+1]);
                        
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

            stdout_str << QObject::tr("Shutting down the cluster...\n");
            stdout_str.flush();
            Cluster::shutdownCluster();
            status = 0;
        }
        else
        {
            hostname = Cluster::hostName();

            //name this process
            Siren::setProcessString( QString("compute_%1").arg(Cluster::hostName()) );
            Siren::setThreadString( "main" );

            //create the resource for this process
            Cluster::addThread();

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

    //kill all threads
    Siren::end_for_ages();

    SireSec_end();

    stdout_str << QObject::tr("Exiting node (%1)... Goodbye!\n").arg(hostname);
    stdout_str.flush();    

    return status;
}
