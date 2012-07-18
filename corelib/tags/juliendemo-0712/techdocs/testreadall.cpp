
#include <QFile>
#include <QByteArray>
#include <QTime>
#include <QDebug>

int main(int argc, const char **argv)
{
    if (argc > 1)
    {
        QFile f(argv[1]);

        if (not f.open(QIODevice::ReadOnly|QIODevice::Unbuffered))
        {
            qWarning() << "Could not read file" << argv[1];
            return -1;
        }

        QTime t;
        t.start();

        QByteArray data = f.readAll();

        int ms = t.elapsed();

        qDebug() << "Reading" << data.count() << "bytes took" << ms << "ms";
    }

    return 0;
}
