
#include <QRegExp>
#include <QString>
#include <QDebug>

static QRegExp vecregexp("([0-9.-]+),\\s{0,}([0-9.-]+),\\s{0,}([0-9.-]+)");

int main(void)
{
        QString s = "(1.0,2.0,3.0)";

        if (vecregexp.indexIn(s) != -1)
        {
            qDebug() << "Found a vector!";
        }
        else
        {
            qDebug() << "Problem with regexp!";
        }
        
        //exiting now causes a segfault
        return 0;
}
