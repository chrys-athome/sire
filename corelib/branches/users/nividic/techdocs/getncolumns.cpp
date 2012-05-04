
#include <cmath>

#include <iostream>

#include <QString>

using namespace std;

static int nColumns(int n)
{
    if (n <= 0)
        return 0;
    else
        return int( log10(n) ) + 1;
}

static QString getFrameNumber(int i, int n)
{
    if (n == 1)
        return "";
    
    else
    {
        return QString("%1").arg( i, n, int(10), QLatin1Char('0'));
    }
}

int main(int argc, const char **argv)
{
    int n = nColumns(1000);

    for (int i=0; i<=1000; ++i)
    {
        cout << "ncolumns(" << qPrintable(getFrameNumber(i,n)) << ") == " << nColumns(i) << endl;
    }

    return 0;
}
