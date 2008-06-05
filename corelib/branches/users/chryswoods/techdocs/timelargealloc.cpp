
#include <QTime>

#include <iostream>
#include <cmath>

using namespace std;

const int n = 60000;

// on my macbook, memcpy time is linear with n
//
//               macbook 2.16Ghz      pentium D
//                 core2 duo           3.4 GHz
// n = 60000000   906 ms            560 ms        =  463 MB (*2)
// n = 6000000     70 ms             65 ms        =   46 MB (*2)
// n = 600000       7 ms              7 ms        = 4687 kB (*2) 
// n = 60000        0 ms              0 ms        =  468 kB (*2)
//
// In all tests, allocating is 0 ms, while deleting does
// scale with n, up to ~100ms for 60000000, but 10ms for 6000000
//
// memcpy time is painful for large n (corresponds to 20M atoms)
// However, realistic system sizes (10-100k atoms) have a low
// penalty, though for 100k this is still a good chunk of
// a 100 moves per second time)

int main(int argc, const char **argv)
{
    double *a, *b;
    
    QTime t;
    
    cout << "n == " << n << endl;
    
    for (int i=0; i<10; ++i)
    {
        t.start();
        
        //allocate an array equivalent to 100000 atoms...
        a = new double[n];
        
        int ms = t.elapsed();
        
        cout << "Allocating 'a' took " << ms << " ms\n";
        
        //fill the array with random gunk...
        for (int i=0; i<n; ++i)
        {
            a[i] = rand();
        }
        
        //now allocate b...
        t.start();
        
        b = new double[n];
        
        ms = t.elapsed();
        
        cout << "Allocating 'b' took " << ms << " ms\n";
        
        //copy a to b
        t.start();
        
        void *output = qMemCopy( b, a, n*sizeof(double) );
        
        ms = t.elapsed();
        
        if (output != b)
            cout << "Something went wrong with the copy...\n";
            
        //validate that the copy is equal
        bool ok = true;
        
        for (int i=0; i<n; ++i)
        {
            if (a[i] != b[i])
            {
                ok = false;
                break;
            }
        }
        
        if (not ok)
            cout << "Detected differences between a and b...\n";
            
        cout << "memcpy took " << ms << " ms\n";
    
        //now delete both arrays
        t.start();
        
        delete[] a;
        delete[] b;
        
        ms = t.elapsed();
        
        cout << "Deleting the arrays took " << ms << " ms\n";
    }


    return 0;
}
