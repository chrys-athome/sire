
#include <iostream>
#include <cstring>
using namespace std;

int main(void)
{
    int *a = new int[20];
    int *b = new int[20];
    
    cout << "Populating the 'a' array...\n";
    for (int i=0; i<20; i++)
    {
        a[i] = i;
        cout << i << " " << a[i] << endl;
    }

    //This won't work!    
    (*b) = (*a);
    
    cout << "Using (*b) = (*a) will not work....\n";
    for (int i=0; i<20; i++)
        cout << b[i] << endl;

    //you need to copy the array manually
    //One possibility is the C 'memcpy' function
    //(from string.h)
    cout << "However, memcpy will work!\n";
    memcpy(&b,&a,sizeof(a));

    for (int i=0; i<20; i++)
        cout << b[i] << endl;

    //there are other possibilities, e.g. using the 
    //C++ std::copy template algorithm, though 
    //you would then need to be using C++ containers 
    //rather than C arrays.

    return 0;
}
