
#include <iostream>

using namespace std;

int multiply(int a, int b)
{
    return a * b;
}

int main(int argc, const char **argv)
{
    const int &r = multiply(2, 3);
    const int &s = multiply(5, 10);

    cout << "&r == " << &r << " &s == " << &s << endl;

    cout << "r == " << r << " s == " << s << endl;

    cout << "Should be;" << endl << "r == 6 s == 50" << endl;

    return 0;
}

