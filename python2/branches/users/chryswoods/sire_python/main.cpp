
//mpich requires that mpi.h is included first
#include <mpi.h>

#include <Python.h>

#include <cstdio>

#include "SireMPI/mpinodes.h"

#include "SireError/errors.h"
#include "SireError/printerror.h"

using std::printf;

using namespace SireMPI;

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

            MPINodes nodes;

            //start the MPI event loop (this runs the loop in the background)
            nodes.exec();

            //now run python
            Py_Initialize();
            status = Py_Main(argc, argv);

            //the script has finished - shutdown the cluster
            nodes.shutdown();
        }
        else
        {
            //this is one of the compute nodes...
            printf("Starting one of the compute nodes (%d of %d)\n", MPI::COMM_WORLD.Get_rank(),
                                                                     MPI::COMM_WORLD.Get_size());

            MPIBackendNodes nodes;

            //this blocks until the nodes are shutdown
            nodes.exec();

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

    //put up a barrier to wait for every MPI node to finish
    if (MPI::COMM_WORLD.Get_rank() == 0)
    {
        printf("Shutting down MPI...\n");
    }

    MPI::COMM_WORLD.Barrier();

    if (MPI::COMM_WORLD.Get_rank() == 0)
    {
        printf("MPI has shutdown on all nodes.\n");
    }

    //make sure we have stopped MPI
    MPI::Finalize();

    return status;
}
