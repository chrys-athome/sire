
//mpich requires that mpi.h is included first
#include <mpi.h>

#include <Python.h>

#include <cstdio>

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include "SireCluster/cluster.h"

using std::printf;

using namespace SireCluster;

#include <QDebug>

int main(int argc, char **argv)
{
    int status = 0;

    try
    {
        //start MPI - ABSOLUTELY must use multi-threaded MPI
        MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);

        //are we the first node in the cluster?
        if (Cluster::getRank() == 0)
        {
            printf("Starting master node (%d of %d)\n", Cluster::getRank(),
                                                        Cluster::getCount());

            //name this process and thread
            SireError::setProcessString("master");
            SireError::setThreadString("main");

            //start the cluster
            MPI::COMM_WORLD.Barrier();
            Cluster::start();
            MPI::COMM_WORLD.Barrier();

            //run python
            Py_Initialize();
            status = Py_Main(argc, argv);
        }
        else
        {
            //this is one of the compute nodes...
            printf("Starting one of the compute nodes (%d of %d)\n", Cluster::getRank(),
                                                                     Cluster::getCount());

            //name this process
            SireError::setProcessString( QString("compute%1").arg(Cluster::getRank()) );
            SireError::setThreadString( "main" );

            //exec the Cluster - this starts the cluster and then
            //blocks while it is running
            MPI::COMM_WORLD.Barrier();
            Cluster::start();
            MPI::COMM_WORLD.Barrier();

            QList<QUuid> uids = Cluster::UIDs();
            qDebug() << Cluster::getRank() << uids;

            Cluster::exec();
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
    if (MPI::COMM_WORLD.Get_rank() == 0)
    {
        printf("Shutting down the cluster...\n");
        Cluster::shutdown();
    }

    //wait for all of the MPI jobs to finish
    MPI::COMM_WORLD.Barrier();

    if (MPI::COMM_WORLD.Get_rank() == 0)
    {
        printf("The entire cluster has now shutdown.\n");
    }

    MPI::Finalize();

    return status;
}
