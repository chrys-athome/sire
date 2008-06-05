
#include <QVariant>
#include <QList>

#include <iostream>

using namespace std;

int main(void)
{
    QList<double> list;
    
    list.append(1);
    list.append(2);

    QVariant variant = QVariant::fromValue(list);
    
    foreach (double val, variant.value< QList<double> >())
    {
        cout << val << endl;
    }

    return 0;
}
