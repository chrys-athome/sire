
#include <mpi.h>

#include <unistd.h>

int main(int argc, char **argv)
{
    MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);

    if (MPI::COMM_WORLD.Get_rank() == 0)
    {
        //do nothing
        sleep(30);
        MPI::COMM_WORLD.Barrier();
    }
    else
    {
        MPI::COMM_WORLD.Barrier();
    }

    MPI::Finalize();

    return 0;
}

