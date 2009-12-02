
#include <iostream>

#include <omp.h>

using std::cout;
using std::endl;

/** This is just a small openmp program that
    prints the number of omp threads available.

    This is used to show that the MPI mini_sire
    program (one per node) can start the 
    OpenMP mini_molpro program (with number
    of threads equal to number of cores 
    per node
*/

int main(int argc, char **argv)
{
    cout << "Running mini_molpro...\n";

    // wait for 10 seconds - give the job something to do!
    sleep(10);

    int num_threads = 0;

    #pragma omp parallel shared(num_threads)
    {
        num_threads = omp_get_num_threads();

        #pragma omp master
        {
            cout << "Number of OMP threads equals " << num_threads << endl;
        }
    }

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
 
        #pragma omp critical
        {
            cout << "Hello - I am thread " << thread_id << " of " << num_threads << endl;
        }
    }

    return 0;
}
