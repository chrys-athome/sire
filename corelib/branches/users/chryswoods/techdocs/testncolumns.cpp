
#include <iostream>

static int nColumns(int n)
{
    int ncol = 0;
    
    if (n < 0)
    {
        n = -n;
        ++ncol;
    }
    else if (n == 0)
        return 1;

    while (n > 0)
    {
        n /= 10;
        ++ncol;
    }
    
    return ncol;
}

int main(int argc, const char **argv)
{
    for (int i=-10; i<=1000; ++i)
    {
        std::cout << i << " : " << nColumns(i) << std::endl;
    }

    return 0;
}
