
#include <iostream>
#include <cmath>

using namespace std;

unsigned long long qHash(const float &val)
{
    union{ double d; unsigned long long i; } converter;
    
    //clear the memory of the union - this should
    //prevent any surprises
    converter.i = 0;
    
    //assign the memory of the union to hold the double
    converter.d = val;
    
    //now return the union as an integer - this forces
    //the bit pattern of the double to be interpreted
    //as an integer
    return converter.i;
}

int main(void)
{

    double oldval = 0.0;

    for (int i=0; i<100; ++i)
    {
        double val = rand() * 100.0;
        
        cout << endl;
        cout << oldval << " = " << qHash(oldval) << endl;
        cout << val << " = " << qHash(val) << endl;
        cout << oldval << " = " << qHash(oldval) << endl;
        cout << val << " = " << qHash(val) << endl;
        cout << endl;

        oldval = val;
    }

    return 0;
}
