
#include <QHash>
#include <QString>
#include <QList>

#include <QDebug>

template<class Iter>
quint32 getID(Iter it)
{
    const void *key = &(*it);

    qDebug() << key;
 
    return 42;
}

template<class Iter>
quint32 getID(Iter begin, Iter end)
{
    if (begin == end)
    {
        qDebug() << "Container is empty";
        return 0;
    }
    else return getID(begin);
}

template<class T>
quint32 getContainerID(const T &container)
{
    return getID( container.constBegin(), container.constEnd() );
}

template<class T>
quint32 getStringID(const T *string)
{
    if (string == 0)
    {
        qDebug() << "The string is empty";
        return 0;
    }
    else
        return getID(string);
}

int main(int argc, const char **argv)
{
    qDebug() << "hashes";

    QHash<QString,QString> dict;

    getContainerID(dict);

    dict.insert( "cat", "mieow" );
    dict.insert( "dog", "woof" );

    QHash<QString,QString> dict2 = dict;

    getContainerID(dict);
    getContainerID(dict2);

    dict.insert( "cow", "moo" );

    getContainerID(dict);
    getContainerID(dict2);

    qDebug() << "\nSTRINGS";

    QString cow;

    getStringID(cow.constData());

    cow = "cow";

    getStringID(cow.constData());

    QString cow2 = cow;

    getStringID(cow2.constData());

    qDebug() << "\nLISTS";

    QList<QString> list;
    list.append( "cat" );
    list.append( "dog" );

    QList<QString> list2 = list;

    getContainerID(list);
    getContainerID(list2);

    list.append( "cow" );

    getContainerID(list);
    getContainerID(list2);

    return 0;
}
