#ifndef SIREBASE_INCREMENT_H
#define SIREBASE_INCREMENT_H

#include <QString>
#include <QRegExp>

namespace SireBase
{

/** A small function that increments a name, e.g. the name foobar_1 will be incremented
    to foobar_2, or foobar will be incremented to foobar_1 */
inline QString increment(QString name)
{
    QRegExp nummatch("_(\\d+)$");
    
    int pos = nummatch.indexIn(name);
    if (pos != -1)
    {
        QString numstring = nummatch.cap(1);
        int lgth = numstring.size();
        int num = numstring.toInt();
        name.remove(pos,name.length());
        num++;
        return QString("%1_%2").arg(name).arg(QString::number(num),lgth,'0');
    }
    else
      return QString("%1_1").arg(name);
}

}

#endif
