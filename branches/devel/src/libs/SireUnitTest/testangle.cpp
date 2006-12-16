
#include <QDebug>

#include "testangle.h"

#include "SireMaths/angle.h"

#include "SireError/errors.h"
#include "SireUnits/units.h"

using namespace SireTest;
using namespace SireMaths;
using namespace SireUnits;

using SireMaths::pi;

TestAngle::TestAngle() : TestBase()
{}

TestAngle::~TestAngle()
{}

void TestAngle::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestAngle::runTests ) );
}

double testCalcAngle(double x, double y)
{
    if (y == 0.0)
    {
        if (x == 0.0)
            return 0.0;
        else if (x > 0.0)
            return 90.0;
        else
            return -90.0;
    }
    else
    {
        double rads = std::atan(x/y);

        if (x < 0.0 and y < 0.0)
            rads -= pi;
        else if (x >= 0.0 and y <= 0.0)
            rads += pi;

        return rads / degrees;
    }
}

void TestAngle::runTests()
{
    qDebug() << QObject::tr("Running SireMaths::Angle Unit Tests...");

    Angle ang(0.0,0.0);

    //this should be a zero angle
    BOOST_CHECK( ang.isZero() );

    //now go around the unit circle. The angle system should use 0 degrees is
    //y-axis (12 o'clock), with positive angles being clockwise, and negative
    //angles being anticlockwise.

    for (int i=10; i>=-10; --i)
    {
        //clamp y between 0 and 1
        double y = 0.1 * i;

        //calculate y on the circle
        double x = std::sqrt( qMax(0.0, 1.0 - y*y) );

        Angle cang(x,y);

        //calculate the angle
        double degs = testCalcAngle(x,y);
        BOOST_CHECK_CLOSE( cang.toDegrees(), degs, 1e-6 );

        qDebug() << QObject::tr("(%1,%2) = %3 degrees").arg(x).arg(y).arg(cang.toDegrees());
    }

    //do the other half of the triangle
    for (int i=-10; i<=10; ++i)
    {
        double y = 0.1 * i;
        double x = -( std::sqrt( qMax(0.0, 1.0-y*y) ) );

        Angle cang(x,y);

        //calculate the angle
        double degs = testCalcAngle(x,y);
        BOOST_CHECK_CLOSE(  cang.toDegrees(), degs, 1e-6 );

        qDebug() << QObject::tr("(%1,%2) = %3 degrees").arg(x).arg(y).arg(cang.toDegrees());
    }

    BOOST_CHECK_CLOSE(  Angle::degrees(35.4343).toDegrees(), 35.4343, 1e-6 );

    ang = Angle::degrees(45.0);
    BOOST_CHECK_CLOSE(  ang.small().toDegrees(), Angle::degrees(45.0).toDegrees(), 1e-6 );
    BOOST_CHECK_CLOSE(  ang.large().toDegrees(), Angle::degrees(-315.0).toDegrees(), 1e-6 );
    BOOST_CHECK_CLOSE(  ang.positive().toDegrees(), Angle::degrees(45.0).toDegrees(), 1e-6 );
    BOOST_CHECK_CLOSE(  ang.negative().toDegrees(), Angle::degrees(-315.0).toDegrees(), 1e-6 );

    qDebug() << QObject::tr("Finished SireMaths::Angle tests...");
}
