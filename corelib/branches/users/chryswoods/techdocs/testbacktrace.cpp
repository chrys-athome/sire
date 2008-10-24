#include <execinfo.h>
#include <stdio.h>
 
void getBackTrace()
{
    void* callstack[128];
    int i, frames = backtrace(callstack, 128);
    char** strs = backtrace_symbols(callstack, frames);
    
    for (i = 0; i < frames; ++i) 
    {
        printf("%s\n", strs[i]);
    }
    
    delete[] strs;
}

class exception
{
public:
    exception()
    {
        getBackTrace();
    }

    ~exception()
    {}
};

void bar()
{
    throw exception();
}

void foo()
{
    bar();
}

int main(int argc, const char **argv)
{
    foo();

    return 0;
}

