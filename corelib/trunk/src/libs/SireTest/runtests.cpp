
#include "SireCAS/qhash_sirecas.h"

#include "runtests.h"

#include <QObject>
#include <QTime>

#include <QDebug>

#include "SireCAS/trigfuncs.h"
#include "SireCAS/values.h"

using namespace SireTest;
using namespace SireMaths;
using namespace SireCAS;

RunTests::RunTests()
{}

RunTests::~RunTests()
{}

void RunTests::run()
{
    qDebug() << QObject::tr("Running personal tests...");

    qDebug() << QObject::tr("Time tests for SireMaths expressions");
    
    QTime t;
    
    //now test with SireCAS
    Symbol x("x");
    Expression ex = (1.0 + Cos(x)) + 0.5*(1 + Cos(2*x))
                       - 0.25*(1 + Cos(3*x));
    
    qDebug() << ex.toString();
    qDebug() << ex.differentiate(x).toString();

    double a = 0.0;
    
    t.start();
    Values vals;
    
    for (int i=0; i<1000000; ++i)
    {
        vals.set(x, 0.01*i);
        a += ex( vals );
    }

    int ms = t.elapsed();
    
    qDebug() << QObject::tr("a = %1, ms = %2").arg(a).arg(ms);
}

