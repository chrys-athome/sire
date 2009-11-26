
#include <iostream>
#include <boost/shared_ptr.hpp>

using namespace std;
using boost::shared_ptr;

class TestShutdown
{
public:
    TestShutdown()
    {
        cout << "Executing on system startup.\n";
    }

    ~TestShutdown()
    {
        cout << "Executing on system shutdown.\n";
    }
};

static shared_ptr<TestShutdown> global_ptr( new TestShutdown() );

int main(int argc, const char **argv)
{
    cout << "Start of main...\n";


    cout << "End of main...\n";

    return 0;
}
