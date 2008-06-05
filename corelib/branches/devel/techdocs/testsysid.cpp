
#include <unistd.h>
#include <iostream>

using namespace std;

int main(void)
{
    char buffer[128];

    cout << "PID == " << getpid() << endl;
    cout << "USERNAME == " << getlogin() << endl;

    cout << "PWD == " << getcwd(buffer,128) << endl;

    chroot("/tmp");

    cout << "PWD == " << getcwd(buffer,128) << endl;

    return 0;
}
