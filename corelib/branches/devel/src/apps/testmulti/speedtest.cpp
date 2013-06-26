
#include "SireMaths/multifloat.h"
#include "SireMaths/multidouble.h"
#include "SireMaths/rangenerator.h"

#include <QTime>
#include <QVector>

#include <QDebug>

static const qint64 NTEST = 1000000;

using namespace SireMaths;

int main(int argc, const char **argv)
{
    QTime t;

    qDebug() << "Creating an array of" << NTEST << "random floating point numbers...";

    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        float *a = (float*)_mm_malloc( NTEST*sizeof(float), 32 );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        float *a = (float*)_mm_malloc( NTEST*sizeof(float), 16 );
    #else
        float *a = (float*)malloc(NTEST*sizeof(float));
    #endif
    #endif

    RanGenerator rand;

    qDebug() << "Generating the random numbers...";
    {
        t.start();

        for (int i=0; i<NTEST; ++i)
        {
            a[i] = rand.rand();
        }

        int ms = t.elapsed();
        qDebug() << "Generating the random numbers took" << ms << "ms";
    }

    qDebug() << "Converting to MultiFloat...";
    t.start();
    QVector<MultiFloat> mf_array = MultiFloat::fromArray(a, NTEST);
    qDebug() << "Took" << t.elapsed() << "ms";

    qDebug() << "Converting to MultiDouble...";
    t.start();
    QVector<MultiDouble> md_array = MultiDouble::fromArray(a, NTEST);
    qDebug() << "Took" << t.elapsed() << "ms";

    qDebug() << "Calculating all of the floating point square roots...";
    {
        t.start();
        const MultiFloat *a = mf_array.constData();

        MultiFloat sum;

        for (int j=0; j<1000; ++j)
        {
            for (int i=0; i<mf_array.count(); ++i)
            {
                sum += a[i].sqrt();
            }
        }

        int ms = t.elapsed();
        qDebug() << "Sum" << sum.toString();
        qDebug() << "took" << ms << "ms";
        float gflops = (1000.0 * NTEST * 2) / (1000000000 * 0.001 * ms);
        qDebug() << "Speed is" << gflops << "GFLOPS";
    }

    qDebug() << "Calculating all of the double precision square roots...";
    {
        t.start();
        const MultiDouble *a = md_array.constData();

        MultiDouble sum;

        for (int j=0; j<1000; ++j)
        {
            for (int i=0; i<mf_array.count(); ++i)
            { 
                sum += a[i].sqrt();
            }
        }

        int ms = t.elapsed();
        qDebug() << "Sum" << sum.toString();
        qDebug() << "took" << ms << "ms";
        float gflops = (1000.0 * NTEST * 2) / (1000000000 * 0.001 * ms);
        qDebug() << "Speed is" << gflops << "GFLOPS";
    }

    free(a);

    return 0;
}

