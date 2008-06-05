
#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
        MPI::Init(argc,argv);
        
        int data = 1;

        MPI::Intracomm netcom = MPI::COMM_WORLD.Split(0,0);
        
        if (netcom.Get_rank() == 0)
        {
            cout << "Sending message to myself...\n";
            netcom.Isend(&data,1,MPI::INT,0,42);
        
            cout << "Probing for the message - this will block until it is received...\n";
            netcom.Probe(MPI_ANY_SOURCE,MPI_ANY_TAG);
        
            if (netcom.Iprobe(0,42))
            {
                cout << "Message from node 0, tag 42 was received!\n";
            }
        }
                
        netcom.Barrier();
        netcom.Free();
        
        MPI::Finalize();
        
        return 0;
}

