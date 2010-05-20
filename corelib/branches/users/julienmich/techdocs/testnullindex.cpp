
#include <QtGlobal>

#include <iostream>
#include <limits>

using namespace std;

int main(void)
{
	qint32 i;

	cout << sizeof(i) << endl;

	i = -(std::numeric_limits<qint32>::max());

	cout << i << endl;
        cout << std::numeric_limits<qint32>::min() << endl;

	return 0;
}
