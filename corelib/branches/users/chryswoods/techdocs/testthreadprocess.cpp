
#include <mpi.h>

#include <QProcess>
#include <QThread>

#include <cstdlib>

#include <QDebug>

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
        qDebug() << "Running 'ls'";

        if (std::system(0))
        {
            std::system("setenv TEXT=\"USER\"; cd /tmp; echo $TEXT | molpro > molpro.tmp.out");

            std::system("setenv USER=\"me\"; cd /tmp; cat molpro.tmp.out");
        }
        else
            qDebug() << "std::system() interpreter is not available";

        qDebug() << "ls process has finished";
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
