
#include <iostream>

using namespace std;

namespace test
{

class Streamer
{
public:
    Streamer(){}
    ~Streamer(){}

    template<class T>
    Streamer& operator<<(const T &value)
    {
        cout << "USING TEMPLATE VERSION TO STREAM OBJECT" << endl;
        return *this;
    }
};

}

class MyVal
{
public:
    MyVal(){}
    ~MyVal(){}
};

test::Streamer& operator<<(test::Streamer &ds, const MyVal &value)
{
    cout << "USING SPECIFIC FUNCTION TO STREAM MYVAL\n";
    return ds;
}

int main(int argc, const char **argv)
{
    test::Streamer ds;

    ds << 42 << MyVal();


    return 0;
}
