
#include <iostream>

using namespace std;

int main(void)
{
    int *iptr = 0;
    double *dptr = 0;
    
    cout << iptr << " " << dptr << endl;
    
    iptr += 1;
    dptr += 1;

    cout << iptr << " " << dptr << endl;
    
    iptr += 10;
    dptr += 10;

    cout << iptr << " " << dptr << endl;

    return 0;
}
