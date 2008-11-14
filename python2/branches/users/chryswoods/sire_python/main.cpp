
//mpich requires that mpi.h is included first
#include <mpi.h>

#include <Python.h>

#include <cstdio>

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include "SireCluster/cluster.h"

using std::printf;

using namespace SireCluster;

int main(int argc, char **argv)
{
    int status = 0;

    try
    {
        //start MPI
        MPI::Init(argc, argv);

        //are we the first node in the cluster?
        if (MPI::COMM_WORLD.Get_rank() == 0)
        {
            printf("Starting master node (%d of %d)\n", MPI::COMM_WORLD.Get_rank(),
                                                        MPI::COMM_WORLD.Get_size());

            //start the cluster
            Cluster::start();

            printf("Starting a Python shell...\n");

            //run python
            Py_Initialize();
            status = Py_Main(argc, argv);
        }
        else
        {
            //this is one of the compute nodes...
            printf("Starting one of the compute nodes (%d of %d)\n", MPI::COMM_WORLD.Get_rank(),
                                                                     MPI::COMM_WORLD.Get_size());

            //exec the Cluster - this starts the cluster and then
            //blocks while it is running
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

    //make sure we have stopped MPI
    MPI::Finalize();

    return status;
}
