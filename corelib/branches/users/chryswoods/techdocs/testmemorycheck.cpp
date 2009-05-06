
#include <malloc/malloc.h>

#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
    struct mstats minfo = mstats();

    char *array = new char[10*1024*1024];

    for (int i=0; i<10*1024*1024; ++i)
    {
        array[i] = 0;
    }

    cout << minfo.bytes_total << " " << minfo.bytes_used << " " << minfo.bytes_free << endl;

    minfo = mstats();

    cout << minfo.bytes_total << " " << minfo.bytes_used << " " << minfo.bytes_free << endl;

    return 0;
}
