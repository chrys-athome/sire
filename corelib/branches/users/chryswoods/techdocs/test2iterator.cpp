
#include <QList>
#include <QMutableListIterator>

#include <iostream>

using namespace std;

void printList(const QList<int> &vals)
{
    cout << "[ ";

    for (int i=0; i<vals.count(); ++i)
    {
        cout << vals.at(i) << " ";
    }

    cout << "]\n";
}

int main(int argc, const char **argv)
{
    QList<int> vals;

    for (int i=0; i<5; ++i)
    {
        for (int j=0; j<5; ++j)
        {
           vals.append(j);
        }
    }

    printList(vals);
 
    int nvals = vals.count();

    int i = 0;
    while (i < nvals-1)
    {
        const int *val0 = &(vals.at(i));

        int j = i+1;

        while (j<nvals)
        {
            cout << i << " " << j << " ";

            const int &val1 = vals.at(j);

            cout << "[" << val0 << "," << val1 << "] ";

            if (*val0 == val1)
            {
                cout << "REMOVED!\n";
                vals.removeAt(j);
                --nvals;

                val0 = &(vals.at(i));
            }
            else
            {
                ++j;
                cout << "KEPT!\n";
            }
        }

        ++i;
    }

    printList(vals);

    return 0;
}