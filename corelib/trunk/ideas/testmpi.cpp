
#include <iostream>

#include <mpi.h>

using namespace std;

int main(int argc, char **argv)
{

    MPI::Init(argc,argv);
    
    int node = MPI::COMM_WORLD.Get_rank();
    
    cout << "I am processor " << node << endl;
    
    if (node % 2 == 0)
    {
        int val = 4723789;
        
        MPI::COMM_WORLD.Send(&val, 1, MPI::INT, node+1, 1);
        
        char result[6];
        
        MPI::COMM_WORLD.Recv(&result, 6, MPI::CHAR, node+1, 1);
        
        cout << "Node " << node << " received result " << result[0]
             << result[1] << result[2] << result[3] << result[4] 
             << " " << int(result[0]) << endl;
    }
    else
    {
        int result;
        
        MPI::COMM_WORLD.Recv(&result, 1, MPI::INT, node-1, 1);
        
        cout << "Node " << node << " received result " << result << endl;
        
        char val[6];
        val[0] = 'h';
        val[1] = 'e';
        val[2] = 'l';
        val[3] = 'l';
        val[4] = 'o';
        val[5] = '\0';
        
        cout << "Sending " << val[0] << val[1] << val[2] << val[3]
                           << val[4] << " " << int(val[0]) << endl;
        
        MPI::COMM_WORLD.Send(&val, 6, MPI::CHAR, node-1, 1);
    }
    
    MPI::Finalize();
    
    return 0;

}
