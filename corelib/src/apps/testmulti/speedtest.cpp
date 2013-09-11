
#include "SireMaths/multifloat.h"
#include "SireMaths/multidouble.h"
#include "SireMaths/rangenerator.h"

#include <QTime>
#include <QVector>

#include <QDebug>

static const qint64 NTEST = 25000;

using namespace SireMaths;

int main(int argc, const char **argv)
{
    QTime t;

    qDebug() << "Calculating the coulomb energy between two groups of"
             << NTEST << "particles. This involves calculating and summing "
             << (NTEST*NTEST) << "coulomb interaction energies.";

    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        float *x0a = (float*)_mm_malloc( NTEST*sizeof(float), 32 );
        float *y0a = (float*)_mm_malloc( NTEST*sizeof(float), 32 );
        float *z0a = (float*)_mm_malloc( NTEST*sizeof(float), 32 );
        float *x1a = (float*)_mm_malloc( NTEST*sizeof(float), 32 );
        float *y1a = (float*)_mm_malloc( NTEST*sizeof(float), 32 );
        float *z1a = (float*)_mm_malloc( NTEST*sizeof(float), 32 );

        float *q0a = (float*)_mm_malloc( NTEST*sizeof(float), 32 );
        float *q1a = (float*)_mm_malloc( NTEST*sizeof(float), 32 );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        float *x0a = (float*)_mm_malloc( NTEST*sizeof(float), 16 );
        float *y0a = (float*)_mm_malloc( NTEST*sizeof(float), 16 );
        float *z0a = (float*)_mm_malloc( NTEST*sizeof(float), 16 );
        float *x1a = (float*)_mm_malloc( NTEST*sizeof(float), 16 );
        float *y1a = (float*)_mm_malloc( NTEST*sizeof(float), 16 );
        float *z1a = (float*)_mm_malloc( NTEST*sizeof(float), 16 );

        float *q0a = (float*)_mm_malloc( NTEST*sizeof(float), 16 );
        float *q1a = (float*)_mm_malloc( NTEST*sizeof(float), 16 );
    #else
        float *x0a = (float*)malloc( NTEST*sizeof(float) );
        float *y0a = (float*)malloc( NTEST*sizeof(float) );
        float *z0a = (float*)malloc( NTEST*sizeof(float) );
        float *x1a = (float*)malloc( NTEST*sizeof(float) );
        float *y1a = (float*)malloc( NTEST*sizeof(float) );
        float *z1a = (float*)malloc( NTEST*sizeof(float) );

        float *q0a = (float*)malloc( NTEST*sizeof(float) );
        float *q1a = (float*)malloc( NTEST*sizeof(float) );
    #endif
    #endif

    RanGenerator rand;
    rand.seed(1234561);

    qDebug() << "Generating the random numbers...";
    {
        t.start();

        for (int i=0; i<NTEST; ++i)
        {
            x0a[i] = rand.rand(-5,-3);
            y0a[i] = rand.rand(-5,-3);
            z0a[i] = rand.rand(-5,-3);
            q0a[i] = rand.rand(-1, 1);

            x1a[i] = rand.rand(3,5);
            y1a[i] = rand.rand(3,5);
            z1a[i] = rand.rand(3,5);
            q1a[i] = rand.rand(1,1);
        }

        int ms = t.elapsed();
        qDebug() << "Generating the random numbers took" << ms << "ms";
    }

    qDebug() << "Converting to MultiFloat...";
    t.start();
    QVector<MultiFloat> x0f = MultiFloat::fromArray(x0a, NTEST);
    QVector<MultiFloat> y0f = MultiFloat::fromArray(y0a, NTEST);
    QVector<MultiFloat> z0f = MultiFloat::fromArray(z0a, NTEST);
    QVector<MultiFloat> q0f = MultiFloat::fromArray(q0a, NTEST);

    QVector<MultiFloat> x1f = MultiFloat::fromArray(x1a, NTEST);
    QVector<MultiFloat> y1f = MultiFloat::fromArray(y1a, NTEST);
    QVector<MultiFloat> z1f = MultiFloat::fromArray(z1a, NTEST);
    QVector<MultiFloat> q1f = MultiFloat::fromArray(q1a, NTEST);
    qDebug() << "Took" << t.elapsed() << "ms";

    qDebug() << "Converting to MultiDouble...";
    t.start();
    QVector<MultiDouble> x0d = MultiDouble::fromArray(x0a, NTEST);
    QVector<MultiDouble> y0d = MultiDouble::fromArray(y0a, NTEST);
    QVector<MultiDouble> z0d = MultiDouble::fromArray(z0a, NTEST);
    QVector<MultiDouble> q0d = MultiDouble::fromArray(q0a, NTEST);

    QVector<MultiDouble> x1d = MultiDouble::fromArray(x1a, NTEST);
    QVector<MultiDouble> y1d = MultiDouble::fromArray(y1a, NTEST);
    QVector<MultiDouble> z1d = MultiDouble::fromArray(z1a, NTEST);
    QVector<MultiDouble> q1d = MultiDouble::fromArray(q1a, NTEST);
    qDebug() << "Took" << t.elapsed() << "ms";

    qDebug() << "Calculating the coulomb energy using floats...";
    {
        t.start();
        const MultiFloat *x0 = x0f.constData();
        const MultiFloat *y0 = y0f.constData();
        const MultiFloat *z0 = z0f.constData();
        const MultiFloat *q0 = q0f.constData();

        const MultiFloat *x1 = x1f.constData();
        const MultiFloat *y1 = y1f.constData();
        const MultiFloat *z1 = z1f.constData();
        const MultiFloat *q1 = q1f.constData();

        MultiDouble coul_nrg(0);

        // it is very important that there are no constructors
        // or destructors called in the inner loop!
        MultiFloat delta;
        MultiFloat dist2;

        MultiFloat x, y, z, q, ox, oy, oz, oq;
        MultiDouble cnrg;

        for (int i=0; i<x0f.count(); ++i)
        {
            x = x0[i];
            y = y0[i];
            z = z0[i];
            q = q0[i];

            for (int j=0; j<x1f.count(); ++j)
            {
                ox = x1[j];
                oy = y1[j];
                oz = z1[j];
                oq = q1[j];

                for (int k=0; k<MultiFloat::count(); ++k)
                {
                    delta = x - ox;
                    dist2 = delta*delta;
                    delta = y - oy;
                    dist2.multiplyAdd(delta, delta);
                    delta = z - oz;
                    dist2.multiplyAdd(delta, delta);

                    cnrg = q * oq * dist2.rsqrt();                
                    coul_nrg += cnrg;

                    //rotate the multifloat to process the other distances
                    ox = ox.rotate();
                    oy = oy.rotate();
                    oz = oz.rotate();
                    oq = oq.rotate();
                }
            }
        }

        int ms = t.elapsed();
        qDebug() << "Energies" << coul_nrg.toString() << "TOTAL" << coul_nrg.doubleSum();
        qDebug() << "took" << ms << "ms";
        float gflops = (NTEST * NTEST * 12) / (1000000000 * 0.001 * ms);
        qDebug() << "Speed is" << gflops << "GFLOPS";
    }

    qDebug() << "Calculating the coulomb energy using doubles...";
    {
        t.start();
        const MultiDouble *x0 = x0d.constData();
        const MultiDouble *y0 = y0d.constData();
        const MultiDouble *z0 = z0d.constData();
        const MultiDouble *q0 = q0d.constData();

        const MultiDouble *x1 = x1d.constData();
        const MultiDouble *y1 = y1d.constData();
        const MultiDouble *z1 = z1d.constData();
        const MultiDouble *q1 = q1d.constData();

        MultiDouble coul_nrg(0);

        // it is very important that there are no constructors
        // or destructors called in the inner loop!
        MultiDouble delta;
        MultiDouble dist2;

        MultiDouble x, y, z, q, ox, oy, oz, oq;

        for (int i=0; i<x0d.count(); ++i)
        {
            x = x0[i];
            y = y0[i];
            z = z0[i];
            q = q0[i];

            for (int j=0; j<x1d.count(); ++j)
            {
                ox = x1[j];
                oy = y1[j];
                oz = z1[j];
                oq = q1[j];

                for (int k=0; k<MultiDouble::count(); ++k)
                {
                    delta = x - ox;
                    dist2 = delta*delta;
                    delta = y - oy;
                    dist2.multiplyAdd(delta, delta);
                    delta = z - oz;
                    dist2.multiplyAdd(delta, delta);

                    coul_nrg += q * oq * dist2.rsqrt();                

                    //rotate the multidouble to process the other distances
                    //(these rotates kill my performance as I haven't worked out yet
                    // how to use SSE/AVX to speed them up...!)
                    ox = ox.rotate();
                    oy = oy.rotate();
                    oz = oz.rotate();
                    oq = oq.rotate();
                }
            }
        }

        int ms = t.elapsed();
        qDebug() << "Energies" << coul_nrg.toString() << "TOTAL" << coul_nrg.sum();
        qDebug() << "took" << ms << "ms";
        float gflops = (NTEST * NTEST * 12) / (1000000000 * 0.001 * ms);
        qDebug() << "Speed is" << gflops << "GFLOPS";
    }

    free(x0a);
    free(x1a);
    free(y0a);
    free(y1a);
    free(z0a);
    free(z1a);

    return 0;
}

