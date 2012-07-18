
#include <mpi.h>

#include <QThread>
#include <QDebug>

class Foo : public QThread
{
public:
    Foo() : QThread()
    {}

    ~Foo()
    {}

protected:
    void run()
    {
        while (true)
        {
            int result;
            qDebug() << "Waiting for a result...";
            MPI::COMM_WORLD.Recv(&result, 1, MPI::INT, 0, 0);

            qDebug() << "Got the result" << result;
  
            if (result == 0)
                break;
        }
    }

};

int main(int argc, char **argv)
{
    MPI::Init(argc, argv);

    Foo foo;
    foo.start();

    for (int i=0; i<argc; ++i)
    {
        int value = std::atoi(argv[i]);

        if (value != 0)
        {
            qDebug() << "Sending" << value;
            MPI::COMM_WORLD.Send(&value, 1, MPI::INT, 0, 0);
            qDebug() << "Value has been sent!";
        }
    }

    //shutdown the background thread
    int value = 0;
    MPI::COMM_WORLD.Send(&value, 1, MPI::INT, 0, 0);
    foo.wait();

    MPI::Finalize();

    return 0;
}
