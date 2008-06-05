
#include <gsl/gsl_sys.h>

#include <iostream>
#include <cmath>

using namespace std;

int main(void)
{
    double a = 10.0;
    
    double b = 9.9;
    
    int nsteps = 100;
    double delta = 0.1 / nsteps;
    
    double tolerance = 1e-6;
    
    int a_exponent;
    double a_mantissa = frexp(a, &a_exponent);
    
    for (int i=0; i<2*nsteps; ++i)
    {
       int compare = gsl_fcmp(a,b,tolerance);
    
       switch(compare)
       {
           case 0:
               cout << "Same! : ";
               break;
           case -1:
               cout << "A < B : ";
               break;
           case 1:
               cout << "A > B : ";
               break;
       }
       
       int b_exponent;
       double b_mantissa = frexp(b, &b_exponent);
       
       cout << a << ", " << b << endl;
       cout << "      : " << a_mantissa << ", " << a_exponent << endl;
       cout << "      : " << b_mantissa << ", " << b_exponent << endl;
       
       double k = max(a_exponent, b_exponent);
       
       double error = tolerance * exp2(k);
       
       cout << "      : " << k << ", " << error << ", " 
                          << exp2(k) << endl;
       
       b += delta;
    }
    
    return 0;
}
