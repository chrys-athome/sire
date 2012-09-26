
#include <QThread>

#include <QDebug>

#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_num_threads() 1
#endif

class OpenMPWorker : public QThread
{
public:
    OpenMPWorker() : QThread()
    {}

    ~OpenMPWorker()
    {}

protected:
    void run();
};

void OpenMPWorker::run()
{
    qDebug() << "Before parallel...";

    qDebug() << "Initialising the team!";

    qDebug() << omp_get_num_threads();

    #pragma omp parallel
    {
        qDebug() << "In parallel!";

        qDebug() << omp_get_num_threads();

        #pragma omp for
        for (int i=0; i<10; ++i)
        {
            qDebug() << i;
        }
    }
}

int main(int argc, const char **argv)
{
    OpenMPWorker worker;

    worker.start();

    worker.wait();

    return 0;
}
