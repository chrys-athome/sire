
#include <iostream>
#include <cmath>

using namespace std;

template<class T>
T getDistance2(int i)
{
    return T(i * T(0.001));
}

template<class T>
T getDistance(int i)
{
    return sqrt( getDistance2<T>(i) );
}

template<class T>
T getCoul(int i)
{
    return T(1.0) / getDistance<T>(i);
}

template<class T>
T getLJ(int i)
{
    T v = getDistance2<T>(i);

    T inv2 = T(1.0) / v;

    T v6 = inv2*inv2*inv2;
    T v12 = v6*v6;

    return v12 - v6;
}

template<class T1, class T2>
void test()
{
    cerr << "Comparing real( " << sizeof(T1) << " bytes ) vs. real( " << sizeof(T2) << " bytes )\n";

    for (int i=1000; i<100000; ++i)
    {
        T1 dist = getDistance<T1>(i);
    
        T1 ljd = getLJ<T1>(i);
        T2 ljf = getLJ<T2>(i);
        
        T1 ljdiff = ljd - T1(ljf);
        
        cout << dist << " " << ljdiff << endl;
    }
    
    cout << endl;
    
    for (int i=1000; i<100000; ++i)
    {
        T1 dist = getDistance<T1>(i);
    
        T1 cd = getCoul<T1>(i);
        T2 cf = getCoul<T2>(i);
        
        T1 cdiff = cd - T1(cf);
        
        cout << dist << " " << cdiff << endl;
    }
    
    cout << endl;
}

int main(void)
{
    test<double,float>();
    
    //test<long double, double>();
    
    return 0;
}
