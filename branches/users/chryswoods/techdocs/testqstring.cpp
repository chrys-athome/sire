
#include <QString>
#include <QStringList>

#include <iostream>

using namespace std;

int main(void)
{
	QStringList columns;

	for (int i=0; i<4; ++i)
		columns.append(QString("ResName%1, ResNum%1, AtomName%1, AtomNum%1")
							.arg(i));
							
	cout << columns.join(", ").toAscii().constData() << endl;

	return 0;
}
