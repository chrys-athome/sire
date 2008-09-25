
#include <Python.h>

#include <mpi.h>

#include <cstdio>

#include "SireMPI/mpinode.h"
#include "SireError/errors.h"

using std::printf;

int main(int argc, char **argv)
{
    try
    {
        //start MPI
        SireMPI::ensureInitializedMPI(argc, argv);

        //are we the first node in the cluster?
        if (MPI::COMM_WORLD.Get_rank() == 0)
        {
            printf("Starting master node (%d of %d)\n", MPI::COMM_WORLD.Get_rank(),
                                                        MPI::COMM_WORLD.Get_size());

            //start the MPI event loop in the background
            SireMPI::bg_exec(argc, argv);

            //now run python
            Py_Initialize();
            int status = Py_Main(argc, argv);

            //the script has finished - shutdown the cluster
            SireMPI::shutdown();

            return status;
        }
        else
        {
            //this is one of the compute nodes...
            printf("Starting one of the compute nodes (%d of %d)\n", MPI::COMM_WORLD.Get_rank(),
                                                                     MPI::COMM_WORLD.Get_size());

            return SireMPI::exec(argc, argv);
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
        SireMPI::writeErrorString(CODELOC, SireError::unknown_exception(
                  QObject::tr("An unknown error occurred!"), CODELOC ) );
    }

    //make sure we have stopped MPI
    MPI::Finalize();

    return -1;
}
