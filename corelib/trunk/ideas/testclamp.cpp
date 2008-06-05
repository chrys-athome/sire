
#include <iostream>

using namespace std;

template<class T, int min, int max>
class Clamp
{
public:
    Clamp() : value()
    {}

    Clamp(const T &val) : value( std::min(double(max), std::max(val, double(min))) )
    {}

    Clamp(const Clamp<T,min,max> &other) : value(other.value)
    {}

    ~Clamp()
    {}

    operator T() const
    {
	return value;
    }

private:

    T value;
};

int main(void)
{

    Clamp<double,0,1> val;

    for (int i=-20; i<=20; ++i)
    {
       double v = i / 10.0;
       val = v;
       cout << "Value is " << val << " (" << v << ")\n"; 
    }

    return 0;
}
