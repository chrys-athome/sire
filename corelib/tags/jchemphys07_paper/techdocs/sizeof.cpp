
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace std;

int main(void)
{
    cout << "Size of int = " << sizeof(int) << " bytes\n";
    cout << "Size of int* = " << sizeof(int*) << " bytes\n";
    cout << "Size of void* = " << sizeof(void*) << " bytes\n";

    cout << "Size of long = " << sizeof(long) << " bytes\n";
    cout << "Size of long long = " << sizeof(long long) << " bytes\n";

    cout << "Size of short = " << sizeof(short) << " bytes\n";
    cout << "Size of char = " << sizeof(char) << " bytes\n";

    cout << "Size of float = " << sizeof(float) << " bytes\n";
    cout << "Size of double = " << sizeof(double) << " bytes\n";
    cout << "Size of long double = " << sizeof(long double) << " bytes\n";

    cout << "Size of a shared_ptr<float> = " << sizeof(boost::shared_ptr<float>) << " bytes\n";
    cout << "Size of a weak_ptr<float> = " << sizeof(boost::weak_ptr<float>) << " bytes\n";
    
    cout << "Size of a detail::shared_count = " << sizeof(boost::detail::shared_count) << " bytes\n";
    cout << "Size of a detail::weak_count = " << sizeof(boost::detail::weak_count) << " bytes\n";

    return 0;
}
