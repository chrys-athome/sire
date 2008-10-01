
#include <QProcess>
#include <QThread>

#include <QDebug>

#include <mpi.h>

class ProcessThread : public QThread
{
public:
    ProcessThread() : QThread()
    {}

    ~ProcessThread()
    {}

protected:
    void run()
    {
        QProcess p;

        qDebug() << "ls";
        p.start("ls");

        qDebug() << "p.waitForFinished();";
        p.waitForFinished();

        qDebug() << p.readAll();
    }
};

class MPIThread : public QThread
{
public:
    MPIThread() : QThread()
    {}

    ~MPIThread()
    {}

protected:
    void run()
    {
        int message;
        MPI::COMM_WORLD.Recv(&message, 1, MPI::INT, MPI_ANY_SOURCE, 1);

        qDebug() << "Message" << message;
    }
};

int main(int argc, char **argv)
{
    MPI::Init(argc, argv);

    MPIThread mpi;
    mpi.start();

    //give the mpi thread time to start
    mpi.wait(1000);

    ProcessThread pt;
    pt.start();

    pt.wait();
    mpi.terminate();

    return 0;
}
