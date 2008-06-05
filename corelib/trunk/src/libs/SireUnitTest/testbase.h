#ifndef SIRETEST_TESTBASE_H
#define SIRETEST_TESTBASE_H

#include <QDebug>

#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;

#include "SireMaths/maths.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
This is the base class of all unit test classes.
 
@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestBase
{
public:
    TestBase();
    virtual ~TestBase();

    /** Initialise this test class and add it to the suite of tests */
    virtual void initialise(test_suite *suite)=0;
};

inline void initialise(TestBase *testclass, test_suite *suite)
{
    testclass->initialise(suite);
}

/** Return true if two numbers are equal to within a tolerance of 'epsilon'
    (normally should be 1x10-6 to 1x10-10. If 'printwarning' is true, then 
    print out a warning if the numbers are not equal. */
inline bool testEqual(double val0, double val1, double epsilon=1.0e-8)
{
    bool areequal = SireMaths::areEqual(val0,val1,epsilon);

    if ( not areequal )
        qDebug() << QObject::tr(
            "Two values are not equal: %1 and %2")
                .arg(val0).arg(val1);
                
    return areequal;
}

}

SIRE_END_HEADER

#endif
