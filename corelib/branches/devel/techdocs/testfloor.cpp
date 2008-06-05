
#include <iostream>
#include <cmath>

using namespace std;

void test(double val)
{
    cout << "floor(" << val << ") = " << std::floor(val);
    cout << "  |  ceil(" << val << ") = " << std::ceil(val);
    cout << "  |  int(" << val << ") = " << int(val) << endl;
}

int main(void)
{

    for (int i=-20; i<=20; i++)
    {
        double v = i / 10.0;
        test(v);
    }

    return 0;
}

